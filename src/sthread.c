#include "../include/sthread.h"

int screate (int prio, void (*start)(void*), void *arg){

	if(currentTid==0){  //inicializa thread main
		initialize();
	}
	if(totalThreads < MAXTHREADS) //se nao excedeu o total de threads
	{
		TCB newThread;
		ucontext_t newCtx;
		char *stack;
		stack = (char*) malloc(SIGSTKSZ*sizeof(char));
		getcontext(&newCtx);
		newCtx.uc_link = &mainCtx;
		newCtx.uc_stack.ss_sp = stack;
		newCtx.uc_stack.ss_size = SIGSTKSZ*sizeof(char);	
		newThread = createThread(currentTid, READY, prio, newCtx, (void*) start, (void*) arg);
		currentTid++; 
		totalThreads++;
		list_ready = insertThread(list_ready, newThread); //insere nova thread em aptos
		printf("\n## SCREATE ## Criacao da Thread tid: %d\n", currentTid-1);
		printCurrentState();
		return newThread.tid; //retorna o identificador da thread
	}
	else{
		printf("!! ## SCREATE ## ERROR. Numero de threads excedeu ao total %d", MAXTHREADS);
		return ERROR;
	}

}
int syield(void){
	
	return 0;
}
int swait(int tid){
	
	TCB* threadAwaited = findThreadById(&list_ready, tid);
	if(threadAwaited->tidThreadBlocked > 0){
		printf("!! ## SWAIT ## ERROR. Thread %d ja aguarda o termino da thread %d !", threadAwaited->tidThreadBlocked, threadAwaited->tid);
		return ERROR;
		}
	else if(threadAwaited->estado != READY){
		printf("!! ## SWAIT ## ERROR. Thread %d nao esta no estado apto !", threadAwaited->tid);
		return ERROR;
	}
	else {
		runningThread.canBeFinished = 0;
		//thread chamadora é bloqueada, indo para lista de threads bloqueadas, e fica em espera
		runningThread.estado = BLOCKED;
		runningThread.waitingFor++;
		threadAwaited->tidThreadBlocked = runningThread.tid;
		list_blocked = insertThreadTop(list_blocked, runningThread);
		printf("\n## SWAIT ## Thread %d espera termino da thread %d \n", runningThread.tid, threadAwaited->tid);
		return swapcontext(&(findThreadById(&list_blocked, runningThread.tid))->context, &mainCtx); //troca o contexto da thread em espera com o do escalonador
	}
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
