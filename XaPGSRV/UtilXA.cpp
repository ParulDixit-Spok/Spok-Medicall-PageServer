
// UtilXA.cpp

// disable deprecation
#pragma warning(disable:4996)

// for CString class etc.  this needs to be before windows.h
//#include <afx.h>
#include <atlstr.h>

#include "common.h"
#include "Registry43.h"


	unsigned __stdcall
ExperimentThread(void *)
	{
	int iCount = 0;

	// delay on startup X seconds;
	Sleep(5 * 1000);

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"ExperimentThread(): [%s]", 
					"Start");

	while(!bKillThreads)
		{
		iCount++;

		// check every X seconds.
		Sleep(1 * 1000);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ExperimentThread(): [%s]", 
						"Run");

		// format with leading space for status line display.
		CString csTemp;

		csTemp.Format("  %5.5d [%s]", iCount, "small data");

		SetStatusText(STATUS_LINE_EXPERIMENT, (LPTSTR) (LPCTSTR) csTemp);
		}

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"ExperimentThread(): [%s]", 
					"Stop");

	return 0;
	}


