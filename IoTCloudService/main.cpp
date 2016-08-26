#include <iostream>
#include <stdlib.h>
using namespace std;


//#include "Utility/JsonUtility.h"
#include "IoTCloudService.h"

#include "Network/TcpClient.h"
//#include "Server/TcpServer.h"
//#include "Utility/IoTSocket.h"

#if defined(WIN32)
#pragma comment(lib,"curl/libcurl.lib")
#include "curl/curl.h"

#elif defined(__linux__)
#include <curl/curl.h>

#endif

#define MAXRECV 2000 //receive buffer max size
#define MAXCLIENTS 100 //max client size
#define SERVERPORT 6210 //Server portnumber

/*
int jsonTest()
{

	char *text = "{\"USER\":{\"ID\":\"DDR\",\"PW\":\"AAA\"}}";
	json_t *root = NULL;
	json_error_t error;
	int i = 0;
	//printf("%s",text);
	root = json_loads(text, 0, &error);


	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}

	if (json_is_object(root))
	{
		puts("root is a object");

		json_t *userData = json_object_get(root, "USER");

		json_t *jsonID = json_object_get(userData, "ID");
		const char *rootLevel = json_string_value(jsonID);
		printf("ID:%s\n", rootLevel);

		jsonID = json_object_get(userData, "PW");
		rootLevel = json_string_value(jsonID);
		printf("PW:%s\n", rootLevel);


		json_t *newPw = json_string("BBBB");
		if (json_object_set(userData, "PW", newPw))
		{
			puts("Json data update failed");
		}

		jsonID = json_object_get(userData, "PW");
		rootLevel = json_string_value(jsonID);
		printf("PW:%s", rootLevel);
	}
	else
	{
		puts("root is not a object");
	}
	return 0;

}
*/

std::stringstream readBuffer;
size_t recvWebCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	//IoTCloudService *obj = static_cast<IoTCloudService*>(userp);
	//cout <<"Server port:" <<obj->serverPort <<endl;
	size_t realsize = size * nmemb;
	//obj->readBuffer.append(contents, realsize);
	for (int i = 0; i<(int)realsize; i++)
	{
		readBuffer << contents[i];
	}
	//readBuffer.append(contents, realsize);
	return realsize;
}

void curlTest(string id,string pw)
{
	CURL *curl;
	CURLcode res;
	string request = string("searchID=") + id + string("&searchPass=") + pw;
	//string request = string("addID=") + id + string("&password=") + pw;
	//string request = string("ID=") + id + string("&PW=") + pw;

	int findResult = -1;
	long readPageTimeOutSecond = 2L;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl)
	{
		//string targeUrl=this->webServerIp;
		//cout << "Target URL:" << targeUrl << endl;

		//curl_easy_setopt(curl, CURLOPT_URL, this->webServerIp.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, "http://104.199.183.220/WeatherDataForGoogleCloudsPlatfrom/login.php");
		//curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1/PairInterface/pairInterface.php");
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recvWebCallback);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, readPageTimeOutSecond);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "ID=DDR&PW=AAA");
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			readBuffer << '\0';
			string temp=readBuffer.str();
			cout << "Response:" << temp << endl;
		}

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}



int main()
{
	/*
	string targetIp("192.168.156.199");
	TcpClient client(targetIp, 6210, MAXRECV);

	client.Connect();
	client.StartListen();
	client.SendData("This is from windows msg!");


	PAUSE;
	client.StopListen();
	client.Disconnect();
	PAUSE;
	*/

	//curlTest("DDR","AAAAA");
	//PAUSE;

	
	IoTCloudService service(SERVERPORT, MAXRECV, MAXCLIENTS);    
    NetworkError error= service.StartIoTService();    
    PAUSE;
    service.StopIoTService();
    



	return 0;
}
