#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include "safeMemory.h"

typedef int (*compFn)(const void *, const void *);

#define MAX_LEN 100
#define DAYS_IN_YEAR 365

typedef struct stationByName {
    size_t id;
    size_t alfaId;
    char * name;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsMember;
    size_t quanRoundTrips;
    struct affluxByYear * affluxByYear;
    struct stationByName * tailByName;
} stationByName;

typedef struct stationByTrip {
    char * name;
    size_t quanTrips;
    struct stationByTrip * tailByTrip;
} stationByTrip;

typedef struct stationMat {
    stationByName * station;
    size_t quanTripsAtoB;
} stationMat;

typedef struct affluxByYear{
    size_t year;
    int ** affluxPerDay;  
    struct affluxByYear * tailByYear;
}affluxByYear;

struct stationsCDT {
    size_t firstYear;
    size_t lastYear;
    struct stationByTrip * firstByTrip;
    struct stationByName * firstByName;
    struct stationByTrip * firstByRoundTrip;
    struct stationByTrip * itTrip;
    struct stationByName * itName;
    struct stationByTrip * itRoundTrip;
    stationMat ** matrix;
    struct stationByName ** orderedIds;
    size_t dim;
};

/* ---- Funciones para el vector de punteros a estaciones ----- */

int compareStations(struct stationByName ** v1, struct stationByName ** v2) {
    size_t s1Id = (*v1)->id;
    size_t s2Id = (*v2)->id;

    if(s1Id < s2Id) {
        return -1;
    } else if(s1Id > s2Id) {
        return 1;
    }
    return 0;
}

static void traverseListToFillArray(stationByName * first, size_t i, struct stationByName ** ids) {
    if(first == NULL) {
        return;
    }
    first->alfaId = i;
    ids[i] = first;
    traverseListToFillArray(first->tailByName, i + 1, ids);
}

void fillOrderedIds(stationsADT stationsAdt, size_t * status) {
    stationsAdt->orderedIds = safeMalloc(stationsAdt->dim * sizeof(struct stationByName *), status, stationsAdt);
    traverseListToFillArray(stationsAdt->firstByName, 0, stationsAdt->orderedIds);
    qsort(stationsAdt->orderedIds, stationsAdt->dim, sizeof(stationByName *), (compFn)compareStations); // casteo el comparador a compFn
}

static struct stationByName * getStationById(stationsADT stationsAdt, size_t id) {
    if (stationsAdt->dim == 0){
        return NULL;
    }
    int low = 0, high = stationsAdt->dim;

    while(high - low > 1) {
        int mid = (low + high) / 2;
        if(stationsAdt->orderedIds[mid]->id <= id) {
            low = mid;
        } else {
            high = mid;
        }
    }

    if(stationsAdt->orderedIds[low]->id != id) {
        return NULL;
    }
    return stationsAdt->orderedIds[low];
}

/* ----- Funciones para resolver el procesamiento de datos a un TAD ----- */

static struct stationByName * addStationRec(stationsADT stationsAdt, stationByName * first, size_t id, char * name, size_t * newFlag, size_t * status) {
    int c;
    if(first == NULL || (c = strcasecmp(first->name, name)) > 0) {
        stationByName * aux = safeCalloc(1, sizeof(stationByName), status, stationsAdt);
        aux->id = id;
        aux->name = safeMalloc(strlen(name) + 1, status, stationsAdt);
        strcpy(aux->name, name);
        aux->tailByName = first;
        *newFlag = 1;
        return aux;
    } else if(c < 0) {
        first->tailByName = addStationRec(stationsAdt, first->tailByName, id, name, newFlag, status);
    }
    return first;
}

void addStation(stationsADT stationsAdt, size_t id, char * name, size_t * status) {
    size_t newFlag = 0;
    stationsAdt->firstByName = addStationRec(stationsAdt, stationsAdt->firstByName, id, name, &newFlag, status);
    stationsAdt->dim += newFlag;
}

static void addTripAtoB(stationMat ** mat, stationByName * stationA, stationByName * stationB, size_t indexA, size_t indexB) {
    if(mat[indexA][indexB].station == NULL) {
        mat[indexA][indexB].station = stationA;
    }
    if(mat[indexB][indexA].station == NULL){
        mat[indexB][indexA].station = stationB;
    }
    mat[indexA][indexB].quanTripsAtoB++;
}

static int ** newMatPerYear(stationsADT stationsAdt, size_t * daysPerMonth, size_t * status){
    int ** aux = safeMalloc(MONTHS * sizeof(int *), status, stationsAdt);
    for(size_t i = 0; i < MONTHS; i++){
        aux[i] = safeCalloc(1, daysPerMonth[i] * sizeof(int), status, stationsAdt); //todos los dias arrancan en 0
    }
    return aux;
}

static struct affluxByYear * addDayRec(stationsADT stationsAdt, struct affluxByYear * first, size_t year, size_t month, size_t day, int state, size_t * status){
    if(first == NULL || first->year > year){
        struct affluxByYear * aux = safeMalloc(sizeof(struct affluxByYear), status, stationsAdt);
        size_t daysPerMonth[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        aux->affluxPerDay = newMatPerYear(stationsAdt, daysPerMonth, status);
        aux->affluxPerDay[month-1][day-1] += state;
        aux->year = year;
        aux->tailByYear = first;
        return aux;
    }
    else if(first->year < year){
        first->tailByYear = addDayRec(stationsAdt, first->tailByYear, year, month, day, state, status);
    }
    else{
        first->affluxPerDay[month-1][day-1] += state;
    }
    return first;
}

void processEvent(stationsADT stationsAdt, size_t year, size_t month, size_t day, size_t fromId, size_t toId, char isMember, char isRange, size_t * status) {
    
    static int flag = 0; //para que se ejecute solamente la primera vez
    if(!flag){
        fillOrderedIds(stationsAdt, status);
        newMat(stationsAdt, status);
        flag = 1;
    }
    
    stationByName * statFrom = getStationById(stationsAdt, fromId);
    stationByName * statTo = getStationById(stationsAdt, toId);

    static size_t shortestYear = MAX_YEAR;
    static size_t biggestYear = 0;

    if(!isRange){
        if(year < shortestYear){
            shortestYear = year;
            stationsAdt->firstYear = year;
        }
        if(year > biggestYear){
            biggestYear = year;
            stationsAdt->lastYear = year;
        }
    }

    size_t flagA, flagB;
    flagA = 0;
    flagB = 0;
    size_t indexA;
    size_t indexB;

    if(statFrom != NULL) {
        if(isMember) {
            statFrom->quanTripsMember++;
        }
	    statFrom->quanTripsMonth[month - 1]++;
	    flagA = 1;
        indexA = statFrom->alfaId;
    }

    if(statTo != NULL) {
        flagB = 1;
        indexB = statTo->alfaId;
    }

    if(flagA && flagB) {
        if(stationsAdt->firstYear <= year && year <= stationsAdt->lastYear) {
            statFrom->affluxByYear = addDayRec(stationsAdt, statFrom->affluxByYear, year, month, day, -1, status);
            statTo->affluxByYear = addDayRec(stationsAdt, statTo->affluxByYear, year, month, day, 1, status);
            if(indexA == indexB){
                statFrom->quanRoundTrips++;
            }
        }
        if(indexA != indexB){
            addTripAtoB(stationsAdt->matrix , statFrom, statTo, indexA, indexB);
        }    
    }
    
}

void newMat(stationsADT stationsAdt, size_t * status) {
    size_t dim = stationsAdt->dim;
    stationMat ** aux = safeMalloc(dim * sizeof(stationMat *), status, stationsAdt);
    for(size_t i = 0; i < dim; i++) {
        aux[i] = safeCalloc(1, dim * sizeof(stationMat), status, stationsAdt);
    }
    stationsAdt->matrix = aux;
}

stationsADT newStations(size_t firstYear, size_t lastYear, size_t * status) {
    stationsADT aux = safeCalloc(1, sizeof(struct stationsCDT), status, NULL);
    if (aux != NULL){
        aux->dim = 0;
        aux->firstYear = firstYear;
        aux->lastYear = lastYear;
    }
    return aux;
}

/* ----- Funciones para crear la lista ordenada por viajes ----- */

static stationByTrip * createStationByTripNode(stationsADT stationsAdt, char * name, size_t quanTrips, size_t * status) {
	stationByTrip * newNode = safeMalloc(sizeof(stationByTrip), status, stationsAdt);
    if (newNode != NULL){
        newNode->name = safeMalloc(strlen(name) + 1, status, stationsAdt);
        strcpy(newNode->name, name);
        newNode->quanTrips = quanTrips;
        newNode->tailByTrip = NULL;
    }
	return newNode;
}

//flag == 0 -> firstByTrip    flag == 1 -> firstByRoundTrip
static void insertByTrip(stationsADT stationsAdt, stationByTrip * newNode, size_t flag) {
    stationByTrip * current = (flag == 0 ? stationsAdt->firstByTrip : stationsAdt->firstByRoundTrip );
	if(current == NULL || newNode->quanTrips > current->quanTrips || (newNode->quanTrips == current->quanTrips && strcasecmp(newNode->name, current->name) < 0)) {
		newNode->tailByTrip = current;
        if(flag == 0) {
            stationsAdt->firstByTrip = newNode;
        } else {
            stationsAdt->firstByRoundTrip = newNode;
        }
	} else {
        while(current->tailByTrip != NULL && (current->tailByTrip->quanTrips > newNode->quanTrips || (current->tailByTrip->quanTrips == newNode->quanTrips && strcasecmp(current->tailByTrip->name, newNode->name) < 0))) {
		    current = current->tailByTrip;
		}
		newNode->tailByTrip = current->tailByTrip;
		current->tailByTrip = newNode;
	}
}

//flag == 0 -> firstByTrip    flag == 1 -> firstByRoundTrip
void rearrangeByTrip(stationsADT stationsAdt, size_t flag, size_t * status) {
	stationByName * current = stationsAdt->firstByName;
	while(current != NULL) {
        stationByTrip * newNode = createStationByTripNode(stationsAdt, current->name, (flag == 0 ? current->quanTripsMember : current->quanRoundTrips), status);
		insertByTrip(stationsAdt, newNode, flag);
		current = current->tailByName;
	}
}

/* ----- Funciones de iteración por viajes ----- */

void toBeginTrip(stationsADT stationsAdt, size_t * status) {
    rearrangeByTrip(stationsAdt, 0, status); // crea la lista ordenada por trips
    stationsAdt->itTrip = stationsAdt->firstByTrip;
}

int hasNextTrip(stationsADT stationsAdt, size_t * status) {
    if (stationsAdt->itTrip != NULL){
        return stationsAdt->itTrip->tailByTrip != NULL;
    }
    *status = 1;    // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return -1;
}

int nextTrip(stationsADT stationsAdt, size_t * status) {
    size_t c;
    if((c = hasNextTrip(stationsAdt, status))) {
        stationsAdt->itTrip = stationsAdt->itTrip->tailByTrip;
    }
    return c;
}

/* ----- Funciones de iteración por viajes circulares ----- */

void toBeginRoundTrip(stationsADT stationsAdt, size_t * status) {
    rearrangeByTrip(stationsAdt, 1, status); // crea la lista ordenada por trips circulares
    stationsAdt->itRoundTrip = stationsAdt->firstByRoundTrip;
}

int hasNextRoundTrip(stationsADT stationsAdt, size_t * status) {
    if (stationsAdt->itRoundTrip != NULL) {
        return stationsAdt->itRoundTrip->tailByTrip != NULL;
    }
    *status = 1; // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return -1;
}

int nextRoundTrip(stationsADT stationsAdt, size_t * status) {
    size_t c;
    if((c = hasNextRoundTrip(stationsAdt, status))) {
        stationsAdt->itRoundTrip = stationsAdt->itRoundTrip->tailByTrip;
    }
    return c;
}

/* ----- Funciones de iteración por nombre ----- */

void toBeginName(stationsADT stationsAdt) {
    stationsAdt->itName = stationsAdt->firstByName;
}

int hasNextName(stationsADT stationsAdt, size_t * status) {
    if(stationsAdt->itName != NULL) {
        return stationsAdt->itName->tailByName != NULL;
    }
    *status = 1; // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return -1;
}

int nextName(stationsADT stationsAdt, size_t * status) {
	size_t c;
	if((c = hasNextName(stationsAdt, status))) {
		stationsAdt->itName = stationsAdt->itName->tailByName;
	}
	return c;
}

/* ----- Funciones para obtener datos del Adt desde queries.c ----- */
char * getNameByName(stationsADT stationsAdt, size_t * status) {
    if(stationsAdt->itName != NULL) {
        return stationsAdt->itName->name;
    }
    *status = 1; // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return NULL;
}

char * getNameByTrip(stationsADT stationsAdt, size_t * status) {
    if(stationsAdt->itTrip != NULL) {
        return stationsAdt->itTrip->name;
    }
    *status = 1; // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return NULL;
}

char * getNameByRoundTrip(stationsADT stationsAdt, size_t * status) {
    if(stationsAdt->itRoundTrip != NULL) {
        return stationsAdt->itRoundTrip->name;
    }
    *status = 1; // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return NULL;
}

int getTotalTripsByTrip(stationsADT stationsAdt, size_t * status) {
    if(stationsAdt->itTrip != NULL) {
        return stationsAdt->itTrip->quanTrips;
    }
    *status = 1; // marcamos que hubo un error durante el programa para explicar los malos resultados pero no abortamos
    return -1;
}

int getTotalTripsByRoundTrip(stationsADT stationsAdt, size_t * status) {
    if(stationsAdt->itRoundTrip != NULL) {
        return stationsAdt->itRoundTrip->quanTrips;
    }
    *status = 1;
    return -1;
}

size_t getTripsByMonth(stationsADT stationsAdt, size_t month, size_t * status) {
    if(month <= 11) {
        return stationsAdt->itName->quanTripsMonth[month];
    }
    *status = 1;
    return -1;
}

char * getMatrixName(stationsADT stationsAdt, size_t indexA, size_t indexB) {
    if(indexA < stationsAdt->dim && indexB < stationsAdt->dim && stationsAdt->matrix != NULL) {
        if(stationsAdt->matrix[indexA][indexB].station != NULL){
            return stationsAdt->matrix[indexA][indexB].station->name;
        }
    }
    return NULL;
}

int getTripsAtoB(stationsADT stationsAdt, size_t indexA, size_t indexB) {
    if(indexA < stationsAdt->dim && indexB < stationsAdt->dim && stationsAdt->matrix != NULL) {
        return stationsAdt->matrix[indexA][indexB].quanTripsAtoB;
    }
    return -1;
}

size_t getDim(stationsADT stationsAdt) {
	return stationsAdt->dim;
}

static void getAffluxInMat(int ** matrix, int * posAfflux, int * neutralAfflux, int * negAfflux) {
    size_t daysPerMonth[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for(size_t i = 0; i < MONTHS; i++) {
        for(size_t j = 0; j < daysPerMonth[i]; j++) {
            if(matrix[i][j] > 0) {
                (*posAfflux)++;
            } else if(matrix[i][j] < 0) {
                (*negAfflux)++;
            } else {
                (*neutralAfflux)++;
            }
        }
    }
}

void getAfflux(stationsADT stationsAdt, size_t firstYear, size_t lastYear, int * posAfflux, int * neutralAfflux, int * negAfflux) {
    *posAfflux = 0;
    *neutralAfflux = 0;
    *negAfflux = 0;
    struct affluxByYear * aux = stationsAdt->itName->affluxByYear;
    if(aux == NULL) {
        *neutralAfflux = DAYS_IN_YEAR * (lastYear - firstYear + 1);
        return;
    }
    size_t iterYear = firstYear;
    while(iterYear <= lastYear) {
        if(aux == NULL) {  //no hay mas años por recorrer
            *neutralAfflux += DAYS_IN_YEAR * (lastYear - iterYear + 1);
            return;
        } else if(aux->year == iterYear) {  //el año coincide
            getAffluxInMat(aux->affluxPerDay, posAfflux, neutralAfflux, negAfflux);
            aux = aux->tailByYear;
            iterYear++;
        } else if(aux->year > iterYear) {
            *neutralAfflux += DAYS_IN_YEAR; //el año no coincide
            iterYear++;
        } else {
            aux = aux->tailByYear;
        }
    }
}

size_t getFirstYear(stationsADT stationsAdt) {
    return stationsAdt->firstYear;
}

size_t getLastYear(stationsADT stationsAdt) {
    return stationsAdt->lastYear;
}


/* ----- Funciones para liberar memoria ----- */

static void freeMatrixByDay(int ** matrix) {
    for(size_t i = 0; i < MONTHS; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

static void freeAffluxByYearRec(struct affluxByYear * first) {
    if(first == NULL) {
        return;
    }
    freeAffluxByYearRec(first->tailByYear);
    freeMatrixByDay(first->affluxPerDay);
    free(first);
}

static void freeStationsRec(stationByName * station) {
    if(station == NULL) {
        return;
    }
    freeStationsRec(station->tailByName);
    free(station->name);
    freeAffluxByYearRec(station->affluxByYear);
    free(station);
}

static void freeStationsRecByTrip(stationByTrip * station) {
    if(station == NULL) {
        return;
    }
    freeStationsRecByTrip(station->tailByTrip);
    free(station->name);
    free(station);
}

static void freeMatrix(stationMat ** matrix, size_t dim) {
    for (size_t i = 0; i < dim; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void freeStations(stationsADT stationsAdt) {
    freeStationsRec(stationsAdt->firstByName);
    freeStationsRecByTrip(stationsAdt->firstByTrip);
    freeStationsRecByTrip(stationsAdt->firstByRoundTrip);
    if (stationsAdt->matrix != NULL){
        freeMatrix(stationsAdt->matrix, stationsAdt->dim);
    }
    free(stationsAdt->orderedIds);
    free(stationsAdt);
}
