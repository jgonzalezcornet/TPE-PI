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

## ELIMINAR ARCHIVOS
	- 'make clean'		   para borrar todos los archivos creados por el programa
	- 'make cleanQueries'	   para borrar los archivos .csv y .html creados por el programa
	- 'make cleanMON'	   para borrar el ejecutable MON y sus archivos objeto
	- 'make cleanNYC' 	   para borrar el ejecutable NYC y sus archivos objeto

* en caso de que se creen los directorios bikeSharingMON.dSYM y bikeSharingNYC.dSYM, debera eliminarlos manualmente.
* los viajes entre estaciones deben comenzar y finalizar en el mismo dia para el óptimo funcionamiento del programa.
* los años comprendidos mediante los cuales se pueden obtener los datos se encuentran entre el año 0 y el año 4000.