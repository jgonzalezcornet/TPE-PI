#include <stdio.h>
#include "queries.h"
#include "stationsADT.h"
#include "fileParsing.h"

#define MON 1
#define DELIM ";"
#define MONTHS 12
#define QUERIES 3

// Función para cerrar todos los archivos
void closeFiles(size_t count, FILE * files[]);

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Cantidad inválida de parámetros.\n");
        fprintf(stdout, "Uso: ./bikeSharingMON archivo_data_alquileres archivo_data_estaciones\n");
        exit(1);
    }

    // Abrimos todos los archivos, estableciendo el errno en 0 para poder chequear si hay errores en la apertura
    errno = 0;
    FILE *events = fopen(argv[1], "r");
    FILE *stations = fopen(argv[2], "r");
    FILE *que1 = fopen("query1.csv", "wt");
    FILE *que2 = fopen("query2.csv", "wt");
    FILE *que3 = fopen("query3.csv", "wt");
    FILE *files[] = {events, stations, query1, query2, query3};
    size_t fileCount = QUERIES + argc - 1;
    
    if(errno == ENOENT) {
        closeFiles(fileCount, files); // si alguno de los archivos no se pudo abrir, cierro todos
        fprintf(stderr, "No se pudo abrir alguno de los archivos.\n");
        exit(1);
    }
    
    stationsADT stationsAdt = newStations();
    if(stationsAdt == NULL) {
        fprintf(stderr, "No hay memoria suficiente para llevar a cabo el programa.\n");
        closeFiles(fileCount, files);
        exit(1);
    }
    
    // Carga de datos
    parseEvents(stationsAdt, events, MON);
    parseStations(stationsAdt, stations, MON);

    // Resolucion de las queries
    htmlTable tableQuery1 = query1(stationsAdt, que1);
    htmlTable tableQuery2 = query2(stationsAdt, que2);
    htmlTable tableQuery3 = query3(stationsAdt, que3);

    // Liberacion de memoria
    freeStations(stationsAdt);
    // Cerramos los files que hayan quedado abiertos
    closeFiles(fileCount, files);
    
    return 0;
}

void closeFiles (size_t count, FILE * files[]){
    for(size_t i=0; i < count; i++) {
        if (files[i] != NULL){
            fclose(files[i]);
        }
    }
    return;
}
