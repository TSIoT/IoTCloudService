#ifndef IoTCloudService_H_INCLUDED
#define IoTCloudService_H_INCLUDED

#include "Network/TcpServer.h"
#include "Utility/JsonUtility.h"
#include <vector>
#include <iostream>
#include <sstream>


using namespace std;


class IoTCloudService:public TcpServer
{
    //const string webServerIp="http://192.168.156.92/PairInterface/pairInterface.php";
	const string webServerIp = "http://104.199.183.220/login.php";
public:

    IoTCloudService(int port,int maxReceiveBuffer, int maxClient);
    ~IoTCloudService();
    NetworkError StartIoTService();
    void StopIoTService();


private:
    enum ConnType{ConnType_Unknown, ConnType_Manager, ConnType_EndNode};
    enum IdCheck{IdCheck_PASS, IdCheck_ID_FAILED, IdCheck_PW_FAILED, IdCheck_Timeout};
    enum PackageType{PackageType_Illegal,PackageType_Unknown, PackageType_REGISTER, PackageType_RESPONSE, PackageType_PairedConnectin};

	/*
    typedef struct
    {
        string userID;
        ConnType connType;
        int pairIndex;
        //char *tempBuffer;
		vector<char> buffer;
        //int receiveCount;
        int errorCount;
    } ConnInfo;
	*/
	class ConnInfo
	{
	public:
		string userID;
		ConnType connType;
		int pairIndex;
		//char *tempBuffer;
		vector<char> buffer;
		//int receiveCount;
		int errorCount;
	};

    //ConnInfo *connInfo;
    std::vector<ConnInfo> *connInfo;
    //JsonUtility jsonUtility;
    const int maxErrorTimes=3;
    std::stringstream readBuffer;

    //package handle
    void clearConnInfo(int index);
    //void clearConnInfoOnlyBuffer(int index);
    void registerProcess(int socketIndex);
    PackageType checkPackageType(int socketIndex);
    int findPairIndex(int connInfoIndex);
    void clearDuplicateId(string ID, ConnType type);


    //communication with php server
    //std::string readBuffer;
    IdCheck curl_check_id_pw(string id, string pw);
    static size_t recvWebCallback(char *contents, size_t size, size_t nmemb, void *userp);

	//Event handler
	//void Event_ReceivedData(int socketIndex, char* data, int dataLength);
	void Event_ReceivedData(int socketIndex, std::vector<char> *buffer, int dataLength);
	void Event_NewConnection(int socketIndex);
	void Event_ConnectionDenied(int socketIndex);
};







#endif



