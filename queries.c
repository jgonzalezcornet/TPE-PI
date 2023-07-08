#include "htmlTable.h"
#include "stationsADT.h"

htmlTable query1(stationsADT stationsAdt, FILE *query1) {
    htmlTable table = newTable("query1.html", 2, "Station", "StartedTrips");
    fprintf(query1, "Station;StartedTrips\n");
    toBeginTrip(stationsAdt);

    do{
        char *name = getName(stationsAdt, 0);
        size_t totalMemberTrips = getTotalMemberTrips(stationsAdt, 0);
        addHTMLRow(table, name, totalMemberTrips);
        fprintf(query1, "%s;%zu\n", name, totalMemberTrips);
    } while(hasNextTrip(stationsAdt));

    closeHTMLTable(table);
    fclose(query1);
    return table;
}

htmlTable query2(stationsADT stationsAdt, FILE *query2) {
    htmlTable table = newTable("query2.html", 4, "StationA", "StationB", "Trips A->B", "Trips B->As");
    toBeginName(stationsAdt);

    do {
        
    } while(nextName(stationsAdt));
}

htmlTable query3(stationsADT stationsAdt, FILE *query3) {
    htmlTable table = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    fprintf(query3, "J;F;M;A;M;J;J;A;S;O;N;D;Station");
    toBeginName(stationsAdt);

    size_t count;
    do{
        for(size_t i=0 ; i<MONTHS ; i++) {
            count = getTripsByMonth(stationsAdt, i, 1);
            fprintf(query3, "%zu;", count);
        }
        fprintf(query3, "%s\n", getName(stationsAdt, 1));
    } while(nextName(stationsAdt));

    closeHTMLTable(table);
    fclose(query3);
    return table;
}

