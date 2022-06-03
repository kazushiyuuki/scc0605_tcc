automatos.o: automatos.c
	gcc -o automatos.o -c automatos.c
main.o: main.c
	gcc -o main.o -c main.c 
main: main.o automatos.o
	gcc -o main automatos.o main.o
all: main
run: all
	./main
clean: 
	rm *.o main