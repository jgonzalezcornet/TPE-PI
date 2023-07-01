#include <stdio.h>
#include "queries.h"
#include "stationsADT.h"

#define DELIM ";"

int main(int argc, char *argv[]) {
    
    if(argc < 3) {
        printf("Cantidad insuficiente de parametros\n");
        printf("Uso: ./bikeSharingMON archivo_data_alquileres archivo_data_estaciones\n");
        return 1;
    }

    stationsADT stationsAdt = newStations();
    
    // Carga de datos

    processEvents(stationsAdt, argv[1]);
    processStations(stationsADT, argv[2]);

    // Procesamiento de queries

    query1(stationsAdt);
    query2(stationsAdt);
    query3(stationsAdt);

    // Liberacion de memoria

    freeStations(stationsAdt);
    return 0;
}