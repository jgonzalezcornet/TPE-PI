#include <stdio.h>
#include <stdlib.h>
#include "queries.h"
#include "stationsADT.h"
#include "fileParsing.h"
#include "safeMemory.h"

#define QUERIES 5

#ifdef MON
#define CITY 1  // si queremos trabajar sobre MON, utilizamos el identificador CITY = 1
#else
#define CITY 0  // si queremos trabajar sobre NYC, utilizamos el identificador CITY = 0
#endif

// Funciones para cerrar todos los archivos
void closeFiles(size_t count, FILE * files[]);
void closeTables(size_t count, htmlTable tables[]);

int main(int argc, char *argv[]) {

    enum {OK=0, ERROR, FILEERROR, MEMORYERROR};
    size_t status = 0;

    if(argc > 5 || argc < 3 || (argc == 5 && atoi(argv[3]) > atoi(argv[4]))) {
        fprintf(stderr, "Cantidad inválida de parámetros.\n");
        fprintf(stdout, "Uso: ./bikeSharingMON archivo_data_alquileres archivo_data_estaciones anio_1 anio_2\n");
        status = ERROR;
        exit(ERROR);
    }

    // Abrimos todos los archivos, estableciendo el errno en 0 para poder chequear si hay errores en la apertura
    errno = 0;
    FILE * events = fopen(argv[1], "r");
    FILE * stations = fopen(argv[2], "r");
    size_t firstYear = (argc > 3 ? atoi(argv[3]) : 0);
    size_t lastYear = (argc > 4 ? atoi(argv[4]) : MAX_YEAR - 1);
    FILE * que1 = fopen("query1.csv", "wt");
    FILE * que2 = fopen("query2.csv", "wt");
    FILE * que3 = fopen("query3.csv", "wt");
    FILE * que4 = fopen("query4.csv", "wt");
    FILE * que5 = fopen("query5.csv", "wt");
    FILE * files[] = {events, stations, que1, que2, que3, que4, que5};
    size_t fileCount = QUERIES + 3 - 1;
    size_t tableCount = QUERIES;

    char isRange = 1;
    if(argc <= 3){
        isRange = 0;
    }

    if(errno == ENOENT) {
        closeFiles(fileCount, files); // si alguno de los archivos no se pudo abrir, cierro todos
        fprintf(stderr, "No se pudo abrir alguno de los archivos.\n");
        status = FILEERROR;
        exit(FILEERROR);
    }

    stationsADT stationsAdt = newStations(firstYear, lastYear, &status);

    // para toda la duracion del programa, si en alguna instancia flag == MEMORYERROR el programa aborta
    if (status == MEMORYERROR){
        fprintf(stderr, "No hay memoria suficiente para llevar a cabo el programa.\n");
        closeFiles(fileCount, files);
        status = MEMORYERROR;
        exit(MEMORYERROR);
    }

    // Carga de datos
    parseStations(stationsAdt, stations, CITY, &status);
    parseEvents(stationsAdt, events, CITY, isRange, &status);

    // Resolucion de las queries (tanto en HTML como en CSV)
    htmlTable tableQuery1 = query1(stationsAdt, que1, &status);
    htmlTable tableQuery2 = query2(stationsAdt, que2, &status);
    htmlTable tableQuery3 = query3(stationsAdt, que3, &status);
    htmlTable tableQuery4 = query4(stationsAdt, que4, &status);
    htmlTable tableQuery5 = query5(stationsAdt, que5, getFirstYear(stationsAdt), getLastYear(stationsAdt), &status);

    htmlTable tables[] = {tableQuery1, tableQuery2, tableQuery3, tableQuery4, tableQuery5};

    // Liberacion de memoria
    freeStations(stationsAdt);

    // Cerramos los files
    closeFiles(fileCount, files);
    closeTables(tableCount, tables);

    return status;  // para saber el estado del programa al finalizar la ejecucion
}

void closeTables(size_t count, htmlTable tables[]) {
    for(size_t i = 0; i < count; i++) {
        if(tables[i] != NULL) {
            closeHTMLTable(tables[i]);
        }
    }
}

void closeFiles(size_t count, FILE * files[]) {
    for(size_t i = 0; i < count; i++) {
        if(files[i] != NULL) {
            fclose(files[i]);
        }
    }
}
