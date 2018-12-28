#pragma warning( disable: 4786 )
#pragma warning( disable: 4514 )

// for CString class etc.  this needs to be before windows.h
//#include <afx.h>
#include "common.h"

#include <atlstr.h>



#include <fcntl.h>
#include <sys\stat.h>
#include <share.h>
#include <io.h>

#include "list2.h"
//#include <string>
#include <sstream>
//#include <list>
#include <exception>
#include <assert.h>
#include "DataBaseAccessor.h"
#include "Impairment.h"

#include "Registry43.h"

#include "AdoWrap.h"

// temporarily reduce to compiler warning level 3
#pragma warning(push, 3)

// standard template library STL stuff.
#include <vector>
#include <map>
#include <string>

// restore to previous compiler warning level
#pragma warning(pop)



// disable deprecation
#pragma warning(disable:4996)

// xnpgado.cpp



extern class DataBaseAccessor * pDBA;

// workspace data item.
//static LIST_DATA OnCallData = {"Text Info", "", "", 1, 2, 3};

using namespace std;

typedef CAtlArray<CString> CStringArray;

// data items for On-Call processing.

// allocate 1 vector container, and 1 vector iterator.
vector<string> veccntOnCall;
vector<string>::iterator vecitrOnCall;

// allocate 1 map container, and 1 map iterator.
map<string, LIST_DATA> mapcntOnCall;
map<string, LIST_DATA>::iterator mapitrOnCall;


	CString
EscapeSqlText(CString csText)
	{
	// when writing strings that may contain any character,
	//  including the delimiter character for sql strings,
	//  (single quote) the single quote must be escaped,
	//  by passing 2 adjacent single quotes instead to the 
	//  sql destination.  only a single quote will be written
	//  to the data file.
	// 
	// replace occurences of single quote in strings with 
	//  two individual single quote chars.
	//  
	csText.Replace("'", "''");
	return csText;
	}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

	// used only for "Commport Failure".
	void
WriteAlarmLog_CommportFailure(int portNumber, int)
	{
	int msgNumber;
	char msg1[250];
	char msg2[250];

    if(DATA_SOURCE_LOCAL == DataSource || param.SupperExt[0] == 0)
		{
		return;
		}

 	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// when externally controlled, and in the standby mode,
		//  no database can be written to, so just return.
		return;
		}

	memset(msg1, 0, sizeof(msg1));
	memset(msg2, 0, sizeof(msg2));

    PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"1 - Writing alarm message [%s]",
					param.SupperExt);

    strcpy_s<250>(msg1, "ALARM Paging Server ");

	strcat_s<250>(msg1, param.stationId);

	sprintf_s<250>(msg2, "COM%i: Failure", (portNumber + 1));

	msgNumber = WriteMessage(0, param.SupperExt, "D ", msg1, msg2);

	// send action reminder if enabled to do so.
	//
    if(param.CommportFailureAlarm)
		{
		CString csText;
		csText.Format("%-2.2s%10.10s%-15.15d", 
						param.ActionIdForCommportFailure, 
						param.SupperExt,
						msgNumber);

		SetActionReminder(param.SupperExt, 
							param.ActionIdForCommportFailure, 
							msgNumber, 
							REMINDER_ADD, 
							0, 
							csText);
		}
	}


	// used only for "Cancelled Page".
	void
WriteSuppLog_Cancelled(int pgNum, int alarm, char * ext)
	{
	int msgNumber;
	char msg1[1000];
	char msg2[1000];

    if(DATA_SOURCE_LOCAL == DataSource)
		{
		return;
		}

	memset(msg1, 0, sizeof(msg1));
	memset(msg2, 0, sizeof(msg2));

	PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"Leaving error message for %s.",
					ext);

    strcpy_s<1000>(msg1, "CANCELLED PAGE ");
	strcat_s<1000>(msg1, page[pgNum].Extension);
	strcat_s<1000>(msg1, "\r\n" );

    sprintf_s<1000>(msg2,
			"%i attempts, display: %s",
			page[pgNum].attempts,
			page[pgNum].DisplayInfo);

	msgNumber = WriteMessage(pgNum, ext, "D ", msg1, msg2);

    if(alarm && param.CancelledPageAlarm)
		{
		CString csText;
		csText.Format("%-2.2s%10.10s%-15.15d", 
						param.ActionIdForCancelledPage, 
						ext, 
						msgNumber);

		SetActionReminder(ext, 
							param.ActionIdForCancelledPage, 
							msgNumber, 
							REMINDER_ADD, 
							0, 
							csText);
		}
	}


	void
WriteStatLog(int pgNum, int type)
	{
    int i;
	char LogFile[85];
	char msg1[1000];
	char msg2[1000];

    if(DATA_SOURCE_LOCAL == DataSource)
		{
		return;
		}

    memset(LogFile, 0, sizeof(LogFile));

    if(type == 5)
		{
        strcat_s<85>(LogFile, page[pgNum].FileName);

        Set_Null(LogFile, sizeof(LogFile));
		}
    else
		{
        strcpy_s<85>(LogFile, page[pgNum].Extension);
		}

    PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"Leaving Status message for %s.",
					LogFile);

	memset(msg1, 0, sizeof(msg1));
	memset(msg2, 0, sizeof(msg2));

	switch(type)
		{
		case 1:
			if(page[pgNum].FileName[0] != 0)
				{
				strcpy_s<1000>(msg1, "Coverage Change");
				strcpy_s<1000>(msg2, page[pgNum].InitiatorId);
				strcat_s<1000>(msg2, ":");
				}
			else
				{
				strcpy_s<1000>(msg1, "Pager Status Change");
				strcpy_s<1000>(msg2, Status[page[pgNum].ACKMin].Text);
				i = page[pgNum].ACKMin;

				if(i != -1 &&
					(Status[ i ].Page[ 0 ] == 'F' ||
					 Status[ i ].Page[ 0 ] == '#' ||
					 Status[ i ].Page[ 0 ] == '0' ||
					 Status[ i ].Page[ 0 ] == 'V' ||
					 Status[ i ].Page[ 0 ] == 'U'))
					{
					strcat_s<1000>(msg2, " #:");
					strcat_s<1000>(msg2, page[pgNum].DisplayInfo);
					}
				}
			strcat_s<1000>(msg2, "\r\n");
			break;

		case 2:
			sprintf_s<1000>( msg1, "Voice greeting updated." );
			break;
		case 3:
			sprintf_s<1000>( msg1, "Pager change to type %3.3s.", page[ pgNum ].PageType );
			break;
		case 4:
			sprintf_s<1000>( msg1, "Password changed to %s.", page[ pgNum ].Password );
			break;
		case 5:
			sprintf_s<1000>( msg1, "Covering for id number %s.", page[ pgNum ].PagerId );
			break;
		case 6:
			sprintf_s<1000>( msg1, "Voice greeting deleted." );
			break;
		default:
			sprintf_s<1000>( msg1, "Unknown change type." );
			break;
		}

	strcat_s<1000>(msg1, "\r\n");
	WriteMessage(pgNum, LogFile, "S ", msg1, msg2);
	}

    	
	// format the contents for each of 2 lines of text
	//  that constitute a message entry, then call the
	//  WriteMessage() function.
	//
	//      msgType  0     Normal
	//      msgType  1     Orginal ext in covering sequence
	//
	// returns message number of message written.
	//
	// (LPTSTR) (LPCTSTR) EscapeSqlText(page[pagePointer].longMessage.c_str()
	int
WriteUserLog(int pgNum, int msgType, int queuePointer)
	{
	char LogFile[85];
	char msg1[1000];
	char msg2[1000];
	char msgText[1000];

    if(DATA_SOURCE_LOCAL == DataSource)
		{
		return -1;
		}

    memset(LogFile, 0, sizeof(LogFile));

	// destination User LogFile is determined depending on whether
	//  the call is recursive or not.
    if(msgType == 0)
		{
		// non-recursive call.
        strcpy_s<85>(LogFile, page[pgNum].Extension);
		}
    else
		{
		// recursive call.
        strcpy_s<85>(LogFile, page[pgNum].InitialExtension);
		}

	// new logic for sms enabled messages.
	memset(msg1, 0, sizeof(msg1));

	// set a "standard" default message format, 
	//  that may be replaced in the case of a list page.
	//
	sprintf_s<1000>(msg1, "%s", page[pgNum].PagerNumber);

	if(PAGE_LIST_POS == pgNum)
		{
		// case of a list-page.
		//
		if(strlen(page[pgNum].AutCode) > 0)
			{
			// show AutCode if it exists.
			sprintf_s<1000>(msg1,
					"List: [%s] PagerNumber: [%s+%s]",
					ListQue.ListExtension,
					page[pgNum].PagerNumber,
					page[pgNum].AutCode
					);
			}
		else
			{
			// no AutCode defined, don't include it.
			sprintf_s<1000>(msg1,
					"List: [%s] PagerNumber: [%s]",
					ListQue.ListExtension,
					page[pgNum].PagerNumber
					);
			}
		}
	else
		{
		// not a list-page.
		if(strlen(page[pgNum].AutCode) > 0)
			{
			// show AutCode if it exists.
			sprintf_s<1000>(msg1,
					"PagerNumber: [%s+%s]",
					page[pgNum].PagerNumber,
					page[pgNum].AutCode
					);
			}
		else
			{
			// no AutCode defined, don't include it.
			sprintf_s<1000>(msg1,
					"PagerNumber: [%s]",
					page[pgNum].PagerNumber
					);
			}
		}
	// end of new logic.

	// for retrieval later, the "Please Call " text will be recognized
	//  and the numbers placed after that, will be spoken.
	//
	memset(msg2, 0, sizeof(msg2));

	// new logic 2009-04-23 kgill.
	//
	CString csDisplayInfo;
	CString csPureNumeric;
	CString csLongMessage;

	csDisplayInfo = page[pgNum].DisplayInfo;						// convert from character array.
	csPureNumeric = csDisplayInfo.SpanIncluding("0123456789* -");		// create a string only taking numeric content.
	csLongMessage = page[pgNum].longMessage.c_str();				// convert from STL string.

	if( (csDisplayInfo.GetLength() > 0) && 
		(csDisplayInfo.GetLength() == csPureNumeric.GetLength()) )
		{
		// length of the original string is non-zero, and PureNumeric string length is the same as the original string, 
		//  this means that only pure-numeric content was found.

		// format the message with the "Please Call" keyword for 
		//  pure-numeric recognition and read-back later.
		_snprintf_s<1000>(msg2, sizeof(msg2), "Please Call %s.", csPureNumeric);
		}
	else
		{
		// once the data is fixed up the first time, it is not changed any more... kgill 2011-05-23.
		//

		// length is different, non-numeric characters must be in the string.
		//  take the message from longMessage if it is not empty, 
		//  otherwise take the content of DisplayInfo if it is not empty.
		//  if there is no message, write a "No Message." message.
		//
		if(csLongMessage.GetLength() > 0)
			{
			// use longMessage.
			_snprintf_s<1000>(msg2, sizeof(msg2), "%s", (LPCTSTR) csLongMessage);
			}
		else
			{
			if(csDisplayInfo.GetLength() > 0)
				{
				// use DisplayInfo.
				_snprintf_s<1000>(msg2, sizeof(msg2), "%s", (LPCTSTR) csDisplayInfo);
				}
			else
				{
				// no message.
				_snprintf_s<1000>(msg2, sizeof(msg2), "No Message.");
				}
			}
		}

	// what is being accomplished here?, I say ... NOTHING... kgill 2011-05-23.
	//
	// if(page[pgNum].ComNumber != -1 &&
	//	param.ComPort[page[pgNum].ComNumber].QueType[page[pgNum].ComOffset][1] == '1' &&
	//	pgNum == PAGE_LIST_POS)
	//	{
 	//	_snprintf(msg2, sizeof(msg2), "%s.", (LPCTSTR) csDisplayInfo);
	//	}

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"page[pgNum].VoiceFile [%d]", 
					page[pgNum].VoiceFile);

 	// the msg2 text is recognized at a later time,
	//  the exact text should not be changed. - "Voice Page %s"
	if(page[pgNum].VoiceFile > 0)
		{
		_snprintf_s<1000>(msg2, sizeof(msg2), "Voice Page %s", page[pgNum].FileName);
		}

	strncat_s<1000>(msg1, "\r\n", sizeof(msg1) - 2);
	strncat_s<1000>(msg1, msg2, sizeof(msg1) - strlen(msg2));

	memset(msgText, 0, sizeof(msgText));
   

    if(page[pgNum].InitialPagerId[0] != 0)
		{
        if(msgType == 0 &&
			strncmp(page[pgNum].PagerId, page[pgNum].InitialPagerId, 10) != 0)
			{
            _snprintf_s<1000>(msgText, sizeof(msgText), "Covering for:%s", page[pgNum].InitialPagerId);

			strncat_s<1000>(msg1, "\r\n", sizeof(msg1) - 2);
			strncat_s<1000>(msg1, msgText, sizeof(msg1) - strlen(msgText));

			// recursive call.
            WriteUserLog(pgNum, 1, queuePointer);
			}
		}

    if(msgType == 1)
		{
         _snprintf_s<1000>(msgText, sizeof(msgText), "Covered by:%s", page[pgNum].PagerId);

		strncat_s<1000>(msg1, "\r\n", sizeof(msg1) - 2);
		strncat_s<1000>(msg1, msgText, sizeof(msg1) - strlen(msgText));
		}

	memset(msg2, 0, sizeof(msg2));
	strncat_s<1000>(msg1, "\r\n", sizeof(msg1) - 2);

	return WriteMessage(pgNum, LogFile, "PG", msg1, msg2);
	}

	bool 
IsSmsType(int PP)
	{
	if(g_csPageTypesWithSmsFlag.Find(page[PP].PageType) >= 0)
		{
		return true;
		}
	else
		{
		return false;
		}
	}

	//7/1/2016 - SOguz - Added new method with Page Type parameter
	//Used mostly by Xnpgd4x.cpp - ST_ADD_ALL_DVCS_Q state
	bool 
IsSmsType(char * PageType)
	{
	if(g_csPageTypesWithSmsFlag.Find(PageType) >= 0)
		{
		return true;
		}
	else
		{
		return false;
		}
	}
	int
WriteUserLogEx(int PagePointer, int queuePointer)
	{
	if(IsSmsType(PagePointer))
		{
		// The SmsFlag is set, the Xtend MessageBase write has already been written.
		//  nothing to do here...
		return -1;
		}

	// The SmsFlag is NOT set, normal sequence of FlatFile Queue write followed by Xtend MessageBase write.
	//  Do the MessageBase write now.
	return WriteUserLog(PagePointer, 0, queuePointer);
	}


enum {UPDATE_EXISTING_COUNT = 0, CREATE_NEW_RECORD, REPORT_CANT_ACCESS_DATABASE};


	// returns a message number.
	//  this message number can be used when
	//  requesting an action reminder.
	//
int WriteMessage(int PagePointer,
	char *msgExtension,
	char *msgType,
	char *msg1,
	char *msg2)
{
	int MessageNumber = 0;
	SYSTEMTIME CurrTime;
	char tempStr[2];
	char string2[200];
	int iStatus;
	int iRecommendedAction;
	CString csSQLString;
	CString csFunctionName = "WriteMessage";
	CString csTableName;

	// 7/1/2016 - DE20747 - SOguz  
	// Added isPageAllDevicesOn condition to log the skipped SMS pages in failsafe mode 
	if(DATA_SOURCE_LOCAL == DataSource && isPageAllDevicesOn == FALSE)
	{
		// no messages need to be written in failsafe mode.
		return FALSE;
	}

	// Get the highest number, if possible, from the Messages table.
	//
	csTableName = "Messages";
	csSQLString.Format(
		"SELECT MessageNumber "
		"FROM Messages "
		"WHERE ProfileId = '%10.10s' "
		"ORDER BY MessageNumber DESC "
		"--SQL-1-1102",
		msgExtension);


	iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	switch(iStatus)
	{
	case DBA_RECORD_FOUND:
		iRecommendedAction = UPDATE_EXISTING_COUNT;

		// existing record was read.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
			hwndDatabaseMsg,
			"WriteMessage(): DBA_RECORD_FOUND");
		break;

	case DBA_RECORD_NOT_FOUND:
		iRecommendedAction = CREATE_NEW_RECORD;

		// no error reading the table, but no records were found
		//  insert a new one.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
			hwndDatabaseMsg,
			"WriteMessage(): DBA_RECORD_NOT_FOUND");
		break;

	case DBA_RECORD_ACCESS_ERROR:
	default:
		iRecommendedAction = REPORT_CANT_ACCESS_DATABASE;

		// unable to read table.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
			hwndDatabaseMsg,
			"WriteMessage(): DBA_RECORD_ACCESS_ERROR");
		break;
	}

	// prepare the text of the message(s) to be written to
	//  the Xtend "Message Database".
	GetLocalTime(&CurrTime);

	memset(string2, 0, sizeof(string2));
	sprintf_s<200>(string2,
		"%04.4i%02.2i%02.2i%02.2i%02.2i",
		CurrTime.wYear,
		CurrTime.wMonth,
		CurrTime.wDay,
		CurrTime.wHour,
		CurrTime.wMinute);

	memset(tempStr, 0, sizeof(tempStr));

	// if the message originates from a page server IVR request (msgType == "PG"),
	//  and the page server is configured to mark pages as delivered,
	//  and this is a status that has messages being stored (page status option == MSG )
	//  set the delivery field as required.
	//
	// if the message is a status change message, 
	//  it too should be marked as delivered.
	//
	if( ((param.DeliverPage) && 
		(Status[page[PagePointer].ACKMin].Page[0] != 'M') && 
		(strncmp(msgType, "PG", 2) == 0)) ||
		((param.DeliverPage) && 
		(strncmp(msgType, "S ", 2) == 0)) )
	{
		tempStr[0] = '1';
	}
	else
	{
		tempStr[0] = '0';
	}

	// update, add, or unable to write, continue here.
	switch(iRecommendedAction)
	{
	case CREATE_NEW_RECORD:
		// set message number to the "stating" value.
		MessageNumber = 1;
		break;

	case UPDATE_EXISTING_COUNT:
		// retrieve the current data, and increase it:
		pDBA->ADOGetNumeric("MessageNumber", &MessageNumber);

		if(MessageNumber >= 1)
		{
			MessageNumber++;
		}
		else
		{
			// negative, or zero is nonsense... set to the starting value. 
			MessageNumber = 1;
		}
		break;

	case REPORT_CANT_ACCESS_DATABASE:
	default:
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
			hwndDatabaseMsg,
			"WriteMessage(): REPORT_CANT_ACCESS_DATABASE");
		return FALSE;
		break;
	}

	// UPDATE_EXISTING_COUNT, and CREATE_NEW_RECORD will continue here.

	// always write a message to Messages table.
	//  optionally write message lines 1, and 2 to MessageLines table.
	csTableName = "Messages";
	csSQLString.Format(
						"INSERT INTO Messages "
						"(ProfileId, Header, MessageType, MessageNumber, "
						"MessageFrom, Delivered, Operator, "
						"CarbonCopy, OriginMessageNumber) "
						"VALUES ('%10.10s', '%s', '%s', %d, '%s', "
						"'%s', '%s', '%s', %d) "
						"--SQL-1-1205",
						msgExtension,
						string2,
						msgType,
						MessageNumber,
						"XN Paging Server",
						tempStr,
						"XN PS",
						"0",
						0);

	pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
	// if unsuccessful at this point... too bad, 
	//  we can't get stuck because a message couldn't be written...

	// Add the message 'line 1' to the MessageLines table.
	int lineCount = 1;

	string longMessage(msg1);
	longMessage.append(" - ");
	longMessage.append(msg2);

	const int MAX_MSG = 255;
	int msgLength = longMessage.length();
	int start = 0;
	int subLength;

	// depending on the length of the message, multiple records (line numbers) 
	//  may be written to the message base. These all represent the same message. 
	//  Each "line" can only contain a limited number of characters, because of 
	//  the database field width.
	//

    for(subLength = msgLength - start; subLength; subLength = msgLength - start)
		{
		if(subLength > MAX_MSG) subLength = MAX_MSG;

		string msgLine;
		msgLine = longMessage.substr(start, subLength);

		std::string::size_type idx = 0;
		for(idx =0; idx < msgLine.length(); ++idx)
			{
			if(msgLine[idx] == 0x27)
				{
				msgLine.replace(idx, 2, "''");
				++idx;	//skip the extra quote added
				}
			}

		csSQLString.Format(
							"INSERT INTO MessageLines "
							"(ProfileId, MessageNumber, LineNumber, Line) "
							"VALUES ('%10.10s', %d, %d, '%s ') "
							"--SQL-1-1207A",
							msgExtension,
							MessageNumber,
							lineCount,
							static_cast<LPCTSTR>(msgLine.c_str()));
		++lineCount;
		pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);

		start += subLength;
		}

#if 0
	const int  MSG_SIZE = 255;
	char *msgPtr;
	msgPtr = msg2;	

	int messageLength = strlen(msgPtr);
	int subMsgLen = 0;
	int start = 0;
	int last = 0;
	csTableName = "MessageLines";

	while(messageLength)
		{
		start = last;
		if(messageLength > MSG_SIZE)
			{
			subMsgLen = MSG_SIZE;
			messageLength -= MSG_SIZE;
			last += MSG_SIZE;
			}
		else
			{
			subMsgLen = messageLength;
			messageLength = 0;
			last += subMsgLen;
			}

		CString msgLine(&msgPtr[start], subMsgLen);
		msgLine.Replace("'","''");

		csSQLString.Format(
							"INSERT INTO MessageLines "
							"(ProfileId, MessageNumber, LineNumber, Line) "
							"VALUES ('%10.10s', %d, %d, '%s ') "
							"--SQL-1-1207B",
							msgExtension,
							MessageNumber,
							lineCount,
							static_cast<LPCTSTR>(msgLine));
		++lineCount;
		pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
		}
#endif

#if 0
	// Add the message 'line 2' to the MessageLines table.
	if(msg2[0] != 0)
		{
		csTableName = "MessageLines";
		csSQLString.Format(
							"INSERT INTO MessageLines "
							"(ProfileId, MessageNumber, LineNumber, Line) "
							"VALUES ('%10.10s', %d, %d, '%-s ') "
							"--SQL-1-1207C",
							msgExtension,
							MessageNumber,
							2,
							(LPTSTR) (LPCTSTR) EscapeSqlText(msg2));

		pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
		}
#endif

	// one last thing to do, if the message is to have "delivered" status,
	//  write a record to the DeliveredMessages table, but
	//  only if the message is to be "marked" as delivered.
	//
	if('1' == tempStr[0])
		{
		// write a record to DeliveredMessages.
		csTableName = "DeliveredMessages";
		csSQLString.Format(
							"INSERT INTO DeliveredMessages "
							"(Profile, MessageNumber, DeliveryMethod, DateTime, Operator) "
							"VALUES ('%10.10s', %d, '%s', '%s', '%s') "
							"--SQL-1-1209",
							msgExtension,		// Profile
							MessageNumber,		// MessageNumber
							"0 ",				// DeliveryMethod set to "0 ", even though
												//  this is not the pageserver's "signature".
							string2,			// DateTime
							"XN PS");			// Operator							

		pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
		}
	else
		{
		// message NOT to be marked as "delivered", 
		//  no record needs to be written to DeliveredMessages table.
		}

	return MessageNumber;
	}

	// Indicates if any new log numeric messages are available (for today)
	// Returns:
	//          0   message base not available - failsafe mode
	//          1   No New Messages
	//          2   No More Messages
	//          3   Page Server Numeric Message found
	//          4   Still Looking
	//          5   Text Messages dial 0 for operator
	//          6   Voice Message found
	//          7   Text Message
	//          8   XN Numeric message found
	//          9   Status change message
	//          10  Coverage change message

	// some controlling constants for this function.
	//  good test value for MAX_MSGS_TO_CONSIDER is 4, release value is 10
	//  good test value for DATABASE_ACCESS_LIMIT is 10, release value is 30
	#define MAX_MSGS_TO_CONSIDER 10
	#define DATABASE_ACCESS_LIMIT 30

		bool 
	CheckVoiceMsg(const char* test)
		{
		bool found = true;
		char* ptr = voiceMessageTypeStr;
		
		while((*test || *ptr) && found)
			{
			if(*test == 0)
				{
				if(*ptr != ' ')
					found = false;
				else
					ptr++;
				}
			else if(*ptr == 0)
				{
				if(*test != ' ') 
					found = false;
				else
					test++;
				}
			else
				{
				if(*test != *ptr) 
					found = false;
				else
					{
					test++;
					ptr++;
					}
				}
			}
		return found;
		}


	// if any error is encountered,
	//  the process can be ended by
	//  returning status: "no new messages".
	//
	int
GetLogPageMsg(int pgNum, char * TDay)
	{
    int	iReturnValue = MSG_FIND_STILL_LOOKING;
	int	iStatus;
	int	iLastMessageNumber;
	CString csTableName;
	CString csFunctionName = "GetLogPageMsg";
	CString csSQLString;
	int iMessageNumber;
	char header[20];
	char delivered[5];
	char msgType[5];
	char msgline1[101];

	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"");

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): begin - pgNum[%d] TDay[%s]",
					pgNum,
					TDay);

	// when DataSource is LOCAL, this implies failsafe mode of operation,
	//  the messages are not up-to date,
	//  no messages are to be retrieved in "failsafe" mode.
	if(DATA_SOURCE_LOCAL == DataSource)
		{
		iReturnValue = MSG_FIND_MSGS_UNAVAILABLE;
		goto FINAL_DISPOSITION;
		}


	page[pgNum].voiceMsg = false;
	// session initialization,
	//  establish limits on how many messages will be considered.
	if(!page[pgNum].bMessageLimitsDefined)
		{
		// initialize message retrieval limits
		//  for this IVR port and this IVR session.
		page[pgNum].iMessageRetrieveRequest = REQUEST_PLAY_FIRST_MESSAGE;
		page[pgNum].iDatabaseAccessLimit = DATABASE_ACCESS_LIMIT;
		page[pgNum].iMessageSearchDirection = MESSAGE_SEARCH_NEWER_TO_OLDER;
		page[pgNum].iNewestMessageNumber = 0;
		page[pgNum].iOldestMessageNumber = 0;
		page[pgNum].iCurrentMessageNumber = 0;

		// zero indicates no successful message has been
		//  retrieved yet this session,
		//  valid message numbers are positive integers.
		page[pgNum].iLastSuccessfulMessageNumber = 0;

		// the session is now initialized.
		page[pgNum].bMessageLimitsDefined = FALSE;
		}

	// bail out if database access limit has been exceeded.
	//  this is to prevent monopolizing of all system resources
	//  doing message retrieval.
	page[pgNum].iDatabaseAccessLimit--;

	if(page[pgNum].iDatabaseAccessLimit < 0)
		{
		iReturnValue = MSG_FIND_MSGS_UNAVAILABLE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): database access limit exceeded");

		goto FINAL_DISPOSITION;
		}

	// if not already set, set the message number retrieval limits
	//  for this IVR session.
	if(!page[pgNum].bMessageLimitsDefined)
		{
		// prevent re-entry into the initialization process.
		page[pgNum].bMessageLimitsDefined = TRUE;

		// set default search direction.
		page[pgNum].iMessageSearchDirection = MESSAGE_SEARCH_NEWER_TO_OLDER;

		csTableName = "Messages";
		csSQLString.Format(
							"SELECT MessageNumber "
							"FROM Messages "
							"WHERE ProfileId = '%10.10s' "
							"ORDER BY MessageNumber DESC "
							"--SQL-1-1103",
							page[pgNum].Extension);

		iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

		if(DBA_RECORD_FOUND == iStatus)
			{
			// successful sql select, and at least one record found in the recordset.
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): DBA_RECORD_FOUND");

			// pDBA->ADOGetNumeric("LastMessageNumber", &iLastMessageNumber);
			pDBA->ADOGetNumeric("MessageNumber", &iLastMessageNumber);
			// need some error checking here.

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndDatabaseMsg,
							"Read message search key: [%10.10s], Msg #: [%i], return [%i]",
							page[pgNum].Extension,
							iLastMessageNumber,
							iStatus);

			if(iLastMessageNumber > 0)
				{
				// the message count is taken as the highest numbered
				//  message, and also as the current message number.
				page[pgNum].iNewestMessageNumber = iLastMessageNumber;
				page[pgNum].iCurrentMessageNumber = iLastMessageNumber;
				}
			else
				{
				// message count is zero, or less than zero,
				//  this is unusual, but possible, and handled here.
				iReturnValue = MSG_FIND_MSGS_UNAVAILABLE;
				goto FINAL_DISPOSITION;
				}

			// once the iCurrentMessageNumber is non-zero,
			//  at least one message has been identified, and
			//  is expected to be available.

			// we will consider a maximum of MAX_MSGS_TO_CONSIDER messages.
			//  now we establish the oldest message number to
			//  consider, as there must be a practical limit.

			// to deliver the maximum number of messages,
			//  use the following calculation to determine
			//  the oldest message number to consider.
			page[pgNum].iOldestMessageNumber = (page[pgNum].iNewestMessageNumber -
												(MAX_MSGS_TO_CONSIDER - 1));

			if(page[pgNum].iOldestMessageNumber < 1)
				{
				// when the standard calculation to determine the lowest
				//  message number to retrieve yields a number that is less than 1,
				//  this is a problem. message numbers are positive non-zero integers.
				// in this case, simply use 1 for the lowest message number.
				page[pgNum].iOldestMessageNumber = 1;
				}

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): IVR session setup");

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): "
							"iNewestMessageNumber [%d] "
							"iOldestMessageNumber [%d] "
							"iCurrentMessageNumber [%d] ",
							page[pgNum].iNewestMessageNumber,
							page[pgNum].iOldestMessageNumber,
							page[pgNum].iCurrentMessageNumber);

			// concrete logic example:
			//  using MAX_MSGS_TO_CONSIDER == 4
			//  oldest message to consider is:
			//   OldestMessage = (NewestMessage - (MAX_MSGS_TO_CONSIDER - 1))
			//
			//
			//  MessageCount   NewestMessage    (Newest - 4)    OldestMessage to use is:  (Newest - (4 - 1))
			//		0				--   		    --				--
			//
			//		1				1				-3				1   <---  when (newest - 4) is less than 1.
			//		2				2				-2				1		  simply use 1 as the lowest message #
			//		3				3				-1				1
			//		4				4				 0				1	<---  when (newest - 4) is:
			//		5				5				 1				2	      greater than zero it is
			//		6				6				 2				3		  ok to use difference
			//		7				7				 3				4         calculation to determine
			//																  the oldest message number
			//																  to consider.
			}
		else
			{
			// the MessageCount table could not be accessed
			//  due to no record has been written for this profile
			//  and there is no record,
			//  or there was a database access error.
			//  either way, no messages are going to be
			//  available this IVR session.
			// report no messages
			iReturnValue = MSG_FIND_MSGS_UNAVAILABLE;
			goto FINAL_DISPOSITION;
			}
		}

	// select a message number based on message retrieve request
	switch(page[pgNum].iMessageRetrieveRequest)
		{
		case REQUEST_PLAY_OLDER_MESSAGE:
			// decrease the current message number.
			page[pgNum].iCurrentMessageNumber--;

			// but keep it within limits.
			if(page[pgNum].iCurrentMessageNumber < page[pgNum].iOldestMessageNumber)
				{
				// decreased too much, put it back to
				//  known maximum message number.
				page[pgNum].iCurrentMessageNumber = page[pgNum].iOldestMessageNumber;

				// attempted to go out of range,
				//  disposition is now "no more records".
				iReturnValue = MSG_FIND_NO_MORE_MSGS;
				goto FINAL_DISPOSITION;
				}

			// also set search direction.
			page[pgNum].iMessageSearchDirection = MESSAGE_SEARCH_NEWER_TO_OLDER;
			break;

		case REQUEST_REPEAT_MESSAGE:
			// assign the most recent successful message number,
			//  if one is available.
			if(0 != page[pgNum].iLastSuccessfulMessageNumber)
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"GetLogPageMsg(): \"repeat message\" requested, message number is [%d]",
								page[pgNum].iLastSuccessfulMessageNumber);

				page[pgNum].iCurrentMessageNumber = page[pgNum].iLastSuccessfulMessageNumber;
				}
			else
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"GetLogPageMsg(): \"repeat message\" requested, but none is known");
				}
			break;

		case REQUEST_PLAY_FIRST_MESSAGE:
			// nothing to do but get the message.
			break;

		case REQUEST_PLAY_NEWER_MESSAGE:
			// increase the current message number.
			page[pgNum].iCurrentMessageNumber++;

			// but keep it within limits.
			if(page[pgNum].iCurrentMessageNumber > page[pgNum].iNewestMessageNumber)
				{
				// increased too much, put it back to
				//  known minimum message number.
				page[pgNum].iCurrentMessageNumber = page[pgNum].iNewestMessageNumber;

				// attempted to go out of range,
				//  disposition is now "no more records".
				iReturnValue = MSG_FIND_NO_MORE_MSGS;
				goto FINAL_DISPOSITION;
				}

			// also set search direction.
			page[pgNum].iMessageSearchDirection = MESSAGE_SEARCH_OLDER_TO_NEWER;
			break;

		case REQUEST_PLAY_NEXT_MESSAGE:
			// this request may come up when looking for qualified
			//  messages ("undelivered or all" or "today's messages since midnight").
			//  the question to be answered is: in the search for a qualifying message,
			//  shall I increase, or decrease the message number?  the answer must be
			//  based on the current search direction, which may have changed based on user input.
			//
			if(MESSAGE_SEARCH_NEWER_TO_OLDER == page[pgNum].iMessageSearchDirection)
				{
				// searching in the newer to older direction.
				//  decrease the current message number.
				page[pgNum].iCurrentMessageNumber--;

				// but keep it within limits.
				if(page[pgNum].iCurrentMessageNumber < page[pgNum].iOldestMessageNumber)
					{
					// decreased too much, put it back to
					//  known maximum message number.
					page[pgNum].iCurrentMessageNumber = page[pgNum].iOldestMessageNumber;

					// attempted to go out of range,
					//  disposition is now "no more records".
					iReturnValue = MSG_FIND_NO_MORE_MSGS;
					goto FINAL_DISPOSITION;
					}
				}
			else
				{
				// searching in the older to newer direction.
				//  increase the current message number.
				page[pgNum].iCurrentMessageNumber++;

				// but keep it within limits.
				if(page[pgNum].iCurrentMessageNumber > page[pgNum].iNewestMessageNumber)
					{
					// increased too much, put it back to
					//  known minimum message number.
					page[pgNum].iCurrentMessageNumber = page[pgNum].iNewestMessageNumber;

					// attempted to go out of range,
					//  disposition is now "no more records".
					iReturnValue = MSG_FIND_NO_MORE_MSGS;
					goto FINAL_DISPOSITION;
					}
				}
			// leave search direction untouched.
			break;
		}

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): updated message number [%d]",
					page[pgNum].iCurrentMessageNumber);

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): user request [%d]",
					page[pgNum].iMessageRetrieveRequest);

	// attempt to retrieve Messages table data item for
	//  current message number.
	//  the message number range is specified,
	//  so you may not get the message number requested,
	//  but if a message is returned, it will be in-range.

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): retrieve current message: page[pgNum].iCurrentMessageNumber:[%d]",
					page[pgNum].iCurrentMessageNumber);

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): page[pgNum].iMessageRetrieveRequest:[%d]",
					page[pgNum].iMessageRetrieveRequest);

	// a newer or older message was requested.
	//  search higher or lower based on the most recent request
	//  higher or lower also implies the search order.
	//
	if(MESSAGE_SEARCH_NEWER_TO_OLDER == page[pgNum].iMessageSearchDirection)
		{
		// looking from newer to older messages.
		//  search from current message toward older messages
		//  and use descending search order.
		csTableName = "Messages";
		csSQLString.Format(
							"SELECT ProfileId, Header, MessageType, MessageNumber, Delivered "
							"FROM Messages "
							"WHERE ProfileId = '%10.10s' "
							"AND MessageNumber <= %d "
							"AND MessageNumber >= %d "
							"ORDER BY ProfileId, MessageNumber DESC "
							"--SQL-1-1105",
							page[pgNum].Extension,
							page[pgNum].iCurrentMessageNumber,
							page[pgNum].iOldestMessageNumber
							);
		}
	else
		{
		// looking from older to newer messages,
		//  search from current message toward newer messages
		//  and use ascending search order.
		csTableName = "Messages";
		csSQLString.Format(
							"SELECT ProfileId, Header, MessageType, MessageNumber, Delivered "
							"FROM Messages "
							"WHERE ProfileId = '%10.10s' "
							"AND MessageNumber >= %d "
							"AND MessageNumber <= %d "
							"ORDER BY ProfileId, MessageNumber ASC "
							"--SQL-1-1106",
							page[pgNum].Extension,
							page[pgNum].iCurrentMessageNumber,
							page[pgNum].iNewestMessageNumber
							);
		}

	// sql statement 1105 or 1106 is defined.
	iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	if(DBA_RECORD_FOUND == iStatus)
		{
		pDBA->ADOGetNumeric("MessageNumber", &iMessageNumber);

		// latch on to the message number that was found.
		//  it may be less than the one that was requested.
		//  due to non-contiguous message numbers.
		//  (this could be as a result of a message purge operation)
		page[pgNum].iCurrentMessageNumber = iMessageNumber;

		memset(header, 0, sizeof(header));
		pDBA->ADOGetString("Header", header, 12);

		memset(delivered, 0, sizeof(delivered));
		pDBA->ADOGetString("Delivered", delivered, 1);

		memset(msgType, 0, sizeof(msgType));
		pDBA->ADOGetString("MessageType", msgType, 2);

		// need some error checking here.
		}
	else
		{
		// record not found, or
		//  error accessing Messages table.
		//  in either case, report "no messages"
		iReturnValue = MSG_FIND_MSGS_UNAVAILABLE;
		goto FINAL_DISPOSITION;
		}

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): retrieving text for message number [%d]",
					page[pgNum].iCurrentMessageNumber);

	// Read message from MessageLines table.

	// first, clear out the destination buffer.
	memset(msgline1, 0, sizeof(msgline1));

	csTableName = "MessageLines";
	csSQLString.Format(
						"SELECT * "
						"FROM MessageLines "
						"WHERE ProfileId = '%10.10s' "
						"AND MessageNumber = %d "
						"AND LineNumber = 1 "
						"--SQL-1-1107",
						page[pgNum].Extension,
						page[pgNum].iCurrentMessageNumber);

	iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	if(DBA_RECORD_FOUND == iStatus)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): MessageLines table read successfully, MessageNumber [%d]",
						page[pgNum].iCurrentMessageNumber);

		pDBA->ADOGetString("Line", msgline1, 100);
		}
	else
		{
		iReturnValue = MSG_FIND_MSGS_UNAVAILABLE;
		goto FINAL_DISPOSITION;
		}

	// qualify messages found in the message database.
	//
	//  by date -
	//    if the "since midnight" option is enabled,
	//    return only messages that have dates matching the
	//    current date. month and day only are compared.
	//
	//  by attribute delivered/undelivered -
	//    the session variable xxx may be set to select
	//    all messages or only undelivered messages.
	//

	// there are several "types".
	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): begin - message qualification");

	//	qualify by month and day.
	//
	// Status Line Options tab of Page Server Config Utility
	//  checkbox labeled "Message Retrieve Prompts" controls
	//  variable param.MessageRtrOpt, when this option is
	//  TRUE, the caller is using IVR commands
	//  previous, current, and next to control message delivery.
	//  when it is FALSE, all messages matching today's month and day
	//  are delivered sequentially without user input.
	//

	// the search ends if the date does not match,
	//  and the "user IVR controlled" option is FALSE.
	//  if the search is not ended here, further
	//  qualification may still be required.
	//  (e.g. delivered/undelivered).
	if((strncmp(header + 4, TDay, 4) != 0) && (param.MessageRtrOpt == FALSE))
		{
		// a message that does not match today's date has been found
		//  since we are looking from highest to lowest message numbers
		//  this is the first non-today's date message.
		//  the search is ended.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): this is not a \"today's\" message.");

		iReturnValue = MSG_FIND_NO_MSGS;
		goto FINAL_DISPOSITION;
		}

	// date qualification is complete,
	//  either the message is for "today",
	//  or we are retrieving "all" messages.
	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): date qualification complete.");

	// qualify the message as delivered/undelivered.
	//  this is only necessary if the caller has requested
	//  only "undelivered" messages.
	//
	//  Joseph re-uses this variable in several ways, in this case
	//   the variable name has nothing to do with its purpose.
	//  information in this variable was filled in ST_STAT_M_GET
	//   where the question ST_STAT_M_MSG "undelivered vs all" messages was asked.
	//   page[pgNum].VoiceFile == 1 indicates that the user pressed '1'
	//    and Undelivered messages are requested.
	//   page[pgNum].VoiceFile == 2 indicates that the user pressed '2'
	//    and all messages are requested.
	//
	if((delivered[0] == '1') &&
		(page[pgNum].VoiceFile == 1))
		{
		// this message is marked as "delivered",
		//  and we are only retrieving un-delivered messages.
		//  keep looking, eventually the limited number of
		//  data accesses or message number limits will be violated,
		//  and the search will end that way.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): delivered message found, keep searching.");

		iReturnValue = MSG_FIND_STILL_LOOKING;
		goto FINAL_DISPOSITION;
		}

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): delivered/undelivered qualification complete.");

	// message (type 'K') - exact text Keywords, located at the beginning of
	//  the message text will be recognized for status change,
	//  and coverage change.
	//
	if(strncmp(msgline1, "Pager Status Change", 19) == 0)
		{
		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);
		CopyLogPageMsg(pgNum, header, msgline1);
		iReturnValue = MSG_FIND_STATUS_CHANGE_MSG;
		goto FINAL_DISPOSITION;
		}

	if(strncmp(msgline1, "Status Change to :", 18) == 0)
		{
		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);
		CopyLogPageMsg(pgNum, header, msgline1);
		iReturnValue = MSG_FIND_STATUS_CHANGE_MSG;
		goto FINAL_DISPOSITION;
		}

	if(strncmp(msgline1, "Parking Gate Status Change", 26) == 0)
		{
		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);
		CopyLogPageMsg(pgNum, header, msgline1);
		iReturnValue = MSG_FIND_STATUS_CHANGE_MSG;
		goto FINAL_DISPOSITION;
		}

	if(strncmp(msgline1, "Coverage Change", 15) == 0)
		{
		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);
		CopyLogPageMsg(pgNum, header, msgline1);
		iReturnValue = MSG_FIND_COVERAGE_CHANGE_MSG;
		goto FINAL_DISPOSITION;
		}

	// message (type 'N') - Numeric messages.
	//
	// source of this message is via dial-in
	//   the keywords "Please Call " preceed the message number
	//   users will not be interested in the modem number.
	//
	// looking within the text, not only at the very beginning,
	//  numeric messages are expected to have the "Please Call "
	//  message text, followed immediately by numeric message digits.
	//
	{
	// some working variables...
	int n;
	CString csCompleteMessage; csCompleteMessage = msgline1;
	
	CString csNumericMessage = "";
	CString csKeyword = "Please Call ";
	n = csCompleteMessage.Find(csKeyword);

	if(n >= 0)	// n will be -1 if the substring is not found.
		{
		// recognized a "valid" keyword message. 

		// extract the characters to the right of the keyword.
		//  start at the point at which the keyword text was found,
		//  add the length of the keyword text, this is the 
		//  first possible starting point of the numeric message.
		//
		csNumericMessage = csCompleteMessage.Mid(n + csKeyword.GetLength());

		// the fixed up message will have the standard keyword, "Please Call ",
		//  instead of the recognized keyword that indicates the same thing,
		//  a numeric message, this text is expected further down the road,
		//  this is sloppy programming, but it needs to be in the fixed 
		//  format (12 chars) to be properly recognized by the IVR routine(s).
		//
		CString csFixedUpMessage; csFixedUpMessage = "Please Call " + csNumericMessage;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): found keyword text: [%s].", 
						csKeyword);

		CopyLogPageMsg(pgNum, header, (LPSTR) (LPCSTR) csFixedUpMessage);
		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);

		iReturnValue = MSG_FIND_NUMERIC_MSG_FOUND;
		goto FINAL_DISPOSITION;
		}

	// Xtend WebXchange leaves this type of keyword for numeric messages.

	// clear the working variables...
	csKeyword = "Message-N: ";
	csNumericMessage = "";
	n = csCompleteMessage.Find(csKeyword);

	if(n >= 0)	// n will be -1 if the substring is not found.
		{
		// recognized a "valid" keyword message. 

		csNumericMessage = csCompleteMessage.Mid(n + csKeyword.GetLength());

		CString csFixedUpMessage; csFixedUpMessage = "Please Call " + csNumericMessage;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): found keyword text: [%s].", 
						csKeyword);

		CopyLogPageMsg(pgNum, header, (LPSTR) (LPCSTR) csFixedUpMessage);
		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);

		iReturnValue = MSG_FIND_NUMERIC_MSG_FOUND;
		goto FINAL_DISPOSITION;
		}

	// Xtend WebXchange leaves this type of keyword for Text messages.

	// clear the working variables...
	csKeyword = "Message-A: ";
	n = csCompleteMessage.Find(csKeyword);

	if(n >= 0)	// n will be -1 if the substring is not found.
		{
		// recognized a "valid" keyword message. 

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): found keyword text: [%s].",
						csKeyword);

		// WebXchange identifies this as a text message, but if there are digits
		//  in the first position, pageserver IVR can recite them, indicate
		//  that this is a NUMERIC message (for the purpose of IVR message retrieval purposes).

		// grab the text after the keyword.
		csNumericMessage = csCompleteMessage.Mid(n + csKeyword.GetLength());

		// examine the first character of the message
		if((csNumericMessage[0] >= '0') && (csNumericMessage[0] <= '9'))
			{
			// numeric was found... (0 thru 9).
			//
			CString csFixedUpMessage; csFixedUpMessage = "Please Call " + csNumericMessage;

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): found keyword text: [%s].", 
							csKeyword);

			CopyLogPageMsg(pgNum, header, (LPSTR) (LPCSTR) csFixedUpMessage);
			MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);

			iReturnValue = MSG_FIND_NUMERIC_MSG_FOUND;
			}
		else
			{
			// no numeric content at the beginning...

			//  report that a "text message" was found.
			iReturnValue = MSG_FIND_DIAL_0_TEXT_MSG;
			}

		goto FINAL_DISPOSITION;
		}
	}

	// message (type 'P') - Pageserver type numeric messages.
	//
	//  these are numeric messages without the "Please Call "
	//  text, for a numeric message to a numeric pager,
	//  the page server will not insert text into this message.
	//
	//  these are messages of type "PG",
	//  that have numeric content, at least the first character
	//  being numeric, can be delivered via IVR.
	//  the search sequence is important, you must look for other
	//  keywords that may be within the message, before
	//  this method is used.
	//
	if(strncmp(msgType, "PG", 2) == 0)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): msgType == \"PG\" ");

		// look for the text "Paged at number", take all text
		//  preceeding this keyword text, and the first '+' char
		//  or to the beginning of the string. as the message.
		char * pKeywordText = strstr(msgline1, "Paged at number");
		char * pPlusText = strstr(msgline1, "+");

		if(pKeywordText && pPlusText)
			{
			// both KeywordText, and PlusText found.
			//  the numeric message could begin just after the 
			//  '+' character.
			//
			// use: pPlusText plus one.
			pPlusText++;

			if((*pPlusText >= '0') && (*pPlusText <= '9'))
				{
				// message character is in the range of '0' thru '9'

				// the numeric message is expected to be preceded by the text
				//  "Please Call ". this is how the IVR expects to find it.
				//
				char Temp[12 + 40 + 1];
				ZeroMemory(Temp, sizeof(Temp));
				sprintf_s<53>(Temp, "Please Call %-40s", pPlusText);

				CopyLogPageMsg(pgNum, header, Temp);
				MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);

				iReturnValue = MSG_FIND_NUMERIC_MSG_FOUND;

				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"plus and keyword, and numeric found");

				goto FINAL_DISPOSITION;
				}
			else
				{
				// a non-numeric char was found.
				//  report that a "text message" was found.
				iReturnValue = MSG_FIND_DIAL_0_TEXT_MSG;

				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"plus and keyword, and NON-numeric found - MSG_FIND_DIAL_0_TEXT_MSG");

				goto FINAL_DISPOSITION;
				}
			}
		else if(pKeywordText)
			{
			// KeywordText found, but no PlusText found.
			//  the numeric message could start at 
			//  the beginning of the message.
			//
			// use the first char of msgline1.

			if((*msgline1 >= '0') && (*msgline1 <= '9'))
				{
				// message character is in the range of '0' thru '9'

				// the numeric message is expected to be preceded by the text
				//  "Please Call ". this is how the IVR expects to find it.
				//
				char Temp[12 + 40 + 1];
				ZeroMemory(Temp, sizeof(Temp));
				sprintf_s<53>(Temp, "Please Call %-40s", msgline1);

				CopyLogPageMsg(pgNum, header, Temp);
				MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);

				iReturnValue = MSG_FIND_NUMERIC_MSG_FOUND;

				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"keyword (no plus), and numeric found");

				goto FINAL_DISPOSITION;
				}
			else
				{
				// a non-numeric char was found.
				//  report that a "text message" was found.
				iReturnValue = MSG_FIND_DIAL_0_TEXT_MSG;

				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"keyword (no plus), and NON-numeric found - MSG_FIND_DIAL_0_TEXT_MSG");

				goto FINAL_DISPOSITION;
				}
			}
		}

	// message (type 'V') - Voice messages.
	//
	//  when param.VoiceDelete is FALSE, voice messages are being
	//  kept for IVR retrieval, if the message is shown as
	//  "Voice Page", the name of the voice file can be extracted
	//  from within the message. the filename is then returned in
	//  page[pgNum].FileName, and a voice message has been found.
	//
	if(param.VoiceDelete != TRUE)
		{
		char * pVoicePage;

		pVoicePage = strstr(msgline1, "Voice Page ");

		if(pVoicePage != NULL)
			{
			// found "Voice Page " text.
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): found 'Voice Page ' text");

			// prepare the filename
			memset(page[pgNum].FileName, 0, sizeof(page[pgNum].FileName));

			// contains a text "purifying" call to setnull();
			CopyLogPageMsg(pgNum, header, pVoicePage);

			// index to, and then skip over the text "Voice Page ".
			strcpy_s<120>(page[pgNum].FileName, pVoicePage + 11);

			CopyLogPageMsg(pgNum, header, pVoicePage);
			MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): voice filename to use: [%s]",
							page[pgNum].FileName);

			iReturnValue = MSG_FIND_VOICE_MSG;
			goto FINAL_DISPOSITION;
			}
		}

	// message (type 'A') - Abbreviated "talk page"
	//  one letter abbreviations within the text of a voice / talk page
	//  are accepted as numeric pages.
	// these are also known as Abbreviated Voice Pages.
	//  for further explanation, refer to the page server document "config.htm"
	//
	if((iReturnValue == MSG_FIND_NO_MSGS) && param.VoiceConv)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"GetLogPageMsg(): Voice Conversion Option");

		// check that all characters of the current message are members of the following list,
		//  if they are, then the "message" can be translated, and spoken in the
		//  limited vocabulary of the "voice conversion" page server feature.
		//	this section checks to see if the message can qualify for translation,
		//  if qualified, indicate that this is a "numeric" message.
		//  if not qualified, then keep searching.

		// first check for an empty message.
		int iLength = (int) strlen(msgline1);
		int j;

		if(iLength > 0)
			{
			// empty message encountered,
			//  continue searching.
			iReturnValue = MSG_FIND_STILL_LOOKING;
			goto FINAL_DISPOSITION;
			}

		for(j = 0; j < iLength; j++)
			{
			if((msgline1[j] < '0') || (msgline1[j] > '9'))
				{
				// only non-numeric characters.
				switch(msgline1[j])
					{
					case ' ':		// ???
					case 'B':		// "back"
					case 'E':		// "emergency"
					case 'N':		// "number"
					case 'R':		// "operating room"
					case 'S':		// "stat"
					case 'X':		//  same as case '*' below.
					case 'P':		// "page"
					case 'O':		// "operator"
					case 'C':		// "call"
					case 'H':		// "home"
					case '*':		// programmable, but typically "extension" or "star"
						//
						// character is ok for "translation",
						//  and keep looking.
						break;

					default:
						// a non-translatable character has been found,
						//  this message is disqualified, but go ahead
						//  and keep searching for other messages.
						iReturnValue = MSG_FIND_STILL_LOOKING;
						goto FINAL_DISPOSITION;
						break;
					}
				}
			}

		// the message qualifies as a "translatable" numeric message.
		iReturnValue = MSG_FIND_XN_NUMERIC_MSG;
		goto FINAL_DISPOSITION;
		}


	
	//jph add message check
	//if(strncmp(msgType, voiceMessageTypeStr, 2) == 0)
	if(CheckVoiceMsg(msgType))
		{
		char Temp=0;
	
		//set voice message flag, copy message type
		page[pgNum].voiceMsg = true;
		CopyLogPageMsg(pgNum, header, &Temp);

		//stick voice file somewhere
		ostringstream vFile;
		string profileId(page[pgNum].Extension);

		vFile	<< pageFileDir << GetLookupDir(profileId) << profileId << '\\' 
				<< page[pgNum].iCurrentMessageNumber << ".wav" << ends;

		strncpy_s<120>(page[pgNum].FileName, vFile.str().c_str(), sizeof(page[pgNum].FileName));
		//you can never be too sure, so null terminate
		page[pgNum].FileName[sizeof(page[pgNum].FileName) - 1] = 0;

		MarkLogPageMsg(page[pgNum].iCurrentMessageNumber, page[pgNum].Extension);
		
		iReturnValue = MSG_FIND_VOICE_MSG;
		goto FINAL_DISPOSITION;		
		}
	
	
	// at this point, no messages qualify,
	//  or no messages have a "recognized format"
	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): end   - message qualification");

	// removed this... 2004-10-22 kgill.
	//  any unknown message type would terminate the search.
	//	iReturnValue = MSG_FIND_NO_MSGS;

	goto FINAL_DISPOSITION;


	FINAL_DISPOSITION:	// label for goto statement.

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): begin - FINAL_DISPOSITION");

	// capture the most recent message retrieval "success".
	//  this information can be re-used if the caller
	//  requests "repeat current message", and a successful
	//  outcome has been experienced sometime
	//  during this IVR session.
	//
	switch(iReturnValue)
		{
		case MSG_FIND_NUMERIC_MSG_FOUND:
		case MSG_FIND_DIAL_0_TEXT_MSG:
		case MSG_FIND_VOICE_MSG:
		case MSG_FIND_TEXT_MSG:
		case MSG_FIND_XN_NUMERIC_MSG:
		case MSG_FIND_STATUS_CHANGE_MSG:
		case MSG_FIND_COVERAGE_CHANGE_MSG:
			// any case of successful message retrieval.
			//  capture the message number for successful outcomes.
			page[pgNum].iLastSuccessfulMessageNumber = page[pgNum].iCurrentMessageNumber;

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"GetLogPageMsg(): repeatable message found.");
			break;

		case MSG_FIND_STILL_LOOKING:
			// the search continues...
			break;

		case MSG_FIND_MSGS_UNAVAILABLE:
 		case MSG_FIND_NO_MSGS:
		case MSG_FIND_NO_MORE_MSGS:
		default:
			// unsuccessful search has been determined.
			//
			// one final politeness is implemented here.
			//  if at sometime during the session a message has been found,
			//  then subsequent message search has failed, a message stating
			//  "no MORE messages", rather than "NO messages" is more appropriate.
			//  the determining factor of whether a message has ever been successfully
			//  found during this IVR session is the value of the variable
			//  page[pgNum].iLastSuccessfulMessageNumber being greater than zero,
			//  indicating a valid message number.
			//
			if(page[pgNum].iLastSuccessfulMessageNumber > 0)
				{
				// say that there are "no MORE messages"
				iReturnValue = MSG_FIND_NO_MORE_MSGS;
				}
			else
				{
				// instead of just "NO messages"
				iReturnValue = MSG_FIND_NO_MSGS;
				}
			break;
		}

	// after completing the first or subsequent iteration,
	//  the user request has been satisfied.
	//  the request is set to REQUEST_PLAY_NEXT_MESSAGE to allow
	//  message cycling, without further user input, play-next
	//  continues on, in the currently set search direction.
	page[pgNum].iMessageRetrieveRequest = REQUEST_PLAY_NEXT_MESSAGE;

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"GetLogPageMsg(): 6 - returning [%s] iCurrentMessageNumber [%d]",
					MessageRetrieveDispositionText[iReturnValue],
					page[pgNum].iCurrentMessageNumber);

	// now, finally, return the disposition.
	return iReturnValue;
	}


	// prepare numeric message information so that it
	//  can be delivered via IVR.
	//
	// write to the DisplayInfo structure member.
	// first 9 characters are the time and date in
	//  in the "format 1" date format:
	//  month, day, hour, minute, <space>, units digit of the year.
	//   0123456789012345678901234567890123456789
	//  "MMDDHHmm Y"
	//  followed by up to 130 characters of the message line.
	//
	// notes:
	//  the parameter "time" is of the form:
	//  YYYYMMDDHHmm as retrieved from Messages table, Header field.
	//
	//  the messageBuf is expected to contain a message of the example form:
	//   "Please call 555-1212."
	//   the first 12 chars are skipped "Please Call " is 12 chars in length.
	//   the number 555-1212 will be spoken via IVR.
	//
	void
CopyLogPageMsg(int pgNum, char * time, char * messageBuf)
	{
    //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
	//sprintf_s<255>(page[pgNum].DisplayInfo,
	//			"%8.8s %1.1s %-130.130s",
	//			time+4,		// MMDDHHmm  (8 chars)
	//			time+3,		// unit year number, for 2002 == 2 (1 char)
	//			messageBuf	// up to 130 chars.
	//			);
    sprintf(page[pgNum].DisplayInfo,
				"%8.8s %1.1s %-130.130s",
				time+4,		// MMDDHHmm  (8 chars)
				time+3,		// unit year number, for 2002 == 2 (1 char)
				messageBuf	// up to 130 chars.
				);

	Set_Null(page[pgNum].DisplayInfo, sizeof(page[pgNum].DisplayInfo));
	}


	void
MarkLogPageMsg(int msgNumber, char *extension)
	{
	int status;
	SYSTEMTIME CurrTime;
	CString csSQLString = "";
	CString csFunctionName = "MarkLogPageMsg";
	CString csTableName = "";

	PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg, 
				"MarkLogPageMsg() - begin");

	csTableName = "Messages";
	csSQLString.Format(
						"UPDATE Messages "
						"SET Delivered = '1' "
						"WHERE ProfileId = '%10.10s' "
						"AND MessageNumber = %d "
						"--SQL-1-1302",
						extension,
						msgNumber);


	status = pDBA->ADORsUpdate(csFunctionName, csTableName, csSQLString);

	if(SUCCESS == status)
		{
		GetLocalTime(&CurrTime);

		csTableName = "DeliveredMessages";
		csSQLString.Format(
							"INSERT INTO DeliveredMessages "
							"(Profile, MessageNumber, DeliveryMethod, DateTime, Operator) "
							"VALUES ('%10.10s', %d, '%s', '%04.4i%02.2i%02.2i%02.2i%02.2i', '%s') "
							"--SQL-1-1208",
							extension,			// Profile
							msgNumber,			// MessageNumber
							"0 ",				// DeliveryMethod "0 ", not pageserver's signature.
							CurrTime.wYear,		// DateTime
							CurrTime.wMonth,
							CurrTime.wDay,
							CurrTime.wHour,
							CurrTime.wMinute,
							"XN PS");			// Operator

		status = pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);

		if(SUCCESS != status)
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg,
							"Unable to deliver message, return [%i]",
							status);

			// each time a message is retrieved, an attempt is made
			//  to write to "DeliveredMessages" table, if the user is
			//  stepping forward, and back repeatedly via IVR, this constitutes many
			//  attempts to deliver the message, the file has a unique key
			//  requirement based on date and time, multiple attempts to deliver
			//  within 1 minute will not be unique (date information is the same),
			//  and subsequently will not be successful.
			//  is this is a subtle problem?, or not really a problem?
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"MarkLogPageMsg(): unable to insert, record must be unique");
			}
		}
	else
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"MarkLogPageMsg(): unable to update");
		}

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"MarkLogPageMsg() - end");
	}


	//
	// returns TRUE if the QueString is listed as a "voice mail" type,
	//  and "ON CALL" is in the .FileName member
	// ...
	//  returns TRUE if "system" type of pager.
	// ...
	//  returns TRUE if CheckFrqOptions() returns FALSE then Parse_Frq() returns TRUE.
	//
	int Get_Frq(int PagePointer, char * QueString, char * QueString2, int index)
	{
		int SelectStatus;
		int retval = FALSE;
		char buffer1[85];
		char buffer3[85];
		CString csSQLString = "";
		CString csFunctionName = "Get_Frq";
		CString csTableName = "";

		PrintMessage(MSGTYPE_SYSTEM,
			hwndGeneralMsg,
			"Get_Frq() - 0 [%d] [%s], [%s]",
			PagePointer,
			QueString,
			QueString2);

		memset(page[PagePointer].PagerNumber, 0, sizeof(page[PagePointer].PagerNumber));
		memset(page[PagePointer].AutCode, 0, sizeof(page[PagePointer].AutCode));

		if(strncmp(QueString, param.vmFrqType, 3) != 0)
		{
			// associated with voice mail ????
			if(strncmp(page[PagePointer].FileName, "ON CALL", 7) == 0)
			{
				retval = TRUE;

				PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"Parse_Frq() - 1 returning [%d]",
					retval);
				return retval;
			}

			// something to do with "system" type of page status ???
			//  see joseph's html document "config.htm"
			//
			if(page[PagePointer].ACKMin != -1 && Status[page[PagePointer].ACKMin].Page[0] == 'S')
			{
				if(strncmp(QueString, Status[page[PagePointer].ACKMin].PageTypes, 3) == 0)
				{
					memset(buffer1, 0, sizeof(buffer1));
					memset(buffer3, 0, sizeof(buffer3));
                    //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
					//sprintf_s<85>(buffer3, "%-3.3s", Status[page[PagePointer].ACKMin].PageTypes);
                    sprintf(buffer3, "%-3.3s", Status[page[PagePointer].ACKMin].PageTypes);

					retval = Parse_Frq(PagePointer, QueString, QueString2, buffer1, buffer3);

					if(retval == TRUE)
					{
						PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"Parse_Frq() - 2 returning [%d]",
							retval);
						return retval;
					}
					else
					{
						PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"Parse_Frq() - 2.5 returning [%d] FAILED -----------------------------------",
							retval);
					}
				}
			}
		}

		// 7/12/2016 - SOguz - DE20747 - Don't want to run the select statement to get frq since 
		// PageAllDevices feature uses another method called "GetAllPagingDevicesFromProfile" to 
		// collect FrqType and FrqNumber information
		if (isPageAllDevicesOn == FALSE)
		{
		// prepare the appropriate number of "OR" clauses to match the number of
		//  items listed in the  "voice queues" field of the page server configuration.

		// determine how many "OR" clauses are required.
		CString csQueString; csQueString = QueString;
		CString csOrClause;
		CString csSubClause;
		int iFoundAt = 0;
		int iLength = csQueString.GetLength();

		// if the string contains any characters, this processing is required.
		while(iLength > 0)
		{
			// find the next occurence of item.
			iFoundAt = csQueString.Find('%', iFoundAt);

			if(iFoundAt == -1)
			{
				// none found, or no more found.
				break;
			}
			else
			{
				// an item has been found, append an another "OR" sub-clause.
				//  of the form:  " OR FrqType = '%41'"
				//
				// grab the 3 characters, and format up an "OR" sub-clause
				//  and add it on to the complete "OR" clause.
				csSubClause.Format(" OR FrqType = '%s'", csQueString.Mid(iFoundAt, 3));

				csOrClause += csSubClause;
				// note that CString.Find() ignores the indexed
				//  character when the index is non-zero.

				// advance 3 characters, and resume search.
				iFoundAt += 3;
			}
		}

		csTableName = "XnFrq";
		csSQLString.Format(
			"SELECT ProfileId, FrqType, FrqNumber "
			"FROM XnFrq "
			"WHERE ProfileId = '%10.10s' "
			"AND (FrqType = '%s' %s) "
			"--SQL-1-1110",
			page[PagePointer].Extension,
			QueString2,
			csOrClause);

		SelectStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

		PrintMessage(MSGTYPE_SYSTEM, 
			hwndGeneralMsg,
			" - 3 FRQ Search key (ProfileId): [%10.10s], SelectStatementStatus [%i]",
			page[PagePointer].Extension,
			SelectStatus);
		}
		// for the given queue type e.g. %57, see if it is
		//  found in the returned recordset from the select statement.
		//  if found, we collect the frqNumber - which contains
		//  the capcode, or dialnumber, and PIN.
		//
		while((isPageAllDevicesOn || DBA_RECORD_FOUND == SelectStatus) && !retval)
		{
			memset(buffer1, 0, sizeof(buffer1));
			memset(buffer3, 0, sizeof(buffer3));
			// 7/12/2016 - SOguz - DE20747 - if pagealldeviceson then use the information 
			// collected by using GetAllPageDevicesFromProfile method.
			if (isPageAllDevicesOn)
			{
				sprintf(buffer3, "%-3.3s", AllPageableDevices[index].FrqType);
			}
			else
			{
				pDBA->ADOGetString("FrqType", buffer3, 3);
			}
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				"Get_Frq(): - 4 ADOGetString('FrqType') [%s]", 
				buffer3);

			// 7/12/2016 - SOguz - DE20747 - if pagealldeviceson then use the information 
			// collected by using GetAllPageDevicesFromProfile method.
			if (isPageAllDevicesOn)
			{
				sprintf(buffer1, "%-64.64s", AllPageableDevices[index].FrqNumber);
			}
			else
			{
				pDBA->ADOGetString("FrqNumber", buffer1, 65);
			}
			

			// save a copy of the capcode, as read from the database.
			//  this may be used later when updating the peg counts
			//  we need to identify the row of Xnfrq that should be updated.
			//  the profileid, and pager type is not sufficient.
			//
            
			ZeroMemory(page[PagePointer].CapCodeFromXnFrq,
				sizeof(page[PagePointer].CapCodeFromXnFrq));
            //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
			//sprintf_s<66>(page[PagePointer].CapCodeFromXnFrq, "%s", buffer1);
            sprintf(page[PagePointer].CapCodeFromXnFrq, "%s", buffer1);
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				"Get_Frq(): - 5 ADOGetString('FrqNumber') [%s]", 
				buffer1);

			// again checking if this is a "voice mail" type.
			if(strncmp(QueString, param.vmFrqType, 3) == 0)
			{
				if(strncmp(buffer3, param.vmFrqType, 3) == 0)
				{
					//sprintf(page[PagePointer].PagerNumber, "%-29.29s", buffer1);
					//pagenumber was increased to 65 chars.
                    //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
					//sprintf_s<65>(page[PagePointer].PagerNumber, "%-64.64s", buffer1);
                    sprintf(page[PagePointer].PagerNumber, "%-64.64s", buffer1);

					Set_Null(page[PagePointer].PagerNumber,
						sizeof( page[PagePointer].PagerNumber));
					retval = TRUE;
				}
			}
			else
			{
				retval = CheckFrqOptions(PagePointer, QueString, buffer1, buffer3);

				if(retval == FALSE)
				{
					PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"Get_Frq(): - 6 calling Parse_Frq()");

					PrintMessage(MSGTYPE_SYSTEM,
						hwndGeneralMsg,
						"Get_Frq(): - 7 [%d] [%s] [%s] [%s] [%s]",
						PagePointer,
						QueString,
						QueString2,
						buffer1,
						buffer3);

					PrintMessage(MSGTYPE_SYSTEM,
						hwndGeneralMsg,
						"Get_Frq(): - 8 PagePointer=[%d] ComNumber=[%d] ComOffset=[%d] ScriptFlag=[%d]",
						PagePointer,
						page[ PagePointer ].ComNumber,
						page[ PagePointer ].ComOffset,
						page[ PagePointer ].ScriptFlag);

					retval = Parse_Frq(PagePointer, QueString, QueString2, buffer1, buffer3);
				}
			}

			if(retval == TRUE)
			{
				break;
			}

			if(SUCCESS == pDBA->ADOStep())
			{
				// step was successful,
				PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"Get_Frq(): - 8 ADOStep() - successful");
			}
			else
			{
				// end of file reached, or error
				PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"Get_Frq(): - 8.5 ADOStep() - NOT successful");

				break;
			}
		}

		PrintMessage(MSGTYPE_SYSTEM,
			hwndGeneralMsg,
			"Parse_Frq() - 99 returning [%d]",
			retval);

		return retval;
	}


	int
CheckFrqOptions( int PagePointer, char *QueString, char *buffer1, char *buffer3 )
	{
	int retval = FALSE;
	char QueTypes1[45];

	memset(QueTypes1, 0, sizeof(QueTypes1));

    if( page[ PagePointer ].ACKMin != -1
		&& Status[ page[ PagePointer ].ACKMin ].PageTypes[ 0 ] == '%'
		&& Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'Y' )
		{
    	page[PagePointer].ScriptNumber = -1;
		page[PagePointer].ComNumber = -1;

		strcpy_s<45>( QueTypes1, Status[ page[ PagePointer ].ACKMin ].PageTypes );

        retval = Parse_Frq(PagePointer, QueTypes1, QueString, buffer1, buffer3);
		return retval;
		}
	else
		{
		return FALSE;
		}
	}


	// retrieve information from the "Xkm" table
	//  contents of "Name" field is saved in page[].tempStr.
	//  contents of "Password" field is saved in page[].Password.
	// retrieve 5 yes/no "Flags"
	//
	// returns TRUE if page[].PageType[0] is non-zero,
	//  page type has been determined, and is not empty.
	//  otherwise FALSE if data access was un-successful
	//  say that .PageType could not be determined.
	//
	int
LogPager(char *Ext, int PagePointer)
	{
	static BOOL iFirstTimeThru = TRUE;
	static CString csFlags;
	int ReturnStatus = FALSE;
	int status;
	char FieldName[20];
	CString csSQLString = "";
	CString csFunctionName = "LogPager";
	CString csTableName = "";

	if(DATA_SOURCE_LOCAL == DataSource)
		{
		return TRUE;
		}

	if(iFirstTimeThru)
		{
		// setup first-time-thru items.
		iFirstTimeThru = FALSE;

		// prepare the flag names to be used in the sql statement
		CString csFlag1, csFlag2, csFlag3, csFlag4, csFlag5;

		// define the fieldnames for YesNo flags, if defined in the configuration file.
		if((param.FlagParkOffset >= 1) && (param.FlagParkOffset <= 25))
			{
			csFlag1.Format(", YNFlag%d", param.FlagParkOffset);
			}
		if((param.FlagForwardOffset >= 1) && (param.FlagForwardOffset <= 25))
			{
			csFlag2.Format(", YNFlag%d", param.FlagForwardOffset);
			}
		if((param.FlagStatusOffset >= 1) && (param.FlagStatusOffset <= 25))
			{
			csFlag3.Format(", YNFlag%d", param.FlagStatusOffset);
			}
		if((param.FlagTtPriorityOffset >= 1) && (param.FlagTtPriorityOffset <= 25))
			{
			csFlag4.Format(", YNFlag%d", param.FlagTtPriorityOffset);
			}
		if((param.FlagVmOffset >= 1) && (param.FlagVmOffset <= 25))
			{
			csFlag5.Format(", YNFlag%d", param.FlagVmOffset);
			}

		// combine all the flag names for use in the sql select statement.
		csFlags = csFlag1 + csFlag2 + csFlag3 + csFlag4 + csFlag5;
		}
	//2012-02-27 Added by LD
		PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg,
							"  New Ext: [%s]",
							Ext);		

	csTableName = "Xkm";
	csSQLString.Format(
						"SELECT Name, \"Password\""
						"%s "
						"FROM Xkm "
						"WHERE (ProfileId = '%10.10s') "
						"AND (Duplicated = 'N') "
						"AND (DuplicatedCounter = 0) "
						"--SQL-1-1111",
						csFlags,
						Ext);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"1 - System Id search key: [%10.10s], return [%i].",
					Ext,
					status);

	if(DBA_RECORD_FOUND == status)
		{
		// Name
		memset( page[ PagePointer ].tempStr, 0, sizeof( page[ PagePointer ].tempStr ) );

		pDBA->ADOGetString("Name", page[PagePointer].tempStr, 40);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"LogPager(): ADOGetString('Name') [%s]",
						page[PagePointer].tempStr);

		Set_Null( page[ PagePointer ].tempStr, sizeof( page[ PagePointer ].tempStr ) );

		// Get user password if available
		memset( page[ PagePointer ].Password, 0, sizeof( page[ PagePointer ].Password ) );

		pDBA->ADOGetString("Password", page[PagePointer].Password, 10);

		{
		// trim space from the left end of the password.

		CString csPassword; csPassword = page[PagePointer].Password;

		csPassword.TrimLeft();
		csPassword.TrimRight();

		// put back to 'C' character array variable.
        //SO - 03/21/2013 - Removed buffer check statement because it crashes the application
		//sprintf_s<11>(page[PagePointer].Password, "%s", (LPCTSTR) csPassword);
        sprintf(page[PagePointer].Password, "%s", (LPCTSTR) csPassword);
		}

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"LogPager(): ADOGetString('Password') [%s]",
						page[PagePointer].Password);

		Set_Null(page[PagePointer].Password, sizeof(page[PagePointer].Password));

		memset(page[PagePointer].Flag1, 0, sizeof(page[PagePointer].Flag1));
		memset(page[PagePointer].Flag2, 0, sizeof(page[PagePointer].Flag2));
		memset(page[PagePointer].Flag3, 0, sizeof(page[PagePointer].Flag3));
		memset(page[PagePointer].Flag4, 0, sizeof(page[PagePointer].Flag4));
		memset(page[PagePointer].Flag5, 0, sizeof(page[PagePointer].Flag5));

		if(param.FlagParkOffset > 0)
			{
			if((param.FlagParkOffset >= 1) && (param.FlagParkOffset <= 25))
				{
                //SO - 03/21/2013 - Removed buffer check statement because it crashes the application
				//sprintf_s<20>(FieldName, "YNFlag%d", param.FlagParkOffset);
                sprintf(FieldName, "YNFlag%d", param.FlagParkOffset);

				pDBA->ADOGetString(FieldName,
								page[PagePointer].Flag1,
								1);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"LogPager(): ADOGetString(%s) [%s]",
								FieldName,
								page[PagePointer].Flag1);
				}
			}

		if(param.FlagForwardOffset > 0)
			{
			if((param.FlagForwardOffset >= 1) && (param.FlagForwardOffset <= 25))
				{
                //SO - 03/21/2013 - Removed buffer check statement because it crashes the application
				//sprintf_s<20>(FieldName, "YNFlag%d", param.FlagForwardOffset);
                sprintf(FieldName, "YNFlag%d", param.FlagForwardOffset);
				pDBA->ADOGetString(FieldName,
								page[PagePointer].Flag2,
								1);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"LogPager(): ADOGetString(%s) [%s]",
								FieldName,
								page[PagePointer].Flag2);
				}
			}

		if(param.FlagStatusOffset > 0)
			{
			if((param.FlagStatusOffset >= 1) && (param.FlagStatusOffset <= 25))
				{
                //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
				//sprintf_s<20>(FieldName, "YNFlag%d", param.FlagStatusOffset);
                sprintf(FieldName, "YNFlag%d", param.FlagStatusOffset);

				pDBA->ADOGetString(FieldName,
								page[PagePointer].Flag3,
								1);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"LogPager(): ADOGetString(%s) [%s]",
								FieldName,
								page[PagePointer].Flag3);
				}
			}

		if(param.FlagTtPriorityOffset > 0)
			{
			// default is 'Y'
			page[PagePointer].Flag4[0] = 'Y';

			if((param.FlagTtPriorityOffset >= 1) && (param.FlagTtPriorityOffset <= 25))
				{
                //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
                //sprintf_s<20>(FieldName, "YNFlag%d", param.FlagTtPriorityOffset); 
				sprintf(FieldName, "YNFlag%d", param.FlagTtPriorityOffset);

				pDBA->ADOGetString(FieldName,
								page[PagePointer].Flag4,
								1);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"LogPager(): ADOGetString(%s) [%s]",
								FieldName,
								page[PagePointer].Flag4);
				}
			}

		if(param.vmFrqType[0] != 0)
			{
			// default is 'Y'
			page[PagePointer].Flag5[0] = 'Y';

			if(param.FlagVmOffset > 0)
				{
				if((param.FlagVmOffset >= 1) && (param.FlagVmOffset <= 25))
					{
                    //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
					//sprintf_s<20>(FieldName, "YNFlag%d", param.FlagVmOffset);
                    sprintf(FieldName, "YNFlag%d", param.FlagVmOffset);

					pDBA->ADOGetString(FieldName,
									page[PagePointer].Flag5,
									1);

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"LogPager(): ADOGetString(%s) [%s]",
									FieldName,
									page[PagePointer].Flag5);
					}
				}
			}

		if((page[PagePointer].Flag5[0] == 'Y') && (param.vmFrqType[0] != 0))
			{
			if(Get_Frq(PagePointer, param.vmFrqType, param.vmFrqType) == FALSE)
				{
				page[PagePointer].Flag5[0] = 'N';
				}
			}

		if(page[PagePointer].PageType[0] != 0)
			{
			ReturnStatus = TRUE;
			}
		}
	else
		{
		//XnResetDb( status, XKM_TABLE, "Xn Get" );
		page[PagePointer].PageStatus[0] = 'N';
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"ERROR Unable to find id [%10.10s] rv %i.",
						Ext,
						status);

		ReturnStatus = FALSE;
		}

	return ReturnStatus;
	}


	// return values:
	//   0  all tests for the proposed coverage were ok.
	//  -2	multiple coverage, and multiple coverage IS permitted.
	//  -3	illegal value for page status.
	//  -4  unable to find pagers table record for this profile.
	//  -5  DefaultPager type is not present in the PageServer config.
	//  -6	multiple coverage, but multiple coverage is NOT permitted.
	//
	//      Covering pager routines reuse to follwing page variables:
	//
	//          page[ ].Filename    Actual covering extension
	//          page[ ].AutCode     Entered covering extension
	//          page[ ].ScriptInfo  Covering extension's page status
	//          page[ ].PageInfo    Covering extension's name
	//          page[ ].PagerNumber Covering extension's covering extension
	//          page[ ].InitiatorId   Covering extension's pager id #
	//
	int
CoverLog(char *Ext, int PagePointer)
	{
	int status;
    char buffer[20];
	CString csSQLString = "";
	CString csFunctionName = "CoverLog";
	CString csTableName = "";

	csTableName = "Pagers";
	csSQLString.Format(
						"SELECT PagerId, ProfileId, Status, "
						"DefaultPager, CoveringExtension "
						"FROM Pagers "
						"WHERE ProfileId = '%10.10s' "
						"--SQL-1-1112",
						Ext);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"Covering search key: %10.10s, return value %i.",
					Ext,
					status);

	if(DBA_RECORD_FOUND == status)
		{
		// Covering Extension's Status
		memset(buffer, 0, sizeof(buffer));

		pDBA->ADOGetString("Status", buffer, 1);

        if(buffer[0] >= '0' && buffer[0] <= 'M')
			{
			// translate page status byte, 
			//  read from the "pagers" table, that represents
			//  the page status value into an integer between 0 and 29.
			//
            page[PAGE_COV_POS].ACKMin = (int) buffer[0] - (int) '0';
			}
        else
			{
            page[PAGE_COV_POS].ACKMin = param.statusDefault;
			}

        if(page[PAGE_COV_POS].ACKMin < 0 || page[PAGE_COV_POS].ACKMin > 29)
			{
			// illegal value for page status.
			return -3;
			}

		// Covering Extension's Covering
        memset(page[PAGE_COV_POS].FileName, 0, sizeof(page[PAGE_COV_POS].FileName));
		memset(buffer, 0, sizeof(buffer));

		pDBA->ADOGetString("CoveringExtension", buffer, 10);

		// swallow spaces.
        XnCopy(page[PAGE_COV_POS].FileName, buffer, 10);

        memset(page[PagePointer].PagerNumber, 0, sizeof(page[PagePointer].PagerNumber));

        if(page[PAGE_COV_POS].FileName[0] != 0)
			{
            strncpy_s<65>(page[PagePointer].PagerNumber, page[PAGE_COV_POS].FileName, sizeof(page[0].PagerNumber));

            PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg,
							"Extension [%s] already covered by [%s]",
							Ext,
							page[PAGE_COV_POS].FileName);

            if(param.MultiCover)
				{
				PrintMessage(MSGTYPE_SYSTEM,
								hwndGeneralMsg, 
								"CoverLog(): multi-cover situation");
				return -2;	// multiple coverage, and multiple coverage IS permitted.
				}

            return -6;		// multiple coverage, but multiple coverage is NOT permitted.
			}

		// Covering Extension Name : Not currently used
        memset(page[PAGE_COV_POS].PageInfo, ' ', sizeof(page[PAGE_COV_POS].PageInfo));

		// Covering Extension Id Number
		memset(page[PAGE_COV_POS].InitiatorId, 0, sizeof(page[PAGE_COV_POS].InitiatorId));
		
		memset(buffer, 0, sizeof(buffer));

		pDBA->ADOGetString("PagerId", buffer, 10);

		// swallow spaces.
		XnCopy(page[PAGE_COV_POS].InitiatorId, buffer, 10);

		// Covering Extension's Default Pager
		memset(buffer, 0, sizeof(buffer));

        if(page[PAGE_COV_POS].ACKMin != -1 &&
			Status[page[PAGE_COV_POS].ACKMin].Page[0] == 'S')
			{
            //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
			//sprintf_s<20>(buffer, "%3.3s", Status[page[PAGE_COV_POS].ACKMin].PageTypes);
            sprintf(buffer, "%3.3s", Status[page[PAGE_COV_POS].ACKMin].PageTypes);
			}
		else
			{
			memset(buffer, 0, sizeof(buffer));

			pDBA->ADOGetString("DefaultPager", buffer, 3);
			}
        buffer[3] = 0;

        if(GetPageType(PAGE_COV_POS, buffer, __FILE__, __LINE__) == FALSE)
			{
			// pagers table record for this profile was found, 
			//  but the value for the DefaultPager type is 
			//  not present in the PageServer config.
            return -5;
			}
		}
    else
		{
		// unable to find pagers table record for this profile.
        PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg, 
						"ERROR Unable find record");
        return -4;
		}

	// all tests for coverage are ok.
    return 0;
	}


	// expect that no further coverage.
	//  too many levels of coverage
	//  or self-coverage would be bad.
	//
	// returns TRUE only if the pager is NOT covered.
	//  that is the expectation at this point.
	//
	int
CoverDoubleCheck(int PagePointer)
	{
	int status;
    int i = 0;
	char buffer[20];
	CString csSQLString = "";
	CString csFunctionName = "CoverDoubleCheck";
	CString csTableName = "";

    PrintMessage(MSGTYPE_SYSTEM, 
					hwndDatabaseMsg, 
					"CoverDoubleCheck(): begin");

    //strncpy(page[PAGE_COV_POS].FileName, page[PagePointer].PagerNumber, sizeof(page[0].FileName));
	strncpy_s<120>(page[PAGE_COV_POS].FileName, page[PagePointer].PagerNumber, sizeof(page[0].PagerNumber));

    i = 0;
    if(strncmp(page[PagePointer ].Extension,
				page[PAGE_COV_POS ].FileName,
				strlen(page[PagePointer].Extension)) == 0)
		{
		i = 1;
		}
    if(strncmp(page[PagePointer].Extension,
				page[PagePointer].AutCode,
				strlen(page[PagePointer].Extension)) == 0)
		{
		i = 1;
		}

    if(i)
		{
        PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg,
						"Extension %s covered by self",
						page[PAGE_COV_POS].FileName );

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndDatabaseMsg, 
						"CoverDoubleCheck(): end 1 FALSE");

        return FALSE;
		}

	// allow up to 10 levels of coverage.
    for(i = 0; i < 10; i++)
		{
        strcpy_s<11>(page[PAGE_COV_POS].Extension, page[PAGE_COV_POS].FileName);

		csTableName = "Pagers";
		//2012-03-07 Added Field of "PagerId" by LD
		csSQLString.Format(
							"SELECT ProfileId, Status, CoveringExtension, PagerId "
							"FROM Pagers "
							"WHERE ProfileId = '%10.10s' "
							"--SQL-1-1113",
							page[PAGE_COV_POS].FileName);

		status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"Covering recursion search key: %10.10s, return value %i.",
						page[PAGE_COV_POS].FileName,
						status);

		if(DBA_RECORD_FOUND == status)
			{
			// Covering Extensions Status
			memset(buffer, 0, sizeof(buffer));

			pDBA->ADOGetString("Status", buffer, 1);

			if(buffer[0] >= '0' && buffer[0] <= 'M' && StatusOn)
				{
				// translate page status byte, 
				//  read from the "pagers" table, that represents
				//  the page status value into an integer between 0 and 29.
				//
				page[PAGE_COV_POS].ACKMin = (int) buffer[0] - (int) '0';
				}
			else
				{
				page[PAGE_COV_POS].ACKMin = param.statusDefault;
				}

			if(page[PAGE_COV_POS].ACKMin < 0 || page[PAGE_COV_POS].ACKMin > 29)
				{
				page[PAGE_COV_POS].ACKMin = -1;
				}

			// Covering Extensions Covering Extension
            memset(page[PAGE_COV_POS].FileName, 0, sizeof(page[PAGE_COV_POS].FileName));
			memset(buffer, 0, sizeof(buffer));

			pDBA->ADOGetString("CoveringExtension", buffer, 10);

			//JPH -- 01/03/13 -- removed xncopy
			// swallow spaces.
            XnCopy(page[PAGE_COV_POS].FileName, buffer, 10);

			//strncpy_s<120>(page[PAGE_COV_POS].FileName, buffer, 10);

			//JPH 12/18/12 --flag
			//2012-03-07 Added Field of "PagerId" by LD
            memset(page[PAGE_COV_POS].PagerId, 0, sizeof(page[PAGE_COV_POS].PagerId));
			memset(buffer, 0, sizeof(buffer));

			pDBA->ADOGetString("PagerId", buffer, 10);

			// swallow spaces.
            XnCopy(page[PAGE_COV_POS].PagerId, buffer, 10);
			//strncpy_s<120>(page[PAGE_COV_POS].FileName, buffer, 10);

			bool mustIgnoreCoverageWhenPageAllDevices = ignoreCoverageWhenPageAllDevices && (CheckIfAllPageFlagIsOn(PAGE_COV_POS) == SUCCESS);

            if( page[PAGE_COV_POS].FileName[0] != 0 && !mustIgnoreCoverageWhenPageAllDevices)
				{
                PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"Extension %s already covered by %s.",
								page[PagePointer].PagerNumber,
								page[PAGE_COV_POS].FileName);

                if(strncmp(page[PagePointer].Extension,
							page[PAGE_COV_POS].FileName,
							strlen(page[PagePointer].Extension)) == 0)
					{
                    PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"Extension %s recusively covered by self",
									page[PAGE_COV_POS].FileName);

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndDatabaseMsg, 
									"CoverDoubleCheck(): end 2 FALSE");

                    return FALSE;
					}
				}
            else
				{
				// note that this is the successful outcome of this routine.
				//  we expect that the given extension is the ultimate
				//  and final pager.  it should NOT be covered by anyone else.
                PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"Extension not covered.");

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndDatabaseMsg, 
								"CoverDoubleCheck(): end 1 TRUE");

                return TRUE;
				}
			}
        else
			{
			PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"ERROR Unable to find id %s rv %i.",
							page[PAGE_COV_POS].FileName,
							status);

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndDatabaseMsg, 
							"CoverDoubleCheck(): end 3 FALSE");

            return FALSE;
			}
		}

	// for-next is exhausted at this point, too many levels of coverage.
	PrintMessage(MSGTYPE_SYSTEM, 
					hwndDatabaseMsg, 
					"CoverDoubleCheck(): end 4 FALSE");
    return FALSE;
	}


	// returns TRUE or FALSE.
	// fills variables:
	//		page[PagePointer].Extension
	//		page[PagePointer].FileName is filled with the text "ON CALL"
	//                                 if the "Pagers" table "CoveringExtension" field
	//                                 contains the text "ON CALL"
	// and others page[PagePointer].xxx thru call to GetPageType()
	//
	// keyNumber == 0 to lookup by Pagers:PagerId
	//  == 1 for Pagers:ProfileId
	//  == 2 for Xkm:PatientId.
	//
	int
GetPagerId(char * Key, int PagePointer, int keyNumber)
	{
	int status;
    int i;
	int j;
	char buffer[20];

	CString csSQLString;
	CString csFunctionName = "GetPagerId";
	CString csTableName;

	//2012-02-27 added by LD
	CString tmpKey;
	int comaIndx;

	PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"  * Key: [%s]",
						Key);
	PrintMessage(MSGTYPE_DEBUG, 
					hwndDatabaseMsg, 
					"GetPagerId(): begin");

	PrintMessage(MSGTYPE_DEBUG,
					hwndDatabaseMsg,
					"21 - .FileName[%s]",
					page[PagePointer].FileName);

	PrintMessage(MSGTYPE_DEBUG, hwndGeneralMsg, "GetPagerId(): begin");
	PrintMessage(MSGTYPE_DEBUG, hwndGeneralMsg, " parameters:");
	PrintMessage(MSGTYPE_DEBUG, hwndGeneralMsg, "  1 - Key        : [%s]", Key);
	PrintMessage(MSGTYPE_DEBUG, hwndGeneralMsg, "  2 - PagePointer: [%d]", PagePointer);
	PrintMessage(MSGTYPE_DEBUG, hwndGeneralMsg, "  3 - keyNumber  : [%d]", keyNumber);
	PrintMessage(MSGTYPE_DEBUG, hwndGeneralMsg, " other inputs:");

	PrintMessage(MSGTYPE_DEBUG, 
					hwndGeneralMsg,
					"  page[PagePointer].PagerId[0]: [0x%02X]",
					page[PagePointer].PagerId[0] & 0x0ff);

	PrintMessage(MSGTYPE_DEBUG, 
					hwndGeneralMsg,
					"  param.statusDefault: [0x%02X]",
					param.statusDefault);

	PrintMessage(MSGTYPE_DEBUG,
					hwndGeneralMsg, 
					"  1-StatusOn: [0x%02X]", 
					StatusOn);

	PrintMessage(MSGTYPE_DEBUG,
					hwndGeneralMsg, 
					"  intermediate data:");

	PrintMessage(MSGTYPE_DEBUG, 
					hwndGeneralMsg, 
					"  Key  : [%10.10s]", 
					Key);

	if(keyNumber == BY_PAGERID)
		{
		//2012-02-27 Added by LD
		
			if (Respect_Paging_Status)
			{

				tmpKey=Key;
				comaIndx=tmpKey.Find(',',0);
				if (comaIndx !=-1)
				{
					tmpKey=tmpKey.Right(tmpKey.GetLength()-(comaIndx+1));

				}

				comaIndx=tmpKey.Find(',',0);
				if (comaIndx !=-1)
				{
					tmpKey=tmpKey.Right(tmpKey.GetLength()-(comaIndx+1));

				}

				Key=tmpKey.GetBuffer(tmpKey.GetLength());

			
			}
		PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg,
							"  * New Key: [%s]",
							Key);			
			
		
			//2012-02-27 end of change LD
		// present the Key as a PagerId to the Pagers table to see if a row exists.
		csTableName = "Pagers";
		csSQLString.Format("SELECT PagerId, ProfileId, Status, Information, "
							"DefaultPager, CoveringExtension "
							"FROM Pagers "
							"WHERE PagerId = '%10.10s' "
							"--SQL-1-1114",
							Key);

		status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
		}
	else if(keyNumber == BY_PROFILEID)
		{
		// present the Key as a ProfileId to the Pagers table to see if a row exists.
		csTableName = "Pagers";
		csSQLString.Format("SELECT PagerId, ProfileId, Status, Information, "
							"DefaultPager, CoveringExtension "
							"FROM Pagers "
							"WHERE ProfileId = '%10.10s' "
							"--SQL-1-1115",
							Key);

		status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
		}
	else if(keyNumber == BY_PATIENTID)
		{
		// when looking up by PatientId, it is a 2 step process.
		//  present the Key as a PatientId to the Xkm table table to see if a row exists.
		//  then find the desired information elements in the PagersTable.
		csTableName = "Xkm";

		CString csText = Key;
		csText.TrimLeft();
		csText.TrimRight();

		csSQLString.Format("SELECT Profileid, PatientId "
							"FROM Xkm "
							"WHERE PatientId = '%-15s' "
							"--SQL-1-1123",
							csText);

		status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

		if(DBA_RECORD_FOUND == status)
			{
			// the row exists in the Xkm table, retrieve the ProfileId from the Xkm table. 
			//  shortly, it will be used to access the Pagers table.
			//
			char ProfileId[11];
			pDBA->ADOGetString("ProfileId", ProfileId, 10);

			csTableName = "Pagers";
			csSQLString.Format("SELECT PagerId, ProfileId, Status, Information, "
								"DefaultPager, CoveringExtension "
								"FROM Pagers "
								"WHERE ProfileId = '%10.10s' "
								"--SQL-1-1124",
								ProfileId);

			status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
			}
		}
	else
		{
		// this should never happen, if it does, it will NOT be seen as successful.
		status = DBA_RECORD_FOUND;
		}

    if(DBA_RECORD_FOUND == status)
		{
 		// at this point, a successful access to Pagers table has been accomplished.
		//  retrieve the necessary data elements from the recordset.
		memset(page[PagePointer].Extension, 0, sizeof(page[PagePointer].Extension));
		memset(buffer, 0, sizeof(buffer));

		pDBA->ADOGetString("ProfileId", buffer, 10);

		// swallow spaces.
		XnCopy(page[PagePointer].Extension, buffer, sizeof(page[PagePointer].Extension));

		if(page[PagePointer].PagerId[0] == 0)
			{
			// PagerId is not known, retrieve it.
			memset(buffer, 0, sizeof(buffer));

			pDBA->ADOGetString("PagerId", buffer, 10);

			// copy ascii chars '0' thru '9' only.
            memset(page[PagePointer].PagerId, 0, sizeof(page[PagePointer].PagerId));
			j = 0;

            for(i = 0; i < 10; i++)
				{
                if(buffer[i] >= '0' && buffer[i] <= '9')
					{
                    page[PagePointer].PagerId[j] = buffer[i];
					j++;
					}
				}
			}

		memset(buffer, 0, sizeof(buffer));
		pDBA->ADOGetString("Status", buffer, 1);

		// if 'status' field in table'Pagers' is in the range '0' thru 'M' inclusive,
		//  and status is to be used (network mode, not standalone mode),
		//  subtract ascii offset '0' to make the value numeric 0 thru 29 inclusive.
		//  these correspond to the definitions in NOPGSTAT.
       
		// 05/18/2017-SO-DE24153-removed "&& StatusOn" from the end of the if statement
		// since it was effecting coverage and we are only verifying that status is
		// in the range '0' and 'M' inclusive
		if(buffer[0] >= '0' && buffer[0] <= 'M')
			{
            page[PagePointer].ACKMin = (int) buffer[0] - (int) '0';
			}
        else
			{
			// use default status as defined in the parameter file.
            page[PagePointer].ACKMin = param.statusDefault;
			}

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"Page status - 0 thru 29 (page[PagePointer].ACKMin): [0x%02X]",
						page[PagePointer].ACKMin & 0x0ff);

		// if the prompt number for this status is defined as -1
		//  then assign this pager to have the default status as defined
		//  in the configuration file parameter.
		if(Status[page[PagePointer].ACKMin].Prompt == -1)
			{
			page[PagePointer].ACKMin = param.statusDefault;
			}

        if(page[PagePointer].ACKMin < 0 || page[PagePointer].ACKMin > 29)
			{
			page[PagePointer].ACKMin = -1;
			}

		PrintMessage(MSGTYPE_DEBUG, 
						hwndDatabaseMsg, 
						"GetPagerId(): before .coverpage");

		PrintMessage(MSGTYPE_DEBUG,
						hwndDatabaseMsg,
						"23 - .FileName[%s]",
						page[PagePointer].FileName);

		// parameter enables check for "covered-by"
		//  this feature is actually a profile covers for another profile.
		if(param.CoverPage)
			{
            memset(page[PagePointer].FileName, 0, sizeof(page[PagePointer].FileName));
			memset(buffer, 0, sizeof(buffer));

			// an entry in this profile determines
			//  if another profile should be paged instead.
			//pDBA->ADOGetString("CoveringExtension", buffer, 10);

			CString stringBuffer;
			pDBA->ADOGetString("CoveringExtension", buffer, 20, stringBuffer);

			if(stringBuffer.Find("ON CALL") >= 0)
				{
				// on-call YES.
				strcpy_s<120>(page[PagePointer].FileName, "ON CALL");
				}
			else
				{
				// on-call NO.
				// transfer the CoveringExtension string to the page[].FileName field.

				// swallow spaces.
				XnCopy(page[PagePointer].FileName, buffer, 20);
				}

            memset(page[PagePointer].PageInfo, 0, sizeof(page[PagePointer].PageInfo));

            if(PagerId)
				{
                memset(page[PagePointer].InitiatorId, 
						0, 
						sizeof(page[PagePointer].InitiatorId));
				}
			}

        if(page[PagePointer].ACKMin != -1 && page[PagePointer].FileName[0] != ' ')
			{
			if( Status[page[PagePointer].ACKMin].Page[0] == 'F' ||
				Status[page[PagePointer].ACKMin].Page[0] == 'O' ||
				Status[page[PagePointer].ACKMin].Page[0] == '#' ||
				Status[page[PagePointer].ACKMin].Page[0] == 'U')
				{
				memset(page[PagePointer].PageInfo, 
						0, 
						sizeof(page[PagePointer].PageInfo));

				pDBA->ADOGetString("Information", page[PagePointer].PageInfo, 65);

				Set_Null(page[PagePointer].PageInfo, 
							sizeof(page[PagePointer].PageInfo));
				}
			}

        memset(buffer, 0, sizeof(buffer));

		// special treatment for "SYS" page types, as defined in the
		//  page server config utility - touchtone interface,
		//  page status parameters, screen.
        if(page[PagePointer].ACKMin != -1 &&
			Status[page[PagePointer].ACKMin].Page[0] == 'S')
			{
            //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
			//sprintf_s<20>(buffer, "%3.3s", Status[page[PagePointer].ACKMin].PageTypes);
            sprintf(buffer, "%3.3s", Status[page[PagePointer].ACKMin].PageTypes);
			}
		else
			{
			// use 'DefaultPager' from 'Pagers' table.
			pDBA->ADOGetString("DefaultPager", buffer, 3);
			}
        buffer[3] = 0;

		if(GetPageType(PagePointer, buffer, __FILE__, __LINE__))
			{
			return TRUE;
			}

        if(page[PagePointer].VoiceFile == TRUE)
			{
			return TRUE;
			}

        if(strncmp(page[PagePointer].FileName, "ON CALL", 7) == 0)
			{
            strncpy_s<3>(page[PagePointer].PageType, Queue[0].Id + 1, 2);
			return TRUE;
			}
		return TRUE;
		}
	else
		{
		return FALSE;
		}
	}


	int
NameId(char * TonesEntered, int PagePointer)
	{
	int status;
    int i;
    int j = 0;
    int k = 0;
    char ReformattedTouchToneName[20];
	char TouchToneNameFromDatabase[20];
	char PagerIdFromDatabase[20];
	char ProfileIdFromDatabase[20];
	char YNFlag25FromDatabase[1];

	CString csSQLString = "";
	CString csFunctionName = "NameId";
	CString csTableName = "";

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					" NameId(): PagePointer:[%d] TonesEntered:[%s]",
					PagePointer,
					TonesEntered);

	// note: in SQL statement LIKE argument,
	//       console.exe writes unnecessary leading
	//       <space> character in this field.
	//       if this gets corrected in console.exe,
	//       the leading space must also be removed here.
	//       to sidestep the problem, add a leading space to the
	//       search argument.
	csTableName = "Xkm";
	csSQLString.Format(
						"SELECT ProfileId, PagerId, TouchToneName, YNFlag25, Duplicated "
						"FROM Xkm "
						"WHERE TouchToneName "
						"LIKE ' %s%%' "
						"AND PagerId > '         0' "
						"AND Duplicated = 'N' "

						"--SQL-1-1116",
						TonesEntered);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	ZeroMemory(TouchToneNameFromDatabase, sizeof(TouchToneNameFromDatabase));
	pDBA->ADOGetString("TouchToneName",
						TouchToneNameFromDatabase,
						sizeof(TouchToneNameFromDatabase));

    // again note leading <space> problem.
    //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
    //sprintf_s<20>(ReformattedTouchToneName, " %-10.10s", TonesEntered);
    sprintf(ReformattedTouchToneName, " %-10.10s", TonesEntered);

    if(strncmp(ReformattedTouchToneName, TouchToneNameFromDatabase, param.NameIdLength + 1) == 0)
		{
		// strings are identical...
		//PrintMessage(MSGTYPE_DATABASE_COMMAND,
		//				hwndDatabaseMsg,
		//				" NameId(): match WAS found "
		//				"NameIdLength:[%d] "
		//				"ReformattedTouchToneName:[%s] "
		//				"TouchToneNameFromDatabase:[%s]",
		//				param.NameIdLength,
		//				ReformattedTouchToneName,
		//				TouchToneNameFromDatabase
		//				);
		}
	else
		{
		//PrintMessage(MSGTYPE_DATABASE_COMMAND,
		//				hwndDatabaseMsg,
		//				" NameId(): match NOT found "
		//				"NameIdLength:[%d] "
		//				"ReformattedTouchToneName:[%s] "
		//				"TouchToneNameFromDatabase:[%s]",
		//				param.NameIdLength,
		//				ReformattedTouchToneName,
		//				TouchToneNameFromDatabase
		//				);
		status = FALSE;
		}

	// find and collect a number of qualifying names that match
	//  the touch-tone-name spelling entered by the user.
	//
    while(status == TRUE && j <= param.nameMax)
		{
		status = FALSE;

		ZeroMemory(ProfileIdFromDatabase, sizeof(ProfileIdFromDatabase));
		ZeroMemory(TouchToneNameFromDatabase, sizeof(TouchToneNameFromDatabase));
		ZeroMemory(YNFlag25FromDatabase, sizeof(YNFlag25FromDatabase));
		ZeroMemory(PagerIdFromDatabase, sizeof(PagerIdFromDatabase));

		pDBA->ADOGetString("ProfileId", ProfileIdFromDatabase, sizeof(ProfileIdFromDatabase));

		pDBA->ADOGetString("TouchToneName",
							TouchToneNameFromDatabase,
							sizeof(TouchToneNameFromDatabase));

		pDBA->ADOGetString("YNFlag25", YNFlag25FromDatabase, sizeof(YNFlag25FromDatabase));
		pDBA->ADOGetString("PagerId", PagerIdFromDatabase, sizeof(PagerIdFromDatabase));

		// find non-empty pager id's
		//
		for(i = 0; i < 10; i++)
			{
			if(PagerIdFromDatabase[i] != ' ')
				{
				status = TRUE;
				break;
				}
			}

		// build a list of PagerId's, up to XX items to be presented
		//  in the page-by-name IVR selection menu of whom to page.
		//  the profile's custom greeting recording is spoken.
		// e.g.
		//  press 1 to page "Keith"
		//  press 2 to page "Joseph"
		//
		if(status == TRUE)
			{
			// for each "found", and "to be included in PBN" PagerId,
			//  add 10 more chars to the list of PagerId's
			//  contained in .tempStr2[].
			//

			// backwards logic used here...
			//  when a profile is added to the database,
			//  typically the flag is set to something other than 'Y'.
			//
			// the definition of the feature as implemented here is that if
			//  the value of the flag is 'Y' then the profile is to be excluded
			//  from the page-by-name feature.  this is so that by default
			//  you opt-in. to be EXCLUDED, you must set the flag to 'Y'.
			//
			if('Y' == YNFlag25FromDatabase[0])
				{
				// "opt'ed out" of this feature, the PagerId is not added to the list.
				//  (our own little version of the national "do-not-call" list).
				PrintMessage(MSGTYPE_SYSTEM,
								hwndDatabaseMsg,
								"NameId(): ProfileId: [%s] opt'ed OUT",
								ProfileIdFromDatabase);

				// skip the "copy to the list" step.
				//   ...
				}
			else
				{
				// the YNFlagXX for this profile indicates that this guy is to be
				//  included in the list of page-by-name PagerId's offered via IVR.
				PrintMessage(MSGTYPE_SYSTEM,
								hwndDatabaseMsg,
								"NameId(): ProfileId: [%s] opt'ed IN",
								ProfileIdFromDatabase);

				// copy to the list...
				j++;
				for(i = 0; i < 10; i++)
					{
					page[PagePointer].tempStr2[k] = PagerIdFromDatabase[i];
					k++;
					}
				}
			}

		if(SUCCESS == pDBA->ADOStep())
			{
			// step was successful,
			}
		else
			{
			// end of records reached, or error
			break;
			}

		// note that the plus 1 on the compare is because of the extra
		//   space before the TouchToneName.
        if(strncmp(ReformattedTouchToneName,
					TouchToneNameFromDatabase,
					param.NameIdLength + 1) != 0)
			{
			// the next TouchToneName in the database no longer matches
			//  what the IVR client entered.
			break;
			}
		}

	// return number of qualifying names found.
    return j;
	}


	void
Change_Log2(int PagePointer)
	{
	int status;
    int i;
    SYSTEMTIME CurrTime;
	char string2[200];
	CString csSQLString = "";
	CString csFunctionName = "Change_Log2";
	CString csTableName = "";
	CString csStatus;
	CString csStatusDateTime;
	CString csInformation;
	CString csInformationDateTime;
	CString csCoveringExtension;

 	csStatus = (char)(page[PagePointer].ACKMin + 48);

    GetLocalTime(&CurrTime);
	csStatusDateTime.Format("%04i%02i%02i%02i%02i",
							CurrTime.wYear,
							CurrTime.wMonth,
							CurrTime.wDay,
							CurrTime.wHour,
							CurrTime.wMinute);

	csInformationDateTime = csStatusDateTime;

    if(param.ClearWhere)
		{
		csInformation.Empty();
		}

    i = page[PagePointer].ACKMin;

    if(i != -1 &&
		(Status[i].Page[0] == 'F' ||
		 Status[i].Page[0] == 'O' ||
		 Status[i].Page[0] == '#' ||
		 Status[i].Page[0] == '0' ||
		 Status[i].Page[0] == 'V' ||
		 Status[i].Page[0] == 'U'))
		{
		memset(string2, 0, sizeof(string2));
        strcpy_s<200>(string2, page[PagePointer].PageInfo);

        if(Status[i].Page[0] == 'O')
			{
            strcat_s<200>(string2, ":Room Number");
			}
        else if(Status[i].Page[0] == 'U')
			{
            strcat_s<200>(string2, ":Exception");
			}
        else
			{
            strcat_s<200>(string2, ":Referred Number");
			}

		csInformation = string2;
		csCoveringExtension.Empty();

        i = TRUE;

        switch( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] )
			{
            case '#':
                memset( page[ PagePointer ].DisplayInfo, 0, sizeof( page[ PagePointer ].DisplayInfo ) );
                strncpy_s<255>(page[ PagePointer ].DisplayInfo,
						page[ PagePointer ].PageInfo,
						sizeof( page[ PagePointer ].DisplayInfo));
                break;
            case '0':
                memset( page[ PagePointer ].DisplayInfo, 0, sizeof( page[ PagePointer ].DisplayInfo ) );
                strncpy_s<255>(page[ PagePointer ].DisplayInfo,
						param.CForwardOperNum,
						sizeof( page[ PagePointer ].DisplayInfo));
                break;
            case 'V':
                memset( page[ PagePointer ].DisplayInfo, 0, sizeof( page[ PagePointer ].DisplayInfo ) );
                strncpy_s<255>(page[ PagePointer ].DisplayInfo,
						param.CForwardVMNum,
						sizeof( page[ PagePointer ].DisplayInfo));
                break;
            case 'F':
            case 'O':
			case 'U':
                memset(page[PagePointer].DisplayInfo, 0, sizeof(page[PagePointer].DisplayInfo));
                strncpy_s<255>(page[PagePointer].DisplayInfo,
						page[PagePointer].PageInfo,
						sizeof(page[PagePointer].DisplayInfo));
				//JPH 01/07/13 -- FALL THRU ????

            default:
                i = FALSE;
                break;
			}

        if( i == TRUE )
			{
            AddSetUp( PagePointer, "External" );
            strncpy_s<3>( page[ PagePointer ].PageType, param.CForwardQueue + 1, 2 );
            page[ PagePointer ].PortNumber = GetQuePointer( PagePointer );

            if((page[PagePointer].PortNumber >= 0) || IsSmsType(PagePointer) || (PagePointer == PAGE_MANUAL_POS))
				{
                page[PagePointer].PriorityQPointer = 0;

                AddQueueEx(PagePointer, page[PagePointer].PortNumber);

                memset( page[ PagePointer ].FileName, 0, sizeof( page[ PagePointer ].FileName ) );
                memset( page[ PagePointer ].InitiatorId, 0, sizeof( page[ PagePointer ].InitiatorId ) );
				}
            Set_Null( page[ PagePointer ].PageInfo, sizeof( page[ PagePointer ].PageInfo ) );
			}
		}
    else if(PagePointer < MAX_IVRPORTS)
		{
		csCoveringExtension.Format("%-10.10s", page[PagePointer].FileName);

        if( page[ PagePointer ].FileName[ 0 ] != 0 &&
			page[ PagePointer ].FileName[ 0 ] != ' ' )
			{
			WriteStatLog(PagePointer, 5);
			}
		}

	csTableName = "Pagers";

	csSQLString.Format(
						"UPDATE Pagers "
						"SET Status = '%s', "
						"StatusOperator = 'PageSrv', "
						"StatusDateTime = '%s', "
						"Information = '%s', "
						"InformationOperator = 'PageSrv', "
						"InformationDateTime = '%s', "
						"CoveringExtension = '%s' "
						"WHERE ProfileId = '%10.10s' "
						"--SQL-1-1303",
						csStatus,
						csStatusDateTime,
						csInformation,
						csInformationDateTime,
						csCoveringExtension,
						page[PagePointer].Extension);

	status = pDBA->ADORsUpdate(csFunctionName, csTableName, csSQLString);

	if(SUCCESS != status)
		{
		PrintMessage(MSGTYPE_SYSTEM,
						hwndGeneralMsg,
						"ERROR Unable Update PAGERS return %d", 
						status);
		}

    WriteStatLog(PagePointer, 1);
	}


	void
UpdateFrqBtrv(int PagePointer)
	{
	int status;
	int weekCount;
	int monthCount;
	char countString[20];
	char mcountString[20];

	// no need to perform updates if local database is in-use.
	if(DATA_SOURCE_LOCAL == DataSource)
		{
		return;
		}

	CString csSQLString = "";
	CString csFunctionName = "UpdateFrqBtrv";
	CString csTableName = "";

	memset(countString, 0, sizeof(countString));
	memset(mcountString, 0, sizeof(mcountString));

	sprintf_s<20>(countString, "Counts%d", WeekNumber + 1);
	sprintf_s<20>(mcountString, "MCounts%d", MonthNumber + 1);

	csTableName = "XnFrq";

	// if you expect the frqindex to have value 1, you may be disappointed.
	//  it won't necessarialy be that way.  to be sure to place the counts
	//  in the appropriate places, match up on the capcode also.

	// record peg counts where the profileid,
	//  frqnumber (capcode), and frqtype all match up.
	//
	csSQLString.Format(
						"SELECT ProfileId, FrqType, FrqIndex, %s, %s "
						"FROM XnFrq "
						"WHERE ProfileId = '%10.10s' "		// profileid
						"AND FrqNumber = '%s' "				// capcode
						"AND FrqType = '%%%s' "				// pagetype
						"--SQL-1-1117",
						countString,
						mcountString,
						page[PagePointer].Extension,		// profileid
						page[PagePointer].CapCodeFromXnFrq,	// capcode
						page[PagePointer].PageType);		// pagetype

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

    if(DBA_RECORD_FOUND == status)
		{
		pDBA->ADOGetNumeric(countString, &weekCount);
		pDBA->ADOGetNumeric(mcountString, &monthCount);

		// increase peg counts for the current week and month
		weekCount++;
		monthCount++;

		// write the increased counts back to the table.
		csTableName = "XnFrq";

		csSQLString.Format(
							"UPDATE XnFrq "
							"SET %s = %d, %s = %d "
							"WHERE ProfileId = '%10.10s' "		// profileid
							"AND FrqNumber = '%s' "				// capcode
							"AND FrqType = '%%%s' "				// pagetype
							"--SQL-1-1304",
							countString,
							weekCount,
							mcountString,
							monthCount,
							page[PagePointer].Extension,		// profileid
							page[PagePointer].CapCodeFromXnFrq,	// capcode
							page[PagePointer].PageType);		// pagetype

		pDBA->ADORsUpdate(csFunctionName, csTableName, csSQLString);
		}
	else
		{
		//XnResetDb( status, FRQ_TABLE, "FRQ Update" );
		}
	}


	void
Change_Log3(int PagePointer)
	{
	int status;
	CString csSQLString = "";
	CString csFunctionName = "Change_Log3";
	CString csTableName = "";

	if(DATA_SOURCE_LOCAL == DataSource)
		{
		return;
		}

	csTableName = "Xkm";
	csSQLString.Format(
						"UPDATE Xkm "
						"SET \"Password\" = '%-10.10s' "
						"WHERE ProfileId = '%10.10s' "
						"AND Duplicated = 'N' "
						"AND DuplicatedCounter = 0 "
						"--SQL-1-1305",
						page[PagePointer].Password,
						page[PagePointer].Extension);

	status = pDBA->ADORsUpdate(csFunctionName, csTableName, csSQLString);

	if(status != SUCCESS)
		{
		// XnResetDb( status, XKM_TABLE, "XKM Upadte" );
		PrintMessage(MSGTYPE_SYSTEM,
						hwndGeneralMsg,
						"ERROR Unable to Update XKM" );
		}

    WriteStatLog(PagePointer, 4);
	}





	// returns TRUE if an on-call profileId has been found.
	//  or FALSE if no-one is on call at this time.
	//
	// if successful, fills the following variables:
	//  OnCallBtrvBuf.OnCallInfo.Extension
	//  OnCallBtrvBuf.OnCallInfo.depPager
	//  OnCallBtrvBuf.OnCallInfo.OCExtension
	//
	// at entry, the value of ListQue.OnCallPosition has been set to -1
	//  if an actual schedule has been found, this value must be replaced
	//  with a reasonable value, not -1.
	//  this is the protocol defined by btrieve, so we must do likewise.
	//
	int
GetFirstOnCall(long *, char * OnCallProfileId, char *)
	{
	// note - unused parameters in ADO version.
	int	status;
    char KeyBuff[24];
    SYSTEMTIME TimeT;
	int iStatus = FALSE;

	CString csSQLString = "";
	CString csFunctionName = "GetFirstOnCall";
	CString csTableName = "";

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndDatabaseMsg, 
					"GetFirstOnCall(): enter");

	PrintMessage(MSGTYPE_SYSTEM,
					hwndDatabaseMsg,
					"GetFirstOnCall(): OnCallProfileId [%s]",
					OnCallProfileId);

    memset(&OnCallBtrvBuf, 0, sizeof(OnCallBtrvBuf));

    GetLocalTime( &TimeT );

    // get the current time - now.
	memset(KeyBuff, 0, sizeof(KeyBuff));
    sprintf_s<24>(KeyBuff,
			"%04.4d%02.2d%02.2d%02.2d:%02.2d",
			TimeT.wYear,
			TimeT.wMonth,
			TimeT.wDay,
			TimeT.wHour,
			TimeT.wMinute);

	// for the given profile, and time (current time of day),
	//  find the member-profile of who is currently on-call.
	csTableName = "OnCall";
	csSQLString.Format(
						"SELECT ProfileId, Period, MemberExtension, DepartmentPager "
						"FROM OnCall "
						"WHERE ProfileId = '%10.10s' "
						"AND Period <= '%s' "
						"ORDER BY ProfileId, Period DESC "
						"--SQL-1-1118",
						OnCallProfileId,
						KeyBuff);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

    PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"On call key: %s %s, Status %d.",
					OnCallProfileId,
					KeyBuff,
					status);

	if(DBA_RECORD_FOUND != status)
		{
		//XnResetDb( status, ON_CALL_TABLE, "On Call LE" );
		PrintMessage(MSGTYPE_SYSTEM,
						hwndDatabaseMsg,
						"GetFirstOnCall(): exit - error - FALSE");
		return FALSE;
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM,
						hwndDatabaseMsg,
						"GetFirstOnCall(): found an on-call schedule to process");
		iStatus = TRUE;
		}

	// get members that are defined in this schedule.
	//  the RequiredPeriod is the exact schedule starting time returned by
	//  the first qualifying on-call record.
	char RequiredStartTime[14];
	char StartTime[14];
	char ProfileId[11];
	char DepartmentPager[2];

	memset(RequiredStartTime, 0, sizeof(RequiredStartTime));

	// get the first qualifying record.
	pDBA->ADOGetString("Period", RequiredStartTime, 13);
	sprintf_s<14>(StartTime, RequiredStartTime);

	// start out with an empty map, and empty vector.
	mapcntOnCall.erase(mapcntOnCall.begin(), mapcntOnCall.end());
	veccntOnCall.erase(veccntOnCall.begin(), veccntOnCall.end());

	// position map iterator and vector iterator to the first element.
	mapitrOnCall = mapcntOnCall.begin();
	vecitrOnCall = veccntOnCall.begin();

	int iCount = 0;

	for(;;)
		{
		// an on-call member has been found, get the record(s).
		iCount++;

		// clear out data space
		memset(StartTime, 0, sizeof(StartTime));
		memset(ProfileId, 0, sizeof(ProfileId));
		memset(DepartmentPager, 0, sizeof(DepartmentPager));

		pDBA->ADOGetString("Period", StartTime, 13);
		pDBA->ADOGetString("MemberExtension", ProfileId, 10);
		pDBA->ADOGetString("DepartmentPager", DepartmentPager, 1);

		// read items until end of list is encountered.
		//  consider adding an artificial limit on how many
		//  items are allowed in a single list.

		if(strcmp(RequiredStartTime, StartTime) == 0)
			{
			// start time matches, this is a member of the current schedule.

			// store only unique items,
			//  try to find() the item that is about to be added to the map
			//  before the addition is actually made.
			if(mapcntOnCall.end() == mapcntOnCall.find(ProfileId))
				{
				// if after the find() operation, the
				//  iterator points "past the end" of the list,
				//  this indicates that key is not
				//  present in the map.
				//LIST_DATA OnCallData = {"Text Info", "", "", 1, 2, 3};
				LIST_DATA OnCallData;

				// prepare information in the OnCallData structure.
				//strncpy_s<10>(OnCallData.ProfileId, ProfileId, 9);
                //strncpy(OnCallData.ProfileId, ProfileId, 9);
				sprintf(OnCallData.ProfileId, "%s", ProfileId);
				sprintf(OnCallData.Message, "%s", ListQue.PageAlpha);
				sprintf(OnCallData.PageType, "%s", "??");

				// store new information in the map.

				// enter the item into the map.
				mapcntOnCall[ProfileId] = OnCallData;

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndDatabaseMsg,
								"storing map item [%s]",
								ProfileId);

				// store new information in the vector also.
				veccntOnCall.push_back(ProfileId);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndDatabaseMsg,
								"storing vector item [%s]",
								ProfileId);
				}
			else
				{
				// ignore any duplicates.
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndDatabaseMsg,
								"Warning: duplicate ProfileId in OnCall ignored [%s]",
								ProfileId);

				// ProfileId is already in the list.
				}

			// for now, just print out the members that qualify.
			PrintMessage(MSGTYPE_SYSTEM,
							hwndDatabaseMsg,
							"GetFirstOnCall(): StartTime [%s] ProfileId [%s] DeptPager[%s] ",
							StartTime,
							ProfileId,
							DepartmentPager);
			}
		else
			{
			// start time no longer matches,
			//  a member of a different schedule has been found.
			break;
			}

		if(SUCCESS == pDBA->ADOStep())
			{
			// step was successful,
			PrintMessage(MSGTYPE_SYSTEM,
							hwndDatabaseMsg,
							"GetFirstOnCall(): ADOStep() successful");
			}
		else
			{
			// end of file reached, or error
			break;
			}
		}

	// on exit from GetFirstOnCall()
	//  leave the map and list iterators pointing to the first element.
	mapitrOnCall = mapcntOnCall.begin();
	vecitrOnCall = veccntOnCall.begin();

	if(iStatus)
		{
		PrintMessage(MSGTYPE_SYSTEM,
						hwndDatabaseMsg,
						"GetFirstOnCall(): exit - TRUE");

		// post anything but -1 indicates success
		//  in finding an OnCall schedule. (btrieve database convention),
		//  ADO version must comply with this convention.
		ListQue.OnCallPosition = 0;
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM,
						hwndDatabaseMsg,
						"GetFirstOnCall(): exit - FALSE");
		}

    return iStatus;
	}


	int
GetNextOnCall(long *, char *)
	{
	int iStatus = FALSE;
	char string2[200];

	// note - unused parameters in ADO version.

	// clear out everything in the structure so that
	//  individual members need not be cleared separately.
    memset(&OnCallBtrvBuf, 0, sizeof(OnCallBtrvBuf));
    memset(string2, 0, sizeof(string2));

	// all on-call members are stored in the vector, and map.
	//  and the iterators are positioned to the next available item.

	//  information goes in string2.
	if(vecitrOnCall != veccntOnCall.end())
		{
		//char cString[100] = "";

		// item retrieved from the vector, get the data.		
        //SO - 03/20/2013 - fixed the iterator
        //sprintf_s<200>(string2, "%10.10s", AAssign(cString, *vecitrOnCall));
        sprintf_s<200>(string2, "%10.10s", (*vecitrOnCall).c_str());

		// don't forget to increase the iterator.
		vecitrOnCall++;

		iStatus = TRUE;
		}
	else
		{
		// end of vector was encountered.
		}

	// output information stored into:	
    //SO - 03/20/2013 - Removed buffer check statement because it crashes the application
    //sprintf_s<10>(OnCallBtrvBuf.OnCallInfo.Extension, "%-10.10s", string2);
    sprintf(OnCallBtrvBuf.OnCallInfo.Extension, "%-10.10s", string2);

	// FOR TEST ALWAYS SEND 'N'
	//sprintf_s(&OnCallBtrvBuf.OnCallInfo.depPager, 1, "%c", 'N');
    sprintf(&OnCallBtrvBuf.OnCallInfo.depPager, "%c", 'N');

	return iStatus;
	}


	// get the first element of a list-page list
	//  this element contains the default "numeric message"
	//  contained in the "XnData" / "Data" field of the "xtpglst" table.
	//  save it in the variable ListQue.PageNumber.
	// returns SUCCESS or FAIL
	int
ListGetFirst(long *, char * dataKey)
	{
	int status;
	CString csSQLString;
	CString csFunctionName = "ListGetFirst";
	CString csTableName;

	// new implementation for ADO.

	CString csProfileId; csProfileId = dataKey;

	// difference from btrieve.
	//  take the leftmost 10 chars from the dataKey,
	//  ignore the "000" that is appended
	//  all that we need is the ProfileId.

	// we are only concerned with the left-most 10 chars
	//  this is the ProfileId.
	csProfileId = csProfileId.Left(10);

	// show received parameters.
	//  note: for ado, the first parameter
	//        "btrPosition" is never used.

	csTableName = "Xtpglst";
	csSQLString.Format(
						"SELECT ProfileId, ListIndex, MemberExtension, XnData, DumData "
						"FROM Xtpglst "
						"WHERE ProfileId = '%10.10s' "
						"AND ListIndex >= '' "
						"AND MemberExtension >= '' "
						"ORDER BY ProfileId, ListIndex, MemberExtension"
						"--SQL-1-1119",						
						csProfileId);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

 	if(DBA_RECORD_FOUND == status)
		{
		// retrieve the default "numeric message", store in ListQue.PageNumber.
		memset(ListQue.PageNumber, 0, sizeof(ListQue.PageNumber));

		pDBA->ADOGetString("XnData", ListQue.PageNumber, 35);

		// purify the string.
		Set_Null(ListQue.PageNumber, sizeof(ListQue.PageNumber));

	    return SUCCESS;
		}
    return FAIL;
	}



// retrieve Data field or MemberExtension field
	int
ListGetNext(long *, int dataType, char * dataDest)
	{
	int GetStringStatus = FAIL;
	int StepStatus = FAIL;

	// try to step.
	StepStatus = pDBA->ADOStep();

	if(SUCCESS == StepStatus)
		{
		// successful step, try to retrieve data.
		if(dataType == 0)
			{
			// initialize the whole destination character array.
			memset(dataDest, 0, sizeof(dataDest));

			GetStringStatus = pDBA->ADOGetString("XnData", dataDest, 35);

			//  2005-04-04 - append the contents of DumData field from record 001
			//               to the text in the Data, or xnData field.
			//               this will provide a total of 35 + 43 = 78 chars of text
			//               for the default alpha text to be used in a list page.
			//
			// retrieve the DumData field and append it to the dataDest field
			//  in the structure, this field is 120 chars in length, no concern
			//  about buffer overflow.
			//
			// reduced error checking here, should be improved.
			//
			pDBA->ADOGetString("DumData", (dataDest + 35), 43);
			}
		else
			{
			memset(dataDest, 0, (10 + 1));
			GetStringStatus = pDBA->ADOGetString("MemberExtension", dataDest, 10);
			}
		}

	PrintMessage(MSGTYPE_SYSTEM,
					hwndDatabaseMsg, 
					"ListGetNext(): end");

	if((SUCCESS == GetStringStatus) && (SUCCESS == StepStatus))
		{
		// very good, return success.
		return TRUE;
		}
	else
		{
		// not good enough, return fail.
		return FALSE;
		}
	}


	void
XnOpenDb(void)
	{
#ifdef NOCO	
	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"XnOpenDb(): opening database");




	// initialize Component Object Model "COM" subsystem.
	if(S_OK == CoInitialize(NULL))
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"XnOpenDb(): CoInitialize: OK");
		}
	else
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"XnOpenDb(): CoInitialize: NG");
		}

#endif


	// prepare to open ADO database system

	// decide which connection string to use
	//  based on failsafe/network command line option.
	CString csConnectionString;

	if(DATA_SOURCE_LOCAL == DataSource)
		{
		// use local drive database (failsafe).
		csConnectionString = param.ConnectStringFailsafe;
		globalDbConnectionStr.assign(param.ConnectStringFailsafe);
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"XnOpenDb(): DATA_SOURCE_LOCAL == DataSource");
		}
	else
		{
		// use network database.
		csConnectionString = param.ConnectStringNetwork;
		globalDbConnectionStr.assign(param.ConnectStringNetwork);
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"XnOpenDb(): DATA_SOURCE_NETWORK == DataSource");
		}

	pscListPage = new SpoolCollection();

	// create the database accessor object(s).
	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"XnOpenDb(): new pDBA");

	pDBA = new DataBaseAccessor();

	// connection string as specified in application configuration parameter.
	pDBA->SetConnectString(csConnectionString);

	if(SUCCESS == pDBA->ADOConnectionConnect())
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"XnOpenDb(): ADOConnectionConnect: OK");
		}
	else
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"XnOpenDb(): ADOConnectionConnect: NG");
		}

		}


	void
XnCloseDb(void)
	{
	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"XnCloseDb(): closing database");

	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"XnCloseDb(): close queue files completed");

	// close ADO database
	if(SUCCESS == pDBA->ADOConnectionRelease())
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"XnCloseDb(): ADOConnectionRelease: OK");
		}
	else
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"XnCloseDb(): ADOConnectionRelease: NG");
		}

	// delete the database accessor object(s).
	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"XnCloseDb(): delete pDBA");
	delete pDBA;

	// make lifetime of SpoolCollections same as database objects
	//  so that I can find the source code.
	delete pscListPage;

	// uninitialize the COM subsystem.
	CoUninitialize();
	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"XnCloseDb(): CoUninitialize: OK");
	}




	void
SQLSelectAndRetrieve3(int iNumberToRetrieve, CStringArray & csaText)
	{
	int iStatus;
	CString csTemp;
	CString csRawText1, csRawText2, csRawText3;
	DataBaseAccessor * pDBA2;

	CString csFunctionName = "SQLSelectAndRetrieve";

	CString csTableName; csTableName = csaText[0];
	CString csSQLString; csSQLString = csaText[1];
	CString csField1Name; csField1Name = csaText[2];
	CString csField2Name; csField2Name = csaText[3];
	CString csField3Name; csField3Name = csaText[4];
#ifdef NOCO
	if(S_OK == CoInitialize(NULL))
		{
		csTemp = "CoInitialize OK";
		}
	else
		{
		csTemp = "CoInitialize NG";
		}
#endif
	// create the database accessor object.
	pDBA2 = new DataBaseAccessor();
	pDBA2->SetConnectString(param.ConnectStringNetwork);

	if(SUCCESS == pDBA2->ADOConnectionConnect())
		{
		csTemp = "connect OK";
		}
	else
		{
		csTemp = "connect NG";
		}

	// skip error check for now, put back in later.
	iStatus = pDBA2->ADORsSelect(csFunctionName, csTableName, csSQLString);

	for(int x = 0; x < iNumberToRetrieve; x++)
		{
		char buffer1[10];
		char buffer2[10];
		char buffer3[10];

        ZeroMemory(buffer1, sizeof(buffer1));
        ZeroMemory(buffer2, sizeof(buffer2));
        ZeroMemory(buffer3, sizeof(buffer3));

		pDBA2->ADOGetString((LPTSTR) (LPCTSTR) csField1Name, buffer1, sizeof(buffer1), csRawText1);
		pDBA2->ADOGetString((LPTSTR) (LPCTSTR) csField2Name, buffer2, sizeof(buffer2), csRawText2);
		pDBA2->ADOGetString((LPTSTR) (LPCTSTR) csField3Name, buffer3, sizeof(buffer3), csRawText3);

		csRawText1.TrimRight();
		csRawText2.TrimRight();
		csRawText3.TrimRight();

		PrintMessage(MSGTYPE_SYSTEM,
						hwndDatabaseMsg,
						"Count: [%d] [%s]: [%s] [%s]: [%s] [%s]: [%s]",
						x,
						csField1Name,
						csRawText1,
						csField2Name,
						csRawText2,
						csField3Name,
						csRawText3);

		// check that ADOStep() was successful,
		//  if not, assume that end of file was encountered.
		if(SUCCESS != pDBA2->ADOStep())
			{
			// no success, end of file encountered.
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndDatabaseMsg, 
							"end of file found...");
			break;
			}
		}

	if(SUCCESS == pDBA2->ADOConnectionRelease())
		{
		csTemp = "release OK";
		}
	else
		{
		csTemp = "release NG";
		}

	// delete the database accessor object.
	delete pDBA2;

	CoUninitialize();
	}


	void
SQLSelectAndRetrieveTest(int iMaxRuntime, CStringArray & csaText)
	{
	CString csTemp;
	CString csRawText1, csRawText2, csRawText3;
	DataBaseAccessor * pDBA2;

	CString csFunctionName = "SQLSelectAndRetrieveTest";

	CString csTableName; csTableName = csaText[0];
	CString csSQLString; csSQLString = csaText[1];
	CString csField1Name; csField1Name = csaText[2];
	CString csField2Name; csField2Name = csaText[3];
	CString csField3Name; csField3Name = csaText[4];
#ifdef NOCO
	if(S_OK == CoInitialize(NULL))
		{
		csTemp = "CoInitialize OK";
		}
	else
		{
		csTemp = "CoInitialize NG";
		}
#endif
	// create the database accessor object.
	pDBA2 = new DataBaseAccessor();
	pDBA2->SetConnectString(param.ConnectStringNetwork);

	if(SUCCESS == pDBA2->ADOConnectionConnect())
		{
		csTemp = "connect OK";
		}
	else
		{
		csTemp = "connect NG";
		}

	// skip error check for now, put back in later.
	
	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg, 
					"before ADORsSelect");

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg, 
					"  time limit [%d]", 
					iMaxRuntime);

	// set timeout property.
	pDBA2->SetCommandTimeout(iMaxRuntime);

	pDBA2->ADORsSelect(csFunctionName, csTableName, csSQLString);

	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"after ADORsSelect");


	for(int x = 0; x < 3; x++)
		{
		char buffer1[10];
		char buffer2[10];
		char buffer3[10];

        ZeroMemory(buffer1, sizeof(buffer1));
        ZeroMemory(buffer2, sizeof(buffer2));
        ZeroMemory(buffer3, sizeof(buffer3));

		pDBA2->ADOGetString((LPTSTR) (LPCTSTR) csField1Name, buffer1, sizeof(buffer1), csRawText1);
		pDBA2->ADOGetString((LPTSTR) (LPCTSTR) csField2Name, buffer2, sizeof(buffer2), csRawText2);
		pDBA2->ADOGetString((LPTSTR) (LPCTSTR) csField3Name, buffer3, sizeof(buffer3), csRawText3);

		csRawText1.TrimRight();
		csRawText2.TrimRight();
		csRawText3.TrimRight();

		PrintMessage(MSGTYPE_SYSTEM,
						hwndDatabaseMsg,
						"Count: [%d] [%s]: [%s] [%s]: [%s] [%s]: [%s]",
						x,
						csField1Name,
						csRawText1,
						csField2Name,
						csRawText2,
						csField3Name,
						csRawText3);

		// check that ADOStep() was successful,
		//  if not, assume that end of file was encountered.
		if(SUCCESS != pDBA2->ADOStep())
			{
			// no success, end of file encountered.
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndDatabaseMsg, 
							"end of file found...");
			break;
			}
		}

	if(SUCCESS == pDBA2->ADOConnectionRelease())
		{
		csTemp = "release OK";
		}
	else
		{
		csTemp = "release NG";
		}

	// delete the database accessor object.
	delete pDBA2;

	CoUninitialize();
	}


	void
SQLSequence3(void)
	{
	CStringArray csaText;

	csaText.Add("Xkm");									// table name
	csaText.Add("SELECT TOP 5 * FROM Xkm --SQL-1-9010");		// select statement
	csaText.Add("ProfileId");							// fields to retrieve
	csaText.Add("PagerId");								//
	csaText.Add("Password");							//

	SQLSelectAndRetrieve3(5, csaText);
	}

	void
SQLSequence4(void)
	{
	CStringArray csaText;

	csaText.Add("Pagers");								// table name
	csaText.Add("SELECT * FROM Pagers  --SQL-1-9011");	// select statement
	csaText.Add("ProfileId");							// fields to retrieve
	csaText.Add("PagerId");								//
	csaText.Add("Status");								//

	SQLSelectAndRetrieve3(5, csaText);
	}

	void
SQLSequence5(void)
	{
	CStringArray csaText;

	csaText.Add("XnFrq");								// table name
	csaText.Add("SELECT * FROM XnFrq  --SQL-1-9012");	// select statement
	csaText.Add("ProfileId");							// fields to retrieve
	csaText.Add("FrqType");								//
	csaText.Add("FrqNumber");							//

	SQLSelectAndRetrieve3(5, csaText);
	}


	void
XnUpdateProfile(int, char *)
	{
	}


	void
XnInsertProfile(int, char *)
	{
	}


	
//*********************************************
//*** DATABASE LOOKUPS FOR PAGE ALL DEVICES *** 
//*********************************************
int
CheckIfAllPageFlagIsOn(int PagePointer)
{
	// Ignoring PageAllDevice flag in case current contact has Coverage status set.
	if (!ignoreCoverageWhenPageAllDevices
		&& (page[PagePointer].ACKMin != -1 && Status[page[PagePointer].ACKMin].Page[0] == 'C'))
	{
		return FAIL;
	}

	// Ignoring PageAllDevice flag in case current contact has Unavailable or Forward status set.
	if (!ignoreStatusWhenPageAllDevices
		&& (page[PagePointer].ACKMin != -1 
			&& (Status[page[PagePointer].ACKMin].Page[0] == 'N' || Status[page[PagePointer].ACKMin].Page[0] == 'F')))
	{
		return FAIL;
	}

	int status;
	CString csSQLString;
	CString csFunctionName = "CheckIfAllPageFlagIsOn";
	CString csTableName;

	//check if page all devices flag is Y which means the feature is turned on
	csTableName = "Xkm";
	csSQLString.Format(
		"SELECT  %s "
		"FROM Xkm "
		"WHERE ProfileId = '%10.10s' "
		"AND %s = 'Y' "
		"--SQL-1-2000",
		g_csYNFlagNameToPageAllDevices,
		page[PagePointer].Extension,
		g_csYNFlagNameToPageAllDevices);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	//Found the record with YNFlag for page all devices with Y setting 
	//This means that page all devices turned on
	if (DBA_RECORD_FOUND == status)
	{
		//page all devices turned on
		return SUCCESS;
	}
	//page all devicess turned off
	return FAIL;
}

//SOguz - 6/17/2016 - This function will find all the pageable devices for a given profile
int
GetAllPagingDevicesFromProfile(int PagePointer)
{
	CString csSQLString;
	CString csFunctionName = "GetAllDevicesFromProfile";
	CString csTableName;
	CString tmpStr;
	CString csINClause;
	char bufferFrqType [85];
	char bufferFrqNumber[85];
	int iQueueControlSize = MAX_QUEUE_FILES;
	int iFoundAt = 0;
	int status;
	int retValue = 0;
	int countOfAllPageableDevices = 0;

	// create inside "IN" operator for the SQL statement based on the pager types in Queue Control
	for (int i = 0; i<iQueueControlSize; i++)
	{
		if (Queue[i].Id != NULL)
		{
			
			tmpStr = CString(Queue[i].Id);
			//check if % exists; this will help us determine if value is empty 
			iFoundAt = tmpStr.Find('%', iFoundAt);
			
			if(iFoundAt == -1)
			{
				// none found, or no more found. Therefore exit.
				break;
			}
			else
			{
				//first record don't add comma
				if (i == 0)
				{
					csINClause = "'" + tmpStr + "'";
				}
				else
				{
					//rest of the records; add comma except the last record
					csINClause = csINClause + "," + "'" + tmpStr + "'";
				}
			}
		}
	}

	//QUERY: select the count of pageable devices from Xnfrq for that profile
	csTableName = "XnFrq";
	csSQLString.Format(
		"SELECT  COUNT(*) AS frqInfoCount "
		"FROM XnFrq "
		"WHERE ProfileId = '%10.10s' "
		"AND FrqType IN (%s)"
		"--SQL-1-2001",
		page[PagePointer].Extension,
		csINClause);

	status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	//run the sql statement and get the count of pageable devices from the "frqInfoCount" field
	if (DBA_RECORD_FOUND == status)
	{
		pDBA->ADOGetNumeric("frqInfoCount", &countOfAllPageableDevices);
				
		if (countOfAllPageableDevices > 0) 
		{
			//we got some pageable devices; copy it to return value
			retValue = countOfAllPageableDevices;
		}
		else
		{
			//we don't have any pageable devices set countOfAllPageableDevices to 0
			countOfAllPageableDevices = 0;
		}

		//Continue only if there are pageable devices
		if (countOfAllPageableDevices > 0 && countOfAllPageableDevices < 20)
		{
			//Now that we have pageable devices, get frqtype and frqnumber from Xnfrq for that profile
			csTableName = "XnFrq";
			csSQLString.Format(
				"SELECT  ProfileId, FrqType, FrqNumber "
				"FROM XnFrq "
				"WHERE ProfileId = '%10.10s' "
				"AND FrqType IN (%s)"
				"--SQL-1-2002",
				page[PagePointer].Extension,
				csINClause);

			status = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

	
			//check if frqtype and frq number found 
			if (DBA_RECORD_FOUND == status)
			{

				for (int i=0; i < countOfAllPageableDevices; i++)
				{
					memset(bufferFrqType, 0, sizeof(bufferFrqType));
					memset(bufferFrqNumber, 0, sizeof(bufferFrqNumber));

					pDBA->ADOGetString("FrqType", bufferFrqType, 3); //Get FrqType

					memset(AllPageableDevices[i].FrqType, 0, sizeof(AllPageableDevices[i].FrqType));  
					sprintf(AllPageableDevices[i].FrqType, "%-3.3s", bufferFrqType);
					
					PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"PageAllDevices is in GetAllPagingDevicesFromProfile() method. - Processing 'FrqType' [%s]", 
					bufferFrqType);

					pDBA->ADOGetString("FrqNumber", bufferFrqNumber, 65);//Get FrqNumber

					memset(AllPageableDevices[i].FrqNumber, 0, sizeof(AllPageableDevices[i].FrqNumber)); 
					sprintf(AllPageableDevices[i].FrqNumber, "%-64.64s", bufferFrqNumber);

					PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"PageAllDevices is in GetAllPagingDevicesFromProfile(): - Processing 'FrqNumber' [%s]", 
					bufferFrqNumber);

					//move to next record
					if (pDBA->ADOStep() == FAIL)
					{
						break;
					} 
				}
			}
		}
	}
	//return the number of pageable devices
	return retValue;
}
