#include <stdlib.h>
#include <stdio.h>
#include "safeMemory.h"

void * safeMalloc(size_t bytes) {
    void * mem = malloc(bytes);
    if(mem == NULL) {
        fprintf(stderr, "Error de memoria");
        fprintf(stdout, "Uso de mas memoria de la que el sistema puede proveer");
    }
    return mem;
}

void * safeCalloc(size_t quan, size_t bytes) {
    void * mem = calloc(quan, bytes);
    if(mem == NULL) {
        fprintf(stderr, "Error de memoria");
        fprintf(stdout, "Uso de mas memoria de la que el sistema puede proveer");
    }
    return mem;
}