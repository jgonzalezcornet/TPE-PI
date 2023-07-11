#include "htmlTable.h"
#include "stationsADT.h"

#define MAX_DIGITS 10

static char * unsignedIntToString(size_t num) {
    char * s = safeMalloc(MAX_DIGITS);
    snprintf(s, sizeof(s), "%zu", num);
    return s;
}

htmlTable query1(stationsADT stationsAdt, FILE * query1) {
    htmlTable table = newTable("query1.html", 2, "Station", "StartedTrips");    // imprimimos el encabezado en el HTML
    fprintf(query1, "Station;StartedTrips\n");                                  // imprimimos el encabezado en el CSV
    toBeginTrip(stationsAdt);

    do {
        char * name = getName(stationsAdt, 0);
        size_t totalMemberTrips = getTotalMemberTrips(stationsAdt, 0);
        addHTMLRow(table, name, unsignedIntToString(totalMemberTrips));    // agregamos la fila al HTML
        fprintf(query1, "%s;%zu\n", name, totalMemberTrips);                  // imprimimos la fila en el CSV
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
                    addHTMLRow(table, nameA, nameB, unsignedIntToString(AB), unsignedIntToString(BA));        // agregamos la fila al HTML
                    fprintf(query2, "%s;%s;%zu;%zu\n", nameA, nameB, AB, BA);                                 // imprimimos la fila en el CSV
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
    } while(nextName(stationsAdt));

    return table;
}

