#define RS_K 6
#define RS_M 62
#define RS_W 4
#define RS_S 1

#define K RS_K                 // Number of data symbols
#define M (RS_M + RS_K - 1)    // Number of coding devices
#define W (8 * RS_W)                   // The width of a symbol (in bits)

#define ADVERT_SIZE 31    // Number of bytes in an advertisment
#define N_DATA_SYMBOLS K  // Number of 32 byte symbols in a ECC Key
#define N_SYMBOLS (K+M)      // Number of total symbols for one ECC Key
                          //  (Including the ones which end up in the overlapped section)
#define N_OVERLAP 5       // Size of the overlap sections at the beggining / end of an
                          //  epoch
                          
#define EPOCH_LENGTH (N_SYMBOLS - N_OVERLAP) // Number of adverts in an epoch

#include "includes/random.h"
#include "includes/mem.h"
#include "includes/ecc.h"
#include "includes/EBN.h"
#include "includes/reed_sol.h"
#include "includes/jerasure.h"
#include "includes/bget.h"

// TODO : Define Log and Debug

// ***** STATIC DATA *****


    // ** State Data **
static uint32_t old_private[N_DATA_SYMBOLS],current_private[N_DATA_SYMBOLS];
static EccPoint old_point, current_point;
static uint8_t old_y,current_y;
static struct EBN_Mac_Address mac_addr;
static struct EBN_Epoch_ID old_id, current_id;
static uint32_t old_syms[N_SYMBOLS];
static uint32_t current_syms[N_SYMBOLS];
static struct EBN_Advert current_adverts[EPOCH_LENGTH];

static uint8_t messageNum; 

    // ** Coding Data **

int * rs_coding_matrix;
uint32_t *old_sym_ptrs[N_SYMBOLS]; // pointers into the old_syms
uint32_t *curr_sym_ptrs[N_SYMBOLS];// pointers into current_syms
uint32_t **old_data_ptr;    // pointers into *_sym_ptrs so jerasure
uint32_t **curr_data_ptr;   //  is sated.
uint32_t **old_code_ptr;
uint32_t **curr_code_ptr;

// ***** PRIVATE FUNCTIONS *****

void Generate_New_Key(){
    
    // Copy over previous epoch's data
    memcpy(&old_point,&current_point,sizeof(EccPoint));
    memcpy(old_private,current_private,N_DATA_SYMBOLS * sizeof(uint32_t));
    old_y = current_y;

    // Generate new epoch's puclig and private keys
    uint32_t entropy[N_DATA_SYMBOLS];
    int rc = 0;
    while(!rc) {
        Fill_Random_Bytes((uint8_t *) &entropy,N_DATA_SYMBOLS * sizeof(uint32_t));
        rc = ecc_make_key(&current_point,current_private,entropy);
    }

    // Generate new Y value
    current_y = (uint8_t) ecc_compress_y_coord(&current_point);
}

void Insert_Checksum(struct EBN_Epoch_ID * epoch){
    uint8_t checksum = 0,i;

    // calculate sum of all the nibbles (except the checksum)
    //  to use as a checksum
    for(i = 0; i < sizeof(struct EBN_Epoch_ID) - 1;i++){
        checksum += (epoch->bytes[i] >> 4) + (epoch->bytes[i] & 0x0F);
    }
    checksum += epoch->bytes[sizeof(struct EBN_Epoch_ID)] >> 4;
    epoch->checksum = checksum;
}

void Generate_New_Epoch_ID() {
    
    // Copy over the previous epoch's data
    memcpy(&old_id,&current_id,sizeof(struct EBN_Epoch_ID));

    // fill with randomness
    Fill_Random_Bytes(current_id.bytes,sizeof(struct EBN_Epoch_ID));

    // Set Y coord
    current_id.y_coord = current_y;

    Insert_Checksum(&current_id); 
}

void Generate_New_Mac_Address(){
    memcpy(&mac_addr.previous,&old_id,sizeof(struct EBN_Epoch_ID));
    memcpy(&mac_addr.current,&current_id,sizeof(struct EBN_Epoch_ID));
}

void Generate_New_Symbols(){

    // copy old symbol data
    memcpy(old_syms,current_syms,N_SYMBOLS * RS_W);
    
    // copy new symbol data into current symbol table
    memcpy(current_syms,current_point.x,sizeof(current_point.x));

    // Generate coding symbol    
    jerasure_matrix_encode(K,M,W,rs_coding_matrix,
            (char **) curr_data_ptr,(char **) curr_code_ptr,
            sizeof(long));

}

void Generate_Epoch_Adverts(){
    int i;
    for(i = 0; i < N_SYMBOLS;i++){
        memset(&current_adverts[i],'\0',sizeof(struct EBN_Advert));
        current_adverts[i].node_type = 1;
        current_adverts[i].message_num = i;
        current_adverts[i].dh_symbol = current_syms[i];
        if(i < N_OVERLAP){
            current_adverts[i].prev_dh_symbol = 
                old_syms[N_SYMBOLS - N_OVERLAP + i];
        }
    }
}

void Epoch_Change(){
    Generate_New_Key();
    Generate_New_Epoch_ID();
    Generate_New_Mac_Address();
    Generate_New_Symbols();
    Generate_Epoch_Adverts();
}

void Generate_RS_Pointers(){

    int i;
    for(i = 0; i < N_SYMBOLS;i++){
        old_sym_ptrs[i] = &old_syms[i];
        curr_sym_ptrs[i] = &current_syms[i];
    }

    old_data_ptr = old_sym_ptrs;
    curr_data_ptr = curr_sym_ptrs;
    old_code_ptr = &old_sym_ptrs[N_DATA_SYMBOLS];
    curr_code_ptr = &curr_sym_ptrs[N_DATA_SYMBOLS];
}

void Init_RS() {
    rs_coding_matrix = reed_sol_vandermonde_coding_matrix(K, M, W);
    Generate_RS_Pointers();
}

// ***** PUBLIC FUNCTIONS *****

void EBN_Init(){
    Random_Init();
    Init_RS();
    Epoch_Change();
   Epoch_Change();
    messageNum = 0;
}

int EBN_Increment_Time(){

    messageNum++;

    if(messageNum >= EPOCH_LENGTH){

        Epoch_Change();
        messageNum = 0;

        return 1;
    }

    return 0;
}

void * EBN_Get_Advert(){
    return &current_adverts[messageNum];
}

void * EBN_Get_Mac_Address(){
    return &mac_addr;
}


