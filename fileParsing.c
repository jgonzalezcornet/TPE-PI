#include <stdlib.h>
#include <stddef.h>
#include "fileParsing.h"
#include "stationsADT.h"
#include <string.h>
#include <time.h>

#define MAX_CHAR 250
#define NYC 0
#define MON 1

void parseStations(stationsADT stationsAdt, FILE *file, size_t identifier) {
    const char delim[2] = ";";
    char str[MAX_CHAR], *name;
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
}

void parseEvents(stationsADT stationsAdt, FILE *file, size_t identifier) {
    const char delim[2] = ";";
    char str[MAX_CHAR], isMember, *aux;
    size_t fromId, toId, month;
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
        strtok(aux, "-");
        month = atoi(strtok(NULL, "-"));
        processEvent(stationsAdt, month, fromId, toId, isMember);
    }
}

int main()
{

    clock_t start,end;
    double execution_time;
    start = clock();
    FILE *bikes = fopen("bikesMON.csv","rt");
    FILE *stations = fopen("stationsMON.csv","rt");

    stationsADT stationsAdt = newStations();

    parseStations(stationsAdt,stations,MON);
    parseEvents(stationsAdt,bikes,MON);
    printMatrix(stationsAdt);

    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("El tiempo de ejecucion fue de %f\n",execution_time);
    freeStations(stationsAdt);
    return 0;
}