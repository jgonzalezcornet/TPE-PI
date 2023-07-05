#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

#define NYC 0
#define MON 1
#define MONTHS 12

typedef struct subStations {
    size_t id;
    char *name;
    size_t AtoB;
    size_t BtoA;
    struct subStations *tailSubStations;
} subStations;

typedef struct station {
    size_t id;
    char *name;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsTotal;
    struct station *tailTrip;
    struct station *tailName;
    struct subStations *firstSubstation;
} station;

struct stationsCDT {
    station *firstTrip;
    station *firstName;
    station *itTrip;
    station *itName;
    size_t dim;
};

void addStation(stationsADT stationsAdt, size_t id, char* name) {
    
}

stationsADT newStations() {
    return calloc(1, sizeof(struct stationsCDT));
}

static void printStationsRec(station * station) {
    if(station == NULL) {
        return;
    }
    printf("nombre: %s id: %d\n", station->name, station->id);
    printStationsRec(station->tailName);
}

void printStations(stationsADT stationsAdt) {
    printStationsRec(stationsAdt->firstName);
}

static void freeStationsRec(station *station) {
    if(station == NULL) {
        return;
    }
    freeStationsRec(station->tailName);
    free(station->name);
    free(station);
}

void freeStations(stationsADT stationsAdt) {
    freeStationsRec(stationsAdt->firstName);
    free(stationsAdt);
}

void toBeginTrip(stationsADT stationsAdt) {
    stationsAdt->itTrip = stationsAdt->firstTrip;
}

size_t hasNextTrip(stationsADT stationsAdt) {
    return stationsAdt->itTrip->tailTrip != NULL;
}

size_t nextTrip(stationsADT stationsAdt) {
    size_t c;
    if(c = hasNextTrip(stationsAdt)) {
        stationsAdt->itTrip = stationsAdt->itTrip->tailTrip;
    }
    return c;
}
