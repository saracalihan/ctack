FLAGS= -Wall -Wextra -ggdb

all: compile run
compile: src/ctack.c
	cc -o build/ctack ${FLAGS} src/ctack.c
run: build/ctack
	./build/ctack examples/sum.ctk
