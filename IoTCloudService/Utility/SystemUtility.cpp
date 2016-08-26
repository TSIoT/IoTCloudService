#include "SystemUtility.h"


void ms_sleep(int ms)
{
#if defined(WIN32)
	Sleep(ms);
#elif defined(__linux__) || defined(__FreeBSD__)
	usleep(ms * 1000);
#endif
}

unsigned long long get_millis()
{
	unsigned long long millisecondsSinceEpoch = 0;
#if defined(__linux__) || defined(__FreeBSD__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	millisecondsSinceEpoch =
		(unsigned long long)(tv.tv_sec) * 1000 +
		(unsigned long long)(tv.tv_usec) / 1000;
	return millisecondsSinceEpoch;
#elif defined(WIN32)
	millisecondsSinceEpoch = GetTickCount();
#endif
	return millisecondsSinceEpoch;
}

void charcat(char *base_buf, char *target, int starIdx, int length)
{
	int i = 0, idx = starIdx;

	for (i = 0; i < length; i++)
	{
		base_buf[idx] = target[i];
		idx++;
	}
}

void printAllChar(char *data, int length)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if (data[i] == '\0')
			cout << ',';			
		else
		{
			cout << data[i];			
		}

	}
	cout << endl;
}

