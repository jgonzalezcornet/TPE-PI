#include "htmlTable.h"
#include "stationsADT.h"

htmlTable query1(stationsADT stationsAdt) {
    htmlTable table = newTable("query1.csv", 2, "Station", "StartedTrips");
    toBeginTrip(stationsAdt);

    while(hasNextTrip(stationsAdt)) {
        addHTMLRow(table, getName(stationsAdt), getTotalTrips(stationsAdt));
        nextTrip(stationsAdt);
    }

    closeHTMLTable(table);
    return table;
}

