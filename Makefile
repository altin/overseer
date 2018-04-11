inject.o: inject.c
	gcc inject.c -o inject

inject: inject.o
	gcc inject.o -o inject

clean:
	rm -f inject
