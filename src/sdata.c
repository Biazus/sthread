#include "../include/sdata.h"

TCB createThread (int tid, int state, int prio, ucontext_t context, void* (*f) (void*), void* args)
{
	TCB newThread;
	newThread.tid = tid;
	newThread.estado = state;
	newThread.prio = prio;
	newThread.canBeFinished = 1;
	newThread.joinRelease = 0;
	newThread.joinWaiting = 0;
	newThread.context = context;
	newThread.f = f;
	newThread.args = args;
	makecontext(&(newThread.context), (void (*)(void)) newThread.f, 1, newThread.args);
	return newThread;
}

void printThreadInfo(TCB thread)
{
	if(thread.tid > 0) //Teste para verificacao de validade da thread*
		printf("[TID: %d, Prio: %d]", thread.tid, thread.prio);
}

void printCurrentState()
{
	printf("Printing state\n");
}
