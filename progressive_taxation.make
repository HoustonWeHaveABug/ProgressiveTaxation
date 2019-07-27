PROGRESSIVE_TAXATION_C_FLAGS=-O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

progressive_taxation: progressive_taxation.o
	gcc -o progressive_taxation progressive_taxation.o -lm

progressive_taxation.o: progressive_taxation.c progressive_taxation.make
	gcc -c ${PROGRESSIVE_TAXATION_C_FLAGS} -o progressive_taxation.o progressive_taxation.c

clean:
	rm -f progressive_taxation progressive_taxation.o
