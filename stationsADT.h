 /*  
    Archivo: stationsADT.h
    ----------------------
    Este archivo es la interfaz de un modulo que exporta
    el tipo asbtracto de dato "stations"
*/

#include <stdio.h>

#ifndef _stations_h
#define _stations_h

typedef struct stationsCDT *stationsADT;

/*
    Funcion: newStations
    Uso: stations = newStations();
    ------------------------------
    Esta funcion crea una nueva instancia del tipo abstracto
    de datos "stations". Inicialmente, no habra informacion cargada.
*/

stationsADT newStations();

/*
    Funcion: freeStations
    Uso: freeStations(stations)
    ---------------------------
    Esta funcion libera los espacios de memoria asociados con "stations".
*/

void freeStations(stationsADT);

/*
    Funcion: addStations
    Uso: addStations(estaciones, archivo, indentificador)
    -----------------------------------------------------
    Esta funcion agrega las estaciones que contiene el archivo.
    A su vez, posee un "identificador" para cada ciudad correspondiente,
    el cual puede tomar 2 valores diferentes de constantes predefinidas:
    . 0 si la ciudad es Nueva York, o un tipo de formato como esta,
    . 1 si la ciudad es Montreal, o un tipo de formato como esta.
*/

void addStations(stationsADT, FILE * file, size_t identifier);

/*
    Funcion: processEvent
    Uso: processEvent(stations, event)
    ----------------------------------
    Esta funcion procesa un evento y modifica los atributos 
    de las estaciones con la informacion relevante para las queries.
*/

void processEvent(stationsADT, FILE * file);

/*
    Funcion: printStations
    Uso: printStations(stationsAdt)
    -------------------------------
    Esta funcion se encarga de imprimir en pantalla las
    caracteristicas de todas las estaciones. Mas que nada
    se le da un uso en los testing. (NO VA A ESTAR EN EL ULTIMO ARCHIVO XQ NO FORMA PARTE DEL BACK-END)
*/

void printStations(stationsADT stationsAdt);

/*
    Funcion: freeStations
    Uso: freeStations(stationsAdt)
    ------------------------------
    Esta funcion se encarga de liberar la memoria ocupada
    por el ADT.
*/

void freeStations(stationsADT stationsAdt);

#endif 