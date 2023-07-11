COMPILER = gcc
FLAGS = -pedantic -std=c99 -Wall -fsanitize=address -g

OUTPUT_FILE_MON = bikeSharingMON
OUTPUT_FILE_NYC = bikeSharingNYC

QUERIES = query1.csv query2.csv query3.csv query1.html query2.html query3.html

DEPENDENCIES = stationsADT.c queries.c fileParsing.c htmlTable.c

all: MON NYC

MON:
	$(COMPILER) -o $(OUTPUT_FILE_MON) bikeSharingMON.c $(DEPENDENCIES) $(FLAGS)

NYC:
	$(COMPILER) -o $(OUTPUT_FILE_NYC) bikeSharingNYC.c $(DEPENDENCIES) $(FLAGS)

clean:
	rm -rf $(OUTPUT_FILE_MON) $(OUTPUT_FILE_NYC) *.o

cleanMON:
	rm -rf $(OUTPUT_FILE_MON) *.o

cleanNYC:
	rm -rf $(OUTPUT_FILE_NYC) *.o

cleanQueries:
	rm -f query1.csv

