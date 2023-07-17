/*
    Archivo: fileParsing.h
    ----------------------
    Este archivo se encarga del parseo de los datos,
    ya sea de las estaciones o de los eventos.
*/

#include <stdio.h>
#include "stationsADT.h"

#ifndef _fileparsing_h
#define _fileparsing_h

/*
    Funcion: parseStations
    Uso: parseStations(stations, file, id);
    ---------------------------------------
    Esta funcion se encarga de procesar el archivo con las
    estacion para asi generar la estructura de estaciones.
    El identificador nos indica si se procesa el archivo cuya estructura corresponde
    a la de Montreal o a la de Nueva York.
*/

void parseStations(stationsADT stationsAdt, FILE *file, size_t identifier);

/*
    Funcion: parseEvents
    Uso: parseEvents(stations, file, id);
    -------------------------------------
    Esta funcion se encarga de procesar el archivo con los eventos
    para asi actualizar los datos de la estructura estaciones.
    El identificador nos indica si se procesa el archivo cuya estructura corresponde
    a la de Montreal o a la de Nueva York.
*/

void parseEvents(stationsADT stationsAdt, FILE *file, size_t identifier);

#endif
