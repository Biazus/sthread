all: teste libsthread sthread.o sdata.o

teste: libsthread
	gcc -g -o ../bin/teste teste_vetor.c -L../lib -lsthread -Wall -lrt

libsthread: sthread.o sdata.o
	ar crs ../lib/libsthread.a ../bin/sdata.o ../bin/sthread.o

sthread.o: ../src/sthread.c sdata.o
	gcc -c ../src/sthread.c -o ../bin/sthread.o
	
sdata.o: ../src/sdata.c
	gcc -c ../src/sdata.c -o ../bin/sdata.o

clean:
	rm -rf ../bin/*.o
	rm -rf ../lib/libsthread.a
