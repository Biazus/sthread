#include "../include/sthread.h"

int screate (int prio, void (*start)(void*), void *arg){

	if(prio < 0 || prio > 2 ){
		printf("!! ############### SCREATE ## ERROR. Prioridade invalida");
		return ERROR;
	}
	if(started==0){  //inicializa
		initialize();
	}
	if(totalThreads < MAXTHREADS) //se nao excedeu o total de threads
	{
		TCB *newThread = (TCB *)malloc( sizeof( TCB ) );;
		newThread->context = novoContexto();
		newThread->context->uc_link = schedulerCtx;
		newThread->tid = currentTid;
		newThread->estado = READY;
		newThread->prio = prio;
		newThread->waitingFor = -1;
		newThread->bloqueando = -1;
		
		getcontext(newThread->context);
		makecontext(newThread->context,(void (*)(void))start,1,arg);
		
		list_ready = insertThread(list_ready, *newThread); //insere nova thread em aptos
		
		currentTid++; 
		totalThreads++;
		printf("\n############### SCREATE ## Criacao da Thread tid: %d ############### \n", currentTid-1);
		printCurrentState();
		return newThread->tid; //retorna o identificador da thread
	}
	else{
		printf("!! ############### SCREATE ## ERROR. Numero de threads excedeu ao total %d", MAXTHREADS);
		return ERROR;
	}

}
int syield(void){
	printf("\n############### SYIELD #################### \n");
	if (list_ready !=NULL){
		list_ready = insertThread(list_ready, *runningThread);
		swapcontext(runningThread->context, schedulerCtx);
	}
	return 0;
}

int swait(int tid){
	printf("\n############### SWAIT ## Thread espera tid: %d ############### \n", tid);
	if (checkThreadExists(&list_ready, tid)){
		searchThreadById(&list_ready, tid)->bloqueando = runningThread->tid;
		runningThread->waitingFor=tid;
		list_blocked=insertThread(list_blocked, *runningThread);

		printCurrentState();
		swapcontext(runningThread->context, schedulerCtx);
	}
	else{ 
		printf("\nTentando dar wait em thread ja terminada :)\n");
		swapcontext(runningThread->context, schedulerCtx);
	}
	return 0;
}

int smutex_init(smutex_t *mtx){
	if(mtx != NULL ){
		mtx->flag = 1; //secao critica livre;
		mtx->next = createThreadsList(); //nenhuma thread esperando
		return 0;
	}
	else return ERROR;
}
int slock (smutex_t *mtx){
	/*if(mtx != NULL){
		if(mtx->flag == 1){//entrada permitida na secao critica
			mtx->flag = 0;
			printf("\nEntrada permitida da thread %d na secao critica\n", runningThread->tid);
			return 0;
		}
		else{
			mtx->next = insertThread(mtx->next, *runningThread);
			runningThread->estado = BLOCKED;
			list_blocked = insertThread(list_blocked, *runningThread);
			printf("\nEntrada negada, thread %d, bloqueada\n", runningThread->tid);
			return swapcontext(runningThread->context, &schedulerCtx);
		}	
	}
	else return ERROR;
	* */
	return 0;
}
int sunlock (smutex_t *mtx){
	/*if(mtx != NULL && mtx->next != NULL){
		thread_t* thr = removeThreadById(&blocked, mtx->next->thread.tid); //remove a thread que estava esperando da lista de bloqueados
		mtx->next->thread.estado = READY;
		mtx->next->thread.context = thr->context; //recupera o contexto salvo quando foi bloqueada
		list_ready = insertThread(list_ready, removeThread(&mtx->next)); //insere na lista de aptos
		if(mtx->next == NULL) //nenhuma thread esperando
			mtx->flag = 1;
	printf("\nUnlock libera thread %d", thr->tid);
	return 0;
	
	}
	else return ERROR;*/
}
