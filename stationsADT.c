#include "stationsADT.h"
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
    char name[MAX_LEN];
    double latitude;
    double longitude;
    size_t quanTripsMonth[MONTHS];
    size_t quanTripsTotal;
    station *tailTrip;
    station *tailName;
} station;

struct stationsCDT{
    station *firstTrip;
    station *firstName;
    size_t dim;
};

