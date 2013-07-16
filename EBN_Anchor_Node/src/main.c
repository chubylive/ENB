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
#include "BT_Stack/includes/hal_uart_dma.h"
#include "BT_Stack/Drivers/includes/lpc17xx_uart.h"

#include "EBN_Core/includes/bget.h"
#include "EBN_Core/includes/mem.h"
#include "EBN_Core/includes/EBN.h"

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include <stdio.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP;

#define MEMSIZE (1 << 13) //8kb 
// TODO: insert other definitions and declarations here

uint8_t mempool[MEMSIZE]; //memory pool

static uint8_t SCAN_COUNT = 0;
static uint8_t SCANNING_NOW = 0;
static uint8_t STATE = 0;
static uint8_t CHANGING_ADDR = 0;

static void scan_handler(struct timer *ts) {
	if (hci_can_send_packet_now(HCI_COMMAND_DATA_PACKET) && (SCAN_COUNT >= 1)
			&& (SCANNING_NOW == 0)) {
		gap_send_cmd(&gap_device_discovery_request, 0x03, 0x00, 0x00);

		SCANNING_NOW = 1;
	}
	run_loop_set_timer(ts, SCAN_INTV);
	run_loop_add_timer(ts);

}
static void epoch_change_handler(struct timer *ts) {
	if (EBN_Increment_Time() && STATE) {

		gap_send_cmd(&gap_end_discoverable);
		CHANGING_ADDR = 1;
		printf("curr BD ADDR: %s\n", bd_addr_to_str(EBN_Get_Mac_Address()));
	}

	gap_send_cmd(&gap_UpdateAdvertisingData, 0x01, 31, EBN_Get_Advert());
	run_loop_set_timer(ts, EPOCH_INTV);
	run_loop_add_timer(ts);
}

// enable LE, setup ADV data
static void packet_handler(uint8_t packet_type, uint8_t *packet, uint16_t size) {
	bd_addr_t addr;
	bd_addr_t blank_addr;
	static uint8_t BROADCASTING = 0;
	static uint8_t PARAM_STATE = 0;
	uint8_t advt[31] = { 0xFF, 0xFF, 0xFF, 0xFF };

	switch (packet_type) {

	case HCI_EVENT_PACKET:
		switch (packet[0]) {

		case BTSTACK_EVENT_STATE:
			// bt stack activated, get started - set local name
			if (packet[2] == HCI_STATE_WORKING) {
				printf("Working!\n");
				hci_send_device_int();
				//gap_send_cmd(&gap_device_init, (GAP_PROFILE_CENTRAL|GAP_PROFILE_BROADCASTER), 5,IRK_CSRK,IRK_CSRK,0x00000001 );
			}
			break;

		case HCI_LE_ExtEvent:

			if ((READ_BT_16(packet, 2) == GAP_DeviceInitDone)) {
				gap_send_cmd(&gap_set_param, 0x01, 0x0000);
				PARAM_STATE++;
				break;
			}

			if ((READ_BT_16(packet, 5) == GAP_SetParam) && PARAM_STATE == 1) {
				gap_send_cmd(&gap_set_param, 0x08, 0x0320); //set minimum advertising interval
				PARAM_STATE++;
				break;
			}

			if ((READ_BT_16(packet, 5) == GAP_SetParam) && PARAM_STATE == 2) {
				gap_send_cmd(&gap_set_param, 0x09, 0x0960); //set maximum adertising interval
				PARAM_STATE++;
				break;
			}

			/*if(READ_BT_16(packet, 5) == GAP_SetParam ){

			 }*/

			if ((READ_BT_16(packet, 5) == GAP_SetParam && PARAM_STATE == 3)) {
				gap_send_cmd(&gap_config_device_addr, 0x01,
						EBN_Get_Mac_Address());
				break;
			}

			if (READ_BT_16(packet, 5) == GAP_ConfigDeviceAddr) {
				if (EBN_Get_Advert()) {
					//hci_send_cmd(&hci_le_set_advertising_data,31, EBN_Get_Advert());
					gap_send_cmd(&gap_UpdateAdvertisingData, 0x01, 31,
							EBN_Get_Advert());
				} else {
					gap_send_cmd(&gap_UpdateAdvertisingData, 0x01, 4, advt);

				}
				break;
			}

			if ((READ_BT_16(packet, 2) == GAP_AdvertDataUpdate)
					&& (packet[5] == 0x01)) { //0x01 Advertisement data
				if (EBN_Get_Advert()) {
					//hci_send_cmd(&hci_le_set_advertising_data,31, EBN_Get_Advert());
					gap_send_cmd(&gap_UpdateAdvertisingData, 0x00, 31,
							EBN_Get_Advert());
				} else {
					gap_send_cmd(&gap_UpdateAdvertisingData, 0x00, 4, advt);

				}
				break;
			}

			if (READ_BT_16(packet, 2) == GAP_AdvertDataUpdate
					&& packet[5] == 0x00) { // ScanResponse data
				if (BROADCASTING == 0) {
					gap_send_cmd(&gap_make_discoverable, 0x03, 0x01,
							&blank_addr, 0x07, 0);
				}

				STATE = 1;
				break;

			}
			if ((READ_BT_16(packet,5) == GAP_MakeDiscoverable)) {
				BROADCASTING = 1;
				break;
			}

			if (READ_BT_16(packet,2) == GAP_EndDiscoverable_Event) {
				BROADCASTING = 0;
				gap_send_cmd(&gap_config_device_addr, 0x01,
						EBN_Get_Mac_Address());

				break;
			}

			/*

			 /* scanning initiation*/

			/*

			 if (READ_BT_16(packet, 2) == GAP_DeviceDiscoveryDone) {
			 uint8_t num_of_DD = packet[5];
			 printf("Number of devices found: %d\n", num_of_DD);
			 SCAN_COUNT += num_of_DD;
			 SCANNING_NOW = 0;
			 break;
			 }

			 if (READ_BT_16(packet,2) == GAP_DeviceInformation) {
			 bd_addr_t dev_addr;

			 bt_flip_addr(dev_addr, &packet[7]);

			 printf("found Device BD ADDR: %s\n", bd_addr_to_str(dev_addr));

			 }*/

		case HCI_EVENT_COMMAND_COMPLETE:
			if (COMMAND_COMPLETE_EVENT(packet, hci_read_bd_addr)) {
				bt_flip_addr(addr, &packet[6]);
				printf("BD ADDR: %s\n", bd_addr_to_str(addr));
				break;
			}
		}
	}
}

int main(void) {
	//hardware config0
	//clock

	memset(mempool, '\0', MEMSIZE);
	bpool((void *) mempool, MEMSIZE);

	EBN_Init();
	btstack_memory_init();
	run_loop_init(RUN_LOOP_EMBEDDED);

	// init HCI
	hci_transport_t * transport = hci_transport_h4_dma_instance();
	remote_device_db_t * remote_db =
			(remote_device_db_t *) &remote_device_db_memory;
	hci_init(transport, NULL, NULL, remote_db);

	//setting up timer
	timer_source_t scan;
	scan.process = &scan_handler;
	run_loop_set_timer(&scan, SCAN_INTV);
	run_loop_add_timer(&scan);

	timer_source_t epoch;
	epoch.process = &epoch_change_handler;
	run_loop_set_timer(&epoch, EPOCH_INTV);
	run_loop_add_timer(&epoch);

	printf("Run...\n\n");

	// turn on!
	hci_power_control(HCI_POWER_ON);

	hci_register_packet_handler(packet_handler);
	// go!
	run_loop_execute();
	//hci_register_packet_handler(packet_handler);
	return 0;
}

