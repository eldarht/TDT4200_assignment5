

CC=gcc
FLAGS := -O3
LINKING := -lblas -fopenmp


.PHONY: serial omp blas omp_test clean

main: main.c
	$(CC) $(FLAGS) $< -o $@ $(LINKING)

serial: main
	./main s

omp: main
	./main o

blas: main
	./main b

omp_test: main
	./main o t

clean:
	rm -f main
