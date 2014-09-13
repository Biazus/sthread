#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
# 
all: libsthread sthread.o sdata.o

libsthread: sthread.o sdata.o
	ar crs ./lib/libsthread.a ./bin/sdata.o ./bin/sthread.o

sthread.o: ./src/sthread.c sdata.o
	gcc -c ./src/sthread.c -o ./bin/sthread.o
	
sdata.o: ./src/sdata.c
	gcc -c ./src/sdata.c -o ./bin/sdata.o

clean:
	rm -rf ./bin/*.o
	rm -rf ./lib/libsthread.a
