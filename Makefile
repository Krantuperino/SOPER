CC = gcc
CFLAGS = -lpthread -lm -g
EXE = ejercicio3 ejercicio4 ejercicio5 ejercicio7 ejercicio9 ejercicio12

all: $(EXE)
test: test3 test4 test5 test7 test9 test12

clean:
	rm -rf $(EXE)

$(EXE): % : %.c
	@echo "Generando $@"
	$(CC) $@.c -o $@ $(CFLAGS)

test3:
	@echo "Testing $@"
	@./ejercicio3
	@echo ""

test4:
	@echo "Testing $@"
	@./ejercicio4
	@echo ""

test5:
	@echo "Testing $@"
	@./ejercicio5
	@echo ""

test7:
	@echo "Testing $@"
	@./ejercicio7 < input7
	@echo ""

test9:
	@echo "Testing $@"
	@./ejercicio9
	@echo ""

test12:
	@echo "Testing $@"
	@./ejercicio12
	@echo ""