#include <stdio.h>
#include "stationsADT.h"
#define NYC 0


#define DELIM ";"

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("Cantidad insuficiente de parametros\n");
        printf("Uso: ./bikeSharingMON archivo_data_alquileres archivo_data_estaciones\n");
        return 1;
    }

    stationsADT stationsAdt = newStations();

    // Carga de datos

    //processEvent(stationsAdt, argv[1]);
    addStations(stationsAdt, argv[2], NYC);

    freeStations(stationsAdt);

    return 0;
}