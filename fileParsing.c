#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "fileParsing.h"
#include "stationsADT.h"

#define MAX_CHAR 250
#define DELIM ";"
#define NYC 0
#define MON 1

void parseStations(stationsADT stationsAdt, FILE * file, size_t identifier) {
    char str[MAX_CHAR], * name;
    size_t id;
    fgets(str, MAX_CHAR, file); // obtiene la primera linea, que solamente aclara el formato.

    while(fgets(str, MAX_CHAR, file) != NULL) {
        if(identifier == NYC) {
            name = strtok(str, DELIM);
            strtok(NULL, DELIM);
            strtok(NULL, DELIM);
            id = atoi(strtok(NULL, DELIM));
        } else {
            id = atoi(strtok(str, DELIM));
            name = strtok(NULL, DELIM);
        }
        addStation(stationsAdt, id, name);
    }
    //fillOrderedIds(stationsAdt); // crea el arreglo ordenado por id
}

void parseEvents(stationsADT stationsAdt, FILE * file, size_t identifier, char isRange) {
    char str[MAX_CHAR], isMember, *aux;
    size_t fromId, toId, month, year, day;
    fgets(str, MAX_CHAR, file); // obtiene la primera linea, que solamente aclara el formato.
    //newMat(stationsAdt); // creo la matriz de (cant_estaciones)x(cant_estaciones)

    while(fgets(str, MAX_CHAR, file) != NULL) {
        isMember = 0;
        aux = strtok(str, DELIM);
        fromId = atoi(strtok(NULL, DELIM));
        strtok(NULL, DELIM);
        toId = atoi(strtok(NULL, DELIM));

        if(identifier == NYC) {
            strtok(NULL, DELIM);
            if(strcmp(strtok(NULL, DELIM), "member\n") == 0) { //la linea del archivo termina con un '\n'
                isMember = 1;
            }
        } else {
            isMember = atoi(strtok(NULL, DELIM));
        }
        year = atoi(strtok(aux, "-"));
        month = atoi(strtok(NULL, "-"));
        day = atoi(strtok(NULL, "-"));
        processEvent(stationsAdt, year, month, day, fromId, toId, isMember, isRange);
    }
}
