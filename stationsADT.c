#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>

#define MAX_LEN 100
#define DAYS_IN_YEAR 365

// !!!!!! falta separar bien el front y el back
// !!!!!! dejo esto para que nos encarguemos bien de chequear todos los frees, aunque no hay leaks pero por las dudas

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
    char * name;
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

/* ----- Funciones para alocar memoria de forma segura ----- */

void * safeMalloc(size_t bytes) {
    void * mem = malloc(bytes);
    if(mem == NULL) {
        fprintf(stderr, "Error de memoria");
        fprintf(stdout, "Uso de mas memoria de la que el sistema puede proveer");
	    errno = ENOMEM;
    }
    return mem;
}

void * safeCalloc(size_t quan, size_t bytes) {
    void * mem = calloc(quan, bytes);
    if(mem == NULL) {
        fprintf(stderr, "Error de memoria");
        fprintf(stdout, "Uso de mas memoria de la que el sistema puede proveer");
	    errno = ENOMEM;
    }
    return mem;
}

/* ---- Funciones para el vector de punteros a estaciones ----- */

static void swap(struct stationByName ** v, size_t i, size_t j) {
    struct stationByName *aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}

static void bubbleSort(struct stationByName ** v, size_t dim) {
    for(size_t i = 0; i < dim - 1; i++) {
        for(size_t j = 0; j < dim - i - 1; j++) {
            if(v[j] != NULL && v[j+1] != NULL && v[j]->id > v[j+1]->id) {
                swap(v, j, j+1);
            }
        }
    }
}

static void traverseListToFillArray(stationByName * first, size_t i, struct stationByName ** ids) {
    if(first == NULL) {
        return;
    }
    first->alfaId = i;
    ids[i] = first;
    traverseListToFillArray(first->tailByName, i + 1, ids);
}

void fillOrderedIds(stationsADT stationsAdt) {
    stationsAdt->orderedIds = safeMalloc(stationsAdt->dim * sizeof(struct stationByName *));
    traverseListToFillArray(stationsAdt->firstByName, 0, stationsAdt->orderedIds);
    bubbleSort(stationsAdt->orderedIds, stationsAdt->dim);
}

static struct stationByName * getStationById(stationsADT stationsAdt, size_t id) {
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

static struct stationByName * addStationRec(struct stationByName * first, size_t id, char * name, size_t * flag) {
    int c;
    if(first == NULL || (c = strcasecmp(first->name, name)) > 0) {
        stationByName * aux = safeCalloc(1, sizeof(stationByName));
        aux->id = id;
        aux->name = safeMalloc(strlen(name) + 1);
        strcpy(aux->name, name);
        aux->tailByName = first;
        *flag = 1;
        return aux;
    } else if(c < 0) {
        first->tailByName = addStationRec(first->tailByName, id, name, flag);
    }
    return first;
}

void addStation(stationsADT stationsAdt, size_t id, char * name) {
    size_t flag = 0;
    stationsAdt->firstByName = addStationRec(stationsAdt->firstByName, id, name, &flag);
    stationsAdt->dim += flag;
}

static void addTripAtoB(stationMat ** mat, char * nameA, char * nameB, size_t indexA, size_t indexB) {
    if(mat[indexA][indexB].name == NULL) {
        mat[indexA][indexB].name = safeMalloc(MAX_LEN);
        strcpy(mat[indexA][indexB].name, nameA);
    }
    if(mat[indexB][indexA].name == NULL){
        mat[indexB][indexA].name = safeMalloc(MAX_LEN);
        strcpy(mat[indexB][indexA].name, nameB); 
    }
    mat[indexA][indexB].quanTripsAtoB++;
}

static int ** newMatPerYear(size_t * daysPerMonth){
    int ** aux = safeMalloc(MONTHS * sizeof(int *));
    for(size_t i = 0; i < MONTHS; i++){
        aux[i] = safeCalloc(1, daysPerMonth[i] * sizeof(int)); //todos los dias arrancan en 0
    }
    return aux;
}

static struct affluxByYear * addDayRec(struct affluxByYear * first, size_t year, size_t month, size_t day, int state){
    if(first == NULL || first->year > year){
        struct affluxByYear * aux = safeMalloc(sizeof(struct affluxByYear));
        size_t daysPerMonth[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        aux->affluxPerDay = newMatPerYear(daysPerMonth);
        aux->affluxPerDay[month-1][day-1] += state;
        aux->year = year;
        aux->tailByYear = first;
        return aux;
    }
    else if(first->year < year){
        first->tailByYear = addDayRec(first->tailByYear, year, month, day, state);
    }
    else{
        first->affluxPerDay[month-1][day-1] += state;
    }
    return first;
}

void processEvent(stationsADT stationsAdt, size_t year, size_t month, size_t day, size_t fromId, size_t toId, char isMember, char isRange) {
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

    char ** nameA = safeMalloc(sizeof(char *));
    char ** nameB = safeMalloc(sizeof(char *));
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
	    *nameA = statFrom->name;
    }

    if(statTo != NULL) {
        flagB = 1;
        indexB = statTo->alfaId;
        *nameB = statTo->name;
    }

    if(flagA && flagB) {
        if(stationsAdt->firstYear <= year && year <= stationsAdt->lastYear) {
            statFrom->affluxByYear = addDayRec(statFrom->affluxByYear, year, month, day, -1);
            statTo->affluxByYear = addDayRec(statTo->affluxByYear, year, month, day, 1);
            if(indexA == indexB){
                statFrom->quanRoundTrips++;
            }
        }
        if(indexA != indexB){
            addTripAtoB(stationsAdt->matrix , *nameA, *nameB, indexA, indexB);
        }
        
    }
    free(nameA);
    free(nameB);
}

void newMat(stationsADT stationsAdt) {
    size_t dim = stationsAdt->dim;
    stationMat ** aux = safeMalloc(dim * sizeof(stationMat *));
    for(size_t i = 0; i < dim; i++) {
        aux[i] = safeCalloc(1, dim * sizeof(stationMat));
    }
    stationsAdt->matrix = aux;
}

stationsADT newStations(size_t firstYear, size_t lastYear) {
    stationsADT aux = safeCalloc(1, sizeof(struct stationsCDT));

    aux->firstYear = firstYear;
    aux->lastYear = lastYear;
    return aux;
}

/* ----- Funciones para crear la lista ordenada por viajes ----- */

static stationByTrip * createStationByTripNode(char * name, size_t quanTrips) {
	stationByTrip * newNode = safeMalloc(sizeof(stationByTrip));
	newNode->name = safeMalloc(strlen(name) + 1);
	strcpy(newNode->name, name);
	newNode->quanTrips = quanTrips;
	newNode->tailByTrip = NULL;
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
void rearrangeByTrip(stationsADT stationsAdt, size_t flag) {
	stationByName * current = stationsAdt->firstByName;
	while(current != NULL) {
        stationByTrip * newNode = createStationByTripNode(current->name, (flag == 0 ? current->quanTripsMember : current->quanRoundTrips));
		insertByTrip(stationsAdt, newNode, flag);
		current = current->tailByName;
	}
}

/* ----- Funciones de iteración por viajes ----- */

void toBeginTrip(stationsADT stationsAdt) {
    stationsAdt->itTrip = stationsAdt->firstByTrip;
}

size_t hasNextTrip(stationsADT stationsAdt) {
    if (stationsAdt->itTrip != NULL){  // !!!!!!! no se si hace falta esto, y no se que hariamos en caso de else, ver como resolver
        return stationsAdt->itTrip->tailByTrip != NULL;
    }
}

size_t nextTrip(stationsADT stationsAdt) {
    size_t c;
    if((c = hasNextTrip(stationsAdt))) {
        stationsAdt->itTrip = stationsAdt->itTrip->tailByTrip;
    }
    return c;
}

/* ----- Funciones de iteración por viajes circulares ----- */

void toBeginRoundTrip(stationsADT stationsAdt) {
    stationsAdt->itRoundTrip = stationsAdt->firstByRoundTrip;
}

size_t hasNextRoundTrip(stationsADT stationsAdt) {
    if (stationsAdt->itRoundTrip != NULL) {  // !!!!!!! no se si hace falta esto, y no se que hariamos en caso de else, ver como resolver
        return stationsAdt->itRoundTrip->tailByTrip != NULL;
    }
}

size_t nextRoundTrip(stationsADT stationsAdt) {
    size_t c;
    if((c = hasNextRoundTrip(stationsAdt))) {
        stationsAdt->itRoundTrip = stationsAdt->itRoundTrip->tailByTrip;
    }
    return c;
}

/* ----- Funciones de iteración por nombre ----- */

void toBeginName(stationsADT stationsAdt) {
    stationsAdt->itName = stationsAdt->firstByName;
}

size_t hasNextName(stationsADT stationsAdt) {
    if (stationsAdt->itName != NULL) {       // !!!!!! no se si hace falta esto, y no se que hariamos en caso de else, ver como resolver
        return stationsAdt->itName->tailByName != NULL;
    }
}

int nextName(stationsADT stationsAdt) {
	size_t c;
	if((c = hasNextName(stationsAdt))) {
		stationsAdt->itName = stationsAdt->itName->tailByName;
	}
	return c;
}

/* ----- Funciones para obtener datos del Adt desde queries.c ----- */

char * getName(stationsADT stationsAdt, size_t flag) {
    if(flag == 0 && stationsAdt->itTrip != NULL) {
        return stationsAdt->itTrip->name;
    } else if(flag == 1 && stationsAdt->itName != NULL) {
        return stationsAdt->itName->name;
    } else if(stationsAdt->itRoundTrip != NULL) {
        return stationsAdt->itRoundTrip->name;
    }
    return NULL;
}

size_t getTotalTrips(stationsADT stationsAdt, size_t flag) {
    if(flag == 0 && stationsAdt->itTrip != NULL) {
        return stationsAdt->itTrip->quanTrips;
    } else if(flag == 1 && stationsAdt->itName != NULL) {
        return stationsAdt->itName->quanTripsMember;
    } else if(stationsAdt->itRoundTrip != NULL) {
        return stationsAdt->itRoundTrip->quanTrips;
    }
    return -1;
}

size_t getTripsByMonth(stationsADT stationsAdt, size_t month) {
    if (month >= 0 && month <= 11) {  // PROGRAMACION DEFENSIVA en realidad >=0 no hace falta porque es un size_t pero bueno ver eso
        return stationsAdt->itName->quanTripsMonth[month];
    }
}

char * getMatrixName(stationsADT stationsAdt, size_t indexA, size_t indexB) {
    if (indexA >= 0 && indexA < stationsAdt->dim && indexB >= 0 && indexB < stationsAdt->dim) {  // PROGRAMACION DEFENSIVA en realidad >=0 no hace falta porque es un size_t pero bueno ver eso
        return stationsAdt->matrix[indexA][indexB].name;
    }
}

size_t getTripsAtoB(stationsADT stationsAdt, size_t indexA, size_t indexB) {
    if (indexA >= 0 && indexA < stationsAdt->dim && indexB >= 0 && indexB < stationsAdt->dim) {  // PROGRAMACION DEFENSIVA en realidad >=0 no hace falta porque es un size_t pero bueno ver eso
        return stationsAdt->matrix[indexA][indexB].quanTripsAtoB;
    }
}

size_t getDim(stationsADT stationsAdt) {
	return stationsAdt->dim;
}

static void getAffluxInMat(int **matrix, int * posAfflux, int * neutralAfflux, int * negAfflux){
    size_t daysPerMonth[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (size_t i = 0; i < MONTHS; i++){
        for (size_t j = 0; j < daysPerMonth[i]; j++){
            if(matrix[i][j] > 0){
                (*posAfflux)++;
            }
            else if(matrix[i][j] < 0){
                (*negAfflux)++;
            }
            else{
                (*neutralAfflux)++;
            }
        }
    }
}


void getAfflux(stationsADT stationsAdt, size_t firstYear, size_t lastYear, int * posAfflux, int * neutralAfflux, int * negAfflux){
    *posAfflux = 0;
    *neutralAfflux = 0;
    *negAfflux = 0;
    struct affluxByYear * aux = stationsAdt->itName->affluxByYear;
    if(aux == NULL){
        *neutralAfflux = DAYS_IN_YEAR * (lastYear - firstYear + 1);
        return;
    }
    size_t iterYear = firstYear;
    while(iterYear <= lastYear){
        if(aux == NULL){  //no hay mas años por recorrer
            *neutralAfflux += DAYS_IN_YEAR * (lastYear - iterYear + 1);
            return;
        }
        else if(aux->year == iterYear){  //el año coincide
            getAffluxInMat(aux->affluxPerDay, posAfflux, neutralAfflux, negAfflux);
            aux = aux->tailByYear;
            iterYear++;
        }
        else if(aux->year > iterYear){
            *neutralAfflux += DAYS_IN_YEAR; //el año no coincide
            iterYear++;
        }
        else{
            aux = aux->tailByYear;
        }
    }
}

size_t getFirstYear(stationsADT stationsAdt){
    return stationsAdt->firstYear;
}

size_t getLastYear(stationsADT stationsAdt){
    return stationsAdt->lastYear;
}


/* ----- Funciones para liberar memoria ----- */

static void freeMatrixByDay(int ** matrix){
    for (size_t i = 0; i < MONTHS; i++){
        free(matrix[i]);
    }
    free(matrix);
}

static void freeAffluxByYearRec( struct affluxByYear * first){
    if(first == NULL){
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

static void freeStationsRecByTrip(stationByTrip * station){ // !!!!!!!!!!!!!! AGREGADO POR LOS LEAK
    if(station == NULL) {
        return;
    }
    freeStationsRecByTrip(station->tailByTrip);
    free(station->name);
    free(station);
}

static void freeMatrix(stationMat ** matrix, size_t dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++){
            if(matrix[i][j].name != NULL) {
                free(matrix[i][j].name);
            }
        }
        free(matrix[i]);
    }
    free(matrix);
}

void freeStations(stationsADT stationsAdt) {
    freeStationsRec(stationsAdt->firstByName);
    freeStationsRecByTrip(stationsAdt->firstByTrip);
    freeStationsRecByTrip(stationsAdt->firstByRoundTrip);
    freeMatrix(stationsAdt->matrix, stationsAdt->dim);
    free(stationsAdt->orderedIds);
    free(stationsAdt);
}

/* ----- Funciones para testeos ----- */

static void printListRec(stationByName * station){
    if(station == NULL){
        return;
    }
    printf("%s\tID: %zu\n",station->name,station->id);
    printListRec(station->tailByName);
}

void printList(stationsADT stationsAdt){
    printListRec(stationsAdt->firstByName);
}

void printOrderedByIds(stationsADT stationsAdt){

    for (size_t i = 0; i < stationsAdt->dim; i++){
        printf("%s\t id:%zu\n", stationsAdt->orderedIds[i]->name, stationsAdt->orderedIds[i]->id);    
    }
}

void printMatPerYear(stationsADT stationAdt)
{
    size_t count = 0;
    if(stationAdt->firstByName->affluxByYear == NULL){
        return;
    }
    size_t daysPerMonth[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    printf("ANIO %zu:\n",stationAdt->firstByName->affluxByYear->year);
    for (size_t i = 0; i < MONTHS; i++)
    {
        for (size_t j = 0; j < daysPerMonth[i]; j++)
        {
            printf("%d ",stationAdt->firstByName->affluxByYear->affluxPerDay[i][j]);
            count++;
        }
        putchar('\n');
    }
}
