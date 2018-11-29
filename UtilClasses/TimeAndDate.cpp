
// TimeAndDate.cpp


#include <windows.h>
#include <stdio.h>

#include "c:\project\pagewin5\utilclasses\timeanddate.h"


	void
GetTimeAndDateString(char * TimeAndDate)
	{
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);	

	sprintf(TimeAndDate, 
			"%04d-%02d-%02d %02d:%02d:%02d",
				stNow.wYear,
				stNow.wMonth,
				stNow.wDay,
				stNow.wHour,
				stNow.wMinute,
				stNow.wSecond);
	}
