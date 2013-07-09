#include "mem.h"

int memcmp(const void *str1, const void *str2, size_t n){
    const char* d1 = (const char *) str1;
    const char* d2 = (const char *) str2;
    int i = 0;
    for(i = 0;i < n;i++){
        if(d1[i] != d2[i]){
            return 0;
        }
    }
    return 1;
}

void *memcpy(void *str1, const void *str2, size_t n){
    char* d = (char *) str1;
    const char* s = (const char *) str2;
    int i = 0;
    for(i = 0;i < n;i++){
        d[i] = s[i];
    }
    return str1;
}

void *memset(void *str, int c, size_t n){
    char* d = (char *) str;
    int i = 0;
    for(i = 0;i < n;i++){
        d[i] = c;
    }
    return str;
}


