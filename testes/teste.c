#include <stdio.h> // Usado para reconhecer o NULL!
#include "../include/sthread.h" // Considera a estrutura de diretórios solicitada

void func0(void *arg) {

        printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));
        syield();
	return;
}

void func1(void *arg) {

      printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
}

int main(int argc, char *argv[]) {

    int	id0, id1, id2, id3;
	int i;

    id0 = screate(0, func0, (void *)&i);
    id1 = screate(1, func1, (void *)&i);
    id2 = screate(0, func1, (void *)&i);

    printf("\n\nTeste... criou threads (%d), (%d), (%d)\n", id0, id1, id2);

    swait(id2);
    id3 = screate(0, func1, (void *)&i);
    swait(id1);
    swait(id2);

    //printf("Eu sou a main voltando para terminar o programa\n");
    return 0;
}
