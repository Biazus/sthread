#include <stdio.h> // Usado para reconhecer o NULL!
#include "../include/sthread.h" // Considera a estrutura de diretórios solicitada

void func0(void *arg) {

        printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));
	return;
}

void func1(void *arg) {

      printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
}

int main(int argc, char *argv[]) {

    int	id0, id1;
	int i;

    id0 = screate(0, func0, (void *)&i);
    id1 = screate(1, func1, (void *)&i);

    printf("Eu sou a main após a criação de ID0 e ID1\n");

    //swait(id0);
    //swait(id1);

    printf("Eu sou a main voltando para terminar o programa\n");
    return 0;
}
