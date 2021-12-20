just-compile: compile
all: compile run clean
compile: phil.c 
	gcc -Wall -g -o phil phil.c -lpthread
run:
	./phil
clean: 
	rm phil