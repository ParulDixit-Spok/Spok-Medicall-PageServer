
// xnpginit.cpp

#include "common.h"

//#include <afx.h>
#include <atlstr.h>
#include "List2.h"
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys\stat.h>
#include "Checksum.h"

// disable deprecation
#pragma warning(disable:4996)



	// return 0 if success, 1 if something went wrong.
	//
	int
XnGetSetUp(void)
	{
	int fh;
	int i;
    int j;
    int n;
    char string[100];
	int BytesExpected;
	int BytesRead;
	char szConfigPathAndFileName[MAX_PATH];
	int iBytesRead;
	int iErrors = 0;

	char FileExtension[_MAX_EXT];

	if(g_bUseAlternateConfigFile)
		{
		// instead of the "standard" .XTD file extension name
		//  for the configuration files, use an alternate
		//  3 digit extension, previously collected 
		//  from the command line "-C" option.
		strcpy(FileExtension, g_ConfigFileExtension);
		}
	else
		{
		// use standard extension == "XTD".
		strcpy(FileExtension, "XTD");
		}

	sprintf(szConfigPathAndFileName, 
				"%s%s.%s", 
				szApplicationPathName, 
				ConfigFileName, 
				FileExtension);

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
					"Reading configuration file: [%s]",
					szConfigPathAndFileName);

    fh = _sopen(szConfigPathAndFileName,
				O_RDONLY | O_BINARY,
				SH_DENYNO,
				S_IREAD | S_IWRITE);

	BytesExpected = sizeof(param);

    if(fh != -1)
		{
        BytesRead = _read(fh, (char *) &param, BytesExpected);
        _close(fh);

		// check the integrity of what was read.
		//  check the filesize
		//	check the signature bytes,
		//	check the parameter file revision.
		//	check the checksum.

		if(BytesExpected != BytesRead)
			{
			::MessageBox(NULL,
							"Cannot continue! \r"
							"configuration file BYTES READ FROM FILE is incorrect.",
							"SPOK PAGE SERVER ENGINE",
							MB_ICONSTOP | MB_OK);
			return(1);
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"FileSize: [%d] is ok!",
							BytesExpected);
			}

		if(strncmp(param.Header.FileSignature, CONFIG_FILE_SIGNATURE, sizeof(param.Header.FileSignature)) != 0)
			{
			::MessageBox(NULL,
							"Cannot continue! \r"
							"configuration file SIGNATURE is incorrect.",
							"SPOK PAGE SERVER ENGINE",
							MB_ICONSTOP | MB_OK);
			return(1);
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"FileSignature: [%s] is ok!",
							param.Header.FileSignature);
			}

		if(strncmp(param.Header.FileVersion, CONFIG_FILE_VERSION, sizeof(param.Header.FileVersion)) != 0)
			{
			::MessageBox(NULL,
							"Cannot continue! \r"
							"configuration file VERSION is incorrect.",
							"SPOK PAGE SERVER ENGINE",
							MB_ICONSTOP | MB_OK);
			return(1);
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"FileVersion: [%s] is ok!",
							param.Header.FileVersion);
			}

		// starting at byte #4, (the 5th byte of the file),
		//  all bytes are included in the checksum.
		unsigned int CalculatedChecksum = GetCRC32( ((char *) &param) + 4, sizeof(param) - 4);

		if(param.Header.FileChecksum32 != CalculatedChecksum)
			{
			::MessageBox(NULL,
							"Cannot continue! \r"
							"configuration file FILECHECKSUM32 is incorrect.",
							"SPOK PAGE SERVER ENGINE",
							MB_ICONSTOP | MB_OK);
			return(1);
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"FileChecksum32: [%08.8X] is ok!",
							param.Header.FileChecksum32);
			}

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"PageServer Configuration file was read OK!");

        for(i = 0; i < MAX_IVRPORTS; i++)
			{
			port[i].DataPointer = -1;
			port[i].LoopCurrent = FALSE;
			}

		memset(pageFileDir, 0, sizeof(pageFileDir));

        if(((param.BkupDir[0] != 0) && (param.BkupDir[0] != ' ')) &&
			(DataSource == DATA_SOURCE_NETWORK))
			{
			strcpy(pageFileDir, param.AutoDir);
			}
		else
			{
			strcpy(pageFileDir, param.BkupDir);
			}

		strcat(pageFileDir, "\\");

//		PageId = TRUE;

        for(i=0; i<4; i++)
			{
            Cats[i].GroupCallWaiting = 0;
            if(param.Cats[i].Line >= 0 && param.Cats[i].Line < PAGE_COM_START)
                port[param.Cats[i].Line].DataPointer = i;
			}

        for(i = 0; i < MAX_COMMPORTS; i++)
			{
            ComPort[i].PortBit = -1;
			}

		// check for serial port external control method.
		//
		if(bExternalControl == false)
			{
			// no external control method has been established yet. 
			//
			// if one of the comm ports is defined as "GO", then 
			//  a signal input from this port is used to control
			//  the active/standby mode of this pageserver.
			//
			for(i = 0; i < MAX_COMMPORTS; i++)
				{
				if(param.ComPort[i].PortActive[0] == 'G')
					{
					// WE are using the serial port as a control input.
					bExternalControl = true;

					g_iExternalControlCommPortToUse = i;

					// poison the possibility of interference from the parallel port method.
//					param.Lpt.Address = 0;


					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"[%s] is configured to be the active/standby control port.",
									CommportName[g_iExternalControlCommPortToUse].FamiliarName);							

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"  note: the DSR signal is on pin 6 of the DB-25 connector,");

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"  and must be pulled high to allow the pageserver to go \"active\".");

					// prevent dtr signal from being manipulated.
					param.ComPort[g_iExternalControlCommPortToUse].toggleDTR = false;

					// look no further, the first one controls it.
					break;
					}
 				}
			}
 
//		// check for parallel port external control method.
//		//
//		if(bExternalControl == false)
//			{
//			// no external control method has been established yet.
//
//			// maybe externally controlled using the parallel port as input.
//			//
//			if(param.Lpt.Address != 0)
//				{
//				// the address IS defined as some non-zero value.
//				//  external control is being used.
//
//				// poison the possibility of interference from the serial port method.
//				g_iExternalControlCommPortToUse = -1;
//				
//				// check each of 8 possible input bit definitions.
//				//  for the "more than 8" controllable items.
//				for(i = 0; i < 8; i++)
//					{
//					switch(param.Lpt.Bit[i][0])
//						{
//						case '1':
//						case '2':
//						case '3':
//						case '4':
//						case '5':
//						case '6':
//						case '7':
//						case '8':
//						case '9':
//							j = atoi(param.Lpt.Bit[i]) - 1;
//							ComPort[j].PortBit = i;							
//							break;
//
//						case 'A':
//							g_iExternalControlParallelPortInputBit = i;
//
//							PrintMessage(MSGTYPE_SYSTEM, 
//											hwndGeneralMsg, 
//											"ACTIVE / STANDBY control using bit # [%d] of port [%04x]", 
//											g_iExternalControlParallelPortInputBit,
//											param.Lpt.Address);
//
//							bExternalControl = true;
//
//							break;
//
//						default:
//							break;
//						}
//					}
//				}
//			}
		}
	else
		{
		// the global variable _pgmptr is defined by the CRT
		//  C RunTime library, and is initialized to hold
		//  the full drive, path, and filename of the
		//  application program executable.

		CString csErrorMessage;
		csErrorMessage.Format("Unable to open PageServer Configuration File.\r"
								"\r"
								"Filename specified was: [%s]\r"
								"this file is expected to be in the same\r"
								"subdirectory as the application program\r"
								"\r"
								"This Application:\r"
								" [%s] \r"
								"\r"
								"Requires Configuration file:\r"
								" [%s] \r",
								ConfigFileName,
								_pgmptr,
								szConfigPathAndFileName);

		::MessageBox(NULL,
						csErrorMessage,
						"SPOK PAGE SERVER ENGINE",
						MB_ICONSTOP | MB_OK);

        PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"ERROR !!! Reading paging configuration file %s.",
						ConfigFileName);

		// cannot continue the starutp process without a valid config file.
		return(1);
		}

	// read NOPGSTAT configuration file, it contains information
	//  about what page statuses are in use.
	// first record 3 chars is "YES" or not "YES", only the first char 'Y' is compared
	//  this controls the StatusOn variable TRUE/FALSE
	//  which controls .... ????
	// then 30 records of 35 chars each,
	// then 30 records of 30 chars each.

	memset(string, 0, sizeof(string));

	if(DATA_SOURCE_LOCAL == DataSource)
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "DATA_SOURCE_LOCAL == DataSource");
		sprintf(string, "%s\\NOPGSTAT", param.BkupDir);
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "DATA_SOURCE_NETWORK == DataSource");
		sprintf(string, "%s\\NOPGSTAT", param.NotisDir);
		}

	// announce where the NOPGSTAT file comes from.
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
					"using NOPGSTAT: [%s] sizeof the structure is [%d] bytes",
					string,
					sizeof(Status));

	// clear out the whole structure.
	// this will help the strings to be null terminated.
	memset(Status, 0, sizeof(Status));

	fh = _sopen(string, O_RDONLY | O_BINARY, SH_DENYNO, S_IREAD | S_IWRITE);

	if(fh != -1)
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "NOPGSTAT: opened ok");

		iBytesRead = _read(fh, string, 3);

		// expected to read exactly 3 bytes.
		if(iBytesRead != 3)
			{
			iErrors++;
			}

		switch(string[0])
			{
			case 'Y':
				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "StatusOn set to: TRUE");
				StatusOn = TRUE;
				break;

			case 'N':
			default:
				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "StatusOn set to: FALSE");
				StatusOn = FALSE;
				break;
			}

		for(i = 0; i < 30; i++)
			{
			iBytesRead = _read(fh, Status[i].Text, 30);

			// expected to read exactly 30 bytes.
			if(iBytesRead != 30)
				{
				iErrors++;
				break;
				}

			Set_Null(Status[i].Text, sizeof(Status[i].Text));

			iBytesRead = _read(fh, (char *) &Status[i].Prompt, 2);

			// expected to read exactly 2 bytes.
			if(iBytesRead != 2)
				{
				iErrors++;
				break;
				}

			iBytesRead = _read(fh, Status[i].Page, 3);

			// expected to read exactly 3 bytes.
			if(iBytesRead != 3)
				{
				iErrors++;
				break;
				}
			}

		for(i = 0; i < 30; i++)
			{
			iBytesRead = _read(fh, Status[i].PageTypes, 30);

			// expected to read exactly 30 bytes.
			if(iBytesRead != 30)
				{
				iErrors++;
				break;
				}
			}

		_close(fh);
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ERROR Reading NOPGSTAT file: %s.", string);
		iErrors++;
		}

	// show the result of what is now in the structure
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						" #          pager status prompt          prompt             page types              option");
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"----   --------------------------------  -------  --------------------------------  ------");
	for(i = 0; i < 30; i++)
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"[%02d] - [%-30.30s]   [%3d]   [%-30.30s]   [%3.3s]",
						i,
						Status[i].Text,
						Status[i].Prompt,
						Status[i].PageTypes,
						Status[i].Page);
		}
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"----   --------------------------------  -------  --------------------------------  ------");

	if(iErrors != 0)
		{
		::MessageBox(NULL,
						"Cannot continue! \r"
						"error reading NOPGSTAT",
						"SPOK PAGE SERVER ENGINE",
						MB_ICONSTOP | MB_OK);
		return(1);
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "NOPGSTAT was read ok!");
		}

	// read the queue control file XTPGQUE.XTD
	//  there are 31 records of 10 chars each.
	//
	memset(string, 0, sizeof(string));

	if(DATA_SOURCE_NETWORK == DataSource)
		{
		strcpy(string, param.AutoDir);
		}
	else
		{
		strcpy(string, param.BkupDir);

		if(param.backupStatus == FALSE)
			{
			StatusOn = FALSE;
			}
		}

	strcat(string, "\\XTPGQUE.XTD");

	// announce where the XTPGQUE.XTD file comes from.
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "using XTPGQUE.XTD: [%s]", string);

    fh = _sopen(string,
				O_RDONLY | O_BINARY,
				SH_DENYNO,
				S_IREAD | S_IWRITE);

    if(fh != -1)
		{
		// read, and setup items 0 thru 30 inclusive, number of items is 31.
		//
        for(i = 0; i < MAX_QUEUE_FILES; i++)
			{
 			// expect successful seek.
			if(_lseek(fh, i * 10, SEEK_SET) == -1L)
				{
				iErrors++;
				break;
				}

            iBytesRead = _read(fh, 
								&Queue[i].Id, 
								sizeof(Queue[i].Id));

			// expected to read exactly 4 bytes.
			if(iBytesRead != sizeof(Queue[i].Id))
				{
				iErrors++;
				break;
				}

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"  Queue[%d].Id: [%s]", 
							i + 1, 
							Queue[i].Id);

			iBytesRead = _read(fh, 
								&Queue[i].StationId, 
								sizeof(Queue[i].StationId));

			// expected to read exactly 4 bytes.
			if(iBytesRead != sizeof(Queue[i].StationId))
				{
				iErrors++;
				break;
				}

 			char Dummy2[2];
			iBytesRead = _read(fh, 
								&Dummy2, 
								sizeof(Dummy2));

			// expected to read exactly 2 bytes.
			if(iBytesRead != sizeof(Dummy2))
				{
				iErrors++;
				break;
				}
			}

		_close(fh);
		}
	else
		{
		// increase error count, just so it will be non-zero,
		//  and an appropriate message will be displayed.
		iErrors++;
        PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ERROR reading Q Control file %s.", string);
		}

	if(iErrors != 0)
		{
		::MessageBox(NULL,
						"Cannot continue! \r"
						"error reading XTPGQUE.XTD",
						"SPOK PAGE SERVER ENGINE",
						MB_ICONSTOP | MB_OK);
		return(1);
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "XTPGQUE.XTD was read ok!");
		}


	// more initialization.
	//

	// each RCC type.
	//
    for(n = 0; n < param.PgTypsNum; n++)
		{
        PageType[ n ].QTimer.wHour = 70;
		}

	// each of 31 queue files.
	//
    for(n = 0; n < MAX_QUEUE_FILES; n++)
		{
        Queue[n].Attempts = 0;
        Queue[n].Errors = 0;
        Queue[n].External = 0;
        Queue[n].alarm = FALSE;
		}

	// no list processing is active at application start up
    ListQue.QTimer.wHour = 70;
    ListQue.Active = FALSE;

    // Initialize comm port items.
	//
    for(n = 0; n < MAX_COMMPORTS; n++)
		{
        ComPort[n].reset = 0;
        ComPort[n].alarm = FALSE;
        ComPort[n].state = RPNOTREADY;
        //ComPort[n].StackState = FALSE;
        ComPort[n].timer.wHour = 70;
        ComPort[n].QTimer.wHour = 70;
        ComPort[n].CSTimer.wHour = 70;
        ComPort[n].ICTimer.wHour = 70;
		memset(ComPort[n].errorResponse, 0, sizeof(ComPort[n].errorResponse));
        page[n + PAGE_COM_START].PageStatus[0] = 'I';
        pstate[n] = PRESET;
        pageerror[n] = PNOERROR;
        ComPort[n].greenLight = TRUE;
		ComPort[n].messageOpt = 0;

        if(param.ComPort[n].SharedMode == BACKUP || param.ComPort[n].SharedMode == SLAVE)
			{
            ComPort[n].greenLight = FALSE;
			}

        if(param.ComPort[n].PortActive[0] == 'Y')
			{
			PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"Commport [%s]: is active in the configuration", 
							CommportName[n].FamiliarName);

			portactive[n] = TRUE;
			}
        else
			{
			portactive[n] = FALSE;
			}

        pageactive[n] = param.ComPort[n].PageActive;

		// init data for 20 possible queues for each comm port.
        for(j = 0; j < 20; j++)
			{
            ComPort[n].QueuePointer[j] = -1;
            ComPort[n].Attempts = 0;
            ComPort[n].Errors = 0;
            ComPort[n].Cancels = 0;
            ComPort[n].External = 0;
			}
		}

	// if you got this far, success!
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "XnGetSetUp() all OK!");
	return 0;
	}


	void
XnInitialize(void)
	{
    int i;
    int j;
    int k;
	SYSTEMTIME CurrTime;
	char string2[100];
	char string3[20];

	NoPaging();

	// for each of the "Misc RCC Dialout Pager" types.
	//
    for(j = 0; j < param.PgTypsNum; j++)
		{
        PageType[j].QueuePointer = GetQuePos(param.PageType[j].QDirect);
        PageType[j].InQueuePointer = GetQuePos(param.PageType[j].pageid);
		}

	// for each of the COMM ports.
	//
    for(j = 0; j < MAX_COMMPORTS; j++)
		{
        for(k = 0; k < 20; k++)
			{
            if(param.ComPort[j].PageId[k][0] == '%')
				{
                ComPort[j].QueuePointer[k] = GetQuePos(param.ComPort[j].QDirect[k]);
                ComPort[j].InQueuePointer[k] = GetQuePos(param.ComPort[j].PageId[k]);
				}
			else
				{
				//  set to -1 to show "not configured".
				ComPort[j].QueuePointer[k] = -1;
				ComPort[j].InQueuePointer[k] = -1;
				}

			}
		}

	QueueDisplayUpdateTime.wHour = 70;

	MultiQTimer1.wHour = 70;
	MultiQTimer2.wHour = 70;
	MidnightTimer.wHour = 70;
	MultiQue.Active = FALSE;

    memset(string2, 0, sizeof(string2));
    memset(string3, 0, sizeof(string3));

    MSGTimeStamp(string2, string3);

	// Yesterday will contain MMDD followed by NUL.
    strncpy(Yesterday, string2, 4);
	Yesterday[4] = 0;

	// HourStart will contain HH followed by NUL.
    strncpy(HourStart, string2 + 4, 2);
	HourStart[2] = 0;

	// pick up text 000 thru 365, day of year 000 == jan 1st
    j = atoi(string3);

	// pickup text 00 thru 06, sunday == 00 thru 06 == saturday
    k = atoi(string3 + 4);

	// calculate the week number of the year
    if(j - k + 1 < 0)
		{
        WeekNumber = 52;
		}
    else
		{
        WeekNumber = (int) ((j - k) / 7);
		}
	// ensure range 0 thru 52 inclusive.
	WeekNumber %= 53;

    string2[2] = 0;
    MonthNumber = atoi(string2) - 1;

	// ensure range 0 thru 11 inclusive.
	MonthNumber %= 12;

    // open the voice files and save file handles
    if(totalD4xChannels > 0)
		{
        memset(string2, 0, sizeof(string2));
        strcpy(string2, param.BkupDir);
        strcat(string2, "\\");
        strcat(string2, param.PromptFile);

        promptfh = vbinit(string2, prompts, MAXPROMPTS);

        PrintMessage(MSGTYPE_SYSTEM,
						hwndD4xMsg, 
						"Voice file %s initialized, FH: %i.", 
						string2, 
						promptfh);

        memset(string2, 0, sizeof(string2));
        strcpy(string2, param.BkupDir);
        strcat(string2, "\\");
        strcat(string2, param.StatusFile);

        StatusFh = vbinit(string2, StatusPrompts, MAXSTATUS);

        PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg, 
						"Status file %s initialized, FH: %i.", 
						string2, 
						StatusFh);

        memset(string2, 0, sizeof(string2));
        strcpy(string2, param.BkupDir);
        strcat(string2, "\\");
        strcat(string2, "digits.vap");

        Digitsfh = vbinit(string2, DigitPrompts, MAXDIGITS);

        PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg, 
						"Digits file %s initialized, FH: %i.", 
						string2, 
						Digitsfh);
		}

    VoiceFileCounter = 0;
    GetLocalTime(&CurrTime);

    do	{
        VoiceFileCounter++;
        memset(string2, 0, sizeof(string2));

		if(DATA_SOURCE_NETWORK == DataSource)
			{
	        sprintf(string2,
					"%s\\%02d%02d%04d.VPO",
					param.AutoDir,
					CurrTime.wMonth,
					CurrTime.wDay,
					VoiceFileCounter);
			}
		else
			{
	        sprintf(string2,
					"%s\\%02d%02d%04d.VPO",
					param.BkupDir,
					CurrTime.wMonth,
					CurrTime.wDay,
					VoiceFileCounter);
			}

        i = GetFileAttributes(string2);
		} while(i != 0xFFFFFFFF && VoiceFileCounter < 9999);

	LptTime.wHour = 70;
	keepAliveTimer.wHour = 70;
	}


	void
CloseMiscFlatFiles(void)
	{
	char HourNow[3];
	char string3[20];
	char string2[85];

	if(param.Statistics != 2)
		{
        memset(string2, 0, sizeof(string2));
        MSGTimeStamp(string2, string3);

        strncpy(HourNow, string2 + 4, 2);
		HourNow[2] = 0;
        memset(string3, 0, sizeof(string3));

        sprintf(string3,
				"%2.2s%2.2s%2.2s",
				xnSystemTime + 8,
				xnSystemTime,
				xnSystemTime + 3);

        param.Statistics = (short int) WriteStatistics(string3, HourNow);
		}

	PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, "closing .VAP files.");

    if(totalD4xChannels > 0)
		{
		// only close if successfully opened earlier.
		if(promptfh > 0)
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Closing Prompt File");
			dx_fileclose(promptfh);
			promptfh = -1;
			}

		if(Digitsfh > 0)
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Closing Digits File");
			dx_fileclose(Digitsfh);
			Digitsfh = -1;
			}

		if(StatusFh > 0)
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Closing Status File");
			dx_fileclose(StatusFh);
			StatusFh = -1;
			}
		}
	}


	int
CreateMsgFile(unsigned long * msgPosition,
			  const char * fileExtension,
			  const char * fileName)
	{
    int i = 0;
	int fileHandle;
    unsigned long j = 0;
    unsigned rv = 0;
    char fname2[40];
    char fname3[40];

    memset(fname2, 0, sizeof(fname2));

    do
		{
        i++;

		sprintf(fname2, "%s\\XTPG%03d.%s", param.BkupDir, i, fileExtension);

		rv = GetFileAttributes(fname2);
		} while(rv != 0xffffffff && i < 999);

    PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
					"Renaming page message file %s.%s as File %s.",
					fileName,
					fileExtension,
					fname2);

    memset(fname3, 0, sizeof(fname3));
	sprintf(fname3, "%s\\%s.%s", param.BkupDir, fileName, fileExtension);

    if(DATA_SOURCE_LOCAL == DataSource)
		{
        i = remove(fname3);
		}
    else
		{
        i = rename(fname3, fname2);
		}

    if(i == 0)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"Rename Completed, Recreating message file.");
		}

    if((fileHandle = _sopen(fname3, 
							O_CREAT | O_RDWR | O_BINARY, 
							SH_DENYNO, 
							S_IREAD | S_IWRITE)) != -1)
		{
        i = 0;
        _lseek(fileHandle, 0L, SEEK_SET);
        _write(fileHandle, (char *) &j, 4);
		}
    else
		{
        PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ERROR Unable to sopen %s.", 
						fname3);
		}

	*msgPosition = 0;

    return fileHandle;
	}



