#include "../include/sthread.h"

int screate (int prio, void (*start)(void*), void *arg){

	if(totalThreads < MAXTHREADS) //se nao excedeu o total de threads
	{
		TCB newThread;
		ucontext_t newCtx;
		//Cria o contexto da nova thread
		char *stack;
		stack = (char*) malloc(SIGSTKSZ*sizeof(char));
		getcontext(&mainCtx);
		newCtx.uc_link = &mainCtx;
		newCtx.uc_stack.ss_sp = stack;
		newCtx.uc_stack.ss_size = SIGSTKSZ*sizeof(char);
		
		newThread = createThread(currentTid, READY, prio, newCtx, (void*) start, (void*) arg);
		currentTid++; //incrementa o identificador de threads
		totalThreads++; //incrementa o total de threads
		//list_ready = insertThread(list_ready, newThread); //inserir nova thread em aptos
		printf("Criacao da Thread tid: %d\n", currentTid-1);
		printCurrentState();
		return newThread.tid; //retorna o identificador da thread
	}
	else
		return ERROR; //caso exceda o total de threads, retorna erro

}
int syield(void){
	return 0;
}
int swait(int tid){
	return 0;
}
int smutex_init(smutex_t *mtx){
	return 0;
}
int slock (smutex_t *varmutex){
	return 0;
}
int sunlock (smutex_t *varmutex){
	return 0;
}
