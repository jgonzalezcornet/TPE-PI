COMPILER = gcc
FLAGS = -pedantic -std=c99 -Wall -fsanitize=address -g

OUTPUT_FILE_MON = bikeSharingMON
OUTPUT_FILE_NYC = bikeSharingNYC

QUERIES = query1.csv query2.csv query3.csv query4.csv query5.csv query1.html query2.html query3.html query4.html query5.csv query5.html

DEPENDENCIES = stationsADT.c queries.c fileParsing.c htmlTable.c safeMemory.c

all: MON NYC

MON:
	$(COMPILER) -o $(OUTPUT_FILE_MON) bikeSharing.c $(DEPENDENCIES) $(FLAGS) -DMON

NYC:
	$(COMPILER) -o $(OUTPUT_FILE_NYC) bikeSharing.c $(DEPENDENCIES) $(FLAGS)

clean:
	rm -rf $(OUTPUT_FILE_MON) $(OUTPUT_FILE_NYC) *.o $(QUERIES)

cleanMON:
	rm -rf $(OUTPUT_FILE_MON) *.o

cleanNYC:
	rm -rf $(OUTPUT_FILE_NYC) *.o

cleanQueries:
	rm -f $(QUERIES)

test:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --verbose --log-file=valgrind_analysis.txt ./bikeSharingMON csvFiles/bikesMON.csv csvFiles/stationsMON.csv
