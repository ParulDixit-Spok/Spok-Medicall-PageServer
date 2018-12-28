
// xnschado.cpp


//#include <afx.h>		// for CString	
#include <atlstr.h>
#include "common.h"
// disable deprecation
#pragma warning(disable:4996)


	int
SetSchedule(int pagePointer)
	{
	int ReturnStatus = FALSE;
	int SQLStatus;
	int updateFlag = FALSE;
	char tempstr[200];
	char GeneralInformation[250 + 1];
	char Header1[12 + 1];		// indicates time of an existing schedule
	char Header2[12 + 1];		// indicates the time of ....
	CString csSQLString;
	CString csFunctionName = "SetSchedule";
	CString csTableName = "Schedule";

 	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// when externally controlled, and in the standby mode, 
		//  no database can be written to, so just return.
		return TRUE;
		}

   if(DATA_SOURCE_NETWORK == DataSource)
		{
		// check for "unavailable until" status.
		if(Status[page[pagePointer].NextAckPtr].Page[0] == 'U')
			{
			// status is already 'U' == "UNV" == unavailable until.
			//  check for a previous schedule that is currently in effect.
			//  if already unavailable, then a change is in order.
			PrintMessage(MSGTYPE_DEBUG, hwndD4xMsg, "SetSchedule(): step 3");

			// because the status is currently "unavailable until",
			//  we want to find and update rather than set
			//  an additional schedule item.
			updateFlag = TRUE;

			// build the header field, containing date and time information.
			//  of the form YYYYMMDDHHMM
			// setup to search for a record matching this time in schedule table.
			memset(Header1, 0, sizeof(Header1));
			sprintf(Header1,
					"%4.4s"
					"%2.2s"
					"%2.2s"
					"%4.4s",
					page[pagePointer].PageInfo +  6,
					page[pagePointer].PageInfo +  0,
					page[pagePointer].PageInfo +  3,
					page[pagePointer].PageInfo + 11);

			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 1.9 - Header1 [%s]", Header1);

			csSQLString.Format(
								"SELECT Header, RecordType, Extension, "
								"RecordCount, TimeType, Frequency, ScheduleTime "
								"FROM Schedule "
								"WHERE RecordType = 11 "
								"AND Extension = '%10.10s' "
								"AND Header = '%s' "
								"--SQL-1-1101",
								page[pagePointer].Extension,
								Header1);

			SQLStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Schedule search: return [%i]", SQLStatus);
			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 4");

			if(DBA_RECORD_FOUND != SQLStatus)
				{
				PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Previous schedule not found");

				PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 5");
				return FALSE;
				}
			}

		// at this point it could be add new, or update,
		//  prepare the required data for either case.
		PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 7");

		memset(tempstr, 0, sizeof(tempstr));
	    strcpy(tempstr, page[pagePointer].tempStr);
        strcat(tempstr, ":Exception");

		PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 8");

		// build the header field, containing date and time information.
		//  of the form YYYYMMDDHHMM
		// setup to enter this time into the schedule table.
		memset(Header2, 0, sizeof(Header2));
		sprintf(Header2,
				"%4.4s"
				"%2.2s"
				"%2.2s"
				"%4.4s",
				page[pagePointer].tempStr +  6,
				page[pagePointer].tempStr +  0,
				page[pagePointer].tempStr +  3,
				page[pagePointer].tempStr + 11);

		PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 1.9 - Header2 [%s]", Header2);

		// continuing on, based on the update flag,
		//  update the existing schedule item,
		//  or create a new schedule item.
		if(updateFlag == FALSE)
			{
			// no update, a new schedule item should be added.
			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "SetSchedule(): step 8.5");

			// this is the format of an exception record.
			//  required to change the page status at some
			//  future time, the "Target Time".
			//
			memset(GeneralInformation, 0, sizeof(GeneralInformation));
			sprintf(GeneralInformation,
					// "schedule record" #1
					"%10.10s"			// target profile Id.
					"%c"				// current status
					"%-65.65s"			// target time in "format6". "MM DD YYYY MM DD HHMM:Exception"
					"%10.10s"
					"%-10.10s"
					"%12.12s"
					"%c"				// status.
					"%c"				// ignore status flag.

					// "schedule record" #2
					"%10.10s"			
					"%c"				
					"%-65.65s"			
					"%10.10s"
					"%-10.10s"
					"%12.12s"
					"%c"				
					"%c",				

					// data for "schedule record" #1
					page[pagePointer].Extension,
					(char) (page[pagePointer].ACKMin + 48),			// current page status.
					tempstr,										// contains target time in "format6".
					"",												// empty
					"Page Srv",										// requester signature
					"",												// empty
					'2',											// always == '2'
					' ',

					// data for "schedule record" #2
					page[pagePointer].Extension,
					(char) (page[pagePointer].NextAckPtr + 48),		// target page status.
					"",												// no time specified.
					"",												// empty
					"Page Srv",										// requester signature									
					"",												// empty
					'2',											// always == '2'
					' ');

			csSQLString.Format(	
								"INSERT INTO Schedule "
								"(Header, RecordType, Extension, RecordCount, TimeType, Executed, "
								"Frequency, ScheduleTime, GeneralInformation) "
								"VALUES ('%s', %d, '%10.10s', %d, %d, %d, %d, '%s', '%-s') "
								"--SQL-1-1201",

								Header2,
								11,
								page[pagePointer].Extension,
								0,
								0,
								0,
								0,
								"",
								GeneralInformation);

			SQLStatus = pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
			}
		else
			{
			// updateFlag == TRUE, update of existing schedule item required.

			// this is the format of an exception record.
			//  required to change the page status at some
			//  future time, the "Target Time".
			//
			memset(GeneralInformation, 0, sizeof(GeneralInformation));
			sprintf(GeneralInformation,
					"%10.10s"			// target profile Id.
					"%c"				// current status, pageserver just changed this to "unavailable until"
					"%-65.65s"			// target time in a strange format. "MM DD YYYY MM DD HHMM:Exception"
					"%10.10s"
					"%-10.10s"
					"%12.12s"
					"%c"				// target status, typically "available for page"
					"%c",
					page[pagePointer].Extension,
					(char) (page[pagePointer].ACKMin + 48),
					tempstr,
					"",
					"XN",
					Header2,
					(char) (page[pagePointer].NextAckPtr + 48),	
					'0');

			csSQLString.Format(	
								"UPDATE Schedule "
								"SET Header = '%s', "
								"RecordCount = 2, "
								"GeneralInformation = '%-s' "
								"WHERE RecordType = 11 "		
								"AND Extension = '%10.10s' "
								"--SQL-1-1301",
								Header2,
								GeneralInformation,
								page[pagePointer].Extension
								);

			SQLStatus = pDBA->ADORsUpdate(csFunctionName, csTableName, csSQLString);
			}

		if(SUCCESS != SQLStatus)
			{
			}	
		else
			{
			ReturnStatus = TRUE;
			}
		}
	return ReturnStatus;
	}


	int
SchedulePage(int pagePointer, int MinutesFromNow)
	{
	int ReturnStatus = FALSE;
	int SQLStatus;
	SYSTEMTIME stTime;
	CString csSQLString = "";
	CString csFunctionName = "SchedulePage";
	CString csTableName = "Schedule";
	char Header1[12 + 1];
	char GeneralInformationText[250 + 1];

	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// when externally controlled, and in the standby mode, 
		//  no database can be written to, so just return.
		return TRUE;
		}

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Scheduled Page %s.", param.schdBtrvFile);

    if(DATA_SOURCE_NETWORK == DataSource)
		{		
		GetLocalTime(&stTime);
		// get local time into a SYSTEMTIME structure
		AddTimeSeconds(stTime, MinutesFromNow * 60);	// add xx minutes
	
		memset(Header1, 0, sizeof(Header1));
		sprintf(Header1,
				"%04.4d%02.2d%02.2d%02.2d%02.2d",
				stTime.wYear,
				stTime.wMonth,
				stTime.wDay,
				stTime.wHour,
				stTime.wMinute);

		memset(GeneralInformationText, 0, sizeof(GeneralInformationText));
		sprintf(GeneralInformationText,
				"%10.10s%c%2.2s%c%-235.235s",
				page[pagePointer].Extension,
				'%',
				page[pagePointer].PageType,
				'1',
				page[pagePointer].DisplayInfo);

		csSQLString.Format(	
							"INSERT INTO Schedule "
							"(Header, RecordType, Extension, RecordCount, TimeType, "
							"Executed, Frequency, ScheduleTime, GeneralInformation) "
							"VALUES ('%s', %d, '%10.10s', %d, %d, %d, %d, '%s', '%-s') "
							"--SQL-1-1202",
							Header1,
							12,
							page[pagePointer].Extension,
							0,
							0,
							0,
							0,
							"",
							GeneralInformationText);			// 250 + 1

		SQLStatus = pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);

		if(FAIL == SQLStatus)
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ERROR! %d, Unable to SchedulePage().",
							SQLStatus);
			}
		else
			{
			ReturnStatus = TRUE;
			}
		}

	return ReturnStatus;
	}


	// SetActionReminder() - writes a "request record" to 
	//   the Xtend "Scheduler Facility".
	//
	//  an Xtend "action reminder" message is sent
	//  to notify the attendant (telephone operator) 
	//  of a significant pageserver event, such as 
	//  the shutdown of the page server application.
	//
	//  for the case of "AlarmType" == "20": 
	//    if a message of this type does not already exist,
	//    a new one is created, and scheduled for 5 minutes 
	//    into the future from the present time. 
	//
	//    if a message already exists, it is updated, 
	//    essentially re-scheduled to 5 minutes into the future
	//    from the present time, thus postponing the delivery 
	//    of the message.
	//
	//    the purpose of the "AlarmType" == "20" message is to 
	//    inform the attendant that the page server is apparently not
	//    doing its job.  when the page server is running, and has
	//    connectivity to the database, and is able to update 
	//    this record, the request will be periodically re-scheduled
	//    so that it never occurs, that is, it is always pushed
	//    5 minutes into the future when things are working correctly.
	//    when the system breaks down, pageserver will be unable to 
	//    re-schedule, and the message will be delivered.
	//
	//  types other than "21", e.g. "8 ", "20", "22", etc. 
	//  are immediately requested.
	//
	// iCommand parameter is used to ADD, UPDATE, or DELETE messages
	// iSeconds parameter is used for future time message delivery
	//   as in the case of a type "20" (keep-alive) message
	//
	void
SetActionReminder(char * Extension,
				  char *,		// unused here, see btrieve version of this function.
				  int,			// unused here, see btrieve version of this function.
				  int iCommand, 
				  int iSeconds,
				  const char * GeneralInformationText)
	{
	int SQLStatus;
	SYSTEMTIME stTime;
	CString csSQLString;
	CString csFunctionName = "SetActionReminder";
	CString csTableName = "Schedule";
	char Header[12 + 1];
	int iRecordType = 6;

	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// when externally controlled, and in the standby mode, 
		//  no database can be written to, so just return.
		return;
		}

	// feature is only to be used if network mode is active.
	//  the scheduler facility only responds to requests
	//  written to the network database.
	//
    if(DATA_SOURCE_NETWORK == DataSource)
		{
		memset(Header, 0, sizeof(Header));

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"SetActionReminder(): ProfileId: [%s]  Command: [%s]",
						Extension,
						pszActionReminderCommandText[iCommand]);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"                     GeneralInformation: [%s]",
						GeneralInformationText);

		// add specified time interval, often this is zero seconds.
		GetLocalTime(&stTime);

		AddTimeSeconds(stTime, iSeconds);	// add xx seconds

		sprintf(Header,
				"%04.4d%02.2d%02.2d%02.2d%02.2d",
				stTime.wYear,
				stTime.wMonth,
				stTime.wDay,
				stTime.wHour,
				stTime.wMinute);

		switch(iCommand)
			{
			case REMINDER_ADD:
				// REMINDER_ADD expects an actual add/insert.

				csSQLString.Format(	
									"INSERT INTO Schedule "
									"(Header, RecordType, Extension, RecordCount, "
									"TimeType, Executed, Frequency, ScheduleTime, "
									"GeneralInformation) "
									"VALUES ('%s', %d, '%10.10s', "
									"0, 0, 0, 0, '', '%-s') "
									"--SQL-1-1210",
									Header,
									iRecordType,
									Extension,
									GeneralInformationText);

				SQLStatus = pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);

				if(FAIL == SQLStatus)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"SetActionReminder(): ERROR! [%d], Unable to INSERT",
									SQLStatus);
					}

				break;

			case REMINDER_UPDATE:
				// REMINDER_UPDATE implies SQL INSERT if the record does not exist.
				
				// use select to see if the record exists.
				csSQLString.Format(
									"SELECT RecordType, Extension, GeneralInformation "
									"FROM Schedule "
									"WHERE Extension = '%10.10s' "
									"AND RecordType = %d "
									"AND GeneralInformation = '%-s' "
									"--SQL-1-1120",
									Extension,
									iRecordType,
									GeneralInformationText);

				SQLStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

				switch(SQLStatus)
					{
					case DBA_RECORD_FOUND:
						// record exists, attempt SQL UPDATE.
						csSQLString.Format(	
											"UPDATE Schedule "
											"SET Header = '%s', "
											"GeneralInformation = '%-s' "
											"WHERE Extension = '%10.10s' "
											"AND RecordType = %d "
											"AND GeneralInformation = '%-s' "
											"--SQL-1-1306",
											Header,
											GeneralInformationText,
											Extension,
											iRecordType,
											GeneralInformationText);

						SQLStatus = pDBA->ADORsUpdate(csFunctionName, csTableName, csSQLString);

						if(FAIL == SQLStatus)
							{
							PrintMessage(MSGTYPE_SYSTEM, 
											hwndGeneralMsg,
											"SetActionReminder(): ERROR! [%d], Unable to UPDATE",
											SQLStatus);
							}
						break;

					case DBA_RECORD_NOT_FOUND:
					case DBA_RECORD_ACCESS_ERROR:
					default:
						// record not found, attempt an SQL INSERT.
						csSQLString.Format(	
											"INSERT INTO Schedule "
											"(Header, RecordType, Extension, RecordCount, "
											"TimeType, Executed, Frequency, ScheduleTime, "
											"GeneralInformation) "
											"VALUES ('%s', %d, '%10.10s', "
											"0, 0, 0, 0, '', '%-s') "
											"--SQL-1-1203",
											Header,
											iRecordType,
											Extension,
											GeneralInformationText);

						SQLStatus = pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);

						if(FAIL == SQLStatus)
							{
							PrintMessage(MSGTYPE_SYSTEM, 
											hwndGeneralMsg,
											"SetActionReminder(): ERROR! [%d], Unable to INSERT",
											SQLStatus);
							}
						break;
					}
				break;

			case REMINDER_DELETE:
				// perform minimal effort delete, application is ending.
				csSQLString.Format(
									"DELETE FROM Schedule "
									"WHERE Extension = '%10.10s' "
									"AND RecordType = %d "
									"AND GeneralInformation = '%-s' "
									"--SQL-1-1401",
									Extension,
									iRecordType,
									GeneralInformationText);

				pDBA->ADORsDelete(csFunctionName, csTableName, csSQLString);
				break;
			}
		}
	}


	int
WriteToScheduleTable(int pagePointer, int DatabaseMessageNumber)
	{
	int ReturnStatus = FALSE;
	int SQLStatus;
	SYSTEMTIME stTime;
	CString csSQLString = "";
	CString csFunctionName = "WriteToScheduleTable";
	CString csTableName = "DeliverySchedule";
	char Header1[12 + 1];
	//char GeneralInformationText[250 + 1];

	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// when externally controlled, and in the standby mode, 
		//  no database can be written to, so just return.
		return TRUE;
		}

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Write to DeliverySchedule table.");

    if(DATA_SOURCE_NETWORK == DataSource)
		{		
		GetLocalTime(&stTime);
	
		memset(Header1, 0, sizeof(Header1));
		sprintf(Header1,
				"%04.4d%02.2d%02.2d%02.2d%02.2d",
				stTime.wYear,
				stTime.wMonth,
				stTime.wDay,
				stTime.wHour,
				stTime.wMinute);

		// choose the data to be written to DeliverySchedule table.
		//  more Mobile Enablement requirements implemented here.
		//  similar to code in AddQueue ().
		//
		CString DeliveryScheduleMessage = "";
		CString csDisplayInfo = page[pagePointer].DisplayInfo;			// convert from character array.
		CString csLongMessage = page[pagePointer].longMessage.c_str();	// convert from STL string.

		if(csLongMessage.GetLength() > 0)
			{
			// use longMessage.
			DeliveryScheduleMessage = csLongMessage;
			}
		else if(csDisplayInfo.GetLength() > 0)
			{
			// use DisplayInfo.
			DeliveryScheduleMessage = csDisplayInfo;
			}
		else
			{
			// no message.
			DeliveryScheduleMessage = "No Message.";
			}
		
		csSQLString.Format(	
							"INSERT INTO DeliverySchedule ("
							"DeliveryType, "			// c-1
							"Date_Time, "				// c-12
							"ProfileId, "				// c-10
							"MessageNumber, "			// i
							"DeliveryFlag, "			// c-1
							"OperatorId, "				// c-10
							"DeliveryAddr, "			// c-150
							"MessagePriority, "			// i
							"MessageToDeliver "			// c-1024
							") VALUES ('%s', '%s', '%10.10s', %d, '%s', '%s', '%s', %d, '%-1000s') " //US30642 - SO - 06/11/2014 - changed the MessageToDeliver value from '%-s' to '%-1000s' 
																									//so that it can truncate the message to 1000 characters due to limitation 
																									//of the MessageToDeliver field in DeliverySchedule table.
							"--SQL-1-1211",
							"W",
							Header1,
							page[pagePointer].Extension,
							DatabaseMessageNumber,
							"C",
							page[pagePointer].InitiatorId,
							page[pagePointer].PagerNumber,
							0,
							(LPTSTR) (LPCTSTR) EscapeSqlText(DeliveryScheduleMessage)
							);		

		SQLStatus = pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);

		if(FAIL == SQLStatus)
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ERROR! %d, Unable to WriteToScheduleTable().",
							SQLStatus);
			}
		else
			{
			ReturnStatus = TRUE;
			}
		}

	return ReturnStatus;
	}
