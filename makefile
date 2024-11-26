myProgram : main.o operations.o Kdam_Assembler.o twoPasses.o
	gcc -g -Wall -ansi main.o Kdam_Assembler.o operations.o twoPasses.o -o myProgram

main.o : main.c header.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o

operations.o : operations.c header.h
	gcc -c -Wall -ansi -pedantic operations.c -o operations.o

Kdam_Assembler.o : Kdam_Assembler.c header.h
	gcc -c -Wall -ansi -pedantic Kdam_Assembler.c -o Kdam_Assembler.o

twoPasses.o : twoPasses.c header.h
	gcc -c -Wall -ansi -pedantic twoPasses.c -o twoPasses.o


