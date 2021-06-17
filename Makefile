time: timeshmm.o
	gcc timeshmm.o -o time -lrt

timeshmm.o: timeshmm.c
	gcc -Wall -c timeshmm.c -o timeshmm.o -lrt

.PHONY: clean

clean:
	rm -rf time *.o
