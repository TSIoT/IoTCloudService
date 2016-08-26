#ifndef TCPSERVER_H_INCLUDED
#define TCPSERVER_H_INCLUDED

using namespace std;
#include <iostream>
#include <vector>

#include "../Utility/thread.h"
#include "../Utility/NetworkUtility.h"


class TcpServer
{
public:
    TcpServer(int port,int maxSize, int maxSockets);
    ~TcpServer();

protected:
    int serverPort,maxReceiveBuffer,maxClient;

    void StartServer();
    void StopServer();
    TSSocket GetSocket(int socketIndex);
    void CutConnection(int socketIndex);
    void SendDataToExistsConnection(int socketIndex,char* buffer, int length);
    void SendDataToExistsConnection(int socketIndex,string buffer);
    void SendDataToExistsConnection(int socketIndex,std::vector<char> *buffer);


private:
    TSSocket listener;
    //TSSocket *clientSockets;
	std::vector<TSSocket> *clientSockets;
    TSThread serverThread;
    fd_set master;
    NetworkError initServer();
    void uninit();
    //static void main_loop_entry(TcpServer serverObj);
	static void server_main_loop_entry(TcpServer *serverObj);
    int server_loop();

    //virtual void Event_ReceivedData(int socketIndex, char* buffer, int dataLength);
	virtual void Event_ReceivedData(int socketIndex, std::vector<char> *buffer,int dataLength);
    virtual void Event_NewConnection(int socketIndex);
    virtual void Event_ConnectionDenied(int socketIndex);
};



#endif // TCPSERVER_H_INCLUDED
