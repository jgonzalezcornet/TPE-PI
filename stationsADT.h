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

void newMat(stationsADT stationsAdt);

 void printMatrix(stationsADT stationsAdt);

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

 void addStation(stationsADT stationsAdt, size_t id, char* name);

/*
    Funcion: printStations
    Uso: printStations(estaciones)
    -------------------------------
    Esta funcion se encarga de imprimir en pantalla las
    caracteristicas de todas las estaciones. Mas que nada
    se le da un uso en los testing. (NO VA A ESTAR EN EL ULTIMO ARCHIVO XQ NO FORMA PARTE DEL BACK-END)
*/

void printStations(stationsADT stationsAdt);

/*
    Funcion: freeStations
    Uso: freeStations(estaciones)
    ------------------------------
    Esta funcion se encarga de liberar la memoria ocupada
    por el ADT.
*/

void processEvent(stationsADT stationsAdt, size_t month, size_t fromId, size_t toId, char isMember);

void freeStations(stationsADT stationsAdt);

/*
    Funcion: toBeginTrip
    Uso: toBeginTrip(estaciones)
    ----------------------------
    Esta funcion hace que al iterador por cantidad de viajes
    "apunte" hacia el comienzo de la lista ordenada descendentemente
    por cantidad de viajes.
*/

void toBeginTrip(stationsADT stationsAdt);

void printLinks(stationsADT stationsAdt);

void printSubStations(stationsADT stationsAdt);

/*
    Funcion: hasNextTrip
    Uso: hasNextTrip(estaciones)
    ----------------------------
    Esta funcion verifica si el iterador por cantidad de viajes
    tiene un elemento siguiente, para evitar problemas de memoria.
*/

size_t hasNextTrip(stationsADT stationsAdt);

/*
    Funcion: nextTrip
    Uso: nextTrip(estaciones)
    -------------------------
    Esta funcion "apunta" el iterador por cantidad de viajes
    a su elemento siguiente, verificando previamente que este
    exista. Si se pudo mover, retorna 1. Si no, 0.
*/

size_t nextTrip(stationsADT stationsAdt);

#endif