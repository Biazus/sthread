#include "../include/sdata.h"

//************ "SISTEMA"
void mainThreadFunc()
{
	printf("EU sou a thread main!!");
}

void initialize()
{
	TCB mainThread;
	char *mainStack;
	mainStack = (char*) malloc(SIGSTKSZ*sizeof(char));
	getcontext(&mainCtx); 
	mainCtx.uc_link = &mainCtx;
	mainCtx.uc_stack.ss_sp = mainStack;
	mainCtx.uc_stack.ss_size = SIGSTKSZ*sizeof(char);
	mainThread = createThread(0, READY, LOW_PRIORITY, mainCtx, (void*) mainThreadFunc, NULL);
	currentTid++; //incrementa o identificador de threads
	printf("\n***************************************************\n");
	printf("Thread MAIN criada com pid %d e prioridade %d\n", mainThread.tid, mainThread.prio);  //pid 0 e prioridade 2
	printf("***************************************************\n\n");
	
	runningThread.tid = 0; 
	runningThread.estado = BLOCKED;
}

//************ Tratamento de threads

TCB createThread (int tid, int state, int prio, ucontext_t context, void* (*f) (void*), void* args)
{
	TCB newThread;
	newThread.tid = tid;
	newThread.estado = state;
	newThread.prio = prio;
	newThread.canBeFinished = 1;
	newThread.tidThreadBlocked = 0;
	newThread.waitingFor = 0;
	newThread.context = context;
	newThread.f = f;
	newThread.args = args;
	if(tid==0) // main thread , nao coloca argumentos no makecontext
		makecontext(&(newThread.context), (void (*)(void)) newThread.f, 0);
	else	
		makecontext(&(newThread.context), (void (*)(void)) newThread.f, 1, newThread.args);
	return newThread;
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
		if ((newNode->thread).prio < (ptAux->thread).prio){ //se ainda no primeiro elemento e o tempo eh menor
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

TCB* findThreadById(threadList** thrList, int id){
	threadList *curr = *thrList;
	while(curr != NULL && curr->thread.tid != id){ 
			curr = curr->next;
	}	
	return &curr->thread;
}

//************ UTILIDADES

void printList(threadList* thrList)//imprime info de elementos da lista
{
	threadList* ptAux = thrList;
	
	while(ptAux != NULL)
	{
		printThreadInfo(ptAux->thread);
		ptAux = ptAux->next;
		printf(" - ");
	}
	printf("Tamanho da lista: %d\n", sizeList(thrList));
}

void printThreadInfo(TCB thread)
{
	if(thread.tid >= 0)
		printf("[TID: %d, Prio: %d]", thread.tid, thread.prio);
}

void printCurrentState()
{
	printf("\nLISTA DE APTOS:\n   ");
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
