#include "../include/sdata.h"

//************ "SISTEMA"
void scheduler()
{
	getcontext(schedulerCtx);
	printf("EU sou o escalonador!! Devo escalonar.\n");
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
	
	runningThread = (TCB *)malloc(sizeof(TCB));
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

threadList* insertThreadTop(threadList* thrList, TCB thr) //insere thread no inicio da lista
{
	threadList* newNode = (threadList*) malloc(sizeof(threadList));
	newNode->thread = thr;
	newNode->next = thrList;
	return newNode;
}

/*TCB* findThreadById(threadList** thrList, int id){
	threadList *curr = *thrList;
	while(curr != NULL && curr->thread.tid != id){ 
			curr = curr->next;
	}	
	printf("AQUI %d %d",curr->thread.prio, curr->thread.tid);
	return &curr->thread;
}*/

threadList* findThreadById(threadList** thrList, int id){

    threadList *ptr = *thrList;
    threadList *tmp = NULL;
    int found = 0;

    while(ptr != NULL)
    {
        if(ptr->thread.tid == id)
        {
            found = 1;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(found)
    {
        if(thrList)
            *thrList = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
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
	printf("Tamanho da lista: %d\n\n", sizeList(thrList));
}

void printThreadInfo(TCB thread)
{
	if(thread.tid >= 0)
		printf("\t[TID: %d, Prio: %d, Waiting: %d]\n", thread.tid, thread.prio, thread.waitingFor);
}

void printCurrentState()
{
	printf("\nRODANDO:\n ");
	printf("TID: %d, Prio: %d, Waiting: %d\n", runningThread->tid, runningThread->prio, runningThread->waitingFor);
	printf("LISTA DE APTOS:\n   ");
	printList(list_ready);
	printf("LISTA DE BLOQUEADOS:\n   ");
	printList(list_blocked);
}

int sizeList(threadList* thrList) //retorna tamanho da lista
{
    int i=0;
    while(thrList != NULL){
         i++;
         thrList = thrList->next;}
    return i;
}
