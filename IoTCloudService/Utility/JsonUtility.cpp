#include "JsonUtility.h"
//#include <string>
#include <sstream>

int JsonUtility::jsonTest()
{
	string text("{\"USER\":{\"ID\":\"DDR\",\"PW\":\"AAA\"},\"USER2\":{\"ID\":\"DDR\",\"PW\":\"AAA\"}}");
    json_t *root;
    json_error_t error;
    //printf("%s",text);
    root= json_loads(text.c_str(), 0, &error);


    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }

    if(json_is_object(root))
    {
        puts("root is a object");
        void *iter = json_object_iter(root);
        const char *key;

        while(iter)
        {
            key = json_object_iter_key(iter);
            //value = json_object_iter_value(iter);
            /* use key and value ... */
            if(string(key).compare("USER2")==0)
            {
                //printf("Key:%s\n",key);
                cout << "Key:" << key <<endl;
            }


            iter = json_object_iter_next(root, iter);
        }

        json_t *userData=json_object_get( root, "USER" );
        json_t *jsonID = json_object_get( userData, "ID" );
        const char *rootLevel=json_string_value(jsonID);
        printf("ID:%s\n",rootLevel);

        jsonID = json_object_get( userData, "PW" );
        rootLevel=json_string_value(jsonID);
        printf("PW:%s\n",rootLevel);


        json_t *newPw=json_string("BBBB");
        if(json_object_set(userData,"PW",newPw))
        {
            puts("Json data update failed");
        }

        jsonID = json_object_get( userData, "PW" );
        rootLevel=json_string_value(jsonID);
        printf("PW:%s",rootLevel);

		//char *data=json_dumps(root, JSON_ENSURE_ASCII| JSON_COMPACT);

    }
    else
    {
        puts("root is not a object");
    }


	return 0;

}

json_t* JsonUtility::LoadJsonData(string text)
{
	json_error_t error;
	json_t *root;

	const char *c_str = text.c_str();
	root = json_loads(c_str, 0, &error);

	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	}

	return root;
}

json_t* JsonUtility::LoadJsonData(std::vector<char> *text)
{
	json_error_t error;
	json_t *root;
	stringstream tempStr;

	for (int i = 0; i < (int)text->size(); i++)
	{
		tempStr << text->at(i);
	}
	tempStr << '\0';

	root = json_loads(tempStr.str().c_str(), 0, &error);

	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	}

	return root;
}


string JsonUtility::ExportJsonContent(json_t *root)
{
	char *temp = json_dumps(root, JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER);
	string returenResult(temp);
	free(temp);
	return returenResult;
	//return string(json_dumps(root, JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
}

void JsonUtility::PrintJasonData(char *text)
{
    json_t *root;
    json_error_t error;

    root= json_loads(text, 0, &error);
    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return ;
    }

    if(json_is_object(root))
    {
        string rootKey=GetFirstKeyName(root);
        cout << "Root Key:" << rootKey <<endl;

        json_t *firstObj=json_object_get( root, rootKey.c_str());

        void *iter = json_object_iter(firstObj);
        const char *key;
        json_t *value;

        while(iter)
        {
            key = json_object_iter_key(iter);
            value = json_object_iter_value(iter);
            cout << "\t" << key << ":";
            if(json_is_string(value))
            {
                const char *tempName=json_string_value(value);
                cout << tempName << endl;
            }

            iter = json_object_iter_next(firstObj, iter);
        }
    }
}

bool JsonUtility::IsLeaglJsonFile(char* text)
{
    json_t *root;
    json_error_t error;
    root= json_loads(text, 0, &error);
	bool result=false;

    if(root)//buffer is a legal json data
    {
        result=true;
    }

    return result;
}

bool JsonUtility::IsLeaglJsonFile(std::vector<char> *text)
{
	json_t *root;
	json_error_t error;
	stringstream tempStr;

	for (int i = 0; i < (int)text->size(); i++)
	{
		tempStr << text->at(i);
	}
	tempStr << '\0';
	//cout << endl;

	//cout << "IsLeaglJsonFile:" << string(tempText) << endl;

	//root = json_loads(tempText, 0, &error);
	root = json_loads(tempStr.str().c_str(), 0, &error);
	bool result = false;

	if (root)//buffer is a legal json data
	{
		result = true;
	}


	return result;
}


string JsonUtility::GetFirstKeyName(json_t *root)
{
    void *iter = json_object_iter(root);
    const char *key;

    key = json_object_iter_key(iter);
    return string(key);
}

string JsonUtility::GetValueInFirstObject(json_t *root,string keyName)
{
    //string resultValue="";
    string rootName=GetFirstKeyName(root);
    json_t *firstObj=json_object_get( root, rootName.c_str() );
    json_t *value = json_object_get( firstObj, keyName.c_str());
    const char *rootLevel=json_string_value(value);

    return string(rootLevel);
}

void JsonUtility::SetValueInFirstObject(json_t *root,string keyName, string value)
{
    if(!root)
    {
        cout << "Json file is not legal" <<endl;
        return ;
    }

    if(json_is_object(root))
    {
        string rootKey=GetFirstKeyName(root);
        //cout << "Root Key:" << rootKey <<endl;

        json_t *firstObj=json_object_get( root, rootKey.c_str());

        void *iter = json_object_iter(firstObj);
        const char *key;
        //json_t *value;

        while(iter)
        {
            key = json_object_iter_key(iter);
            if(keyName==string(key))
            {
                //cout << "Found the key:" <<endl;
                json_t *newStr=json_string(value.c_str());
                json_object_set(firstObj,key,newStr);
                break;
            }
            iter = json_object_iter_next(firstObj, iter);
        }
    }


}


string JsonUtility::GetValueInRootObject(json_t *root, string keyName)
{
	json_t *value = json_object_get(root, keyName.c_str());
	const char *rootLevel = json_string_value(value);

	return string(rootLevel);
}
