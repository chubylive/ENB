
// Simple implementation of the basic memory functions that
//  allow us to override the use of the standard libraries
//
#ifndef MEM_H
#define MEM_H

#define exit(val) {while(1);}
#define assert(val) if(!(val)) { exit(1); }
#define NULL 0
typedef unsigned int size_t;

int memcmp(const void *str1, const void *str2, size_t n);

void *memcpy(void *str1, const void *str2, size_t n);

#define bzero(p,n)  memset(p,0,n)
void *memset(void *str, int c, size_t n);

#endif
