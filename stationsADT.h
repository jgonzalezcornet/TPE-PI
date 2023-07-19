 /*  
    Archivo: stationsADT.h
    ----------------------
    Este archivo es la interfaz del modulo que exporta
    el tipo asbtracto de dato "stations"
*/

#include <stddef.h>

#ifndef _stations_h_
#define _stations_h_
#define MONTHS 12
#define MAX_YEAR 4000

typedef struct stationsCDT *stationsADT;

/*
    Funcion: newStations
    Uso: stations = newStations();
    ------------------------------
    Esta funcion crea una nueva instancia del tipo abstracto
    de datos "stations". Inicialmente, no habra informacion cargada.
*/

stationsADT newStations(size_t firstYear, size_t lastYear, size_t * status);

/*
    Funcion: newMat
    Uso: newMat(stations);
    ----------------------
    Esta funcion crea una matriz de  dimension dimStations*dimStations,
    con todas sus casillas inicializadas en cero. La matriz sirve para
    registrar los viajes de i->j (Acceso: mat[i][j]).
*/

void newMat(stationsADT stationsAdt, size_t * status);

/*
    Funcion: addStations
    Uso: addStations(stations, file, identifier);
    ---------------------------------------------
    Esta funcion agrega las estaciones que contiene el archivo.
    A su vez, posee un "identificador" para cada ciudad correspondiente,
    el cual puede tomar 2 valores diferentes de constantes predefinidas:
    . 0 si la ciudad es Nueva York, o un tipo de formato como esta,
    . 1 si la ciudad es Montreal, o un tipo de formato como esta.
*/

void addStation(stationsADT stationsAdt, size_t id, char * name, size_t * status);

/*
    Funcion: freeStations
    Uso: freeStations(stations);
    ----------------------------
    Esta funcion se encarga de liberar toda la memoria ocupada
    por el ADT.
*/

void freeStations(stationsADT stationsAdt);

/*
    Funcion: processEvent
    Uso: processEvent(stations. month, fromId, toId, isMember);
    -----------------------------------------------------------
    Esta funcion se encarga de procesar un único evento, completando
    los campos correspondientes en el TAD y en la matriz.
*/

void processEvent(stationsADT stationsAdt, size_t year, size_t month, size_t day, size_t fromId, size_t toId, char isMember, char isRange, size_t * status);

/*
    Funcion: rearrangeByTrip
    Uso: arrangeByTrip(stations);
    -----------------------------
    Esta funcion crea una nueva lista ordenada descendentemente
    por quanTripsMember.
*/

//flag == 0 -> firstByTrip    flag == 1 -> firstByRoundTrip
void rearrangeByTrip(stationsADT stationsAdt, size_t flag, size_t * status);

/*

*/

void rearrangeByRoundTrip(stationsADT stationsAdt);

/*
    Funcion: toBeginTrip
    Uso: toBeginTrip(stations);
    ---------------------------
    Esta funcion hace que al iterador por cantidad de viajes
    "apunte" hacia el comienzo de la lista ordenada descendentemente
    por cantidad de viajes.
*/

void toBeginTrip(stationsADT stationsAdt, size_t * status);

/*
    Funcion: hasNextTrip
    Uso: hasNextTrip(stations);
    ---------------------------
    Esta funcion verifica si el iterador por cantidad de viajes
    tiene un elemento siguiente, para evitar problemas de memoria.
*/

int hasNextTrip(stationsADT stationsAdt, size_t * status);

/*
    Funcion: nextTrip
    Uso: nextTrip(stations);
    ------------------------
    Esta funcion "apunta" el iterador por cantidad de viajes
    a su elemento siguiente, verificando previamente que este
    exista. Si se pudo mover, retorna 1. Si no, 0.
*/

int nextTrip(stationsADT stationsAdt, size_t * status);

/*
    Funcion: toBeginName
    Uso: toBeginName(stations);
    ---------------------------
    Esta funcion hace que al iterador por nombre
    "apunte" hacia el comienzo de la lista ordenada lexicográficamente.
*/

void toBeginName(stationsADT stationsAdt);

/*
    Funcion: hasNextName
    Uso: hasNextName(stations);
    ---------------------------
    Esta funcion verifica si el iterador por orden alfabético
    tiene un elemento siguiente, para evitar problemas de memoria.
*/

int hasNextName(stationsADT stationsAdt, size_t * status);

/*
    Funcion: nextName
    Uso: nextName(stations);
    ------------------------
    Esta funcion "apunta" el iterador por nombre
    a su elemento siguiente, verificando previamente que este
    exista. Si se pudo mover, retorna 1. Si no, 0.
*/

int nextName(stationsADT stationsAdt, size_t * status);

void toBeginRoundTrip(stationsADT stationsAdt, size_t * status);

int hasNextRoundTrip(stationsADT stationsAdt, size_t * status);

int nextRoundTrip(stationsADT stationsAdt, size_t * status);

/*
    Funcion: getName
    Uso: name = getName(stations, flag);
    ------------------------------------
    Esta funcion retorna una copia del "name"
    del iterador correspondiente según el valor
    de la flag: 1 = itName, 0 = itTrip.
*/

char * getNameByName(stationsADT stationAdt, size_t * status);

char * getNameByTrip(stationsADT stationAdt, size_t * status);

char * getNameByRoundTrip(stationsADT stationAdt, size_t * status);

/*
    Funcion: getTotalMemberTrips
    Uso: count = getTotalMemberTrips(stations, flag);
    -------------------------------------------------
    Esta funcion retorna una copia del "quanTripsMember"
    del iterador correspondiente según el valor
    de la flag: 1 = itName, 0 = itTrip, 2 = itRoundTrip.
*/

int getTotalTripsByTrip(stationsADT stationAdt, size_t * status);

int getTotalTripsByRoundTrip(stationsADT stationAdt, size_t * status);

/*
    Funcion: getTripsByMonth
    Uso: monthlyCount[i] = getTripsByMonth(stationsAdt, i+1);
    ---------------------------------------------------------
    Esta funcion retorna una copia de la cantidad de viajes
    comenzados en la estacion itName en e "month" correspondiente.
*/

size_t getTripsByMonth(stationsADT stationsAdt, size_t month, size_t * status);

/*
    Funcion: getTripsAtoB
    Uso: AB = getTripsAtoB(stations, indexA, indexB);
    -------------------------------------------------
    Esta funcion busca en la stationMat la cantidad de viajes iniciados
    en la estación A y finalizados en la estación B. Retorna una copia
    del valor.
*/

int getTripsAtoB(stationsADT stationsAdt, size_t indexA, size_t indexB);

/*
    Funcion: getMatrixName
    Uso: name = getMatrixName(stations, indexA, indexB);
    ----------------------------------------------------
    Esta funcion retorna una copia del "name" almacenado en
    stationMat[indexA][indexB], que se corresponde al nombre
    de la estación en la que comienzan los viaje en esa posicion
    de la matriz.
*/

char * getMatrixName(stationsADT stationsAdt, size_t indexA, size_t indexB);

/*
    Funcion: getDim
    Uso: dim = getDim(stations);
    ----------------------------
    Esta funcion retorna una copia de la cantidad de estaciones
    en stations.
*/

size_t getDim(stationsADT stationsAdt);

void getAfflux(stationsADT stationsAdt, size_t firstYear, size_t lastYear, int * posAfflux, int * neutralAfflux, int * negAfflux);

/*
    Funcion: fillOrderedIds
    Uso: fillOrderedIds(stations);
    ------------------------------
    Esta funcion replica la lista alfabeticamente
    ordenada en un arreglo, almacenando un puntero
    al nodo y su id, y luego ordena el arreglo
    ascendentemente por ids.
*/

size_t getFirstYear(stationsADT stationsAdt);

size_t getLastYear(stationsADT stationsAdt);

void fillOrderedIds(stationsADT stationsAdt, size_t * status);

#endif
