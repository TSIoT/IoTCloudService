#ifndef NetworkUtility_H_INCLUDED
#define NetworkUtility_H_INCLUDED

#if defined(WIN32)
#include <windows.h>
#pragma comment(lib, "ws2_32.lib") //Winsock Library

typedef SOCKET TSSocket;
typedef TIMEVAL TimeSpan;
#define CloseTSSocket closesocket
#define PAUSE system("Pause");

#elif defined(__linux__) || defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
typedef int TSSocket;
typedef struct timeval TimeSpan;
#define CloseTSSocket close
#define PAUSE printf("Press Enter any key to continue..."); fgetc(stdin);

#endif

enum NetworkError
{
	NetworkError_NoError,
	NetworkError_UnknownError,
	NetworkError_InitError,
	NetworkError_SocketError,
	NetworkError_ConnectError,

};


#endif
