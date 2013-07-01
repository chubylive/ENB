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
#include "BT_Stack/includes/hal_uart_dma.h"
#include "BT_Stack/Drivers/includes/lpc17xx_uart.h"

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP;

#include <stdio.h>

// TODO: insert other definitions and declarations here

static uint8_t HCI_WORKING = 0;
static uint8_t COUNT = 0;
static uint8_t SCAN_COUNT = 0;
static uint8_t SCANNING_NOW = 0;

static void device_init(void) {
	hal_uart_dma_init();
	uint8_t rx_payload[47];
	uint8_t tx_payload[] = { 0x01, 0x00, 0xFE, 0x26, 0x04, 0x04, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
			0x00 };
	UART_FullModemForcePinState((LPC_UART_TypeDef *) LPC_UART1,
			UART1_MODEM_PIN_RTS, INACTIVE);
	hal_uart_dma_send_block(tx_payload, 42);
	hal_uart_dma_receive_block(rx_payload, 9);
	hal_uart_dma_receive_block(rx_payload, 47);
	UART_DeInit((LPC_UART_TypeDef*) LPC_UART1 );

}

// enable LE, setup ADV data
static void packet_handler(uint8_t packet_type, uint8_t *packet, uint16_t size) {
	bd_addr_t addr;
	uint8_t adv_data[31] = "\x03\x02\x02" "\x05\x09mbed" "\x03\x02\xf0\xff";

	switch (packet_type) {

	case HCI_EVENT_PACKET:
		switch (packet[0]) {

		case BTSTACK_EVENT_STATE:
			// bt stack activated, get started - set local name
			if (packet[2] == HCI_STATE_WORKING) {
				printf("Working!\n");
				hci_send_device_int();
				HCI_WORKING = 1;
			}
			break;

		case HCI_EVENT_VENDOR_SPECIFIC:

			// handle in-coming scan response or passive scans.
			// write macros for event codes that i will encounter

			if (READ_BT_16(packet, 2) == GAP_DeviceInitDone) {
				hci_send_cmd(&hci_le_set_advertising_parameters, 0x0400, 0x0800,
						0x03, 0, 0, &addr, 0x07, 0);
				break;
			}
			if ((READ_BT_16(packet,2 ) == 0x0602) && COUNT == 0) { //find macro for this
				hci_send_cmd(&hci_le_set_advertising_data, 31, adv_data);
				COUNT++;
				break;
			}
			if ((READ_BT_16(packet,2 ) == 0x0602) && COUNT == 1) { //and this
				hci_send_cmd(&hci_le_set_advertise_enable, 1);
				COUNT++;
				break;

			}

			/* scanning initiation*/

			if ((READ_BT_16(packet, 2) == 0x0602) && COUNT == 2) { // and this
				puts("hci_le_set_scan_response_data ran");
				gap_send_cmd(&gap_device_discovery_request, 0x03, 0x00, 0x00);
				break;
			}

			if (READ_BT_16(packet, 2) == GAP_DeviceDiscoveryDone) {
				uint8_t num_of_DD = packet[5];
				printf("Number of devices found: %d\n", num_of_DD);
				SCAN_COUNT++;
				SCANNING_NOW = 0;
				break;
			}

			if (READ_BT_16(packet,2) == GAP_DeviceInformation) {
				bd_addr_t dev_addr;
				bt_flip_addr(dev_addr, &packet[7]);
				printf("found Device BD ADDR: %s\n", bd_addr_to_str(dev_addr));

			}

		case HCI_EVENT_COMMAND_COMPLETE:
			if (COMMAND_COMPLETE_EVENT(packet, hci_read_bd_addr)) {
				bt_flip_addr(addr, &packet[6]);
				printf("BD ADDR: %s\n", bd_addr_to_str(addr));
				break;
			}
			if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertising_parameters)) {
				if (packet[5] == 0) {
					puts("hci_le_set_advertising_parameters success");
				}

				gap_send_cmd(&gap_UpdateAdvertisingData, 0x03, 0x00, 0x01,
						0x00);

				break;
			}
			if (COMMAND_COMPLETE_EVENT(packet,hci_le_set_advertise_enable)) {
				puts("hci_le_set_scan_parameters success");
				hci_send_cmd(&hci_le_set_scan_response_data, 31, adv_data);

				break;
			}
			if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_scan_response_data)) {
				hci_send_cmd(&hci_le_set_scan_parameters, 1, 0x0010, 0x0010, 0,
						0);
				break;
			}
			if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_scan_parameters)) {
				if (packet[5] == 0) {
					puts("hci_le_set_advertising_parameters success");
					gap_send_cmd(&gap_set_param, 2, 0xFFFA);
				}
			}

		}
	}
}

static void scan_handler(struct timer *ts) {
	if (hci_can_send_packet_now(HCI_COMMAND_DATA_PACKET) && (SCAN_COUNT >= 1)
			&& (SCANNING_NOW == 0)) {
		gap_send_cmd(&gap_device_discovery_request, 0x03, 0x00, 0x00);
		SCANNING_NOW = 1;
	}
	run_loop_set_timer(ts, SCAN_INTV);
	run_loop_add_timer(ts);

}

int main(void) {
	//hardware config
	//clock
	//adc

	//device_init();
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

	printf("Run...\n\n");

	// turn on!
	hci_power_control(HCI_POWER_ON);

	hci_register_packet_handler(packet_handler);
	// go!
	run_loop_execute();
	//hci_register_packet_handler(packet_handler);
	return 0;
}

