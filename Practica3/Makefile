CPP = gcc
CFLAGS = -g -Wall -pedantic
LDLIBS = -lrt -pthread

EXE = ejercicio3_consumidor ejercicio3_consumidor_b ejercicio3_productor ejercicio3_productor_b proceso_a proceso_b proceso_c ejercicio4 ejercicio2 ejercicio2_solved


all: $(EXE)

ejercicio2: ejercicio2.o
	$(CPP) $(CFLAGS) ejercicio2.o -o ejercicio2 $(LDLIBS)
ejercicio2_solved: ejercicio2_solved.o
	$(CPP) $(CFLAGS) ejercicio2_solved.o -o ejercicio2_solved $(LDLIBS)

queue.o: queue.c queue.h
	$(CPP) $(CFLAGS) -c queue.c
ejercicio3_productor: ejercicio3_productor.o queue.o
	$(CPP) $(CFLAGS) ejercicio3_productor.o queue.o -o ejercicio3_productor $(LDLIBS)
ejercicio3_consumidor: ejercicio3_consumidor.o queue.o
	$(CPP) $(CFLAGS) ejercicio3_consumidor.o queue.o -o ejercicio3_consumidor $(LDLIBS)
ejercicio3_productor_b: ejercicio3_productor_b.o queue.o
	$(CPP) $(CFLAGS) ejercicio3_productor_b.o queue.o -o ejercicio3_productor_b $(LDLIBS)
ejercicio3_consumidor_b: ejercicio3_consumidor_b.o queue.o
	$(CPP) $(CFLAGS) ejercicio3_consumidor_b.o queue.o -o ejercicio3_consumidor_b $(LDLIBS)

ejercicio4: ejercicio4.o
	$(CPP) $(CFLAGS) ejercicio4.o -o ejercicio4 $(LDLIBS)
proceso_a: proceso_a.o
	$(CPP) $(CFLAGS) proceso_a.o -o proceso_a $(LDLIBS)
proceso_b: proceso_b.o
	$(CPP) $(CFLAGS) proceso_b.o -o proceso_b $(LDLIBS)
proceso_c: proceso_c.o
	$(CPP) $(CFLAGS) proceso_c.o -o proceso_c $(LDLIBS)

ejercicio2.o: ejercicio2.c
	$(CPP) $(CFLAGS) -c ejercicio2.c $(LDLIBS)
ejercicio2_solved.o: ejercicio2_solved.c
	$(CPP) $(CFLAGS) -c ejercicio2_solved.c $(LDLIBS)
ejercicio3_productor.o: ejercicio3_productor.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_productor.c $(LDLIBS)
ejercicio3_consumidor.o: ejercicio3_consumidor.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_consumidor.c $(LDLIBS)
ejercicio3_productor_b.o: ejercicio3_productor_b.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_productor_b.c $(LDLIBS)
ejercicio3_consumidor_b.o: ejercicio3_consumidor_b.c queue.h
	$(CPP) $(CFLAGS) -c ejercicio3_consumidor_b.c $(LDLIBS)
ejercicio4.o: ejercicio4.c
	$(CPP) $(CFLAGS) -c ejercicio4.c $(LDLIBS)
proceso_a.o: proceso_a.c
	$(CPP) $(CFLAGS) -c proceso_a.c $(LDLIBS)
proceso_b.o: proceso_b.c
	$(CPP) $(CFLAGS) -c proceso_b.c $(LDLIBS)
proceso_c.o: proceso_c.c
	$(CPP) $(CFLAGS) -c proceso_c.c $(LDLIBS)

clean:
	rm -f $(EXE) *.o
	rm -f /dev/shm/sem.sem_*
