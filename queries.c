#include "htmlTable.h"
#include "stationsADT.h"
#include <stdlib.h>
#include "safeMemory.h"

#define MAX_DIGITS 15

static char * intToString(int num, size_t * status) {
    if(num < 0) { // en la query , por si fallo nextTrip
        return NULL;
    }
    char * s = malloc(MAX_DIGITS);
    if (s == NULL){
        *status = 3;  // error de memoria --> va a hacer que el front aborte el programa
    }
    int sz = MAX_DIGITS;
    snprintf(s, sz, "%zu", (size_t)num);
    return s;
}

htmlTable query1(stationsADT stationsAdt, FILE * query1, size_t * status) {
    htmlTable table = newTable("query1.html", 2, "Station", "StartedTrips");
    fprintf(query1, "Station;StartedTrips\n");

    if(getDim(stationsAdt) == 0) { // no hay estaciones
        return table;
    }

    toBeginTrip(stationsAdt, status);

    do {
        char * name = getNameByTrip(stationsAdt, status);
        size_t totalMemberTrips = getTotalTripsByTrip(stationsAdt, status);
        char * totalMemberTripsStr = intToString(totalMemberTrips, status);
        addHTMLRow(table, name, totalMemberTripsStr);
        fprintf(query1, "%s;%zu\n", name, totalMemberTrips);
        if(totalMemberTripsStr != NULL) {
            free(totalMemberTripsStr);
        }
    } while(nextTrip(stationsAdt, status));

    return table;
}

htmlTable query2(stationsADT stationsAdt, FILE * query2, size_t * status) {
    htmlTable table = newTable("query2.html", 4, "StationA", "StationB", "Trips A->B", "Trips B->A");
    fprintf(query2, "StationA;StationB;Trips A->B;Trips B->A\n");

    size_t dim = getDim(stationsAdt); // siendo la matriz de n*n, obtenemos n (cantidad de estaciones)

    if(dim == 0) { // no hay estaciones
        return table;
    }

    for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {
            if(i != j) {                     // pues no hay que considerar los viajes circulares
                int AB = getTripsAtoB(stationsAdt, i, j);
                int BA = getTripsAtoB(stationsAdt, j, i);
                char * nameA = getMatrixName(stationsAdt, i, j);
                char * nameB = getMatrixName(stationsAdt, j, i);
                if(AB > 0) {
                    char *abStr = intToString(AB, status);
                    char *baStr = intToString(BA, status);
                    addHTMLRow(table, nameA, nameB, abStr, baStr); // agregamos la fila al HTML
                    fprintf(query2, "%s;%s;%d;%d\n", nameA, nameB, AB, BA); // imprimimos la fila en el CSV
                    if(abStr != NULL) {
                        free(abStr);
                    }
                    if(baStr != NULL) {
                        free(baStr);
                    }
                }
            }
        }
    }

    return table;
}

htmlTable query3(stationsADT stationsAdt, FILE * query3, size_t * status) {
    htmlTable table = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    fprintf(query3, "J;F;M;A;M;J;J;A;S;O;N;D;Station\n");

    if(getDim(stationsAdt) == 0) { // no hay estaciones
        return table;
    }

    toBeginName(stationsAdt);
    int count;
    do {
        char * countMonth[MONTHS];
        char * name = getNameByName(stationsAdt, status);
        for(size_t i=0 ; i<MONTHS ; i++) {
            count = getTripsByMonth(stationsAdt, i, status);
            countMonth[i] = intToString(count, status);
            fprintf(query3, "%d;", count);
        }
        addHTMLRow(table, countMonth[0], countMonth[1], countMonth[2], countMonth[3], countMonth[4], countMonth[5], countMonth[6], countMonth[7], countMonth[8], countMonth[9], countMonth[10], countMonth[11], name);
        fprintf(query3, "%s\n", name);  // luego de imprimir todos los datos por mes en el CSV, cerramos la linea imprimiendo el nombre\n
        for (int i = 0; i < MONTHS; i++) {
            if(countMonth[i] != NULL) {
                free(countMonth[i]);
            }
        }
    } while(nextName(stationsAdt, status));

    return table;
}

htmlTable query4(stationsADT stationsAdt, FILE * query4, size_t * status) {
    htmlTable table = newTable("query4.html", 2, "Station", "RoundingTrips");
    fprintf(query4, "Station;RoundingTrips\n");

    if(getDim(stationsAdt) == 0) { // no hay estaciones
        return table;
    }

    toBeginRoundTrip(stationsAdt, status);

    do {
        char * name = getNameByRoundTrip(stationsAdt, status);
        int totalRoundTrips = getTotalTripsByRoundTrip(stationsAdt, status);
        char * totalRoundTripsStr = intToString(totalRoundTrips, status);
        addHTMLRow(table, name, totalRoundTripsStr);    // agregamos la fila al HTML
        fprintf(query4, "%s;%d\n", name, totalRoundTrips);  // imprimimos la fila en el CSV
        if(totalRoundTripsStr != NULL) {
            free(totalRoundTripsStr);
        }
    } while(nextRoundTrip(stationsAdt, status));

    return table;
}

htmlTable query5(stationsADT stationsAdt, FILE * query5, size_t firstYear, size_t lastYear, size_t * status) {
    htmlTable table = newTable("query5.html", 4, "Station", "PosAfflux", "NeutralAfflux", "NegativeAfflux");
    fprintf(query5, "Station;PosAfflux;NeutralAfflux;NegativeAfflux\n");

    if(getDim(stationsAdt) == 0) { // no hay estaciones
        return table;
    }

    toBeginName(stationsAdt);

    do {
        char * name = getNameByName(stationsAdt, status);
        int posAfflux;
        int neutralAfflux;
        int negAfflux;
        getAfflux(stationsAdt, firstYear, lastYear, &posAfflux, &neutralAfflux, &negAfflux);
        char * posAffluxStr = intToString(posAfflux, status);
        char * neutralAffluxStr = intToString(neutralAfflux, status);
        char * negAffluxStr = intToString(negAfflux, status);
        addHTMLRow(table, name, posAffluxStr, neutralAffluxStr, negAffluxStr);    // agregamos la fila al HTML
        fprintf(query5, "%s;%d;%d;%d\n", name, posAfflux, neutralAfflux, negAfflux);  // imprimimos la fila en el CSV
        // no chequeamos que estos 3 sean distintos a NULL porque no hay caso posible en el cual sean NULL
        free(posAffluxStr);
        free(neutralAffluxStr);
        free(negAffluxStr);
    } while(nextName(stationsAdt, status));

    return table;
}
