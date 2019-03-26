CC = gcc
CFLAGS = -lpthread -g
EXE = ejercicio2 ejercicio3 ejercicio4 ejercicio6a ejercicio6b ejercicio8 ejercicio9

all: $(EXE)
test: test2 test3 test4 test6a test6b test8 test9

clean:
	rm -rf $(EXE)

$(EXE): % : %.c
	@echo "Generando $@"
	$(CC) $@.c -o $@ $(CFLAGS)

test2:
	@echo "Testing $@"
	@./ejercicio2
	@echo ""

test3:
	@echo "Testing $@"
	@./ejercicio3
	@echo ""

test4:
	@echo "Testing $@"
	@./ejercicio4
	@echo ""

test6a:
	@echo "Testing $@"
	@./ejercicio6a
	@echo ""

test6b:
	@echo "Testing $@"
	@./ejercicio6b
	@echo ""

test8:
	@echo "Testing $@"
	@./ejercicio8
	@echo ""

test9:
	@echo "Testing $@"
	@./ejercicio9
	@echo ""