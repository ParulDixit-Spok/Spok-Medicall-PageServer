
// disable deprecation

// QueueBufferClass.cpp


#include "common.h"				// needed for PrintMessage().

//#include "QueueBufferClass.h"

#pragma warning(disable:4996)

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


using namespace std;

QueueBufferClass::QueueBufferClass()
	{
	InitializeMemberVars();
	}


QueueBufferClass::~QueueBufferClass()
	{	
	}


	void
QueueBufferClass::InitializeMemberVars(void)
	{
	m_csPageType = "";			    
	m_csPageStatus = "";                        
	m_csInitiatedDate = "";        
	m_csInitiatedTime = "";	
	m_csProfileId = "";				// also known as Extension             
	m_csPagerId = "";              
	m_csInitiatorProfileId = "";	
	m_sACKMin = 1;					// pageserver always writes 1, other values may be read.	
	m_usNextAckPtr = 0;				// pageserver always writes 0, other values may be read.   	
	m_csPrinted = 'N';	
	m_usVoiceFile = 0;		
	m_csPageInfo = "";				   
	}


	char *
QueueBufferClass::GetBufferPointer(void)
	{
	// transfer data from member variables to buffer structure, 
	//  and then return a pointer to the buffer data.
	//
	return TransferToQueueBuffer();
	}


int QueueBufferClass::SetBufferPointer(QUEUE_BUF_STRUCT& bPtr, string msgStr, bool continuation)
	{
	//code copied from procedure below
	//you can never be too sure
	memset(static_cast<void*>(&bPtr), ' ', sizeof(QUEUE_BUF_STRUCT));

	m_sACKMin = 1;			// pageserver always writes  1 , other values may be read.	
	m_usNextAckPtr = 0;		// pageserver always writes  0 , other values may be read.   	
	m_csPrinted = 'N';		// pageserver always writes 'N', other values may be read.

	// trim left and right...
	m_csPageType.TrimLeft();
	m_csPageType.TrimRight();

	m_csPageStatus.TrimLeft();
	m_csPageStatus.TrimRight();

	m_csInitiatedDate.TrimLeft();
	m_csInitiatedDate.TrimRight();

	m_csInitiatedTime.TrimLeft();
	m_csInitiatedTime.TrimRight();

	m_csProfileId.TrimLeft();
	m_csProfileId.TrimRight();

	m_csPagerId.TrimLeft();
	m_csPagerId.TrimRight();

	m_csInitiatorProfileId.TrimLeft();
	m_csInitiatorProfileId.TrimRight();

	m_csPageInfo.TrimLeft();
	m_csPageInfo.TrimRight();

	sprintf(bPtr.PageType, "%-2.2s", m_csPageType);
	sprintf(bPtr.PageStatus, "%-2.2s", m_csPageStatus);
	sprintf(bPtr.InitiatedDate, "%-10.10s", m_csInitiatedDate);
	sprintf(bPtr.InitiatedTime, "%-8.8s", m_csInitiatedTime);
	sprintf(bPtr.Extension, "%-8.8s", m_csProfileId);
	sprintf(bPtr.PagerId, "%-7.7s", m_csPagerId);
	sprintf(bPtr.InitiatorId, "%-10.10s", m_csInitiatorProfileId);

	if(continuation)
		bPtr.dataContinuation = '#';
	else
		bPtr.dataContinuation = 0;

	bPtr.ACKMin = m_sACKMin;	
	bPtr.NextAckPtr = m_usNextAckPtr;
	sprintf(bPtr.Printed, "%1.1s", m_csPrinted);
	bPtr.VoiceFile = m_usVoiceFile;	

	sprintf(bPtr.PageInfo, "%-148.148s", msgStr.c_str());

	return msgStr.length();
	}

	
	char *
QueueBufferClass::TransferToQueueBuffer(void)
	{
	static QUEUE_BUF_STRUCT qbsBuffer;

	// note that some items of the structure have no corresponding 
	//  member variables, these items are never used.
	//
	// first, initialize the structure to all <spaces>
	//  then, fill in numeric - never used" items
	//  finally, transfer data from class member variables.
	//
	// some items are never used.
	//  initialize to "expected" values, some external application may
	//  need to see the "expected"value.
	//
	// set to all spaces.
	//
	memset(&qbsBuffer, 0x20, sizeof(qbsBuffer));
	//memset(&qbsBuffer, 0x0, sizeof(qbsBuffer));		// changed 2006-02-16

	// note that items like CompletedDate, and CompletedTime, xxxx, and yyyy
	//  are always written as spaces and have no corresponding member 
	//  variables, or set / get accessors because the data read from the 
	//  disk is never used within the pageserver application.

	// initialize the "always written the same" items to the expected values.
	//
	m_sACKMin = 1;			// pageserver always writes  1 , other values may be read.	
	m_usNextAckPtr = 0;		// pageserver always writes  0 , other values may be read.   	
	m_csPrinted = 'N';		// pageserver always writes 'N', other values may be read.	

	// trim left and right...

	m_csPageType.TrimLeft();
	m_csPageType.TrimRight();

	m_csPageStatus.TrimLeft();
	m_csPageStatus.TrimRight();

	m_csInitiatedDate.TrimLeft();
	m_csInitiatedDate.TrimRight();

	m_csInitiatedTime.TrimLeft();
	m_csInitiatedTime.TrimRight();

	m_csProfileId.TrimLeft();
	m_csProfileId.TrimRight();

	m_csPagerId.TrimLeft();
	m_csPagerId.TrimRight();

	m_csInitiatorProfileId.TrimLeft();
	m_csInitiatorProfileId.TrimRight();

	m_csPageInfo.TrimLeft();
	m_csPageInfo.TrimRight();

	// fill structure items from member variables.
	//
	sprintf(qbsBuffer.PageType, "%-2.2s", m_csPageType);
	sprintf(qbsBuffer.PageStatus, "%-2.2s", m_csPageStatus);
	sprintf(qbsBuffer.InitiatedDate, "%-10.10s", m_csInitiatedDate);
	sprintf(qbsBuffer.InitiatedTime, "%-8.8s", m_csInitiatedTime);
	sprintf(qbsBuffer.Extension, "%-8.8s", m_csProfileId);
	sprintf(qbsBuffer.PagerId, "%-7.7s", m_csPagerId);
	sprintf(qbsBuffer.InitiatorId, "%-10.10s", m_csInitiatorProfileId);

	qbsBuffer.dataContinuation = 0;

	// This code will not handle continuations properly
	//
	// if(m_dataContinuation.IsEmpty())
	// 	    qbsBuffer.dataContinuation = 0;
	// else
	//  	qbsBuffer.dataContinuation = m_dataContinuation[0];

	qbsBuffer.ACKMin = m_sACKMin;	
	qbsBuffer.NextAckPtr = m_usNextAckPtr;
	sprintf(qbsBuffer.Printed, "%1.1s", m_csPrinted);
	qbsBuffer.VoiceFile = m_usVoiceFile;	
	sprintf(qbsBuffer.PageInfo, "%-148.148s", m_csPageInfo);
	
	return (char *) &qbsBuffer;
	}


	CString
QueueBufferClass::FillCString(char * Buffer, int iLength)
	{
	CString csDest;

	// there is no expectation that the Buffer 
	//  is nul terminated, but we will stop filling the CString 
	//  at the first encountered nul char.

	// step thru each character of the given string, 
	//  building up a CString.
	//
	for(int i = 0; i < iLength; i++)
		{
		if(Buffer[i] == 0)
			{
			// nul character encountered, we're all done.
			break;
			}

		// put the non-nul character into the CString;
		csDest.Insert(i, Buffer[i]);
		}

	// the CString has been built.
	return csDest;
	}


	// translate any characters 0x7f and above
	//  to the '*' (asterisk) character.
	//
	void
QueueBufferClass::AllowOnlyASCII(char * Text, int iLength)
	{
	// any characters 0x7f and above will be translated
	//  to a "*" character. these characters cannot be sent to
	//  the Zetron 15, Zetron 16, or other third party destinations.
	//

 	// don't allow nonsense strings to be processed
	if(iLength <= 0)
		{
		return;
		}

	for(int x = 0; x < iLength; x++)
		{
		// take the value of the character as a number between 0 and 255.
		//  the bitwise AND is required because we don't want sign extension 
		//  when the character is translated to a number.
		//  all chars would be negative numbers.
		//
		if((Text[x] & 0xff) >= 0x7f)
			{
			// replace each undesirable character.
			//
			Text[x] = '*';
			}
		}
	}


	void
QueueBufferClass::TransferToMemberVars(char * Buf, int MesagePartsExpected)
	{
	const int bufferSize = 148;

	QUEUE_BUF_STRUCT * pQBS = (QUEUE_BUF_STRUCT *) Buf;

	// filter out any unacceptable data from the PageInfo field.
	//  this is to eliminate any characters that are 127, and higher.
	//  (this routine should probably be somewhere else,
	//   but is here for convenience.)
	//
	AllowOnlyASCII(pQBS->PageInfo, sizeof(pQBS->PageInfo));

	// fill member variables from structure items.	
	//
	m_csPageType           = FillCString(pQBS->PageType,      sizeof(pQBS->PageType));
	m_csPageStatus         = FillCString(pQBS->PageInfo,      sizeof(pQBS->PageStatus));
	m_csInitiatedDate      = FillCString(pQBS->InitiatedDate, sizeof(pQBS->InitiatedDate));
	m_csInitiatedTime      = FillCString(pQBS->InitiatedTime, sizeof(pQBS->InitiatedTime));
    // unused QQQQQ - formerly CompletedDate  
    // unused RRRRR - formerly CompletedTime
	m_csProfileId          = FillCString(pQBS->Extension,     sizeof(pQBS->Extension));
	m_csPagerId            = FillCString(pQBS->PagerId,       sizeof(pQBS->PagerId));
	m_csInitiatorProfileId = FillCString(pQBS->InitiatorId,   sizeof(pQBS->InitiatorId));
	// unused yyyy
	m_dataContinuation	   = FillCString(&pQBS->dataContinuation,   sizeof(pQBS->dataContinuation));
	m_sACKMin              = pQBS->ACKMin;
	m_usNextAckPtr         = pQBS->NextAckPtr;
	m_csPrinted            = FillCString(pQBS->Printed,       sizeof(pQBS->Printed));
	// unused xxx
	m_usVoiceFile          = pQBS->VoiceFile;

	m_csPageInfo = FillCString(pQBS->PageInfo, bufferSize);

	// copy everything after last '+' to message buffer;
	const char* bPtr;
	int dialNumSize;
	int maxSizeMsg;
	//int plusCount = 1;
	int findPlusCount = 0;
	//int i;

	int plusCount = MesagePartsExpected - 1;

	//for(i = 0, bPtr = static_cast<LPCTSTR>(m_csPageInfo); i < 120; ++i, ++bPtr)
	//	{
	//	if(*bPtr == '+') 
	//		{
	//		findPlusCount++;
	//		}
	//	}

	//if(findPlusCount > 1) 
	//	{
	//	plusCount = 2;
	//	}

	// copy at most 120 chars of (DialNumber and PagerId), it just shouldn't be that long.
	//
	for(dialNumSize = 0, bPtr = static_cast<LPCTSTR>(m_csPageInfo); *bPtr && plusCount; ++bPtr, ++dialNumSize)
		{
		if(*bPtr == '+')
			{
			--plusCount;
			}
		}	

 	maxSizeMsg = bufferSize - dialNumSize;

	const char* msgStart = bPtr;

	for(; *bPtr && maxSizeMsg; --maxSizeMsg, ++bPtr);

	int len = bPtr - msgStart;	// pointer math

	if(IsFirstWrite())
		{
		// the first time thru, assign to the message payload, subsequently append to it.
		m_wholeMessage = string(msgStart, len);
		Mark();
		}
	else
		{
		// append to the message payload.
		m_wholeMessage.append(msgStart, len);
		}

	if(IsLastDataField())
		{
		m_csPageInfo.TrimRight();		// trim spaces from the right. 

		// right trim
		m_wholeMessage.erase(m_wholeMessage.find_last_not_of(" \r\n\t") + 1 ) ;
		}
	}


	// "SET" functions:

	void
QueueBufferClass::SetPageType(CString value)
	{
	m_csPageType = value;
	}


	void
QueueBufferClass::SetPageStatus(CString value)
	{
	m_csPageStatus = value;
	}


	void
QueueBufferClass::SetInitiatedDate(CString value)
	{
	m_csInitiatedDate = value;
	}


	void
QueueBufferClass::SetInitiatedTime(CString value)
	{
	m_csInitiatedTime = value;
	}


	void
QueueBufferClass::SetProfileId(CString value)
	{
	m_csProfileId = value;
	}


	void
QueueBufferClass::SetPagerId(CString value)
	{
	m_csPagerId = value;
	}


	void
QueueBufferClass::SetInitiatorProfileId(CString value)
	{
	m_csInitiatorProfileId = value;
	}


	void
QueueBufferClass::SetVoiceFileNumber(int value)
	{
	m_usVoiceFile = (unsigned short) value;
	}


	void
QueueBufferClass::SetPageInfo(CString value)
	{
	m_csPageInfo = value;
	}


	// "GET" functions:

bool QueueBufferClass::IsLastDataField() const
	{
	return !(m_dataContinuation == "#");
	}


	CString
QueueBufferClass::GetPageType(void)
	{
	return m_csPageType;
	}


	CString 
QueueBufferClass::GetPageStatus(void)
	{
	return m_csPageStatus;
	}


	CString 
QueueBufferClass::GetInitiatedDate(void)
	{
	return m_csInitiatedDate;
	}


	CString 
QueueBufferClass::GetInitiatedTime(void)
	{
	return m_csInitiatedTime;
	}


	CString
QueueBufferClass::GetProfileId(void)
	{
	return m_csProfileId;
	}


	CString 
QueueBufferClass::GetPagerId(void)
	{
	return m_csPagerId;
	}


	CString 
QueueBufferClass::GetInitiatorProfleId(void)
	{
	return m_csInitiatorProfileId;
	}

	
	short 
QueueBufferClass::GetACKMin(void)
	{
	return m_sACKMin;
	}


	unsigned short 
QueueBufferClass::GetNextAckPtr(void)
	{
	return m_usNextAckPtr;
	}


	CString
QueueBufferClass::GetPrinted(void)
	{
	return m_csPrinted;
	}


	unsigned short 
QueueBufferClass::GetVoiceFile(void)
	{
	return m_usVoiceFile;
	}

		
	CString
QueueBufferClass::GetPageInfo(void)
	{
	return m_csPageInfo;
	}

