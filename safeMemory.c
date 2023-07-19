#include <stdlib.h>
#include <stdio.h>
#include "safeMemory.h"

void * safeMalloc(size_t bytes, size_t * status, stationsADT stationsAdt) {
    void * mem = malloc(bytes);
    if(mem == NULL) {
        freeStations(stationsAdt);
        *status = 3;    // error de memoria
    }
    return mem;
}

void * safeCalloc(size_t quan, size_t bytes, size_t * status, stationsADT stationsAdt) {
    void * mem = calloc(quan, bytes);
    if(mem == NULL) {
        freeStations(stationsAdt);
        *status = 3;    // error de memoria
    }
    return mem;
}
