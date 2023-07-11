/*
    Archivo: fileParsing.h
    ----------------------
    Este archivo se encarga del parseo de los datos,
    ya sea de las estaciones o de los eventos.
*/

#include <stdio.h>
#include "stationsADT.h"

#ifndef _fileparsing_h
#define _fileparsing_h

void parseStations(stationsADT stationsAdt, FILE *file, size_t identifier);

void parseEvents(stationsADT stationsAdt, FILE *file, size_t identifier);

#endif

