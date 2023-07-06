# TRABAJO PRACTICO FINAL - 72.31 PROGRAMACION IMPERATIVA
	- 1ER CUATRIMESTRE 2023
	- FECHA DE ENTREGA 11/07

## INTEGRANTES
	* JUAN PABLO BIRSA 64500 (jbirsa)
	* JUAN IGNACIO CANTARELLA 64509 (jicanta)
	* JOSEFINA GONZALEZ CORNET 64550 (jgonzalezcornet)

## CREACION DEL EJECUTABLE
	- para crear ambos ejecutables correr el comando 'make' o 'make all' en la consola
	- para crear el ejecutable de Montreal, correr el comando 'make MON'
	- para crear el ejecutable de Nueva York, correr el comando 'make NYC'

## EJECUCION
	* el archivo ejecutable de Montreal se llamara "bikeSharingMON"
	* el archivo ejecutable de Nueva York se llamara "bikeSharingNYC"

	- se deberan pasar 2 (dos) argumentos por linea de comando (en el orden establecido):
		1. path al archivo con los datos sobre los viajes (ej: bikesNYC.csv)
		2. path al archivo con los datos sobre las estaciones (ej: stationsMON.csv)

## DEBUGGING
	- correr el comando 'make debug' y se crearan ambos ejecutables.
	- los flags adicionales seran: -fsanitize=address y -g

## ELIMINAR ARCHIVOS
	- 'make clean'		para borrar todos los archivos
	- 'make cleanQueries'	para borrar los archivos .csv creados por el programa
	- 'make clean MON'	para borrar el ejecutable MON y sus archivos objeto
	- 'make clean NYC' 	para borrar el ejecutable NYC y sus archivos objeto

