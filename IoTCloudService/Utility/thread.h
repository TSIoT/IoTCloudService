#ifndef Thread_H_INCLUDED
#define Thread_H_INCLUDED

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

typedef int(*TSThreadProc)(void *);

#ifdef _WIN32
typedef HANDLE TSThread;
#else
typedef pthread_t TSThread;
typedef void* (*PTHREAD_START_ROUTINE)(void *);
#endif

int Thread_create(TSThread *t, TSThreadProc func, void *arg);
int Thread_run(TSThread *t);
int Thread_stop(TSThread *t);
int Thread_kill(TSThread *t);
void Thread_wait(int ms);

//*---------------------------------------------------------------------
//Explanation: Mutex
//*---------------------------------------------------------------------
#ifdef _WIN32
typedef HANDLE TSMutex;
#else
typedef pthread_mutex_t TSMutex;
#endif	// _WIN32

int Mutex_create(TSMutex *m);
int Mutex_lock(TSMutex *m);
int Mutex_unlock(TSMutex *m);
int Mutex_trylock(TSMutex *m);
void Mutex_free(TSMutex *m);

#endif
