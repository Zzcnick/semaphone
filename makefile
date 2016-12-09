GCC = gcc -g

all: semaphone.o scntl.o
	$(GCC) -o scntl.out scntl.o && $(GCC) -o semaphone.out semaphone.o

semaphone.o: semaphone.c
	$(GCC) -c semaphone.c

scntl.o: scntl.c
	$(GCC) -c scntl.c

run: semaphone.out
	./semaphone.out

clean:
	rm -f *~ *.out *.o *.txt
