#ifndef SystemUtility_H_INCLUDED
#define SystemUtility_H_INCLUDED

#include <iostream>
using namespace std;

#if defined(WIN32)
#include <windows.h>
#define PAUSE system("Pause");

#elif defined(__linux__) || defined(__FreeBSD__)
#include <unistd.h>
#include <sys/time.h>
#define PAUSE printf("Press Enter any key to continue..."); fgetc(stdin);

#endif

void ms_sleep(int ms);
unsigned long long get_millis();
void charcat(char *base_buf, char *target, int starIdx, int length);
void printAllChar(char *data, int length);

#endif
