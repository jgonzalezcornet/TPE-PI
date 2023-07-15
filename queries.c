#include "htmlTable.h"
#include "stationsADT.h"
#include "stdlib.h"

#define MAX_DIGITS 10

static char * unsignedIntToString(size_t num) {
    char * s = safeMalloc(MAX_DIGITS);
    int sz = sizeof(s);
    snprintf(s, sz, "%zu", num);
    return s;
}

htmlTable query1(stationsADT stationsAdt, FILE * query1) {
    htmlTable table = newTable("query1.html", 2, "Station", "StartedTrips");    // imprimimos el encabezado en el HTML
    fprintf(query1, "Station;StartedTrips\n");                                  // imprimimos el encabezado en el CSV
    toBeginTrip(stationsAdt);

    do {
        char * name = getName(stationsAdt, 0);
        size_t totalMemberTrips = getTotalTrips(stationsAdt, 0);
        char *totalMemberTripsStr = unsignedIntToString(totalMemberTrips);
        addHTMLRow(table, name, totalMemberTripsStr);    // agregamos la fila al HTML
        fprintf(query1, "%s;%zu\n", name, totalMemberTrips);                  // imprimimos la fila en el CSV
        free(totalMemberTripsStr);
    } while(nextTrip(stationsAdt));

    return table;
}

htmlTable query2(stationsADT stationsAdt, FILE * query2) {    
    htmlTable table = newTable("query2.html", 4, "StationA", "StationB", "Trips A->B", "Trips B->A");    // imprimimos el encabezado en el HTML
    fprintf(query2, "StationA;StationB;Trips A->B;Trips B->A\n");                                          // imprimimos el encabezado en el CSV

    size_t dim = getDim(stationsAdt);        // siendo la matriz de n*n, obtenemos n (cantidad de estaciones)

    for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {
            if(i != j) {                     // pues no hay que considerar los viajes circulares
                size_t AB = getTripsAtoB(stationsAdt, i, j);
                size_t BA = getTripsAtoB(stationsAdt, j, i);
                char * nameA = getMatrixName(stationsAdt, i, j);
                char * nameB = getMatrixName(stationsAdt, j, i);
                if(AB > 0) {
                    char *abStr = unsignedIntToString(AB);
                    char *baStr = unsignedIntToString(BA);
                    addHTMLRow(table, nameA, nameB, abStr, baStr);        // agregamos la fila al HTML
                    fprintf(query2, "%s;%s;%zu;%zu\n", nameA, nameB, AB, BA);       // imprimimos la fila en el CSV
                    free(abStr);
                    free(baStr);
                }
            }
        }
    }

    return table;
}

htmlTable query3(stationsADT stationsAdt, FILE * query3) {
    htmlTable table = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");    // imprimimos el encabezado en el HTML
    fprintf(query3, "J;F;M;A;M;J;J;A;S;O;N;D;Station\n");                                                                      // imprimimos el encabezado en el CSV
    toBeginName(stationsAdt);
    size_t count;
    do {
        char * countMonth[MONTHS];
        char * name = getName(stationsAdt, 1);
        for(size_t i=0 ; i<MONTHS ; i++) {
            count = getTripsByMonth(stationsAdt, i);
            countMonth[i] = unsignedIntToString(count);
            fprintf(query3, "%zu;", count);
        }
        addHTMLRow(table, countMonth[0], countMonth[1], countMonth[2], countMonth[3], countMonth[4], countMonth[5], countMonth[6], countMonth[7], countMonth[8], countMonth[9], countMonth[10], countMonth[11], name);
        fprintf(query3, "%s\n", name);           // luego de imprimir todos los datos por mes en el CSV, cerramos la linea imprimiendo el nombre\n
        for (int i = 0; i < MONTHS; i++) {
            free(countMonth[i]);
        }
    } while(nextName(stationsAdt));

    return table;
}

htmlTable query4(stationsADT stationsAdt, FILE * query4) {
    htmlTable table = newTable("query4.html", 2, "Station", "RoundingTrips");
    fprintf(query4, "Station;RoundingTrips\n");
    toBeginRoundTrip(stationsAdt);

    do {
        char * name = getName(stationsAdt, 2);
        size_t totalRoundTrips = getTotalTrips(stationsAdt, 2);
        char * totalRoundTripsStr = unsignedIntToString(totalRoundTrips);
        addHTMLRow(table, name, totalRoundTripsStr);    // agregamos la fila al HTML
        fprintf(query4, "%s;%zu\n", name, totalRoundTrips);  // imprimimos la fila en el CSV
        free(totalRoundTripsStr);

    } while(nextRoundTrip(stationsAdt));

    return table;
}

