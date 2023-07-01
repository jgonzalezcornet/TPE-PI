#include "htmlTable.h"
#include "stationsADT.h"

htmlTable query1(stationsADT stationsAdt) {
    htmlTable table = newTable("query1.csv", 2, "Station", "StartedTrips");
    
    closeHTMLTable(table);
    return table;
}