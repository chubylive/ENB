/*
 * SimpleCache.c
 *
 *  Created on: Jul 2, 2013
 *      Author: chuby
 */

#include "includes/SimpleCache.h"
#include "includes/sd.h"

uint8_t cache [CACHE_SIZE];
uint8_t cache_pos = 0;


void add_to_cache(uint8_t *data, uint8_t size){
	uint8_t idx = 0 ;
	uint8_t *src= data;
	uint8_t *dst = cache;
	uint8_t pos = cache_pos;
	if( (cache_pos + size) >= CACHE_SIZE){ //if the buffer might overflow flush and the try again
		flush();
		add_to_cache(data, size);
	}else{
		while( pos < size){
			dst[pos++] = src[idx++];
		}
	}
}

uint8_t flush(){
	uint8_t status = sd_write_block(cache,1);
	cache_pos = 0; //reset cache position
	return status;
}

