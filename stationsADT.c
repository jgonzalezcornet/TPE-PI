#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>

#define MAX_LEN 100

typedef struct stationByName {
    size_t id;
    size_t alfaId;
    char * name;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsMember;
    size_t quanRoundTrips;
    struct stationByName * tailByName;
} stationByName;

typedef struct stationByTrip {
    char * name;
    size_t quanTrips;
    struct stationByTrip * tailByTrip;
} stationByTrip;

typedef struct stationMat{
    char * name;
    size_t quanTripsAtoB;
} stationMat;

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

static void swap(struct stationByName **v, size_t i, size_t j){
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

void processEvent(stationsADT stationsAdt, size_t month, size_t fromId, size_t toId, char isMember, size_t year) {
    stationByName * statFrom = getStationById(stationsAdt, fromId);
    stationByName * statTo = getStationById(stationsAdt, toId);

    char ** nameA = safeMalloc(MAX_LEN);
    char ** nameB = safeMalloc(MAX_LEN);
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
        if(stationsAdt->firstYear <= year && year <= stationsAdt->lastYear && indexA == indexB) {
            statFrom->quanRoundTrips++;
        } else {
            addTripAtoB(stationsAdt->matrix , *nameA, *nameB, indexA, indexB);
        }
    }
    free(nameA);
    free(nameB); //!!!!!!!!!!!!!!AGREGADO POR LOS LEAK
}

void newMat(stationsADT stationsAdt) {
    size_t dim = stationsAdt->dim;
    stationMat ** aux = safeMalloc(dim * sizeof(stationMat *));
    for(size_t i = 0; i < dim; i++) {
        aux[i] = safeCalloc(1, dim * sizeof(stationMat));
    }
    stationsAdt->matrix = aux;
}

stationsADT newStations() {
    return safeCalloc(1, sizeof(struct stationsCDT));
}

/* ----- Funciones para crear la lista ordenada por viajes ----- */

static stationByTrip * createStationByTripNode(char * name, size_t quanTripsMember) {
	stationByTrip * newNode = safeMalloc(sizeof(stationByTrip));
	newNode->name = safeMalloc(strlen(name) + 1);
	strcpy(newNode->name, name);
	newNode->quanTrips = quanTripsMember;
	newNode->tailByTrip = NULL;
	return newNode;
}

static void insertByTrip(stationsADT stationsAdt, stationByTrip * newNode) {
	stationByTrip * current = stationsAdt->firstByTrip;
	if(current == NULL || newNode->quanTrips > current->quanTrips || (newNode->quanTrips == current->quanTrips && strcasecmp(newNode->name, current->name) < 0)) {
		newNode->tailByTrip = current;
		stationsAdt->firstByTrip = newNode;
	}
	else {
		while(current->tailByTrip != NULL && (current->tailByTrip->quanTrips > newNode->quanTrips || (current->tailByTrip->quanTrips == newNode->quanTrips && strcasecmp(current->tailByTrip->name, newNode->name) < 0))) {
			current = current->tailByTrip;
		}
		newNode->tailByTrip = current->tailByTrip;
		current->tailByTrip = newNode;
	}
}

void rearrangeByTrip(stationsADT stationsAdt) {
	stationByName * current = stationsAdt->firstByName;
	while(current != NULL) {
		stationByTrip * newNode = createStationByTripNode(current->name, current->quanTripsMember);
		insertByTrip(stationsAdt, newNode);
		current = current->tailByName;
	}
}

/* ----- Funciones para crear la lista ordenada por viajes circulares ----- */

static stationByTrip * createStationByRoundTripNode(char * name, size_t quanRoundTrips) {
	stationByTrip * newNode = safeMalloc(sizeof(stationByTrip));
	newNode->name = safeMalloc(strlen(name) + 1);
	strcpy(newNode->name, name);
	newNode->quanTrips = quanRoundTrips;
	newNode->tailByTrip = NULL;
	return newNode;
}

static void insertByRoundTrip(stationsADT stationsAdt, stationByTrip * newNode) {
	stationByTrip * current = stationsAdt->firstByRoundTrip;
	if(current == NULL || newNode->quanTrips > current->quanTrips || (newNode->quanTrips == current->quanTrips && strcasecmp(newNode->name, current->name) < 0)) {
		newNode->tailByTrip = current;
		stationsAdt->firstByTrip = newNode;
	}
	else {
		while(current->tailByTrip != NULL && (current->tailByTrip->quanTrips > newNode->quanTrips || (current->tailByTrip->quanTrips == newNode->quanTrips && strcasecmp(current->tailByTrip->name, newNode->name) < 0))) {
			current = current->tailByTrip;
		}
		newNode->tailByTrip = current->tailByTrip;
		current->tailByTrip = newNode;
	}
}

void rearrangeByRoundTrip(stationsADT stationsAdt) {
	stationByName * current = stationsAdt->firstByName;
	while(current != NULL) {
		stationByTrip * newNode = createStationByRoundTripNode(current->name, current->quanRoundTrips);
		insertByTrip(stationsAdt, newNode);
		current = current->tailByName;
	}
}

/* ----- Funciones de iteración por viajes ----- */

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

/* funciones de iteracion por viajes circulares */

void toBeginRoundTrip(stationsADT stationsAdt) {
    stationsAdt->itRoundTrip = stationsAdt->firstByRoundTrip;
}

size_t hasNextRoundTrip(stationsADT stationsAdt) {
    return stationsAdt->itRoundTrip->tailByTrip != NULL;
}

size_t nextRoundTrip(stationsADT stationsAdt) {
    size_t c;
    if((c = hasNextTrip(stationsAdt))) {
        stationsAdt->itRoundTrip = stationsAdt->itRoundTrip->tailByTrip;
    }
    return c;
}

/* ----- Funciones de iteración por nombre ----- */

void toBeginName(stationsADT stationsAdt) {
	stationsAdt->itName = stationsAdt->firstByName;
}

size_t hasNextName(stationsADT stationsAdt) {
	return stationsAdt->itName->tailByName != NULL;
}

size_t nextName(stationsADT stationsAdt) {
	size_t c;
	if((c = hasNextName(stationsAdt))) {
		stationsAdt->itName = stationsAdt->itName->tailByName;
	}
	return c;
}

/* ----- Funciones para obtener datos del Adt desde queries.c ----- */

char * getName(stationsADT stationsAdt, size_t flag) {
    if(flag == 0) {
        return stationsAdt->itTrip->name;
    } else if(flag == 1) {
       return stationsAdt->itName->name;
    }
    return stationsAdt->itRoundTrip->name;
}

size_t getTotalTrips(stationsADT stationsAdt, size_t flag) {
    if(flag == 0) {
        return stationsAdt->itTrip->quanTrips;
    } else if(flag == 1) {
       return stationsAdt->itName->quanTripsMember;
    }
    return stationsAdt->itRoundTrip->quanTrips;
}

size_t getTripsByMonth(stationsADT stationsAdt, size_t month) {
    return stationsAdt->itName->quanTripsMonth[month];
}

char * getMatrixName(stationsADT stationsAdt, size_t indexA, size_t indexB) {
    return stationsAdt->matrix[indexA][indexB].name;
}

size_t getTripsAtoB(stationsADT stationsAdt, size_t indexA, size_t indexB) {
    return stationsAdt->matrix[indexA][indexB].quanTripsAtoB;
}

size_t getDim(stationsADT stationsAdt) {
	return stationsAdt->dim;
}

/* ----- Funciones para liberar memoria ----- */

static void freeStationsRec(stationByName * station) {
    if(station == NULL) {
        return;
    }
    freeStationsRec(station->tailByName);
    free(station->name);
    free(station);
}

static void freeStationsRecByTrip(stationByTrip * station){ //!!!!!!!!!!!!!!AGREGADO POR LOS LEAK
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

