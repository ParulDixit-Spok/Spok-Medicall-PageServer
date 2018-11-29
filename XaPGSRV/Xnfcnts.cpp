
// xnfcnts.cpp

#include "common.h"

#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <share.h>
#include <winspool.h>
#include "SpoolCollection.h"
#include "Append2File.h"


// disable deprecation
#pragma warning(disable:4996)


Append2File LogList;


	void
Set_Null(char *string, int BufferSize)
	{
	int i;

	// a non-zero, positive number for the
	//  buffer size is required.
	if(BufferSize <= 0)
		{
		return;
		}

	// ensure that the buffer is nul terminated.
	string[BufferSize - 1] = 0;

    for(i = (BufferSize - 1); i >= 0; i--)
		{
		// working from the end of the string, toward the
		//  beginning, replace listed characters
		//  with nul character.
		// once a char that does NOT match the listed chars
		//  is found, stop replacing. don't stop
		//  processing when a nul char is encountered
		//  in the given string.
        if( string[i] == 32 ||  // space
			string[i] == 13 ||  // cr
			string[i] == 10 ||  // lf
			string[i] == 0)     // nul
			{
			string[i] = 0;	  // replace with nul
			}
		else
			{
			break;
			}
		}
	}


	// transfer characters from source to destination
	//  only if the chars are non-zero, and non-space.
	// swallow ALL spaces.

	// this may leave the destination string without null terminator.
	void
XnCopy(char * destString, char * sourceString, int destLen)
	{
	int i;
	int j = 0;

	for(i = 0; i < destLen; i++)
		{
		if(sourceString[i] != 0 &&
			sourceString[i] != ' ')
			{
			destString[j] = sourceString[i];
			j++;
			}
		}
	}


	void
XnBtrCopy(char * destString, char * sourceString, int destLen)
	{
	int i;

	for(i = 0; (i < destLen) && (i < (int) strlen(sourceString)); i++)
		{
		if(sourceString[i] != 0)
			{
			// copy non-nul chars from source to destination
			destString[ i ] = sourceString[ i ];
			}
		else
			{
			// if the char is nul, replace it with a space
			destString[i] = ' ';
			}
		}

	// replace nul chars with space chars for the
	//  remaining string string length.
	for(i; i < destLen; i++)
		{
		if(destString[i] == 0)
			{
			destString[i] = ' ';
			}
		}
	}


	BOOL
Wait(SYSTEMTIME *ReferenceTime, long Interval)
	{
    unsigned long difference_msec;
    unsigned long reference_msec;
    unsigned long current_msec;
    unsigned long interval_msec;
    SYSTEMTIME CurrentTime;

	// see if the specified Interval
	//  has passed since the given ReferenceTime.

	// remember that the Interval is specified in 1/100 seconds
	//  and needs to be translated into milliseconds.
    interval_msec = (unsigned long) Interval * 10;

	// get the time as of now!
    GetLocalTime(&CurrentTime);

	// translate times to millisecond basis.
    reference_msec = ReferenceTime->wHour * 3600000L +
					 ReferenceTime->wMinute * 60000L +
					 ReferenceTime->wSecond  * 1000L +
					 ReferenceTime->wMilliseconds;

    current_msec = 	 CurrentTime.wHour * 3600000L +
					 CurrentTime.wMinute * 60000L +
					 CurrentTime.wSecond *  1000L +
					 CurrentTime.wMilliseconds;

	// calculate the time difference
    difference_msec = TimeDiff(current_msec, reference_msec);

    if(difference_msec >= interval_msec)
		{
		// time has elapsed
		return TRUE;
		}
	else
		{
		// time has not been exceeded
		return FALSE;
		}
	}


	unsigned long
TimeDiff( unsigned long endtime, unsigned long starttime)
	{
    unsigned long tdiff;
	
	if(endtime >= starttime)
		{
		tdiff = endtime - starttime;
		}
	else
		{
		// if endtime is not larger than starting time,
		//  then the time has crossed midnight.
		//  adjust the end time by 24 hours to accomodate
		//  the fact that the difference must always be a
		//  small positive value.
		tdiff = (endtime + (24 * 60 * 60 * 1000)) - starttime;
		}
 	return(tdiff);
	}


	int
WriteLog(const char *FileName, char *string)
	{
	int fh;
 	SYSTEMTIME	currTime;
	char Date[30];
	char fname3[100];

    if(DATA_SOURCE_LOCAL == DataSource)
		{
		return 0;
		}

	GetLocalTime(&currTime);
	memset(Date, 0, sizeof(Date));
	sprintf(Date,
		"%02.2d/%02.2d/%04.4d %02.2d:%02.2d:%02.2d ",
		currTime.wMonth,
		currTime.wDay,
		currTime.wYear,
		currTime.wHour,
		currTime.wMinute,
		currTime.wSecond);	

	sprintf(fname3, "%s\\%s", param.BkupDir, FileName);

    fh = _sopen(fname3,
				O_CREAT | O_APPEND | O_WRONLY | O_BINARY,
				SH_DENYWR,
				S_IREAD | S_IWRITE);

	if(fh != -1)
		{
        _write(fh, Date, 20);
        _write(fh, string, strlen(string));
        _write(fh, "\r\n", 2);
        _close(fh);
		}
	else
		{
        PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ERROR !!! Unable to sopen Log. [%s]", 
						fname3);
		}
	return 0;
	}
	

	void
SendToHtmlFile(int PrintOption, CString csMessage)
	{
	SYSTEMTIME CurrentTime;	
	CString csTemp;
	CString csBaseFilename;
	CString csSubdirectoryName;

	switch(PrintOption)
		{
		case START_PAGE:
			// define the target subdirectory, relative to application's .exe
			//  to receive .html documents.
			csSubdirectoryName = "ListPage";

			// prepare a date "formatted" string to be used as a "base" subdirectory name.
			//  remember the characters that cannot be used in a filename.
			//
			GetLocalTime(&CurrentTime);
			csBaseFilename.Format("%04d-%02d-%02d_%02d-%02d-%02d", 
									CurrentTime.wYear,
									CurrentTime.wMonth,
									CurrentTime.wDay,
									CurrentTime.wHour,
									CurrentTime.wMinute,
									CurrentTime.wSecond);
			// open the logfile.
			LogList.SetProperty(Append2File::LOG_SET_DIRECTORY, csSubdirectoryName);
			LogList.SetProperty(Append2File::LOG_SET_BASE_FILENAME, csBaseFilename);
			LogList.SetProperty(Append2File::LOG_SET_FILE_EXTENSION, (CString) "html");
			LogList.SetProperty(Append2File::LOG_SET_MAX_FILES_IN_ROTATION, 10);
			LogList.SetProperty(Append2File::LOG_SET_MAX_FILE_SIZE, 100 * 1000);
			LogList.SetProperty(Append2File::LOG_SET_USE_SEQUENCE_NUMBER, (CString) "FALSE");
			LogList.SetProperty(Append2File::LOG_SET_ENABLE_LOGGING, (CString) "TRUE");

			LogList.CreateSubdirectory();

			// xhtml document - document beginning.
			csTemp.Format(
				"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
				"<!DOCTYPE html\r\n" 
					"PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\r\n" 
					"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\r\n"
				"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en-US\" lang=\"en-US\">\r\n"

				"<head>\r\n"
					"<title>List Page Report</title>\r\n"
					"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\r\n"
					"<link rel=\"stylesheet\" href=\"Style1.css\" type=\"text/css\">\r\n"
				"</head>\r\n"

				"</body>\r\n");

			LogList.LogMessage(csTemp, LOG_OPTION_RAW);
			break;

		case HEADER_1:
			LogList.LogMessage((CString) "<h1>\r\n", LOG_OPTION_RAW);
			LogList.LogMessage((CString) "<hr></hr>\r\n", LOG_OPTION_RAW);

			csTemp.Format("<p> %s </p>\r\n", csMessage);
			LogList.LogMessage(csTemp , LOG_OPTION_RAW);

			LogList.LogMessage((CString) "</h1>\r\n", LOG_OPTION_RAW);
			break;

		case HEADER_2:
			LogList.LogMessage((CString) "<h2>\r\n", LOG_OPTION_RAW);

			csTemp.Format("<p> %s </p>\r\n", csMessage);
			LogList.LogMessage(csTemp , LOG_OPTION_RAW);

			LogList.LogMessage((CString) "</h2>\r\n", LOG_OPTION_RAW);
			break;

		case HEADER_3:
			LogList.LogMessage((CString) "<h3>\r\n", LOG_OPTION_RAW);

			csTemp.Format("<p> %s </p>\r\n", csMessage);
			LogList.LogMessage(csTemp , LOG_OPTION_RAW);

			LogList.LogMessage((CString) "</h3>\r\n", LOG_OPTION_RAW);
			LogList.LogMessage((CString) "<hr></hr>\r\n", LOG_OPTION_RAW);
			break;

		case BEGIN_TABLE_HEADER:
			LogList.LogMessage((CString) "<tr><th>\r\n", LOG_OPTION_RAW);
			break;

		case END_TABLE_HEADER:
			LogList.LogMessage((CString) "</th></tr>\r\n", LOG_OPTION_RAW);
			break;

		case BEGIN_TABLE_ROW:
			LogList.LogMessage((CString) "<tr><td>\r\n", LOG_OPTION_RAW);
			break;

		case END_TABLE_ROW:
			LogList.LogMessage((CString) "</td></tr>\r\n", LOG_OPTION_RAW);
			break;

		case BEGIN_TABLE:
			LogList.LogMessage((CString) "<table>\r\n", LOG_OPTION_RAW);
			break;

		case END_TABLE:
			LogList.LogMessage((CString) "</table>\r\n", LOG_OPTION_RAW);
			break;

		case MID_PAGE:			
			csTemp.Format("%s\r\n", csMessage);
			LogList.LogMessage(csTemp, LOG_OPTION_RAW);
			break;

		case END_PAGE:
			// print an html "document close" items.
			LogList.LogMessage((CString) "<hr></hr>\r\n", LOG_OPTION_RAW);

			// body begin...
			LogList.LogMessage((CString) "</body>\r\n", LOG_OPTION_RAW);

			LogList.LogMessage((CString) "</html>\r\n", LOG_OPTION_RAW);
			LogList.LogMessage(csTemp, LOG_OPTION_RAW);

			// finish any pending writes.
			LogList.FlushMessagesToDisk(Append2File::FINALIZE_CURRENT_FILENAME);
			break;
		}
	}


	void
SendToSpoolCollection(int PrintOption, CString csMessage)
	{
	CString csHr('-', 65);	// "horizontal rule", a string of XX '-' chars.

	// use the global SpoolCollection object 
	//  reserved for list-page reports.
	switch(PrintOption)
		{
		case START_PAGE:
			pscListPage->FindPrintersOfType(SpoolCollection::PrinterType_ALL);

			pscListPage->SetTopMargin(3);
			pscListPage->SetLeftMargin(6);
			pscListPage->SetLinesPerPage(55);
			break;

		case HEADER_1:
			pscListPage->AddLine(csHr);
			pscListPage->AddLine(csMessage);
			break;

		case HEADER_2:
			pscListPage->AddLine(csMessage);
			break;

		case HEADER_3:
			pscListPage->AddLine(csMessage);
			pscListPage->AddLine(csHr);
			pscListPage->AddLine(" ");
			break;

		case MID_PAGE:
			pscListPage->AddLine(csMessage);
			break;

		case END_PAGE:
			pscListPage->AddLine(" ");
			pscListPage->AddLine(csHr);
			pscListPage->Close();
			break;

		default:
			// unknown text file option.
			break;
		}
	}


	void
ListPagePrinting(int PrintOption, char  *lpMessage, ...)
	{
	// FIX THIS TO NOT OVERRUN THE BUFFER.
	CString csTemp;
	va_list marker;
	char cBuf[1024];

	if(lpMessage != NULL)
		{
		memset(cBuf, 0, sizeof(cBuf));

		va_start(marker, lpMessage);
		vsprintf(cBuf, lpMessage, marker);
 		va_end(marker);

		csTemp = cBuf;
		}

	if((1 == 1) && param.CodePrint)
		{
		SendToSpoolCollection(PrintOption, csTemp);
		}
	}


	void
XnAutoFormat(int pagePointer)
	{
	int i;
	int textLen;
	int textLen2;
	char *cPos;
	char tempStr[150];

	if(param.autoFormat && page[pagePointer].pageFormatted == FALSE)
		{
		if(param.autoFormatStarStop)
			{
			if(strchr(page[pagePointer].DisplayInfo, '*') != NULL)
				{
				return;
				}
			}

		textLen = strlen(page[pagePointer].DisplayInfo);
		textLen2 = textLen;
		cPos = strchr(page[pagePointer].DisplayInfo, '*');

		if(cPos != NULL)
			{
			textLen = cPos - page[pagePointer].DisplayInfo;
			}
		memset(tempStr, 0, sizeof(tempStr));

		switch(textLen)
			{
			case 5:
				tempStr[ 0 ] = page[pagePointer].DisplayInfo[0];
				tempStr[ 1 ] = '-';
				for( i=1; i<textLen2; i++ )
					{
					if(page[pagePointer].DisplayInfo[i] == '*')
						tempStr[i + 1] = param.autoFormatStar[0];
					else
						tempStr[i + 1] = page[pagePointer].DisplayInfo[i];
					}

				memset(page[pagePointer].DisplayInfo,
						0,
						sizeof(page[pagePointer].DisplayInfo));

				strcpy(page[pagePointer].DisplayInfo, tempStr);
				break;

			case 6:
				tempStr[0] = page[pagePointer].DisplayInfo[0];
				tempStr[1] = page[pagePointer].DisplayInfo[1];
				tempStr[2] = '-';
				for( i=2; i<textLen2; i++ )
					{
					if(page[pagePointer].DisplayInfo[i] == '*')
						tempStr[i + 1] = param.autoFormatStar[0];
					else
						tempStr[i + 1] = page[pagePointer].DisplayInfo[i];
					}
				memset(page[pagePointer].DisplayInfo,
						0,
						sizeof(page[pagePointer].DisplayInfo));

				strcpy(page[pagePointer].DisplayInfo, tempStr);
				break;

			case 7:
				tempStr[0] = page[pagePointer].DisplayInfo[0];
				tempStr[1] = page[pagePointer].DisplayInfo[1];
				tempStr[2] = page[pagePointer].DisplayInfo[2];
				tempStr[3] = '-';
				for( i=3; i<textLen2; i++ )
					{
					if(page[pagePointer].DisplayInfo[i] == '*')
						tempStr[i + 1] = param.autoFormatStar[0];
					else
						tempStr[i + 1] = page[pagePointer].DisplayInfo[i];
					}
				memset(page[pagePointer].DisplayInfo,
						0,
						sizeof(page[pagePointer].DisplayInfo));

				strcpy( page[ pagePointer ].DisplayInfo, tempStr );
				break;

			case 8:
				tempStr[0] = page[pagePointer].DisplayInfo[0];
				tempStr[1] = '-';
				tempStr[2] = page[pagePointer].DisplayInfo[1];
				tempStr[3] = page[pagePointer].DisplayInfo[2];
				tempStr[4] = page[pagePointer].DisplayInfo[3];
				tempStr[5] = '-';
				for(i = 4; i < textLen2; i++)
					{
					if(page[pagePointer].DisplayInfo[ i ] == '*')
						tempStr[i + 2] = param.autoFormatStar[0];
					else
						tempStr[i + 2] = page[pagePointer].DisplayInfo[i];
					}

				memset(page[pagePointer].DisplayInfo,
						0,
						sizeof(page[pagePointer].DisplayInfo));

				strcpy(page[pagePointer].DisplayInfo, tempStr);
				break;

			case 9:
				tempStr[0] = page[pagePointer].DisplayInfo[0];
				tempStr[1] = '-';
				tempStr[2] = page[pagePointer].DisplayInfo[1];
				tempStr[3] = page[pagePointer].DisplayInfo[2];
				tempStr[4] = page[pagePointer].DisplayInfo[3];
				tempStr[5] = page[pagePointer].DisplayInfo[4];
				tempStr[6] = ' ';
				for( i=5; i<textLen2; i++ ){
					if( page[ pagePointer ].DisplayInfo[ i ] == '*' )
						tempStr[ i + 2 ] = param.autoFormatStar[ 0 ];
					else
						tempStr[ i + 2 ] = page[ pagePointer ].DisplayInfo[ i ];
				}
				memset( page[ pagePointer ].DisplayInfo,
						0,
						sizeof( page[ pagePointer ].DisplayInfo ) );
				strcpy( page[ pagePointer ].DisplayInfo, tempStr );
				break;

			case 10:
				tempStr[ 0 ] = page[ pagePointer ].DisplayInfo[ 0 ];
				tempStr[ 1 ] = page[ pagePointer ].DisplayInfo[ 1 ];
				tempStr[ 2 ] = page[ pagePointer ].DisplayInfo[ 2 ];
				if( param.autoFormatACSpace )
					tempStr[ 3 ] = ' ';
				else
					tempStr[ 3 ] = '-';
				tempStr[ 4 ] = page[ pagePointer ].DisplayInfo[ 3 ];
				tempStr[ 5 ] = page[ pagePointer ].DisplayInfo[ 4 ];
				tempStr[ 6 ] = page[ pagePointer ].DisplayInfo[ 5 ];
				tempStr[ 7 ] = '-';
				for( i=6; i<textLen2; i++ )
					{
					if( page[ pagePointer ].DisplayInfo[ i ] == '*' )
						tempStr[ i + 2 ] = param.autoFormatStar[ 0 ];
					else
						tempStr[ i + 2 ] = page[ pagePointer ].DisplayInfo[ i ];
					}
				memset( page[ pagePointer ].DisplayInfo,
						0,
						sizeof( page[ pagePointer ].DisplayInfo ) );
				strcpy( page[ pagePointer ].DisplayInfo, tempStr );
				break;

			case 11:
				tempStr[ 0 ] = page[ pagePointer ].DisplayInfo[ 0 ];
				if( param.autoFormatACSpace )
					tempStr[ 1 ] = ' ';
				else
					tempStr[ 1 ] = '-';
				tempStr[ 2 ] = page[ pagePointer ].DisplayInfo[ 1 ];
				tempStr[ 3 ] = page[ pagePointer ].DisplayInfo[ 2 ];
				tempStr[ 4 ] = page[ pagePointer ].DisplayInfo[ 3 ];
				if( param.autoFormatACSpace )
					tempStr[ 5 ] = ' ';
				else
					tempStr[ 5 ] = '-';
				tempStr[ 6 ] = page[ pagePointer ].DisplayInfo[ 4 ];
				tempStr[ 7 ] = page[ pagePointer ].DisplayInfo[ 5 ];
				tempStr[ 8 ] = page[ pagePointer ].DisplayInfo[ 6 ];
				tempStr[ 9 ] = '-';
				for( i=7; i<textLen2; i++ )
					{
					if( page[ pagePointer ].DisplayInfo[ i ] == '*' )
						tempStr[ i + 3 ] = param.autoFormatStar[ 0 ];
					else
						tempStr[ i + 3 ] = page[ pagePointer ].DisplayInfo[ i ];
					}
				memset( page[ pagePointer ].DisplayInfo,
						0,
						sizeof( page[ pagePointer ].DisplayInfo ) );
				strcpy( page[ pagePointer ].DisplayInfo, tempStr );
				break;

			default:
				for( i=0; i<textLen2; i++ )
					{
					if( page[ pagePointer ].DisplayInfo[ i ] == '*' )
						tempStr[ i ] = param.autoFormatStar[ 0 ];
					else
						tempStr[ i ] = page[ pagePointer ].DisplayInfo[ i ];
					}
				memset( page[ pagePointer ].DisplayInfo,
						0,
						sizeof( page[ pagePointer ].DisplayInfo ) );
				strcpy( page[ pagePointer ].DisplayInfo, tempStr );
				break;
			}
		page[ pagePointer ].pageFormatted = TRUE;

		PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg, 
						"Autoformat Display: %s", 
						tempStr);
		}
	}


	void
Remove_CrLf( char *string, int StringSize )
	{
	int i;

    for( i=StringSize-1; i>=0; i-- )
		{
        if( string[ i ] == 13 || string[ i ] == 10 || string[ i ] == 0 )
			string[ i ] = 0;
		}
	}


	void
Remove_CtrlChars( char *string, int StringSize )
	{
	int i;

    for( i=0; i<StringSize-1; i++ )
		{
		if( string[ i ] == 0 )
			break;

        if( string[ i ] > 126 || string[ i ] < 32 )
			string[ i ] = 32;
		}
	}

