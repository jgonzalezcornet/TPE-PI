/*
    Archivo: queries.h
    ------------------
    Este archivo se encarga de la correcta
    ejecucion de las queries.
*/


#ifndef _queries_h_
#define _queries_h_
#include "htmlTable.h"
#include "stationsADT.h"

/*
    Funcion: query1
    Uso: query1(stations, file);
    ----------------------------
    Esta funcion devuelve, en orden descendente por
    cantidad de viajes, el nombre de cada estacion
    y la cantidad de viajes iniciados en esa estacion
    por usuarios miembros. 
*/

htmlTable query1(stationsADT stationsAdt, FILE * query1, size_t * status);

/*
    Funcion: query2
    Uso: query2(stations, file);
    ----------------------------
    Esta funcion devuelve, para todo par
    de estaciones A y B, el nombre de ambas estaciones,
    la cantidad de viajes de A a B y la cantidad de
    viajes de B a A, ordenando alfabeticamente por
    el nombre de A y desempatando por el de B.
*/

htmlTable query2(stationsADT stationsAdt, FILE * query2, size_t * status);

/*
    Funcion: query3
    Uso: query3(stations, file);
    ----------------------------
    Esta funcion devuelve para cada estacion,
    la cantidad total de viajes iniciados en los meses
    en todos los años. La informacion es listada
    en orden alfabetico por nombre de la estacion.
*/

htmlTable query3(stationsADT stationsAdt, FILE * query3, size_t * status);

/*
    Funcion: query4
    Uso: query4(stations, file);
    ----------------------------
    Esta funcion devuelve para cada estacion,
    la cantidad total de viajes circulares entre
    los años recibidos como argumentos por línea
    de comando. Orden descendente por cantidad
    de viajes y desempatado alfabeticamente.
*/

htmlTable query4(stationsADT stationsAdt, FILE * query4, size_t * status);

/*
    Funcion: query5
    Uso: query5(stations, file, firstYear, lastYear);
    -------------------------------------------------
    Esta funcion devuelve para cada estacion,
    la cantidad total de dias (entre los años
    recibidos como argumentos por línea de
    comando) con afluencia positiva, neutral,
    y negativa. Orden alfabetico.
*/

htmlTable query5(stationsADT stationsAdt, FILE * query5, size_t firstYear, size_t lastYear, size_t * status);

#endif
