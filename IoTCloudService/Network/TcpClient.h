#ifndef TcpClient_H_INCLUDED
#define TcpClient_H_INCLUDED

#include <iostream>
#include <vector>
#include "../Utility/thread.h"
#include "../Utility/NetworkUtility.h"
//#include "../Utility/ErrorCode.h"

using namespace std;

class TcpClient
{
public:
	//targe port, max receive buffer size, 
	TcpClient(string targetIp,int port,int maxRecvSize);
    ~TcpClient();
	bool IsConnected;

	NetworkError Connect();
	void Disconnect();

	void SendData(char* buffer, int length);
	void SendData(string buffer);
	void SendData(std::vector<char> *buffer);

	void RecviveData(std::vector<char> *buffer);

	void StartListen();
	void StopListen();

protected:
	string targetIp;
	int serverPort, maxReceiveBuffer;	

private:
	TSSocket clientSocket;	
	TSThread clientThread;

	void initClient();
	static void client_main_loop_entry(TcpClient *clientObj);
	void client_loop();

	//Events for override by child class	
	virtual void Event_ReceivedData(std::vector<char> *data, int dataLength);
};


#endif // TcpClient_H_INCLUDED
