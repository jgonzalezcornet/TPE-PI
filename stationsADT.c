#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <errno.h>

#define MAX_LEN 100

typedef struct stationByName {
    size_t id;
    size_t alfaId;
    char * name;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsMember;
    struct stationByName * tailByName;
} stationByName;

typedef struct stationByTrip{
    char * name;
    size_t quanTripsMember;
    struct stationByTrip * tailByTrip;
} stationByTrip;

typedef struct stationMat{
    char * name;
    size_t quanTripsAtoB;
} stationMat;

struct stationsCDT {
    struct stationByTrip * firstByTrip;
    struct stationByName * firstByName;
    struct stationByTrip * itTrip;
    struct stationByName * itName;
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

static void swap(struct stationByName * p1, struct stationByName * p2) {
    stationByName temp = *p2;
    *p2 = *p1;
    *p1 = temp;
}

static void bubbleSort(struct stationByName ** v, size_t dim) {
    for(size_t i = 0; i < dim - 1; i++) {
        for(size_t j = 0; j < dim - i - 1; j++) {
             if (v[j]->id > v[j+1]->id) {
                swap(&(*v[j]), &(*v[j + 1]));
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
    stationsAdt->orderedIds = safeMalloc(stationsAdt->dim * sizeof(struct  stationByName *));
    traverseListToFillArray(stationsAdt->firstByName, 0, stationsAdt->orderedIds);
    bubbleSort(stationsAdt->orderedIds, stationsAdt->dim);
    /// ahora, el arreglo esta ordenado por ids
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

static struct stationByName * addStationRec(struct stationByName * first, size_t id, char * name) {
    int c;
    if(first == NULL || (c = strcasecmp(first->name, name)) > 0) {
        stationByName * aux = safeCalloc(1, sizeof(stationByName));
        aux->id = id;
        aux->name = safeMalloc(strlen(name) + 1);
        strcpy(aux->name, name);
        aux->tailByName = first;
        return aux;
    } else if(c < 0) {
        first->tailByName = addStationRec(first->tailByName, id, name);
    }
    return first;
}

void addStation(stationsADT stationsAdt, size_t id, char * name) {
    stationsAdt->firstByName = addStationRec(stationsAdt->firstByName, id, name);
    stationsAdt->dim++;
}

static void addTripAtoB(stationMat ** mat, char * nameA, char * nameB, size_t indexA, size_t indexB) {
    if(mat[indexA][indexB].name == NULL) {
        mat[indexA][indexB].name = safeMalloc(MAX_LEN);
        strcpy(mat[indexA][indexB].name, nameA);
    }
    if(mat[indexB][indexA].name == NULL){
        mat[indexB][indexA].name = safeMalloc(MAX_LEN);
        strcpy(mat[indexB][indexA].name, nameA);
    }
    mat[indexA][indexB].quanTripsAtoB++;
}

void processEvent(stationsADT stationsAdt, size_t month, size_t fromId, size_t toId, char isMember) {
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
        *nameA = statFrom->name;
        flagA = 1;
        indexA = statFrom->alfaId;
    }

    if(statTo != NULL) {
        flagB = 1;
        indexB = statTo->alfaId;
    }

    if(flagA  && flagB && indexA != indexB) {
        addTripAtoB(stationsAdt->matrix , *nameA, *nameB, indexA, indexB);
    }
    free(nameA);
}

void printMatrix(stationsADT stationsAdt) {
    for(int i = 0; i < stationsAdt->dim; i++) {
        for(int j = 0; j < stationsAdt->dim; j++) {

            if(stationsAdt->matrix[i][j].name != NULL && stationsAdt->matrix[j][i].name != NULL){
                char * auxA = stationsAdt->matrix[i][j].name;
                char * auxB = stationsAdt->matrix[j][i].name;
                printf("%s,HASTA %s, %d VECES\n", auxA, auxB, stationsAdt->matrix[i][j].quanTripsAtoB);
            }
        }
    }
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

static void printStationsRec(stationByName * station) {
    if(station == NULL) {
        return;
    }
    printf("nombre: %s \t\tid: %d\n", station->name, station->id);
    printStationsRec(station->tailByName);
}

void printStations(stationsADT stationsAdt) {
    printStationsRec(stationsAdt->firstByName);
}

static void printLinksStations(stationByName * station) {
    if(station == NULL){
        return;
    }

    printf("nombre: %s, id: %d viajes por mes: %d\n", station->name, station->id,station->quanTripsMember);

    for(int i = 0; i < MONTHS; i++) {
        printf("viaje mes %d: %d\n", i + 1, station->quanTripsMonth[i]);
    }
    printLinksStations(station->tailByName);
}

void printLinks(stationsADT stationsAdt) {
    printLinksStations(stationsAdt->firstByName);
}

/* ----- Funciones para crear la lista ordenada por viajes ----- */

static stationByTrip * createStationByTripNode(char * name, size_t quanTripsMember) {
	stationByTrip * newNode = safeMalloc(sizeof(stationByTrip));
	newNode->name = safeMalloc(strlen(name) + 1);
	strcpy(newNode->name, name);
	newNode->quanTripsMember = quanTripsMember;
	newNode->tailByTrip = NULL;
	return newNode;
}

static void insertByTrip(stationsADT stationsAdt, stationByTrip * newNode) {
	stationByTrip * current = stationsAdt->firstByTrip;
	if(current == NULL || newNode->quanTripsMember > current->quanTripsMember || (newNode->quanTripsMember == current->quanTripsMember && strcasecmp(newNode->name, current->name)<0)) {
		newNode->tailByTrip = current;
		stationsAdt->firstByTrip = newNode;
	}
	else {
        // OJO: refactorizar linea
		while(current->tailByTrip != NULL && (current->tailByTrip->quanTripsMember > newNode->quanTripsMember || (current->tailByTrip->quanTripsMember == newNode->quanTripsMember && strcasecmp(current->tailByTrip->name, newNode->name) < 0))) {
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

/* ----- Funciones de iteración por nombre ----- */

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

/* ----- Funciones para obtener datos del Adt desde queries.c ----- */

char * getName(stationsADT stationsAdt, size_t flag) {
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
    free(stationsAdt->firstByTrip);
    freeMatrix(stationsAdt->matrix, stationsAdt->dim);
    free(stationsAdt->orderedIds);
    free(stationsAdt);
}
