#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>

#define NYC 0
#define MON 1
#define MONTHS 12

typedef struct stationByName {
    size_t id;
    char *name;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsMember;
    struct stationByName *tailByName;
} station;

typedef struct stationByTrip{
    char *name;
    size_t quantTripsMember;
    struct stationByTrip* tailByTrip;
};

typedef struct stationMat{
    char *nameA;
    char *nameB;
    size_t quantTripsAtoB;
}stationMat;


struct stationsCDT {
    struct stationByTrip *firstByTrip;
    struct stationByName *firstByName;
    struct stationByTrip *itTrip;
    struct stationByName *itName;
    stationMat *matrix;
    size_t dim;
};

static struct stationByName *addStationRec(struct stationByName* first, size_t id, char *name)
{
    int c;
    if(first == NULL || (c = strcmp(first->name, name)) > 0)
    {
        struct stationByName *aux = calloc(1,sizeof(station));
        aux->id = id;
        aux->name = malloc(strlen(name) + 1); //ver xq no anda sin el malloc!
        strcpy(aux->name, name);
        aux->tailByName = first;
        return aux;
    }
    else if(c < 0)
        first->tailByName = addStationRec(first->tailByName, id, name);

    return first;
}

void addStation(stationsADT stationsAdt, size_t id, char* name) {
    stationsAdt->firstByName = addStationRec(stationsAdt->firstByName, id, name);
    stationsAdt->dim++;
}

void addTrip(struct stationByName* station, char **nameA, char **nameB, size_t* indexA, size_t* indexB, size_t month, size_t fromId, size_t toId, char isMember, char *existsIdFlag){

    char flagA, flagB;
    flagA = flagB = 0;
    size_t i = 0;

    struct stationByName *aux = station;
    while( (!flagA || !flagB) )
    {
        if(aux == NULL) //si llego a un NULL significa que alguno de los 2 id no coincide con alguna estacion.
        {
            *existsIdFlag = 0;
            return;
        }
        if(aux->id == fromId)
        {
            if(isMember){
                aux->quanTripsMember++;
            }
            aux->quanTripsMonth[month - 1]++;
            *nameA = aux->name;
            *indexA = i;
            flagA = 1;
        }
        if(aux->id == toId) {
            *nameB = aux->name;
            *indexB = i;
            flagB = 1;
        }
        aux = aux->tailByName;
        i++;
    }
}

static void addTripAtoB(stationMat *mat, char *nameA, char *nameB ,size_t indexA, size_t indexB, int rowSize)
{
    if((mat + (indexA * rowSize) + indexB)->nameA == NULL){
        (mat + (indexA * rowSize) + indexB)->nameA = malloc(100);
        (mat + (indexA * rowSize) + indexB)->nameB = malloc(100);
        strcpy((mat + (indexA * rowSize) + indexB)->nameA, nameA);
        strcpy((mat + (indexA * rowSize) + indexB)->nameB, nameB);
    }
    (mat + (indexA * rowSize) + indexB)->quantTripsAtoB++;
}

void processEvent(stationsADT stationsAdt, size_t month, size_t fromId, size_t toId, char isMember)
{
    char existsIdFlag = 1;
    char **nameA = malloc(500);
    char **nameB = malloc(500);
    size_t indexA;
    size_t indexB;

    addTrip(stationsAdt->firstByName, nameA, nameB, &indexA, &indexB, month, fromId, toId, isMember, &existsIdFlag);

    if(existsIdFlag && (strcmp(*nameA, *nameB) != 0))
    {
        addTripAtoB(stationsAdt->matrix ,*nameA, *nameB, indexA, indexB, stationsAdt->dim);
    }
    free(nameA);
    free(nameB);
}

void printMatrix(stationsADT stationsAdt){
    for (int i = 0; i < stationsAdt->dim; i++) {
        for (int j = 0; j < stationsAdt->dim; j++) {
            char *auxA = (stationsAdt->matrix + (i * stationsAdt->dim) + j)->nameA;
            char *auxB = (stationsAdt->matrix + (i * stationsAdt->dim) + j)->nameB;
            if((stationsAdt->matrix + (i * stationsAdt->dim) + j)->quantTripsAtoB != 0)
                printf("%s HASTA %s, %d VECES\n",auxA, auxB, (stationsAdt->matrix + (i * stationsAdt->dim) + j)->quantTripsAtoB);
        }
        putchar('\n');
    }
}

void newMat(stationsADT stationsAdt){
    stationMat *aux = calloc(1, (stationsAdt->dim * stationsAdt->dim) * sizeof(stationMat));
    stationsAdt->matrix = aux;
}


stationsADT newStations() {
    return calloc(1, sizeof(struct stationsCDT));
}

static void printStationsRec(struct stationByName * station) {
    if(station == NULL) {
        return;
    }
    printf("nombre: %s \t\tid: %d\n", station->name, station->id);
    printStationsRec(station->tailByName);
}

void printStations(stationsADT stationsAdt) {
    printStationsRec(stationsAdt->firstByName);
}


static void freeStationsRec(struct stationByName *station) {
    if(station == NULL) {
        return;
    }
    freeStationsRec(station->tailByName);
    free(station->name);
    free(station);
}

void freeStations(stationsADT stationsAdt) {
    freeStationsRec(stationsAdt->firstByName);
    //free(stationsAdt->firstByTrip);
    free(stationsAdt->matrix);
    free(stationsAdt);
}

static void printLinksStations(struct stationByName* station)
{
    if(station == NULL){
        return;
    }

    printf("nombre: %s, id: %d viajes por mes: %d\n", station->name, station->id,station->quanTripsMember);

    for (int i = 0; i < MONTHS; i++) {
        printf("viaje mes %d: %d\n", i + 1, station->quanTripsMonth[i]);
    }
    printLinksStations(station->tailByName);
}

void printLinks(stationsADT stationsAdt)
{
    printLinksStations(stationsAdt->firstByName);
}

void toBeginTrip(stationsADT stationsAdt) {
    stationsAdt->itTrip = stationsAdt->firstByTrip;
}

size_t hasNextTrip(stationsADT stationsAdt) {
    return stationsAdt->itTrip->tailByTrip != NULL;
}

size_t nextTrip(stationsADT stationsAdt) {
    size_t c;
    if((c = hasNextTrip(stationsAdt))) {
        stationsAdt->itTrip = stationsAdt->itTrip->tailByTrip;
    }
    return c;
}

void toBeginName(stationsADT stationsAdt) {
	stationsAdt->itName = stationsAdt->firstByName;
}

size_t hasNextName(stationsADT stationsAdt) {
	return stationsAdt->itName->tailByName != NULL;
}

size_t nextName(stationsADT stationsAdt) {
	size_t c;
	if ((c = hasNextName(stationsAdt))){
		stationsAdt->itName = stationsAdt->itName->tailByName;
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

    return stationsAdt->itTrip->quantTripsMember;
}

size_t getTripsByMonth(stationsADT stationsAdt, size_t month, size_t flag) {

        return stationsAdt->itName->quanTripsMonth[month];
}
