#ifndef JSONUTILITY_H
#define JSONUTILITY_H


#include <iostream>
#include <vector>

using namespace std;

#if defined(WIN32)
//windows version need add the directory in Property > Linker > Additional library Directories
//and Property > Linker > Input > Additional Dependencies with jansson_d.lib
#include "../jansson/jansson.h"

#elif defined(__linux__)
//linux version need install jansson first, then add -ljansson in compile command
#include <jansson.h>

#endif


class JsonUtility
{
public:
	static int jsonTest();

	//void LoadJsonData(json_t *root, string *text);
	static json_t* LoadJsonData(string text);
	static json_t* LoadJsonData(std::vector<char> *text);
	static string ExportJsonContent(json_t *root);
	static void PrintJasonData(char *text);
	static bool IsLeaglJsonFile(char* text);
	static bool IsLeaglJsonFile(std::vector<char> *text);
	static string GetFirstKeyName(json_t *root);
	static string GetValueInFirstObject(json_t *root,string keyName);
	static void SetValueInFirstObject(json_t *root,string keyName, string value);
	static string GetValueInRootObject(json_t *root, string keyName);

	/*
	void DumpTest()
	{
		char *deviceDesc = "{\"IOTDEV\":{\"DeviceName\":\"Moisture sensor\",\"FunctionGroup\":\"Sensor\",\"DeviceID\":\"arduino.grove.mois01\",\"Component\":[{\"ID\":\"mois\",\"Group\":\"1\",\"Name\":\"Moisture Value\",\"Type\":\"Content\",\"ReadFrequency\":\"2000\"}]}}";
		int len = strlen(deviceDesc);
		char bigMemory[500];
		memset(bigMemory,0, 500);
		memcpy(bigMemory, deviceDesc, len);


		//char *cmd = "{\"IOTCMD\":{\"DeviceName\":\"MO&LU\"}}";

		json_error_t error;
		json_t *root;

		//root = json_loads(deviceDesc, 0, &error);
		root = json_loads(bigMemory, 0, &error);

		if (!root)
		{
			fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		}

		char *temp = json_dumps(root,0);


		int a = 10;
	}
	*/
	/*
	void CommandTest()
	{
		char *cmd = "{\"IOTCMD\":{\"Type\":\"Man\",\"ID\":\"Dis_NPx\",\"Value\":0}}";
		//char *cmd = "{\"IOTCMD\":{\"DeviceName\":\"MO&LU\"}}";

		json_error_t error;
		json_t *root;

		root = json_loads(cmd, 0, &error);

		if (!root)
		{
			fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		}
	}
	*/
	/*
	void ArrayTest()
	{
		char *deviceDesc = "{\"IOTDEV\":{\"DeviceName\":\"MO&LU\",\"FunctionGroup\":\"Sensor\",\"DeviceID\":\"arduino.grove.pro.molu01\",\"Component\":[{\"ID\":\"lux\",\"Group\":1,\"Name\":\"Illuminace\",\"Type\":\"Content\",\"ReadFrequency\":10000},{\"ID\":\"mois\",\"Group\":2,\"Name\":\"Moisture\",\"Type\":\"Content\",\"ReadFrequency\":10000}]}}";

		json_t *root = this->LoadJsonData(deviceDesc);

		string rootName = this->GetFirstKeyName(root);
		json_t *firstObj = json_object_get(root, rootName.c_str());
		json_t *arr = json_object_get(firstObj, "Component");
		int arrSize = json_array_size(arr);

		for (int i = 0; i < arrSize; i++)
		{
			json_t *com = json_array_get(arr, i);
			json_t *value = json_object_get(com, "ID");
			const char *rootLevel = json_string_value(value);
			cout << "ComID:" << rootLevel << endl;
		}
	}
	*/

private:

};
#endif // JSONUTILITY_H



