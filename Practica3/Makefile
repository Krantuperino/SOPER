CPP = gcc
CFLAGS = -g -Wall -pedantic
LDLIBS = -lrt -pthread

BOLD=\e[1m
NC=\e[0m



all: queue.o ejercicio3_productor ejercicio3_consumidor ejercicio3_consumidor_b ejercicio3_productor_b

queue.o: queue.c queue.h
	$(CPP) $(CFLAGS) -c queue.c
ejercicio3_productor: ejercicio3_productor.o
	$(CPP) $(CFLAGS) ejercicio3_productor.o queue.o -o ejercicio3_productor $(LDLIBS)
ejercicio3_consumidor: ejercicio3_consumidor.o
	$(CPP) $(CFLAGS) ejercicio3_consumidor.o queue.o -o ejercicio3_consumidor $(LDLIBS)
ejercicio3_productor_b: ejercicio3_productor_b.o
	$(CPP) $(CFLAGS) ejercicio3_productor_b.o queue.o -o ejercicio3_productor_b $(LDLIBS)
ejercicio3_consumidor_b: ejercicio3_consumidor_b.o
	$(CPP) $(CFLAGS) ejercicio3_consumidor_b.o queue.o -o ejercicio3_consumidor_b $(LDLIBS)


ejercicio3_productor.o: ejercicio3_productor.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_productor.c $(LDLIBS)
ejercicio3_consumidor.o: ejercicio3_consumidor.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_consumidor.c $(LDLIBS)
ejercicio3_productor_b.o: ejercicio3_productor_b.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_productor_b.c $(LDLIBS)
ejercicio3_consumidor_b.o: ejercicio3_consumidor_b.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_consumidor_b.c $(LDLIBS)



clean:
	rm -f ejercicio3_consumidor ejercicio3_productor ejercicio3_consumidor.o ejercicio3_productor.o queue.o
	rm -f /dev/shm/sem.sem_*
