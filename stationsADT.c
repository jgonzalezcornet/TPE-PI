#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

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
    size_t quanTripsMember;
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

static station *addStationRec(station* first, size_t id, char *name) {
    int c;
    if(first == NULL || (c = strcmp(first->name, name)) > 0) {
        struct station *aux = calloc(1, sizeof(station));
        aux->id = id;
        aux->name = malloc(strlen(name) + 1); //ver xq no anda sin el malloc!
        strcpy(aux->name, name);
        aux->tailName = first;
        return aux;
    } else if(c < 0){
        first->tailName = addStationRec(first->tailName, id, name);
    }
    return first;
}

void addTrip(struct station* station, char **nameA, char **nameB, size_t month, size_t fromId, size_t toId, char isMember){
    char flagA, flagB;
    flagA = flagB = 0;

    struct station *aux = station;
    while(!flagA || !flagB) {
        if(aux->id == fromId) {
            if(isMember){
                aux->quanTripsMember++;
            }
            aux->quanTripsMonth[month - 1]++;
            *nameA = aux->name;
            flagA = 1;
        }
        if(aux->id == toId) {
            *nameB = aux->name;
            flagB = 1;
        }
        aux = aux->tailName;
    }
}

static struct subStations *addTripAtoBRec( struct subStations* first, char *name, char flagIsBtoA) {
    int c;
    if(first == NULL || (c = strcmp(first->name, name)) > 0) {
        struct subStations *aux = calloc(1,sizeof(station));
        aux->name = name;
        if(flagIsBtoA) {
            aux->BtoA++;
        } else {
            aux->AtoB++;
        }
        aux->tailSubStations = first;
        return aux;
    }
    if(c < 0) {
        first->tailSubStations = addTripAtoBRec(first->tailSubStations, name, flagIsBtoA);
    }
    return first;
}

static void addTripAtoB(stationsADT stationsAdt, char *nameA, char *nameB, char flagIsBtoA) {
    struct station* aux = stationsAdt->firstName;
    while (aux != NULL) {
       if(strcmp(aux->name, nameA) == 0) {
           aux->firstSubstation = addTripAtoBRec(aux->firstSubstation, nameB, flagIsBtoA);
           return;
       }
       aux = aux->tailName;
    }
}

void processEvent(stationsADT stationsAdt, size_t month, size_t fromId, size_t toId, char isMember) {
    char **nameA = malloc(500);
    char **nameB = malloc(500);
    char **nameAux;
    char flagIsBtoA = 0;

    addTrip(stationsAdt->firstName, nameA, nameB, month, fromId, toId, isMember);

    if(strcmp(*nameA,*nameB) > 0) {
        nameAux = nameA;
        nameA = nameB;
        nameB = nameAux;
        flagIsBtoA = 1;
    }
    if(strcmp(*nameA, *nameB) != 0) {
        addTripAtoB(stationsAdt ,*nameA, *nameB, flagIsBtoA);
    }

    free(nameA);
    free(nameB);
}

void addStation(stationsADT stationsAdt, size_t id, char* name) {
    stationsAdt->firstName = addStationRec(stationsAdt->firstName, id, name);
    stationsAdt->dim++;
}

stationsADT newStations() {
    return calloc(1, sizeof(struct stationsCDT));
}

static void printStationsRec(station * station) {
    if(station == NULL) {
        return;
    }
    printf("nombre: %s \t\tid: %d\n", station->name, station->id);
    printStationsRec(station->tailName);
}

void printStations(stationsADT stationsAdt) {
    printStationsRec(stationsAdt->firstName);
}

static void freeSubStationsRec(subStations *subStation) {
    if(subStation == NULL) {
        return;
    }
    freeSubStationsRec(subStation->tailSubStations);
    free(subStation);
}

static void freeStationsRec(station *station) {
    if(station == NULL) {
        return;
    }

    freeStationsRec(station->tailName);
    freeSubStationsRec(station->firstSubstation);
    free(station->name);
    free(station);
}

void freeStations(stationsADT stationsAdt) {
    freeStationsRec(stationsAdt->firstName);
    free(stationsAdt);
}

static void printLinksStations(struct station* station) {
    if(station == NULL) {
        return;
    }

    printf("nombre: %s, id: %d viajes por mes: %d\n", station->name, station->id,station->quanTripsMember);
    
    for (int i = 0; i < MONTHS; i++) {
        printf("viaje mes %d: %d\n",i+1,station->quanTripsMonth[i]);
    }

    printLinksStations(station->tailName);
}

void printLinks(stationsADT stationsAdt) {
    printLinksStations(stationsAdt->firstName);
}

static void printSubStationsRec(struct subStations* subStation) {
    if(subStation == NULL) {
        return;
    }
    printf("nombre: %s,\t\t\t\t\t\t\t\t AtoB:%d, BtoA:%d\n",subStation->name, subStation->AtoB, subStation->BtoA);
    printSubStationsRec(subStation->tailSubStations);
}

static void printSubStations2(struct station* station) {
    if(station == NULL) {
        return;
    }
    printf("ESTA SON LAS SUBESTACIONES DE %s\n",station->name);
    printSubStationsRec(station->firstSubstation);
    printSubStations2(station->tailName);
}

void printSubStations(stationsADT stationsAdt) {
    printSubStations2(stationsAdt->firstName);
}

void toBeginTrip(stationsADT stationsAdt) {
    stationsAdt->itTrip = stationsAdt->firstTrip;
}

size_t hasNextTrip(stationsADT stationsAdt) {
    return stationsAdt->itTrip->tailTrip != NULL;
}

size_t nextTrip(stationsADT stationsAdt) {
    size_t c;
    if((c = hasNextTrip(stationsAdt))) {
        stationsAdt->itTrip = stationsAdt->itTrip->tailTrip;
    }
    return c;
}

void toBeginName(stationsADT stationsAdt) {
	stationsAdt->itName = stationsAdt->firstName;
}

size_t hasNextName(stationsADT stationsAdt) {
	return stationsAdt->itName->tailName != NULL;
}

size_t nextName(stationsADT stationsAdt) {
	size_t c;
	if((c = hasNextName(stationsAdt))) {
		stationsAdt->itName = stationsAdt->itName->tailName;
	}
	return c;
}

char *getName(stationsADT stationsAdt, size_t flag) { // flag = 1 means check name it, flag = 0 means check trip it
    if(flag) {
        return stationsAdt->itName->name;
    }

    return stationsAdt->itTrip->name;
}

size_t getTotalMemberTrips(stationsADT stationsAdt, size_t flag) {
    if(flag) {
        return stationsAdt->itName->quanTripsMember;
    }

    return stationsAdt->itTrip->quanTripsMember;
}

size_t getTripsByMonth(stationsADT stationsAdt, size_t month, size_t flag) {
    if(flag) {
        return stationsAdt->itName->quanTripsMonth[month]; 
    }

    return stationsAdt->itTrip->quanTripsMonth[month];
}
