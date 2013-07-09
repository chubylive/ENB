#ifndef EBN_H
#define EBN_H

#include <stdint.h>

#pragma pack(push,1)
struct EBN_Epoch_ID{
    union {
        struct {
            unsigned int checksum : 4 ;
            unsigned int random   : 19;
            unsigned int y_coord  : 1 ;
        };
        uint8_t bytes[3];
    };
};

struct EBN_Mac_Address{
    union {
        struct {
            struct EBN_Epoch_ID previous;
            struct EBN_Epoch_ID current;
        };
        uint8_t byte[6];
    };
};

struct EBN_Advert{
    union {
        struct {
            union { // Access header data as a single byte
                //   with the proper bit field. 
                struct {
                    unsigned int node_type : 1;
                    unsigned int message_num : 7;
                };
                uint8_t header;
            };
            uint32_t dh_symbol;
            union { // Treat the payload as either a full
                    //   chunk of data, or as some of the 
                    //   previous DH handshake and a 
                    //   smaller data payload. 
                struct {
                    uint32_t prev_dh_symbol;
                    uint8_t small_data[22];
                };
                uint8_t data[26];
            };
        };
        uint8_t bytes[31];
    };
};
#pragma pack(pop)

void EBN_Init();
void Epoch_Change();

#endif
