/*
 * SimpleCache.h
 *
 *  Created on: Jul 2, 2013
 *      Author: chuby
 */

#ifndef SIMPLECACHE_H_
#define SIMPLECACHE_H_

#include "lpc_types.h"


#define CACHE_SIZE 512 //bytes

extern uint8_t cache[CACHE_SIZE];
extern uint8_t cache_pos;

void add_to_cache(uint8_t *data, uint8_t size);
uint8_t flush();


#endif /* SIMPLECACHE_H_ */
