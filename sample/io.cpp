/****************************************************************************
 *
 * eSpeed, Inc.
 * 
 *
 * This system and/or its use is protected under one or more of the following 
 * United States patents:  5,905,974; 6,560,580.  All rights reserved.  Other 
 * patents pending 
 * 
 ***************************************************************************/

#include <stdio.h>
#include <stdarg.h>

#if !defined(_WIN32)
#include <pthread.h>
#else
#include <windows.h>
#endif

#include "io.h"


static FILE *fd = 0;

#if !defined(_WIN32)

// Output window mutex lock
pthread_mutex_t mutex;

#else

HANDLE mutex;

#endif

void Output(const char *fmt, ...)
{
#if !defined(_WIN32)
	pthread_mutex_init(&mutex, NULL);
#else
	WaitForSingleObject(mutex, INFINITE);
#endif

	char buf[65536];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

    printf("%s\n", buf);

	if (fd)
		fprintf(fd, "%s\n", buf);

#if !defined(_WIN32)
	pthread_mutex_unlock(&mutex);
#else
	ReleaseMutex(mutex);
#endif

	return;
}

void SetLog(char *file)
{
	if (file)
	{
		fd = fopen(file, "w");
		if (fd)
			Output("Created log file \"%s\"", file);
		else
			Output("Failed to create log file \"%s\"", file);
	}
	return;
}

void Screen(const char *banner, int /* iUseCurses */)
{
#if !defined(_WIN32)
	pthread_mutex_init(&mutex, NULL);
#else
	mutex = CreateMutex(NULL, FALSE, "lock");
#endif
	Output(banner);
}

void CloseScreen()
{
}

void Input(char *string)
{
	char ch;
	int charcount = 0;
	*string = 1;

#if defined _WIN32
#pragma warning( disable : 4127 )
#endif
	while(1)
	{
		ch = (char)getchar();

		switch(ch)
		{
		case '\n':
			*string = 0;
			return;
		case '\b':
			if (charcount > 0)
			{
				string--;
				charcount--;
			}
			break;
		default:
			*string++ = ch;
			charcount++;
			break;
		}
	}
}

void Refresh()
{
}
