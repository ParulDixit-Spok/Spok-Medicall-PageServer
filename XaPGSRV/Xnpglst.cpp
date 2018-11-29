
// xnpglst.cpp

#pragma warning( disable: 4786 )
#pragma warning( disable: 4514 )

#include "common.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>

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


extern void OpenListPageTextReport(char *);
extern void WriteListPageTextReport(char *);
extern void CloseListPageTextReport(void);

using namespace std;

// workspace data item.
//LIST_DATA ListData = {"Text Info", "", "", 1, 2, 3};

// list processng vector and map items.

// allocate 1 vector container, and 1 vector iterator
vector<string> veccntPageList;
vector<string>::iterator vecitrPageList;

// allocate 1 map container, and 1 map iterator.
map<string, LIST_DATA> mapcntPageList;
map<string, LIST_DATA>::iterator mapitrPageList;


#if 0
	// utility function to convert a standard library
	//  "string" type into a standard "C" language character array.
	//  note: there has to be a better way
	//        to do this that I have not discovered yet.
	//  the string .copy member does some of this but is not
	//   significantly more convenient. ... still searching.
	//
	char *
AAssign(char * destination, string source)
	{
	int p = 0;

	while(source[p])
		{
		// copy source to destination.
		destination[p] = source[p];
		p++;
		}

	// ensure termination on destination string.
	destination[p] = 0;

	// for the convenience of the caller,
	//  return a pointer to the beginning
	//  of the destination string.
	return destination;
	}
#endif

	void
ProcessListQueue(void)
	{
    int i;
    char string3[20];
	char string2[200];
	int counter = 1; 

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ProcessListQueue(): Begin ++++++++++++++++++++++++++");

    if(strcmp(ListQue.ListExtension, page[PAGE_LIST_POS].Extension) == 0  && ListQue.OnCallActive == FALSE)
		{
        return;
		}

	//voice page type.
	//10/3/2016 - SO -Page all devices is not implemented for voice list paging 
	//since it is no longer being used by the customers
    if(ListQue.PageVoice[0] != 0)
		{
		GetPagerId(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS, BY_PROFILEID);

		LogPager(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS);

		if(param.ListStatus)
			{
			if(page[PAGE_LIST_POS].ACKMin == -1 ||
				Status[page[PAGE_LIST_POS].ACKMin].Page[0] != 'Y')
				{
				ListPagePrinting(MID_PAGE,
								"ProfileId: %s.",
								page[PAGE_LIST_POS].Extension);

				ListPagePrinting(MID_PAGE,
								"Page Type: %%%2.2s.",
								page[PAGE_LIST_POS].PageType);

				ListPagePrinting(MID_PAGE,
								"Invalid Page Status: %s.",
								Status[page[PAGE_LIST_POS].ACKMin].Text);

				ListPagePrinting(MID_PAGE,
								"Pager Id: %s.",
								page[PAGE_LIST_POS].PagerId);
				return;
				}
			}

		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ProcessListQueue(): Step 6");

		memset(page[PAGE_LIST_POS].PagerNumber, 0, sizeof(page[PAGE_LIST_POS].PagerNumber));
		memset(page[ PAGE_LIST_POS ].DisplayInfo, 0, sizeof( page[ PAGE_LIST_POS ].DisplayInfo ) );
		memset(page[ PAGE_LIST_POS ].AutCode, 0, sizeof( page[ PAGE_LIST_POS ].AutCode ) );

		if(ListQue.OnCallActive)
			{
			memset(page[PAGE_LIST_POS].FileName, 0, sizeof(page[PAGE_LIST_POS].FileName));
			}

        i = FALSE;
        if(param.CodeVoiceQues[0] != 0)
			{
            page[PAGE_LIST_POS].ComNumber = -1;
            page[PAGE_LIST_POS].ScriptNumber = -1;

			i = Get_Frq(PAGE_LIST_POS, param.CodeVoiceQues, param.VoiceQues2);
			}

        if(i == FALSE)
			{
			i = Get_Frq(PAGE_LIST_POS, param.VoiceQues, param.VoiceQues2);
			}

        if(i == TRUE)
			{
            memset(string2, 0, sizeof(string2));
            sprintf_s<200>(string2, "%4.4s", ListQue.PageVoice + strlen(ListQue.PageVoice) - 7);
            page[PAGE_LIST_POS].VoiceFile = atoi(string2);
            memset(string3, 0, sizeof(string3));

			// list is either a code-list or a CATS page
            if(strncmp(ListQue.InitiatorId, "CATS PG", 7) != 0)
				{
                strcpy_s<20>(string3, "Code List");
				}
            else
				{
                strcpy_s<20>(string3, "CATS PG");
				}

            AddSetUp(PAGE_LIST_POS, string3);

			page[PAGE_LIST_POS].PortNumber = GetQuePointer(PAGE_LIST_POS);

            if((page[PAGE_LIST_POS].PortNumber >= 0) || (page[PAGE_LIST_POS].PortNumber == -3) || IsSmsType(PAGE_LIST_POS))
				{
                if(AddQueueEx(PAGE_LIST_POS, page[PAGE_LIST_POS].PortNumber))
					{
					// AddQueue () was successful.
					//
					PrintMessage(MSGTYPE_DATABASE_COMMAND, hwndGeneralMsg, "ProcessListQueue(): 45 - non-zero");
					ListQue.voiceQueued = TRUE;

					sprintf_s<200>(string2,
							"voice paged, file %s, PagerId %s.",
							ListQue.PageVoice,
							page[PAGE_LIST_POS].PagerId);

					ListPagePrinting(MID_PAGE, string2);

					WriteListPageTextReport(string2);

					if(param.UserMsg[0] != 'N')
						{
						WriteUserLogEx(PAGE_LIST_POS, page[PAGE_LIST_POS].PortNumber);
						}
					}
				else
					{
					sprintf_s<200>(string2,
							"voice page que error, Pager Id %s.",
							page[PAGE_LIST_POS].PagerId);

					if(param.CodePrint)
						{
						ListPagePrinting(MID_PAGE, string2);
						}

					WriteListPageTextReport(string2);
					}
				}
            else
				{
				ListQue.voiceQueued = TRUE;

                sprintf_s<200>(string2,
						"Extension %s List Voice Q add not ready %s, fh %i.",
						page[PAGE_LIST_POS].Extension,
						page[PAGE_LIST_POS].PageType,
						page[PAGE_LIST_POS].PortNumber);

                WriteLog("NOPGABRT", string2);

				ListPagePrinting(MID_PAGE, "");
				ListPagePrinting(MID_PAGE, string2);
				ListPagePrinting(MID_PAGE, "");

				WriteListPageTextReport(string2);
				WriteListPageTextReport("           ");

                PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);
				}
			}
        else
			{
			ListQue.voiceQueued = TRUE;

			WriteListPageTextReport("       Not Voice  Paged          ");
 			}
		}
	else
		{
		ListQue.voiceQueued = TRUE;
		}

    page[PAGE_LIST_POS].VoiceFile = FALSE;

	// numeric or alpha page type.
    if(ListQue.PageNumber[0] != 0 || ListQue.PageAlpha[0] != 0)
		{
		GetPagerId(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS, BY_PROFILEID);

		isPageAllDevicesOn = FALSE;

		// SOguz - DE20747 - 10/03/2016 - check if page all devices YNFlag is set to Y for the profile
		if (CheckIfAllPageFlagIsOn(PAGE_LIST_POS) == SUCCESS)
		{				
			PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"ListPage: PageAllDevices turned ON for profileid = %s",
							page[PAGE_LIST_POS].Extension);
			isPageAllDevicesOn = TRUE; // YNFlag is set to Y; therefore isPageAllDevicesOn to TRUE
			//SOguz -10/3/2016  - Get the number of devices for the profile
			counter = GetAllPagingDevicesFromProfile (PAGE_LIST_POS);	
		}
		else
		{
			PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"ListPage: PageAllDevices turned OFF for profileid = %s",
							page[PAGE_LIST_POS].Extension);
		}


		if(LogPager(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS) == FALSE)
			{
			sprintf_s<200>(string2,
					"Extension [%s] pager not assigned, check log file. 2",
					page[PAGE_LIST_POS].Extension);

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

			ListPagePrinting(MID_PAGE, "");
			ListPagePrinting(MID_PAGE, string2);
			ListPagePrinting(MID_PAGE, "");
			}

		// 5/19/2017 - DE24153 - Sedat - Added '&& isPageAllDevicesOn == FALSE' to the if statement so that we check
		// for page all devices flag before deciding not to send the page, due to profile's status.
		// If page all devices flag (isPageAllDevicesOn) is false then profile status will be respected. 
		// That means if the profile status is set to anything other than 'Y' then the page won't be sent.
		if(param.ListStatus)
			{
			if((page[ PAGE_LIST_POS ].ACKMin == -1 ||
				Status[ page[ PAGE_LIST_POS ].ACKMin ].Page[ 0 ] != 'Y') && isPageAllDevicesOn == FALSE)
				{
				ListPagePrinting(MID_PAGE,
								"Profile Id: [%s]. Page Type: [%%%2.2s].",
								page[PAGE_LIST_POS].Extension,
								page[PAGE_LIST_POS].PageType);

				PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"ListPage: Profile Id: [%s]. Page Type: [%%%2.2s].",
								page[PAGE_LIST_POS].Extension,
								page[PAGE_LIST_POS].PageType);

				ListPagePrinting(MID_PAGE,
								"Invalid Page Status: [%s]    Pager Id: [%s].",
								Status[page[PAGE_LIST_POS].ACKMin].Text,
								page[PAGE_LIST_POS].PagerId);

				PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"ListPage: Invalid Page Status: [%s]    Pager Id: [%s]. The page won't be sent to this profile. Please change the status to available.",
								Status[page[PAGE_LIST_POS].ACKMin].Text,
								page[PAGE_LIST_POS].PagerId);
				
				PrintMessage(MSGTYPE_SYSTEM,
							hwndGeneralMsg,
							"ListPage: The page won't be sent to Pager Id: [%s]. Please change the status to available.",								
								page[PAGE_LIST_POS].PagerId);

				// all done with this request, no retries required, set the success flag
				//  on this one, to proceed to the next one.
				//
				ListQue.pageQueued = TRUE;

				return;
				}
			}

		memset(page[PAGE_LIST_POS].PagerNumber,
				0,
				sizeof(page[PAGE_LIST_POS].PagerNumber));

		memset(page[ PAGE_LIST_POS ].DisplayInfo,
				0,
				sizeof( page[ PAGE_LIST_POS ].DisplayInfo ) );

		memset(page[ PAGE_LIST_POS ].AutCode,
				0,
				sizeof( page[ PAGE_LIST_POS ].AutCode ) );

		if( ListQue.OnCallActive )
			{
			memset(page[PAGE_LIST_POS].FileName, 0, sizeof(page[PAGE_LIST_POS].FileName));
			}

        i = FALSE;

		// SOguz - DE20747 - 10/03/2016  
		// Based on the number of devices for the profile the loop will run
		// to add each device into the appropiate queue for paging
		for (int idx=0; idx < counter; idx++)
		{
			// SOguz - DE20747 - 10/03/2016  
			// if Page all devices flag set for the profile then get frq type of the pager
			if (isPageAllDevicesOn)
			{			
				memset( string3, 0, sizeof( string3 ) );
				sprintf(string3, "%-3.3s", AllPageableDevices[idx].FrqType);
			}
			else
			{
				memset( string3, 0, sizeof( string3 ) );
				sprintf_s<20>( string3, "%%%s", page[PAGE_LIST_POS].PageType );
			}

			if(param.CodeQues[0] != 0)
				{
				page[PAGE_LIST_POS].ComNumber = -1;
				page[PAGE_LIST_POS].ScriptNumber = -1;

				i = Get_Frq(PAGE_LIST_POS, param.CodeQues, string3, idx);
				}

			if(i == FALSE && page[ PAGE_LIST_POS ].PageType[ 0 ] != 0)
				{
				if(strstr( param.VoiceQues, page[ PAGE_LIST_POS ].PageType ) == NULL)
					{
					i = Get_Frq(PAGE_LIST_POS, string3, string3, idx);
					}
				}

			if(i == TRUE && page[ PAGE_LIST_POS ].ScriptNumber != -1)
				{
				if(strchr( param.PageType[ page[ PAGE_LIST_POS ].ScriptNumber ].Script, 'V' ) != NULL )
					{
					i = FALSE;
					}
				}

			if(i == TRUE && param.MultiQueNumber2 > 0)
				{
				if( strncmp( string3, Queue[ param.MultiQueNumber2 ].Id, 3 ) == 0 )
					if( strstr( param.VoiceQues, string3 ) != NULL )
						i = FALSE;
				}

			if(i == TRUE)
				{
				strcpy_s<255>( page[ PAGE_LIST_POS ].DisplayInfo, ListQue.PageNumber );
				if( ListQue.PageAlpha[ 0 ] != 0  )
					{
					if( page[ PAGE_LIST_POS ].ComNumber > -1 )
						{
						if(param.ComPort[page[PAGE_LIST_POS].ComNumber].\
							QueType[page[PAGE_LIST_POS].ComOffset][1] == '1')
							{
							memset( page[ PAGE_LIST_POS ].DisplayInfo,
									0,
									sizeof( page[ PAGE_LIST_POS ].DisplayInfo ) );

							strcpy_s<255>( page[ PAGE_LIST_POS ].DisplayInfo, ListQue.PageAlpha );
							}
					}
					if( i == TRUE && param.MultiQueNumber2 > 0 )
						{
						if( strncmp( string3, Queue[ param.MultiQueNumber2 ].Id, 3 ) == 0 )
							{
							memset( page[ PAGE_LIST_POS ].DisplayInfo,
									0,
									sizeof( page[ PAGE_LIST_POS ].DisplayInfo ) );

							strcpy_s<255>( page[ PAGE_LIST_POS ].DisplayInfo, " +" );
							strcat_s<255>( page[ PAGE_LIST_POS ].DisplayInfo, ListQue.PageAlpha );
							}
						}
					}

				// Reseting longMessage to be empty in case of a pure numeric message 
				// as later based on longMessage emptiness is judged if the message is pure numeric.
				if (strncmp(page[ PAGE_LIST_POS ].DisplayInfo, ListQue.PageNumber, sizeof(ListQue.PageNumber)) == 0)
				{
					page[ PAGE_LIST_POS ].longMessage = "";
				}

				i = page[PAGE_LIST_POS].ACKMin;
				memset(string3, 0, sizeof(string3));

				if(strncmp(ListQue.InitiatorId, "CATS PG", 7) != 0)
					strcpy_s<20>(string3, "Code List");
				else
					strcpy_s<20>(string3, "CATS PG");

				AddSetUp(PAGE_LIST_POS, string3);

				page[PAGE_LIST_POS].PortNumber = GetQuePointer(PAGE_LIST_POS);

				if(AddQueueEx(PAGE_LIST_POS, page[PAGE_LIST_POS].PortNumber))
					{
					// AddQueue () was successful.
					//
					ListQue.pageQueued = TRUE;

					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Profile Id:   [%s] queued.",
									page[ PAGE_LIST_POS ].Extension);
					ListPagePrinting(END_TABLE_ROW, "");

				
					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Page Type:    [%%%2.2s]",
									page[ PAGE_LIST_POS ].PageType);
					ListPagePrinting(END_TABLE_ROW, "");

					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Date / Time:  [%10.10s]  [%8.8s]",
									page[PAGE_LIST_POS].InitiatedDate,
									page[PAGE_LIST_POS].InitiatedTime);
					ListPagePrinting(END_TABLE_ROW, "");


					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Page Status:  [%s]",
									Status[i].Text);
					ListPagePrinting(END_TABLE_ROW, "");

					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Pager Id:     [%s]",
									page[PAGE_LIST_POS].PagerId);
					ListPagePrinting(END_TABLE_ROW, "");

					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Pager Number: [%s]",
									page[PAGE_LIST_POS].PagerNumber);
					ListPagePrinting(END_TABLE_ROW, "");

					ListPagePrinting(BEGIN_TABLE_ROW, "");
					ListPagePrinting(MID_PAGE,
									"Auth code:    [%s]",
									page[PAGE_LIST_POS].AutCode);
					ListPagePrinting(END_TABLE_ROW, "");

					ListPagePrinting(END_TABLE, "");

					sprintf_s<200>(string2,
								"  %20.20s %20.20s  Page %s",
								page[PAGE_LIST_POS].tempStr ,Status[i].Text,
								page[PAGE_LIST_POS].PagerId);
					WriteListPageTextReport(string2);

					// option is controlled by the ReportingOptions Tab, "Leave User Message" option of the 
					//  PageServer Config. Possible values are CANCEL, EXTERN, NEVER. The first character only 
					//  is examined. Anyhing other than N for NEVER will write messages to the 
					//  list member (user) profiles.
					//
					if(param.UserMsg[0] != 'N')
						{
						WriteUserLogEx(PAGE_LIST_POS, page[PAGE_LIST_POS].PortNumber);
						}
					}
				else
					{
					ListPagePrinting(MID_PAGE,					
										"ProfileID: [%s] error: unable to AddQueue ().", 
										page[PAGE_LIST_POS].Extension);

					ListPagePrinting(MID_PAGE, "Page Type: [%%%2.2s]", page[PAGE_LIST_POS].PageType);
					ListPagePrinting(MID_PAGE, "Date: [%10.10s]", page[PAGE_LIST_POS].InitiatedDate);
					ListPagePrinting(MID_PAGE, "Time: [%8.8s]", page[PAGE_LIST_POS].InitiatedTime);

					sprintf_s<200>(string2,
							"                                  PagerId: [%s] error: unable to AddQueue ().",
							page[PAGE_LIST_POS].PagerId);

					WriteListPageTextReport(string2);
					}
				}
			
			else
				{
				i = page[PAGE_LIST_POS].ACKMin;
				ListQue.pageQueued = TRUE;

				sprintf_s<200>(string2,
						"Profile Id:   [%s] no alpha-numeric pager.",
						page[PAGE_LIST_POS].Extension);

				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

				ListPagePrinting(MID_PAGE, "");
				ListPagePrinting(MID_PAGE, string2);
				ListPagePrinting(MID_PAGE, "");

				sprintf_s<200>(string2,
						"   %25.25s  Not Paged   %s",
						Status[i].Text,
						page[PAGE_LIST_POS].PagerId);

				WriteListPageTextReport(string2);			
				}
			} //end for loop
		}
	else
		{
		ListQue.pageQueued = TRUE;
		}

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ProcessListQueue(): End ---------------------");
	}

// service routines to write to a list-page text report.
//  note that any existing report with this name is overwritten.
//

int fhListPageTextReport = -1;


	void
OpenListPageTextReport(char * PathAndFilename)
	{
	// if found to be open... close it.
	//
	if(fhListPageTextReport != -1)
		{
		CloseListPageTextReport();
		}


	// open for new report.
	errno_t err = _sopen_s(&fhListPageTextReport, PathAndFilename,
									_O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY,
									_SH_DENYNO,
									_S_IREAD | _S_IWRITE);
	if(err != 0)
	{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ERROR OPENING PAGE REPORT");
	}

	}


	void
WriteListPageTextReport(char * TextMessage)
	{
	if(TextMessage == NULL)
	{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "invalid parameter NULL WriteListPageTextReport");
		return;
	}
	if(fhListPageTextReport != -1)
		{
		// write the message.
		_write(fhListPageTextReport, TextMessage, strlen(TextMessage));	

		// followed by <cr> and <lf>
		_write(fhListPageTextReport, "\r\n", 2);	
		}
	}


	void
CloseListPageTextReport(void)
	{
	// close the file, if it is open.

	if(fhListPageTextReport != -1)
		{
		_close(fhListPageTextReport);
		}

	// mark it as closed.
	fhListPageTextReport = -1;
	}

// end list-page text report routines.


	// Check to see if the list queue needs to be processed.
	void
CheckStandardPriorityListQueue(void)
	{
	// if the queue is not defined, nothing to do...
	if(Queue[LOW_PRIORITY_LIST_QUEUE].Id[0] != '%')
		{
		return;
		}

	// post the standard queue information
	//  to the "queue status" window.
	UpdateQueueStatusWindow(LOW_PRIORITY_LIST_QUEUE);

	// check for items in low priority queue.
	//
	if(CheckQueue(LOW_PRIORITY_LIST_QUEUE) > 0)
		{
		// items found...
		// there are record(s) in the queue...
		//  attempt to caim one.
		//
		if(ClaimQueueItem(LOW_PRIORITY_LIST_QUEUE, 2) == TRUE)
			{
			// a queue record has been found,
			//  and claimed for processing.
			//
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "  low priority list item claimed:");

			// // clean up, and get ready for a new list page.
			ResetBuffer(PAGE_LIST_POS);

			// read from low priority flat-file queue
			GetQueue(PAGE_LIST_POS, -1, LOW_PRIORITY_LIST_QUEUE, false, __FILE__, __LINE__);		// read LOW priority
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "  low priority list item read from queue:");

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "1 - Page Info: [%s]", page[PAGE_LIST_POS].longMessage.c_str());


			// 2010-02-01 - it is necessary to reassemble the capcode info and message info, 
			//  this is a special case, where we are rewriting the queue, from %22 to %21.

			string NewLongMessage = "";
			string NewNumericPart = page[PAGE_LIST_POS].PagerNumber;
			string NewAlphaPart = page[PAGE_LIST_POS].longMessage.c_str();

			NewLongMessage.assign(NewNumericPart);
			NewLongMessage.append("+");
			NewLongMessage.append(NewAlphaPart);

			page[PAGE_LIST_POS].longMessage.assign(NewLongMessage);

			// //
			// //  this raw info is available, place it where it will be used...
			// //
			strncpy_s<148>(page[0].PageInfo, page[PAGE_LIST_POS].RawPageInfo, sizeof(page[0].PageInfo));			

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "2 - Page Info: [%s]", page[PAGE_LIST_POS].longMessage.c_str());

			AddQueueEx(PAGE_LIST_POS, HIGH_PRIORITY_LIST_QUEUE);	// write to HIGH priority

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "  low priority list item written to high priority queue:");
			}
		}
	else
		{
		// no low priority items found.
		//PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "  no low priority list items found.");
		}
	}


	// Check to see if the list queue needs to be processed.
	//  if there has been nothing processed in a few cycles, 
	//  consider transferring an item from the low-priority queue 
	//  into the high priority queue.
	void
CheckCodePriorityListQueue(void)
	{
	// StandardPriorityCount counter is used to make standard priority list paging 
	//  run only after high priority has been idle for approx 10 seconds.
	static int StandardPriorityCount = 0;
	
	int i;
	int j;
	char string2[200];
    char string3[30];
	char string5[200];
	char string7[200];

	ZeroMemory(string2, sizeof(string2));

	//2012-03-08 Added by LD

	ListMember_Covered = false;
	// if a list-page is in progress.
	//  a new one cannot be started,
    if((param.ListPass[0] != 0) && (ListQue.Active == FALSE))
		{
		// a list password is defined,
		//  and no list page is currently active.

		// start the timer if not already running.
        if(ListQue.QTimer.wHour == 70)
			{
			GetLocalTime(&ListQue.QTimer);
			}

		// check for new items in the list queue every XX seconds.
		//
        if(Wait(&ListQue.QTimer, (long) (2 * 100)))
			{
			// time interval has passed.
			//PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, 
			//	"CheckCodePriorityListQueue(): 2 seconds... checking...");

			// stop the timer, it will be started next time thru if it is not running.
            ListQue.QTimer.wHour = 70;

			// remember, Queue[0], the first element is
			//  "system-defined == hard-coded" as the list-page queue.
			//  and the "page[106]" element is assigned for list-paging.
            if(CheckQueue(HIGH_PRIORITY_LIST_QUEUE) > 0)
				{
				// there is an item available in the list queue,
				//  a list page to be processed.
				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, 
					"CheckCodePriorityListQueue(): list-queue item detected");

				// any action on a new high priority item will restart 
				//  the standard priority delay count.
				StandardPriorityCount = 0;	

				if(ClaimQueueItem(HIGH_PRIORITY_LIST_QUEUE, 2) == TRUE)
					{
					// a queue record has been found, and claimed.
					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, 
						"CheckCodePriorityListQueue(): list-queue item claimed");

					// clean up, and get ready for the new list page.
					ResetBuffer(PAGE_LIST_POS);

					// Want3 = false. - for "list page"
					GetQueue(PAGE_LIST_POS, -1, HIGH_PRIORITY_LIST_QUEUE, false, __FILE__, __LINE__);

					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"CheckCodePriorityListQueue(): list-queue item data read from NOPG21.QUE");

					// display the almost "raw" copy of the contents of the
					//  paging script, for list-page processing use.
					//  not quite raw because Set_Null() used to cleanup first.
					Set_Null(page[PAGE_LIST_POS].RawPageInfo,
								sizeof(page[PAGE_LIST_POS].RawPageInfo));

					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
									"RawPageInfo [%s]",
									page[PAGE_LIST_POS].RawPageInfo);

					// a list page was found in the queue,
					//  begin processing it now.

					// the numeric, alpha, and voice page
					//  data should be in the appropriate data items
					//  of the page[PAGE_LIST_POS]. structure.

					// read the btrieve list page file "xtpglst.btr".
					// retrieve the first 3 records in the list,
					//  these are defined as:
					//  default numeric,
					//  default alpha,
					//  and default voice list-page path and filename
					//   (as seen by the page server).
					memset(string3, 0, sizeof(string3));
					sprintf_s<30>(string3, "%10.10s000          ", page[PAGE_LIST_POS].Extension);

					memset(ListQue.ListExtension, 0, sizeof( ListQue.ListExtension));
					strcpy_s<11>(ListQue.ListExtension, page[PAGE_LIST_POS].Extension);

					// create a text file of the list page processing details
					//  the filename is based on the extension number
					//  and is of the form "2000.TXT", and located on the network
					//  if operating in network mode, and on the local drive
					//  if operating in standalone/failsafe mode.
					if(DATA_SOURCE_NETWORK == DataSource)
						{
						strcpy_s<200>(string2, param.AutoDir);
						}
					else
						{
						strcpy_s<200>(string2, param.BkupDir);
						}

					strcat_s<200>(string2, "\\");

					// trim any spaces off of the profileId,
					//  leading spaces in a filename is not good form.
					//
					CString csBaseName;
					csBaseName = page[PAGE_LIST_POS].Extension;

					csBaseName.TrimLeft();
					csBaseName.TrimRight();

					strcat_s<200>(string2, (LPTSTR) (LPCTSTR) csBaseName);
					strcat_s<200>(string2, ".TXT");

					OpenListPageTextReport(string2);
					WriteListPageTextReport("Begin: List-Page-Report.");

					// clear out default list-page information:
					//  numeric message,
					//  alpha message.
					//  voice path and file information.
					memset(ListQue.PageNumber, 0, sizeof(ListQue.PageNumber));
					memset(ListQue.PageAlpha,  0, sizeof(ListQue.PageAlpha));
					memset(ListQue.PageVoice,  0, sizeof(ListQue.PageVoice));

					// the first 3 records in this file hold
					//  default list-page information.
					//  record 000 is for default list-page numeric message
					//  record 001 is for default alphanumeric message
					//  record 002 is defined to contain the path and filename
					//   for a voice page .vox file.
					//  record 003 may contain the text "STACKED" in the data field
					//   to indicate a statcked voice page.
					//  record 003 may contain the text "STATUS" in the data field
					//   and if "STATUS" is defined, then a number field follows
					//   representing the numeric status.
					//
					// the eleventh is record "1XX" is the
					//  first member of the list page list.
					//  all records with with (listindex == "1XX") are list members.
					// the list is defined/maintained with the xtend utility "pagelist.exe"

					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
									"CheckCodePriorityListQueue(): before calling ListGetFirst()");

					j = ListGetFirst(&ListQue.listPosition, string3);

					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
									"CheckCodePriorityListQueue(): after calling ListGetFirst() j [%d]",
									j);

					if(SUCCESS == j)
						{
						// the list exists in the btrieve file (xtpglst.btr),
						//  begin processing the list page.

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
										"CheckCodePriorityListQueue(): ListQue.Active set to TRUE");

						ListQue.Active = TRUE;
						ListQue.OnCallActive = FALSE;

						// fixed values for deprecated items.
						page[PAGE_LIST_POS].PriorityQPointer = 0;	// deprecated 2005-06-14

						SYSTEMTIME stLocal;
						CString csDateTimeStation;
						CString csCustomerInfo;

						ListPagePrinting(START_PAGE, NULL);

						// report header1/title:
						ListPagePrinting(HEADER_1, "LIST PAGE REPORT");

						// report header2:
						ListPagePrinting(HEADER_2, "Spok, Inc. PageServer - List Page Report");

						// customer identification information.
						csCustomerInfo.Format("Customer Name: [%s]", param.CustomerInfoFullName);
						ListPagePrinting(HEADER_2, (LPSTR) (LPCTSTR) csCustomerInfo);

						// report header3 - date presented:
						GetLocalTime(&stLocal);
						csDateTimeStation.Format("Date: [%04d-%02d-%02d]  Time: [%02d:%02d:%02d]  PageServer StationId: [%s]",
													stLocal.wYear,
													stLocal.wMonth,
													stLocal.wDay,
													stLocal.wHour,
													stLocal.wMinute,
													stLocal.wSecond,
													param.stationId);

						ListPagePrinting(HEADER_3, (LPSTR) (LPCTSTR) csDateTimeStation);

						ListPagePrinting(BEGIN_TABLE, "");

						ListPagePrinting(BEGIN_TABLE_HEADER, "");
						ListPagePrinting(MID_PAGE, "CODE LIST:    [%10.10s] activated.", string3);
						ListPagePrinting(END_TABLE_HEADER, "");

						ListPagePrinting(BEGIN_TABLE_ROW, "");
						ListPagePrinting(MID_PAGE, "");
						ListPagePrinting(END_TABLE_ROW, "");

						memset(ListQue.InitiatorId, 0, sizeof(ListQue.InitiatorId));

						strncpy_s<10>(ListQue.InitiatorId, page[ PAGE_LIST_POS ].InitiatorId, 10);

						ListGetNext(&ListQue.listPosition, 0, ListQue.PageAlpha); // record "001"
						Set_Null(ListQue.PageAlpha, sizeof(ListQue.PageAlpha));

						ListGetNext( &ListQue.listPosition, 0, ListQue.PageVoice); // record "002"
						Set_Null(ListQue.PageVoice, sizeof(ListQue.PageVoice));

						for(i = 0; i < 7; i++)		// 003 thru 009 inclusive are ignored.
							{
							// "003" and "004" are no longer used,
							// "005" thru "009" are reserved (undefined) items,
							//  read and ignore these.
							ListGetNext(&ListQue.listPosition, 0, string2);
							Set_Null(string2, sizeof(string2));
							}

						// dump default paging info
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default numeric    [%s]", ListQue.PageNumber);
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default alpha      [%s]", ListQue.PageAlpha);
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "default voicefile  [%s]", ListQue.PageVoice);

						// dump queue source information.
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
										"queue numeric      [%s]",
										page[PAGE_LIST_POS].PagerNumber);

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
										"queue alpha        [%s]",
										page[PAGE_LIST_POS].DisplayInfo);

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
										"queue voicefile    [%s]",
										page[PAGE_LIST_POS].FileName);

						// choose the queue information if it exists.
						//  otherwise use the default items from xtpglst.btr if
						//  they are defined, if all else fails, insert a '0' for numeric
						//  and a '0' for alpha.
						{
						LPTX ListPgText;

						// fill the structure with available information.
						strncpy_s<148>(ListPgText.RawQueue,
								page[PAGE_LIST_POS].RawPageInfo,
								sizeof(ListPgText.RawQueue));

						strncpy_s<148>(ListPgText.DefaultNumeric,
								ListQue.PageNumber,
								sizeof(ListPgText.DefaultNumeric));

						strncpy_s<148>(ListPgText.DefaultAlpha,
								ListQue.PageAlpha,
								sizeof(ListPgText.DefaultAlpha));

						// decide which data to use.
						SelectListPageText(&ListPgText);

						// store the information
						strncpy_s<80>(ListQue.PageNumber,
								ListPgText.FinalNumeric,
								sizeof(ListQue.PageNumber));

						strncpy_s<148>(ListQue.PageAlpha,
								ListPgText.FinalAlpha,
								sizeof(ListQue.PageAlpha));
						}

						// clean up strings, and terminate if necessary.
						Set_Null(ListQue.PageNumber, sizeof(ListQue.PageNumber));
						Set_Null(ListQue.PageAlpha, sizeof(ListQue.PageAlpha));
						Set_Null(ListQue.PageVoice, sizeof(ListQue.PageVoice));

						if(page[PAGE_LIST_POS].VoiceFile != 0)
							{
							memset(ListQue.PageVoice, 0, sizeof(ListQue.PageVoice));
							strcpy_s<120>(ListQue.PageVoice, page[PAGE_LIST_POS].FileName);
							}

						ListQue.voiceQueued = TRUE;
						ListQue.pageQueued = TRUE;
						ListQue.retry = 0;

                        sprintf_s<200>(string5,
								"Numeric Msg:  [%s]",
								ListQue.PageNumber);

						sprintf_s<200>(string7,
								"Voice:        [%s]",
								ListQue.PageVoice);

						// dump fixed up information.
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "fixed-up numeric   [%s]", ListQue.PageNumber);
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "fixed-up alpha     [%s]", ListQue.PageAlpha);
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "fixed-up voicefile [%s]", ListQue.PageVoice);

						ListPagePrinting(BEGIN_TABLE_ROW, "");
						ListPagePrinting(MID_PAGE, string5);
						ListPagePrinting(END_TABLE_ROW, "");

						const int MAX_WIDTH = 40;
						string printMsg = page[ PAGE_LIST_POS ].longMessage;

						int msgLength = printMsg.length();
						int subIndex = 0;
						int subLength = msgLength;
						int lineNo = 1;

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "list process: content of long message: [%s]", printMsg.c_str());

						while(subIndex != msgLength)
							{
							if(subLength > MAX_WIDTH) subLength = MAX_WIDTH;
							string oneLine(printMsg.substr(subIndex, subLength));


							ListPagePrinting(BEGIN_TABLE_ROW, "");
							ListPagePrinting(MID_PAGE, "Alpha Msg %d:  [%s]", lineNo++, oneLine.c_str());
							ListPagePrinting(END_TABLE_ROW, "");

							subIndex += subLength;
							subLength = msgLength - subIndex;
							}

						ListPagePrinting(BEGIN_TABLE_ROW, "");
						ListPagePrinting(MID_PAGE, string7);
						ListPagePrinting(END_TABLE_ROW, "");

						ListPagePrinting(BEGIN_TABLE_ROW, "");
						ListPagePrinting(MID_PAGE,
							"Initialized:  [%10.10s], [%8.8s], by [%s].",
							page[PAGE_LIST_POS].InitiatedDate,
							page[PAGE_LIST_POS].InitiatedTime,
							page[PAGE_LIST_POS].InitiatorId);
						ListPagePrinting(END_TABLE_ROW, "");

						ListPagePrinting(END_TABLE, "");

						WriteListPageTextReport(string2);

						sprintf_s<200>(string2,
								"        Initialized: [%10.10s], [%8.8s], by [%s] ",
								page[PAGE_LIST_POS].InitiatedDate,
								page[PAGE_LIST_POS].InitiatedTime,
								page[PAGE_LIST_POS].InitiatorId );
						WriteListPageTextReport(string2);

						// processing differently in ADO version.
						if(XN_ADO == param.dataEngine)
							{
							// for ADO method, retrieve all remaining
							//  items from the recordset at this time,
							//  store them in an STL map container for convenience.
							//  play them back later. this was implemented
							//  so that only one recordset at a time is needed.
							//  it would get even more messy if multiple recordsets
							//  were open simultaneously.

							// start out with an empty map.
							mapcntPageList.erase(mapcntPageList.begin(), mapcntPageList.end());

							// start out with an empty vector.
							veccntPageList.erase(veccntPageList.begin(), veccntPageList.end());

							// read items until end of list is encountered.
							//  consider adding an artificial limit on how many
							//  items are allowed in a single list.
							for(;;)
								{
								if(ListGetNext(&ListQue.listPosition, 1, string2))
									{
									// store information in the STL map container.

									// store only unique items,
									//  try to find() the item that is about to be added to the map
									//  before the addition is actually made.
									if(mapcntPageList.end() == mapcntPageList.find(string2))
										{
										// if after the find() operation, the
										//  iterator points "past the end" of the list,
										//  this indicates that key is not
										//  present in the map.
										
										// prepare information in the ListData structure.
										LIST_DATA ListData;

										sprintf(ListData.ProfileId, "%s", string2);
										sprintf(ListData.Message, "%s", ListQue.PageAlpha);
										sprintf(ListData.PageType, "%s", "??");

										// enter the item into the map.
										mapcntPageList[string2] = ListData;

										PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
														"storing map item [%s]",
														string2);

										// store new information in the vector also.
										veccntPageList.push_back(string2);

										PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
														"storing vector item [%s]",
														string2);
										}
									else
										{
										// ignore any duplicates.
										PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
													"Warning: duplicate ProfileId in list ignored [%s]",
													string2);

										// ProfileId is already in the list.
										}
									}
								else
									{

									// index to the first element.
									PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "vector iterator set to beginning");
									vecitrPageList = veccntPageList.begin();
									break;

									}
								}
							PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "read all items from Xtpglst - END");
							}
						}
					else
						{
						// "2 second interval" not passed yet...
						Queue[0].Errors++;
						sprintf_s<200>(string2, "ERROR !!! Unable to process list.");
						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

						// close out the report.
						WriteListPageTextReport("End: List-Page-Report 1.");
						CloseListPageTextReport();

						// a final error message on the printed report,
						//  and close the report.
						ListPagePrinting(MID_PAGE, string2);
						ListPagePrinting(END_PAGE, NULL);
						return;
						}
					}
				else
					{
					// available item was claimed by someone else,
					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "CheckCodePriorityListQueue(): list-queue already claimed");
					}
				}
			else
				{
				// no high-priority items, considering low-priority items "occasionally".

				if(StandardPriorityCount++ >= 5)
					{
					StandardPriorityCount = 0;

					// act now.
					CheckStandardPriorityListQueue();
					}
				}
			}
		}

	// begin, or continue processing the list-page.
	if(TRUE == ListQue.Active)
		{
        if(ListQue.OnCallActive == FALSE)
			{
			// on-call is NOT active.
            memset(string2, 0, sizeof(string2));

			if((ListQue.voiceQueued == TRUE && ListQue.pageQueued == TRUE) || ListQue.retry > 10)
				{
				if(ListQue.voiceQueued == FALSE || ListQue.pageQueued == FALSE)
					{
					sprintf_s<200>(string2,
							"List Que %s Error Profile Id: %s. Page Type: %%%2.2s.",
							ListQue.ListExtension,
							page[PAGE_LIST_POS].Extension,
							page[PAGE_LIST_POS].PageType);
					WriteLog("NOPGERR", string2);
		            memset(string2, 0, sizeof(string2));
					}

				// get the next list member.
				//  from btrieve file for btrieve list method
				//  or from STL map container if ADO method
				if(XN_ADO == param.dataEngine)
					{
					// ADO database using STL map container method.
					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "2 - retrieve item from STL vector - BEGIN");

					// vector returns info in correct sequence.
					//
					// get item from the STL vector,
					//  simulate a call to ListGetNext().
					//  status goes in variable i,
					//  information goes in string2.
					if(vecitrPageList != veccntPageList.end())
						{
						//char cString[100];

						// item retrieved from the vector, get the data.
						//sprintf_s<200>(string2, "%10.10s", AAssign(cString, *vecitrPageList));
                            sprintf_s<200>(string2, "%10.10s", (*vecitrPageList).c_str());

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "retrieved [%s]", string2);

						// increase the iterator.
						vecitrPageList++;

						// item successfully read.
						i = TRUE;

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "vector -- ListItem [%s]", string2);
						}
					else
						{
						// end of vector was encountered.
						i = FALSE;

						PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "all vector items read");
						}

					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "2 - retrieve item from vector - END");
					}
				else
					{
					// retrieve item from Btrieve database list-page method.
					i = ListGetNext(&ListQue.listPosition, 1, string2);
					}

				Set_Null(string2, sizeof(string2));

				ListQue.voiceQueued = FALSE;
				ListQue.pageQueued = FALSE;
				ListQue.retry = 0;
				}
			else
				{
				i = TRUE;
				ListQue.retry++;
				memset(string2, 0, sizeof(string2));
				strcpy_s<200>(string2, ListQue.Extension);
				}

			// variable i contains the result of ListGetNext()
			//  TRUE indicates that a page-list item was retrieved,
			//
            if(i == TRUE)
				{
				// deal with each individual list-item.
				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Processing list-item [%s]", string2);

				// process one list-page item.
                memset(ListQue.Extension, 0, sizeof(ListQue.Extension));
                strcpy_s<11>(ListQue.Extension, string2);

				if(XN_ADO == param.dataEngine)
					{
					// ado processing,
					}
				else
					{
					// Btrieve database processing.

					// clear out page[].PagerId
					memset(page[PAGE_LIST_POS].PagerId,
							0,
							sizeof(page[PAGE_LIST_POS].PagerId));
					j = 0;

					for(i = 0; i < 10; i++)
						{
						if(lstBtrvBuf.lstInfo.memberId[i] != ' ')
							{
							// move data from .memberExtension which contains
							//  the ProfileId to .PagerId field
							page[PAGE_LIST_POS].PagerId[j] =
								lstBtrvBuf.lstInfo.memberExtension[i];
							j++;
							}
						}
					}

                strncpy_s<11>(page[PAGE_LIST_POS].Extension, ListQue.Extension, 10);
                sprintf_s<200>(string2, "List Member:  [%s]", ListQue.Extension);

				ListPagePrinting(BEGIN_TABLE, "");

				ListPagePrinting(BEGIN_TABLE_ROW, "");
				ListPagePrinting(MID_PAGE, "");
				ListPagePrinting(END_TABLE_ROW, "");

				ListPagePrinting(BEGIN_TABLE_HEADER, "");
				ListPagePrinting(MID_PAGE, string2);
				ListPagePrinting(END_TABLE_HEADER, "");

				strncpy_s<11>(page[PAGE_LIST_POS].InitialExtension, ListQue.Extension, 10);

				sprintf_s<200>(string2, "   %10.10s  ", ListQue.Extension);
				WriteListPageTextReport(string2);

                // check for the .log file present
                i = TRUE;
                j = 0;

                if(param.CoverPage)
					{
                    memset(page[PAGE_LIST_POS].FileName, 0, sizeof(page[PAGE_LIST_POS].FileName));
                    memset(page[PAGE_LIST_POS].PagerId, 0, sizeof(page[PAGE_LIST_POS].PagerId));

					// call to GetPagerId() presenting a ProfileId.
					GetPagerId(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS, BY_PROFILEID);

                    LogPager(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS);
                    strncpy_s<11>(page[PAGE_LIST_POS].InitialPagerId,
							page[PAGE_LIST_POS].PagerId,
							10 );

					bool mustIgnoreCoverageWhenPageAllDevices = ignoreCoverageWhenPageAllDevices && (CheckIfAllPageFlagIsOn(PAGE_LIST_POS) == SUCCESS);

                    if(strncmp(page[PAGE_LIST_POS].FileName, "ON CALL", 7) == 0 ||
						strncmp(page[PAGE_LIST_POS].FileName, "ONCALL", 6) == 0)
						{
						// if you got here, the call to GetPagerId()
						//  filled in the .FileName field with the text "ON CALL"
						//  because the contents of the "CoveringExtension" field
						//  of "Pagers" table contained "ON CALL".

                        sprintf_s<200>(string2,
								"OnCall Name:  [%-20.20s]",
								page[PAGE_LIST_POS].tempStr);

                        PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

						ListPagePrinting(BEGIN_TABLE, "");

						ListPagePrinting(BEGIN_TABLE_ROW, "");
						ListPagePrinting(MID_PAGE, "");
						ListPagePrinting(MID_PAGE, "Begin: OnCall Schedule");
						ListPagePrinting(MID_PAGE, "");
						ListPagePrinting(END_TABLE_ROW, "");


						ListPagePrinting(BEGIN_TABLE_ROW, "");
						ListPagePrinting(MID_PAGE, string2);
						ListPagePrinting(MID_PAGE, "");
						ListPagePrinting(END_TABLE_ROW, "");

						ListPagePrinting(END_TABLE, "");

						WriteListPageTextReport(string2);

						// beginning of on-call processing.
                        ListQue.OnCallActive = TRUE;
                        ListQue.OnCallPosition = -1;

                        i = FALSE;
						}
                    else if(page[PAGE_LIST_POS].FileName[0] != 0 && !mustIgnoreCoverageWhenPageAllDevices)
						{
						// .FileName is non-zero, this indicates
						//  that there is a covering pager.
 						j = 1;
                        memset(page[PAGE_LIST_POS].PagerNumber,
								0,
								sizeof(page[PAGE_LIST_POS].PagerNumber));

                        strncpy_s<65>(page[PAGE_LIST_POS].PagerNumber,
								page[PAGE_LIST_POS].FileName,
								sizeof(page[0].PagerNumber));

                        i = CoverDoubleCheck(PAGE_LIST_POS);

                        if(i == TRUE)
							{
                            strcpy_s<11>(page[PAGE_LIST_POS].Extension,
									page[PAGE_COV_POS].Extension);

                            // SO - 5/10/2013
                            // The following statement fixes US15859
                            // The pageServer does not populate messages into DB 
                            // and flat queue file for those list members that are covered by
                            strcpy_s<11>(page[PAGE_LIST_POS].PagerId,
                                page[PAGE_COV_POS].PagerId);

                            sprintf_s<200>(string2,
									"%20.20s  covered by extension %s %s",
									page[PAGE_LIST_POS].tempStr,
									page[PAGE_LIST_POS].Extension,
									page[PAGE_LIST_POS].PagerId);

							ListPagePrinting(MID_PAGE, string2);

 							WriteListPageTextReport(string2);

                            sprintf_s<200>(string2,
										" CV %10.10s  ",
										page[PAGE_LIST_POS].Extension );
 							WriteListPageTextReport(string2);
							PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "string2 --xx--xx [%s]", page[PAGE_COV_POS].PagerId);
							}
                        else
							{
                            sprintf_s<200>(string2,
									"recursively covered  Not Paged   %s",
									page[PAGE_LIST_POS].PagerId);

							ListPagePrinting(MID_PAGE, string2);

							WriteListPageTextReport(string2);
  							}
						}
					}

                if(i)
					{
                    ProcessListQueue();
					}
                else
					{
					if(ListQue.OnCallActive != TRUE)
						{
						ListQue.voiceQueued = TRUE;
						ListQue.pageQueued = TRUE;
                        sprintf_s<200>(string2,
								"Extension [%s] not recognized check log file",
								page[PAGE_LIST_POS].Extension);
                        PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);
                        Queue[0].Errors++;

						ListPagePrinting(MID_PAGE, "");
						ListPagePrinting(MID_PAGE, string2);
						}
					}
				}
            else
				{
				// if you got here, list-page was active
				//  but no more items were returned by ListGetNext.
				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
								"CheckCodePriorityListQueue(): no more items in page-list");

                sprintf_s<200>(string2, "List processing completed.");
                PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

                if(TRUE == ListQue.Active)
					{
					// report footer:
                    ListPagePrinting(END_PAGE, NULL);
					}

				// list processing is complete.
				PrintMessage(MSGTYPE_SYSTEM,
								hwndGeneralMsg,
								"CheckCodePriorityListQueue(): ListQue.Active set to FALSE");

                ListQue.Active = FALSE;

 				// close out the report.
				WriteListPageTextReport("End: List-Page-Report 2.");
				CloseListPageTextReport();
 				}
			}
        else
			{
			// on-call IS active.

			// Process On Call Extension
            PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Processing on-call extension.");

            if(ListQue.OnCallPosition == -1)
				{
				// first on-call, if any.

                ListQue.OnCallActive =
					GetFirstOnCall(&ListQue.OnCallPosition, ListQue.Extension, ListQue.OnCallKey);

				ListQue.voiceQueued = FALSE;
				ListQue.pageQueued = FALSE;
				ListQue.retry = 0;
				}
            else
				{
				// subsequent on-call

				if((ListQue.voiceQueued == TRUE && ListQue.pageQueued == TRUE) ||
					ListQue.retry > 10)
					{
					// too many retries, write an error message.
					if(ListQue.voiceQueued == FALSE || ListQue.pageQueued == FALSE)
						{
						memset(string2, 0, sizeof(string2));
						sprintf_s<200>(string2,
								"List Que %s Error OnCall Profile Id: %s. Page Type: %%%2.2s.",
								ListQue.ListExtension,
								page[PAGE_LIST_POS].Extension,
								page[PAGE_LIST_POS].PageType);
						WriteLog("NOPGERR", string2);
						}

					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "CheckCodePriorityListQueue(): calling GetNextOnCall");

	                ListQue.OnCallActive =
						GetNextOnCall(&ListQue.OnCallPosition, ListQue.OnCallKey);

					ListQue.voiceQueued = FALSE;
					ListQue.pageQueued = FALSE;
					ListQue.retry = 0;
					}
				else
					{
					memset(OnCallBtrvBuf.OnCallInfo.Extension,
							0,
							sizeof(OnCallBtrvBuf.OnCallInfo.Extension));					

                    //SO - 03/20/2013 - Removed buffer check statement because it crashes the application 
                    //sprintf_s<10>(OnCallBtrvBuf.OnCallInfo.Extension,
					//		"%s",
					//		page[PAGE_LIST_POS].Extension);

                    sprintf(OnCallBtrvBuf.OnCallInfo.Extension,
							"%s",
							page[PAGE_LIST_POS].Extension);

					ListQue.retry++;
					}
				}

            if(ListQue.OnCallActive &&
				OnCallBtrvBuf.OnCallInfo.Extension[0] != 0 &&
				ListQue.retry < 10)
				{
                sprintf_s<200>(string2,
						"OnCall Profile Id: [%-10.10s]",
						OnCallBtrvBuf.OnCallInfo.Extension);

                PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

                sprintf_s<200>(string2,
						" OnCall %10.10s  ",
						OnCallBtrvBuf.OnCallInfo.Extension);
				WriteListPageTextReport(string2);

                j=0;
                memset(page[PAGE_LIST_POS].Extension,
						0,
						sizeof(page[PAGE_LIST_POS].Extension));

                if(OnCallBtrvBuf.OnCallInfo.depPager == 'N')
					{
					// not the "department pager", an individual pager. (.depPager == 'N')

					// purify the Extension.
                    for(i = 0; i < 10; i++)
						{
                        if(OnCallBtrvBuf.OnCallInfo.Extension[i] != ' ')
							{
                            page[PAGE_LIST_POS].Extension[j] =
								OnCallBtrvBuf.OnCallInfo.Extension[i];
                            j++;
							}
						}
					}
                else
					{
					// this is the "department pager" (.depPager == 'Y')

					// purify the OCExtension.
                    for(i = 0; i < 10; i++)
						{
                        if(OnCallBtrvBuf.OnCallInfo.OCExtension[i] != ' ')
							{
                            page[PAGE_LIST_POS].Extension[j] =
								OnCallBtrvBuf.OnCallInfo.OCExtension[i];
                            j++;
							}
						}
					}

                i = TRUE;
                page[PAGE_LIST_POS].PageStatus[0] = 'R';
                memset(page[PAGE_LIST_POS].FileName, 0, sizeof(page[PAGE_LIST_POS].FileName));
                memset(page[PAGE_LIST_POS].PagerId, 0, sizeof(page[PAGE_LIST_POS].PagerId));

				GetPagerId(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS, BY_PROFILEID);

                LogPager(page[PAGE_LIST_POS].Extension, PAGE_LIST_POS);

                if(param.CoverPage && OnCallBtrvBuf.OnCallInfo.depPager == 'N')
					{
					// not the department pager, is an individual pager.

                    if(page[PAGE_LIST_POS].FileName[0] != 0)
						{
                        memset(page[PAGE_LIST_POS].PagerNumber,
								0,
								sizeof(page[PAGE_LIST_POS].PagerNumber));

                        strncpy_s<65>(page[PAGE_LIST_POS].PagerNumber, page[PAGE_LIST_POS].FileName, sizeof(page[0].PagerNumber));

                        i = CoverDoubleCheck(PAGE_LIST_POS);
						ListMember_Covered = i != 0;

                        if(i == TRUE)
							{
                            strcpy_s<11>(page[PAGE_LIST_POS].Extension, page[PAGE_COV_POS].Extension);
                            sprintf_s<200>(string2,
									"covered by extension [%s]  [%s]",
									page[PAGE_LIST_POS].Extension,
									page[PAGE_LIST_POS].PagerId);

                            ListPagePrinting(MID_PAGE, string2);

							WriteListPageTextReport(string2);

                            sprintf_s<200>(string2, " CV %10.10s", page[PAGE_LIST_POS].Extension);
							WriteListPageTextReport(string2);
							}
                        else
							{
                            sprintf_s<200>(string2,
									"         recursively covered  Not Paged   %s",
									page[ PAGE_LIST_POS ].PagerId );

                            ListPagePrinting(MID_PAGE, string2);

							WriteListPageTextReport(string2);
 							}
						}
					}
				else
					{
					// this is the department pager.
					}

                if(i)
					{
					ProcessListQueue();
					}
				else
					{
					ListQue.voiceQueued = TRUE;
					ListQue.pageQueued = TRUE;
					}
				}
            else if(ListQue.OnCallActive != TRUE)
				{
				ListQue.voiceQueued = TRUE;
				ListQue.pageQueued = TRUE;

 				ListPagePrinting(MID_PAGE, "End: OnCall Schedule");
				ListPagePrinting(MID_PAGE, string2);
				}
            else
				{
				ListQue.voiceQueued = TRUE;
				ListQue.pageQueued = TRUE;

                sprintf_s<200>(string2, "OC                         No One OnCall");
                PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);

				WriteListPageTextReport(" ");
				WriteListPageTextReport(string2);
				}
			}
		}
	}

