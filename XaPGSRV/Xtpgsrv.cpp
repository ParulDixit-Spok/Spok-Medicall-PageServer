#include "common.h"

#include <io.h>

// xtpgsrv.cpp - PageWin4
//#include <afx.h>
#include <atlstr.h>
//#include <crtdbg.h>
//#include <stdlib.h>

//#include <cstringt.h>

#include <string>

#include <commctrl.h>
#include <sys\locking.h>

#include "registry43.h"
#include <windows.h>
#include <process.h>

#include "Transaction.h"
#include "IniFilesReadWrite.h"
#include "Append2File.h"

#include "VersionXtend.h"
#include "Impairment.h"

#include <atlbase.h>
#include <atlconv.h>
#include <winbase.h>

// disable deprecation
#pragma warning(disable:4996)

extern void XnUpdateProfile(int, char *);
extern void XnInsertProfile(int, char *);

extern void TestSendListPage1(void);
extern void TestSendListPage2(void);
extern void TestSendListPage3(void);
extern void TestSendListPage4(void);



extern BOOL InitializeApplication(void);
extern BOOL InitializeInstance(INT);
extern VOID CommandHandler(HWND, WPARAM, LPARAM);


// a mutex to allow only a single instance of PageServer4 to run.
//
HANDLE g_hmutexPageServerAppRun = NULL;

HWND hwndMDIClient      = NULL;		// Handle to MDI client
HWND hwndActive         = NULL;		// Handle to currently activated child
HWND hwndActiveEdit     = NULL;		// Handle to edit control
HWND hwndControl		= NULL;		// Handle to list control

HANDLE hMainPageThread;
HANDLE hNewExeChecker;
HANDLE hExperimentThread;
HANDLE hAppRestartChecker;

int	iSelectedListboxItem;			// Indicates which listbox line item was double clicked.
int	iSelectedItem;					// iSelectedListboxItem adjusted 
									//  to account for the 3 title lines.



	LONG
	WINAPI
MyUnhandledExceptionFilter(EXCEPTION_POINTERS * pExPtrs)
	{
	CString csTemp1, csTemp2;


	//pExPtrs->ExceptionRecord->


	csTemp1.Format("MyUnhandledExceptionFilter(): ExceptionCode:[%08X]",
						pExPtrs->ExceptionRecord->ExceptionCode);

	csTemp2.Format("MyUnhandledExceptionFilter(): ExceptionAddress:[%p]",
						pExPtrs->ExceptionRecord->ExceptionAddress);

	// log the information, and exit,
	//  batch file may restart the application
	//  if it recognizes the exit code.

	// search for the following may be useful.
	//  STATUS_ACCESS_VIOLATION
	//  STATUS_PRIVILEGED_INSTRUCTION
	//  EXCEPTION_ACCESS_VIOLATION

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"MyUnhandledExceptionFilter(): - Application Exit Code: [%d] [%s]",
					iAppExitReason,
					szAppExitReason[iAppExitReason]);

	// write to the START_ log file.
	g_LogStart.LogMessage(csTemp1, LOG_OPTION_DATE);
	g_LogStart.LogMessage(csTemp2, LOG_OPTION_DATE);
	g_LogStart.FlushMessagesToDisk(Append2File::FINALIZE_CURRENT_FILENAME);

	// write to the ALL_ log file.
	g_LogAll.LogMessage(csTemp1, LOG_OPTION_DATE);
	g_LogAll.LogMessage(csTemp2, LOG_OPTION_DATE);
	g_LogAll.FlushMessagesToDisk(Append2File::FINALIZE_CURRENT_FILENAME);

	return EXCEPTION_CONTINUE_SEARCH; // EXCEPTION_EXECUTE_HANDLER;
	}

	void
OpenLogFiles(void)
	{
  
	// setup log files.
	// varying file capacities, but all are a group of 10 files.
	//
	g_LogAll.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "All_");
	g_LogAll.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
	g_LogAll.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
	g_LogAll.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
	g_LogAll.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 10 * 1000000);

	// an option to disable "standard" file logging.
	if(bCmdLineEnableStandardLogging)
		{
		g_LogAll.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "TRUE");
		}
	else
		{
		g_LogAll.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "FALSE");
		}

	g_LogAll.CreateSubdirectory();

	g_LogSys.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "Sys_");
	g_LogSys.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
	g_LogSys.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
	g_LogSys.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
	g_LogSys.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 1 * 1000000);

	g_LogCom.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "Com_");
	g_LogCom.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
	g_LogCom.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
	g_LogCom.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
	g_LogCom.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 1 * 1000000);

	g_LogIvr.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "Ivr_");
	g_LogIvr.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
	g_LogIvr.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
	g_LogIvr.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
	g_LogIvr.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 1 * 1000000);

	g_LogDbs.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "Dbs_");
	g_LogDbs.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
	g_LogDbs.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
	g_LogDbs.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
	g_LogDbs.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 10 * 1000000);

	g_LogDbg.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "Dbg_");
	g_LogDbg.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
	g_LogDbg.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
	g_LogDbg.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
	g_LogDbg.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 1 * 1000000);
	}


	void
ReadIniParameters(void)
	{
	// get .ini parameter info for SMS PageTypes.

	// first pickup some ini parameters.
	CString csPathAndFilename;
	csPathAndFilename.Format("%sXaPgSrv.ini", szApplicationPathName);


	// log information as read from .ini file.
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Name and location of PageServer .INI file: [%s]", 
					csPathAndFilename);

	// SOguz - 6/3/2016 - DE20747
	// constains the YNFLAG Field Name so we can use it to check if page all devices turned ON
	g_csYNFlagNameToPageAllDevices = GetIniString("GENERAL", "YNFLAG_NAME_TO_PAGE_ALL_DEVICES", "", (LPCTSTR) csPathAndFilename);
	// SOguz - 5/4/2017 - DE24153
	// TRUE or FALSE flag to decide whether to follow coverage when paging all devices
	//FollowPageAllDevicesCoverage = GetIniBool("GENERAL", "FOLLOW_PAGE_ALL_DEVICES_COVERAGE", "TRUE", (LPCTSTR) csPathAndFilename);
	// Defaulting FollowPageAllDevicesCoverage to be always False as Coverage and other profile statuses are following IGNORE_STATUS_WHEN_PAGE_ALL_DEVICES general setting.
	FollowPageAllDevicesCoverage = false;

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "YNFlag Name Field Name used for Page All Devices: [%s]", 
					g_csYNFlagNameToPageAllDevices);

	ignoreCoverageWhenPageAllDevices = GetIniBool("GENERAL", "IGNORE_COVERAGE_WHEN_PAGE_ALL_DEVICES", "FALSE",  (LPCTSTR) csPathAndFilename);
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Ignore Coverage when Page All Devices: [%s]", 
					ignoreCoverageWhenPageAllDevices ? "True" : "False");

	ignoreStatusWhenPageAllDevices= GetIniBool("GENERAL", "IGNORE_STATUS_WHEN_PAGE_ALL_DEVICES", "FALSE",  (LPCTSTR) csPathAndFilename);
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Ignore Status when Page All Devices: [%s]", 
					ignoreStatusWhenPageAllDevices ? "True" : "False");

	// this one is a list of page types containing nopaging items that have the SmsFlag set. This is the "%r4" option.
	//  e.g. "%57,%56,%52". PageCfg.exe has scanned NoPaging to prepare this list, and has written it into the .ini file.
	//
	g_csPageTypesWithSmsFlag = GetIniString("SMS_OPTIONS", "PAGE_TYPES_WITH_SMS_FLAG", "", (LPCTSTR) csPathAndFilename);

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "SMS Page Types: [%s]", 
					g_csPageTypesWithSmsFlag);

	// impaired paging needs a format type.
	//
	int i = atoi(GetIniString("IMPAIRMENT_OPTIONS", "FORMAT_NUMBER", "75", (LPCTSTR) csPathAndFilename));
	// 2012-02-22 LD added this parameter for TAP In protocol
	// to take care of page status for Masco

	Respect_Paging_Status= GetIniBool("RESPECT_PAGING_STATUS_OPTIONS", "RESPECT_PAGING_STATUS", "FALSE",  (LPCTSTR) csPathAndFilename);
	if(i == 0)
		{
		// anything that is not convertible to integer by atoi() will yield a 0 value.
		//  we use 75 as the default value.
		g_iImpairmentFormatNumber = 75;
		}
	else
		{
		g_iImpairmentFormatNumber = i;
		}

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Format Number for use with Impairment feature: [%d]", 
					g_iImpairmentFormatNumber);
	}







void my_terminate_handler();
void ( *old_terminate )( ) = set_terminate(my_terminate_handler); 






void my_terminate_handler()
{
	old_terminate();		
	ReleaseMutex(g_hmutexPageServerAppRun);
	exit(EXIT_FAILURE);
}



class SmartFile
{
private:
	HANDLE filePtr;

public:
	SmartFile(HANDLE fp)
	{
		filePtr = fp;
	}

	~SmartFile()
	{
		CloseHandle(filePtr);
	}

	operator::HANDLE()
	{
		return filePtr;
	}

};





//int ExitMEmoryCheck()
//{
//   _CrtDumpMemoryLeaks();
//   delete logFile;
//   return 0;
//}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	CString csTemp;
	//OSVERSIONINFOEX OsVerInfoEx;
	MSG msg;
	char LogPathName[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];


auto_ptr<SmartFile> logFile(new SmartFile(CreateFile(".\\logError.txt", GENERIC_WRITE, 
			FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, NULL)));


	// the application START_  file is NOT optional.
	//  no command line parameter to control it,
	//  it may be logging an exception.
	//
	{

		/*
		* Set the debug-heap flag to keep freed blocks in the
		* heap's linked list - This will allow us to catch any
		* inadvertent use of freed memory
		*/
		int tmpDbgFlag;

		tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag(tmpDbgFlag);
				  
		//_onexit( ExitMEmoryCheck );

		//setup memory leak debugger 

		// Send all reports to STDOUT 
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE ); 
		_CrtSetReportFile( _CRT_WARN, (HANDLE)*logFile); 
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE ); 
		_CrtSetReportFile( _CRT_ERROR, (HANDLE)*logFile); 
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE ); 
		_CrtSetReportFile( _CRT_ASSERT, (HANDLE)*logFile); 


		

		//_RPT0(_CRT_WARN,"file message\n");


		//char msgTest[] = {"Hello world\n"};
		//DWORD bWrite;

		//WriteFile((HANDLE)*logFile, msgTest, sizeof(msgTest), &bWrite, NULL);


		g_LogStart.SetProperty(Append2File::LOG_SET_BASE_FILENAME, (CString) "Start_");
		g_LogStart.SetProperty(Append2File::LOG_SET_DIRECTORY, (CString) "Log");
		g_LogStart.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "Txt");
		g_LogStart.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
		g_LogStart.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 1 * 1000000);
		g_LogStart.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "TRUE");

		CString csMessage = "PageServer4 - Application Started";
		g_LogStart.LogMessage(csMessage, LOG_OPTION_DATE);

		csMessage.Format("  CommandLine: [%s]",  GetCommandLine());
		g_LogStart.LogMessage(csMessage, LOG_OPTION_DATE);

		// command line options have not been processed yet, 
		//  the version cannot be shown yet, sorry.

		// that's all for the "start" file.
		g_LogStart.FlushMessagesToDisk(Append2File::FINALIZE_CURRENT_FILENAME);
	

	// give them a chance to hold down the <ctrl> <alt>, and <shift> keys,
	//  at startup to run the PSDiag.exe application.
	//  the Cue is the beep at startup, 
	//  they have 2 seconds to press and hold.
	//
	Beep(1000, 50);
	Sleep(2000);

	if(GetAsyncKeyState(VK_SHIFT) & 0x8000 &&
		GetAsyncKeyState(VK_MENU) & 0x8000 &&
		GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		// ms bit is set, the key is down.
		_spawnl(_P_WAIT, "PSDiag.exe", GetCommandLine(), NULL);

		exit(-100);
	}

	// first things first, capture the application
	//  instance handle to a global variable.
	hInst = hInstance;

	// Get the application name string, we need this information
	//  early on, so that if the application is already running,
	//  we can find its window, and activate it.
	LoadString(hInst, IDS_APP_NAME, szAppName, sizeof(szAppName));





	// to protect against an instance of terminal services running
	//  an additional instance of this application...
	//
	//  create a "global" named mutex.
	//  attempt to wait on it
	//  if successful wait, then you are allowed to run,
	//  otherwise, someone else has beaten you to it.
	//
	// we don't care if it was created or not at this point,
	//  it may exist already, or not.  The decision point
	//  is when we attempt to gain ownership.
	//
	g_hmutexPageServerAppRun = CreateMutex(NULL, FALSE, "Global\\SpokPageServer4");

	if(NULL == g_hmutexPageServerAppRun)
	{
		// NULL means, that we were unable to create, or open
		//  the mutex, very serious, but nothing we can do, except
		//  refuse to run.
		//
		// terminate THIS instance.
		return FALSE;
	}

	// now that we have a handle to the mutex, attempt to
	//  take ownership of it, timeout unsuccessfully,
	//  after a short interval. 1000 = 1 sec.
	//
	switch(WaitForSingleObject(g_hmutexPageServerAppRun, 1000))
	{
	case WAIT_OBJECT_0:
		// successful wait.
		//  the application will be allowed to run.
		//
		break;

	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
	default:
		// unsuccessful wait, someone else owns the mutex.
		::MessageBox(NULL,
			"Cannot continue! \r"
			"\r"
			"An instance of PageServer is already running. \r"
			"\r"
			"The instance could be running in another \r"
			"\"Windows Terminal Services\" session, \r"
			"or just minimized to the Windows taskbar. \r",
			"SPOK PAGESERVER ENGINE",
			MB_ICONSTOP | MB_OK);

		// attempt to activate the window of the instance in THIS session.
		//  this will only work if the window name can be matched.

		// when a window with the window classname that we are about
		//  to use is found, activate that window, restore
		//  it if necessary and terminate this instance
		//  of the application.
		HWND hwnd;
		HWND hwndPopup;

		// attempt to find a window with the specified ClassName.
		hwnd = FindWindow(szAppName, NULL);

		if(IsWindow(hwnd))
		{
			// it is a valid window handle, this app is already running.

			// find the most recently active window of the application.
			hwndPopup = GetLastActivePopup(hwnd);

			if(IsWindow(hwndPopup))
			{
				hwnd = hwndPopup;
			}

			// set this one as the current active window.
			SetForegroundWindow(hwnd);

			// check if the window is minimized
			if(IsIconic(hwnd))
			{
				// existing instance is minimized, restore it.
				ShowWindow(hwnd, SW_RESTORE);
			}

			// terminate THIS instance, another instance has been found
			//  and activated.
		}

		// terminate THIS instance.
		return FALSE;
		break;
	}

	// attempt to find or create a subdirectory
	//  to write log information to.

	// take the C RunTime Library path, split it apart,
	_splitpath(_pgmptr, drive, dir, fname, ext);

	// the name of the "Log" subdirectory can most easily be built using sprintf().
	//  note: if we use _makepath() we will have an undesired trailing '\'
	//  with a trailing '\' the directory name will not be found.
	//
	sprintf(LogPathName, "%s%sLog", drive, dir);

	// attempt to find the "Log" subdirectory.
	WIN32_FIND_DATA w32fd;
	HANDLE hFindFile = FindFirstFile(LogPathName, &w32fd);

	// we are only interested in one subdirectory,
	//  so it is ok to close the "FindFile" file handle right away.
	FindClose(hFindFile);

	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		// a directory or file by that name exists.

		if(w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// directory attribute found,
			//  the directory exists, doesn't need to be created.
			//
			// no action required.
		}
		else
		{
			// no directory attribute found,
			//  there probably exists a file by the name of "Log"
			::MessageBox(NULL,
				"A file with the name 'Log' was found!, \r"
				"PageServer needs to create a subdirectory by this name, \r",
				"SPOK PAGESERVER ENGINE",
				MB_ICONSTOP | MB_OK);

			// post reason for application exit
			SetExitCode(APP_EXIT_CANT_CREATE_LOG_SUBDIRECTORY);
			return iAppExitReason;
		}
	}
	else
	{
		// named directory was not found,
		//  now, attempt to create the named directory.

		if(CreateDirectory(LogPathName, NULL) == 0)
		{
			// unable to create directory,
			//  display a dialog box, exit the application.
			::MessageBox(NULL,
				"log directory could not be created \r",
				"SPOK PAGESERVER ENGINE",
				MB_ICONSTOP | MB_OK);

			// post reason for application exit
			SetExitCode(APP_EXIT_CANT_CREATE_LOG_SUBDIRECTORY);
			return iAppExitReason;
		}
	}

	// the following protects all threads from unhandled
	//  exceptions. if one happens, the filter function
	//  referenced below will get control.
	//  the exit code returned from the process will
	//  be the un-translated exception code.
	//
	//  in a batch file the following may be useful.
	//  C0000005 == -1073741819  == access exception
	//  C0000094 == -1073741676  == divide by zero exception
	//
	// this is placed in sequence after log files are open,
	// and before any threads are started.


	// JPH ____
#if 1
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
#endif
	// get command line options into global variables.
	if(!ProcessCommandLineOptions())
	{
		// post reason for application exit
		SetExitCode(APP_EXIT_BAD_COMMAND_LINE_OPTION);

		// write a shutdown message.

		return iAppExitReason;
	}
	else
	{
		// command line options read ok.

		// determine which log-files to allow writing to, in debug mode.
		if(bCmdLineEnableDebugLogging)
		{
			// "debug logging" is enabled.
			g_LogSys.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "FALSE");
			g_LogCom.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "FALSE");
			g_LogIvr.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "FALSE");
			g_LogDbs.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "FALSE");
			g_LogDbg.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "TRUE");
		}
	}

	OpenLogFiles();

	// once the logging object is defined, PrintMessage() can be used,
	//  messages will go to the appropriate logfile.
	//  if a window is not open yet, the message will be ignored.
	PrintMessage(MSGTYPE_SYSTEM,
		NULL,
		"WinMain(): Application Start.");

	PrintMessage(MSGTYPE_SYSTEM,			NULL, "WinMain(): TEST MESSAGE1 - MSGTYPE_SYSTEM.");
	PrintMessage(MSGTYPE_ERROR,				NULL, "WinMain(): TEST MESSAGE1 - MSGTYPE_ERROR.");
	PrintMessage(MSGTYPE_IVR,				NULL, "WinMain(): TEST MESSAGE1 - MSGTYPE_IVR.");
	PrintMessage(MSGTYPE_DEBUG,				NULL, "WinMain(): TEST MESSAGE1 - MSGTYPE_DEBUG.");
	PrintMessage(MSGTYPE_COMM,				NULL, "WinMain(): TEST MESSAGE1 - MSGTYPE_COMM.");
	PrintMessage(MSGTYPE_DATABASE_COMMAND,	NULL, "WinMain(): TEST MESSAGE1 - MSGTYPE_DATABASE_COMMAND.");

	// once the 'C' run-time tells you that it's version 5 or better,
	//  then the GetVersionEx() function can be called to determine
	//  more precise version info, specifically the service pack version.


	g_bOkToRunApplication = TRUE;

	OSVERSIONINFOEX osviEx;

	ZeroMemory(&osviEx, sizeof(OSVERSIONINFOEX));
	osviEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&osviEx)))
	{

		// initialize the structure size member
		//  with the size of the structure.

		// decide whether to run on that version, and service pack.
		g_bOkToRunApplication = FALSE;

		if(osviEx.dwMajorVersion == 5)
		{
			switch(osviEx.dwMinorVersion)
			{

				//if windows 2000 and service pack 3 or better
			case 0:
				if(osviEx.wServicePackMajor >= 3)
					g_bOkToRunApplication = TRUE;
				break;

				// if Windows XP and service pack 1 or better.
			case 1:
				if(osviEx.wServicePackMajor >= 1)
					g_bOkToRunApplication = TRUE;
				break;

				// if Windows 2003 server.
			case 2:
				if(osviEx.wServicePackMajor >= 0)
					g_bOkToRunApplication = TRUE;
				break;

			}
		}
		else if(osviEx.dwMajorVersion == 6)
		{
			switch(osviEx.dwMinorVersion)
			{
			case 0:
				if(osviEx.wServicePackMajor >= 0)
					g_bOkToRunApplication = TRUE;
				break;

				// windows 7 reports version 6.1
			case 1:
				if(osviEx.wServicePackMajor >= 0)
					g_bOkToRunApplication = TRUE;
				break;
				// DE17682 - SO - 8/12/2015 - Windows 8 and Windows 2012 Server support
			case 2:
				if(osviEx.wServicePackMajor >= 0)
					g_bOkToRunApplication = TRUE;
				break;
			// DE17682 - SO - 8/12/2015 - Windows 8.1 and Windows 2012 R2 Server support
			case 3:
				if(osviEx.wServicePackMajor >= 0)
					g_bOkToRunApplication = TRUE;
				break;
			}
		}
	}

	// Get the application version string,
	//  from the standard .EXE file version info block.

	GetFileVersionString(szApplicationFileName, szAppVersion);

	PrintMessage(MSGTYPE_SYSTEM,
		NULL,
		"Application Name: [%s]", szApplicationFileName);

	PrintMessage(MSGTYPE_SYSTEM,
		NULL,
		"Application Version: [%s]", szAppVersion);

	// terminate the application if unacceptable operating system
	//  or service pack version found.
	if(!g_bOkToRunApplication)
	{
		// not ok to run the application based on version
		//  of the operating system and service pack.

		// post reason for application exit
		SetExitCode(APP_EXIT_OPERATING_SYSTEM_VERSION_NG);

		// write a shutdown message.

		return iAppExitReason;
	}

	if(!InitializeApplication())
	{
		// post reason for application exit
		SetExitCode(APP_EXIT_INIT_APPLICATION_FAILED);

		// write a shutdown message.

		return iAppExitReason;
	}

	// Create the frame and do other initialization
	if(!InitializeInstance(nCmdShow))
	{
		// post reason for application exit
		SetExitCode(APP_EXIT_INIT_INSTANCE_FAILED);

		// write a shutdown message.
		return iAppExitReason;
	}

	// based on command line option or lack of one,
	//  select the source of data to be used,
	//  Network, or Local.
	if(bCmdLineFailSafeMode)
	{
		DataSource = DATA_SOURCE_LOCAL;

		// post DataSource as "LOCAL" to registry.
		//
		SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
			"DataSource",
			REGISTRY_POST_DATA_SOURCE_LOCAL);
		PrintMessage(MSGTYPE_SYSTEM,
			hwndGeneralMsg, 
			"Registry info - DataSource set to %d", 
			REGISTRY_POST_DATA_SOURCE_LOCAL);
	}
	else
	{
		DataSource = DATA_SOURCE_NETWORK;

		// post DataSource as "NETWORK" to registry.
		//
		SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
			"DataSource",
			REGISTRY_POST_DATA_SOURCE_NETWORK);
		PrintMessage(MSGTYPE_SYSTEM,
			hwndGeneralMsg, 
			"Registry info - DataSource set to %d", 
			REGISTRY_POST_DATA_SOURCE_NETWORK);
	}

	// based on command line option or lack of one,
	//  select the message level to be used
	//  this is more of a debug option, so it is tied
	//  to the command line option variable that enables
	//  the alternate, all options menu.
	if(bCmdLineUseProgrammerMenu)
	{
		// message enable settings set for programmer debug mode.
		MessageEnableMask = MESSAGE_GROUP_DEBUG;
	}
	else if(bCmdLineUseAlternateMenu)
	{
		// message enable settings set for programmer debug mode.
		MessageEnableMask = MESSAGE_GROUP_STANDARD;
	}
	else
	{
		// message enable settings set for standard customer mode.
		MessageEnableMask = MESSAGE_GROUP_STANDARD;
	}

	// initialization items before reading the configuration file.

	// create the mutexes to synchronize access
	//  between ReadComm() threads, and MainPageThread()
	//  variables protected are
	//  ComPort[].buffer, and ComPort[].nextWrite.
	//
	// initialize other "per comm port" data items.
	{
		int x;

		for(x = 0; x < MAX_COMMPORTS; x++)
		{
			hMutexComm[x] = CreateMutex(NULL, FALSE, NULL);

			pstate[x] = PRESET;
			pageerror[x] = PNOERROR;
			portactive[x] = FALSE;
			pageactive[x] = COMM_NOT_ACTIVE;

			ccounter[x] = 0;
			countA[x] = 0;
		}
	}

	// read parameter file.
	//  XnGetSetUp needs to know DataSource DATA_SOURCE_LOCAL or DATA_SOURCE_NETWORK.
	//  DataSource is determined from command line option.
	//
	if(XnGetSetUp() != 0)
	{
		// non-zero means configuration information is
		//  corrupt, not available, or not intelligible.
		//  sensible program execution is not possible without
		//  "good" configuration info.
		//
		iAppExitReason = APP_EXIT_BAD_PARAMETER_FILE;
		return(iAppExitReason);
	}

	// a few more misc items in the .ini file.
	ReadIniParameters();

	// program operating parameters have been read,
	//  now it is ok to use param.xxx items.
	//  now it is ok to use Queue[i].xxx items.

	// object creation...
	//

	// create the QueueFileManager objects 
	//  one for each possible Queue File nopgXX.que.
	//
	{
		int i;

		for(i = 0; i < MAX_QUEUE_FILES; i++)
		{
			CString Path;
			CString Filename;
			int iQueueNumber = 0;

			// properly defined queue name is '%' followed by 
			//  2 numeric digits, followed by nul char.
			//
			if((Queue[i].Id[0] == '%') && (Queue[i].Id[3] == 0))
			{
				// if the queue is defined in the configuration...
				//
				// take the 2 numeric digits after the '%' character,
				//  and transform into an integer.
				iQueueNumber = atoi(&Queue[i].Id[1]);

				if(DATA_SOURCE_NETWORK == DataSource)
				{
					// use network queue files.
					Path.Format("%s", param.AutoDir);
					Filename.Format("NOPG%s.QUE", Queue[i].Id + 1);
				}
				else
				{
					// use local queue files.
					Path.Format("%s", param.BkupDir);
					Filename.Format("NOPG%s.QUE", Queue[i].Id + 1);
				}
			}
			else
			{
				// not defined in the configuration...
				// define a "nonsense" filename.
				//
				Path.Format("%s", param.BkupDir);
				Filename.Format("NOPG%s.QUE", "XX");
			}

			// create the object.
			//
			Queue[i].pQFM = new QueueFileManager(Path, Filename, iQueueNumber);
			Queue[i].pQBC = new QueueBufferClass();
		}
	}

	// start one thread for each comm port that is configured.
	//  
	pageit(SETUP);
	pageit(ACTIVATE);

	hMainPageThread = (HANDLE) _beginthreadex(NULL,
		0,
		MainPageThread,
		(void *) NULL,
		CREATE_SUSPENDED,
		NULL);
	// Start a thread to check for a new .EXE every few seconds.
	//  This option is enabled by a command line option,
	//  and is for software development use.
	//  When a new .EXE file is detected,
	//  The thread will send a windows message
	//  to request that the application be closed.
	//  The application is intended to be started by a batch file.
	//  the batch file analyzes the exit code from the application
	//  and decides whether to re-start or exit the batch file.
	//  see the PAGERUN.BAT batch file, which is part of this project.
	//
	if(bCmdLineCheckForNewEXE)
	{
		// using the bCmdLineCheckForNewEXE automated option implies
		//  that you probably don't want to see
		//  the "ok to exit application?" dialog either.
		bCmdLineUseExitDialog = FALSE;

		hNewExeChecker = (HANDLE) _beginthreadex(NULL,
			0,
			CheckForNewExeThread,
			(void *) NULL,
			CREATE_SUSPENDED,
			NULL);
	}

	if(bCmdLineEnableExperimentThread)
	{
		hExperimentThread = (HANDLE) _beginthreadex(NULL,
			0,
			ExperimentThread,
			(void *) NULL,
			CREATE_SUSPENDED,
			NULL);
	}

	// Start a thread to check for automatic application exit-restart.
	//
	if(param.ExitForRestartEnable)
	{
		PrintMessage(MSGTYPE_SYSTEM, 
			hwndGeneralMsg, 
			"Exit-For-Restart IS enabled");

		hAppRestartChecker = (HANDLE) _beginthreadex(NULL,
			0,
			CheckForAppRestartThread,
			(void *) NULL,
			CREATE_SUSPENDED,
			NULL);
	}
	else
	{
		PrintMessage(MSGTYPE_SYSTEM, 
			hwndGeneralMsg, 
			"Exit-For-Restart NOT enabled");
	}


	// All initialization is done, the "other" threads
	//  can be allowed to run at this time, just before
	//  the main message loop is started.
	//
	// start threads that were created with CREATE_SUSPENDED option.
	{
		int x;

		for(x = 0; x < MAX_COMMPORTS; x++)
		{
			// if present in the system configuration,
			//  resume the comm port thread.
			if(portactive[x])
			{
				ResumeThread(ComPort[x].hReadThread);
			}
		}
	}

	// always start (resume) the "main page" thread.
	ResumeThread(hMainPageThread);

	// start (resume), if enabled.
	if(bCmdLineCheckForNewEXE)
	{
		ResumeThread(hNewExeChecker);
	}

	// start (resume), if enabled.
	if(bCmdLineEnableExperimentThread)
	{
		ResumeThread(hExperimentThread);
	}

	// start (resume), if enabled.
	if(param.ExitForRestartEnable)
	{
		ResumeThread(hAppRestartChecker);
	}

	// minimize main window on startup,
	//  this is important when many applications are running,
	//  and we don't want to bogart the screen.
	//
	if(bCmdLineMinimizeWindowAtStartup)
	{
		HWND hwnd;

		hwnd = FindWindow(szAppName, NULL);
		ShowWindow(hwnd, SW_SHOWMINIMIZED);
	}

	// Enter main "message loop"
	while(GetMessage(&msg, NULL, 0, 0))
	{
		// If a keyboard message is for the MDI , let the MDI client
		//  take care of it.  Otherwise, check to see if it's a normal
		//  accelerator key (like F3 = find next).  Otherwise, just handle
		//  the message as usual.
		//
		if(!TranslateMDISysAccel(hwndMDIClient, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// after message loop ends, or exception handler is done,
	//  continue here.

	// for better log message sequenceing, delay a short while
	//  for threads to sense, and respond to the value of the bKillThreads flag.
	//
	Sleep(3000);
	PrintMessage(MSGTYPE_SYSTEM, 
		hwndGeneralMsg, 
		"Windows \"message loop\" exited, begin shutting down threads.");

	// wait for threads to be signalled == shutdown.


	/*generalLogStream.flush();
	phoneLogStream.flush();
	commLogStream.flush();*/
	dbLogStream->flush();



	// wait for "look for new exe" thread to terminate, if the thread is enabled.
	if(bCmdLineCheckForNewEXE)
	{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "begin waiting for CheckForNewExeThread to stop.");

		switch(WaitForSingleObject(hNewExeChecker, 10000))
		{
		case WAIT_OBJECT_0:
			{
				DWORD dwExitCode;

				// thread ended, retrieve the exit code
				GetExitCodeThread(hNewExeChecker, &dwExitCode);

				CloseHandle(hNewExeChecker);
			}
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_OBJECT_0");
			break;

		case WAIT_TIMEOUT:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_TIMEOUT");
			break;

		default:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default");
			break;
		}
	}

	// wait for "exit for restart" thread to terminate, if the thread is enabled.
	//
	if(param.ExitForRestartEnable)
	{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "begin waiting for CheckForAppRestartThread to stop.");

		switch(WaitForSingleObject(hAppRestartChecker, 10000))
		{
		case WAIT_OBJECT_0:
			{
				DWORD dwExitCode;

				// thread ended, retrieve the exit code
				GetExitCodeThread(hAppRestartChecker, &dwExitCode);

				CloseHandle(hAppRestartChecker);
			}
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_OBJECT_0");
			break;

		case WAIT_TIMEOUT:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_TIMEOUT");
			break;

		default:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default");
			break;
		}
	}

	// wait for "Experiment" thread to terminate, if the thread is enabled.
	//
	if(bCmdLineEnableExperimentThread)
	{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "begin waiting for ExperimentThread to stop.");

		switch(WaitForSingleObject(hExperimentThread, 10000))
		{
		case WAIT_OBJECT_0:
			{
				DWORD dwExitCode;

				// thread ended, retrieve the exit code
				GetExitCodeThread(hNewExeChecker, &dwExitCode);

				CloseHandle(hExperimentThread);
			}
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_OBJECT_0");
			break;

		case WAIT_TIMEOUT:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_TIMEOUT");
			break;

		default:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default");
			break;
		}
	}

	// wait for the "main page" thread to terminate, it is always enabled.
	//
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "begin waiting for MainPageThread to stop.");
	switch(WaitForSingleObject(hMainPageThread, 10000))
	{
	case WAIT_OBJECT_0:
		{
			DWORD dwExitCode;

			// thread ended, retrieve the exit code
			GetExitCodeThread(hMainPageThread, &dwExitCode);

			CloseHandle(hMainPageThread);
		}
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_OBJECT_0");
		break;

	case WAIT_TIMEOUT:
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "WAIT_TIMEOUT");
		break;

	default:
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default");
		break;
	}

	pageit(DEACTIVATE);

	// object cleanup.
	//
	// delete the QueueFileManager objects.
	{
		int i;

		for(i = 0; i < MAX_QUEUE_FILES; i++)
		{
			delete Queue[i].pQFM;
			delete Queue[i].pQBC;
		}
	}

	// release the mutex, so that another instance 
	//  may be allowed to run in the future.
	//	
	if(ReleaseMutex(g_hmutexPageServerAppRun) != 0)
	{
		PrintMessage(MSGTYPE_SYSTEM,
			NULL,
			"ApplicationRun mutex WAS released.");
	}
	else
	{
		PrintMessage(MSGTYPE_SYSTEM,
			NULL,
			"ApplicationRun mutex NOT released.");
	}

    //SO - 04/09/2013 - Commented due to memory leak error - 0x13 bytes
	//malloc(16);


	// post shutdown mode information to the registry.
	//  2 items are required.
	//
	//  RunMode is Shutdown.
	//  DataSource is None.
	//
	// post RunMode as "SHUTDOWN".
	//
	SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
		"RunMode",
		REGISTRY_POST_RUN_MODE_SHUTDOWN);
	PrintMessage(MSGTYPE_SYSTEM, 
		hwndGeneralMsg, 
		"Registry info - RunMode set to %d", 
		REGISTRY_POST_RUN_MODE_SHUTDOWN);

	// post DataSource as "NONE".
	SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
		"DataSource",
		REGISTRY_POST_DATA_SOURCE_NONE);
	PrintMessage(MSGTYPE_SYSTEM, 
		hwndGeneralMsg, 
		"Registry info - DataSource set to %d", 
		REGISTRY_POST_DATA_SOURCE_NONE);


	PrintMessage(MSGTYPE_SYSTEM, 
		hwndGeneralMsg, 
		"WinMain(): - Application Exit Code: [%d] [%s]",
		iAppExitReason,
		szAppExitReason[iAppExitReason]);

	// final messages to message file.
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "PageServer Is Shut Down!");

	PrintMessage(MSGTYPE_SYSTEM,
		NULL,
		"WinMain(): Application Stop.");


	return iAppExitReason;
	//exit(iAppExitReason);
	}
}


// all database access - ADO needs to be from within this
//  one thread, for now, for simplicity.
DataBaseAccessor* pDBA = 0;
SpoolCollection* pscListPage = 0;

	// go active:
	//  perform startup tasks.
	//
	void
GoActive(void)
	{
	CString csTemp;

	// clear screen on all "status" windows,

	// "Queue Status" Window.
	InitializeQueueStatusWindow(0);

	// "IVR Port Status" Window.
	InitializePhoneLineStatusWindow(0);

	// "Comm Port Status" Window.
	InitializeCommPortStatusWindow(0);

	// show the "Activated" information in the status line
	//	when in the "active" mode, supress when not externally controlled.
	//
	if(bExternalControl)
		{
		csTemp = "  A c t i v a t e d";
		SetStatusText(STATUS_LINE_MESSAGE, (LPTSTR) (LPCTSTR) csTemp);
		}

	XnInitialize();

	WriteLog("NOPGERR", (LPTSTR) (LPCTSTR) csTemp);
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, (LPTSTR) (LPCTSTR) csTemp);

	::CoInitialize(NULL);	//JPH ADDED 01/04/13
	//::CoInitializeEx(NULL, COINIT::COINIT_APARTMENTTHREADED);
	XnOpenDb();

	PageThePageSupervisor(PAGE_MISC_POS);

	// post to registry so that external programs can know 
	//  the pageserver internal operating mode.
	//
	SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
						"RunMode",
						REGISTRY_POST_RUN_MODE_ACTIVE);

 	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"Registry info - RunMode set to %d",
					REGISTRY_POST_RUN_MODE_ACTIVE);
	}


	// go inactive:
	//  perform shutdown tasks.
	//
	void
GoStandby(void)
	{
	CString csTemp;

	PrintMessage(MSGTYPE_SYSTEM, hwndDatabaseMsg, "GoStandby():");

	// this is a race condition...
	//  page the page supervisor,
	//  but everything is shutting down.
	PageThePageSupervisor(PAGE_MISC_POS);

	csTemp = "  S t a n d i n g   B y";
	SetStatusText(STATUS_LINE_MESSAGE, (LPTSTR) (LPCTSTR) csTemp);

	WriteLog("NOPGERR", (LPTSTR) (LPCTSTR) csTemp);
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, (LPTSTR) (LPCTSTR) csTemp);

	CloseMiscFlatFiles();

	PrintMessage(MSGTYPE_SYSTEM, hwndDatabaseMsg, "GoStandby(): close database");
	XnCloseDb();

	// post to registry so that external programs can know 
	//  the pageserver internal operating mode.
	//
	SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
						"RunMode",
						REGISTRY_POST_RUN_MODE_STANDBY);

 	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"Registry info - RunMode set to %d",
					REGISTRY_POST_RUN_MODE_STANDBY);

	// finally, clear screen on all "status" windows,
	//  and post a "standing by" message in each window.

	// "Queue Status" Window.
	InitializeQueueStatusWindow(1);

	// "IVR Port Status" Window.
	InitializePhoneLineStatusWindow(1);

	// "Comm Port Status" Window.
	InitializeCommPortStatusWindow(1);
	}


	// startup, in the inactive state,
	//  the system has never been active, or inactive,
	//  this is only used at system startup.
	//
	void
StartInStandby()
	{
	char string2[200];

	// post to registry so that external programs can know 
	//  the pageserver internal operating mode.
	//
	SetRegistryULong("HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Settings",
						"RunMode",
						REGISTRY_POST_RUN_MODE_STANDBY);

 	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"Registry info - RunMode set to %d",
					REGISTRY_POST_RUN_MODE_STANDBY);

	// finally, clear screen on all "status" windows,
	//  and post a "standing by" message.
	//
	sprintf(string2, "  s t a n d i n g   b y");
	SetStatusText(STATUS_LINE_MESSAGE, string2);

	// "Queue Status" Window.
	//  parameter == 1 means standby message is to be used.
	//
	InitializeQueueStatusWindow(1);

	// "IVR Port Status" Window.
	//  parameter == 1 means standby message is to be used.
	//
	InitializePhoneLineStatusWindow(1);

	// "Comm Port Status" Window.
	//  parameter == 1 means standby message is to be used.
	//
	InitializeCommPortStatusWindow(1);
	}

    //SO - 04/15/2013 - This is to log any invalid parameter or buffer size 
void 
myInvalidParameterHandler(const wchar_t* expression,
    const wchar_t* function, 
    const wchar_t* file, 
    unsigned int line, 
    uintptr_t pReserved)

    {
       char buffer[1000];
   
       wsprintf(buffer, "Invalid parameter detected in function %S."
                " File: %S Line: %d\n", function, file, line);    

        PrintMessage(MSGTYPE_SYSTEM, 
					    hwndGeneralMsg, 
					    "%s", buffer);

        wsprintf(buffer, "Expression: %S\n", expression);

        PrintMessage(MSGTYPE_SYSTEM, 
					    hwndGeneralMsg, 
					    "%s", buffer);
   
    }

	unsigned __stdcall
MainPageThread(void *)
	{
	int j;
	char Today[10];
	char string3[85];
	char string2[200];
	CString csConnectionString;
	int Event = EVENT_NONE;

	// configuration file has been read before this section
	//  is reached.
	//
	// some options are disabled or ignored.
	//  no matter what you enter in the configuration file,
	//  these options will remain programmaticaly disabled.

	// delay on startup X seconds;
	Sleep(2000);

    //SO - 04/15/2013 - This is to log any invalid parameter or buffer size 
    _invalid_parameter_handler oldHandler, newHandler;
   newHandler = myInvalidParameterHandler;
   oldHandler = _set_invalid_parameter_handler(newHandler);

   // Disable the message box for assertions.
   _CrtSetReportMode(_CRT_ASSERT, 0);

	// restore on-screen "status" window positions.
	XnRestoreWindowPosition(hwndComStatus,  "COM_STATUS");
	XnRestoreWindowPosition(hwndD4xStatus,  "D4X_STATUS");
	XnRestoreWindowPosition(hwndQueStatus,  "QUE_STATUS");


	// Dead code ... removed 2010-07-14 kgill.
	//
	//
	//set voice message type from ini file
	//set canceled page email from ini file
	//
	//	std::string pFileName(param.BkupDir);
	//	pFileName += "\\PageSetup.INI";

	//	DWORD ret = GetPrivateProfileString(
	//		"Message",						// points to section name
	//		"VoiceMessageType",				// points to key name
	//		"S ",								// points to default string
	//		voiceMessageTypeStr,				// points to destination buffer
	//		sizeof(voiceMessageTypeStr),      // size of destination buffer
	//		static_cast<const char*>(pFileName.c_str())		// points to initialization filename
	//		);


	// based on command line option or lack of one,
	//  display Failsafe Mode, or Newtork Mode.
	//  in the status line.
	{
	CString csTemp;

	if(DATA_SOURCE_LOCAL == DataSource)
		{
		csTemp = "  Failsafe Mode";
		}
	else
		{
		csTemp = "  Network Mode";
		}

	SetStatusText(STATUS_LINE_DATABASE_MODE, (LPTSTR) (LPCTSTR) csTemp);
	}

	// delay on startup X seconds;
	Sleep(1000);

	memset(string2, 0, sizeof(string2));
    memset(string3, 0, sizeof(string3));

	// variable Today contains MMDD
    MSGTimeStamp(string2, string3);

    strncpy(Today, string2, 4);
	Today[4] = 0;

	// to start... zero Dialogic channels are available.
	totalD4xChannels = 0;

	// don't do initialization if dialogic is not configured
	//  for use by PageServer.
	//
	if(bCmdLineUseDialogicHardware)
		{
		XnD4xInitialize();
		}

	// setup to use impairment messages if this feature is configured.
	if(param.UseImpairmentAnnouncements)
		{
		// impairment feature is enabled,
		//  set some properties of the Impairment class
		//  these were read as configuration items in
		//  xgprm1.xtd file.

		// the functionality is contained in a class
		//  the classname is: "Impairment", and the global instance of this class is: "g_IMP".

		// any that are defined in the config parameters will be used, 
		//  otherwise default values will be used.
		if(param.ImpairmentCheckInterval > 0)
			{
			// a non-zero number is defined.
			g_IMP.setCheckInterval(param.ImpairmentCheckInterval);
			}

		if(param.ImpairmentProfileIdPattern[0] != 0)
			{
			// the first character of the string is non-null, the item is defined.
			g_IMP.setProfileIdPattern(param.ImpairmentProfileIdPattern);
			}

		if(param.ImpairmentStatusToEnable != 0)
			{
			// the first character of the string is non-null, the item is defined.
			g_IMP.setStatusToEnable(param.ImpairmentStatusToEnable);
			}
		}

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"param.ImpairmentProfileIdPattern: [%s]", 
					param.ImpairmentProfileIdPattern);

	// initialize resources for input from device to see if
	//  external control is being used.
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "MODE_IDLE - init...");

//	// initialize anything that needs initializing...
//	//
//	if(bExternalControl && (param.Lpt.Address > 0))
//		{
//		// parallel port - external control being used.
//		}

	if(bExternalControl && (g_iExternalControlCommPortToUse != -1))
		{
		// serial port - external control being used.
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"MainPageThread() opening... [%s]:", 
						CommportName[g_iExternalControlCommPortToUse].FamiliarName);

		ComPort[g_iExternalControlCommPortToUse].hComm = CreateFile(CommportName[g_iExternalControlCommPortToUse].OSName,
																		GENERIC_READ | GENERIC_WRITE,
																		(DWORD) 0,
																		0,
																		(DWORD) OPEN_EXISTING,
																		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
																		NULL);
		// make RTS low, and DTR high.
		EscapeCommFunction(ComPort[g_iExternalControlCommPortToUse].hComm, CLRRTS);
		EscapeCommFunction(ComPort[g_iExternalControlCommPortToUse].hComm, SETDTR);			
		}

	// the pageserver main program loop.
	while(!bKillThreads)
		{
		// the thread main loop starts here!

		// test code - begin
		// this test code is to test structured exception handling

		// create an intentional memory access exception
		if(bActivateIntentionalAccessException)
			{
			// Win32 API call to attempt to
			//  copy memory from location 0,
			//  to location 0, length 1 byte.
			::CopyMemory(0, 0, 1);
			}

		// create an intentional zero divide exception
		if(bActivateIntentionalZeroDivideException)
			{
			int y = 0;
			int z;

			// an attempt to divide by zero
			//  will cause a "divide by zero" exception
			z = 1/y;
			}

		// test code - end

//		// check if external control is enabled.
//		//  this is to control ACTIVE/STANDBY mode
//		//  using an input bit of the parallel port.
//		//
//		// if enabled, sample the input bit (parallel port, or serial port)
//		//  once per second.
//		//
//		if(bExternalControl && (param.Lpt.Address > 0))
//			{
//			if(LptTime.wHour == 70)
//				{
//				GetLocalTime(&LptTime);
//				}
//
//			// see if 1 second has passed. 100/100 == 1 sec.
//			j = Wait(&LptTime, 100L);
//
//			if(j)
//				{
//				LptTime.wHour = 70;
//
//				Event = GetExternalControlEvent();
//
//				//SetResetBit();
//				}
//			}
//
		if(bExternalControl && (g_iExternalControlCommPortToUse != -1))
			{
			if(LptTime.wHour == 70)
				{
				GetLocalTime(&LptTime);
				}

			// see if 1 second has passed. 100/100 == 1 sec.
			j = Wait(&LptTime, 100L);

			if(j)
				{
				LptTime.wHour = 70;

				Event = GetExternalControlEvent();
				}
			}

		// new page server operating mode
		//  state machine.
		//
		switch(PsOpMode)
			{
			case MODE_IDLE:
				// this is the active mode at system startup.
				//  decide what to do based on whether
				//  ExternalControl is enabled.
				//  if external control is not enabled,
				//   simply enter the ALWAYS_ACTIVE mode.
				//  if external control is enabled, then
				//   we must wait for the external signal
				//   to become stable, and produce a
				//   EVENT_GO_ACTIVE or EVENT_GO_STANDBY signal.
				//
				if(bExternalControl)
					{
					// action based on an event from the parallel port monitoring
					//  the event is extracted about once per second,
					//  see above GetExternalControlEvent().
					//
					switch(Event)
						{
						case EVENT_GO_ACTIVE:
							PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "MODE_IDLE - EVENT_GO_ACTIVE");
							GoActive();
							PsOpMode = MODE_ACTIVE;
							break;

						case EVENT_GO_STANDBY:
							PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "MODE_IDLE - EVENT_GO_STANDBY");
							StartInStandby();
							PsOpMode = MODE_STANDBY;
							break;

						case EVENT_NONE:
						default:
							// ignore these.
							break;
						}
					// clear the value of the event to EVENT_NONE
					//  after it has been processed,
					//  a new event is found only every 1 second or so.
					}
				else
					{
					GoActive();
					PsOpMode = MODE_ALWAYS_ACTIVE;
					}
				break;

			case MODE_ALWAYS_ACTIVE:
				// no possible exit from this state.
				//  don't even attempt to get an "event",
				//  we don't own the parallel port.
				//
				break;

			case MODE_STANDBY:
				switch(Event)
					{
					case EVENT_GO_ACTIVE:
						GoActive();
						PsOpMode = MODE_ACTIVE;
						break;

					case EVENT_GO_STANDBY:
					case EVENT_NONE:
					default:
						// ignore these.
						break;
					}
				break;

			case MODE_ACTIVE:
				switch(Event)
					{
					case EVENT_GO_STANDBY:
						GoStandby();
						PsOpMode = MODE_STANDBY;
						break;

					case EVENT_GO_ACTIVE:
					case EVENT_NONE:
					default:
						// ignore these.
						break;
					}
				break;
			}


		// the bulk of the main loop...

		//  the main loop runs if in MODE_ALWAYS_ACTIVE
		//  or MODE_ACTIVE, other modes are some kind of
		//  standby, or not running yet.
		//
		if(MODE_ACTIVE == PsOpMode || MODE_ALWAYS_ACTIVE == PsOpMode)
			{
			// not standing by... do important work...

			// process list queue if enabled, 
			//  always process the list in standalone/failsafe mode.
			if(param.bEnableListQueueProcessing || (DATA_SOURCE_LOCAL == DataSource))
				{
				// List Queue Processing.

				// Every 2 seconds the List (usually %21) will take a chance to run.  
				//  If at that time there is nothing to process, 
				//  the lower priority list can be considered.
				//
				CheckCodePriorityListQueue();
				}

			if(totalD4xChannels > 0)
				{
				D4xWorkerThread();
				}

			scanaport();
			CheckCommState();

			for(j = 0; j < totalD4xChannels; j++)
				{
				CheckD4xMessages(j, Today);

				RCC_Check(j);
				Check_D4xHookState(j);
				Check_D4x_Add_Que(j);
				Check_RCC_Queues(j);
				}

			// Check to see if the multiple page queue needs to be processed.
			if(param.MultiQueNumber > 0)
				CheckMultiQue( param.MultiQueNumber, &MultiQTimer1, param.MultiPageQues);

			if(param.MultiQueNumber2 > 0)
				CheckMultiQue( param.MultiQueNumber2, &MultiQTimer2, param.MultiPageQues2);

			CheckCallSign();
			CheckCatsCall();
			CheckKeepAlive();

			CheckQueueErrorMonitor();
			
			HourMidnight();

			// if the Impairment feature is enabled, call the service routine.
			//
			if(param.UseImpairmentAnnouncements)
				{
				// check to see if the "Impairment" Profiles need to be read.
				//  The database read will only be performed when and internal
				//  timeout expires... to keep the overhead low.
				//
				g_IMP.CheckImpairment();
				}

			// service routine for "Queue Status Window" display update.
			//  display for all queue files, whether or not they are serviced 
			//  (read-from) by PageServer.
			//
			if(QueueDisplayUpdateTime.wHour == 70)
				{
				GetLocalTime(&QueueDisplayUpdateTime);
				}

			// update the screen display information at regular intervals.
			//
			if(Wait(&QueueDisplayUpdateTime, 300L))
				{
				QueueDisplayUpdateTime.wHour = 70;

				// update only in "active" modes.

				switch(PsOpMode)
					{
					case MODE_ACTIVE:
					case MODE_ALWAYS_ACTIVE:
						// update the queue information.
						//
						{
						for(int iQueueFileNumber = 0; 
							iQueueFileNumber < MAX_QUEUE_FILES; 
							iQueueFileNumber++)
							{
							// check if defined in queue control.
							if(Queue[iQueueFileNumber].Id[0] == '%')
								{
								// call and ignore the results of IsItemAvailable(),
								//  this will refresh the data that will be displayed by
								//  UpdateQueueStatusWindow().
								//
								// note that if the call to IsItemAvailable() was not made, 
								//  the data for queues that are not serviced by
								//  PageServer would not be updated, and would not change.
								//
								Queue[iQueueFileNumber].pQFM->IsItemAvailable();

								UpdateQueueStatusWindow(iQueueFileNumber);
								}
							}
						}
						break;

					case MODE_IDLE:
					default:
						// do nothing...
						break;
					}
				}
			}
		else
			{
			// when in the standby mode... do just about nothing.
			}

		// don't bogart all cpu time.
		Sleep(1);
		}

	// Application is closing, this thread is ending.


	if(bCmdLineUseDialogicHardware)
		{
		XnD4xClose();
		}

	// if shutting down from standby mode... 
	//  no cleanup should be necessary.
	//  and it is important not to attempt closing / cleaning up
	//  resources that don't exist or were never opened.
	//  (don't invite Dr. Watson)
	//
	if(MODE_ACTIVE == PsOpMode || MODE_ALWAYS_ACTIVE == PsOpMode)
		{
		// in active modes, close resources.
		CloseMiscFlatFiles();
		XnCloseDb();
		}

	// close resources used used for active/standby control.
	//  particularly the serial port used for input.
	//
	if(MODE_ACTIVE == PsOpMode || MODE_STANDBY == PsOpMode)
		{	
		// un-initialize anything that needs it...
		//
//		if(bExternalControl && (param.Lpt.Address > 0))
//			{
//			// parallel port - external control being used.
//			}

		if(bExternalControl && (g_iExternalControlCommPortToUse != -1))
			{
			// serial port - external control being used.
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"MainPageThread: closing... [%s]", 
							CommportName[g_iExternalControlCommPortToUse].FamiliarName);

			// make RTS low, and DTR low.
			EscapeCommFunction(ComPort[g_iExternalControlCommPortToUse].hComm, CLRRTS);
			EscapeCommFunction(ComPort[g_iExternalControlCommPortToUse].hComm, CLRDTR);	

			Sleep(1000);
			
			if(CloseHandle(ComPort[g_iExternalControlCommPortToUse].hComm))
				{
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndComMsg,
								"MainPageThread(): [%s]: Closed.",
								CommportName[g_iExternalControlCommPortToUse].FamiliarName);
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndComMsg,
								"MainPageThread(): Error [%s]: Not Closed.",
								CommportName[g_iExternalControlCommPortToUse].FamiliarName);
				}
			}
		}
   
	return 0;
	}


	LONG CALLBACK
MPFrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	static int wCharHeight;

    switch(msg)
		{
        case WM_CREATE:
			{
			CLIENTCREATESTRUCT ccs;
			TEXTMETRIC tm;
            HDC hdc;

			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			wCharHeight = (int) (tm.tmHeight + tm.tmExternalLeading + 4);
			ReleaseDC(hwnd, hdc);

            // Find window menu where children will be listed
            ccs.hWindowMenu = GetSubMenu(GetMenu(hwnd), WINDOWMENU);
            ccs.idFirstChild = IDM_WINDOWCHILD;

            // Create the MDI client filling the client area
			InitCommonControls();

            hwndMDIClient = CreateWindow("mdiclient",
                                          NULL,
                                          WS_CHILD | WS_CLIPCHILDREN |
                                          WS_VSCROLL | WS_HSCROLL,
                                          0,
                                          0,
                                          0,
                                          HIWORD(lParam) - wCharHeight - 4,
                                          hwnd,
                                          (HMENU)0xCAC,
                                          hInst,
                                          (LPSTR)&ccs);

            ShowWindow(hwndMDIClient,SW_SHOW);

			{
			RECT rc;

			GetClientRect(hStatusWindow, &rc);
			InvalidateRect(hStatusWindow, &rc, TRUE);
			}
			break;
			}

		case WM_INITMENU:
            // Set up the menu state
            InitializeMenu((HMENU)wParam);
            break;

		case WM_PAINT:
			{
			PAINTSTRUCT ps;
			BeginPaint( hwndFrame, &ps );
			ps.fIncUpdate = TRUE;
			EndPaint ( hwndFrame, &ps );
			}
			break;

        case WM_COMMAND:
            // Direct all menu selection or accelerator commands to another
            //  function
            CommandHandler(hwnd, wParam, lParam);
            break;

        case WM_CLOSE:
			{
			int Answer = IDNO;

			// ok to present the dialog box, or just skip it.
			if(bCmdLineUseExitDialog)
				{
				Answer = ::MessageBox(GetFocus(),
									"Are you sure you want to exit?",
									"SPOK PAGESERVER",
									(UINT) (MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION));
				}
			else
				{
				// if you skip the dialog box, the answer must be set to
				//  IDYES, as if returned by the messageboox.
				Answer = IDYES;
				}

			// if the answer is IDYES, then
			if(Answer == IDYES)
				{
				// save on-screen positions of "status" windows.
				XnSaveWindowPosition(hwndComStatus,  "COM_STATUS");
				XnSaveWindowPosition(hwndD4xStatus,  "D4X_STATUS");
				XnSaveWindowPosition(hwndQueStatus,  "QUE_STATUS");

				// go ahead and perform shut-down tasks.

				// note that the sequence here, is significant.
				//  because the message type is NOT checked,
				//  only the extension (ProfileId).
				//
				//  1- remove type 20 message on this profile,
				//  2 - add type 21 message to this profile.

				// if keep-alive is active, cancel the future notification alarm
				//  message type "20" on normal application shutdown.
				if(param.keepAliveAlarm)
					{
					// note:  delete deletes the record
					//  now respecting message type. 2006-04-26.
					CString csText;
					csText.Format("%-2.2s", param.ActionIdForKeepAlive);

					SetActionReminder(param.ActionReminderProfileId, 
										param.ActionIdForKeepAlive, 
										0, 
										REMINDER_DELETE, 
										0, 
										csText);
					}

				// check if "page server exit alarm" is enabled.
				if(param.exitAlarm)
					{
					// add one.
					CString csText;
					csText.Format("%-2.2s", param.ActionIdForApplicationExit);

					SetActionReminder(param.ActionReminderProfileId, 
										param.ActionIdForApplicationExit, 
										0, 
										REMINDER_ADD, 
										0, 
										csText);
					}

				// indicate normal SYSTEM-requested shutdown
				//  the iAppExitReason may have been set earlier
				//  if the menu item was selected versus the
				//  system menu X box.
				//  The exit code can only be set once,
				//  first requester wins.

				// post reason for application exit
				SetExitCode(APP_EXIT_USER_REQUEST_SYSTEM);

				DestroyWindow(hwnd);
				}
			}
            break;

         case WM_DESTROY:
 			PostQuitMessage(0);		// allow the message loop to exit.
									//  the standard place for this call
									//  is in the WM_DESTROY message.
			return 0;

         case WM_NCDESTROY:
			// this is the final message to the closing window.

			 // set the window handle to NULL to indicate that
			 //  no more messages should be sent to this window.
			hwndFrame = NULL;

			// try setting all window handles to NULL, once
			//  the frame window is gone, all child windows
			//  are certainly closed.
			//
			// with this, I am trying to prevent access violations
			//  C0000005 on program close.
			hwndGeneralMsg = NULL;
			hwndD4xMsg = NULL;
			hwndComMsg = NULL;
			hwndDatabaseMsg = NULL;

			hwndD4xStatus = NULL;
			hwndQueStatus = NULL;
			hwndComStatus = NULL;

			// request that all threads shut themselves down.
			bKillThreads = TRUE;

			break;

		case WM_SIZE:
			{
			if(hStatusWindow)
				{
				int nParts[STATUS_LINE_N_PARTS];

				MoveWindow(hwndMDIClient,
							0,
							0,
							LOWORD(lParam),
							HIWORD(lParam) - wCharHeight-4,
							TRUE);

				MoveWindow(hStatusWindow,
							0,
							HIWORD(lParam) - wCharHeight,
							LOWORD(lParam),
							HIWORD(lParam),
							TRUE);

				// the numbers below represent the column widths of the segments 
				//  in the status bar from left to right. note that the leftmost
				//  column is typically wider than requested, depending on window size.
				nParts[0] = LOWORD (lParam) - (200 + 100 + 175 + 135 + 105 + 140);
				nParts[1] = LOWORD (lParam) - (      100 + 175 + 135 + 105 + 140);	
				nParts[2] = LOWORD (lParam) - (            175 + 135 + 105 + 140);	
				nParts[3] = LOWORD (lParam) - (                  135 + 105 + 140);	
				nParts[4] = LOWORD (lParam) - (                        105 + 140);               
				nParts[5] = LOWORD (lParam) - (                              140);	
				nParts[6] = -1;

				SendMessage(hStatusWindow,
							SB_SETPARTS,
							STATUS_LINE_N_PARTS,
							(LPARAM) nParts);
				{
				CString csTemp;
				csTemp.Format("  %s    %s", szApplicationFileName, szAppVersion);
				csTemp.MakeUpper();

				SetStatusText(STATUS_LINE_VERSION, (LPTSTR) (LPCTSTR) csTemp);
				}

				// show build time and date only in test mode(s).
				if(bCmdLineUseProgrammerMenu)
					{
					SetStatusText(STATUS_LINE_BUILD_DATE_AND_TIME, "  " __DATE__"   " __TIME__);
					}
				{
				// update after screen re-size.
				char TempString[40];

				sprintf(TempString, " %s", "  ");
				SetStatusText(STATUS_LINE_EXPERIMENT, TempString);
				}
				}
			}
			return DefFrameProc (hwnd,hwndMDIClient,msg,wParam,lParam);

        default:
			//  use DefFrameProc() instead of DefWindowProc() since there
            //  are things that have to be handled differently because of MDI
			return DefFrameProc (hwnd,hwndMDIClient,msg,wParam,lParam);
		}
    return 0;
	}


	LONG CALLBACK
MPMDIChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	int selectedLine;
    HWND hwndEdit;
	HMENU hmenu;
	char temp[1000];

    switch(msg)
		{
        case WM_CREATE:
            // Create an edit control
             hwndEdit = CreateWindow("listbox",
                                     NULL,
                                     WS_CHILD | WS_HSCROLL | WS_MAXIMIZE | \
									 WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
                                     0,
                                     0,
                                     0,
                                     0,
                                     hwnd,
                                     (HMENU)ID_EDIT,
                                     hInst,
                                     NULL);
			hmenu = GetSystemMenu(hwnd, FALSE);
			DeleteMenu(hmenu, SC_CLOSE, (UINT)MF_BYCOMMAND);
			DeleteMenu(hmenu, 5, (UINT)MF_BYPOSITION);

            // Remember the window handle and initialize some window attributes
            SetWindowLong(hwnd, GWL_HWNDEDIT, (LONG) hwndEdit);
            SetFocus(hwndEdit);
				{
				RECT rc;
				GetClientRect(hStatusWindow, &rc);
				InvalidateRect( hStatusWindow, &rc, TRUE );
				}
            break;

        case WM_MDIACTIVATE:
            // If we're activating this child, remember it
            if(GET_WM_MDIACTIVATE_FACTIVATE(hwnd, wParam, lParam))
				{
                hwndActive     = hwnd;
                hwndActiveEdit = (HWND) GetWindowLong(hwnd, GWL_HWNDEDIT);
				}
            else
				{
                hwndActive     = NULL;
                hwndActiveEdit = NULL;
				}
            break;

		case WM_MOVE:
			{
			RECT rc;

			GetClientRect(hStatusWindow, &rc);
			InvalidateRect(hStatusWindow, &rc, TRUE);

			GetClientRect(hwnd, &rc);
			InvalidateRect(hwnd, &rc, TRUE);

			goto CallDCP;
			}

        case WM_SIZE:
			{
			RECT rc;

			// On creation or resize, size the edit control.
			hwndEdit = (HWND)GetWindowLong(hwnd, GWL_HWNDEDIT);
			GetClientRect(hwnd, &rc);
			MoveWindow(hwndEdit, (int)rc.left, (int)rc.top, (int)(rc.right - rc.left),
				(int)(rc.bottom - rc.top), TRUE );

			GetClientRect(hStatusWindow, &rc);
			InvalidateRect( hStatusWindow, &rc, TRUE );

			goto CallDCP;
			}

        case WM_SETFOCUS:
            SetFocus ((HWND)GetWindowLong (hwnd, GWL_HWNDEDIT));
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
				{
                case ID_EDIT:
                    switch (GET_WM_COMMAND_CMD(wParam, lParam))
						{
						case LBN_DBLCLK:
							selectedLine = (int) SendMessage((HWND) lParam,
																LB_GETCURSEL,
																(WPARAM) 0,
																(LPARAM) 0);
							if( selectedLine != LB_ERR )
								{
								SendMessage((HWND) lParam,
											LB_GETTEXT,
											(WPARAM) selectedLine,
											(LPARAM) temp);

								::MessageBox(GetFocus(),
												temp,
												"Information",
												(UINT) (MB_OK | MB_ICONINFORMATION));
								}
							break;

                        case EN_CHANGE:
                            // If the contents of the edit control have changed,
                            //   set the changed flag
                            SetWindowWord(hwnd, GWW_CHANGED, TRUE);
							break;

                        case EN_ERRSPACE:
                            // If the control is out of space, honk
                            // MessageBeep (0);
                            break;

                        default:
                            goto CallDCP;
						}
                    break;

                default:
                    goto CallDCP;
				}
            break;

CallDCP:
        default:
            // Again, since the MDI default behaviour is a little different,
            // call DefMDIChildProc instead of DefWindowProc()
            return DefMDIChildProc (hwnd, msg, wParam, lParam);
		}

    return FALSE;
	}


	LONG CALLBACK
MPMDIStatusWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
    HWND	hwndEdit;
	HMENU	hmenu;

	switch(msg)
		{
        case WM_CREATE:
           // Create an edit control
             hwndEdit = CreateWindowEx( WS_EX_DLGMODALFRAME,
				 "listbox",
                 NULL,
                 WS_CHILD | WS_VISIBLE| WS_VSCROLL | \
				 LBS_NOTIFY | LBS_USETABSTOPS,
                 0,
                 0,
                 0,
                 0,
                 hwnd,
                 (HMENU) ID_EDIT,
                 hInst,
                 NULL);
			hmenu = GetSystemMenu(hwnd, FALSE);
			DeleteMenu(hmenu, SC_CLOSE, (UINT) MF_BYCOMMAND);
			DeleteMenu(hmenu, 5, (UINT)MF_BYPOSITION);
			DeleteMenu(hmenu, 2, (UINT)MF_BYPOSITION);

            // Remember the window handle and initialize some window attributes
            SetWindowLong(hwnd, GWL_HWNDEDIT, (LONG) hwndEdit);
            SetFocus(hwndEdit);
				{
				RECT rc;
				GetClientRect(hStatusWindow, &rc);
				InvalidateRect(hStatusWindow, &rc, TRUE);
				}
            break;

        case WM_MDIACTIVATE:
            // If we're activating this child, remember it
            if (GET_WM_MDIACTIVATE_FACTIVATE(hwnd, wParam, lParam))
				{
                hwndActive     = hwnd;
                hwndActiveEdit = (HWND)GetWindowLong (hwnd, GWL_HWNDEDIT);
				}
            else
				{
                hwndActive     = NULL;
                hwndActiveEdit = NULL;
				}
            break;

		case WM_MOVE:
			{
			RECT rc;

			GetClientRect(hwnd, &rc);
			InvalidateRect(hwnd, &rc, TRUE);

			GetClientRect(hStatusWindow, &rc);
			InvalidateRect(hStatusWindow, &rc, TRUE);

			goto CallDCP;
			}

        case WM_SIZE:
			{
			RECT rc;

			// On creation or resize, size the edit control.
			hwndEdit = (HWND) GetWindowLong(hwnd, GWL_HWNDEDIT);
			GetClientRect(hwnd, &rc);
			MoveWindow(hwndEdit, (int)rc.left, (int)rc.top, (int)(rc.right - rc.left),
				(int)(rc.bottom - rc.top), TRUE);

			GetClientRect(hStatusWindow, &rc);
			InvalidateRect(hStatusWindow, &rc, TRUE);

			goto CallDCP;
			}

        case WM_SETFOCUS:
            SetFocus((HWND) GetWindowLong(hwnd, GWL_HWNDEDIT));
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
				{
                case ID_EDIT:
                    switch(GET_WM_COMMAND_CMD(wParam, lParam))
						{
						case LBN_DBLCLK:
							if(lParam == (long) hwndD4xStatus)
								{
								iSelectedListboxItem = (int) SendMessage(hwndD4xStatus, 
																	LB_GETCURSEL, 
																	(WPARAM) 0, 
																	(LPARAM) 0);
								if(iSelectedListboxItem != LB_ERR)
									{
									PrintMessage(MSGTYPE_SYSTEM,  
													hwndGeneralMsg, 
													"Phone Line Status - DoubleClick");

									iSelectedItem = iSelectedListboxItem -
													GetNumberOfTitleLinesInPhoneLineStatusWindow();

									// ignore double click on header/title items.
									if((iSelectedItem >= 0) && 
										(iSelectedItem < totalD4xChannels))
										{
										PrintMessage(MSGTYPE_SYSTEM,  
														hwndGeneralMsg, 
														"iSelectedItem: [%d]",
														iSelectedItem);

										DialogBox(hInst, (LPSTR) IDD_D4X_DIALOG, hwnd, DlgLineProc);
										}
									}
								}
							else if(lParam == (long) hwndQueStatus)
								{
								// determine the listbox item number that is selected.
								//  translate this into the queue number by subtracting 3
								//  
								iSelectedListboxItem = (int) SendMessage(hwndQueStatus, 
																	LB_GETCURSEL, 
																	(WPARAM) 0, 
																	(LPARAM) 0);

								if(iSelectedListboxItem != LB_ERR)
									{
									PrintMessage(MSGTYPE_SYSTEM,  
													hwndGeneralMsg, 
													"Queue Status - DoubleClick");

									iSelectedItem = iSelectedListboxItem - 
													GetNumberOfTitleLinesInQueueStatusWindow();

									// ignore double click on header/title items.
									if((iSelectedItem >= 0) &&
										(Queue[iSelectedItem].pQFM->GetLastWritten() > 0))
										{
										PrintMessage(MSGTYPE_SYSTEM,  
														hwndGeneralMsg, 
														"iSelectedItem: [%d]",
														iSelectedItem);

										DialogBox(hInst, (LPSTR) IDD_QUEUE, hwnd, QueueStatusProc);
										}
									}
								}
							else if(lParam == (long) hwndComStatus)
								{
								 iSelectedListboxItem = (int) SendMessage(hwndComStatus, 
																	LB_GETCURSEL, 
																	(WPARAM) 0, 
																	(LPARAM) 0);

								if(iSelectedListboxItem != LB_ERR)
									{
									PrintMessage(MSGTYPE_SYSTEM,
													hwndGeneralMsg,
													"Comm Port Status - DoubleClick");

									iSelectedItem = iSelectedListboxItem -
													GetNumberOfTitleLinesInPhoneLineStatusWindow();

									// ignore double click on header/title items.
									if((iSelectedItem >= 0) && 
										(portactive[iSelectedItem] == TRUE))
										{									
										PrintMessage(MSGTYPE_SYSTEM,  
														hwndGeneralMsg, 
														"iSelectedItem: [%d]",
														iSelectedItem);

										DialogBox(hInst, (LPSTR) IDD_DATASCOPE, hwnd, DataScopeDlgProc);
										}
									}
								}
							break;

                        case EN_CHANGE:
                            // If the contents of the edit control have changed,
                            //  set the changed flag
                            //
                            SetWindowWord (hwnd, GWW_CHANGED, TRUE);
							break;

                        case EN_ERRSPACE:
                            // If the control is out of space, honk
                            // MessageBeep (0);
                            break;

                        default:
                            goto CallDCP;
                    }
                    break;
                default:
                    goto CallDCP;
            }
            break;

        default:
CallDCP:
            // Again, since the MDI default behaviour is a little different,
            // call DefMDIChildProc instead of DefWindowProc()
            return DefMDIChildProc (hwnd, msg, wParam, lParam);
		}

    return FALSE;
	}


	BOOL CALLBACK
AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM)
	{
    switch(msg)
		{
        case WM_INITDIALOG:
			// post information to dialog box

 			SendDlgItemMessage(hwnd,
								IDC_APP_VERSION,
								WM_SETTEXT,
								0,
								(LPARAM) szAppVersion);

  			SendDlgItemMessage(hwnd,
								IDC_EXE_NAME,
								WM_SETTEXT,
								0,
								(LPARAM) szApplicationFileName);
			break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
				{
                case IDCANCEL:
					//Beep(400, 200);
                    EndDialog(hwnd, 0);
                    break;

                case IDOK:
                    EndDialog(hwnd, 0);
                    break;

                default:
                    return FALSE;
				}
            break;

        default:
            return FALSE;
		}

    return TRUE;
	}


	const TCHAR	*
portTypeMsg[] =
	{
	"NOT_ACTIVE",
	"Zetron 15B",
	"Zetron 2000",
	"Zetron 16",
	"Pronet IPT",
	"IXO Modem",
	"Leased Line TAP",
	"TAP Input",
	"Page In",
	"IXO Discard",
	"CATS Group Call",
	"TAP Arch UVA",
	"IXO Direct Connect",
	"IXX Direct",
	"TA2 TAP Variation 2",
	"TA3 TAP Variation 3",
	"NOT_DEFINED"
	};


#define MAX_DATASCOPE_SCREEN_LINES 100


	BOOL CALLBACK
DataScopeDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM)
	{
	int i;
	HWND hwndTemp;
	char tempString[100];
	LRESULT dwNumLines;
	TCHAR buf1[100];
	TCHAR buf2[100];

    switch(msg)
		{
        case WM_INITDIALOG:
			hwndDataScopeDlg = hwnd;

			DataScopeChar = 3;
			dataScopeHex = FALSE;
			dataScopeScroll = TRUE;
			hwndDataScopeLB = GetDlgItem(hwnd, IDC_DSLIST);

			for(i = 0; i < MAX_COMMPORTS; i++)
				{
				if(portactive[i] == TRUE)
					{
					EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1 + i), TRUE);
					}
				}

			memset(tempString, 0, sizeof(tempString));
			sprintf(tempString,
					"      Data Scope      COM%i:    -    %s   -   (%s) ",
					iSelectedItem + 1,
					CommPortDescriptionText[pageactive[iSelectedItem]],
					portTypeMsg[pageactive[iSelectedItem]]);

			SetWindowText(hwnd, tempString);
			DataScopeShowState(iSelectedItem, hwnd);

			// number of listbox data items on the display.
			//  arbitrary even number required.
			for(i = 0; i < MAX_DATASCOPE_SCREEN_LINES; i++)
				{
				SendMessage(hwndDataScopeLB,
							LB_ADDSTRING,
							(WPARAM) 0,
							(LPARAM) (LPCTSTR) " ");
				}

			memset(tempString, 0, sizeof(tempString));
			sprintf(tempString, "[Data COM%i:]", iSelectedItem + 1);

			// delete as many as you insert.
			SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
			SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
			SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
			SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);

			SendMessage(hwndDataScopeLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPCTSTR) tempString );
			SendMessage(hwndDataScopeLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPCTSTR) "" );
			SendMessage(hwndDataScopeLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPCTSTR) "TX ");
			SendMessage(hwndDataScopeLB, LB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPCTSTR) "RX ");

			// show the bottom most lines of screen.
			dwNumLines = SendMessage(hwndDataScopeLB, LB_GETCOUNT, (WPARAM) 0, (LPARAM) 0L);
			SendMessage(hwndDataScopeLB, LB_SETCURSEL, (WPARAM) (dwNumLines - 1), (LPARAM) 0L);
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
				{
				case IDC_BUTTON1:
				case IDC_BUTTON2:
				case IDC_BUTTON3:
				case IDC_BUTTON4:
				case IDC_BUTTON5:
				case IDC_BUTTON6:
				case IDC_BUTTON7:
				case IDC_BUTTON8:
				case IDC_BUTTON9:
				case IDC_BUTTON10:
				case IDC_BUTTON11:
				case IDC_BUTTON12:
				case IDC_BUTTON13:
				case IDC_BUTTON14:
				case IDC_BUTTON15:
				case IDC_BUTTON16:
				case IDC_BUTTON17:
				case IDC_BUTTON18:
				case IDC_BUTTON19:
				case IDC_BUTTON20:
				case IDC_BUTTON21:
				case IDC_BUTTON22:
				case IDC_BUTTON23:
				case IDC_BUTTON24:
				case IDC_BUTTON25:
				case IDC_BUTTON26:
				case IDC_BUTTON27:
				case IDC_BUTTON28:
				case IDC_BUTTON29:
				case IDC_BUTTON30:
				case IDC_BUTTON31:
				case IDC_BUTTON32:
				case IDC_BUTTON33:
				case IDC_BUTTON34:
					iSelectedItem = LOWORD (wParam) - IDC_BUTTON1;

					memset(tempString, 0, sizeof(tempString));
					sprintf(tempString,
							"      Data Scope  COM%i:  %s",
							iSelectedItem + 1,
							portTypeMsg[pageactive[iSelectedItem]]);

					SetWindowText(hwnd, tempString);
					DataScopeShowState(iSelectedItem, hwnd);

					memset(buf1, 0, sizeof(buf1));
					memset(buf2, 0, sizeof(buf2));

					dwNumLines = SendMessage(hwndDataScopeLB, LB_GETCOUNT, (WPARAM) 0, (LPARAM) 0L);

					SendMessage(hwndDataScopeLB, LB_GETTEXT, (WPARAM) dwNumLines - 2, (LPARAM) buf1);
					SendMessage(hwndDataScopeLB, LB_GETTEXT, (WPARAM) dwNumLines - 1, (LPARAM) buf2);

					if(dataScopeFh != -1)
						{
						Log_Message(dataScopeFh, &dataScopeMesgByte, buf1);
						Log_Message(dataScopeFh, &dataScopeMesgByte, buf2);
						}

					SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
					SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
					SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
					SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);

					memset(buf1, 0, sizeof(buf1));
					sprintf(buf1, "[Data COM%i:]", iSelectedItem + 1);

					SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) -1, (LPARAM) buf1);
					SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) -1, (LPARAM) (LPSTR) "");
					SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) -1, (LPARAM) (LPSTR) "TX ");
					SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) -1, (LPARAM) (LPSTR) "RX ");

					// show the bottom most lines of screen.
					SendMessage(hwndDataScopeLB, LB_SETCURSEL, (WPARAM) (dwNumLines - 1), (LPARAM) 0L);

					DataScopeChar = 3;

					if(dataScopeFh != -1)
						{
						Log_Message(dataScopeFh, &dataScopeMesgByte, buf1);
						Log_Message(dataScopeFh, &dataScopeMesgByte, "");
						}
					break;

                case IDCANCEL:
					hwndDataScopeLB = NULL;
					hwndDataScopeDlg = NULL;

					if( dataScopeFh > 0 )
						_close( dataScopeFh );

					dataScopeFh = -1;
                    EndDialog(hwnd, 0);
                    break;

				case IDC_HEX:
					hwndTemp = GetDlgItem(hwnd, IDC_HEX);
					if( dataScopeHex == TRUE )
						{
						SetWindowText(hwndTemp, "Hex Mode");
						dataScopeHex = FALSE;
						}
					else
						{
						SetWindowText(hwndTemp, "Ascii Mode");
						dataScopeHex = TRUE;
						}
					break;

				case IDC_LOGFILE:
					hwndTemp = GetDlgItem(hwnd, IDC_LOGFILE);

					if(dataScopeFh == -1)
						{
						dataScopeFh = CreateMsgFile(&dataScopeMesgByte, "DAT", "XTPG1");

						if(dataScopeFh != -1)
							SetWindowText(hwndTemp, "Close Log");
						}
					else
						{
						if(dataScopeFh > 0)
							{
							_close(dataScopeFh);
							}

						dataScopeFh = -1;
						SetWindowText(hwndTemp, "Open Log");
						}
					break;

				case IDC_NOTES:
					memset(tempString, 0, sizeof(tempString));

					sprintf(tempString,
							"Notes COM%d: %s",
							(iSelectedItem + 1),
							param.ComPort[iSelectedItem].UserDefinedShortDesignation);

					::MessageBox(GetFocus(),
									param.ComPort[iSelectedItem].UserDefinedNotes,
									tempString,
									(UINT) MB_OK);
					break;

				case IDC_SCROLL:
					hwndTemp = GetDlgItem ( hwnd, IDC_SCROLL );

					if( dataScopeScroll == TRUE )
						{
						SetWindowText( hwndTemp, "Start Scroll");
						dataScopeScroll = FALSE;
						}
					else
						{
						SetWindowText( hwndTemp, "Stop Scroll");
						dataScopeScroll = TRUE;
						}
					break;

				case IDC_BUTTON111:
					DataScopeShowState(iSelectedItem, hwnd);
					break;

				default:
                    return FALSE;
            }
            break;

        default:
            return FALSE;
		}

    return TRUE;
	}


	const TCHAR	*
portStatMsg[] =
	{
	"Not Ready",
	"Ready",
	"Paging",
	"Voice",
	"Com",
	"Abort",
	"Waiting",
	"Talk Timeout",
	"Message"
	};


	void
DataScopeShowState(int lport, HWND hwnd)
	{
	HWND hwndText;
	TCHAR buffer[40];

	hwndText = GetDlgItem( hwnd, IDC_RESET );

	if( hwndText != NULL )
		{
		sprintf(buffer, "%d", ComPort[lport].reset);
		SendMessage( hwndText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LPCTSTR)buffer );
		}

	hwndText = GetDlgItem( hwnd, IDC_ERRORS );

	if( hwndText != NULL )
		{
		sprintf( buffer, "%d", ComPort[ lport ].Errors );
		SendMessage( hwndText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LPCTSTR)buffer );
		}

	hwndText = GetDlgItem( hwnd, IDC_CANCEL );

	if( hwndText != NULL )
		{
		sprintf( buffer, "%d", ComPort[ lport ].Cancels );
		SendMessage( hwndText, WM_SETTEXT, (WPARAM)0, (LPARAM)(LPCTSTR)buffer );
		}

	hwndText = GetDlgItem( hwnd, IDC_STATUS );

	if( hwndText != NULL )
		{
		// RPNOTREADY == 0 thru RPMESSAGE == 8.

		if( ComPort[ lport ].state >= 0 && ComPort[ lport ].state <= 8)
			sprintf( buffer, "%s", portStatMsg[ ComPort[ lport ].state ] );
		else
			sprintf( buffer, "Unknown" );

		SendMessage(hwndText, WM_SETTEXT, (WPARAM) 0, (LPARAM) (LPCTSTR) buffer);
		}
	}


	void
UpdateMenuCheckmark(int Command, int BitMask, UINT MenuItem)
	{
	HMENU hMenu = GetMenu(hwndFrame);

	switch(Command)
		{
		case CHECKMARK_RESET:
			// bitwise "and" it low.
			MessageEnableMask &= ~BitMask;
			break;

		case CHECKMARK_SET:
			// bitwise "or" it high.
			MessageEnableMask |= BitMask;
			break;

		case CHECKMARK_TOGGLE:
			// toggle the bit.
			if(MessageEnableMask & BitMask)
				{
				// bit is on, turn it off.
				MessageEnableMask &= ~BitMask;
				}
			else
				{
				// bit is off, turn it on.
				MessageEnableMask |= BitMask;
				}
			break;

		case CHECKMARK_UPDATE:
		default:
			// do nothing.
			break;
		}

	// enable or disable the checkmark based on the bit.
	if(MessageEnableMask & BitMask)
		{
		CheckMenuItem(hMenu, MenuItem, (UINT) MF_CHECKED);
		}
	else
		{
		CheckMenuItem(hMenu, MenuItem, (UINT) MF_UNCHECKED);
		}
	}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : Initializemenu ( hMenu )                                   *
 *                                                                          *
 *  PURPOSE    : Sets up greying, enabling and checking of main menu items  *
 *               based on the app's state.                                  *
 *                                                                          *
 ****************************************************************************/
	VOID
InitializeMenu(HANDLE hmenu)
	{
    WORD status = MF_ENABLED;
    INT j;

    // The following menu items are enabled if there is an active window
    EnableMenuItem ((HMENU) hmenu, IDM_WINDOWTILE, status);
    EnableMenuItem ((HMENU) hmenu, IDM_WINDOWCASCADE, status);
    EnableMenuItem ((HMENU) hmenu, IDM_WINDOWICONS, status);
    EnableMenuItem ((HMENU) hmenu, IDM_D4X_DIAGS, status);

	for(j = 0; j < MAX_COMMPORTS; j++)
		{
		if(portactive[j] == TRUE)
			{
		    EnableMenuItem((HMENU) hmenu, (UINT) IDM_DATASCOPE, (UINT) MF_ENABLED);
			iSelectedItem = j;
			break;
			}
		}

	// update checkmarks for "messages" menu
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_SYSTEM, IDM_MSGTYPE_SYSTEM);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_ERROR,  IDM_MSGTYPE_ERROR);

	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_IVR,						IDM_MSGTYPE_IVR_GENERAL);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_IVR_VOICE_PROMPT_TEXT,	IDM_MSGTYPE_IVR_VOICE_PROMPT_TEXT);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DIALOGIC_4,				IDM_MSGTYPE_DIALOGIC_4);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_IVR_PROGRESS,				IDM_MSGTYPE_DIALOGIC_8);

	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DATABASE_COMMAND, IDM_MSGTYPE_DATABASE_COMMAND);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DATABASE_DATA,    IDM_MSGTYPE_DATABASE_DATA);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DATABASE_DETAIL,  IDM_MSGTYPE_DATABASE_DETAIL);

	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DEBUG,		IDM_MSGTYPE_DEBUG_1000);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DEBUG_2000,	IDM_MSGTYPE_DEBUG_2000);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DEBUG_4000,	IDM_MSGTYPE_DEBUG_4000);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_DEBUG_8000,	IDM_MSGTYPE_DEBUG_8000);

	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_COMM,			IDM_MSGTYPE_COMM_GENERAL);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_COMM_ERROR,	IDM_MSGTYPE_COMM_ERROR);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_COMM_DTR,		IDM_MSGTYPE_COMM_DTR);
	UpdateMenuCheckmark(CHECKMARK_UPDATE, MSGTYPE_COMM_RESET,	IDM_MSGTYPE_COMM_RESET);
	}


	int
XnSetMenuText(int menuId, const LPTSTR strMessage)
	{
	HMENU hMainMenu;
	MENUITEMINFO lpmii;

	hMainMenu = GetMenu(hwndFrame);

	lpmii.cbSize = sizeof(MENUITEMINFO);
	lpmii.fMask = MIIM_TYPE;
	lpmii.fType = MFT_STRING;
	lpmii.dwTypeData = strMessage;
	lpmii.cch = strlen(strMessage);

	SetMenuItemInfo(hMainMenu, (UINT) menuId, FALSE, &lpmii);

	return TRUE;
	}


	void
SetStatusText(int position, char * message)
	{
	char string2[100];

	// set the text of a status bar
	SendMessage(hStatusWindow, SB_GETTEXT, (WPARAM) position, (LPARAM) (LPCTSTR) string2);

	// update the message only if the text has changed
	//  since the last posting.
	if(lstrcmpi(message, string2) != 0)
		{
		SendMessage(hStatusWindow, SB_SETTEXT, (WPARAM) position, (LPARAM) (LPCTSTR) message);
		}
	}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : CommandHandler ()                                          *
 *                                                                          *
 *  PURPOSE    : Processes all "frame" WM_COMMAND messages.                 *
 *                                                                          *
 ****************************************************************************/
	VOID
CommandHandler(HWND hwnd, WPARAM wParam, LPARAM lParam)
	{
    switch (LOWORD(wParam))
		{
		case LBN_DBLCLK:
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "List Box Click");
			break;

        case IDM_HELPABOUT:
            DialogBox(hInst, (LPSTR) IDD_ABOUT, hwnd, AboutDlgProc);
            break;

 		case IDM_EXIT:
			// indicate normal USER-requested shutdown
			SetExitCode(APP_EXIT_USER_REQUEST_MENU);

			// WM_CLOSE message sent to the Main Frame window,
			//  will eventually stop the application.
			SendMessage(hwndFrame, WM_CLOSE, 0L, 0L);            
			break;

		case IDM_DATASCOPE:
            DialogBox(hInst, (LPSTR) IDD_DATASCOPE, hwnd, DataScopeDlgProc);
			break;

		case IDM_PAGE:
            DialogBox(hInst, (LPSTR) IDD_PAGE, hwnd, PageDlgProc);
			break;

        // The following are window commands - these are handled by the MDI Client.
        case IDM_WINDOWTILE:
            // Tile MDI windows
			EnableWindow( (HWND)GetWindowLong( hwndComStatus, GWL_HWNDPARENT), FALSE );
			EnableWindow( (HWND)GetWindowLong( hwndD4xStatus, GWL_HWNDPARENT), FALSE );
			EnableWindow( (HWND)GetWindowLong( hwndQueStatus, GWL_HWNDPARENT), FALSE );

            SendMessage(hwndMDIClient,
						WM_MDITILE,
						(WPARAM) MDITILE_HORIZONTAL | MDITILE_SKIPDISABLED,
						0L);

			EnableWindow( (HWND)GetWindowLong( hwndComStatus, GWL_HWNDPARENT), TRUE );
			EnableWindow( (HWND)GetWindowLong( hwndD4xStatus, GWL_HWNDPARENT), TRUE );
			EnableWindow( (HWND)GetWindowLong( hwndQueStatus, GWL_HWNDPARENT), TRUE );
            break;

        case IDM_WINDOWCASCADE:
            // Cascade MDI windows
			EnableWindow( (HWND)GetWindowLong( hwndComStatus, GWL_HWNDPARENT), FALSE );
			EnableWindow( (HWND)GetWindowLong( hwndD4xStatus, GWL_HWNDPARENT), FALSE );
			EnableWindow( (HWND)GetWindowLong( hwndQueStatus, GWL_HWNDPARENT), FALSE );
            SendMessage( hwndMDIClient, WM_MDICASCADE, (WPARAM)MDITILE_SKIPDISABLED, 0L);
			EnableWindow( (HWND)GetWindowLong( hwndComStatus, GWL_HWNDPARENT), TRUE );
			EnableWindow( (HWND)GetWindowLong( hwndD4xStatus, GWL_HWNDPARENT), TRUE );
			EnableWindow( (HWND)GetWindowLong( hwndQueStatus, GWL_HWNDPARENT), TRUE );
            break;

        case IDM_WINDOWICONS:
            // Auto - arrange MDI icons
            SendMessage (hwndMDIClient, WM_MDIICONARRANGE, 0, 0L);
            break;

		case IDM_D4X_DIAGS:
			{
			HMENU hMenu = GetMenu(hwndFrame);
			if(d4xDiags == FALSE)
				{
				d4xDiags = TRUE;
				CheckMenuItem(hMenu, (UINT)IDM_D4X_DIAGS, (UINT)MF_CHECKED);
				}
			else
				{
				d4xDiags = FALSE;
				CheckMenuItem(hMenu, (UINT)IDM_D4X_DIAGS, (UINT)MF_UNCHECKED);
				}
			}
			break;

		case IDM_EXCEPTION_ACCESS:
			// for testing, signal a request for an intentional memory pointer
			//  access exception.
			bActivateIntentionalAccessException = TRUE;
			break;

		case IDM_EXCEPTION_ZERO_DIVIDE:
			// for testing, signal a request for an intentional divide by zero
			//  access exception.
			bActivateIntentionalZeroDivideException = TRUE;
			break;

		case IDM_SHOW_COMMAND_LINE_HELP:
			// show the command line help screen that is presented
			//  when command line XNPGSRV.EXE /H or /? is entered.
			ShowHelpScreen();
			break;

		case IDM_SET_STANDARD_MESSAGES:
			MessageEnableMask = MESSAGE_GROUP_STANDARD;
			break;

		case IDM_SET_DEBUG_MESSAGES:
			MessageEnableMask = MESSAGE_GROUP_DEBUG;
			break;

		case IDM_SET_ALL_MESSAGES:
			MessageEnableMask = MESSAGE_GROUP_ALL;

			// turn on joseph's "Dialogic Messages" also.
			d4xDiags = TRUE;
			break;

		case IDM_SET_NO_MESSAGES:
			MessageEnableMask = MESSAGE_GROUP_NONE;
			break;

		case IDM_MSGTYPE_SYSTEM:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_SYSTEM, IDM_MSGTYPE_SYSTEM);
			break;

		case IDM_MSGTYPE_ERROR:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_ERROR, IDM_MSGTYPE_ERROR);
			break;

		case IDM_MSGTYPE_IVR_GENERAL:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_IVR, IDM_MSGTYPE_IVR_GENERAL);
			break;

		case IDM_MSGTYPE_IVR_VOICE_PROMPT_TEXT:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_IVR_VOICE_PROMPT_TEXT, IDM_MSGTYPE_IVR_VOICE_PROMPT_TEXT);
			break;

		case IDM_MSGTYPE_DIALOGIC_4:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DIALOGIC_4, IDM_MSGTYPE_DIALOGIC_4);
			break;

		case IDM_MSGTYPE_DIALOGIC_8:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_IVR_PROGRESS, IDM_MSGTYPE_DIALOGIC_8);
			break;

		case IDM_MSGTYPE_DATABASE_COMMAND:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DATABASE_COMMAND, IDM_MSGTYPE_DATABASE_COMMAND);
			break;

		case IDM_MSGTYPE_DATABASE_DATA:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DATABASE_DATA, IDM_MSGTYPE_DATABASE_DATA);
			break;

		case IDM_MSGTYPE_DATABASE_DETAIL:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DATABASE_DETAIL, IDM_MSGTYPE_DATABASE_DETAIL);
			break;

		case IDM_MSGTYPE_DEBUG_1000:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DEBUG, IDM_MSGTYPE_DEBUG_1000);
			break;

		case IDM_MSGTYPE_DEBUG_2000:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DEBUG_2000, IDM_MSGTYPE_DEBUG_2000);
			break;

		case IDM_MSGTYPE_DEBUG_4000:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DEBUG_4000, IDM_MSGTYPE_DEBUG_4000);
			break;

		case IDM_MSGTYPE_DEBUG_8000:
			UpdateMenuCheckmark(CHECKMARK_TOGGLE, MSGTYPE_DEBUG_8000, IDM_MSGTYPE_DEBUG_8000);
			break;

		case IDM_SEND_ALPHA_PAGE1:
			TestSendAlphaPage1();
			break;

		case IDM_SEND_ALPHA_PAGE2:
			TestSendAlphaPage2();
			break;

		case IDM_SEND_SCHEDULED_ALPHA_PAGE:
			TestSendScheduledAlphaPage();
			break;

		case IDM_SEND_SCHEDULED_VOICE_PAGE:
			TestSendScheduledVoicePage();
			break;

		case IDM_SEND_OVERHEAD_PAGE:
			TestSendOverheadPage();
			break;

		case IDM_SEND_SCHEDULED_OVERHEAD_PAGE:
			TestSendScheduledOverheadPage();
			break;

		case IDM_SEND_NUMERIC_PAGE:
			TestSendNumericPage();
			break;

		case IDM_SEND_VOICE_PAGE_1:
			TestSendVoicePage1();
			break;

		case IDM_SEND_VOICE_PAGE_2:
			TestSendVoicePage2();
			break;

		case IDM_SEND_TALK_PAGE_1:
			TestSendTalkPage1();
			break;

		case IDM_SEND_TALK_PAGE_2:
			TestSendTalkPage2();
			break;

		case IDM_SEND_TALK_PAGE_3:
			TestSendTalkPage3();
			break;

		case IDM_SEND_LIST_PAGE1:
			TestSendListPage1();
			break;

		case IDM_SEND_LIST_PAGE2:
			TestSendListPage2();
			break;

		case IDM_SEND_LIST_PAGE3:
			TestSendListPage3();
			break;

		case IDM_SEND_LIST_PAGE4:
			TestSendListPage4();
			break;

		case IDM_SEND_ONCALL_PAGE:
			TestSendOnCallPage();
			break;

 		case IDM_SEND_3_ALPHA_PAGES:
			TestSend3AlphaPages();
			break;

 		case IDM_SEND_BAD_PAGE:
			TestSendBadPage();
			break;

		case IDM_MISC_SCHEDULER_70:
			// action reminder for "commport failure" (ps1 == 70, ps2 == 75).
			{
			CString csText;
			csText.Format("%-2.2s", param.ActionIdForCommportFailure);

			SetActionReminder(param.ActionReminderProfileId, 
								(LPTSTR) (LPCTSTR) param.ActionIdForCommportFailure, 
								0, 
								REMINDER_ADD, 
								0, 
								csText);
			}
			break;

		case IDM_MISC_SCHEDULER_71:
			// action reminder for "queue warning" (ps1 == 71, ps2 == 76).
			{
			CString csText;
			csText.Format("%-2.2s", param.ActionIdForQueueWarning);

			SetActionReminder(param.ActionReminderProfileId, 
								(LPTSTR) (LPCTSTR) param.ActionIdForQueueWarning, 
								0, 
								REMINDER_ADD, 
								0, 
								csText);
			}
			break;

		case IDM_MISC_SCHEDULER_72:
			// action reminder for "cancelled page" (ps1 == 72, ps2 == 77).
			{
			CString csText;
			csText.Format("%-2.2s", param.ActionIdForCancelledPage);

			SetActionReminder(param.ActionReminderProfileId, 
								(LPTSTR) (LPCTSTR) param.ActionIdForCancelledPage, 
								0, 
								REMINDER_ADD, 
								0, 
								csText);
			}
			break;

		case IDM_MISC_SCHEDULER_73:
			// action reminder for "application exit" (ps1 == 21, ps2 == 61).
			{
			CString csText;
			csText.Format("%-2.2s", param.ActionIdForApplicationExit);

			SetActionReminder(param.ActionReminderProfileId, 
								(LPTSTR) (LPCTSTR) param.ActionIdForApplicationExit, 
								0, 
								REMINDER_ADD, 
								0, 
								csText);
			}
			break;

 		case IDM_MISC_SCHEDULER_74:
			// action reminder for "queue access" (ps1 == 74, ps2 == 79).
			{
			CString csText;
			csText.Format("%-2.2s", param.ActionIdForQueueAccessError);

			SetActionReminder(param.ActionReminderProfileId, 
								(LPTSTR) (LPCTSTR) param.ActionIdForQueueAccessError, 
								0, 
								REMINDER_ADD, 
								0, 
								csText);
			}
			break;

		case IDM_INFO_1:
			ShowInfoGeneral();
			break;

 		case IDM_INFO_2:
			ShowInfoDatabase();
			break;

 		case IDM_INFO_3:
			ShowInfoCustomer();
			break;

 		case IDM_INFO_4:
			ShowInfoParameters();
			break;

		case IDM_INFO_5:
			ShowImpairedVendors();
			break;

 		case IDM_INFO_6:
			break;

 		case IDM_INFO_7:
			break;

 		case IDM_INFO_8:
			break;

 		case IDM_INFO_9:
			break;

       default:
            // This is essential, since there are frame WM_COMMANDS generated
            // by the MDI system for activating child windows via the
            // window menu.
            //
            DefFrameProc(hwnd, hwndMDIClient, WM_COMMAND, wParam, lParam);
		}
	}


	int
xnAddEntries(int row, 
			 CString csProfileId,
			 CString csPagerId, 
			 CString csDate, 
			 CString csTime, 
			 CString csInfo)
	{
	LV_ITEM item;

	item.iItem = row;

	item.mask     = LVIF_TEXT;
	item.pszText  = (LPTSTR) (LPCTSTR) csProfileId;
	item.iSubItem = 0;
	ListView_InsertItem(hwndControl, &item);

	item.mask     = LVIF_TEXT;
	item.pszText  = (LPTSTR) (LPCTSTR) csPagerId;
	item.iSubItem = 1;
	ListView_SetItem(hwndControl, &item);

	item.mask     = LVIF_TEXT;
	item.pszText  = (LPTSTR) (LPCTSTR) csDate;
	item.iSubItem = 2;
	ListView_SetItem(hwndControl, &item);

	item.mask     = LVIF_TEXT;
	item.pszText  = (LPTSTR) (LPCTSTR) csTime;
	item.iSubItem = 3;
	ListView_SetItem(hwndControl, &item);

	item.mask     = LVIF_TEXT;
	item.pszText  = (LPTSTR) (LPCTSTR) csInfo;
	item.iSubItem = 4;
	ListView_SetItem(hwndControl, &item);

	return 0;
	}


	BOOL CALLBACK
QueueStatusProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM)
	{
	int	wmId;
	LV_COLUMN col;
	CString csQueueName;

	switch(msg)
		{
		case WM_INITDIALOG:
			// title for the window.
			//
			csQueueName.Format("Queue:  [%3.3s]", Queue[iSelectedItem].Id);
			SetWindowText(hDlg, csQueueName);

			// Setup the File List Box
			hwndControl = GetDlgItem(hDlg, IDC_LIST4);

			// use fixed font.
			SendMessage(hwndControl, WM_SETFONT, (WPARAM) GetStockObject(SYSTEM_FIXED_FONT), TRUE);

			col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			col.fmt = LVCFMT_LEFT;
			col.cx = 100;

			col.pszText  = _TEXT("System Id");
			ListView_InsertColumn((HWND) hwndControl, 0, &col);

			col.pszText  = _TEXT("Pager Id");
			ListView_InsertColumn((HWND) hwndControl, 1, &col);

			col.pszText  = _TEXT("Date");
			ListView_InsertColumn((HWND) hwndControl, 2, &col);

			col.pszText  = _TEXT("Time");
			ListView_InsertColumn((HWND) hwndControl, 3, &col);

			col.pszText  = _TEXT("Page Info");
			ListView_InsertColumn((HWND) hwndControl, 4, &col);

			ListView_SetColumnWidth((HWND)hwndControl, (int) 0, 20 + 65);
			ListView_SetColumnWidth((HWND)hwndControl, (int) 1, 20 + 65);
			ListView_SetColumnWidth((HWND)hwndControl, (int) 2, 30 + 70);
			ListView_SetColumnWidth((HWND)hwndControl, (int) 3, 20 + 65);
			ListView_SetColumnWidth((HWND)hwndControl, (int) 4, 400);

			{
			// a temporary instance of QueueBufferClass.
			QueueBufferClass myQBC;

			for(int i = 0; i < Queue[iSelectedItem].pQFM->GetLastWritten(); i++)
				{
 				// use the QueueFileManager object to do the work.
				Queue[iSelectedItem].pQFM->ReadQueueItem(i, &myQBC);

				xnAddEntries(i,
								myQBC.GetProfileId(),
								myQBC.GetPagerId(),
								myQBC.GetInitiatedDate(),
								myQBC.GetInitiatedTime(),
								myQBC.GetPageInfo());
				}
			}
			break;

		case WM_DESTROY:
			break;

		case WM_COMMAND:
			wmId = LOWORD (wParam);

			switch(wmId)
				{
				case IDOK:
					EndDialog(hDlg, 0);
					return (TRUE);

				case IDCANCEL:
					EndDialog(hDlg, 0);
					break;

				case IDC_RESET:
					// truncate the queue.
					if(Queue[iSelectedItem].pQFM->TruncateQueue())
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										"Queue Truncated Ok [%d]", 
										iSelectedItem);
						}
					else
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										"Unable to Truncate Queue [%d]", 
										iSelectedItem);
						}

					// the data in the window is no longer valid, close the window.
					EndDialog(hDlg, 0);
					break;

				default:
					break;
				}
			break;

		default:
			break;
		}

	return FALSE;
	}


/************************************************************
 *        NAME: VolumeDlgProc()
 * DESCRIPTION: Dialog Box procedure to allow user to modify
 *              volume setting for file playback.
 ************************************************************/
	BOOL CALLBACK
DlgLineProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	int				wmId;
	char			buffer[ 200 ] = "";
	static short	curVolPos;			// current volume slider position
	static short	curSpeedPos;		// current speed slider position
	static HWND		hVolSlider;			// handle to volume slider
	static HWND		hSpeedSlider;		// handle to speed slider

	switch(msg)
		{
	case WM_INITDIALOG:
		strcpy( buffer, dlgctrls[iSelectedItem].str );
		strcat( buffer, " Diagnostics" );
		SetWindowText( hDlg, buffer );

		DlgLineInfo( hDlg );
		hVolSlider = GetDlgItem( hDlg,IDC_SLIDER1 );
		hSpeedSlider = GetDlgItem( hDlg,IDC_SLIDER2 );
		SendMessage( hVolSlider, TBM_SETRANGE, TRUE, MAKELONG(-10, 10 ) );
		SendMessage( hSpeedSlider, TBM_SETRANGE, TRUE, MAKELONG(-10, 10 ) );
		SendMessage( hVolSlider, TBM_SETPOS, TRUE, (LPARAM)0 );
		SendMessage( hSpeedSlider, TBM_SETPOS, TRUE, (LPARAM)0 );
		break;

	case WM_DESTROY:
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);

		switch(wmId) 
			{
		case IDOK:
			EndDialog(hDlg, 0);
			return (TRUE);

		case IDC_REFRESH:
			DlgLineInfo( hDlg );
			break;

		case IDC_ONHOOK:
			PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i Manual Set Onhook", iSelectedItem);
			port[iSelectedItem].timer.wHour = 70;
			port[iSelectedItem].state = ST_ONHK;
			XnSetHook(iSelectedItem, DX_ONHOOK);
			DlgLineInfo(hDlg);
			break;

		case IDC_OFFHOOK:
			PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i Manual Set Offhook", iSelectedItem);
			port[iSelectedItem].timer.wHour = 70;
			port[iSelectedItem].state = ST_VP_PAGE;
			XnSetHook(iSelectedItem, DX_OFFHOOK);
			DlgLineInfo(hDlg);
			break;

		case IDC_TESTTONE:
			DialogBox(hInst, (LPSTR)IDD_TGEN_DIALOG, hDlg, TngenDlgProc );
			break;

		default:
			break;
		}
		break;

	case WM_HSCROLL:
		if( (HWND)lParam == hVolSlider )
			{
			switch(LOWORD(wParam))
				{
				case TB_PAGEUP:
				case TB_PAGEDOWN:
					curVolPos = (short)SendMessage( hVolSlider, TBM_GETPOS, 0,(LPARAM)0);
					break;
				case TB_THUMBTRACK:
					curVolPos = HIWORD( wParam );
					break;
				default:
					break;
				}
			dx_adjsv( port[iSelectedItem].d4xHandle, SV_VOLUMETBL, SV_ABSPOS, (unsigned short)curVolPos );
			sprintf( buffer, "%i DB", curVolPos );
			SendMessage( GetDlgItem(hDlg, IDC_VOLUME_LEVEL), WM_SETTEXT, (WPARAM)0,(LPARAM)(LPCTSTR)buffer );
			}
		else if( (HWND)lParam == hSpeedSlider )
			{
			switch(LOWORD(wParam))
				{
				case TB_PAGEUP:
				case TB_PAGEDOWN:
					curSpeedPos = (short)SendMessage( hSpeedSlider, TBM_GETPOS, 0,(LPARAM)0);
					break;
				case TB_THUMBTRACK:
					curSpeedPos = HIWORD( wParam );
					break;
				default:
					break;
				}
			dx_adjsv( port[iSelectedItem].d4xHandle, SV_SPEEDTBL, SV_ABSPOS, (unsigned short)curSpeedPos );
			sprintf( buffer, "%i", curSpeedPos );
			SendMessage( GetDlgItem(hDlg, IDC_SPEED_LEVEL), WM_SETTEXT, (WPARAM)0,(LPARAM)(LPCTSTR)buffer );
			}
		break;

		default:
			break;
		}
	return (FALSE);
	}


	void
DlgLineInfo( HWND hDlg )
	{
	long	li;
	char	buffer[ 200 ] = "";

	memset( buffer, 0, sizeof( buffer ) );
	strcpy( buffer, "Line Status: " );
	li = ATDX_STATE( port[iSelectedItem].d4xHandle );
	switch( li )
		{
		case CS_DIAL: strcat( buffer, "Dial State" ); break;
		case CS_CALL: strcat( buffer, "Call State" ); break;
		case CS_GTDIG: strcat( buffer, "Get Digit State" ); break;
		case CS_HOOK: strcat( buffer, "Hook State" ); break;
		case CS_IDLE: strcat( buffer, "Idle State" ); break;
		case CS_PLAY: strcat( buffer, "Play State" ); break;
		case CS_RECD: strcat( buffer, "Record State" ); break;
		case CS_STOPD: strcat( buffer, "Stopped State" ); break;
		case CS_TONE: strcat( buffer, "Playing Tone State" ); break;
		case CS_WINK: strcat( buffer, "Wink State" ); break;
		case CS_SENDFAX: strcat( buffer, "Send Fax State" ); break;
		case CS_RECVFAX: strcat( buffer, "Recieve Fax State" ); break;
		default: sprintf( buffer, "Unkown State %li", li ); break;
		}
	SendMessage( GetDlgItem(hDlg, IDC_D4X_STATUS), WM_SETTEXT, (WPARAM)0,(LPARAM)(LPCTSTR)buffer );

	memset( buffer, 0, sizeof( buffer ) );
	if( port[iSelectedItem].t1 == FALSE)
		{
		strcpy( buffer, "Hook State: " );

		li = ATDX_HOOKST( port[iSelectedItem].d4xHandle );

		switch( li )
			{
			case DX_OFFHOOK: strcat( buffer, "Off Hook" );
				break;

			case DX_ONHOOK: strcat( buffer, "On Hook" );
				break;

			default: sprintf( buffer, "Unkown Hook State %li", li );
				break;
			}
		}
	else
		{
		long tsBits;
		int	arcv;
		int brcv;
		int axmt;
		int bxmt;

		tsBits = ATDT_TSSGBIT( port[iSelectedItem].d4xT1Handle);

		if(tsBits == AT_FAILURE)
			{
			sprintf(buffer, "T1 Error %s", ATDV_ERRMSGP( (long)port[iSelectedItem].d4xT1Handle));
			}
		else
			{
			arcv = ( tsBits & DTSG_RCVA ) ? 1 : 0;
			brcv = ( tsBits & DTSG_RCVB ) ? 1 : 0;
			axmt = ( tsBits & DTSG_XMTA ) ? 1 : 0;
			bxmt = ( tsBits & DTSG_XMTB ) ? 1 : 0;
			sprintf( buffer, "T1 State:   A Rcv %d,   B Rcv %d,     A Xmt %d,   B Xmt %d", arcv, brcv, axmt, bxmt );
			}
		}
	SendMessage( GetDlgItem(hDlg, IDC_D4X_HOOK_ST), WM_SETTEXT, (WPARAM)0,(LPARAM)(LPCTSTR)buffer );

	sprintf(buffer, "Last Error: %s", ATDV_ERRMSGP( (long)port[iSelectedItem].d4xHandle ) );
	SendMessage( GetDlgItem(hDlg, IDC_D4X_ERROR), WM_SETTEXT, (WPARAM)0,(LPARAM)(LPCTSTR)buffer );

	sprintf(buffer, "Paging Server State: %d", port[iSelectedItem].state );
	SendMessage( GetDlgItem(hDlg, IDC_D4X_STATE), WM_SETTEXT, (WPARAM)0,(LPARAM)(LPCTSTR)buffer );
	}


/************************************************************
 *        NAME: TngenDlgProc()
 * DESCRIPTION: Dialog Box procedure to obtain tone definition
 *              for tone generation.
 ************************************************************/
	BOOL CALLBACK
TngenDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM)
	{
	int	wmId;
	HWND FocusWindow;
	static TN_GEN tngen;        // tone generation template
	static TN_GEN tngenDflt = {TN_SINGLE, 440, 350, -10, -10, 500}; // tone generation template
	static DV_TPT tptplay[] =	{
									{IO_CONT, DX_MAXDTMF, 1, TF_MAXDTMF},
									{IO_EOT, DX_LCOFF, 1, TF_LCOFF}
								};
	switch(msg)
		{
	case WM_INITDIALOG:
		// We need to initialize stuff in the dialog box...
		SetDlgItemInt( hDlg, IDC_EDIT1, tngenDflt.tg_freq1, TRUE);
		SetDlgItemInt( hDlg, IDC_EDIT2, tngenDflt.tg_freq2, TRUE);
		SetDlgItemInt( hDlg, IDC_EDIT3, (UINT)tngenDflt.tg_ampl1, TRUE);
		SetDlgItemInt( hDlg, IDC_EDIT4, (UINT)tngenDflt.tg_ampl2, TRUE);
		SetDlgItemInt( hDlg, IDC_EDIT5, (UINT)tngenDflt.tg_dur, TRUE);
		return (TRUE);

	case WM_DESTROY:
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		switch(wmId) {
		case ID_MAKE_TONE:
			tngen.tg_freq1 = (unsigned short)GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			tngen.tg_freq2 = (unsigned short)GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
			tngen.tg_ampl1 = (short)GetDlgItemInt(hDlg, IDC_EDIT3, NULL, TRUE);
			tngen.tg_ampl2 = (short)GetDlgItemInt(hDlg, IDC_EDIT4, NULL, TRUE);
			tngen.tg_dur   = (short) GetDlgItemInt(hDlg, IDC_EDIT5, NULL, TRUE);
			if( tngen.tg_freq2 == 0 )
				tngen.tg_dflag = TN_SINGLE;
			else
				tngen.tg_dflag = TN_DUAL;
			dx_clrdigbuf(port[iSelectedItem].d4xHandle);
			dx_playtone(port[iSelectedItem].d4xHandle, &tngen, tptplay, EV_ASYNC);
			break;

		case IDCANCEL:
			EndDialog( hDlg, 0 );
			return( TRUE );

		case IDC_DEFAULT:
			SetDlgItemInt(hDlg, IDC_EDIT1, tngenDflt.tg_freq1,TRUE);
			SetDlgItemInt(hDlg, IDC_EDIT2, tngenDflt.tg_freq2,TRUE);
			SetDlgItemInt(hDlg, IDC_EDIT3, (UINT)tngenDflt.tg_ampl1,TRUE);
			SetDlgItemInt(hDlg, IDC_EDIT4, (UINT)tngenDflt.tg_ampl2,TRUE);
			SetDlgItemInt(hDlg, IDC_EDIT5, (UINT)tngenDflt.tg_dur,TRUE);
			FocusWindow = GetDlgItem(hDlg, IDC_EDIT1);
			SetFocus(FocusWindow);
			break;
		default:
			break;
      }
      break;
	default:
		break;
	}

	return (FALSE);
	}


	BOOL CALLBACK
PageDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM)
	{
	int i;
	int wmId;
	static int xnEditMode = 0;  //	0 = edit mode, 1 = add new mode;
	DWORD dwIndex;
	char buffer[200] = "";

	switch(msg)
		{
		case WM_INITDIALOG:
			// fill the combo box with the text description
			//  for each of the 30 possible statuses 0 thru 29 inclusive.
			for(i = 0; i < 30; i++)
				{
				SendMessage(GetDlgItem((HWND)hDlg, IDC_COMBO1),
							CB_ADDSTRING,
							0,
							(LPARAM) (LPCTSTR) Status[i].Text);
				}

			SetDlgItemText(hDlg, ID_EDIT, "Add New");
			return (TRUE);

		case WM_DESTROY:
			break;

		case WM_COMMAND:
			wmId = LOWORD(wParam);
			switch(wmId)
				{
				case IDC_EDIT1:
					wmId = HIWORD(wParam);

					if(wmId == EN_KILLFOCUS && xnEditMode == 0)
						{
						EnableWindow( GetDlgItem(hDlg, IDOK), FALSE);
						SetDlgItemText( hDlg, ID_EDIT, "Add New" );

						SetDlgItemText( hDlg, IDC_STATUS, "" );
						SetDlgItemText( hDlg, IDC_EDIT4, "" );
						SetDlgItemText( hDlg, IDC_EDIT5, "" );
						SetDlgItemText( hDlg, IDC_EDIT6, "" );
						SetDlgItemText( hDlg, IDC_EDIT7, "" );
						SetDlgItemText( hDlg, IDC_EDIT8, "" );
						memset(page[ PAGE_MANUAL_POS ].Extension,
								0,
								sizeof( page[ PAGE_MANUAL_POS ].Extension ) );
						memset( page[ PAGE_MANUAL_POS ].PagerId,
								0,
								sizeof( page[ PAGE_MANUAL_POS ].PagerId ) );

						page[ PAGE_MANUAL_POS ].VoiceFile = FALSE; //Indicates not a voice page

						memset( buffer, 0, sizeof( buffer ) );
						GetDlgItemText( hDlg, IDC_EDIT1, buffer, 11 );

						// stop copying when space or null is encountered.
						for(i = 0; i < 10; i++)
							{
							if( buffer[ i ] == ' ' || buffer[ i ] == 0 )
								{
								break;
								}
							page[ PAGE_MANUAL_POS ].Extension[ i ] = buffer[i];
							}

						strncpy(page[PAGE_MANUAL_POS].PagerId,
								page[ PAGE_MANUAL_POS].Extension, 10);

						// quit if no information was entered.
						if(page[PAGE_MANUAL_POS].Extension[0] == 0)
							{
							break;
							}

						//if(true == true)
						////if(PageId)
						//	{

						i = GetPagerId(page[PAGE_MANUAL_POS].Extension,
										PAGE_MANUAL_POS,
										BY_PAGERID);

						if(i == FALSE)
							{
							PrintMessage(MSGTYPE_SYSTEM,
											hwndD4xMsg,
											"1-Pager id %s not found",
											page[PAGE_MANUAL_POS].PagerId);
							break;
							}
						SetDlgItemText( hDlg, ID_EDIT, "Edit" );

					//		}

						SetDlgItemText( hDlg, IDC_EDIT4, page[ PAGE_MANUAL_POS ].Extension );

						if(LogPager( page[ PAGE_MANUAL_POS ].Extension, PAGE_MANUAL_POS ) == FALSE )
							{
							PrintMessage(MSGTYPE_SYSTEM,
											hwndGeneralMsg,
											"Pager not assigned in the log file.");
							}
						else
							{
							SetDlgItemText( hDlg, IDC_EDIT7, page[ PAGE_MANUAL_POS ].tempStr );

							i = page[ PAGE_MANUAL_POS ].ACKMin;
							// check that .ACKMin -- "page status" is not -1
							if(i != -1)
								{
								dwIndex = (unsigned long)
											SendDlgItemMessage(hDlg,
																IDC_COMBO1,
																CB_FINDSTRING,
																0,
																(LPARAM) (LPCSTR) Status[i].Text);

								SendMessage( GetDlgItem ( hDlg, IDC_COMBO1),
											(UINT)CB_SETCURSEL, (WPARAM)dwIndex,
											(LPARAM) 0L );
								if(Status[ i ].Page[ 0 ] == 'F' ||
									Status[ i ].Page[ 0 ] == '#' ||
									Status[ i ].Page[ 0 ] == '0' ||
									Status[ i ].Page[ 0 ] == 'V' ||
									Status[ i ].Page[ 0 ] == 'U' )
									{
									strcpy( buffer, " #:" );
									strcat( buffer, page[ PAGE_MANUAL_POS ].PageInfo );
									SetDlgItemText( hDlg, IDC_STATUS, buffer );
									}
								}

							if( page[ PAGE_MANUAL_POS ].FileName[ 0 ] != 0 )
								{
								strcpy( buffer, page[ PAGE_MANUAL_POS ].FileName );
								SetDlgItemText( hDlg, IDC_STATUS, buffer );
								}

							memset( buffer, 0, sizeof( buffer ) );
							sprintf( buffer, "%%%s", page[ PAGE_MANUAL_POS ].PageType );
							SetDlgItemText( hDlg, IDC_EDIT5, buffer );

							if( Get_Frq( PAGE_MANUAL_POS, buffer, buffer ) == FALSE )
								{
								PrintMessage(MSGTYPE_SYSTEM,
												hwndGeneralMsg,
												"No pager currently assigned. Check frq.");
								break;
								}
							}

						strncpy(buffer, page[PAGE_MANUAL_POS].PagerNumber, sizeof(buffer));

						if( page[ PAGE_MANUAL_POS ].AutCode[ 0 ] != 0 )
							{
							strcat( buffer, "+" );
							strcat( buffer, page[ PAGE_MANUAL_POS ].AutCode );
							}

						SetDlgItemText( hDlg, IDC_EDIT6, buffer );

						if( page[ PAGE_MANUAL_POS ].ComNumber != -1 )
							{
							memset(buffer, 0, sizeof(buffer));
							strcpy(buffer,
									param.ComPort[page[PAGE_MANUAL_POS].ComNumber].description[page[PAGE_MANUAL_POS].ComOffset]);
							SetDlgItemText(hDlg, IDC_EDIT8, buffer);
							}
						else
							{
							if( page[ PAGE_MANUAL_POS ].ScriptNumber != -1 )
								{
								memset( buffer, 0, sizeof( buffer ) );
								strcpy( buffer, param.PageType[ page[ PAGE_MANUAL_POS ].ScriptNumber ].description );
								SetDlgItemText( hDlg, IDC_EDIT8, buffer );
								}
							}

						if((page[ PAGE_MANUAL_POS ].ComNumber != -1 &&
							param.ComPort[ page[ PAGE_MANUAL_POS ].ComNumber ].QueType[page[PAGE_MANUAL_POS].ComOffset][1] == '1') ||
							page[ PAGE_MANUAL_POS ].PageStatus[ 0 ] == 'L' ) // =='L' means list page
							{
							ShowWindow( GetDlgItem(hDlg, IDC_EDIT2 ), SW_SHOW );
							ShowWindow( GetDlgItem(hDlg, IDC_EDIT3 ), SW_HIDE );
							SetFocus( GetDlgItem(hDlg, IDC_EDIT2 ) );
							}
						else
							{
							ShowWindow( GetDlgItem(hDlg, IDC_EDIT3 ), SW_SHOW );
							ShowWindow( GetDlgItem(hDlg, IDC_EDIT2 ), SW_HIDE );
							SetFocus( GetDlgItem(hDlg, IDC_EDIT3 ) );
							}
						EnableWindow( GetDlgItem(hDlg, IDOK), TRUE );
						}
					break;

			case IDC_EDIT5: // page type edit box
				wmId = HIWORD(wParam);
				if( wmId == EN_KILLFOCUS )
					{
					SetDlgItemText( hDlg, IDC_EDIT6, "" );
					SetDlgItemText( hDlg, IDC_EDIT8, "" );
					memset( buffer, 0, sizeof( buffer ) );
					GetDlgItemText( hDlg, IDC_EDIT5, buffer, 4 );

					if( GetPageType( PAGE_MANUAL_POS, buffer, __FILE__, __LINE__ ) )
						{
						if( Get_Frq( PAGE_MANUAL_POS, buffer, buffer ) == FALSE )
							{
							PrintMessage(MSGTYPE_SYSTEM,
											hwndGeneralMsg,
											"No pager currently assigned. Check frq." );
							}
						else
							SetDlgItemText( hDlg, IDC_EDIT6, page[ PAGE_MANUAL_POS ].tempStr2 );

						if( page[ PAGE_MANUAL_POS ].ComNumber != -1 )
							{
							memset( buffer, 0, sizeof( buffer ) );
							strcpy(buffer,
									param.ComPort[page[PAGE_MANUAL_POS].ComNumber].description[page[PAGE_MANUAL_POS].ComOffset]);
							SetDlgItemText(hDlg, IDC_EDIT8, buffer);
							}
						else{
							if( page[ PAGE_MANUAL_POS ].ScriptNumber != -1 )
								{
								memset( buffer, 0, sizeof( buffer ) );
								strcpy(buffer,
										param.PageType[page[PAGE_MANUAL_POS].ScriptNumber].description);
								SetDlgItemText( hDlg, IDC_EDIT8, buffer );
								}
							}
						}
					}
				break;

		case IDC_COMBO1: // page status drop-down box
			wmId = HIWORD(wParam);
			if( wmId == LBN_SELCHANGE )
				{
				i = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO1),
									CB_GETCURSEL,
									(WPARAM)0,
									(LPARAM)0);

				if(i >= 0 && i < 30)
					{
					page[ PAGE_MANUAL_POS ].ACKMin = i;

					if( Status[ i ].Page[ 0 ] == 'F' ||
						Status[ i ].Page[ 0 ] == '#' ||
						Status[ i ].Page[ 0 ] == '0' ||
						Status[ i ].Page[ 0 ] == 'V' ||
						Status[ i ].Page[ 0 ] == 'U' ||
						Status[ i ].Page[ 0 ] == 'C' )
						{
						EnableWindow( GetDlgItem(hDlg, IDC_STATUS ), TRUE );
						SetFocus( GetDlgItem(hDlg, IDC_STATUS ) );
						break;
						}
					memset( page[ PAGE_MANUAL_POS ].FileName, 0, sizeof( page[ PAGE_MANUAL_POS ].FileName ) );
					SetDlgItemText( hDlg, IDC_STATUS, page[ PAGE_MANUAL_POS ].FileName );
					EnableWindow( GetDlgItem(hDlg, IDC_STATUS ), FALSE );
					if( xnEditMode == 0 )
						{
						Change_Log2( PAGE_MANUAL_POS );
						}
					}
				}
			break;

		case IDC_STATUS:  // covered by edit box
			wmId = HIWORD(wParam);
			if( wmId == EN_KILLFOCUS )
				{
				i = page[ PAGE_MANUAL_POS ].ACKMin;
				if(Status[ i ].Page[ 0 ] == 'F' ||
					Status[ i ].Page[ 0 ] == '#' ||
					Status[ i ].Page[ 0 ] == '0' ||
					Status[ i ].Page[ 0 ] == 'V' ||
					Status[ i ].Page[ 0 ] == 'U' )
					{
					GetDlgItemText(hDlg,
									IDC_STATUS,
									page[ PAGE_MANUAL_POS ].PageInfo,
									sizeof( page[ PAGE_MANUAL_POS ].PageInfo ) - 1 );
					if( xnEditMode == 0 )
						{
						Change_Log2( PAGE_MANUAL_POS );
						}
					EnableWindow( GetDlgItem(hDlg, IDC_STATUS ), FALSE );
					}

				if(Status[i].Page[0] == 'C')
					{
					memset(page[PAGE_MANUAL_POS].AutCode, 0, sizeof(page[PAGE_MANUAL_POS].AutCode));
					GetDlgItemText(hDlg, IDC_STATUS, page[PAGE_MANUAL_POS].AutCode, 10);

					Set_Null(page[PAGE_MANUAL_POS].AutCode, sizeof(page[PAGE_MANUAL_POS].AutCode));

					if(page[PAGE_MANUAL_POS].AutCode[0] == 0)
						{
						PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, "Must enter covering id number");
						break;
						}
					memset( page[ PAGE_MANUAL_POS ].InitiatorId, 0, sizeof(page[PAGE_MANUAL_POS].InitiatorId));

					//if(true == true)
					////if(PageId)
					//	{

					i = GetPagerId(page[PAGE_MANUAL_POS].AutCode, PAGE_COV_POS, BY_PAGERID);
					if(i == TRUE)
						{
						memset(page[PAGE_MANUAL_POS].AutCode, 0, sizeof(page[PAGE_MANUAL_POS].AutCode));
						strcpy(page[PAGE_MANUAL_POS].AutCode, page[PAGE_COV_POS].Extension);
						}

					//	else
					//		{
					//		break;
					//		}
					//	}

					i = CoverLog( page[ PAGE_MANUAL_POS ].AutCode, PAGE_MANUAL_POS );
					page[ PAGE_MANUAL_POS ].ScriptNumber = page[ PAGE_COV_POS ].ACKMin;
					strncpy( page[ PAGE_MANUAL_POS ].PageInfo, page[ PAGE_COV_POS ].PageInfo, 25 );
					strncpy( page[ PAGE_MANUAL_POS ].InitiatorId, page[ PAGE_COV_POS ].InitiatorId, 22 );

					switch(i)
						{
						case 0:
							if( strncmp( page[ PAGE_MANUAL_POS ].Extension, page[ PAGE_MANUAL_POS ].AutCode, strlen( page[ PAGE_MANUAL_POS ].Extension ) ) == 0 )
								{
								break;
								}

							if(page[ PAGE_COV_POS ].ACKMin != -1)
								{
								if( Status[ page[ PAGE_COV_POS ].ACKMin ].Page[ 0 ] == 'Y' )
									{
									strcpy( page[ PAGE_MANUAL_POS ].FileName, page[ PAGE_MANUAL_POS ].AutCode );
									if( xnEditMode == 0 )
										{
										Change_Log2( PAGE_MANUAL_POS );
										}
									SetDlgItemText( hDlg, IDC_STATUS, page[ PAGE_MANUAL_POS ].FileName );
									EnableWindow( GetDlgItem(hDlg, IDC_STATUS ), FALSE );
									}
								}
							break;

						case -2:
							i = CoverDoubleCheck( PAGE_MANUAL_POS );
							if(i == TRUE)
								{
								if( Status[ page[ PAGE_COV_POS ].ACKMin ].Page[ 0 ] == 'Y' ||
									Status[ page[ PAGE_COV_POS ].ACKMin ].Page[ 0 ] == 'C' )
									{
									strcpy( page[ PAGE_MANUAL_POS ].FileName, page[ PAGE_MANUAL_POS ].AutCode );
									if( xnEditMode == 0 )
										{
										Change_Log2( PAGE_MANUAL_POS );
										}
									SetDlgItemText( hDlg, IDC_STATUS, page[ PAGE_MANUAL_POS ].FileName );
									EnableWindow( GetDlgItem(hDlg, IDC_STATUS ), FALSE );
									}
								}
							break;

						default:
							break;
					}
					break;
				}
			}
			break;

		case ID_EDIT:
			memset( buffer, 0, sizeof( buffer ) );
			GetDlgItemText( hDlg, ID_EDIT, buffer, sizeof( buffer ) - 1 );
			Set_Null(buffer, sizeof(buffer));

			if( strcmp( buffer, "Edit" ) == 0 )
				{
				SetDlgItemText( hDlg, ID_EDIT, "Save" );
				EnableWindow( GetDlgItem(hDlg, IDC_EDIT5 ), TRUE );
				EnableWindow( GetDlgItem(hDlg, IDC_EDIT6 ), TRUE );
				EnableWindow( GetDlgItem(hDlg, IDC_EDIT7 ), TRUE );
				SetDlgItemText( hDlg, IDC_EDIT6, page[ PAGE_MANUAL_POS ].tempStr2 );
				}
			else
				{
				if(strcmp(buffer, "Save") == 0)
					{
					EnableWindow( GetDlgItem(hDlg, IDC_EDIT5 ), FALSE );
					EnableWindow( GetDlgItem(hDlg, IDC_EDIT6 ), FALSE );
					EnableWindow( GetDlgItem(hDlg, IDC_EDIT7 ), FALSE );
					memset( buffer, 0, sizeof( buffer ) );
					GetDlgItemText( hDlg, IDC_EDIT5, buffer, 4 );
					strcpy( page[ PAGE_MANUAL_POS ].PageType, buffer + 1 );
					memset( buffer, 0, sizeof( buffer ) );
					GetDlgItemText( hDlg, IDC_EDIT6, buffer, sizeof( buffer ) - 1 );
					memset(page[PAGE_MANUAL_POS].tempStr,
							0,
							sizeof(page[PAGE_MANUAL_POS].tempStr));
					GetDlgItemText(hDlg, IDC_EDIT7,
									page[ PAGE_MANUAL_POS ].tempStr,
									sizeof(page[PAGE_MANUAL_POS].tempStr) - 1);

					XnUpdateProfile(PAGE_MANUAL_POS, buffer);

					SetDlgItemText( hDlg, IDC_STATUS, "" );
					SetDlgItemText( hDlg, IDC_EDIT1, "" );
					SetDlgItemText( hDlg, IDC_EDIT2, "" );
					SetDlgItemText( hDlg, IDC_EDIT3, "" );
					SetDlgItemText( hDlg, IDC_EDIT4, "" );
					SetDlgItemText( hDlg, IDC_EDIT5, "" );
					SetDlgItemText( hDlg, IDC_EDIT6, "" );
					SetDlgItemText( hDlg, IDC_EDIT7, "" );
					SetDlgItemText( hDlg, IDC_EDIT8, "" );
					EnableWindow( GetDlgItem(hDlg, IDOK), FALSE );
					SetDlgItemText( hDlg, ID_EDIT, "Add New" );
				}
				else
					{
					if(strcmp(buffer, "Add New") == 0)
						{
						SetDlgItemText(hDlg, ID_EDIT, "Save New");
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT4 ), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT5 ), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT6 ), TRUE);
						EnableWindow(GetDlgItem(hDlg, IDC_EDIT7 ), TRUE);
						xnEditMode = 1;
						}
					else
						{
						if(strcmp(buffer, "Save New") == 0)
							{
							EnableWindow( GetDlgItem(hDlg, IDC_EDIT4 ), FALSE );
							EnableWindow( GetDlgItem(hDlg, IDC_EDIT5 ), FALSE );
							EnableWindow( GetDlgItem(hDlg, IDC_EDIT6 ), FALSE );
							EnableWindow( GetDlgItem(hDlg, IDC_EDIT7 ), FALSE );

							memset(page[PAGE_MANUAL_POS].PagerId,
									0,
									sizeof( page[ PAGE_MANUAL_POS ].PagerId ) );
							GetDlgItemText(hDlg,
											IDC_EDIT1,
											page[ PAGE_MANUAL_POS ].PagerId, 10 );
							memset(page[PAGE_MANUAL_POS].Extension,
									0,
									sizeof( page[ PAGE_MANUAL_POS ].Extension ) );
							GetDlgItemText(hDlg,
											IDC_EDIT4,
											page[ PAGE_MANUAL_POS ].Extension, 10 );

							memset(buffer, 0, sizeof( buffer ) );
							GetDlgItemText(hDlg, IDC_EDIT5, buffer, 4 );
							strcpy(page[PAGE_MANUAL_POS ].PageType, buffer + 1 );
							memset(buffer, 0, sizeof( buffer ) );
							GetDlgItemText(hDlg,
											IDC_EDIT6,
											buffer, sizeof( buffer ) - 1 );
							memset(page[PAGE_MANUAL_POS ].tempStr,
									0,
									sizeof( page[ PAGE_MANUAL_POS ].tempStr ) );
							GetDlgItemText(hDlg,
											IDC_EDIT7,
											page[ PAGE_MANUAL_POS].tempStr,
											sizeof( page[ PAGE_MANUAL_POS ].tempStr ) - 1);

							XnInsertProfile( PAGE_MANUAL_POS, buffer );

							SetDlgItemText( hDlg, IDC_STATUS, "" );
							SetDlgItemText( hDlg, IDC_EDIT1, "" );
							SetDlgItemText( hDlg, IDC_EDIT2, "" );
							SetDlgItemText( hDlg, IDC_EDIT3, "" );
							SetDlgItemText( hDlg, IDC_EDIT4, "" );
							SetDlgItemText( hDlg, IDC_EDIT5, "" );
							SetDlgItemText( hDlg, IDC_EDIT6, "" );
							SetDlgItemText( hDlg, IDC_EDIT7, "" );
							SetDlgItemText( hDlg, IDC_EDIT8, "" );
							EnableWindow( GetDlgItem(hDlg, IDOK), FALSE );
							SetDlgItemText( hDlg, ID_EDIT, "Add New" );
							xnEditMode = 0;
							}
						}
					}
				}
			break;

		case IDOK: 	// "PAGE" button.
			{
			char inputText[400];
			int bytesRead;

			if((page[ PAGE_MANUAL_POS ].ComNumber != -1 &&
				param.ComPort[ page[ PAGE_MANUAL_POS].ComNumber].QueType[page[PAGE_MANUAL_POS].ComOffset][1] == '1') ||
				page[ PAGE_MANUAL_POS ].PageStatus[ 0 ] == 'L')
				{
				//GetDlgItemText( hDlg,
				//	IDC_EDIT2,
				//	page[PAGE_MANUAL_POS].DisplayInfo,
				//	sizeof(page[PAGE_MANUAL_POS].DisplayInfo) - 1);

				bytesRead = GetDlgItemText( hDlg,
					IDC_EDIT2,
					inputText,
					sizeof(inputText));


				}
			else
				{
				//GetDlgItemText(hDlg, IDC_EDIT3,
				//	page[PAGE_MANUAL_POS].DisplayInfo,
				//	sizeof(page[PAGE_MANUAL_POS].DisplayInfo) - 1);

				bytesRead = GetDlgItemText(hDlg, IDC_EDIT3,
					inputText,
					sizeof(inputText));
				}
			
			page[PAGE_MANUAL_POS].longMessage.assign(inputText, bytesRead);
			//page[PAGE_MANUAL_POS].longMessage.append("\0", 1);

			strncpy(page[PAGE_MANUAL_POS].DisplayInfo, inputText, sizeof(page[PAGE_MANUAL_POS].DisplayInfo));

			//append a zero at end of string when message > limit
			if(page[PAGE_MANUAL_POS].longMessage.length() >= sizeof(page[PAGE_MANUAL_POS].DisplayInfo))
				{
				page[PAGE_MANUAL_POS].DisplayInfo[sizeof(page[PAGE_MANUAL_POS].DisplayInfo) - 1] = 0;
				}

			AddSetUp(PAGE_MANUAL_POS, "XP Keybd");

			page[PAGE_MANUAL_POS].PortNumber = GetQuePointer(PAGE_MANUAL_POS);
			
			if((page[PAGE_MANUAL_POS].PortNumber >= 0) || IsSmsType(PAGE_MANUAL_POS))
				{
				page[PAGE_MANUAL_POS].PriorityQPointer = 0;
				AddQueueEx(PAGE_MANUAL_POS, page[PAGE_MANUAL_POS].PortNumber);
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"Q add not ready %s, %s, fh %i.",
								page[PAGE_MANUAL_POS].Extension,
								page[PAGE_MANUAL_POS].PageType,
								page[PAGE_MANUAL_POS].PortNumber);
				}
			
			SetDlgItemText( hDlg, IDC_STATUS, "" );
			SetDlgItemText( hDlg, IDC_EDIT3, "" );
			SetDlgItemText( hDlg, IDC_EDIT4, "" );
			SetDlgItemText( hDlg, IDC_EDIT5, "" );
			SetDlgItemText( hDlg, IDC_EDIT6, "" );
			SetDlgItemText( hDlg, IDC_EDIT7, "" );
			SetDlgItemText( hDlg, IDC_EDIT8, "" );
			EnableWindow( GetDlgItem(hDlg, IDOK), FALSE );
			SetDlgItemText( hDlg, ID_EDIT, "Add New" );
			}
			break;

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		default:
			break;
		}
		break;

	default:
		break;
		}

	return FALSE;
	}


