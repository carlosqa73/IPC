all: time timepipe

time: timeshmm.o
	gcc timeshmm.o -o time -lrt

timepipe: timepipe.o
	gcc timepipe.o -o timepipe -lrt

timeshmm.o: timeshmm.c
	gcc -Wall -c timeshmm.c -o timeshmm.o -lrt

timepipe.o: timepipe.c
	gcc -Wall -c timepipe.c -o timepipe.o -lrt

.PHONY: clean

clean:
	rm -rf time timepipe *.o
