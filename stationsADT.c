#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <errno.h>

#define NYC 0
#define MON 1
#define MONTHS 12
#define MAX_LEN 100

typedef struct stationByName {
    size_t id;
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
    size_t dim;
};

/* ----- Funciones para alocar memoria de forma segura ----- */

static void * safeMalloc(size_t bytes) {
    void * mem = malloc(bytes);
    if(mem == NULL || errno == ENOMEM) {
        fprintf(stderr, "Error de memoria");
        fprintf(stdout, "Uso de mas memoria de la que el sistema puede proveer");
        exit(1);
    }
    return mem;
}

static void * safeCalloc(size_t quan, size_t bytes) {
    void * mem = calloc(quan, bytes);
    if(mem == NULL || errno == ENOMEM) {
        fprintf(stderr, "Error de memoria");
        fprintf(stdout, "Uso de mas memoria de la que el sistema puede proveer");
        exit(1);
    }
    return mem;
}

/* ----- Funciones para resolver el procesamiento de datos a un TAD ----- */

static struct stationByName *addStationRec(struct stationByName * first, size_t id, char *name) {
    int c;
    if(first == NULL || (c = strcasecmp(first->name, name)) > 0) {
        stationByName * aux = safeCalloc(1, sizeof(stationByName));
        aux->id = id;
        aux->name = safeMalloc(strlen(name) + 1); //ver xq no anda sin el malloc!
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

void addTrip(stationByName * station, char ** nameA, char ** nameB, size_t * indexA, size_t * indexB, size_t month, size_t fromId, size_t toId, char isMember, char * existsIdFlag) {
    size_t flagA, flagB;
    flagA = flagB = 0;
    size_t i = 0;

    stationByName * aux = station;
    while(!flagA || !flagB) {
        if(aux == NULL) { //si llego a un NULL significa que alguno de los 2 id no coincide con alguna estacion.
            *existsIdFlag = 0;
            return;
        }
        if(aux->id == fromId) {
            if(isMember) {
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

static void addTripAtoB(stationMat ** mat, char *nameA, size_t indexA, size_t indexB) {
    if(mat[indexA][indexB].name == NULL) {
        mat[indexA][indexB].name = safeMalloc(MAX_LEN);
        strcpy(mat[indexA][indexB].name, nameA);
    }
    mat[indexA][indexB].quanTripsAtoB++;
}


void processEvent(stationsADT stationsAdt, size_t month, size_t fromId, size_t toId, char isMember) {
    char existsIdFlag = 1;
    char ** nameA = safeMalloc(MAX_LEN);
    char ** nameB = safeMalloc(MAX_LEN);
    size_t indexA;
    size_t indexB;

    addTrip(stationsAdt->firstByName, nameA, nameB, &indexA, &indexB, month, fromId, toId, isMember, &existsIdFlag);

    if(existsIdFlag && (strcasecmp(*nameA, *nameB) != 0)) {
        addTripAtoB(stationsAdt->matrix ,*nameA, indexA, indexB);
    }
    free(nameA);
    free(nameB);
}

void printMatrix(stationsADT stationsAdt) {
    for (int i = 0; i < stationsAdt->dim; i++) {
        for (int j = 0; j < stationsAdt->dim; j++) {

            if( stationsAdt->matrix[i][j].name != NULL && stationsAdt->matrix[j][i].name != NULL){
                char * auxA = stationsAdt->matrix[i][j].name;
                char * auxB = stationsAdt->matrix[j][i].name;
                printf("%s,HASTA %s, %d VECES\n", auxA, auxB,stationsAdt->matrix[i][j].quantTripsAtoB);
            }
        }
    }
}

void newMat(stationsADT stationsAdt) {
    size_t dim = stationsAdt->dim;
    stationMat ** aux = safeMalloc(dim * sizeof(stationMat*));
    for (size_t i = 0; i < dim; i++) {
        aux[i] = safeCalloc(1,dim * sizeof(stationMat));
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

    for (int i = 0; i < MONTHS; i++) {
        printf("viaje mes %d: %d\n", i + 1, station->quanTripsMonth[i]);
    }
    printLinksStations(station->tailByName);
}

void printLinks(stationsADT stationsAdt) {
    printLinksStations(stationsAdt->firstByName);
}

/* ----- Funciones para crear la lista ordenada por viajes ----- */

stationByTrip * createStationByTripNode(char * name, size_t quanTripsMember) {
	stationByTrip * newNode = safeMalloc(sizeof(stationByTrip));
	newNode->name = safeMalloc(strlen(name)+1);
	strcpy(newNode->name, name);
	newNode->quanTripsMember = quanTripsMember;
	newNode-> tailByTrip = NULL;
	return newNode;
}

void insertByTrip(stationsADT stationsAdt, stationByTrip * newNode) {
	stationByTrip * current = stationsAdt->firstByTrip;
	if (current == NULL || newNode->quanTripsMember > current->quanTripsMember || (newNode->quanTripsMember == current->quanTripsMember && strcasecmp(newNode->name, current->name)<0)) {
		newNode->tailByTrip = current;
		stations->firstByTrip = newNode;
	}
	else {
		while (current->tailByTrip != NULL && (current->tailByTrip->quanTripsMember > newNode->quanTripsMember || (current->tailByTrip->quanTripsMember == newNode->quanTripsMember && strcasecmp(current->tailByTrip->name, newNode->name) < 0))) {
			current = currrent->tailByTrip
		}
		newNode->tailByTrip = current->tailByTrip;
		current->tailByTrip = newNode;
	}
}

void rearrangeByTrip(stationsADT stationsAdt) {
	stationByName * current = stationsAdt->firstByName;
	while (current != NULL) {
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

char * getName(stationsADT stationsAdt, size_t flag) { // flag = 1 means check name it, flag = 0 means check trip it
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

char* getMatrixName(stationsADT stationsAdt, size_t indexA, size_t indexB) {
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
    stationMat * aux;
    for (int i = 0; i < dim; i++) {
        aux = matrix[i];
        free(aux);
    }
    free(matrix);
}

void freeStations(stationsADT stationsAdt) {
    freeStationsRec(stationsAdt->firstByName);
    free(stationsAdt->firstByTrip);
    freeMatrix(stationsAdt->matrix, stationsAdt->dim);
    free(stationsAdt);
}
