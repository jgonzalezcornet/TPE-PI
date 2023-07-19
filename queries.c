#include "htmlTable.h"
#include "stationsADT.h"
#include <stdlib.h>
#include "safeMemory.h"

#define MAX_DIGITS 10

static char * intToString(int num) { // !!!!! chequear si esto esta bien asi, hacemos malloc de un tamaño fijo, puede sobrar o faltar espacio
    if(num < 0) { // fallo nextTrip
        return NULL;
    }
    char * s = safeMalloc(MAX_DIGITS);
    int sz = sizeof(s);
    snprintf(s, sz, "%d", num);
    return s;
}

htmlTable query1(stationsADT stationsAdt, FILE * query1) {
    htmlTable table = newTable("query1.html", 2, "Station", "StartedTrips");
    fprintf(query1, "Station;StartedTrips\n");
    toBeginTrip(stationsAdt);

    do {
        char * name = getNameByTrip(stationsAdt);
        size_t totalMemberTrips = getTotalTripsByTrip(stationsAdt);
        char * totalMemberTripsStr = intToString(totalMemberTrips);
        addHTMLRow(table, name, totalMemberTripsStr);
        fprintf(query1, "%s;%zu\n", name, totalMemberTrips);
        if(totalMemberTripsStr != NULL) {
            free(totalMemberTripsStr);
        }
    } while(nextTrip(stationsAdt));

    return table;
}

htmlTable query2(stationsADT stationsAdt, FILE * query2) {    
    htmlTable table = newTable("query2.html", 4, "StationA", "StationB", "Trips A->B", "Trips B->A");
    fprintf(query2, "StationA;StationB;Trips A->B;Trips B->A\n");

    size_t dim = getDim(stationsAdt); // siendo la matriz de n*n, obtenemos n (cantidad de estaciones)

    for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {
            if(i != j) {                     // pues no hay que considerar los viajes circulares
                int AB = getTripsAtoB(stationsAdt, i, j);
                int BA = getTripsAtoB(stationsAdt, j, i);
                char * nameA = getMatrixName(stationsAdt, i, j);
                char * nameB = getMatrixName(stationsAdt, j, i);
                if(AB > 0) {
                    char *abStr = intToString(AB);
                    char *baStr = intToString(BA);
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

htmlTable query3(stationsADT stationsAdt, FILE * query3) {
    htmlTable table = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    fprintf(query3, "J;F;M;A;M;J;J;A;S;O;N;D;Station\n");
    toBeginName(stationsAdt);
    int count;
    do {
        char * countMonth[MONTHS];
        char * name = getNameByName(stationsAdt);
        for(size_t i=0 ; i<MONTHS ; i++) {
            count = getTripsByMonth(stationsAdt, i);
            countMonth[i] = intToString(count);
            fprintf(query3, "%d;", count);
        }
        addHTMLRow(table, countMonth[0], countMonth[1], countMonth[2], countMonth[3], countMonth[4], countMonth[5], countMonth[6], countMonth[7], countMonth[8], countMonth[9], countMonth[10], countMonth[11], name);
        fprintf(query3, "%s\n", name);  // luego de imprimir todos los datos por mes en el CSV, cerramos la linea imprimiendo el nombre\n
        for (int i = 0; i < MONTHS; i++) {
            if(countMonth[i] != NULL) {
                free(countMonth[i]);
            }
        }
    } while(nextName(stationsAdt));

    return table;
}

htmlTable query4(stationsADT stationsAdt, FILE * query4) {
    htmlTable table = newTable("query4.html", 2, "Station", "RoundingTrips");
    fprintf(query4, "Station;RoundingTrips\n");
    toBeginRoundTrip(stationsAdt);

    do {
        char * name = getNameByRoundTrip(stationsAdt);
        int totalRoundTrips = getTotalTripsByRoundTrip(stationsAdt);
        char * totalRoundTripsStr = intToString(totalRoundTrips);
        addHTMLRow(table, name, totalRoundTripsStr);    // agregamos la fila al HTML
        fprintf(query4, "%s;%d\n", name, totalRoundTrips);  // imprimimos la fila en el CSV
        if(totalRoundTripsStr != NULL) {
            free(totalRoundTripsStr);
        }
    } while(nextRoundTrip(stationsAdt));

    return table;
}

htmlTable query5(stationsADT stationsAdt, FILE * query5, size_t firstYear, size_t lastYear) {
    htmlTable table = newTable("query5.html", 4, "Station", "PosAfflux", "NeutralAfflux", "NegativeAfflux");
    fprintf(query5, "Station;PosAfflux;NeutralAfflux;NegativeAfflux\n");
    toBeginName(stationsAdt);

    do {
        char * name = getNameByName(stationsAdt);
        int posAfflux;
        int neutralAfflux;
        int negAfflux;
        getAfflux(stationsAdt, firstYear, lastYear, &posAfflux, &neutralAfflux, &negAfflux);
        char * posAffluxStr = intToString(posAfflux);
        char * neutralAffluxStr = intToString(neutralAfflux);
        char * negAffluxStr = intToString(negAfflux);
        addHTMLRow(table, name, posAffluxStr, neutralAffluxStr, negAffluxStr);    // agregamos la fila al HTML
        fprintf(query5, "%s;%d;%d;%d\n", name, posAfflux, neutralAfflux, negAfflux);  // imprimimos la fila en el CSV
        free(posAffluxStr);
        free(neutralAffluxStr);
        free(negAffluxStr);
    } while(nextName(stationsAdt));

    return table;
}
