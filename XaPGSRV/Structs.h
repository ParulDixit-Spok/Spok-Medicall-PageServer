
// structs.h
#pragma once

#include "QueueFileManager.h"

// this struct used with STL maps, and vectors
//  for ListPage, and OnCall.
typedef struct
	{
	// all of the contents of this structure are not used at this time.
	//  the plan is to use this "tag-along" information
	//  to produce a good PageList or OnCall report / record.
	char ProfileId[10];
	char PageType[3];			// '??' == unknown.
	char Message[120];
	int X;
	int Y;
	int Z;
	} LIST_DATA; 

//class LIST_DATA
//	{
//
//	public:
//	// all of the contents of this structure are not used at this time.
//	//  the plan is to use this "tag-along" information
//	//  to produce a good PageList or OnCall report / record.
//	char ProfileId[10 + 1];
//	char PageType[3 + 1];			// '??' == unknown.
//	char Message[120 + 1];
//	int X;
//	int Y;
//	int Z;
//	}; 


#if _XN_NEWDATAFORMAT

// On Call Btrieve Data Structure - Console database
typedef struct 
	{
    char OCExtension[10];
    char Date[8];
    char Time[5];
    char Extension[10];
	char memberComment[32];
	char op[10];
	char opTime[12];
	short int titleIndex;
    char depPager;					// 'Y' or 'N'
	} OnCallBtrv_Rec;

#else

// On Call Btrieve Data Structure - Notis database
typedef struct 
	{
    char OCExtension[10];
    char Date[8];
    char Time[5];
    char Name[40];
    char Extension[10];
    char Filler[79];
    char depPager;
    char Filler2[47];
	} OnCallBtrv_Rec;

#endif


// channel device dialog box control
typedef struct
	{
	int ctrlId;
	int def;
	char str[25];
	char t1Str[25];
	} DLG_CTRLS;


typedef struct 
	{
	char RawQueue[148];				// raw list page queue data
	char FinalNumeric[148];			// output Numeric message
	char FinalAlpha[148];			// output Alpha message
	char DefaultNumeric[148];		// default Numeric message
	char DefaultAlpha[148];			// default Alpha message
	} LPTX;


typedef struct
	{
    int QueuePointer;               // Actual queue storing info
    int InQueuePointer;             // Page type identifier 
    unsigned long Attempts;         // Number of page attempts,
                                    // # of covering doctors 
    SYSTEMTIME QTimer;				// Timer for checking que 
	} PageTypes;


// one of these structures for each of XX comm ports.
typedef struct 
	{
    int  QueuePointer[20];          // Actual queue string info
    int  InQueuePointer[20];        // Page Type identifier
    int  state;
    int  xxxStackState;				// no longer used 2010-04-20 kgill.
    int  PortBit;

    int  alarm;
    int  greenLight;
	int	 messageOpt;
    unsigned int Attempts;
    unsigned int External;
    unsigned int Errors;
    unsigned int Cancels;
    unsigned int reset;
    unsigned int rxStart;           // (removed/abandoned) Last displayed recieve buffer
	unsigned int nextRead;			// Next character read from buffer
	unsigned int nextWrite;			// Next character to write into buffer
    char oldCapcode[30];
	char errorResponse[81];			// Error response in TAP protocol
	char buffer[1025];
    SYSTEMTIME timer;
    SYSTEMTIME QTimer;
    SYSTEMTIME CSTimer;
    SYSTEMTIME ICTimer;
	HANDLE hComm;
	HANDLE hReadThread;
	int BlockSendAttempts;			// number of times to send/resend the page request
									//  datablock before giving up.  This starts at 3,
									//  and when it reaches 0, after the final try
									//  the error becomes a "hard" error.
	} ComPorts;


typedef struct 
	{
    int  Group1On;
    int  Group2On;
    int  GroupCallWaiting;          // 1 indicates ready for group call
    char FileName[80];
    int  fh;
    int  fh2;
    SYSTEMTIME timer;				// cats timer                  
    SYSTEMTIME StartTime;           // Start of cat call            
	} CatsStruct;


// Paging backup data structure
struct Bkp_Rec
	{
    char PagerId[11];
    char PageType[4];
    char PageNumber[20];
    char Extension[10];
    char dummy[19];							// 2002-02-07 dummy is to make the record a total of 
											//  64 chars so that records are easy to view
											// with list.com utility.
	};


// SOguz - 6/20/2016 - This struct will be used for holding all pageable devices of a profile
struct PageableDevicesInfo
{
	char FrqType[3];
	char FrqNumber[65];
}; 


// Whereabout status structure 
typedef struct 
	{
    char Text[31];							// e.g. "in hospital on page"
    short Prompt;							// dialogic prompt number in voice file xxx.vap
    char Page[4];						
    char PageTypes[31];					
	} StatusStruct;


// data structure which defines the vbase40/index play format 
struct vbase40
	{
	unsigned long ofst;
	unsigned long size;
	};


// List Page Structure 
typedef struct 
	{
    int  Active;
    int  OnCallActive;
    int  xxx_Stacked;
    int  xxx_StackFh;
    int  xxx_Status;
	int	 voiceQueued;
	int	 pageQueued;
	int  retry;
    long OnCallPosition;
	long listPosition;
    unsigned int LastRead;
    unsigned int LastWrite;
    SYSTEMTIME QTimer;
    int  xxx_txtFh;						// now unused 2005-01-04.
    FILE *xxx_txtFp;					// now unused 2005-01-04.
    char OnCallKey[24];
    char PageNumber[80];
    //char PageAlpha[120];
    char PageAlpha[148];                //SO - 4/15/2013 - Increase the size from 120 to 148 to accomodate output Alpha message in LPTX
    char PageVoice[120];
    char Extension[11];
    char ListExtension[11];
    char xxx_StackInfo[148];			// now unused 2010-04-20 kgill.
    char xxx_StackType[3];				// now unused 2010-04-20 kgill.
    char InitiatorId[10];
	} ListStruct;


// Multiple Page Structure 
typedef struct 
	{
    int  Active;
    int  QTypePointer;
	int  retry;
    char PageNumber[80];
    char PageAlpha[120];
    char PageVoice[13];
    char InitiatorId[10];
	} MultiPageStruct;


// Queue Control Structure
//  information block for pageserver queue files 
//  e.g. NOPG57.QUE.  there are 31 of these queue structures.
//  items are: 0 thru 30.
//
typedef struct 
	{
    int alarm;
    unsigned int Attempts;
    unsigned int External;
    unsigned int Errors;
    unsigned int Cancels;	
    char Id[4];					// item read from "queue control file".
    char StationId[4];			// item read from "queue control file".
 
	// reference to queue file manager object.
	QueueFileManager * pQFM;

	// and QueueBufferClass
	QueueBufferClass * pQBC;
	} QueueStruct;


//// Statistics Data Structure
typedef struct
	{
    char Date[6];
    char Time[4];
    char PSId[3];
    char Type[3];
    unsigned short bucket[8];
	} StatisticStruct;


// Lst btrieve data structures
typedef struct 
	{
    char extension[10];
    char index[3];
    char memberExtension[10];
    char memberId[10];
    char data[35];
    char DumData[65];
	} LstBtrv_Rec;


// compound types.
typedef union tag_BTRLSTUNION
	{
	LstBtrv_Rec lstInfo;
    long RecordPosition;
	} BTRLSTUNION;


typedef union tag_BTRIEVEUNION
	{
    OnCallBtrv_Rec OnCallInfo;
    long RecordPosition;
	} BTRIEVEUNION;



typedef struct tag_COMMPORT_NAME
	{
	char * OSName;
	char * FamiliarName;
	} COMMPORT_NAME;
