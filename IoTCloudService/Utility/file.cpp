#ifdef _WIN32
#include <io.h>
#else
// Must check this
//#include <sys/io.h>
#include <dirent.h>
#endif

#include "file.h"
#include <string.h>

//*---------------------------------------------------------------------
//Explanation: File
//*---------------------------------------------------------------------

int File_open(TSFile *f, char *name, unsigned int attrib)
{
#ifdef _WIN32
	return ((*f = _open(name, attrib)) != (-1));
#else
	return ((*f = open(name, attrib)) != (-1));
#endif
}

int File_create_open(TSFile *f, char *name, unsigned int  attrib)
{
#ifdef _WIN32
	return ((*f = _open(name, attrib | O_CREAT, _S_IWRITE | _S_IREAD)) != (-1));
#else
	return ((*f = open(name, attrib | O_CREAT, S_IWRITE | S_IREAD)) != (-1));
#endif
}

int File_write(TSFile *f, void *buffer, unsigned int  len)
{
#ifdef _WIN32
	return (_write(*f, buffer, len) == (unsigned int)len);
#else
	return (write(*f, buffer, len) == (unsigned int)len);
#endif
}

unsigned int  File_read(TSFile *f, void *buffer, unsigned int  len)
{
#ifdef _WIN32
	return (_read(*f, buffer, len));
#else
	return (read(*f, buffer, len));
#endif
}

int File_seek(TSFile *f, unsigned long pos, unsigned int  attrib)
{
#ifdef _WIN32
	return (_lseek(*f, pos, attrib) != (-1));
#else
	return (lseek(*f, pos, attrib) != (-1));
#endif
}

unsigned long File_size(TSFile *f)
{
#ifdef _WIN32
	return (_filelength(*f));
#else
	struct stat fs;
	fstat(*f, &fs);
	return (fs.st_size);
#endif
}

int File_close(TSFile *f)
{
#ifdef _WIN32
	return (_close(*f) != (-1));
#else
	return (close(*f) != (-1));
#endif
}

int File_status(char *filename, TSFileStat *filestat)
{
#ifdef _WIN32
	return (_stati64(filename, filestat) != (-1));
#else
	return (stat(filename, filestat) != (-1));
#endif
}

int File_find_first(TSFileFind *filefind, char *path, TSFileInfo *fileinfo)
{
#ifdef _WIN32
	int ret = 0;
	char *p = path + strlen(path);

	*p = '\\';
	*(p + 1) = '*';
	*(p + 2) = '\0';
	ret = (((*filefind) = _findfirst(path, fileinfo)) != (-1));
	*p = '\0';
	return ret;
#else
	strncpy(filefind->path, path, NAME_MAX);
	filefind->path[NAME_MAX] = '\0';
	if ((filefind->handle = opendir(path)) != NULL)
		return File_find_next(filefind, fileinfo);

	return 0;
#endif
}

int File_find_next(TSFileFind *filefind, TSFileInfo *fileinfo)
{
#ifdef _WIN32
	return (_findnext(*filefind, fileinfo) != (-1));
#else
	struct dirent *de;
	//****** Must be changed ******
	char z[NAME_MAX + 1];
	memset(z, '\0', sizeof(z));
	if ((de = readdir(filefind->handle)) != NULL)
	{
		struct stat fs;

		strcpy(fileinfo->name, de->d_name);
		strcpy(z, filefind->path);
		strncat(z, "/", NAME_MAX);
		strncat(z, fileinfo->name, NAME_MAX);
		z[NAME_MAX] = '\0';

		stat(z, &fs);

		if (fs.st_mode & S_IFDIR)
			fileinfo->attrib = A_SUBDIR;
		else
			fileinfo->attrib = 0;

		fileinfo->size = fs.st_size;
		fileinfo->time_write = fs.st_mtime;

		return 1;
	};

	return 0;
#endif
}

void File_find_close(TSFileFind *filefind)
{
#ifdef _WIN32
	_findclose(*filefind);
#else
	closedir(filefind->handle);
#endif
}


