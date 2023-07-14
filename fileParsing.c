#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "fileParsing.h"
#include "stationsADT.h"

#define MAX_CHAR 250

void parseStations(stationsADT stationsAdt, FILE *file, size_t identifier) {
    const char delim[2] = ";";
    char str[MAX_CHAR], * name;
    size_t id;
    fgets(str, MAX_CHAR, file); // obtiene la primera linea, que solamente aclara el formato.

    while(fgets(str, MAX_CHAR, file) != NULL) {
        if(identifier == NYC) {
            name = strtok(str, delim);
            strtok(NULL, delim);
            strtok(NULL, delim);
            id = atoi(strtok(NULL, delim));
        } else {
            id = atoi(strtok(str, delim));
            name = strtok(NULL, delim);
        }
        addStation(stationsAdt, id, name);
    }
    fillOrderedIds(stationsAdt); // crea el arreglo ordenado por id
}

void parseEvents(stationsADT stationsAdt, FILE *file, size_t identifier) {
    const char delim[2] = ";";
    char str[MAX_CHAR], isMember, *aux;
    size_t fromId, toId, month, year, day;
    fgets(str, MAX_CHAR, file); // obtiene la primera linea, que solamente aclara el formato.
    newMat(stationsAdt); // creo la matriz de (cant_estaciones)x(cant_estaciones)

    while(fgets(str, MAX_CHAR, file) != NULL) {
        isMember = 0;
        aux = strtok(str, delim);
        fromId = atoi(strtok(NULL, delim));
        strtok(NULL, delim);
        toId = atoi(strtok(NULL, delim));

        if(identifier == NYC) {
            strtok(NULL, delim);
            if(strcmp(strtok(NULL, delim), "member\n") == 0) { //la linea del archivo termina con un '\n'
                isMember = 1;
            }
        } else {
            isMember = atoi(strtok(NULL, delim));
        }
        year = atoi(strtok(aux, "-"));
        month = atoi(strtok(NULL, "-"));
        day = atoi(strtok(NULL, "-"));
        processEvent(stationsAdt, year, month, day, fromId, toId, isMember);
    }
    rearrangeByTrip(stationsAdt, 0); // crea la lista ordenada por trips
    rearrangeByTrip(stationsAdt, 1); // crea la lista ordeanada por trips circulares
}

