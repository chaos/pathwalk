all: pathwalk

pathwalk: err.o pathwalk.o
	$(CC) -o $@ err.o pathwalk.o

clean:
	rm -f pathwalk *.o
