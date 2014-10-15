
/*
 *	Programa de exemplo de uso da biblioteca sthread
 *
 *	Versão 1.0 - 20/08/2014
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */

#include "../include/sthread.h"

smutex_t* varMutex;

void func0(void *arg) {
	slock(varMutex);
    printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));
    syield();
    sunlock(varMutex);
	return;
}

void func1(void *arg) {
	  slock(varMutex);
      printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
}

int main(int argc, char *argv[]) {

    int	id0, id1, id2;
	int i;
	varMutex = malloc(sizeof(smutex_t));
	smutex_init(varMutex);	
	
	
    id0 = screate(2, func0, (void *)&i);
    id1 = screate(1, func1, (void *)&i);
    id2 = screate(2, func0, (void *)&i);

    printf("Eu sou a main após a criação de ID0 e ID1\n");

    swait(0);
    swait(1);

    printf("Eu sou a main voltando para terminar o programa\n");
}

