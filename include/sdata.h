#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

// DEFINES
#define BILLION 1E9

#define ERROR -1 //erro na execucao de funcao
#define MAXTHREADS 100 //maximo de threads

#define READY 1 //thread pronta
#define RUNNING 0 //thread executando
#define BLOCKED -1 //thread bloqueada

#define LOW_PRIORITY 2
#define MEDIUM_PRIORITY 1
#define HIGH_PRIORITY 0

// ************************************** TYPEDEFS
 
 typedef struct tcb {
	int tid; //id da thread
	int estado; 
	int prio; //prioridade da thread ( 0 == alta, 1 == media, 2 == baixa)
	int canBeFinished; //indica se thread pode ser finalizada
	int tidThreadBlocked; //qual thread precisa ser liberada com o termino desta thread
    int waitingFor; //quantas threads esta thread esta esperando terminar
	ucontext_t context; //contexto da thread
	void* (*f) (void*); //funcao que a thread executa
	void* args; //argumento(s) da funcao
	//struct tcb *next; //ponteiro para o proximo elemento da lista (poderia ser feita uma lista de threads)
 } TCB;
 
struct tList {
	TCB thread; //elemento thread
	struct tList *next; //ponteiro para o proximo elemento da lista
};
typedef struct tList threadList; //define o tipo lista
 
 
typedef struct mutex {
	int flag;
    TCB *first;
	TCB *last;
} smutex_t;

// ************************************* FUNCS

void mainThreadFunc();
void initialize();

TCB createThread (int tid, int state, int prio, ucontext_t context, void* (*f) (void*), void* args);
threadList* insertThread(threadList* thrList, TCB thr);
TCB* findThreadById(threadList** thrList, int id);
threadList* insertThreadTop(threadList* thrList, TCB thr);

void printThreadInfo(TCB thread);
void printCurrentState(void);
void printList(threadList* thrList);
int sizeList(threadList* thrList);

// ************************************* VARS

int totalThreads; //numero total de thread (thread main nao inclusa)
int currentTid; //tid do ultimo elemento inserido na lista para controlar proximo tid

TCB runningThread; //thread em estado running

threadList* list_ready; //lista de threads com estado ready
threadList* list_blocked; //lista de threads com estado blocked

ucontext_t mainCtx; //contexto do escalonador
