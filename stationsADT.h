/*  
    Archivo: stationsADT.h
    ----------------------
    Este archivo es la interfaz de un modulo que exporta
    el tipo asbtracto de dato "stations"
*/

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
    Funcion: processEvent
    Uso: processEvent(stations)
    ----------------------------------
    Esta funcion procesa un evento y modifica los atributos 
    de las estaciones con la informacion relevante para las queries.
*/

void processEvent(stationsADT);
#endif