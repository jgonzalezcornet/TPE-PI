#include "stationsADT.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NYC 0
#define MON 1
#define MAX_CHAR 250
#include <stddef.h>

#define MAX_LEN 50
#define BLOCK 1000
#define MONTHS 12

typedef struct {
    char *start;
    char *end;
    size_t isMember;
    size_t idStart;
    size_t idEnd;
} event;

typedef struct station{
    size_t id;
    char *name;
    double latitude;
    double longitude;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsTotal;
    struct station *tailTrip;
    struct station *tailName;
} station;

struct stationsCDT{
    station *firstTrip;
    station *firstName;
    size_t dim;
};

double strToDouble( char *s ){
    double ans = 0;
    double flag = 1;
    int decimal,i,j;

    if( *s == '-' )
    {
        flag = -1;
        s = s+1;
    }
    for (i = 0; s[i] != '.' ; i++)
        ;
    decimal = i;
    for (i = decimal-1, j = 0; i >= 0; i--) {
        ans += ( s[j++] - '0') * pow(10,i);
    }
    for (i = decimal+1; s[i] != '\0' ; i++) {
        ans +=  ( s[i] - '0') * pow(10, decimal-i);
    }
    ans *= flag;
    return ans;
}
static size_t strToInt( char *s )
{
    size_t ans = 0;
    int i,j;
    for (i = strlen(s)-1, j = 0; i >= 0; i--) {
        ans += ( s[j++] - '0') * pow(10,i);
    }
    return ans;
}

static station * newStationRec(station * first, char *name, double latit, double longit, size_t id){
    int c;
    if( first == NULL || ( c = strcmp(first->name, name)) > 0){

        station *aux = calloc(1,sizeof(station));
        aux->name = malloc(strlen(name) + 1);
        strcpy(aux->name, name);
        aux->id = id;
        aux->latitude = latit;
        aux->longitude = longit;
        aux->tailName = aux->tailTrip = first;
        return aux;
    }
    if( c < 0)
        first->tailName = first->tailTrip = newStationRec(first->tailName, name, latit, longit, id);

    return first;
}

static station * addStation( station * first, char *str, size_t identifier){
    const char delim[2] = ";";
    char *id, *longit, *latit, *name;


    if(identifier == NYC){
        name = strtok(str,delim);
        latit = strtok(NULL,delim);
        longit = strtok(NULL,delim);
        id = strtok(NULL,delim);
    }
    else{
        id = strtok(str,delim);
        name = strtok(NULL,delim);
        latit = strtok(NULL,delim);
        longit = strtok(NULL,delim);
    }
    first = newStationRec(first, name, strToDouble(latit), strToDouble(longit), strToInt(id));
    return first;
}

stationsADT newStations(){
    return calloc(1,sizeof(struct stationsCDT));
}

void addStations (stationsADT stationsAdt, FILE * file, size_t identifier ){

    char str[MAX_CHAR];
    fgets(str, MAX_CHAR, file); // obtiene la primera linea, que solamente aclara el formato.
    while( fgets(str, MAX_CHAR, file) != NULL ){
        stationsAdt->firstName = stationsAdt->firstTrip = addStation(stationsAdt->firstName, str,identifier);
    }

}

static void printStationsRec(station * station){
    if(station == NULL)
        return;
    printf("nombre: %s longitud: %f, latitud: %f, id: %d\n", station->name, station->longitude, station->latitude, station->id);
    printStationsRec(station->tailName);
}

void printStations(stationsADT stationsAdt){
    printStationsRec(stationsAdt->firstName);
}

static void freeStationsRec(station * station)
{
    if(station == NULL)
        return;
    freeStationsRec(station->tailName);
    free(station->name);
    free(station);
}

void freeStations(stationsADT stationsAdt){
    freeStationsRec(stationsAdt->firstName);
    free(stationsAdt);
}
