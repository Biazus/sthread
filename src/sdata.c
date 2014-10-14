#include "../include/sdata.h"

//************ "SISTEMA"

//para fazer assignment de uma thread de removeThread proceder assim:
// *runningThread = removeThread(&list_ready);

void scheduler()
{
	getcontext(schedulerCtx);
	printf("\nCONTEXTO DO ESCALONADOR.\n");
	while(list_ready){
		*runningThread = removeThread(&list_ready);
	
		printCurrentState();
		swapcontext(schedulerCtx,runningThread->context);
		//testar aqui se a thread que terminou nao desbloqueia alguma 
		if(runningThread->bloqueando != -1 && !checkThreadExists(&list_ready, runningThread->tid)){ //significa q ela esta bloqueando alguma
			runningThread->bloqueando = -1;
			removeThreadBlocked(&list_blocked, runningThread->tid);//remover
		}
			
		}
	while(runningThread!=NULL)
		swapcontext(schedulerCtx,runningThread->context);
	printCurrentState();
}
void threadFinalizada()
{
	setcontext(schedulerCtx);
}

ucontext_t *novoContexto()
{
	//Cria um PCB para o novo processo
	ucontext_t *novoContext = (ucontext_t *)malloc( sizeof( ucontext_t ) );

    novoContext->uc_stack.ss_sp   = (char *)malloc(SIGSTKSZ);
    novoContext->uc_stack.ss_size = SIGSTKSZ;

    return novoContext;
}

void initialize()
{
	list_blocked= NULL;
	list_ready= NULL;
	started = 1;
	currentTid++;
	
	invalidThread = (TCB *)malloc(sizeof(TCB));
	
	runningThread = (TCB *)malloc(sizeof(TCB));
	runningThread->bloqueando = -1;
	runningThread->prio = 2;
	runningThread->context = novoContexto();
	(runningThread->context)->uc_link = NULL;
	
	schedulerCtx = novoContexto();
	schedulerCtx->uc_link = runningThread->context;
	getcontext(schedulerCtx);
	makecontext(schedulerCtx,(void (*)(void))scheduler, 0);
	
	finalizadaCtx = novoContexto();
	finalizadaCtx->uc_link = schedulerCtx;
	getcontext(finalizadaCtx);
	makecontext(finalizadaCtx,(void (*)(void))threadFinalizada, 0);
	
	getcontext(runningThread->context);
}


threadList* insertThread(threadList* thrList, TCB thr) //insere thread por prioridade
{	
	threadList* newNode = (threadList*) malloc(sizeof(threadList));
	newNode->thread = thr;
	newNode->next = NULL;

	if(thrList == NULL){
		thrList = newNode;
		return thrList;
	}

	threadList* ptAux = thrList;

	while(ptAux->next != NULL && ((ptAux->next->thread).prio <= (newNode->thread).prio)){
		ptAux = ptAux->next;
	}
	if(ptAux == thrList){
		if ((newNode->thread).prio < (ptAux->thread).prio){ //se ainda no primeiro element
			newNode->next = ptAux;
			thrList = newNode;	
		}
		else{
			newNode->next = ptAux->next;
			ptAux->next = newNode;
		}
	}
	else{
		newNode->next = ptAux->next;
		ptAux->next = newNode;
	}
	return thrList;
}

threadList* insertThreadTop(threadList* thrList, TCB thr) //insere thread no inicio da lista (nao sei se vou usar mas ne)
{
	threadList* newNode = (threadList*) malloc(sizeof(threadList));
	newNode->thread = thr;
	newNode->next = thrList;
	return newNode;
}

TCB removeThread(threadList** thrList)
{
     threadList* curr = (*thrList);
     TCB head = curr->thread;
     *thrList = curr->next;
     return head;
}


/*TCB* findThreadById(threadList** thrList, int id){
	threadList *curr = *thrList;
	while(curr != NULL && curr->thread.tid != id){ 
			curr = curr->next;
	}	
	printf("AQUI %d %d",curr->thread.prio, curr->thread.tid);
	return &curr->thread;
}*/

TCB removeThreadBlocked(threadList** thrList, int tid){
	
	threadList *curr = *thrList;
	threadList *prev = NULL;
	TCB th = curr->thread;
	if (curr->next == NULL){
		th.waitingFor = -1;
		*thrList = curr->next;
		list_ready = insertThread(list_ready, th);
		return th;
	}
	//refatorar a partir daqui
	else{
		while(curr->next != NULL && curr->thread.waitingFor != tid){ 
				prev = curr;
				curr = curr->next;
		}
	}
	return curr->thread;
}


int checkThreadExists(threadList** thrList, int tid){
	threadList *curr = *thrList;
	while(curr != NULL && curr->thread.tid != tid)
		curr = curr->next;
	if (curr!= NULL)
		return 1;
	else return 0;	
}

TCB* searchThreadById(threadList** thrList, int id){
	threadList *curr = *thrList;
	if(thrList == NULL){
		invalidThread->tid = -1;
		return invalidThread;
	}
	while(curr != NULL && curr->thread.tid != id){ 
		if (curr->next != NULL)
			curr = curr->next;
		else printf("AFFF");
	}	
	return &curr->thread;
}

//cria lista de threads
threadList* createThreadsList()
{
	return NULL;
}

//************ UTILIDADES

void printList(threadList* thrList)//imprime info de elementos da lista
{
	threadList* ptAux = thrList;
	
	while(ptAux != NULL)
	{
		printThreadInfo(ptAux->thread);
		ptAux = ptAux->next;
		printf("  ");
	}
	printf("\tTamanho da lista: %d\n\n", sizeList(thrList));
}

void printThreadInfo(TCB thread)
{
	if(thread.tid >= 0)
		printf("\t[TID: %d, Prio: %d, Waiting: %d, Bloqueando: %d]\n", thread.tid, thread.prio, thread.waitingFor, thread.bloqueando);
}

void printCurrentState()
{
	printf("\nRODANDO:\n");
	printf("\tTID: %d, Prio: %d, Waiting: %d, Bloqueando: %d\n", runningThread->tid, runningThread->prio, runningThread->waitingFor, runningThread->bloqueando);
	printf("LISTA DE APTOS:\n   ");
	printList(list_ready);
	printf("LISTA DE BLOQUEADOS:\n   ");
	printList(list_blocked);
	printf("\n\n");
}

int sizeList(threadList* thrList) //retorna tamanho da lista
{
    int i=0;
    while(thrList != NULL){
         i++;
         thrList = thrList->next;}
    return i;
}
