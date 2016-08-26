#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#ifdef __linux__
	#define _THREAD
#endif
//*---------------------------------------------------------------------
//Explanation: Thread
//*---------------------------------------------------------------------
int Thread_create(TSThread *t, int(*func)(void *), void *arg)
{
#ifdef _WIN32
	DWORD z;
	*t = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)func, (LPVOID)arg, CREATE_SUSPENDED, &z);
	return (*t != NULL);
#else
#ifdef __linux__
#ifdef _THREAD
	if (pthread_create(t, NULL, (PTHREAD_START_ROUTINE)func, arg) == 0)
		return (pthread_detach(*t) == 0);

	return 0;
#else
#ifdef _FORK
	switch (fork())
	{
	case 0:
		(*func)(arg);
		exit(0);
	case (-1) :
		return FALSE;
	}

	return TRUE;
#else
	(*func)(arg);
	return 1;
#endif	// _FORK
#endif	// _THREAD
#else
	(*func)(arg);
	return TRUE;
#endif
#endif	// _WIN32
}

int Thread_run(TSThread *t)
{
#ifdef _WIN32
	return (ResumeThread(*t) != 0xFFFFFFFF);
#else
	return 1;
#endif	// _WIN32
}

int Thread_stop(TSThread *t)
{
#ifdef _WIN32
	return (SuspendThread(*t) != 0xFFFFFFFF);
#else
	return 1;
#endif	// _WIN32
}

int Thread_kill(TSThread *t)
{
#ifdef _WIN32
	return (TerminateThread(*t, 0) != 0);
#else
	//return (pthread_kill(*t)==0);
	return 1;
#endif	// _WIN32
}

void Thread_wait(int ms)
{
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif	// _WIN32
}

//*---------------------------------------------------------------------
//Explanation: Mutex
//*---------------------------------------------------------------------
int Mutex_create(TSMutex *m)
{
#ifdef _THREAD
	return (pthread_mutex_init(m, NULL) == 0);
#elif defined (_WIN32)
	return ((*m = CreateMutex(NULL, FALSE, NULL)) != NULL);
#else
	return 1;
#endif
}

int Mutex_lock(TSMutex *m)
{
#ifdef _THREAD
	return (pthread_mutex_lock(m) == 0);
#elif defined (_WIN32)
	return (WaitForSingleObject(*m, INFINITE) != WAIT_TIMEOUT);
#else
	return 1;
#endif
}

int Mutex_unlock(TSMutex *m)
{
#ifdef _THREAD
	return (pthread_mutex_unlock(m) == 0);
#elif defined(_WIN32)
	return ReleaseMutex(*m);
#else
	return 1;
#endif
}

int Mutex_trylock(TSMutex *m)
{
#ifdef _THREAD
	return (pthread_mutex_trylock(m) == 0);
#elif defined(_WIN32)
	return (WaitForSingleObject(*m, 0) != WAIT_TIMEOUT);
#else
	return 1;
#endif
}

void Mutex_free(TSMutex *m)
{
#ifdef _THREAD
	pthread_mutex_destroy(m);
#elif defined (_WIN32)
	CloseHandle(*m);
#else
	;
#endif
}
