#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef NAME_MAX
#define NAME_MAX	1024
#endif

#ifdef _WIN32
#define O_APPEND	_O_APPEND
#define O_CREAT 	_O_CREAT 
#define O_EXCL		_O_EXCL
#define O_RDONLY	_O_RDONLY
#define O_RDWR		_O_RDWR 
#define O_TRUNC		_O_TRUNC
#define O_WRONLY	_O_WRONLY
#define O_TEXT		_O_TEXT
#define O_BINARY	_O_BINARY

#define A_HIDDEN	_A_HIDDEN
#define A_NORMAL	_A_NORMAL
#define A_RDONLY	_A_RDONLY
#define	A_SUBDIR	_A_SUBDIR
#else
#define	A_SUBDIR	1
#define O_BINARY	0
#define O_TEXT		0
#endif	// _WIN32


#ifdef _WIN32
typedef struct _stati64 TSFileStat;
typedef struct _finddata_t TSFileInfo;
typedef long TSFileFind;
#else
#include <unistd.h>
#include <dirent.h>

typedef struct stat TSFileStat;

typedef struct finddata_PW
{
	char name[NAME_MAX + 1];
	int attrib;
	unsigned int size;
	time_t time_write;
} TSFileInfo;

typedef struct
{
	char path[NAME_MAX + 1];
	DIR *handle;
} TSFileFind;
#endif

typedef int TSFile;

int File_open(TSFile *f, char *name, unsigned int attrib);
int File_create_open(TSFile *f, char *name, unsigned int attrib);
int File_close(TSFile *f);

int File_write(TSFile *f, void *buffer, unsigned int len);
unsigned int File_read(TSFile *f, void *buffer, unsigned int len);

int File_seek(TSFile *f, unsigned long pos, unsigned int attrib);
unsigned long File_size(TSFile *f);

int File_status(char *filename, TSFileStat *filestat);

int File_find_first(TSFileFind *filefind, char *path, TSFileInfo *fileinfo);
int File_find_next(TSFileFind *filefind, TSFileInfo *fileinfo);
void File_find_close(TSFileFind *filefind);

