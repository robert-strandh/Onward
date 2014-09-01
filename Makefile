all: forth

forth: Forth_system.o Forth_words.o main.o
	gcc -g -o forth $<

Forth_system.o: Forth_system.h Forth_system.h 
	gcc -Wall -W -c Forth_system.c

Forth_words.o: Forth_system.h Forth_words.h Forth_words.c
	gcc -Wall -W -c Forth_words.c

main.o: main.c
	gcc -Wall -W -c main.c

clean:
	rm -f *.o forth
