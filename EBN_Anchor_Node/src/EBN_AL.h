/*
 * EBN_AL.h
 *
 *  Created on: Jul 2, 2013
 *      Author: chuby
 */


uint8_t setRandomAddrss(bd_addr_t address);
uint8_t setAdvertisingData(uint8_t *data, uint8_t length);
uint8_t setScanResponseData(uint8_t *data, uint8_t length);
uint8_t setUndirectedAdvertisingParams(uint16_t minInterval, uint16_t maxInterval, uint8_t advType, uint8_t ownBDADDRType, uint8_t filterType);

