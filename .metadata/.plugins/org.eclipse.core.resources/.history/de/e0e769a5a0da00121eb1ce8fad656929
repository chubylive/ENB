/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BT_Stack/includes/config.h"
#include "BT_Stack/includes/btstack_memory.h"
#include "BT_Stack/includes/hci.h"
#include "BT_Stack/includes/hci_dump.h"
#include "BT_Stack/includes/run_loop.h"
#include "BT_Stack/includes/l2cap.h"
#include "BT_Stack/includes/att.h"

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include <stdio.h>

// TODO: insert other definitions and declarations here
static att_connection_t att_connection;
static uint16_t         att_response_handle = 0;
static uint16_t         att_response_size   = 0;
static uint8_t          att_response_buffer[28];

static void att_try_respond(void){
    if (!att_response_size) return;
    if (!att_response_handle) return;
    if (!hci_can_send_packet_now(HCI_ACL_DATA_PACKET)) return;

    // update state before sending packet
    uint16_t size = att_response_size;
    att_response_size = 0;
    l2cap_send_connectionless(att_response_handle, L2CAP_CID_ATTRIBUTE_PROTOCOL, att_response_buffer, size);
}


static void att_packet_handler(uint8_t packet_type, uint16_t handle, uint8_t *packet, uint16_t size){
    if (packet_type != ATT_DATA_PACKET) return;

    att_response_handle = handle;
    att_response_size = att_handle_request(&att_connection, packet, size, att_response_buffer);
    att_try_respond();
}


// enable LE, setup ADV data
static void packet_handler ( uint8_t packet_type, uint8_t *packet, uint16_t size){
    bd_addr_t addr;
    uint8_t adv_data[] = { 02, 01, 05, 03, 02, 0xc0, 0x0l ,0x1e, 0xe7};
    switch (packet_type) {

		case HCI_EVENT_PACKET:
			switch (packet[0]) {

                case BTSTACK_EVENT_STATE:
					// bt stack activated, get started - set local name
					if (packet[2] == HCI_STATE_WORKING) {
					   printf("Working!\n");
						hci_send_cmd(&hci_read_local_supported_features);
					}
					break;
/*
                case DAEMON_EVENT_HCI_PACKET_SENT:
                    att_try_respond();
                    break;*/

                case HCI_EVENT_LE_META:
                    switch (packet[2]) {
                        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
                            // reset connection MTU
                            att_connection.mtu = 23;
                            break;
                        default:
                            break;
                    }
                    break;

                case BTSTACK_EVENT_NR_CONNECTIONS_CHANGED:
				    if (packet[2]) {
                       printf("CONNECTED");
                    } else {
				        printf("NOT CONNECTED");
                    }
                    break;

                case HCI_EVENT_DISCONNECTION_COMPLETE:
                    att_response_handle =0;
                    att_response_size = 0;

                    // restart advertising
                    hci_send_cmd(&hci_le_set_advertise_enable, 1);
                    break;

				case HCI_EVENT_COMMAND_COMPLETE:
					if (COMMAND_COMPLETE_EVENT(packet, hci_read_bd_addr)){
    					bt_flip_addr(addr, &packet[6]);
					    printf("BD ADDR: %s\n", bd_addr_to_str(addr));
						break;
					}
					if (COMMAND_COMPLETE_EVENT(packet, hci_read_local_supported_features)){
					    printf("Local supported features: %04X%04X\n", READ_BT_32(packet, 10), READ_BT_32(packet, 6));
                        hci_send_cmd(&hci_set_event_mask, 0xffffffff, 0x20001fff);
						break;
					}
					if (COMMAND_COMPLETE_EVENT(packet, hci_set_event_mask)){
                        hci_send_cmd(&hci_write_le_host_supported, 1, 1);
						break;
					}
					if (COMMAND_COMPLETE_EVENT(packet, hci_write_le_host_supported)){
				        hci_send_cmd(&hci_le_set_event_mask, 0xffffffff, 0xffffffff);
                    	break;
					}
					if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_event_mask)){
				        hci_send_cmd(&hci_le_read_buffer_size);
                    	break;
					}
				    if (COMMAND_COMPLETE_EVENT(packet, hci_le_read_buffer_size)){
					    printf("LE buffer size: %u, count %u\n", READ_BT_16(packet,6), packet[8]);
					   hci_send_cmd(&hci_le_read_supported_states);
					   break;
					}
				    if (COMMAND_COMPLETE_EVENT(packet, hci_le_read_supported_states)){
				    	printf("i cant believe it");
					   hci_send_cmd(&hci_le_set_advertising_parameters,  0x0400, 0x0800, 0, 0, 0, &addr, 0x07, 0);
					   break;
					}
				    if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertising_parameters)){

					   hci_send_cmd(&hci_le_set_advertising_data, sizeof(adv_data), adv_data);
					   break;
					}
				    if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertising_data)){
					   hci_send_cmd(&hci_le_set_scan_response_data, 10, adv_data);
					   break;
					}
				    if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_scan_response_data)){
				    	printf("we did it");
					   hci_send_cmd(&hci_le_set_advertise_enable, 1);
					   break;
					}
				    if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertise_enable)){
                        hci_discoverable_control(1);
                        break;
					}

			}
	}
}



int main(void) {

	btstack_memory_init();
	run_loop_init(RUN_LOOP_EMBEDDED);

 // init HCI
	    hci_transport_t    * transport = hci_transport_h4_dma_instance();
	    remote_device_db_t * remote_db = (remote_device_db_t *) &remote_device_db_memory;
	    static hci_uart_config_t  config;
	        config.device_name   = "CC2540";   // 5529
	        config.baudrate_init = 0;
	        config.baudrate_main = 0;
	        config.flowcontrol = 1;
	    hci_init(transport, &config, NULL, remote_db);

	    // set up l2cap_le



	   /*     __enable_interrupt(); */

	        printf("Run...\n\n");

	        // turn on!
	        hci_power_control(HCI_POWER_ON);
	       /* hci_send_cmd(&hci_set_event_mask, 0xffffffff, 0x20001fff);*/
	        hci_register_packet_handler(packet_handler);
	        // go!
	        hci_send_cmd(&hci_read_bd_addr);
	        hci_send_cmd(&hci_read_local_supported_features);
	        hci_send_cmd(&hci_le_read_supported_states);

	        run_loop_execute();



	    //hci_register_packet_handler(packet_handler);
	return 0 ;
}
