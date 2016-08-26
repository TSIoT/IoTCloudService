#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<iostream>

#include "IoTCloudService.h"
#include "Utility/JsonUtility.h"
#include "Utility/file.h"
//#include "Utility/IoTUtility.h"


#if defined(WIN32)
#pragma comment(lib,"curl/libcurl.lib")
#include "curl/curl.h"

#elif defined(__linux__)
#include <curl/curl.h>

#endif


//std::string IoTCloudService::readBuffer;
//public member
IoTCloudService::IoTCloudService(int port,int maxReceiveBuffer, int maxClient):TcpServer(port,maxReceiveBuffer,maxClient)
{
    cout << "IoTCloudService(int port,int maxReceiveBuffer, int maxClient) has be init" << endl;

    int i=0;
    //this->connInfo=new ConnInfo[this->maxClient];
    this->connInfo=new std::vector<ConnInfo>(this->maxClient);
    for(i=0;i<this->maxClient;i++)
    {
        //this->connInfo[i].tempBuffer=new char[maxReceiveBuffer];
		this->connInfo->at(i).buffer.reserve(this->maxReceiveBuffer);
        //this->clearConnInfo(this->connInfo[i]) ;
    }
}

IoTCloudService::~IoTCloudService()
{
    cout <<"Service closed!"<<endl;
	/*
    int i=0;
    for(i=0;i<this->maxClient;i++)
    {
        delete[] this->connInfo[i].tempBuffer;
    }
    delete[] this->connInfo;
	*/
}

NetworkError IoTCloudService::StartIoTService()
{
    //this->tcpServer->StartServer();
    this->StartServer();
    return NetworkError_NoError;
}

void IoTCloudService::StopIoTService()
{
    //this->tcpServer->StopServer();
    this->StopServer();
}

// private member



//package handle
void IoTCloudService::clearConnInfo(int index)
{
    //memset(this->connInfo[index].tempBuffer,0,this->maxReceiveBuffer);
    //this->connInfo[index].receiveCount=0;

	this->connInfo->at(index).buffer.clear();
    this->connInfo->at(index).connType=ConnType_Unknown;

    if(this->connInfo->at(index).pairIndex>=0)
    {

        this->connInfo->at(this->connInfo->at(index).pairIndex).pairIndex=-1;
        //this->connInfo[this->connInfo[index].pairIndex].pairIndex=-1;
    }

    this->connInfo->at(index).pairIndex=-1;
    this->connInfo->at(index).userID="";
    this->connInfo->at(index).errorCount=0;
}

/*
void IoTCloudService::clearConnInfoOnlyBuffer(int index)
{
    //memset(this->connInfo[index].tempBuffer,0,this->maxReceiveBuffer);
    std::fill_n(this->connInfo[index].tempBuffer,this->maxReceiveBuffer,'\0');
    this->connInfo[index].receiveCount=0;
    this->connInfo[index].errorCount=0;
}
*/

void IoTCloudService::registerProcess(int socketIndex)
{
    json_t *root;
    //root= json_loads(connInfo[socketIndex].tempBuffer, 0, NULL);

	root = JsonUtility::LoadJsonData(&this->connInfo->at(socketIndex).buffer);

    string userType,userId,userPw;
    string unknownTypeMsg="The login type is unknown";
    string loginFiledMsg="Login failed";
    string loginTimeoutMsg="Login server timeout";
    string loginSuccessMsg="Success";

    ConnType registerConnType=ConnType_Unknown;

    int pairIndex=-1;

    //this->jsonUtility.SetValueInFirstObject(root,string("ID"),string("CCC"));
    //this->jsonUtility.PrintJasonData(connInfo[socketIndex].tempBuffer);

    userId= JsonUtility::GetValueInFirstObject(root,string("ID"));
    userPw= JsonUtility::GetValueInFirstObject(root,string("PW"));

    cout << "User ID:"<<  userId<<endl;
    cout << "User PW:"<<  userPw<<endl;

    IdCheck checkResult=this->curl_check_id_pw(userId,userPw);

    switch(checkResult)
    {
    case IdCheck_PASS:
        cout << "Check passed" << endl;
        userType="";
        userType= JsonUtility::GetValueInFirstObject(root,string("TYPE"));

        if(userType.compare("MANAGER")==0)
        {
            cout << "A manager registered"<<endl;
            registerConnType=ConnType_Manager;
        }
        else if(userType.compare("END_NODE")==0)
        {
            cout << "A end-node registered"<<endl;
            registerConnType=ConnType_EndNode;
        }
        else
        {
            cout << "A unknown registered"<<endl;
            this->SendDataToExistsConnection(socketIndex,unknownTypeMsg);
            this->CutConnection(socketIndex);
        }

        if(registerConnType!=ConnType_Unknown)
        {
            this->clearDuplicateId(userId,registerConnType);

            this->connInfo->at(socketIndex).userID=userId;
            this->connInfo->at(socketIndex).connType=registerConnType;
            pairIndex=this->findPairIndex(socketIndex);
            if(pairIndex>=0)
            {
                this->connInfo->at(socketIndex).pairIndex=pairIndex;
                this->connInfo->at(pairIndex).pairIndex=socketIndex;
            }

            this->SendDataToExistsConnection(socketIndex,loginSuccessMsg);

            cout << "Register finished" << endl;
        }
        break;

    case IdCheck_FAILED:
        cout << "Failed" << endl;
        this->SendDataToExistsConnection(socketIndex,loginFiledMsg);
        this->CutConnection(socketIndex);
        break;

    case IdCheck_Timeout:
        cout << "Timeout" << endl;
        this->SendDataToExistsConnection(socketIndex,loginTimeoutMsg);
        this->CutConnection(socketIndex);
        break;

    default :
        cout << "ID Check unknown result" << endl;
        break;

    }
}

IoTCloudService::PackageType IoTCloudService::checkPackageType(int socketIndex)
{
    PackageType type=PackageType_Unknown;
    json_t *root;
    //int pairIndex=this->connInfo[socketIndex].pairIndex;

    if(this->connInfo->at(socketIndex).pairIndex>=0)
    {
        type=PackageType_PairedConnectin;
    }
    else
    {
		//string receivedText(&this->connInfo[socketIndex].buffer.at(0));
        //if(this->jsonUtility.IsLeaglJsonFile(connInfo[socketIndex].tempBuffer))
		if (JsonUtility::IsLeaglJsonFile(&this->connInfo->at(socketIndex).buffer))
        {
            cout << "Received completed json file"<<endl;;
            //root= json_loads(this->connInfo[socketIndex].tempBuffer, 0, NULL);
			//string jsonTextData(&this->connInfo[socketIndex].buffer.at(0));
			//root = JsonUtility::LoadJsonData(jsonTextData);
			root = JsonUtility::LoadJsonData(&this->connInfo->at(socketIndex).buffer);
            string rootName= JsonUtility::GetFirstKeyName(root);
            if(rootName.compare("REGISTER")==0)
            {
                type=PackageType_REGISTER;
            }
            else
            {
                type=PackageType_Illegal;
            }
        }
        else
        {
            cout << "Received file(Not json file)" <<endl;;
            type=PackageType_Unknown;
        }
    }

    return type;
}

int IoTCloudService::findPairIndex(int connInfoIndex)
{
    int i=0,targetIndex=-1;
    ConnType type=this->connInfo->at(connInfoIndex).connType;
    ConnType targetType=(type==ConnType_Manager)?ConnType_EndNode:ConnType_Manager;
    string targetId=this->connInfo->at(connInfoIndex).userID;

    for(i=0;i<this->maxClient;i++)
    {
        if(this->connInfo->at(i).connType==targetType &&
           this->connInfo->at(i).userID.compare(targetId)==0)
        {

            cout << "Found pair index!" <<endl;
            targetIndex=i;
            break;
        }
    }

    return targetIndex;
}

void IoTCloudService::clearDuplicateId(string ID, ConnType type)
{
    int i=0;
    for(i=0;i<this->maxClient;i++)
    {
        if(this->connInfo->at(i).userID== ID&&
           this->connInfo->at(i).connType==type)
        {
            cout << "Found duplicate ID, clear it" <<endl;
            this->clearConnInfo(i);
        }
    }
}

//communication with php server
IoTCloudService::IdCheck IoTCloudService::curl_check_id_pw(string id, string pw)
{
	CURL *curl;
	CURLcode res;
	string request = string("searchID=") + id + string("&searchPass=") + pw ;


	IdCheck checkResult= IdCheck_FAILED;
	int findResult=-1;
	long readPageTimeOutSecond=2L;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl)
	{
	    //string targeUrl=this->webServerIp;
	    //cout << "Target URL:" << targeUrl << endl;

		curl_easy_setopt(curl, CURLOPT_URL, this->webServerIp.c_str());
		//curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.156.92/PairInterface/pairInterface.php");
		//curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1/PairInterface/pairInterface.php");
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->recvWebCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT , readPageTimeOutSecond);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "ID=DDR&PW=AAA");
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
        {
            //string temp=readBuffer.str();
            //cout << "Temp:" << temp << endl;
            findResult = readBuffer.str().find("pass");
			json_t*root = JsonUtility::LoadJsonData(readBuffer.str());
			string idResult = JsonUtility::GetValueInRootObject(root,"ID");
			string pwResult = JsonUtility::GetValueInRootObject(root, "Password");


            if (idResult=="pass" && pwResult=="pass")
            {
                checkResult = IdCheck_PASS;
            }
            else
            {
                checkResult = IdCheck_FAILED;
            }

        }
        else
        {
            checkResult=IdCheck_Timeout;
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }
        readBuffer.clear();
        readBuffer.str(std::string());
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
    //delete curl;

	return checkResult;
}

size_t IoTCloudService::recvWebCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    IoTCloudService *obj=static_cast<IoTCloudService*>(userp);
    //cout <<"Server port:" <<obj->serverPort <<endl;
    size_t realsize = size * nmemb;
    //obj->readBuffer.append(contents, realsize);
    for(int i=0;i<(int)realsize;i++)
    {
        obj->readBuffer<<contents[i];
    }
	//readBuffer.append(contents, realsize);
	return realsize;
}

//Event handler
//void IoTCloudService::Event_ReceivedData(int socketIndex, char* data, int dataLength)
void IoTCloudService::Event_ReceivedData(int socketIndex, std::vector<char> *buffer, int dataLength)
{

	cout << "Child class invoked" << endl;
	if ((int)this->connInfo->at(socketIndex).buffer.size() + dataLength > this->maxReceiveBuffer)
	{
		//this->clearConnInfo(this->connInfo[socketIndex]);
		//this->connInfo[socketIndex].receiveCount = 0;
		//std::fill_n(this->connInfo[socketIndex].tempBuffer, this->maxReceiveBuffer, '\0');
		this->connInfo->at(socketIndex).buffer.clear();
		cout << "buffer overflow, clear all" << endl;
	}
	else
	{
		//strncat(this->connInfo[socketIndex].tempBuffer, data, dataLength);

		this->connInfo->at(socketIndex).buffer.insert(
			this->connInfo->at(socketIndex).buffer.end(),
			buffer->begin(),
			buffer->begin()+ dataLength
		);

		//this->connInfo[socketIndex].receiveCount += dataLength;
		PackageType type = this->checkPackageType(socketIndex);

		if (type == PackageType_Unknown)// mabye the data just not complete, wait receive maxErrorTimes
		{
			cout << "Received data not a completed Json format" << endl;
			this->connInfo->at(socketIndex).errorCount++;

			if (this->connInfo->at(socketIndex).errorCount>this->maxErrorTimes)
			{
				cout << "cut the connection" << endl;
				//this->clearConnInfo(socketIndex); //cut connection will generate a connection denied event, clear there
				this->CutConnection(socketIndex);
			}
		}
		else
		{
			if (type == PackageType_PairedConnectin)
			{
				cout << "Forwarding package" << endl;
				int pairedIndex=this->findPairIndex(socketIndex);
                this->SendDataToExistsConnection(pairedIndex,&this->connInfo->at(socketIndex).buffer);
                this->connInfo->at(socketIndex).buffer.clear();

			}
			else if (type == PackageType_REGISTER)
			{
				cout << "Entering register process" << endl;
				this->registerProcess(socketIndex);

			}
			else if (type == PackageType_Illegal)
			{
				cout << "Received a illegal package, cut this connection" << endl;
				this->clearConnInfo(socketIndex);
				this->CutConnection(socketIndex);
			}
			//this->clearConnInfoOnlyBuffer(socketIndex);
			this->connInfo->at(socketIndex).buffer.clear();
		}
	}

	//cout << string(this->connInfo[socketIndex].tempBuffer)<<endl;
}

void IoTCloudService::Event_NewConnection(int socketIndex)
{
	this->clearConnInfo(socketIndex);
}

void IoTCloudService::Event_ConnectionDenied(int socketIndex)
{
	this->clearConnInfo(socketIndex);
}

/*
//Config file
void loadConfigFile()
{

	printf("Start load config file\n");
	TSFile fileHandle;
	const int bufferSize = 1000;
	char buffer[bufferSize];
	memset(buffer,'\0', bufferSize);


	File_open(&fileHandle,"C:\\Users\\loki.chuang\\Desktop\\MobileDesc.txt", O_RDONLY);
	File_read(&fileHandle, buffer, bufferSize);
	File_close(&fileHandle);

	printAllChar(buffer, bufferSize);

}
*/
