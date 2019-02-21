CC = gcc
CFLAGS = -lpthread -lm
EXE = ejercicio3 ejercicio4 ejercicio7 ejercicio9 ejercicio12

all: $(EXE)

clean:
	rm -rf $(EXE)

$(EXE): % : %.c
	@echo "Generando $@"
	$(CC) $@.c -o $@ $(CFLAGS)
