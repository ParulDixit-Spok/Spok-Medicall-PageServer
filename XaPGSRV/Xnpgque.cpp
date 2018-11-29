
// xnpgque.cpp
#include "common.h"

#include <fcntl.h>
#include <sys\stat.h>
#include <share.h>
#include <io.h>
#include <sys\locking.h>
#include <memory>
#include <string>


// disable deprecation
#pragma warning(disable:4996)

using namespace std;


	// return value meaning:
	//
	//  0  - there is nothing to do, no records
	//       are available to be claimed.
	//
	//  1 - at least one record in the queue file
	//      is available to be claimed.
	//
	int
CheckQueue(int QPointer)
	{
	int iReturnValue = 0;

	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// don't bother, we are "externally controlled", and currently "standing by"...
		return iReturnValue;
		}

	// perform the queue item availability query.
	//
	if(Queue[QPointer].pQFM->IsItemAvailable())
		{
		// indicate that item(s) are available.
		//
		iReturnValue = 1;
		}
	else
		{
		// no items are available...
		//  consider recycling the queue file at this time.
		//  evaluate whether the number of items in the queue
		//  file exceeds the recycle limit parameter.
		// and if this PageServer station has been assigned to rename this queue.
		//
		if((Queue[QPointer].pQFM->GetLastWritten() >= param.MaxQueSz) && 
			(strcmp(param.stationId, Queue[QPointer].StationId) == 0))
			{
			// attempt recycle, intentionally ignore the return code
			//  from this call, becaule if unsuccessful, maybe the next time
			//  will be successful, CheckQueue() is called frequently,
			//  and continuously by the application.
			//
			Queue[QPointer].pQFM->RecycleRequest();
			}
		else
			{
			// either there are unprocessed items in queue,
			//  or the number of items in the queue is zero,
			//  and no recycle is necessary.
			}
		}
	
	return iReturnValue;
	}


	void
RemoveAlpha(int FilterOption, char * SourceTextBuffer, int SourceLength)
	{
	int	i, j;
	char TargetString[256];
	int MaxTargetIndex;

	// remove alpha characters on the given text buffer.
	//  the buffer is expected to be null terminated,
	//  but we won't require this,
	// we will process the buffer characters up to
	//  the stated source length size MINUS 1 char.
	//  we don't touch the final char of the buffer
	//  it should be a nul but we don't go near it,
	//  We will return a null terminated string
	//  in that buffer, after our RemoveAlpha() proecssing.

	// don't allow zero length strings to be processed
	if(SourceLength <= 0)
		{
		return;
		}

	// process strings up to the given Length less 1 character,
	//  or to the max length of the temporary string
	//  whichever is shorter, don't forget that the
	//  target string must be nul terminated.

	if(SourceLength >= sizeof(TargetString))
		{
		MaxTargetIndex = (sizeof(TargetString) - 1);
		}
	else
		{
		MaxTargetIndex = (SourceLength - 1);
		}

	// fill the target string with nul chars, so that
	//  when the last char is transferred, the
	//  string will be properly terminated.
	memset(TargetString, 0, sizeof(TargetString));

	// transfer and filter chars from given string
	//  to the target string.
	j = 0;

	for(i = 0; i <= MaxTargetIndex; i++)
		{
		// examine each source character.

		// always allow numeric chars '0' thru '9' to pass
		if((SourceTextBuffer[i] >= '0') && (SourceTextBuffer[i] <= '9'))
			{
			// accept the char into the destination string.
			TargetString[j] = SourceTextBuffer[i];
			j++;
			continue;
			}

		// not in range '0' thru '9',
		//  some other chars may be allowed to pass.

		// allow special "numeric" characters through,
		//  dependent on destination port type.
		switch(FilterOption)
			{
			case NUMERIC_FILTER_COMM_PORT:
				// Numeric Com Port Pages
				//   typically these characters are allowed for
				//   formatting control characters by most RCC's.
				if(	SourceTextBuffer[ i ] == '-' ||
					SourceTextBuffer[ i ] == '*' ||		// added for compatibility with zetron 2200 numeric.
					SourceTextBuffer[ i ] == ' ' )		//  2006-01-11 (for St. Raphael, New Haven, CT)
					{
					// accept the char into the target string.
					TargetString[j] = SourceTextBuffer[i];
					j++;
					}
				break;

			case NUMERIC_FILTER_DIALOGIC_PORT:
				// Numeric Dialogic Port Pages
				//  these characters are to be expected in a Dialogic
				//  command string passed to the the dial() function.
				//  '*' and '#' represent valid TouchTones.
				//  the phone line, T for TouchTone Dialing,
				//  ',' represents a delay,
				//  '&' represents a hookswitch flash.
				if( SourceTextBuffer[ i ] == 'T' ||
					SourceTextBuffer[ i ] == '*' ||
					SourceTextBuffer[ i ] == '#' ||
					SourceTextBuffer[ i ] == ',' ||
					SourceTextBuffer[ i ] == '&')
					{
					// accept the char into the target string.
					TargetString[j] = SourceTextBuffer[i];
					j++;
					}
				break;

			case NUMERIC_FILTER_ABSOLUTE:
				// no "special" chars allowed through
				break;

			default:
				break;
			}
		}

	// after all chars are filtered, put back to the source text buffer.
	memset(SourceTextBuffer, 0, SourceLength);
	strcpy(SourceTextBuffer, TargetString);
	}


	void
XnLimitDisplay(int maxLen, char *textString, int stringLen)
	{
	int	i;
	char tempStr[500];




	i = strlen(textString);

	if(i > maxLen && maxLen > 0)
		{
		memset(tempStr, 0, sizeof(tempStr));

		strcpy(tempStr, textString + i - maxLen);

		memset(textString, 0, stringLen);
		strcpy(textString, tempStr);
		}
	}



	char * 
CopyString(char* dest, int destSize, const char* src, int srcSize)
	{
	char* start=dest;

	for(; ((destSize > 0) && (srcSize > 0)) && *src; ++dest, ++src, --destSize, --srcSize) *dest = *src;

	//make sure the string is null terminated, when the copy overruns the buffer.
	if(destSize == 0) --dest;

	*dest = 0;
	return start;
	}

	
	struct 
QSearch
	{
	int countElements;
	int currentElement;
	char* last;
	char buffer[TAP_MESSAGE_BUFFER_SIZE];
	};


	char * 
GetFirstElement(char* str, int strSize, QSearch& dat)
	{

	if(str == NULL)
	{
 		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"Invalid argument GetFirstElement");
		return NULL;
	}

	strncpy_s<8000>(dat.buffer, str, strSize);

	// count number of elements
	dat.countElements = 0;
	dat.currentElement = 0;

	char * start = dat.buffer;
	char * ptr;

	dat.last = start;

	for(ptr = dat.buffer; *ptr; ++ptr) if(*ptr == '+') ++dat.countElements;
	
	if(dat.countElements > 0)
		{
		for(ptr = dat.buffer; *ptr; ++ptr) 
			{
			if(*ptr == '+')
				{
				*ptr = 0;
				dat.last = ++ptr;
				break;
				}
			}
		}
	++dat.currentElement;

	return start;
	}


	char * 
GetNextElement(QSearch& dat)
	{
	if(dat.currentElement > dat.countElements) return NULL;

	char* ptr;
	char* start = dat.last;

	for(ptr = dat.last; *ptr; ++ptr) if(*ptr == '+') break;

	if(*ptr == '+')
		{
		*ptr = 0;
		++ptr;
		}
	dat.last = ptr;
	dat.currentElement++;

	return start;
	}


	void GetQueueAux(char* pageInfo, int PagePointer, int QPointer, int& items, bool Want3)
	{
		char * dNumber;
		char * aCode;
		char * message;
		QSearch qSearch;

		items = 0;	

		// looking for tokens separated by the '+' char
		//  don't assume string has any '+' delimiters.

		dNumber = GetFirstElement(pageInfo, 255, qSearch);
		if(dNumber == NULL) return;

		if(*dNumber == 0)
		{
			dNumber = GetNextElement(qSearch);
			if(dNumber == NULL) return;

            //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
            //strncpy_s<65>(page[PagePointer].PagerNumber, dNumber, sizeof(page[0].PagerNumber));
			strncpy(page[PagePointer].PagerNumber, dNumber, sizeof(page[0].PagerNumber));
			if(*dNumber != 0) items++;
		}
		else
		{
            //SO - 03/21/2013 - Removed buffer check statement because it crashes the application 
            //strncpy_s<65>(page[PagePointer].PagerNumber, dNumber, sizeof(page[0].PagerNumber));
			strncpy(page[PagePointer].PagerNumber, dNumber, sizeof(page[0].PagerNumber));
			if(*dNumber != 0) items++;

			int itemsFound = 0;

			if(Want3)
			{
				itemsFound = qSearch.countElements + 1;
				if(itemsFound != 3)
				{
					//log an error
					PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Expecting 3 segment IXO MODEM format message, and found only 2 segments");

				}
			}
			else
			{
				itemsFound = 2;
			}


			switch(itemsFound)
			{
			case 2:
				message = GetNextElement(qSearch);

				if(message != NULL && *message != 0) items++;
				break;

			case 3:
				aCode = GetNextElement(qSearch);
				if(aCode == NULL) break;

				strncpy_s<15>(page[PagePointer].AutCode, aCode, sizeof(page[0].AutCode));
				if(*aCode != 0) items++;
				message = GetNextElement(qSearch);

				if(message != NULL && *message != 0) items++;
				break;
			}
		}

		//long message already read in from queue
		CopyString(page[PagePointer].DisplayInfo, 255, Queue[QPointer].pQBC->GetWholeMessage().c_str(), 250);

		//copy long message to page struct
		page[PagePointer].longMessage = Queue[QPointer].pQBC->GetWholeMessage();
	}


	void 
CopyQueueToPageInfo(char* pageInfo, int PagePointer, int QPointer, bool Want3)
	{
	
	_snprintf(page[PagePointer].PageType, 2, "%2.2s", Queue[QPointer].pQBC->GetPageType());
	_snprintf(page[PagePointer].PageStatus, 2, "%2.2s", Queue[QPointer].pQBC->GetPageStatus());
	_snprintf(page[PagePointer].InitiatedDate, 10, "%10.10s", Queue[QPointer].pQBC->GetInitiatedDate());
	_snprintf(page[PagePointer].InitiatedTime, 8, "%8.8s", Queue[QPointer].pQBC->GetInitiatedTime());
	_snprintf(page[PagePointer].Extension, 8, "%8.8s", Queue[QPointer].pQBC->GetProfileId());
	_snprintf(page[PagePointer].PagerId, 8, "%8.8s", Queue[QPointer].pQBC->GetPagerId());
	_snprintf(page[PagePointer].InitiatorId, 8, "%8.8s", Queue[QPointer].pQBC->GetInitiatorProfleId());
	
	page[PagePointer].ACKMin = Queue[QPointer].pQBC->GetACKMin();
	page[PagePointer].NextAckPtr = Queue[QPointer].pQBC->GetNextAckPtr();
	_snprintf(page[PagePointer].Printed, 1, "%s", Queue[QPointer].pQBC->GetPrinted());
	page[PagePointer].VoiceFile = Queue[QPointer].pQBC->GetVoiceFile();
	
	// is this Set_Null() really necessary?
	Set_Null(page[PagePointer].RawPageInfo, sizeof(page[PagePointer].RawPageInfo));
	
	_snprintf(page[PagePointer].PageInfo, 148, "%-148.148s", Queue[QPointer].pQBC->GetPageInfo());
	
	// a second copy of the info stored for list paging usage.
	_snprintf(page[PagePointer].RawPageInfo, 148, "%-148.148s", Queue[QPointer].pQBC->GetPageInfo());
	
	// manipulate statistics information that will ultimately
	//  be recorded in .STA files.
	Queue[QPointer].Attempts++;
	
	if(strncmp(page[PagePointer].InitiatorId, "External", 8) == 0)
		{
		Queue[QPointer].External++;
		}
	
	if(page[PagePointer].VoiceFile > 0)
		{
		char TempString1[300];
		memset(TempString1, 0, sizeof(TempString1));
		
		sprintf(TempString1, "%04i.VPO", page[PagePointer].VoiceFile);
		
		if(strncmp(page[PagePointer].Extension, "CATS PG", 7) == 0)
			{
			sprintf(TempString1, "%04i.CAT", page[PagePointer].VoiceFile);
			
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				"GetQueue(): recognized text CATS PG in ProfileId");
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				"GetQueue(): NO - CATS PG found in ProfileId");
			}
		
		if(strncmp(page[PagePointer].InitiatorId, "CATS PG", 7) == 0)
			{
			sprintf(TempString1, "%04i.CAT", page[PagePointer].VoiceFile);
			
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				"GetQueue(): recognized text CATS PG in InitiatorId");
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				"GetQueue(): NO - CATS PG found in InitiatorId");
			}
		
		char TempString0[300];
		memset(TempString0, 0, sizeof(TempString0));
		
		TempString0[0] = page[PagePointer].InitiatedDate[0];
		TempString0[1] = page[PagePointer].InitiatedDate[1];
		TempString0[2] = page[PagePointer].InitiatedDate[3];
		TempString0[3] = page[PagePointer].InitiatedDate[4];
		
		sprintf(page[PagePointer].FileName,
			"%s%s%s",
			pageFileDir,
			TempString0,
			TempString1);
		
		PrintMessage(MSGTYPE_SYSTEM, 
			hwndGeneralMsg,
			"GetQueue(): Voice File FileName [%s]",
			page[PagePointer].FileName);
		}
	}


	// read a page request record from the disk file
	//  as indexed by QPointer.
	//  the record was previously claimed  by a call to
	//  ClaimQueueItem().
	//

	// parameter 1 - PagePointer, 
	//  is the index into the block of page[] structures (0 thru 135).
	// parameter 2 - PTPointer, 
	//  is a positive number indexing into the configuration parameters
	//  to identify the RCC vendor types, specifically to identify
	//  the IVR script to use (param.PageType[PTPointer].Script), 
	//  if the current request is indeed an RCC type.
	//  parameter 2 will be meaningless in the case of a "List Page",
	//  or a "Multi Queue" type. In these cases, the parameter will be
	//  passed as -1, don't use it as an index in this case.
	//  just skip the processing when -1 is passed.
	//
	void
GetQueue(int PagePointer, int PTPointer, int QPointer, bool Want3, char* file, int line)
	{
    int items = 0;
 	char pageInfo[255];

	// data is currently in member variables of the QueueBufferClass.
	//  we need this data to be available in the page[n].xxxx data block.
	
	// first clear out the destination structure.
	//

	// will clobber the stl string, with memset
	//int xxxx = sizeof(string);

	unsigned char strTemp[sizeof(string)];
	memcpy(strTemp, &page[PagePointer].longMessage, sizeof(string));
	memset(&page[PagePointer], 0, sizeof(page[PagePointer]));
	memcpy(&page[PagePointer].longMessage, strTemp, sizeof(string));

	// transfer information from QueueBufferClass 
	//  into page[n].xxx structure members.
	
	// copy 148 chars of paging information.
	strncpy(pageInfo, static_cast<LPCTSTR>(Queue[QPointer].pQBC->GetPageInfo()), sizeof(page[PagePointer].PageInfo));

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "GetQueue Called from File: %s, Line: %d", file, line);

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "ProfileId:     [%s]", Queue[QPointer].pQBC->GetProfileId());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "PagerId:       [%s]", Queue[QPointer].pQBC->GetPagerId());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "InitiatedDate: [%s]", Queue[QPointer].pQBC->GetInitiatedDate());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "InitiatedTime: [%s]", Queue[QPointer].pQBC->GetInitiatedTime());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Pageinfo:      [%s]", Queue[QPointer].pQBC->GetPageInfo());

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "LongString:    [%s]", Queue[QPointer].pQBC->GetWholeMessage().c_str());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "VoiceFile:     [%d]", Queue[QPointer].pQBC->GetVoiceFile());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "InitiatorId:   [%s]", Queue[QPointer].pQBC->GetInitiatorProfleId());

    if(strncmp(pageInfo, "NOPAGE", 6) == 0)
		{
		// an undocumented feature (of the list page editor) 
		//  that is used to set the status of all list members, 
		//  without sending a page out to anyone.
		//
		memset(pageInfo, 0, sizeof(pageInfo));
		}
    else
		{
		CopyQueueToPageInfo(pageInfo, PagePointer, QPointer, Want3);
		GetQueueAux(pageInfo, PagePointer, QPointer, items, Want3);		
		}

	// end of processing of up to 3 tokens, delimited by 2 '+' characters.

	// looking for ".VCE", within .DisplayInfo. if this can
	//  be found it will indicate that a filename for voice paging is present.
 
    if(strstr(page[PagePointer].DisplayInfo, ".VCE") != NULL)
		{
		// ".VCE" found within the string.
		//  copy from .DisplayInfo into .FileName.
        memset(page[PagePointer].FileName, 0, sizeof(page[PagePointer].FileName));
        sprintf(page[PagePointer].FileName, "%-119.119s", page[PagePointer].DisplayInfo);
		}

	// if PagePointer is in the comm port range,
	//  build a "%XX" string so that GetPageType() can be called
	//  to determine if the .DisplayInfo should contain only
	//  numeric information.
	// if numeric only type is required,
	//  use RemoveAlpha() to translate the string into acceptable form.
	if(PagePointer >= PAGE_COM_START && PagePointer < PAGE_LIST_POS)
		{
		char TempString[4];

		sprintf(TempString, "%%%s", page[PagePointer].PageType);

		if(GetPageType(PagePointer, TempString,  __FILE__, __LINE__) == TRUE)
			{
			if(page[PagePointer].ComNumber >= 0)
				{
				if(param.ComPort[page[PagePointer].ComNumber].QueType[page[PagePointer].ComOffset][1] == '0')
					{
					// comm port type numeric pages cannot contain alpha chars.
					//  a different set of "numerics" are allowed on comm ports
					//  vs dialogic ports.
					RemoveAlpha(NUMERIC_FILTER_COMM_PORT,
									page[PagePointer].DisplayInfo,
									sizeof(page[PagePointer].DisplayInfo));
					}
				}
			}
		}

    page[PagePointer].attempts = 0;
    page[PagePointer].PageStatus[0] = 'R';

    Set_Null(page[PagePointer].PagerNumber, sizeof(page[PagePointer].PagerNumber));
    Set_Null(page[PagePointer].AutCode, sizeof(page[PagePointer].AutCode));
    Set_Null(page[PagePointer].DisplayInfo, sizeof(page[PagePointer].DisplayInfo));
    Set_Null(page[PagePointer].FileName, sizeof(page[PagePointer].FileName));
    Set_Null(page[PagePointer].Extension, sizeof(page[PagePointer].Extension));
    Set_Null(page[PagePointer].PagerId, sizeof(page[PagePointer].PagerId));
    Set_Null(page[PagePointer].InitiatorId, sizeof(page[PagePointer].InitiatorId));
    Set_Null(page[PagePointer].PageStatus, sizeof(page[PagePointer].PageStatus));
    Set_Null(page[PagePointer].PageType, sizeof(page[PagePointer].PageType));
    Set_Null(page[PagePointer].PageInfo, sizeof(page[PagePointer].PageInfo));

	// dialogic port type pages are always numeric,
	//  limit message to a set of allowable "numeric" characters.
	//  note: if a list-page is being processed, PTPointer == -1
	//
    if((PagePointer < PAGE_COM_START) && (PTPointer >= 0))
		{
		// dialogic type pages.
		RemoveAlpha(NUMERIC_FILTER_DIALOGIC_PORT,
					page[PagePointer].PagerNumber,
					sizeof(page[PagePointer].PagerNumber));
		RemoveAlpha(NUMERIC_FILTER_DIALOGIC_PORT,
					page[PagePointer].AutCode,
					sizeof(page[PagePointer].AutCode));
		RemoveAlpha(NUMERIC_FILTER_DIALOGIC_PORT,
					page[PagePointer].DisplayInfo,
					sizeof(page[PagePointer].DisplayInfo));
		}

	// decide to "limit" the length of information sent out
	//  enabled / controlled by configuration parameters.
	if(page[PagePointer].PageType[0] != 0)
		{
		if(strstr(param.pageLimitTypes, page[PagePointer].PageType) != NULL)
			{
			XnLimitDisplay(param.pageLimitMax,
							page[PagePointer].DisplayInfo,
							sizeof(page[PagePointer].DisplayInfo));
			}
		if(strstr(param.pageLimitTypes2, page[PagePointer].PageType) != NULL)
			{
			XnLimitDisplay(param.pageLimitMax2,
							page[PagePointer].DisplayInfo,
							sizeof(page[PagePointer].DisplayInfo));
			}
		}

	// check for valid que defined
	//
    if((PagePointer < PAGE_COM_START) && (param.TonePage == FALSE) && (PTPointer >= 0))
		{
		// scan the rcc touchtone parameter script
		//
		int itemsneeded = 0;

		for(int i = 0; i < (int) strlen(param.PageType[PTPointer].Script); i++)
			{
			// count how many items are in the dial script
			//
            switch(param.PageType[PTPointer].Script[i])
				{
                case 'C':
                case 'c':
                case 'D':
                case 'd':
                case 'A':
                case 'a':
                    itemsneeded++;
                    break;

                default:
                    break;
				}
			}

        if(items < itemsneeded)
			{
			// status == 'K' means kill the page, due to
			//  not enough information.
			page[PagePointer].PageStatus[0] = 'K';
			}

		char string2[300];
		memset(string2, 0, sizeof(string2));

        sprintf(string2, "Items Needed: [%i], Items Retrieved: [%i].", itemsneeded, items);
        PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);
		}

	// all done, store the collected information in the local
	//  flat-file database.
    if(page[PagePointer].PageStatus[0] != 'K')
		{
 		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"GetQueue() - Page Info: Number (capcode)  [%s]",
						page[PagePointer].PagerNumber);

		PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg, 
						"                        AuthCode (pin)    [%s]",
						page[PagePointer].AutCode);


		PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg,
						"                        Display (message) [%s]",
						//longfix
						//page[PagePointer].DisplayInfo); 
						page[PagePointer].longMessage.c_str());

		PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg,
						"                        Want3 (parameter) [%s]",
						Want3 ? "true" : "false"); 

		PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg,
						"                        items             [%d]",
						items);		
		}

	// for debug... show the contents of the complete message that was re-assembled from the queue.
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "x-GetQueue(): length:     [%d]", Queue[QPointer].pQBC->GetWholeMessage().length());
	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "x-GetQueue(): content:    [%s]", Queue[QPointer].pQBC->GetWholeMessage().c_str());
	}


	// claim the queue record by updating the "LastRead" pointer.
	//  return TRUE if LastRead has been increased.
	//  return FALSE if unable to lock, or unable to claim.
	bool
ClaimQueueItem(int QPointer, int MessagePartsRequired)
	{
	// message parts required is typically 2 but may be 3 in the case of IXO_MODEM.

	// Alpha Messages may contain the <+> char, we would be fooled, if we were 
	//  just counting <+> characters.
	
	// for list queues, the information is a Numeric Message + Alpha Message.
	// for TAP_IN, TAP_OUT, there is a PagerId + Message.
	// for IXO_MODEM there is a Dial Number + PagerId + Message.

	// generalization, Paging queue information fields are separated by
	//  <+> characters, IXO_MODEM type of queues are the only type that have 3 parts, 
	//  all others have 2 parts. This 2part / 3part distinction is critical when
	//  parsing the flat-file queue content.

	bool bReturnStatus = false;

 	// use the QueueFileManager object to do the work.
	//
	if(Queue[QPointer].pQFM->ClaimQueueItem(Queue[QPointer].pQBC, MessagePartsRequired))
		{
		// successful claim.
		bReturnStatus = true;

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ClaimQueueItem(): - successfully claimed an item");

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ClaimQueueItem(): - number of unclaimed items is: [%d]", 
						Queue[QPointer].pQFM->GetUnclaimedCount());
		}
	else
		{
		// unable to claim.
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ClaimQueueItem(): - unable to claim an item");
		}

	return bReturnStatus;
	}


	bool 
CheckForNumericPage(string& page)
	{
	// any characters outside the "numeric only" range of '0' thru '9' 
	//  contained in the message means that this is NOT a pure numeric message.
	//
	// return true only if pure numeric content is found.
	//
	if(page.empty())
		{
		// empty strings are not considered to be numeric.
		return false;
		}

	string::iterator iter = page.begin();

	for(; iter != page.end(); ++iter)
		{
        //SO - 03/14/2013 - Added *,-, space to be validated
		if(!((*iter>= '0' && *iter <= '9') || (*iter == '*' || *iter == '-' || *iter == ' ')))
			{	
                return false;
			}        
		}
	return true;
	}


	// New version of AddQueue () that supports "%r4" SMS Flag.
	//  for Mobile Enablement.
	//
	// Now, AddQueueEx () WriteUserLogEx () work closely together to do
	//  operations in the required sequence. 
	//
	// option 1 - Traditional Flat-File queues:
	//  Flat-File queue write followed by Xtend MessageBase write.
	
	// option 2 - Xtend MessageBase and Schedule table queue.
	//  Xtend MessageBase write followed by Schedule table database write.
	//
	// Use option 2 if the PagerType is listed in the g_csPageTypesWithSmsFlag list. 
	//  All other cases, use option 1.
	//
	bool
AddQueueEx(int PagePointer, int QPointer)
	{
	bool bReturnStatus = false;
	int iRecordNumberFromDatabase = 0;

	// Consult the SmsFlag to decide what to do now.
	//
	if(IsSmsType(PagePointer))
		{
		// FOUND the current PageType in our list of SmsFlagged types.
		//  Schedule Table queue write is required.
		//
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "SmsFlagged PageType: [%s]", page[PagePointer].PageType);
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "List of Sms types: [%s]", (LPCTSTR) g_csPageTypesWithSmsFlag);

		// identify the message data to be written.
		//  could be a numeric only message. contained in page[PagePointer].DisplayInfo.
		// xxxxxxxxxxxxxxxxxxx


		// The required sequence is to first write to Xtend MessageBase, 
		//  and second, write to the Schedule table.

		// First...
		// no "please call" required for SMS messages.
		//iRecordNumberFromDatabase = WriteUserLog(PagePointer, 0, QPointer, false);	
		iRecordNumberFromDatabase = WriteUserLog(PagePointer, 0, QPointer);	

		// Second...		
		if(iRecordNumberFromDatabase != -1)
			{
			// sensible message number, write a page request to the Schedule table.
			WriteToScheduleTable(PagePointer, iRecordNumberFromDatabase);
			}
		// SOguz - DE20747 - 10/03/2016 - Added the following to handle page all devices in 
		// failsafe mode where we write a message to the screen and the log that we won't be 
		// paging SMS type pagers in failsafe mode. This was agree upon by Debra Bahra and Bob Erdman 
		else
		{
			 if(DATA_SOURCE_LOCAL == DataSource)
			 {
				char msg1[250];
				char msg2[250];

				sprintf(msg1,"Page Server is in failsafe mode. Therefore, it is skipping SMS pagertype %%%s for profile %s.",
				page[PagePointer].PageType,
				page[PagePointer].Extension);

				sprintf(msg2, "");

				PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,								
				"PageAllDevices: %s",
				msg1);
				
			 }
		}
			// indicate that the page is now completed, just like AddQueue() would do.
			//  this is an essential part of the retry mechanism used by IVR processing.
			//  2010-02-25 kgill.
			//
			//DE20568 - SOguz - 1/8/2016 - I took the following statement inside of 
			//if(iRecordNumberFromDatabase != -1) code above. In FAILSAFE mode, it 
			//was causing IVR lines to be stuck in Hang Up mode since we cannot send  
			//SMS pages in FAILSAFE (MEConnector won't pick the message)  
			//
			strcpy(page[PagePointer].PageStatus, "C");
			

		// claim success.
		bReturnStatus = true;
		}
	else
		{
		// This queue type was NOT found in the list of SMS types, a flat-file queue write only is required, same as "old" way.
		//
		// write to Flat-File page queue.
		bReturnStatus = AddQueue(PagePointer, QPointer);
		}

	return bReturnStatus;
	}


	// returns true if Success, false if Fail.
	// PagePointer indexes the page[].xxx structure.
	//
	bool
AddQueue(int PagePointer, int QPointer)
	{
	bool bReturnStatus = false;

	// what info does the message contain? -- show it.
	PrintMessage(MSGTYPE_SYSTEM, 
		hwndGeneralMsg, 
		"AddQueue (): PageInfo: page[PagePointer].longMessage.c_str(): [%s]", 
		page[PagePointer].longMessage.c_str());

	// apply priority queue status if indicated.
	//
	// redirect the request to the high priority queue file.
	//
	if(page[PagePointer].PriorityQPointer > 0)
		{
		QPointer = param.PriorityPointer[QPointer][page[PagePointer].PriorityQPointer];
		}

	// before allowing a write to any queue 
	//  make sure that the queue is defined in the configuration.
	//  anything that is not defined is expected to be -1
	//  and will be ignored.
	//
	if(QPointer < 0)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"AddQueue (): the queue is not defined in the configuration. Failed to add to queue.");
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"AddQueue (): Suggestion: Please check QueueControl and Comports in pageserver configuration program.");
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"AddQueue (): Suggestion: if queue is defined in the configuration.",
						hwndGeneralMsg,
						"AddQueue (): Suggestion: Open the configuration program, save, and exit. Restart page server.");

		return bReturnStatus;	// failure
		}

	// all data is currently in the "page" structure (0 thru 135)
	//  selected data needs to be transferred to the QueueBufferClass.
	//
	// transfer items into QueueBufferClass.

	CString csTemp;

	csTemp.Format("%10.10s", page[PagePointer].InitiatedDate);
	Queue[QPointer].pQBC->SetInitiatedDate(csTemp);

	csTemp.Format("%8.8s", page[PagePointer].InitiatedTime);
	Queue[QPointer].pQBC->SetInitiatedTime(csTemp);

	csTemp.Format("%8.8s", page[PagePointer].Extension);	// ProfileId - 8 chars in Queue Structure
	Queue[QPointer].pQBC->SetProfileId(csTemp);

	csTemp.Format("%2.2s", page[PagePointer].PageType);
	Queue[QPointer].pQBC->SetPageType(csTemp);

	csTemp.Format("%2.2s", page[PagePointer].PageStatus);
	Queue[QPointer].pQBC->SetPageStatus(csTemp);

	if (ListMember_Covered==TRUE)
	{
		csTemp.Format("%7.7s", page[PAGE_COV_POS].PagerId);

	}
	else
	{
		csTemp.Format("%7.7s", page[PagePointer].PagerId);
	}
	//csTemp.Format("%7.7s", page[PagePointer].PagerId);
	Queue[QPointer].pQBC->SetPagerId(csTemp);

	csTemp.Format("%10.10s", page[PagePointer].InitiatorId);
	Queue[QPointer].pQBC->SetInitiatorProfileId(csTemp);

	Queue[QPointer].pQBC->SetVoiceFileNumber(page[PagePointer].VoiceFile);

	csTemp.Format("%-148.148s", page[PagePointer].PageInfo);
	Queue[QPointer].pQBC->SetPageInfo(csTemp);

	// need to copy over the long message from the page, jph 10/29/2007
	string tmpLongMessage = page[PagePointer].longMessage;

	int longMessageLength = (tmpLongMessage.erase(tmpLongMessage.find_last_not_of(" \r\n\t") + 1)).length();

	char * pStatus = page[PagePointer].PageType;

	// don't forget to check for pure numeric content.
	if(page[PagePointer].longMessage.empty())
		{
		// the long message is empty, take info from the "old" DisplayInfo field.
		//
		Queue[QPointer].pQBC->SetWholeMessage(string(page[PagePointer].DisplayInfo));
		}
	else
		{
		// The following code adds "Please Call" before a numeric page sent to 
		// an alpha-numeric pager.

		// use pageinfo, it could be a numeric page!!
		// check for numeric only		

		// DE23060 - SOguz - Changed the logic of the next 4 statements to fix a defect 
		// If the user sends list page with 148 or more characters and the last characters are numeric, 
		// then the application would break that into two parts and only send the second (numeric)
		// part of the message due to logic error in the code below.  
		// The old code was using DisplayInfo (second part which was numeric) to check whether 
		// the message was numeric. If it was numeric then it would use that to set the whole message
		// (pQBC->SetWholeMessage). This caused only the numeric part of the message to be sent to 
		// receipient. I fixed issue by using long message which has the complete message and append 
		// the "Please Call " text only if the long (complete) message is numeric.		

		bool numeric = CheckForNumericPage(tmpLongMessage);   

		if(numeric)
			{			
			tmpLongMessage.insert(0, "Please Call ");			
			}

		Queue[QPointer].pQBC->SetWholeMessage(tmpLongMessage);		
		}

	// save page number and auth code in queueBufferClass object!!!
	//
	Queue[QPointer].pQBC->pagerNumber.assign(page[PagePointer].PagerNumber);
	Queue[QPointer].pQBC->pagerAuthCode.assign(page[PagePointer].AutCode);


	// recognize the standard priority queue as well as the high priority queue.
	//
	bool StatPriority_listPage = (strncmp(page[PagePointer].PageType, Queue[LOW_PRIORITY_LIST_QUEUE].Id + 1, 2) == 0);
	bool StdPriority_listPage = (strncmp(page[PagePointer].PageType, Queue[HIGH_PRIORITY_LIST_QUEUE].Id + 1, 2) == 0);
	bool IsList = StatPriority_listPage || StdPriority_listPage; 

	// now, perform the queue write!
	if(Queue[QPointer].pQFM->WriteQueueItem(Queue[QPointer].pQBC, IsList))
		{
		// indicate that the page is now completed.
		//
		strcpy(page[PagePointer].PageStatus, "C");

		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "AddQueue (): added OK.");
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "PagerId: [%s]", page[PagePointer].PagerId);
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "VoiceFile: [%d]", page[PagePointer].VoiceFile);
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "PageInfo: [%s]", page[PagePointer].longMessage.c_str());

		// claim success.
		bReturnStatus = true;
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "AddQueue (): failed to add.");
		}	

	return bReturnStatus;
	}


	// message fix-up for alpha and numeric type messages
	//  translate '*' to '-' on some numeric messages,
	//  prepend "Please Call " message to alpha pagers
	//   requested thru dialogic ports that can only receive touch-tone chars.
	void
AddSetUp(int PagePointer, char * InitiatorId)
	{
    int i;
    SYSTEMTIME	CurrTime;
	char string2[300];

    GetLocalTime(&CurrTime);

    sprintf(page[ PagePointer ].InitiatedDate,
			"%02i/%02i/%04i",
			CurrTime.wMonth,
			CurrTime.wDay,
			CurrTime.wYear);

    sprintf(page[ PagePointer ].InitiatedTime,
			"%02i:%02i:%02i",
			CurrTime.wHour,
			CurrTime.wMinute,
			CurrTime.wSecond);

    strncpy( page[ PagePointer ].InitiatorId, InitiatorId, 10 );

    if( page[ PagePointer ].PageStatus[ 0 ] != 'L' )
        strncpy( page[ PagePointer ].PageStatus, "R", 2 );

    memset( page[ PagePointer ].PageInfo, 0, sizeof(page[PagePointer].PageInfo));

	// if .PagerNumber is empty, use default information from the btrieve file.
    if(page[PagePointer].PagerNumber[0] != 0)
		{
        strncpy(page[PagePointer].PageInfo, page[PagePointer].PagerNumber, sizeof(page[0].PageInfo));
		}

	// if necessary, append '+' and contents of .AutCode onto the .PageInfo.
    if(page[PagePointer].AutCode[0] != 0)
		{
        if(strlen(page[PagePointer].PageInfo) > 0)
			{
            strcat(page[PagePointer].PageInfo, "+" );
            strncat(page[PagePointer].PageInfo, page[PagePointer].AutCode, 15);
			}
        else
			{
            strncpy(page[PagePointer].PageInfo, page[PagePointer].AutCode, 15);
			}
		}

    if(page[PagePointer].DisplayInfo[0] != 0)
		{
        memset(string2, 0, sizeof(string2));
        Set_Null(page[PagePointer].DisplayInfo, sizeof(page[PagePointer].DisplayInfo));

		// several conditions indicate that this is an alpha page that
		//  may require message fix-up.
		//
        if((page[PagePointer].ComNumber != -1 &&   // comm port 0 thru 9 is defined.
			param.ComPort[page[PagePointer].ComNumber].QueType[page[PagePointer].ComOffset][1] == '1' && // == '1' means Alpha
			PagePointer < PAGE_LIST_POS) ||  // dialogic port, or comm port type page.
			(page[PagePointer].PageStatus[0] == 'L' &&						// 'L' is list page.
			PagePointer >= PAGE_COM_START && PagePointer < PAGE_LIST_POS))	// 96 thru 106 are comm ports.
			{
			// comm port 0 thru 9 is defined, and the page is alpha, and it is a comm or dialogic type page,
			// or, list page on a comm port.
            if( page[PagePointer].PageStatus[0] == 'L')
				{
                strcpy(string2, page[PagePointer].DisplayInfo);
                strcat(string2, "+");
				}

			/// //// // no canned alpha messages.
			if(strncmp(InitiatorId, "External", 8) == 0)
				{
				// prepend the param.AlphaMsg to the numeric message
				//  for alpha pagers, if non-null message use it.
				//  this applies to messages from pageserver IVR interface.
				strcpy(string2, param.AlphaMsg);
				strcat(string2, page[PagePointer].DisplayInfo);
				}
			else
				{
				strcpy(string2, page[PagePointer].DisplayInfo);
				}
			}
		else
			{
			// not an alpha page that requires message fix-up.

			// numeric page that may require message fix-up
			//  such as translate '*' character to '-' character.
			i = page[PagePointer].ComNumber;

			if(i != -1)
				{
				// == '0' means Numeric.
				if(param.ComPort[i].QueType[page[PagePointer].ComOffset][1] == '0' &&
					PagePointer < PAGE_COM_START)  // dialogic port.
					{
					// numeric page on a dialogic port.
					for(i = 0; i < (int) strlen(page[PagePointer].DisplayInfo); i++)
						{
						if(page[PagePointer].DisplayInfo[i] == '*')
							{
							// translate '*' to '-'
							string2[i] = '-';
							}
						else
							{
							// no translation, just copy the character.
							string2[i] = page[PagePointer].DisplayInfo[i];
							}
						}
					}
				else
					{
					strcpy( string2, page[ PagePointer ].DisplayInfo );
					}
				}
			else
				{
				strcpy( string2, page[ PagePointer ].DisplayInfo );
				}
			}

		int pageInfoRemain = sizeof(page[PagePointer].PageInfo) - strlen(page[PagePointer].PageInfo) - 1;
        if( strlen( page[ PagePointer ].PageInfo ) > 0 )
			{
            strncat( page[ PagePointer ].PageInfo, "+", pageInfoRemain);
			--pageInfoRemain;
            strncat( page[ PagePointer ].PageInfo, string2, pageInfoRemain);
			}
        else
			{
            strncat( page[ PagePointer ].PageInfo, string2, pageInfoRemain);
			}
		}
	}


	// returns 0 when
	//  page[].PageStatus[0] == 'L'   - list page is indicated
	//  page[].FileName == "ON CALL"  - an on-call is indicated
	//
	// returns -1 when ????
	// returns 0
	//
	// modifies:
	//  page[].interfaceType
	//  page[].ScriptNumber
	//  page[].ComNumber
	//  page[].ComOffset
	//
	// examines:
	//  page[].PageType
	//  page[].ScriptNumber
	//
	int
GetQuePointer(int PagePointer)
	{
    int i;
    int j;
    int QPointer = -1;
	char string2[300];

	memset(page[PagePointer].intefaceType, 0, sizeof(page[PagePointer].intefaceType));

    if(page[PagePointer].PageStatus[0] == 'L')	// 'L' is for list-page
		{
		// for list paging there is no physical port to be identified.
		// 
		// this could resolve to HIGH_PRIORITY_LIST_QUEUE, or LOW_PRIORITY_LIST_QUEUE e.g. 
		//  (%21 or %22). use PageType, it contains the required information.
		//
		sprintf(page[PagePointer].intefaceType, "%%%s", page[PagePointer].PageType);

		// find where the queue type is in the configuration.
		//  hint: it will be 0 or 29, but we must look, because it's not always %22 or %21.
		
		int PagePointerIndex = -1;

		for(int x = 0; x < 30; x++)
			{
			if(strcmp(page[PagePointer].intefaceType, Queue[x].Id) == 0)
				{
				// found it.
				PagePointerIndex = x;
				break;
				}
			}

		return PagePointerIndex;
		}

    if(strncmp(page[PagePointer].FileName, "ON CALL", 7) == 0)
		{
		strcpy(page[PagePointer].intefaceType, Queue[0].Id);
		return 0;
		}

    memset(string2, 0, sizeof(string2));
    strcpy(string2, "%");
    strncat(string2, page[PagePointer].PageType, 2);

    if(strncmp(string2, Queue[0].Id, 3) == 0)
		{
		strcpy(page[PagePointer].intefaceType, Queue[0].Id);
		return(0);
		}

    if(param.MultiQueNumber > 0)
		{
        if(strncmp(string2, Queue[param.MultiQueNumber ].Id, 3) == 0 )
			{
			strcpy( page[ PagePointer ].intefaceType, Queue[ param.MultiQueNumber ].Id );
            QPointer = param.MultiQueNumber;
			}
		}

    if( param.MultiQueNumber2 > 0 )
		{
        if( strncmp( string2, Queue[ param.MultiQueNumber2 ].Id, 3 ) == 0 )
			{
			strcpy( page[ PagePointer ].intefaceType, Queue[ param.MultiQueNumber2 ].Id );
            QPointer = param.MultiQueNumber2;
			}
		}

    if( page[ PagePointer ].ScriptNumber != -1 )
		{
        for( i=0; i<param.PgTypsNum; i++ )
			{
			// RCC pager id types.
            if( strncmp( string2, param.PageType[ i ].pageid, 3 ) == 0 )
				{
                page[ PagePointer ].ScriptNumber = i;
                page[ PagePointer ].ComNumber = -1;
                QPointer = PageType[ i ].QueuePointer;
				strcpy( page[ PagePointer ].intefaceType, param.PageType[ i ].QDirect );
                break;
				}
			}
		}
    else
		{
        for(i = 0; i < MAX_COMMPORTS; i++)
			{
            if(param.ComPort[i].PortActive[0] == 'N')
				continue;

            for(j = 0; j < 20; j++)
				{
                if(strncmp(string2, param.ComPort[i].PageId[j], 3) == 0)
					{
                    page[PagePointer].ScriptNumber = -1;
                    page[PagePointer].ComNumber = i;
                    page[PagePointer].ComOffset = j;
                    QPointer = ComPort[i].QueuePointer[j];
					strcpy(page[PagePointer].intefaceType, param.ComPort[i].QDirect[j]);
                    break;
					}
				}
			}
		}
    return QPointer;
	}


	// process IVR queue.
	//
	// the 96 IVR (Dialogic) queues, and ports are each numbered (0 thru 95) 
	//

	// parameter 1 - QueueUse
	//  for RCC types (0 thru 9) parameter 1
	//  indexes the PageType[QueUse].InQueuePointer to identify
	//  one of the 10 RCC structures.
	//
	// parameter 2 - channel 
	//  channel may have values (0 - 95) and indexes the page[], 
	//  and port[] data structures.
	//
	void
ProcessIvrQueue(int QueUse, int channel)
	{
	char string2[300];
    page[channel].ComNumber = -1;
	
    //US26901 - Sedat Oguz - 09/03/2013 - Commented the statements below
    // to fix a problem where page server does not process %60 types
    //******************************************************
    //bool want3params;

	//want3params = (pageactive[QueUse] == IXO_MODEM);

	//int MessagePartsRequired = want3params ? 3 : 2;
	//******************************************************

    //US26901 - Sedat Oguz - 09/03/2013 - Commented the statement below
    // to fix a problem where page server does not process %60 types
    //***************************************************************************************
    //if(ClaimQueueItem(PageType[QueUse].InQueuePointer, MessagePartsRequired,false) == TRUE)
    //***************************************************************************************
    //US26901 - Sedat Oguz - 09/03/2013 - number of message parts == 2 for RCC type pages.
    if(ClaimQueueItem(PageType[QueUse].InQueuePointer, 2) == TRUE)
		{
         //US26901 - Sedat Oguz - 09/03/2013 - Commented the statement below
        // to fix a problem where page server does not process %60 types.
        //***************************************************************************************
		// Want3 = true. - for "IVR page".
		//GetQueue(channel, QueUse, PageType[QueUse].InQueuePointer, want3params, __FILE__, __LINE__);	
        //***************************************************************************************

        //US26901 - Sedat Oguz - 09/03/2013 - The Want3 parameter should be set to false for RCC type pages.
		GetQueue(channel, QueUse, PageType[QueUse].InQueuePointer, false, __FILE__, __LINE__);

		if((strlen(page[channel].DisplayInfo) > 0 ||
				      param.TonePage) &&
					  strlen(page[channel].PagerNumber) > 0)
			{
			if(page[channel].PageStatus[0] != 'K')
				{
				port[channel].PortStatus = 2;
				port[channel].state = ST_CCPG_CALL;
				port[channel].timer.wHour = 70;
				port[channel].PageNumber = channel;
				port[channel].Attempts++;

				if(strncmp(page[channel].InitiatorId, "External", 8) == 0)
					{
					port[channel].External++;
					}

				page[channel].ScriptFlag = 0;
				sprintf(string2, "%%%2s", page[channel].PageType);
				page[channel].ScriptNumber = IdentifyDlType(QueUse, string2);
				page[channel].PortNumber = channel;
				page[channel].PageStatus[0] = 'B';
				CheckScript(channel);
				}
			else
				{
				page[channel].PageStatus[0] = 'I';
				}
			}
		else
			{
			page[channel].PageStatus[0] = 'I';
			}
		}

	if(page[channel].PageStatus[0] == 'R')
		{
		page[channel].PageStatus[0] = 'I';
		}

	if(page[channel].PageStatus[0] == 'K')
		{
		page[channel].PageStatus[0] = 'I';
		}
	}


	int
GetQuePos(char * QueId)
	{
    int i;

    for(i = 0; i < MAX_QUEUE_FILES; i++)
		{
        if(strncmp(QueId, Queue[i].Id, 3) == 0 && QueId[0] == '%')
			{
			return i;
			}
		}
    return -1;
	}


	// returns 0 for not complete,
	//  returns 1 for all done.
	//
	// all done, means that all queues have been renamed, 
	//  or don't need it, or have run out of retries.
	//
	bool
FileService(int iCommand)
	{
	const int MAX_ATTEMPTS = 10;
	static int Index = MAX_QUEUE_FILES;
	static int iAttemptCount = 0;

	// at midnight, attempt to rename all queue files,
	//  whether they need it or not.

	// after initialization,
	//  this routine gets called repeatedly, until
	//  the Index reaches its maximum value.
	//
	if(iCommand == 1)
		{
		// initialize static variables.
		Index = 0;
		iAttemptCount = 0;

		// and return true.
		return true;
		}

	// check if item is present in the application configuration.
	//
    if((Queue[Index].Id[0] == '%') && 
		(strcmp(param.stationId, Queue[Index].StationId) == 0))
		{
		// a configred item, that this PageServer 
		//  is responsible for recycling was detected...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"FileService(): - QueueNumber: [%d]  QueueId: [%s].",
						Index + 1, 
						Queue[Index].Id);	

		// request that the queue file be recycled at this time,
		//  regardless of the number of items it contains.
		//
		if(Queue[Index].pQFM->RecycleRequest())
			{
			// successful recycle of this file,
			//  advance the Index.
			//
			Index++;
			iAttemptCount = 0;
			}
		else
			{
			// unable to recycle... try MAX_ATTEMPTS times
			//  this could be due to file lock contention
			//  and should be resolved by simple try-again method.
			//
			iAttemptCount++;

			if(iAttemptCount >= MAX_ATTEMPTS)
				{
				// give up if retry count is at the limit,
				//  and advance to the next item.
				//
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"FileService(): - unable to recycle QueueNumber: [%d]",
								Index + 1);	
				Index++;
				iAttemptCount = 0;
				}
			}
		}
    else
		{
		// the queue item is not configured, 
		//  or this PageServer is NOT responsible for recyce.
		//  no action required, advance to the next item.
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"FileService(): - QueueNumber: [%d]  item not configured.",
						Index + 1);	
		Index++;
		}

	if(Index >= MAX_QUEUE_FILES)
		{
		// all have been processed, successful or not.
		return true;
		}
	else
		{
		// all items have not been completed, or retrying.
		return false;
		}
	}


	BOOL
CheckLeapYear(int year)
	{
	// returns true if the given year
	//  is a leap year 2000 - 2080
	switch(year)
		{
		case 2000:
		case 2004:
		case 2008:
		case 2012:
		case 2016:
		case 2020:
		case 2024:
		case 2028:
		case 2032:
		case 2036:
		case 2040:
		case 2044:
		case 2048:
		case 2052:
		case 2056:
		case 2060:
		case 2064:
		case 2068:
		case 2072:
		case 2076:
		case 2080:
			// these are leap years
			return TRUE;

		default:
			// non leap years.
			return FALSE;
		}
	}


	int
DaysSoFarThisYear(int year, int month, int day)
	{
	// returns day of year 0 thru 365, for given
	//  year, month and day.

	// table for leap year;
	int DaysInPreviousMonthsLeap[] =
		{
		(0														  ),
		(0 + 31													  ),
		(0 + 31 + 29											  ),
		(0 + 31 + 29 + 31										  ),
		(0 + 31 + 29 + 31 + 30									  ),
		(0 + 31 + 29 + 31 + 30 + 31								  ),
		(0 + 31 + 29 + 31 + 30 + 31 + 30						  ),
		(0 + 31 + 29 + 31 + 30 + 31 + 30 + 31					  ),
		(0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31				  ),
		(0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30			  ),
		(0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31	  ),
		(0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 )
		};

	// table for non-leap standard year;
	int DaysInPreviousMonthsStandard[] =
		{
		(0														  ),
		(0 + 31													  ),
		(0 + 31 + 28											  ),
		(0 + 31 + 28 + 31										  ),
		(0 + 31 + 28 + 31 + 30									  ),
		(0 + 31 + 28 + 31 + 30 + 31								  ),
		(0 + 31 + 28 + 31 + 30 + 31 + 30						  ),
		(0 + 31 + 28 + 31 + 30 + 31 + 30 + 31					  ),
		(0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31				  ),
		(0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30			  ),
		(0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31	  ),
		(0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 )
		};

	// table is 0 thru 11, months are 1 thru 12
	month--;

	// minus 1 because january 1st is day # 0
	//  january 1st is passed in as day == 1.
	day--;

	if(CheckLeapYear(year))
		{
		return(DaysInPreviousMonthsLeap[month] + day);
		}
	else
		{
		return(DaysInPreviousMonthsStandard[month] + day);
		}
	}


	/* prepare 3 formatted date and time strings,
	 *
	 *  b - blank space
	 *  MM - month
	 *  DD  - day
	 *  YYYY -  year
	 *	HH - hour
	 *  mm - minute
	 *  Y -	single digit year (units)
	 *  JJJ - "julian" date - day of year
	 *  W - weekday 0 == sunday, 6 == saturday
	 *
	 *              0000000001111111111222
	 *              1234567890123456789012
	 *  string   - "MM/DD/YYYYbbHH:mm:SSb" = 22 chars including NUL
	 *  dstring  - "MMDDHHWY"
	 *  dystring - "JJJ.0W"  used for "billing" statistics.
	 */

	void
MSGTimeStamp(char *dstring, char *dystring)
	{
	SYSTEMTIME st;
    char string[45];

	GetLocalTime(&st);

	// prepare full string for main window status line display.
    sprintf(string,
			"%02d/%02d/%04d  %02d:%02d:%02d ",
			st.wMonth,
			st.wDay,
			st.wYear,
			st.wHour,
			st.wMinute,
			st.wSecond);

	// post 19 char format to xnSystemTime
	//  this global variable is used
	//  many places in the application..
	//
	sprintf(xnSystemTime, "%20.20s", string);

    sprintf(dstring,
			"%02.2d%02.2d%02.2d%02.2d%01.1d%01.1d ",
			st.wMonth,
			st.wDay,
			// no year info required here.
			st.wHour,
			st.wMinute,
			st.wDayOfWeek,
			(st.wYear) % 10);	// take least significant digit of year

	// prepare a day of year, and weekday string

	// find day of year.
	//  january 1st == day # 0 of the year,
	//  december 31st == day # 365 of a leap year.
    sprintf(dystring,
			"%03d.%02d",
			DaysSoFarThisYear(st.wYear, st.wMonth, st.wDay),
			st.wDayOfWeek);

	// prepare global string with ISO-8601 style format.
	//  note 2 spaces between date and time parts of the string.
    sprintf(DateTimeISO8601,
			"%04.4d-%02.2d-%02.2d  %02.2d:%02.2d:%02.2d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond);

	// format with leading space for status line display.
    CString csDateAndTime;
	csDateAndTime.Format("  %s", DateTimeISO8601);

	SetStatusText(STATUS_LINE_TIME_OF_DAY, (LPTSTR) (LPCTSTR) csDateAndTime);
	}


	void
PageThePageSupervisor(int PagePointer)
	{
    int i;
    char string[10];
	char string2[300];

    memset(page[PagePointer].Extension, 0, sizeof(page[PagePointer].Extension));
    memset(page[PagePointer].PagerId, 0, sizeof(page[PagePointer].PagerId));

    if(param.SupperExt[0] == 0 || param.SupperExt[0] == ' ')
		{
		return;
		}

    page[ PagePointer ].VoiceFile = FALSE; // Indicates not a voice page
    memset( string2, 0, sizeof( string2));
    strcpy( string2, param.SupperExt );

    // validate the digits for range 0-9 
    for(i = 0; i < 10; i++)
		{
        if(string2[i] == ' ' || string2[i] == 0)
			break;
        page[PagePointer].Extension[i] = string2[i];
		}

    strncpy(page[PagePointer].PagerId, page[PagePointer].Extension, 10);

	GetPagerId(page[PagePointer].Extension, PagePointer, BY_PROFILEID);

    // check for the .log file present 
	if(LogPager(page[PagePointer].Extension, PagePointer) == FALSE)
		{
		PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg, 
						"Pager not assigned in the log file.");
		page[PagePointer].PageStatus[0] = 'I';
		return;
		}
	else
		{
		memset(string, 0, sizeof(string));
		sprintf(string, "%%%s", page[PagePointer].PageType);

		if(Get_Frq(PagePointer, string, string) == FALSE)
			{
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndGeneralMsg, 
							"No pager currently assigned. Check frq file - 3.");
			return;
			}
		}

    strcpy(page[PagePointer].DisplayInfo, "4114115678");
    AddSetUp(PagePointer, "External");
    page[PagePointer].PortNumber = GetQuePointer(PagePointer);

    if(page[PagePointer].PortNumber >= 0 || IsSmsType(PagePointer))
		{
        page[PagePointer].PriorityQPointer = 0;
 
        AddQueueEx(PagePointer, page[PagePointer].PortNumber);
        WriteUserLogEx(0, page[PagePointer].PortNumber);
		}
	return;
	}


	// fills variable page[PagePointer].PageType, (2 chars), and returns TRUE 
	//  if the PgType is found in the page server configuration, returns FALSE 
	//  if all attempts to find it fail. when the PgType is found, 
	//  various structure members page[PagePointer].xxx are filled, 
	//  dependent on where the PgType was found in the configuration.
	//
	int
GetPageType(int PagePointer, char * PgType, char* file, int line)
	{
    int j, k;
	char string2[300];

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
					"GetPageType(): Parameters: PagePointer [%d] PgType [%s]",
					PagePointer,
					PgType);


	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
					"Called From File:%s, Line %d",
					file,
					line);


    page[PagePointer].ComNumber = -1;
    page[PagePointer].ScriptNumber = -1;
    memset(page[PagePointer].PageType, 0, sizeof(page[PagePointer].PageType));

	// check for "stat priority"  list page, hard-coded to queue position 0.
	//
    if(strncmp(PgType, Queue[HIGH_PRIORITY_LIST_QUEUE].Id, 3) == 0)
		{
		// this is a LIST page - e.g. "%21".
        strncpy(page[PagePointer].PageType, Queue[HIGH_PRIORITY_LIST_QUEUE].Id + 1, 2);
        page[PagePointer].PageStatus[0] = 'L';

        sprintf(string2, "Stat Priority List page [%s] identified.", page[PagePointer].PageType);
        PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);

		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"GetPageType(): Results: page[PagePointer].PageType [%s]",
						page[PagePointer].PageType);
        return TRUE;
		}

	// check for "standard priority" list page, hard-coded to queue position 29.
	//
	// added 2009-04-27 kgill
	//
    if(strncmp(PgType, Queue[LOW_PRIORITY_LIST_QUEUE].Id, 3) == 0)
		{
		// this is a LIST page - e.g. "%22".
        strncpy(page[PagePointer].PageType, Queue[LOW_PRIORITY_LIST_QUEUE].Id + 1, 2);
        page[PagePointer].PageStatus[0] = 'L';

        sprintf(string2, "Standard Priority List page [%s] identified.", page[PagePointer].PageType);
        PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);

		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
						"GetPageType(): Results: page[PagePointer].PageType [%s]",
						page[PagePointer].PageType);
        return TRUE;
		}

    if(param.MultiQueNumber > 0)
		{
        if(strncmp(PgType, Queue[param.MultiQueNumber].Id, 3) == 0)
			{
            strncpy(page[PagePointer].PageType, Queue[param.MultiQueNumber].Id + 1, 2);
            PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, "Multi page 1 identified.");

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"GetPageType(): Results: page[PagePointer].PageType [%s]",
							page[PagePointer].PageType);
            return TRUE;
			}
		}

	// for each RCC page type, number of RCC page types listed in
	//  page server configuration utility misc. screen.
    for(j = 0; j < param.PgTypsNum; j++)
		{
        if(strncmp(PgType, param.PageType[j].pageid, 3) == 0)
			{
            page[PagePointer].ScriptNumber = j;
            page[PagePointer].ScriptFlag = 0;
            strncpy(page[PagePointer].PageType, param.PageType[j].pageid + 1, 2);

            sprintf(string2, "Dialogic page type %i identified.", j);
            PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"GetPageType(): Results: page[PagePointer].PageType [%s]",
							page[PagePointer].PageType);
			return TRUE;
			}
		}

	// RCC can have an alias.
    for(j = 0; j < param.PgTypsNum; j++)
		{
        if(strncmp(PgType, param.PageType[j].alias, 3) == 0)
			{
            page[PagePointer].ScriptNumber = j;
            page[PagePointer].ScriptFlag = 0;
            strncpy(page[PagePointer].PageType, param.PageType[j].pageid + 1, 2);

            sprintf(string2, "Dialogic page alias %i identified.", j);
            PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"GetPageType(): Results: page[PagePointer].PageType [%s]",
							page[PagePointer].PageType);
            return TRUE;
			}
		}

	// for each active com port 0 thru XX, and each queue id 0 thru 19 ... PageId
    for(j = 0; j < MAX_COMMPORTS; j++)
		{
        if(param.ComPort[j].PortActive[0] == 'N')
			continue;

        for(k = 0; k < 20; k++)
			{
            if(param.ComPort[j].PageId[k][0] == '%')
				{
                if(strncmp(PgType, param.ComPort[j].PageId[k], 3) == 0)
					{
					// match found... the given PgType, and config parameter PageId match.
                    page[PagePointer].ComNumber = j;
                    page[PagePointer].ComOffset = k;
                    page[PagePointer].ScriptFlag = -99;

					// in the configuration parameters for com ports,
					//  we need to index one character past the '%' char
					//  in the string "%57", to pick-up just the "57"
					//  numeric part, and assign 2 chars to .PageType member.
                    strncpy(page[PagePointer].PageType, param.ComPort[j].PageId[k] + 1, 2);

                    sprintf(string2, 
							"Serial page type comport[%i] entry[%i] identified.", 
							j + 1,
							k);

                    PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"GetPageType(): Results: [%s]",
									page[PagePointer].PageType);
					return TRUE;
					}
				}
			}
		}

	// for each active com port, and each queue id ... Alias
    for(j = 0; j < MAX_COMMPORTS; j++)
		{
        if(param.ComPort[j].PortActive[0] == 'N')
			continue;

        for(k = 0; k < 20; k++)
			{
            if(param.ComPort[j].PageId[k][0] == '%')
				{
                if(strncmp(PgType, param.ComPort[j].Alias[k], 3) == 0)
					{
					// match found... the given PgType, and config parameter Alias match.
                    page[PagePointer].ComNumber = j;
                    page[PagePointer].ComOffset = k;
                    page[PagePointer].ScriptFlag = -99;

                    strncpy(page[PagePointer].PageType, param.ComPort[j].PageId[k] + 1, 2);

                    sprintf(string2, 
							"Serial page alias comport[%i] entry[%i] identified.", 
							j + 1,
							k);

                    PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2);

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"GetPageType(): Results: [%s]",
									page[PagePointer].PageType);
					return TRUE;
					}
				}
			}
		}

    if(param.MultiQueNumber2 > 0)
		{
 		if(strncmp(PgType, Queue[param.MultiQueNumber2].Id, 3) == 0)
			{
            strncpy(page[PagePointer].PageType, Queue[param.MultiQueNumber2].Id + 1, 2);
            PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, "Multi page 2 identified.");

			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
							"GetPageType(): Results: page[PagePointer].PageType [%s]",
							page[PagePointer].PageType);
            return TRUE;
			}
		}

	PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "GetPageType(): Results: FALSE");
	return FALSE;
	}


	int
IdentifyComType(int j, char * PgType)
	{
    int k;

    for(k = 0; k < 20; k++)
		{
        if(param.ComPort[j].PageId[k][0] == '%')
			{
            if(strncmp(PgType, param.ComPort[j].PageId[k], 3) == 0)
				{
                return k;
				}
			}
		}
    return k;
	}


	int
IdentifyDlType(int pType, char * PgType)
	{
    int j;

    for(j = 0; j < param.PgTypsNum; j++)
		{
        if(strncmp(PgType, param.PageType[j].pageid, 3) == 0)
			{
            return j;
			}
		}
    return pType;
	}


	int
GetNumberOfTitleLinesInQueueStatusWindow(void)
	{
	const int HeaderLines = 3;

	return (int) HeaderLines;
	}


	void
WriteToQueueStatusWindow(int Line, LPCSTR Text)
	{
	// disable re-draw temporarily.
	SendMessage(hwndQueStatus, WM_SETREDRAW, (WPARAM) FALSE, (LPARAM) 0);

	// update line item.
	SendMessage(hwndQueStatus, LB_DELETESTRING, (WPARAM) Line, (LPARAM) 0L);
	SendMessage(hwndQueStatus, LB_INSERTSTRING, (WPARAM) Line, (LPARAM) (LPSTR) Text);

	// allow re-draw now.
	SendMessage(hwndQueStatus, WM_SETREDRAW, (WPARAM) TRUE, (LPARAM) 0);
	}


	void
InitializeQueueStatusWindow(int IsStandby)
	{
	int L;

	// display the title/header lines:
	//
	WriteToQueueStatusWindow(0, "Queue   Queue  Total  Items  ");
	WriteToQueueStatusWindow(1, "Number  Name   Items  Waiting");
	WriteToQueueStatusWindow(2, "------  -----  -----  -------");

	// write empty info to each line.
	//
	for(L = 0; L < MAX_QUEUE_FILES; L++)
		{
		WriteToQueueStatusWindow(GetNumberOfTitleLinesInQueueStatusWindow() + L, 
									" ");
		}

	// post a standby message if requested.
	//
	if(IsStandby)
		{
		WriteToQueueStatusWindow(4, "  Standing By ...");
		}
	}


	// post the given message to the "queue status" window.
	//  for this queue.
	// if the message is empty, post the standard
	//  format message indicating 4 columns of
	//  standard queue file info.
	//   queue id,
	//   items in the queue file,
	//   number Unclaimed (waiting),
	//   file handle for the queue file.
	void
UpdateQueueStatusWindow(int QPointer)
	{
    char NewMessageContent[80];
    char CurrentMessageContent[80];

	// if the window is not open, or has been closed,
	//  the handle should be NULL,
	//  skip any window access/update.
	if(hwndQueStatus == NULL)
		{
		return;
		}

	// decide where to put the information inside the listbox,
	//  leaving header lines undisturbed.
	//
	int ListboxLineItemToUpdate = GetNumberOfTitleLinesInQueueStatusWindow() + 
									QPointer;

 	// get some up-to-date information.
	//
	int iLastWritten = Queue[QPointer].pQFM->GetLastWritten();
	int iUnclaimedCount = Queue[QPointer].pQFM->GetUnclaimedCount();

 	sprintf(NewMessageContent,
			"  %2.2i     %3.3s    %03u     %03u",
			QPointer + 1,
			Queue[QPointer].Id,
			iLastWritten,
			iUnclaimedCount);

	// before requesting an update of the information in the listbox,
	//  check to see if what we are about to write, is what's
	//  there already.
	//
	memset(CurrentMessageContent, 0, sizeof(CurrentMessageContent));

	SendMessage(hwndQueStatus, 
				LB_GETTEXT, 
				(WPARAM) ListboxLineItemToUpdate, 
				(LPARAM) CurrentMessageContent);

	if(strcmp(CurrentMessageContent, NewMessageContent) == 0)
		{
		// if text is the same, no need to update.
		return;
		}
	else
		{
		WriteToQueueStatusWindow(ListboxLineItemToUpdate, NewMessageContent);
		}
	}


	void
SetQueueCancel(int iport, int PagePointer)
	{
    int ScriptPointer;
    int i = -1;

	// only 1 of iport, or PagePointer will be defined...
	//  the other one will have a value of -1.
	//
	// iport is a comm port number having values between 0 and MAX_COMMPORTS.
	// PagePointer is a ivr port number having values between 0 and 95.

	// backtrack thru the configuration to find the queue number 0 thru 29 
	//  that should have its cancel count increased.
	//
    if(iport >= 0 && iport < MAX_COMMPORTS)
		{
        i = ComPort[iport].InQueuePointer[page[iport + PAGE_COM_START].ComOffset];
		}

    if(PagePointer > 0 && PagePointer < PAGE_COM_START)
		{
        ScriptPointer = page[PagePointer].ScriptNumber;
        i = PageType[ScriptPointer].InQueuePointer;
		}

    if((i >= 0) && (i < MAX_QUEUE_FILES))
		{
  		Queue[i].Cancels++;
		}
	}


	void
SetQueueError(int iport, int PagePointer, int Type)
	{
    int ScriptPointer;
    int i = -1;

    if((iport >= 0) && (iport < MAX_COMMPORTS))
		{
        i = ComPort[iport].InQueuePointer[page[iport + PAGE_COM_START].ComOffset];
		}

    if(PagePointer > 0 && PagePointer < PAGE_COM_START)
		{
        ScriptPointer = page[PagePointer].ScriptNumber;
        i = PageType[ScriptPointer].InQueuePointer;
		}

    if(i >= 0 && i < MAX_QUEUE_FILES)
		{
        if(Type == 2)
			Queue[i].Errors++;
		else
			Queue[i].Errors++;
		}
	}


	void
CancelPrint(int PagePointer)
	{
	// if the "Critical Printing" option is set 
	//  to either 'B' or 'C' the report will be printed.  
	// note: 'A' for ABORTED is a mystery, 
	//  and will not be printed... what does ABORTED mean???
	//
	SYSTEMTIME stLocal;
	CString csDateTimeStation;
	CString csCustomerInfo;
	CString csHr('-', 65);	// "horizontal rule", a string of XX '-' chars.

	SpoolCollection * pscCancelledPage = 0;

	pscCancelledPage = new SpoolCollection();

    pscCancelledPage->CancelledPagePrinting(START_PAGE, NULL);

	// report title:
	pscCancelledPage->CancelledPagePrinting(HEADER_1, "CANCELLED PAGE REPORT");

	// report header:
	pscCancelledPage->CancelledPagePrinting(HEADER_2, "Spok, Inc. PageServer - Cancelled Page Report");

	// customer identification information.
	csCustomerInfo.Format("Customer Name: [%s]", param.CustomerInfoFullName);
	pscCancelledPage->CancelledPagePrinting(HEADER_2, (LPSTR) (LPCTSTR) csCustomerInfo);

	// date presented:
	GetLocalTime(&stLocal);
	
	csDateTimeStation.Format("Date: [%04d-%02d-%02d]  Time: [%02d:%02d:%02d]  PageServer StationId: [%s]", 
								stLocal.wYear,
								stLocal.wMonth,  
								stLocal.wDay, 
								stLocal.wHour, 
								stLocal.wMinute, 
								stLocal.wSecond,
								param.stationId);

	pscCancelledPage->CancelledPagePrinting(HEADER_3, (LPSTR) (LPCTSTR) csDateTimeStation);

    pscCancelledPage->CancelledPagePrinting(MID_PAGE,
					"Profile Id: [%s]  Id: [%s]",
					page[PagePointer].Extension,
					page[PagePointer].PagerId);

    pscCancelledPage->CancelledPagePrinting(MID_PAGE,
					"Initiated: [%10.10s], [%8.8s]",
					page[PagePointer].InitiatedDate,
					page[PagePointer].InitiatedTime);

    pscCancelledPage->CancelledPagePrinting(MID_PAGE,
					"PageType : [%%%2.2s]  Initiated By: [%s]",
					page[PagePointer].PageType,
					page[PagePointer].InitiatorId);

	if(PagePointer >= PAGE_COM_START && PagePointer < PAGE_LIST_POS)
		{
		if(ComPort[PagePointer - PAGE_COM_START].errorResponse[0] != 0)
			{
			pscCancelledPage->CancelledPagePrinting(MID_PAGE,
							"Response : [%s]",
							ComPort[PagePointer - PAGE_COM_START].errorResponse);
			}
		}

    pscCancelledPage->CancelledPagePrinting(MID_PAGE,
					"Number Sent: [%s]",
					page[PagePointer].PagerNumber);

    pscCancelledPage->CancelledPagePrinting(MID_PAGE,
					"Id Number: [%s]",
					page[PagePointer].AutCode);

	int msgLength = strlen(page[PagePointer].DisplayInfo);
	int start;
	int last = 0;
	int count;

	pscCancelledPage->CancelledPagePrinting(MID_PAGE, "Message Sent:");

	while(msgLength)
		{
		start = last;
		if(msgLength > 65)
			{
			msgLength -= 65;
			last += 65;
			count = 65;
			}
		else
			{
			last += msgLength;
			count = msgLength;
			msgLength = 0;
			}

		CString msg(&page[PagePointer].DisplayInfo[start], count);
		pscCancelledPage->CancelledPagePrinting(MID_PAGE,
					"%s",
					static_cast<LPCTSTR>(msg));
		}

	// report footer:
	pscCancelledPage->CancelledPagePrinting(MID_PAGE, " ");
	pscCancelledPage->CancelledPagePrinting(MID_PAGE, (LPTSTR) (LPCTSTR) csHr);

	pscCancelledPage->CancelledPagePrinting(END_PAGE, NULL);
	}


	//  multiple page queue process. 
	//
	void
CheckMultiQue(int QueNumber, SYSTEMTIME * multiQTimer, char * MultiPageQues)
	{
    char string3[ 10 ];
	char string2[ 300 ];

    if(MultiQue.Active == FALSE)
		{
        if(multiQTimer->wHour == 70)
			{
			GetLocalTime(multiQTimer);
			}

		// changed from hardcoded value of 500 or 200,
		//  what is the purpose of this delay???
        if(Wait(multiQTimer, (long) 50))
			{
			multiQTimer->wHour = 70;

            if(CheckQueue(QueNumber) > 0)
				{
				if(ClaimQueueItem(QueNumber, 2) == TRUE)
					{
					// Want3 = false. - for "multi-queue" page.
					GetQueue(PAGE_MULTI_POS, -1, QueNumber, false, __FILE__, __LINE__);		

					strncpy(MultiQue.PageNumber, page[PAGE_MULTI_POS].PagerNumber, sizeof(MultiQue.PageNumber));
					strncpy(MultiQue.PageAlpha, page[PAGE_MULTI_POS].DisplayInfo,
								sizeof(MultiQue.PageAlpha) - 1);
					Set_Null( MultiQue.PageNumber, sizeof( MultiQue.PageNumber ) );
					Set_Null( MultiQue.PageAlpha, sizeof( MultiQue.PageAlpha ) );
					MultiQue.Active = QueNumber;
					MultiQue.QTypePointer = 0;
					MultiQue.retry = 0;
					memset( MultiQue.InitiatorId, 0, sizeof( MultiQue.InitiatorId ) );
					strncpy( MultiQue.InitiatorId, page[ PAGE_MULTI_POS ].InitiatorId, 10 );

					page[ PAGE_MULTI_POS ].PriorityQPointer = 0;
					if(page[ PAGE_MULTI_POS ].VoiceFile > 0)
						{
						memset(string2, 0, sizeof( string2));
						sprintf(string2, "%04i.VPO", page[PAGE_MULTI_POS].VoiceFile );

						if(strncmp(page[PAGE_MULTI_POS].InitiatorId, "CATS PG", 7) == 0)
							{
							sprintf(string2, "%04i.CAT", page[PAGE_MULTI_POS].VoiceFile);
							PrintMessage(MSGTYPE_SYSTEM, 
											hwndGeneralMsg, 
											"CheckMultiQue(): change .vpo to .cat");
							}
						else
							{
							PrintMessage(MSGTYPE_SYSTEM, 
											hwndGeneralMsg, 
											"CheckMultiQue(): leave .vpo as .vpo");
							}

						memset(MultiQue.PageVoice, 0, sizeof(MultiQue.PageVoice));
						/* MultiQue.PageVoice[ 0 ] = 'N';  6/14/96 */
						MultiQue.PageVoice[ 0 ] = page[ PAGE_MULTI_POS ].InitiatedDate[ 0 ];
						MultiQue.PageVoice[ 1 ] = page[ PAGE_MULTI_POS ].InitiatedDate[ 1 ];
						MultiQue.PageVoice[ 2 ] = page[ PAGE_MULTI_POS ].InitiatedDate[ 3 ];
						MultiQue.PageVoice[ 3 ] = page[ PAGE_MULTI_POS ].InitiatedDate[ 4 ];
						strcat( MultiQue.PageVoice, string2 );
						}
					sprintf(string2, 
							"Multi Extension %s, Number %s, Alpha %s, Voice %s.", 
							page[ PAGE_MULTI_POS ].Extension, 
							MultiQue.PageNumber, 
							MultiQue.PageAlpha, 
							MultiQue.PageVoice);

					PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg, string2 );
					}
				}
            else
				{
                multiQTimer->wHour = 70;
				}
			}
		}

    if(MultiQue.Active == QueNumber)
		{
        memset(string3, 0, sizeof(string3));

        int i = strlen(MultiPageQues);
		int qTypePointerStart = MultiQue.QTypePointer;

        for(MultiQue.QTypePointer=MultiQue.QTypePointer;
				MultiQue.QTypePointer < i;
				MultiQue.QTypePointer++)
			{
            if(MultiPageQues[MultiQue.QTypePointer] == '%')
				{
                string3[ 0 ] = MultiPageQues[ MultiQue.QTypePointer ];
                MultiQue.QTypePointer++;
                string3[ 1 ] = MultiPageQues[ MultiQue.QTypePointer ];
                MultiQue.QTypePointer++;
                string3[ 2 ] = MultiPageQues[ MultiQue.QTypePointer ];
                break;
				}
			}

        if( string3[ 0 ] != 0 )
			{
            page[ PAGE_MULTI_POS ].ComNumber = -1;
            page[ PAGE_MULTI_POS ].ScriptNumber = -1;
            if( Get_Frq( PAGE_MULTI_POS, string3, string3 ) == TRUE )
				{
                strcpy( page[ PAGE_MULTI_POS ].DisplayInfo, MultiQue.PageNumber );
                if( page[ PAGE_MULTI_POS ].ComNumber != -1 &&
					param.ComPort[ page[ PAGE_MULTI_POS ].ComNumber ].QueType[ page[ PAGE_MULTI_POS ].ComOffset ][ 1 ] == '1' )
					{
                    if( MultiQue.PageAlpha[ 0 ] != 0 )
						{
                        memset( page[ PAGE_MULTI_POS ].DisplayInfo, 0 ,sizeof( page[ PAGE_MULTI_POS ].DisplayInfo ) );
                        strcpy( page[ PAGE_MULTI_POS ].DisplayInfo, MultiQue.PageAlpha );
						}
					}
                AddSetUp( PAGE_MULTI_POS, MultiQue.InitiatorId );

                if( page[ PAGE_MULTI_POS ].ScriptNumber == -1 )
                    page[ PAGE_MULTI_POS ].PortNumber = ComPort[ page[ PAGE_MULTI_POS ].ComNumber ].QueuePointer[ page[ PAGE_MULTI_POS ].ComOffset ];
                else
                    page[ PAGE_MULTI_POS ].PortNumber = PageType[ page[ PAGE_MULTI_POS ].ScriptNumber ].QueuePointer;

                if(page[PAGE_MULTI_POS].PortNumber >= 0)
					{
                    page[PAGE_MULTI_POS].PriorityQPointer = 0;

                    if(AddQueueEx(PAGE_MULTI_POS, page[PAGE_MULTI_POS].PortNumber))
						{
						// AddQueue () was successful.
						//
						MultiQue.retry = 0;
						}
					else
						{
						if(MultiQue.retry < 10)
							{
							MultiQue.retry++;
							MultiQue.QTypePointer = qTypePointerStart;
							}
						else
							{
							MultiQue.retry = 0;
							memset(string2, 0, sizeof(string2));
							sprintf(string2,
									"Multi Que Error Profile Id: %s. Page Type: %s.",
									page[PAGE_MULTI_POS].Extension,
									string3 );
							WriteLog("NOPGERR", string2);
							}
						}
					}
                else
					{
                    sprintf(string2,
							"Multi Q add not ready %s, %s, fh %i.",
							page[PAGE_MULTI_POS].Extension,
							page[PAGE_MULTI_POS].PageType,
							page[PAGE_MULTI_POS].PortNumber);

                    WriteLog("NOPGABRT", string2);
                    PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);
					}
				}
			}
        else
			{
            sprintf(string2, "Multi-Page processing completed");
            PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, string2);
            MultiQue.Active = FALSE;
			}
		}
	}


	void
NoPaging(void)
	{
    int i;
    int j;

    for(i = 0; i < param.PgTypsNum; i++)
		{
        PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
			"Id: %s, Alias: %s, Directed %s, Prefix %s, Number: %s, Authcode: %s, Type %i.",
			param.PageType[i].pageid,
			param.PageType[i].alias,
			param.PageType[i].QDirect,
			param.PageType[i].prefix,
			param.PageType[i].number,
			param.PageType[i].autCode,
			param.PageType[i].PortType);
		}

    for(i = 0; i < MAX_COMMPORTS; i++)
		{
        if(param.ComPort[i].PortActive[0] != 'N')
			{
            for(j = 0; j < 20; j++)
				{
                if(param.ComPort[i].PageId[j][0] == '%')
					{
                    PrintMessage(MSGTYPE_SYSTEM,  hwndGeneralMsg,
						"COM%i: Id: %s, Alias: %s, Directed %s, Prefix: %s, Number: %s, Suffix: %s, Type %2.2s.",
						(i + 1),
						param.ComPort[i].PageId[j],
						param.ComPort[i].Alias[j],
						param.ComPort[i].QDirect[j],
						param.ComPort[i].Prefix[j],
						param.ComPort[i].number[j],
						param.ComPort[i].Suffix[j],
						param.ComPort[i].QueType[j]);
					}
				}
			}
		}

    for(i = 1; i < MAX_QUEUE_FILES; i++)
		{
        if(Queue[i].Id[0] == '%')
			{
            PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
				"%s: Priorities: %i, %i, %i, %i, %i, %i, %i, %i, %i, %i.",
				Queue[i].Id,
				param.PriorityPointer[i][0],
				param.PriorityPointer[i][1],
				param.PriorityPointer[i][2],
				param.PriorityPointer[i][3],
				param.PriorityPointer[i][4],
				param.PriorityPointer[i][5],
				param.PriorityPointer[i][6],
				param.PriorityPointer[i][7],
				param.PriorityPointer[i][8],
				param.PriorityPointer[i][9]);
			}
		}
	}


	// returns xxx if success
	//  returns yyy if failure,
	//  what is success? what is failure.
	//  what is the purpose.
	//
	// fills - page[PagePointer].PagerNumber,
	//         page[PagePointer].AutCode
	//
	// page[PagePointer].PageType is filled, if it is a multi-queue type
	// page[PagePointer].PageType if it is a list type pager,
	//  page[PagePointer].PageStatus[0] == 'L'
	//
	// if it is a "comm port" type pager, ... (commnumber == -1)
	// if it is an "IVR" type pager, ...	(scriptnumber == -1)

	int
Parse_Frq(int PagePointer,
			char * QueString,
			char * QueString2,
			char * buffer1,
			char * buffer3)
	{
    int ScriptPointer;
    int retval = FALSE;
    int i;
    int j;
    unsigned int k;
    char ServerScript[85];
    char SrvTok[5];
    char string1[85];
    char *number;

	PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"Parse_Frq() - 1 PagePointer [%d], QueString [%s], QueString2 [%s]",
					PagePointer,
					QueString,
					QueString2);

	PrintMessage(MSGTYPE_SYSTEM,
					hwndGeneralMsg,
					"Parse_Frq() - 2 buffer1 [%s], buffer3[%s]",
					buffer1,
					buffer3);

    ScriptPointer = page[PagePointer].ScriptNumber;

    memset(string1, 0, sizeof(string1));
    strncpy(string1, buffer3, 3 );
    Set_Null(string1, sizeof(string1));

    if((strstr(QueString, string1) != NULL) ||
		(strstr(QueString2, string1) != NULL))
		{
		// buffer3 found in QueString OR buffer3 found in QueString2 .
		//

		// fill string1 with "capcode"
        memset(string1, 0, sizeof(string1));
        strcpy(string1, buffer1);
        Set_Null(string1, sizeof(string1));
        Remove_CrLf(string1, sizeof(string1));

        if(ScriptPointer == -1 && page[PagePointer].ComNumber == -1)
			{
			// check for "stat priority" list or for "standard priority" list.
            if(strncmp(buffer3, Queue[HIGH_PRIORITY_LIST_QUEUE].Id, 3) == 0)
				{
				// matches with the first element, ... this is a "list".
                page[PagePointer].PageStatus[0] = 'L';
                strncpy(page[PagePointer].PageType, Queue[HIGH_PRIORITY_LIST_QUEUE].Id + 1, 2);
                return TRUE;
				}

 			// check for "standard priority" list.
           if(strncmp(buffer3, Queue[LOW_PRIORITY_LIST_QUEUE].Id, 3) == 0)
				{
				// matches with the first element, ... this is a "list".
                page[PagePointer].PageStatus[0] = 'L';
                strncpy(page[PagePointer].PageType, Queue[LOW_PRIORITY_LIST_QUEUE].Id + 1, 2);
                return TRUE;
				}

            if( param.MultiQueNumber > 0)
				{
                if(strncmp(buffer3, Queue[param.MultiQueNumber].Id, 3) == 0)
					{
                    strncpy(page[PagePointer].PageType,
							Queue[param.MultiQueNumber].Id + 1,
							2);

                    return TRUE;
					}
				}

            if(param.MultiQueNumber2 > 0)
				{
                if(strncmp( buffer3, Queue[param.MultiQueNumber2].Id, 3) == 0)
					{
                    strncpy(page[PagePointer].PageType,
							Queue[param.MultiQueNumber2].Id + 1,
							2);

                    return TRUE;
					}
				}
			}

		// pager type is unknown, see if we can identify it
		//  as a comm port type.
        if(ScriptPointer == -1)
			{
			PrintMessage(MSGTYPE_SYSTEM_PROGRESS, hwndGeneralMsg, "Processing for COMM port types");

            for(i = 0; i < MAX_COMMPORTS; i++)
				{
 				if(param.ComPort[i].PortActive[0] == 'N')
					{
					continue;
					}

				// there are up to 20 elements configured for each comm port
                for(j = 0; j < 20; j++)
					{
					// PageId is a misnomer, this variable contains the
					//  queue type e.g. "%57", we are working toward identifying
					//  the comm port to use.
                    if(strncmp(buffer3, param.ComPort[i].PageId[j], 3) == 0 ||
						strncmp(buffer3, param.ComPort[i].Alias[j], 3) == 0)
						{
						// something good just happened, remember which comm port to use
						//  and which element as well.
						// buf3 matches one of the 20 items (column1 of the configuration)
						//  for this comm port.
						//
                        if( strstr( QueString, param.ComPort[ i ].PageId[ j ] ) == NULL )
							{
							// allow this item, it is listed
							//  as a voice queue in the configuration (queue info tab)
							continue;
							}

						// a match between the xnfrq database item,
						//  that is listed in the p.s. configuration
						//  and is listed as a voice type pager has been found.
						//
						page[ PagePointer ].ComNumber = i;
                        page[ PagePointer ].ComOffset = j;
                        page[ PagePointer ].ScriptFlag = -99;

                        strncpy(page[PagePointer].PageType,
								param.ComPort[i].PageId[j] + 1,
								2);

                        PrintMessage(MSGTYPE_SYSTEM_FINAL, hwndGeneralMsg,
										"Page on COM%i: identified, "
										"type [%i] [%s], QueType [%2.2s]",
										i + 1,
										j,
										param.ComPort[i].PageId[j],
										param.ComPort[i].QueType[j]);

                        if(pageactive[i] != IXO_MODEM)
							{
							//   != IXO_MODEM
                            memset(string1, 0, sizeof(string1));
                            strcpy(string1, param.ComPort[i].Prefix[j]);

                            strcat(string1, buffer1);

                            Set_Null(string1, sizeof(string1));

                            strcat(string1, param.ComPort[i].Suffix[j]);
                            strncpy(page[PagePointer].PagerNumber,
										string1,
										sizeof(page[PagePointer].PagerNumber));
							}
                        else
							{
							//   == IXO_MODEM
                            if(param.ComPort[i].number[j][0] != 0)
								{
                                strncpy(page[PagePointer].PagerNumber,
										param.ComPort[i].number[j],
										sizeof(page[PagePointer].PagerNumber));

                                strncpy(page[PagePointer ].AutCode,
										buffer1,
										sizeof(page[ PagePointer].AutCode));

								Set_Null(page[PagePointer].AutCode,
											sizeof(page[PagePointer].AutCode));
								}
                            else
								{
                                memset(string1, 0, sizeof(string1));
                                strcpy(string1, buffer1);
                                Set_Null(string1, sizeof(string1));
                                Remove_CrLf(string1, sizeof(string1));
                                number = strtok(string1, "+\0");

                                if(number != NULL)
									{
	                                strncpy(page[PagePointer].PagerNumber,
											param.ComPort[i].Prefix[j],
											sizeof(page[PagePointer].PagerNumber));

                                    strncat(page[PagePointer].PagerNumber,
											number,
											sizeof(page[PagePointer].PagerNumber));

									strncat(page[PagePointer].PagerNumber,
											param.ComPort[i].Suffix[j],
											sizeof(page[PagePointer].PagerNumber));

                                    number = strtok( NULL, "+\0" );

                                    if( number != NULL )
										{
                                        strncpy(page[PagePointer].AutCode, number,
												sizeof( page[ PagePointer ].AutCode ) );

                                        Set_Null(page[PagePointer].AutCode,
													sizeof( page[ PagePointer ].AutCode ) );
										}
									}
								}
							}
                        retval = TRUE;
						}
					}
				}
			}

        if(page[PagePointer].ComNumber == -1)
			{
			// it's not a comm port type page,
			//  maybe it's an IVR type pager. (rcc type pager)
			PrintMessage(MSGTYPE_SYSTEM_PROGRESS, hwndGeneralMsg, "Processing for RCC port types");

			// for each RCC page type
            for(j = 0; j < param.PgTypsNum; j++)
				{
				PrintMessage(MSGTYPE_SYSTEM_PROGRESS,
								hwndGeneralMsg,
								"checking RCC [%d]", j);

                if((strncmp(buffer3, param.PageType[j].pageid, 3) == 0) ||
					(strncmp(buffer3, param.PageType[j].alias, 3) == 0))
					{
					PrintMessage(MSGTYPE_SYSTEM_FINAL,
									hwndGeneralMsg,
									"RCC page type [%d] found, Config param:[%s] buffer3: [%s]",
									j,
									param.PageType[j].pageid,
									buffer3);

                    retval = TRUE;

                    page[ PagePointer ].ScriptNumber = j;
                    strncpy( page[ PagePointer ].PageType, param.PageType[ j ].pageid+1, 2 );

                    SrvTok[0] = 0;

                    strcpy(ServerScript, param.PageType[j].Script);

                    for(k = 0; k<strlen(ServerScript); k++)
						{
                        if( ServerScript[k] == 'c' ||
							ServerScript[k] == 'C' ||
							ServerScript[k] == 'a' ||
							ServerScript[k] == 'A' ||
							ServerScript[k] == 'd' ||
							ServerScript[k] == 'D' )
							{
                            SrvTok[0] = ServerScript[k];
                            break;
							}
						}

                    memset(string1, 0, sizeof(string1));
                    strcpy(string1, buffer1);
                    Set_Null(string1, sizeof(string1));
                    Remove_CrLf(string1, sizeof( string1));
                    number = strtok(string1, "+\0");

                    while(SrvTok[0] != 0)
						{
                        switch(SrvTok[0])
							{
                            case 'C':
                            case 'c':
                                if(param.PageType[j].number[0] == 0 && number != NULL)
									{
                                    strncpy(page[PagePointer].PagerNumber, param.PageType[j].prefix, sizeof(page[0].PagerNumber));
                                    strncat(page[PagePointer].PagerNumber, number, sizeof(page[0].PagerNumber));
                                    number = strtok(NULL, "+\0");
									}
                                else
									{
                                    strncpy(page[PagePointer].PagerNumber,
											param.PageType[j].number,
											sizeof(page[PagePointer].PagerNumber));
									}
                                break;

                            case 'A':
                            case 'a':
                                if(param.PageType[j].autCode[0] == 0 && number != NULL)
									{
                                    strncpy(page[PagePointer].AutCode,
											number,
											sizeof(page[PagePointer].AutCode));
                                    number = strtok(NULL, "+\0");
									}
                                else
									{
                                    strncpy(page[PagePointer].AutCode,
											param.PageType[j].autCode,
											sizeof(page[PagePointer].AutCode));
									}
                                break;

                            case 'D':
                            case 'd':
							case 'T':
								PrintMessage(MSGTYPE_SYSTEM_PROGRESS,
												hwndGeneralMsg,
												"processing D, d,  and T");

                                if(number != NULL)
									{
									if(page[PagePointer].DisplayInfo[0] == 0)
										{
										strncpy(page[PagePointer].DisplayInfo,
												number,
												sizeof(page[PagePointer].DisplayInfo));
										}
									else
										{
										number = strtok(NULL, "+\0");
										}
									}
                                break;

                            default:
                                break;
							}

                        SrvTok[0] = 0;

                        for(k++; k < strlen(ServerScript); k++)
							{
                            if( ServerScript[k] == 'c' ||
								ServerScript[k] == 'C' ||
								ServerScript[k] == 'a' ||
								ServerScript[k] == 'A' ||
								ServerScript[k] == 'd' ||
								ServerScript[k] == 'D' )
								{
                                SrvTok[0] = ServerScript[k];
                                break;
								}
							}
						}
					}
				}
			}
		}

    return retval;
	}

