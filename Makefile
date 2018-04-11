overseer.o: overseer.c
	gcc overseer.c -o overseer

overseer: overseer.o
	gcc overseer.o -o overseer

clean:
	rm -f overseer
