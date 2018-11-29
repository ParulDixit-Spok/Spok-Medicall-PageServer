
// Util2.cpp

#include "common.h"

#include "Util2.h"
#include <io.h>
#include <sys\stat.h>
#include "Registry43.h"
#include "Impairment.h"
#include "Append2File.h"


// disable deprecation
#pragma warning(disable:4996)


	unsigned __stdcall
CheckForNewExeThread(void * temp)
	{
	temp;

	// delay on startup X seconds;
	Sleep(1000);

	PrintMessage(MSGTYPE_DEBUG, 
					hwndGeneralMsg, 
					"CheckForNewExeThread(): [%s]", 
					"Start");

	while(!bKillThreads)
		{
		CheckForNewEXE();

		// check every X seconds for new .exe arrival.
		Sleep(1000);
		}

	PrintMessage(MSGTYPE_DEBUG, 
					hwndGeneralMsg, 
					"CheckForNewExeThread(): [%s]", 
					"Stop");

	return 0;
	}


	unsigned __stdcall
CheckForAppRestartThread(void *)
	{
	while(!bKillThreads)
		{
		CheckForAppRestart(param.ExitForRestartHour, param.ExitForRestartMinute);

		// check every XX seconds.
		Sleep(3 * 1000);
		}

	return 0;
	}


	void
ShowHelpScreen(void)
	{	
	CString HelpText =
		"\r"
		" -B      Backup mode / failsafe mode\r"
		"\r"
		" -C      Use Configuration file name as specified on the command line. \r"
		"                Example: when the command line option -C002 \r"
		"                is used, the configuration file named \r"
		"                XGPRM1.002 will be used instead of the \r"
		"                default XGPRM1.XTD. \r"
		"                Extensions are restricted to exactly 3 characters. \r"
		"\r"
		" -H      Show this command line Help screen\r"
		" -?       Same as -H\r"
		"\r"
		" -M      Use the installer Menu\r"
		"                exposes \"Message\" menu items. \r"
		"                also, 1000 line message listboxes for message windows. \r"
		"\r"
		" -E      Check for new Executable in:\r"
		"                the subdirectory \"NEW\" located off of the \r"
		"                the subdirectory where the PageServer .EXE \r"
		"                is running from. \r"
		"                when a new .EXE is found, Page Server application will exit. \r"
		"                A batch file can copy the new .EXE to the appropriate location \r"
		"                and re-start using the new version \r"
		"\r"
		" -D      Use this to Disable the \"Are you Sure?\" Dialog Box\r"
		"                as the application exits.\r"
		"\r"
		" -I      Ignore IVR Hardware.\r"
		"                Dialogic - IVR hardware is disabled, \r"
		"                and will not be opened, or used by this application. \r"
		"\r"
		" -S      Enable beep sounds.\r"
		"                This is a Software Development option only. \r"
		"\r"
		" -J      Enable state change messages.\r"
		"                This is a Software Development option only. \r"
		"\r"
		" -T      Log all comm port Traffic to logfile.\r"
		"                This may have negative impact on performance, \r"
		"                turn it off when you are finished troubleshooting. \r"
		"\r"
		" Notes:\r"
		"\r"
		"    1) Options can be specified in upper or lower case \r"
		"    2) Options can be specified in any order \r"
		"    3) Options can begin with ' - ' or ' / ' \r"
		"\r";

	::MessageBox(NULL,
				HelpText,
				"Help For Spok Page Server Command Line Options",
				MB_ICONINFORMATION | MB_OK);
	}


	void
ShowInfoGeneral(void)
	{
	CString csGeneralInfo;
	
	csGeneralInfo.Format(
						"\r\t\t\t\t\t\t\t\t"
						"\r"
						"CustomerInfoShortName: \t[%s]\r"
						"\r"
						"PageServer Station/Unit ID: \t[%s]\r"
						"\r"
						"AppName: \t\t[%s]\r"
						"AppVersion: \t\t[%s]\r"
						"\r"
						"Ini Files Directory: \t\t[%s]\r"
						"Data Files Directory: \t[%s]\r"
						"Local Files Directory: \t[%s]\r"
						"\r"
						"DataSource: \t\t[%s]\r"
						"\r"
						"Command Line: \t\t[%s]\r"
						"\r",

						param.CustomerInfoShortName,
						param.stationId,
						szAppName,
						szAppVersion,
						param.NotisDir,
						param.AutoDir,
						param.BkupDir,
						pszDataSourceText[DataSource],
						GetCommandLine()
						);

	::MessageBox(NULL, 
				csGeneralInfo, 
				"GENERAL INFORMATION:", 
				MB_ICONINFORMATION | MB_OK);
	}


	void
ShowInfoDatabase(void)
	{
	CString csGeneralInfo;
	
	if(bCmdLineUseProgrammerMenu)
		{
		// the programmer gets to see everything, 
		//  including connnect strings with passwords.
		csGeneralInfo.Format(
							"\r\t\t\t\t\t\t\t\t\t"
							"\r"
							"CustomerInfoShortName: \t\t[%s]\r"
							"\r"
							"ConnectStringNetwork: \t\t[%s]\r"
							"ConnectStringFailsafe: \t\t[%s]\r"
							"\r"
							"g_iDBMSInUse : \t\t\t[DBMS_MICROSOFT_SQL_SERVER]\r"
							"\r",
							param.CustomerInfoShortName,
							param.ConnectStringNetwork,
							param.ConnectStringFailsafe
							);
		}
	else
		{
		// no connect strings will be shown for database security.
		//  the database userid, and password are present in these strings.
		csGeneralInfo.Format(
							"\r\t\t\t\t\t\t\t\t\t"
							"\r"
							"CustomerInfoShortName: \t\t[%s]\r"
							"\r"
							"g_iDBMSInUse : \t\t\t[DBMS_MICROSOFT_SQL_SERVER]\r"
							"\r",
							param.CustomerInfoShortName
							);
		}

	::MessageBox(NULL, 
				csGeneralInfo, 
				"DATABASE INFORMATION:", 
				MB_ICONINFORMATION | MB_OK);
	}


	void
ShowInfoCustomer(void)
	{
	CString csGeneralInfo;
	
	csGeneralInfo.Format(
						"\r\t\t\t\t\t\t\t"
						"\r"
						"CustomerInfoShortName:\t[%s]\r"
						"\r"
						"CustomerInfoFullName: \t[%s]\r"
						"CustomerInfoCity: \t\t[%s]\r"
						"CustomerInfoState:\t[%s]\r"
						"\r",
						param.CustomerInfoShortName,
						param.CustomerInfoFullName,
						param.CustomerInfoCity,
						param.CustomerInfoState	
						);

	::MessageBox(NULL, 
				csGeneralInfo, 
				"CUSTOMER INFORMATION:", 
				MB_ICONINFORMATION | MB_OK);
	}


	void
ShowInfoParameters(void)
	{
	CString csGeneralInfo;

	csGeneralInfo.Format(
						"\r\t\t\t\t\t\t\t"
						"\r"
						"CustomerInfoShortName:\t[%s]\r"
						"\r"
						"FileChecksum32: \t[%08X]\r"
						"FileSignature: \t[%s]\r"
						"FileVersion:\t[%s]\r"
						"\r"
						"DateWritten: \t[%s]\r"
						"TimeWritten: \t[%s]\r"
						"\r"
						"Sequence: \t[%d]\r"
						"\r",

						param.CustomerInfoShortName,
						param.Header.FileChecksum32,
						param.Header.FileSignature,
						param.Header.FileVersion,
						param.Header.DateWritten,
						param.Header.TimeWritten,
						param.Header.ConfigSequenceNumber
						);

	::MessageBox(NULL, 
				csGeneralInfo, 
				"PARAMETER INFORMATION:", 
				MB_ICONINFORMATION | MB_OK);
	}


	void
ShowImpairedVendors(void)
	{
	CString csGeneralInfo;
	CString csElement;
	CString csDataBlock = "";

	// add info to data block for each impaired vendor
	//  with <cr> after each element.
	//
	for(int i = 0; i < MAX_QUEUE_FILES; i++)
		{
		if(Queue[i].Id[0] == '%')
			{
			// queue item is defined, AND is in the list of impaired vendors.
			//
			// format up each element.
			csElement.Format("%d\t%s\t%s\t%s\r", 
							i + 1,
							Queue[i].Id, 
							g_IMP.IsImpairedVendor(Queue[i].Id) ? "True" : "False",
							g_IMP.GetCustomGreetingProfileId(Queue[i].Id)
							);

			// append element to the main string.
			csDataBlock += csElement;
			}
		}

	csGeneralInfo.Format(
						"\r\t\t\t\t\t\t\r"
						"Use Impairment Announcements:\t[%s]\r"
						"Impairment Check Interval (seconds):\t[%d]\r"
						"Impairment ProfileId Pattern:\t\t[%s]\r"
						"Impairment Status To Enable:\t\t[%s]\r"
						"Impairment Format Number:\t\t[%d]\r"
						"\r"
						"Item\tQueue\tImpaired\tAnnouncement\r"
						"Number\tId\tStatus\tProfile Id\r"
						"-----------\t-----------\t-----------\t-----------\r"
						"%s \r",
						param.UseImpairmentAnnouncements ? "True" : "False",
						param.ImpairmentCheckInterval,
						param.ImpairmentProfileIdPattern,
						param.ImpairmentStatusToEnable,
						g_iImpairmentFormatNumber,
						csDataBlock
						);

	::MessageBox(NULL, 
				csGeneralInfo, 
				"IMPAIRED VENDOR INFORMATION:", 
				MB_ICONINFORMATION | MB_OK);
	}


	BOOL
ProcessCommandLineOptions(void)
	{
	int x;
	CString csFileName, csOption, csTemp;
	CString csProgramExeFileName, csProgramExePathName;
	int nPosition;

	// need to look at functions and globals from:
	//  CRTL - C RunTime Library, that do this stuff already.
	//  e.g. _splitpath(), _pgmptr

	// process command line options if any.
	//  there is always at least one.
	//  __argv[0] is the name of the executable,
	//    "util.exe" or "c:\util\util.exe" depending
	//    on how the program was started, batch file,
	//    windows shortcut, etc.
	//  __argv[1] holds the first actual command line option.

	// show the entire command line.
	strcpy(szApplicationPathAndFileName, __argv[0]);

	// show the bare executable name.
	csTemp = __argv[0];

	// find the final backslash character.
	nPosition = csTemp.ReverseFind('\\');

	// take 1 char past the backslash thru the end of the string
	//  as the bare filename.
	csProgramExeFileName = csTemp.Mid(nPosition + 1);
	strcpy(szApplicationFileName, csProgramExeFileName);

	// take up to and including the final backslash
	//  as the application file pathname.
	csProgramExePathName = csTemp.Left(nPosition + 1);
	strcpy(szApplicationPathName, csProgramExePathName);

	// note that this routine is called "early on" and
	//  can only write to message files.
	for(x = 1; x < __argc; x++)
		{
		// manipulate the argument before any further processing

		// copy to a CString for easier manipulation
		csOption = __argv[x];

		//  translate to all uppercase
		csOption.MakeUpper();

		//  translate "/" to "-" for program options.
		csOption.Replace('/', '-');

		// attempt to recognize each command line option.
		if(csOption == "-B")
			{
			// the -B option indicates that the FAILSAFE
			//  mode of operation is to be used.
			bCmdLineFailSafeMode = TRUE;
			}
		else if(csOption.Find("-C") == 0)
			{
			// option "-C" recognized.
			//
			// strict rules:
			// must be a 3 char string, representing a file extension.

			if(csOption.GetLength() == 5)
				{
				// check for illegal chars, ...implement later.

				// extract the 3 char extension that follows the 2 chars "-C"
				CString csFileExtension; csFileExtension = csOption.Mid(2, 3);

				// put the result in a simple 'C' style character array.
				strcpy(g_ConfigFileExtension, csFileExtension);

				// post that an alternate file extension 
				//  is to be used.
				g_bUseAlternateConfigFile = TRUE;
				}
			else
				{
				// too long or too short.
				return FALSE;
				}
			}
		else if(csOption == "-D")
			{
			// option recognized.
			bCmdLineUseExitDialog = FALSE;
			}
		else if(csOption == "-E")
			{
			// option recognized.
			bCmdLineCheckForNewEXE = TRUE;
			}
		else if((csOption == "-H") || (csOption == "-?"))
			{
			// Help option recognized.
			ShowHelpScreen();

			// return FALSE, application will exit.
			return FALSE;
			}
		else if(csOption == "-I")
			{
			// IVR - Dialogic hardware is usually enabled, 
			//  specify this option on the command line to DISABLE.
			bCmdLineUseDialogicHardware = FALSE;
			}
		else if(csOption == "-M")
			{
			// option recognized.
			bCmdLineUseAlternateMenu = TRUE;
			}
		else if(csOption == "-P")
			{
			// option recognized.
			bCmdLineUseProgrammerMenu = TRUE;
			}
		else if(csOption == "-S")
			{
			// option recognized.
			bCmdLineEnableBeepSound = TRUE;
			}
		else if(csOption == "-L")
			{
			// option recognized.
			bCmdLineEnableDebugLogging = TRUE;
			}
		else if(csOption == "-N")
			{
			// No Standard Logging to logfiles - option recognized.
			bCmdLineEnableStandardLogging = FALSE;
			}
		else if(csOption == "-J")
			{
			// option recognized.
			d4xDiags = TRUE;
			}
		else if(csOption == "-X")
			{
			// run the Experimental thread function. 
			bCmdLineEnableExperimentThread = TRUE;
			}
		else if(csOption == "-Z")
			{
			// minimiZe the pageserver application at startup. 
			bCmdLineMinimizeWindowAtStartup = TRUE;
			}
		else if(csOption == "-T")
			{
			// log all commport Traffic, for debug only. 
			bCmdLineEnableLogCommportTraffic = TRUE;
			}
		else
			{
			// option not recognized, exit the program.
			return FALSE;
			}
		}
	// everything went ok,
	//  no unrecognized options encountered.
	return TRUE;
	}


	void
CheckForNewEXE(void)
	{
	static BOOL FirstTimeThru = TRUE;
	static struct _stat buf1;
	struct _stat buf2;
	int rcode;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char NewDir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char NewProgramExeFilename[MAX_PATH];

	// need to locate the "new" subdirectory where
	//  the application was loaded from,
	//  not a named hard-coded subdirectory.
	//
	// take the C RunTime Library path, split it apart,
	_splitpath(_pgmptr, drive, dir, fname, ext);

	// append "NEW" to the directory part.
	sprintf(NewDir, "%sNEW", dir);

	//  then build a path to the new EXE file.
	_makepath(NewProgramExeFilename,
				drive,
				NewDir,
				fname,
				ext);

	if(FirstTimeThru)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"CheckForNewEXE(): Started.");

		PrintMessage(MSGTYPE_SYSTEM,
						hwndGeneralMsg, 
						"CheckForNewEXE(): old file: [%s]", 
						_pgmptr);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"CheckForNewEXE(): new file: [%s]", 
						NewProgramExeFilename);
	
		// capture time and date of currently running exe file
		//   using _stat() function and examining the
		//   modification time.
		FirstTimeThru = FALSE;

		rcode = _stat(_pgmptr, &buf1);

		if(rcode == 0)
			{
			// got file information regarding this file.
			}
		else
			{
			// file could not be located.
			//  no file date information available.
			}
		}

	/* check for newer exe file available in ..\new subdirectory
	 *  new file must arrive in a different subdirectory
	 *  because the currently running file is in-use,
	 *  and cannot be replaced.
	 */
    rcode  = _stat(NewProgramExeFilename, &buf2);

    if(rcode == 0)
		{
		// no problem getting stat information,
		//  compare to original info
		if(buf1.st_mtime != buf2.st_mtime)
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"CheckForNewEXE(): New EXE found.");

			Sleep(5000);

			SetExitCode(APP_EXIT_NEW_EXE_DETECTED);

			// WM_CLOSE message sent to the Main Frame window,
			//  will eventually stop the application.
			SendMessage(hwndFrame, WM_CLOSE, 0L, 0L);
			}
		}
	}



	void
CheckForAppRestart(int RestartHour, int RestartMinute)
	{
	static BOOL FirstTimeThru = TRUE;
	SYSTEMTIME stTimeNow;
	static BOOL MidnightRollover = FALSE;
	static int SavedHour = 0;
	char Time[100];
	int CurrentHour;
	int CurrentMinute;

	if(FirstTimeThru)
		{
		FirstTimeThru = FALSE;

		GetLocalTime(&stTimeNow);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"CheckForAppRestart(): Started.");

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"CheckForAppRestart(): current time: [%02d:%02d]",
						stTimeNow.wHour,
						stTimeNow.wMinute);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"CheckForAppRestart(): target time: [%02d:%02d]",
						RestartHour,
						RestartMinute);

		// if the target time has not already passed,
		//  at the time of starting the application, go ahead
		//  and set the MidnightRollover flag to true,
		//  so that the exit-restart will happen today,
		//  at the appointed time, between now, and midnight.
		//
		// first, look at just the hour,
		//  if current hour is less than target hour, set the flag.
		// next,
		//  if the current hour is equal to the target hour ,
		//  then the minutes values must be examined.
		//
		if((stTimeNow.wHour < RestartHour) ||
			((stTimeNow.wHour == RestartHour) && (stTimeNow.wMinute < RestartMinute)))
			{
			// target time has not passed yet.
			MidnightRollover = TRUE;
			}

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"CheckForAppRestart(): MidnightRollover flag:  [%d]",
						MidnightRollover);
		}

	// get local time into a SYSTEMTIME structure
	GetLocalTime(&stTimeNow);
	CurrentHour = stTimeNow.wHour;
	CurrentMinute = stTimeNow.wMinute;

	sprintf(Time,
			"%04.4d-%02.2d-%02.2d %02.2d:%02.2d",
			stTimeNow.wYear,
			stTimeNow.wMonth,
			stTimeNow.wDay,
			stTimeNow.wHour,
			stTimeNow.wMinute);

	/* check if midnight has passed,
	 *  only after midnight can the appointed time be compared.
	 */
	if(MidnightRollover)
		{
		// once midnight has come and gone...
		//  consider the Restart time.

		if((CurrentHour >= RestartHour) &&
			(CurrentMinute >= RestartMinute))
			{
			// time to shutdown the app.
			//  batch file is expected to restart the app.
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg,
							"CheckForAppRestart(): time to exit-restart the app.");

			//Sleep(5000);

			// using the bCmdLineCheckForAppRestart automated option implies
			//  that you probably don't want to see
			//  the "ok to exit application?" dialog either.
			bCmdLineUseExitDialog = FALSE;

			SetExitCode(APP_EXIT_EXIT_FOR_RESTART);

			// WM_CLOSE message sent to the Main Frame window,
			//  will eventually stop the application.
			SendMessage(hwndFrame, WM_CLOSE, 0L, 0L);
			}
		else
			{
			//PrintMessage(hwndGeneralMsg,
			//	"CheckForAppRestart(): not time to exit-restart yet.");
			}
		}
	else
		{
		// keep looking for midnight roll-over.
		if(SavedHour < CurrentHour)
			{
			// capture the highest value of the current hour seen so far.
			SavedHour = CurrentHour;
			}

		// no midnight rollover yet... attempt to detect one.
		//  this is when suddenly the CurrentHour has
		//  become less than the SavedHour...
		//  just like just after midnight when the
		//  value of CurrentHour goes from 23 back to 0.
		if(CurrentHour < SavedHour)
			{
			MidnightRollover = TRUE;
			}
		}
	}



	void
SetExitCode(int ExitCode)
	{
	static BOOL FirstTimeThru = TRUE;

	// only the first comer can fill the exit code.
	if(FirstTimeThru)
		{
		FirstTimeThru = FALSE;

		iAppExitReason = ExitCode;
		}
	}


	void
DumpHexBytes(char * Buffer, int Count)
	{
	char * pBuffer;
	int i;
	char S[10], String[80];

	pBuffer = Buffer;

	while(Count >= 10)
		{
		// prepare output string of 10 chars
		String[0] = 0;

		for(i =0; i < 10; i++)
			{
			char c = *pBuffer++;
			sprintf(S, "[%02x] ", (int) c);
			strcat(String, S);
			}

		// print the prepared String
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg, 
						"%s", 
						String);
		Count -= 10;
		}

	// last group of 9 or less chars.
	//  could be none.
	if(Count != 0)
		{
		String[0] = 0;

		for(i=0; i < Count; i++)
			{
			char c = *pBuffer++;
			sprintf(S, "[%02x] ", (int) c);
			strcat(String, S);
			}

		// print the short String
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg, 
						"%s", 
						String);
		}
	}


	void
SelectListPageText(LPTX *ListPageText)
	{
	CString csRawQue, csNumeric, csAlpha;
	CString csTemp, csPureNumeric;

	csRawQue = ListPageText->RawQueue;

	if(csRawQue.IsEmpty())
		{
		// found an empty string,
		//  empty strings will be filled subsequently.
		}
	else if(csRawQue.Find("++") == 0)
		{
		// found "++" at the very beginning
		//  this means one alpha item was supplied
		//  by Xtend Console, or by Xtend WebXchange.

		// select the whole string, except the 2 leading characters.
		csAlpha = csRawQue.Mid(2);
		}
	else if(csRawQue.Find('+') != -1)
		{
		// found '+' somewhere in the string.
		//  this means 2 items available
		// split the string into 2 pieces.

		int PlusLocation = csRawQue.Find('+');

		csNumeric = csRawQue.Left(PlusLocation);
		csAlpha   = csRawQue.Mid(PlusLocation + 1);
		}
	else
		{
		// no '+' found, 1 item only received
		//  could be alpha, could be numeric.

		// any data can pe placed in the alpha field
		//  go ahead and put it there.
		csAlpha = csRawQue;
		}

	// at this point, we have the best raw information from the queue in 
	//  the csAlpha, and csNumeric variables, they may be empty, but 
	//  if any usable information is present, it is split on the '+' char.


	// 2009-05-08 kgill
	//
	// changed the logic here, note that IVR list page request puts an empty numeric message,
	//  and "+" followed by a numeric message in the queue. we need to fixup in this situation also.

	// prefer queue data to any default data.
	//  once the field is filled with any content, the default data will not be used.

	// examine the data, if it is "pure" numeric, then it can be put
	//  into the numeric field as well, otherwise, the empty field will
	//  eventually be filled with a '0', this is the best that we can do.

	// SpanIncluding() returns a string that contains only "acceptable" members.
	//  if any undesirable characters are found, the length
	//  of the csPureNumeric string will be shorter.

	// 2006-01-11 for customer St. Raphael,  consider a '*' to be "pure numeric".
	//  this may have other downstream implications, but that's what
	//  is requested today.
	//

	// examine numeric information extracted from the queue data.
	//  if the numeric part is empty, attempt to extract pure numeric 
	//  info from the alpha message field.
	//
	if(csNumeric.IsEmpty())
		{
		// no numeric information discovered in the data read from the queue.
		//  attempt to extract pure numeric information from the alpha field.
		//  this seldom works, but is worth trying for.
		//
		//csPureNumeric = csAlpha.SpanIncluding("0123456789*");	// purify alpha to numeric.

		// 2010-06-25 kgill - now include <space>, and <minus> characters as "pure" numeric.
		//  this was for customer MercyMedicalRedding, they are using IVR dial-in to set off
		//  list pages, and they use the autoformat feature of pageserver. Previous to this
		//  fix, the autoformat messed up the "pure" numeric nature of the message digits
		//  entered using the telephone keypad.
		//
		csPureNumeric = csAlpha.SpanIncluding("0123456789* -");	// purify alpha to numeric.

		// if the length is found to be the same, 
		//  then the alpha message contained only "legal" numeric elements.
		if(csPureNumeric.GetLength() == csAlpha.GetLength())
			{
			// pure numbers were found, use them to fill the numeric field.
			//  note that the string could still be empty.
			csNumeric = csPureNumeric;
			}
		else
			{
			// length is different, set the numeric string to empty.
			csNumeric.Empty();
			}
		}
	else
		{
         //SO - 03/07/2013 - Added space and dash to accepted characters 
         // because it was cutting off anything after dash

		// the numeric field has some content, purify it.
		//
		csNumeric = csNumeric.SpanIncluding("0123456789* -");	// purify numeric to numeric.
		}

	// almost done, use hard coded default data only if necessary.
	//
	if(csNumeric.IsEmpty())
		{
		csNumeric = ListPageText->DefaultNumeric;
		}

	if(csAlpha.IsEmpty())
		{
		csAlpha = ListPageText->DefaultAlpha;
		}

	// and finally, use hard coded '0' as a last resort, if these items
	//  have not been filled yet.
	//
	if(csNumeric.IsEmpty())
		{
		csNumeric = "0";
		}

	if(csAlpha.IsEmpty())
		{
		csAlpha = "0";
		}

	// move CString data items into the structure.
	strncpy(ListPageText->FinalNumeric,
				csNumeric,
				sizeof(ListPageText->FinalNumeric));

	strncpy(ListPageText->FinalAlpha,
				csAlpha,
				sizeof(ListPageText->FinalAlpha));
	}						


	void
PrintMsg2Screen(HWND hwndChildWinHandle, char * Msg)
	{
	DWORD dwCount;

	// disable re-draw temporarily.
	SendMessage(hwndChildWinHandle, WM_SETREDRAW, (WPARAM) FALSE, (LPARAM) 0);

	// replace one item
	SendMessage(hwndChildWinHandle, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0L);
	SendMessage(hwndChildWinHandle, LB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPSTR)Msg);

	// set cursor on the last item of the screen to show
	//  most recent information.
	dwCount = SendMessage(hwndChildWinHandle, LB_GETCOUNT, (WPARAM) 0, (LPARAM) 0L);
	SendMessage(hwndChildWinHandle, LB_SETCURSEL, (WPARAM) (dwCount-1), (LPARAM) 0L);

	// allow re-draw now.
	SendMessage(hwndChildWinHandle, WM_SETREDRAW, (WPARAM) TRUE, (LPARAM) 0);
	}


	void
SendMessageToLogfile(int MessageTypeMask, char * MessageText)
	{
	CString csMessageText; csMessageText = MessageText;

	// log everything including debug messages.
	//  no qualification required to include everything.
	g_LogDbg.LogMessage(csMessageText, LOG_OPTION_DATE);

	// everything except debug messages.
	if(MessageTypeMask & (MSGTYPE_DEBUG | 
							MSGTYPE_DEBUG_2000 |
							MSGTYPE_DEBUG_4000 |
							MSGTYPE_DEBUG_8000))
		{
		// debug message found... skip it.
		}
	else
		{
		// all others are logged.
		g_LogAll.LogMessage(csMessageText, LOG_OPTION_DATE);
		}


	// now, log selectively, based on message type.

	// system and error messages to the system logfile.
	if(MessageTypeMask & (MSGTYPE_SYSTEM | 
							MSGTYPE_ERROR | 
							MSGTYPE_SYSTEM_FINAL))
		{
		g_LogSys.LogMessage(csMessageText, LOG_OPTION_DATE);
		}

	// database messages to the database logfile.
	if(MessageTypeMask & (MSGTYPE_DATABASE_COMMAND | 
							MSGTYPE_DATABASE_DATA |
							MSGTYPE_DATABASE_DETAIL |
							MSGTYPE_DATABASE_8 |
							MSGTYPE_DATABASE_ERROR))
		{
		g_LogDbs.LogMessage(csMessageText, LOG_OPTION_DATE);
		}

	// ivr (dialogic) messages to the ivr logfile.
	if(MessageTypeMask & (MSGTYPE_IVR | 
							MSGTYPE_IVR_VOICE_PROMPT_TEXT |
							MSGTYPE_DIALOGIC_4 |
							MSGTYPE_IVR_PROGRESS))
		{
		g_LogIvr.LogMessage(csMessageText, LOG_OPTION_DATE);
		}

	// comportmessages to the com logfile.
	if(MessageTypeMask & (MSGTYPE_COMM |
							MSGTYPE_COMM_ERROR |
							MSGTYPE_COMM_DTR |
							MSGTYPE_COMM_RESET))
		{
		g_LogCom.LogMessage(csMessageText, LOG_OPTION_DATE);
		}

	// debug messages to the debug logfile.
	if(MessageTypeMask & (MSGTYPE_DEBUG | 
							MSGTYPE_DEBUG_2000 |
							MSGTYPE_DEBUG_4000 |
							MSGTYPE_DEBUG_8000))
		{
		g_LogDbg.LogMessage(csMessageText, LOG_OPTION_DATE);
		}
	}



	void
SendMessageToScreen(int MessageTypeMask, HWND hwndChildWinHandle, char * MessageText)
	{
	SYSTEMTIME currTime;
	int iLineWidth = 115;
	int iLineMargin = 24;
	char ScreenMessage[1000 + 20];
	char cBuffer[1000 + 20 + 1];
	CString csWhole, csChunk;

	ZeroMemory(ScreenMessage, sizeof(ScreenMessage));

	// prepare time of day
	GetLocalTime(&currTime);

	// combine time of day, and formatted message content,
	//  and send it to the screen.

	// show milliseconds time resolution.
	sprintf(ScreenMessage, 
			"%2.2d:%2.2d:%2.2d.%03d %s",
			currTime.wHour,
			currTime.wMinute,
			currTime.wSecond,
			currTime.wMilliseconds,
			MessageText);

	// limit messages to only enabled types.
	//  directed to windows that are currently open.
	if((MessageTypeMask & MessageEnableMask) && (hwndChildWinHandle != NULL))
		{
		// move string with time information prepended,
		//  into a CString for easy manipulation.
		csWhole = ScreenMessage;

		if(csWhole.GetLength() > iLineWidth)
			{
			// the whole string consists of
			//  more than XXX chars, break it into pieces
			//  and print it on separate lines.
			do	{
				// grab a chunk from the left of the whole string.
				csChunk = csWhole.Mid(0, iLineWidth);
				strcpy(cBuffer, csChunk);

				PrintMsg2Screen(hwndChildWinHandle, cBuffer);

				// cut off the chunk that has just been printed.
				csWhole = csWhole.Mid(iLineWidth);

				// add indentation string,
				//  filled with spaces on the "continued" lines.
				CString csSpaces(' ', iLineMargin);
				csWhole =  csSpaces + csWhole;
				}
			while(csWhole.GetLength() > iLineWidth);

			// print the final chunk, what's left of the whole thing.
			strcpy(cBuffer, csWhole);
			PrintMsg2Screen(hwndChildWinHandle, cBuffer);
			}
		else
			{
			// 80 chars or less.
			PrintMsg2Screen(hwndChildWinHandle, ScreenMessage);
			}
		}
	}


	// note: use the 4 parameter version of this if the string data 
	//       might contain any sequence that could possibly be 
	//       construed as a "format string" itself.
	//       when in doubt, use "%s" as the third parameter.

	// PrintMessage(): send information to a window,
	//  and log file, screen and logfile implement
	//  their own filtering, and formatting rules.
	//  of bit-fields of the form MSGTYPE_XXXXX.
	//
	void
PrintMessage(int MessageTypeMask, HWND hwndChildWinHandle, char * FormatString, ...)
	{
	va_list marker;
	char MessageText[1000];
	ZeroMemory(MessageText, sizeof(MessageText));

	// iterate the variable parameters, format according
	//  to the format string.
	va_start(marker, FormatString);
	_vsnprintf(MessageText, (sizeof(MessageText) - 1), FormatString, marker);
	va_end(marker);

	// pass the message text to the logging facility,
	//  the it will apply its own filtering, and formatting.
	SendMessageToLogfile(MessageTypeMask, MessageText);

	// pass the message to the screen logging facility,
	//  it will apply its own filtering, and formatting.
	SendMessageToScreen(MessageTypeMask, hwndChildWinHandle, MessageText);
	}


	void DumpMessage(int MessageTypeMask, HWND hwndChildWinHandle, char* msg)
	{
		SendMessageToLogfile(MessageTypeMask, msg);

		// pass the message to the screen logging facility,
		//  it will apply its own filtering, and formatting.
		SendMessageToScreen(MessageTypeMask, hwndChildWinHandle, msg);
	}


	unsigned long 
GetNextRegistryNumber(void)
	{
	unsigned long Number;

	CString csKeyName;
	CString csCounterName;

	// define the registry key name to use.
	csKeyName = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Spok\\PageServer\\4-20-XXXX\\Counters";
	csCounterName = "SequenceNumber";

	Number = GetRegistryULong(csKeyName, csCounterName);
	Number++;
	SetRegistryULong(csKeyName, csCounterName, Number);

	return Number;
	}


	int
FindIndexOfQueueStructure(const char * Q)
	{
	int x;
	int QueueIndex = -1;

	// scan Queue[].Id to find page type matching
	//  data from the passed structure QBS.
	for(x = 0; x < 30; x++)
		{	
		// structure QBS.PageType contains data of the form 57 and
		//  Queue[x].Id is of the form %57, so start at char offset 1.
		if((Q[0] == Queue[x].Id[1]) && (Q[1] == Queue[x].Id[2]))
			{
			// found what we were looking for.
			QueueIndex = x;

			// no point to look any further.
			break;
			}
		}
	return x;
	}


	void
TestSendScheduledAlphaPage(void)
	{
	}


	void
TestSendScheduledVoicePage(void)
	{
	}


	void
TestSendScheduledOverheadPage(void)
	{
	}


	void
TestSendNumericPage(void)
	{
	}


	void
TestSendOverheadPage(void)
	{
	}


	void
TestSendVoicePage1(void)
	{
	}


	void
TestSendVoicePage2(void)
	{
	}


	void
TestSendTalkPage1(void)
	{
	}


	void
TestSendTalkPage2(void)
	{
	}


	void
TestSendTalkPage3(void)
	{
	}


	void
TestSendOnCallPage(void)
	{
	}


	void
TestSend3AlphaPages(void)
	{
	int i;

	for(i = 0; i < 3; i++)
		{
		TestSendAlphaPage1();
		}
	}


	void
TestSendBadPage(void)
	{
	}


