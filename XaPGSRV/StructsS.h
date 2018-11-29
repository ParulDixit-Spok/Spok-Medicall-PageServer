#pragma once
// structsS.h

// structures that are Shared/Included in both project types
//  pageserver (xn, xo, xa), and configuration utility.

// this file contains data structures common to both 
//  pageserver and pageconfig applications.

#include <string>
#include <dxdigit.h>

typedef short int             BTI_SINT;

//#define LEN_DIGBUF   31          /* Max # of entries for collecting DTMF */
//#define DG_MAXDIGS   LEN_DIGBUF  /* Max Digits Returned by dx_getdig() */
//#define DG_END       -1        /* Terminator for dg_type Array in DV_DIGIT */
///* 
// * DV_DIGIT structure
// */
//typedef struct dv_digit {
//   char dg_value[DG_MAXDIGS +1];
//   char dg_type[DG_MAXDIGS +1];
//} DV_DIGIT;


	typedef struct 
tag_IVR_PORT_STRUCT
	{
    int state;						// next state number to execute 
	int d4xHandle;					// Handle to channnel
	int d4xT1Handle;				// Handle to T1 channel	
    int file_handle;				// record file handle           
    char digsave[MAXDIG+1];			// dtmf digits and term char 
	DV_DIGIT digBuf;				//	Raw Digit Data
    int DataPointer;				// Pointer to Cats Structure or
									// pointer to Pagetype structure
									// for PortType 5
    int PortStatus;					// 0 idle 
									// 1 off hook
									// 2 outgoing page
									// 3 incoming page request
    int LoopCurrent;				// FALSE indicates drop in LC
    int PageNumber;					// Corresponding page on this port
    int ComNumber;					// Corresponding comport
    unsigned int Attempts;
    unsigned int External;
    unsigned int Errors;
    unsigned int Cancels;
	int	t1;							// true indicates t1 card
    SYSTEMTIME timer;				// port timer
	} IVR_PORT_STRUCT;


// Page structure "page" contains information for all incoming and        
// outging pages. This structure is linked to the "Line" structure as 
// well as the "PageType" structure

typedef struct tag_PAGE_STRUCT
	{
    long btrPosition;
	char intefaceType[4];					// Queue Type directed to.
    char PagerNumber[65];					// Number to be paged.
											//  increased from 30 to 65 to support the full size
											//  of the xnfrq database field. that's where this info comes from.
											//  need to support longer email addresses.
											//  change request prompted by customer MASCO. kgill - 2007-01-15.
    char DisplayInfo[255];					// Info to be sent to pager    
	char AutCode[15];                       /* Pager authorization code     */
                                            /* - Used to store covering ext */
    char InitiatedDate[10];                 /* Date page was initiated      */
    char InitiatedTime[8];                  /* Time page was initiated      */
	char InitiatorId[30];                   /* Indentifies page source      */
    char PageStatus[2];                     /* Status of page               */
                                            /* R -  Page Recieved waiting to send */
                                            /* I -  Idle page channel       */
                                            /* B -  Page channel busy      */
                                            /* A -  Page aborted, retry     */
                                            /* C -  Incoming Page completed */
                                            /* X -  Uncompleted, terminated */
                                            /* N -  No pager. Do not page   */
                                            /* M -  No pager assigned       */
                                            /* O -  Voice Page OK to Talk   */
                                            /* K -  Cancel page not enough info*/
                                            /* L -  List page ok 0 queue    */
                                            /* S -  Changed status to covered*/

    char CompletedDate[10];                 /* Date page was completed      */
	char CompletedTime[8];                  /* Time page was completed      */
    char PageType[3];                     /* Type of page 00 - 99         */
    int  ACKMin;                            /* Minutes for ACK              */
    char Extension[11];                   /* Extension to be paged == ProfileId */
    char PagerId[11];                     // Pager id number
    char InitialPagerId[11];              /* Initial PagerId              */
											/*     ... Exception Date		*/
    char InitialExtension[11];            /* Initial Extension            */
											/*     ... Exception Time		*/
    unsigned int NextAckPtr;                /* Pointer to next record to acknowlege */
                                            /* Pointer to next capcode for  */
                                            /*          stacked pages.      */
                                            /* Pointer to next id for page  */
                                            /*          by name.            */
    char Printed[1];						/* If not ack has it been printed */
                                            /* P - indicates password verified */
                                            /*     for status change lines. */
    int  VoiceFile;                         /* File used to record voice    */

    char PageInfo[148];						// PagerId and Message contents read from flat-file page queue record.    
	
    char Password[11];						/* User password or 123         */
    int  ScriptNumber;                      /* Stript selected to play      */
	int  ScriptFlag;                        /* Current Script pointer       */
    unsigned int attempts;                  /* # of times pages attempted   */

    int  PortNumber;                        /* Corresponding port this page */
                                            /* will be sent in or out       */
                                            /*                              */
                                            /* Used as QueuePointer when    */
                                            /* adding page                  */

    int  ComNumber;                         /* Radio connection for page    */
    int  ComOffset;                         /* Radio queue type             */
    int  PriorityQPointer;                  /* Pointer to priority Queue    */
    char FileName[120];
    char Flag1[2];                        /* Y indicates self park & page */
                                            /* feature active for dial in   */
                                            /* Y indicates repeat current   */
                                            /*   message for message read.  */
    char Flag2[2];                        /* Y indicates no remote        */
                                            /*   forwarding restrictions    */
                                            /* Y Messages found on Message  */
                                            /*   retreval                   */
    char Flag3[2];                        /* Y indicates status change    */
                                            /*   restrictions to pageable.  */
    char Flag4[2];                        /* Y indicates priority page    */
                                            /*   password allowed           */
    char Flag5[2];                        // Y indicates transfer to VM   
                                            //   password allowed
    char tempStr[40 + 1];                 //  - Park Transfer String      
                                            //  - Exception time entered    
                                            //  - Name from log file  - 40 characters.     
	char tempStr2[110];						//	- List of Page-by-Name Pager Id's
											//     built up by the NameId() function.
											//	- Raw Frq Data after call to Get_Frq
	int	 tempInt;							//  - message number when retieveing
											//		messages
											//  1 - Proceed to status feature
											//    - Proceed to page feature
    BOOL pageAdded;                         // TRUE indicates page added via dial in
	BOOL pageFormatted;						// TRUE indicates page formatted via dial in 

											// these items aid in the processing of list-page
											//  deciding what information is available
											//  and whether to use the default if queue
											//  information is not available.
    char RawPageInfo[148];                  //  added 2001-10-02 kgill
    char DefaultNumeric[36];                //  note: only 35 chars are allocated in the
    char DefaultAlpha[36];                  //        list definition database file
											//        (xtpglst.btr) for default data items.

	std::string longMessage;
	bool voiceMsg;

	// structure items used in the process of
	//  retrieving messages via dial-in lines.
	//  kgill 2002-09-23.
	int iMessageRetrieveRequest;
	BOOL bMessageLimitsDefined;
	int iMessageSearchDirection;			

	int iNewestMessageNumber;
	int iOldestMessageNumber;
	int iCurrentMessageNumber;
	int iDatabaseAccessLimit;
	int iLastSuccessfulMessageNumber;

	// kgill 2003-06-27
	char CapCodeFromXnFrq[65 + 1];			// hold a copy of the capcode as read from
											//  the database... no prefix, or suffix added.  
											//  table - Xnfrq, field - FrqNumber.
	} PAGE_STRUCT;



struct Cats_Rec_Dest
	{
    int  Line;                              /* Pointer to port structure    */
    int  VoicePrompt;                       /* Voice prompt offset          */
    char PageNumber[ 31 ];                  /* Cap code for Cats page       */
    char PageQue[3];                        /* Dialogic voice line for page */
    int  Group1Port;                        /* Com port group call          */
    int  Group1Button;                      /* Button to press for group call*/
    char Group1Number[ 11 ];                /* Group call number            */
    int  Group2Port;                        /* Com port group call          */
    int  Group2Button;                      /* Button to press for group call*/
    char Group2Number[ 11 ];                /* Group call number            */
	};


struct PageType_Rec_Dest
	{
    char pageid[ 4 ];
    char alias[ 4 ];
    char QDirect[ 4 ];
    char number[ 31 ];
    char prefix[ 10 ];
    char autCode[ 16 ];
    char description[ 40 ];
    int  PortType;
    char Script[ 20 ];
    unsigned char intflg;
    int  stdely;
    int  lowerfrq;
    int  upperfrq;
    unsigned char timefrq;
    unsigned char rejctfrq;
    unsigned char intfltr;
    unsigned int  retry;
    int  wait1;
    int  wait2;
    int  wait3;
    unsigned char nbrbeg;
	};


	struct 
ComPort_Rec_Dest
	{
    char PortActive[2];
    int  PageActive;				// Type
    bool  toggleDTR;
    unsigned int ComBaud;
    unsigned int ComSetup;
    char CallSignExtension[10];		// Extension with callsign info          
    long CallSignInterval;          // Frequency of queing callsign          
    unsigned int Retry;
    int  VoiceLine;
    char Prefix[20][20];
    //char Suffix[20][10];		
    char Suffix[20][55];			// length of .Suffix changed from 10 to 55. 2010-01-22 kgill.
									//  this eventually gets put into .PagerNumber which has size 65.
    char number[20][31];
    char Alias[20][4];
    char QDirect[20][4];
    char PageId[20][4];				// bad name... actually "Device Type" e.g. %57
    int  ControlPort[20];
    char QueType[20][3];			// Byte 1: P-Page T-Talk S-Status V-Voice
                                    //         B-Beep                       
                                    // Byte 2: 1-Alphanumeric 0-numeric     
    char description[20][40];		// Description of queue type
	char Init1[12];					// Modem reset string.
	char Init2[41];					// Modem itialization string.
    char SharedPsId[4];				// Id of Page Server to share port with 
    int  SharedPort;                // shared port                          
    int  SharedMode;                // shared mode                          
                                    //   ACTIVE: Starts active, cooperative 
                                    //   BACKUP: Start inactive, cooperative
                                    //   MASTER: Start active, no sharing   
                                    //   SLAVE:  Start inactive, activate   
                                    //           when told                  

	char UserDefinedNotes[60];				// appears in datascope window
	char UserDefinedShortDesignation[13];	// appears in status window
	};


	struct 
IVRPort_Rec_Dest
	{
	int DataPointer;
	int LoopCurrent;
	int iType;								// integer representation        1     2      3
	char UserDefinedShortDesignation[13];	// appears in status window
											// description large enough to hold "800-555-1212"
	short int speedLevel;
	short int volumeLevel;
	};


// Page Format Structure 

	struct 
PFormat_Rec_Dest
	{
    int Position;
    char Id[2];
    char VoiceFuncs[11];
    char MsgFuncs[11];
    char ToneFuncs[11];
	};


// Parallel Port Bit Assignments 

	struct 
ParallelStruct_Dest
	{
    unsigned long Address;
    char Bit[8][4];
    unsigned int QueueLevel;
    unsigned int ResetLevel;
	};


// Structure used to map out the buttons of a phone set
	struct 
PhoneStruct_Dest
	{
    int Button[16][16];
    int Release[16];
	};


// Two-Tone Format Structure 

struct TwoToneStruct_Dest
	{
    char Id[3];
    char Array[10][3];
	};


// Global Tone Detection Parameters

	struct 
GlobalToneDectStruct_Dest
	{
    int tid;
    int freq1;
    int fq1dev;
    int freq2;
    int fq2dev;
    int ontime;
    int ontdev;
    int offtime;
    int offtdev;
    int repcnt;
	};


	enum 
DestinationType 
	{
	// Critical Printing (List-Printing) destinations.
	PRINTER,
	TEXT_FILE,
	HTML_FILE,

	// Cancelled page destinations.
	MESSAGE_BASE,
	SNPP_MESSAGE,
	};


	typedef struct 
tag_PG_XG_HEADER
	{   

	unsigned int FileChecksum32;	// checksum of the whole parameter file
									//  excluding the first 4 bytes, this checksum.
									//  CRC-32 checksum.

	char FileSignature[8 + 1];		// recognizable text pattern (8 hex digits).
	char FileVersion[9 + 1];		// parameter file version (9 char string).
									//  this is of the format of the application
									//  version e.g. "4-00-0045" the Configurator
									//  will refuse to run if this number doesn't match
									//  what is built into the pageserver executable.

	char DateWritten[10 + 1];			// in the format 2003-01-04
	char TimeWritten[8 + 1];			// in the format 23:59:59
	unsigned int ConfigSequenceNumber;	// sequence number 0 - 4 billion.
	} PG_XG_HEADER;


	// this is the output of the pagserver configuration application,
	//  and input to the PageServer.  This structure is included
	//  as a part of both projects. 
	//  
	// ok to add, remove, and change position of items
	//  in this structure.
	//	
	typedef struct 
tag_PARAM_STRUCT_XG
	{
	PG_XG_HEADER Header;

	// first actual configuration item follows:
	char CustomerInfoFullName[60 + 1];
	char CustomerInfoShortName[20 + 1];
	char CustomerInfoCity[40 + 1];
	char CustomerInfoState[20 + 1];			
	
    BOOL PromptId;				/* Idicate whether pager id is prompted    */
                                /* the thankyou goodbye message             */
    BOOL RetryId;             /* Indicates if retries are allwed on       */
                                /* invalid  extensions.                     */
    BOOL CustPrompt;          /* Y - users are allowed to record prompts  */
    BOOL CoverPage;           /* Y - extensions allowed to be covered     */
    BOOL PressAfter;          /* Yes indicates "press #" is prompted after*/
                                /* prompt instead of before                 */
    BOOL PageOpTransfer;      /* Allwo transfer to operator after page.   */
    BOOL IgnoreStatus;        /* Ignore page status on page line.         */
    BOOL GreetingOnly;        /* Only Play custom greeting                */
    BOOL NPark;               /* Yes: Use npark self park option          */
                                /* No:  Use switch self park option         */
	BOOL bEnableDatabaseStoredProcedures;		// this is now redundant. 2009-12-01
    BOOL MessageRtrOpt;       /* Yes indicates user is prompted to        */
                                /* next, previous, and current messages     */
    BOOL DeliverPage;         /* Mark direct dial pages as delivered?     */
    BOOL ClearWhere;          /* Y clear whereabouts field on stat change */
    BOOL PageTicks;           /* Y keep track of pages added to q files   */
    BOOL TimeOutTransfer;     /* Yes: transfer to operator if no tones    */
    BOOL timeoutRetry;        /* True = retry if no id number entered     */
    BOOL ConfirmChange;       /* Touchtone confirmation for status change */
    BOOL PromptCurrent;       // if set to 'Y', then speak 'Current status is'
    BOOL StPromptStat;        /* Yes: Prompt status on status menu        */
    BOOL Status10s;           /* TRUE: Page status in 10-19 valid         */
    BOOL Status20s;           /* TRUE: Page status in 20-29 valid         */
    BOOL VoiceConv;           /* Characters converted to voice prompts pgs*/
    BOOL TonePage;            /* YES indicates tone pages are allowed     */
    BOOL VoiceDelete;         /* Yes - indicates delte voice file after page */
    BOOL ExitOnhk;            /* Yes: hang up all lines on exit           */
                                /* No:  Take lines offhook on exit          */
    BOOL DetectCurrent;       /* Yes: Detect Loop current                 */
                                /* No:  Do not detect Loop current          */
    BOOL QueueDelete;         // Yes: delete que files instead of recycle
    BOOL CodePrint;           /* YES indicates codes are to printed       */
    //BOOL MiscTroubleAndCancelAlarm;   // YES: Alarm reminder Misc Troubles, including: cancelled page - replaced.
    BOOL CommportFailureAlarm;	// YES: Alarm reminder - comport failure
    BOOL QueueWarningAlarm;		// YES: Alarm reminder - queue warning
    BOOL CancelledPageAlarm;	// YES: Alarm reminder - cancelled page 

    BOOL CatsConfirm;         /* Touchtone confirmation requered for CATS */

    BOOL MultiCover;          /* TRUE indicates multiple coverage allowed */
                              /*      from status change menu             */
    BOOL ListStatus;          /*  TRUE check page status for list         */
    BOOL voiceSquelch;        /*  TRUE indicates to squelch voice pages   */
    BOOL voiceAGC;            /*  TRUE = automatic gain control for voice pages */
    BOOL voicePagerDtmfPass;  /*  TRUE DTMF's check for password for voice */
                                /*      pages. Must enter TT before record  */
	BOOL waveFileFormat;	  //  TRUE = Record voice pages, and greetings 
							  //		in WAVE format
	BOOL autoFormat;			// TRUE -	Activates auto formatting of numeric pages
								//			via dialogic lines
	BOOL autoFormatStarStop;	// TRUE -	Star inhibits autoformatting
	BOOL autoFormatACSpace;		// TRUE -	Area code characters formatted with space
								//			instead of dash
	BOOL backupStatus;			// TRUE -	backup mode uses page status
	BOOL statusOn;				// TRUE -	use page status
	BOOL exitAlarm;				// Set Action Reminder Alarm On Exit
	BOOL keepAliveAlarm;		// Use Action Reminder Keep Alive Alarm
	BOOL promptNamePass;		// prompt page by name password
	BOOL renameAlarm;			// TRUE -	Alarm after 100 rename attempts
	BOOL queueAccessError;		// Queue Access Error.

	char autoFormatStar[2];		// Character to replace star.

    unsigned char TouchToneTimeOut;		// Timeout for recieving touchtones   
    unsigned char MaxSilence;			// Maximum silence for recording of voice   
    int VoicePromptTime;		// Time allowed for recording voice prompt  

    short int  PromptType;		/* Indicates type ov voice prompt to use    */
								/* 19 - extension                           */
								/* 12 - id number                           */
    short int  MinMessage;       /* Minimum callback characters accpeted     */
    short int  XnBtrvCvrOffset;  /* Offset into btieve file for covering ext */
    short int  XnBtrvStatusOffset;/* Offset into btr file for status          */
    short int  XnBtrvWhereOffset; /* Offset into btr file for whereabouts     */
    short int  XnBtrvWhereLen;  /* whereabouts len for btrieve file         */
    short int  XnPagerIdOffset; /* Btrieve Pager Id Offset                  */
    BTI_SINT  XnBtrvNameIdKey;  /* Btrieve Pager Name key number            */
    short int  nameMax;         /* Max page by name entries                 */
    short int  NameIdLength;    /* Length of page by name id                */
    short int  XnNameIdOffset;  /* Btrieve Name Id Offset                   */
    short int  IdPromptLength;  /* Amount if id digits to be prompted       */
    short int  IdLength;
	short int  numIntroSilence;	// number of 1/2 silence periods bore intro
    short int  BkupIdLength;    /* length of pager id in backup file        */
    short int  MessageRtrFlg;   /* Numris indicating position of delivered  */
                                /* message flag. Null if not to mark        */
                                /* retrieved messages as delivered.         */
	short int  pageLimitMax;	// Max number of characters for page type indicated 
								//   by pageLimitTypes
	short int  pageLimitMax2;	// Max number of characters for page type indicated 
								//   by pageLimitTypes2
    BTI_SINT XnBtrvPgIdKey;     /* Btrieve Pager Id key number              */
    short int  statusDefault;   /* Default status if blank                  */
    short int  StatusNgPrompt;  /* Non zero indactes addition prompt is play */
                                /* for unpagable status's                   */
    short int  StatusFdPrompt;  /* Non zero indactes addition prompt is play */
                                /* for FWD status's                         */
    short int  TalkPageRpts;    /* Number of times to repeat talk type pages*/
	short int  voicePageTime;	//	Seconds for voice page		
    short int  StarPrompt;      /* Prompt to use instead of * for talk page */
    short int  MaxQueSz;
    short int  MultiQueNumber;  /* Points to Multi Que in Queue Structure */
    short int  MultiQueNumber2; /* Points to Multi Que in Queue Structure */
    short int  Statistics;      /* Hourly Statistics Routine Flag           */
                                /* 0 - not in statistics routine            */
                                /* 1 - in statistics routine                */
                                /* 2 - do not activate statistics routine   */
    short int FlagParkOffset;			/* Non zero indicates to check log file for */
										/* class of service for self park and page. */
    short int FlagForwardOffset;		/* Non zero indicates check log file for    */
										/* unrestriced remote call forwarding.      */
    short int FlagStatusOffset;		/* Non zero indicates check log file for    */
										/* restriced status changes.                */
    short int FlagTtPriorityOffset;	/* Non zero indicates check log file for    */
										/* priority page via touchtone password     */
	short int FlagVmOffset;			//	Non zero indicates check flag for transfer to VM
										//		Feature	
    short int PgTypsNum;				// number of RCC page types, 
										//  from pageserver config utility Misc tab.
    short int PriorityPointer[MAX_QUEUE_FILES][10];	// there are up to 10 priorities listed in nopaging,
										//  for each of 31 queues.
	short int	altMenuPrompt[ 30 ];	//	Alternate status menu prompts

	unsigned short VoiceFileFormat;		// Number of bits and sample rate
	unsigned short t1CmdOffHook;		// Command to take t1 line off hook
	unsigned short t1CmdOnHook;			// Command to put t1 line on hook
	unsigned short t1CmdWink;			// Command to send t1 wink
	unsigned short t1EvtRing;			// T1 ring event mask
	unsigned short t1EvtOnHook;			// T1 on hook event mask
	unsigned short t1EvtWink;			// T1 wink event
	int	t1WinkLen;						// T1 Wink len in 10 ms Units
	int dataEngine;						// Btrieve, ADO
	long t1OffHookPause;					// T1 pause after going off hook

    char AnotherPage[2];      /* Character for additional pages after 1st */
    char OpTransfer[16];      /* Operator transfer string                 */
    char AlphaMsg[16];        /* Prefix for alpha messages                 */
    char GreetingExt[13];
    char GreetingExt2[13];
	char greetingExtPark[11];	//	Greeting Extension for self park line
    char PromptFile[ 13 ];      /* Voice prompt file name                   */
    char ParkTransfer1[ 11 ];   /* 1st string dialed, park-meetme transfer  */
    char ParkTransfer2[ 11 ];   /* 3rd string dialed, park-meetme transfer  */
    char StatusFile[ 13 ];      /* Voice status prompt file name            */
    char StatPress[ 12 ][ 4 ];  /* Status menu key function array           */
    char OnCallFile[ 13 ];      /* On call btrieve file                     */
    char XnBtrvFile[ 13 ];
    char FrqBtrvFile[ 13 ];
	char schdBtrvFile[ 40 ];		// Btrv file for scheduler
	char pagersBtrvFile[ 13 ];		// Paging database Btrv file
	char msgBtrvFile[ 13 ];			// Messages Btrv file
	char msgLineBtrvFile[ 13 ];		// Messages Line Btrv file
	char msgCountBtrvFile[ 13 ];	// Message count Btrv file
	char msgDeliveredBtrvFile[ 13 ];// Delivered messages btrv file
    char StatPoundOpt[ 4 ];     /* Pound sign option on status options menu */
    char CForwardQueue[ 4 ];    /* Queue identifier for call forward info   */
    char CForwardVMNum[ 11 ];   /* Global call forward to voice mail #      */
    char CForwardOperNum[ 11 ]; /* Global call forward to operator #        */
    char CForwardDigits[ 11 ];  /* Valid call forward leading digits        */
    char NotisDir[ 46 ];
    char LogDir[ 46 ];
    char AutoDir[ 46 ];
    char pageLimitTypes[ 46 ];	// Page types to lime to pageLimitMax
	char pageLimitTypes2[ 46 ];	// Page types to lime to pageLimitMax2
    char ActionReminderProfileId[11];	// profile id for SetActionReminder() to use.
								
    char BkupDir[ 46 ];
    char TwoToneId[ 2 ];
    char TwoToneQues[ 21 ];
    char MidFiles[46];        // Other ques to rename at midnight ???- misnamed?
    char MenuPass[ 10 ];        /* If non-spaces status menu allowed on     */
                                /* page line.                               */
    char ParkPass[ 10 ];        /* If non-spaces indicates park and page    */
                                /* allowed on page line                     */
    char VoicePass[ 10 ];       /* If non-spaces indicates voice paging is  */
                                /* allowed on an incoming dialogic line     */
    char ListPass[ 10 ];        /* If non-spaces indicates list paging is   */
                                /* allowed on an incoming dialogic line     */
    char ListPassN[ 10 ];       /* Numeric list password                    */
    char VPromptPass[ 10 ];     /* Password to modify custom voice prompts. */
    char ReenterPass[ 10 ];     /* Password to re-enter callback number     */
    char PNamePass[ 10 ];       /* Password for page by name                */
    char PriorityPass[ 10 ];    /* Password indicates a priority page       */
	char didPagePass[ 11 ];		//	Password to indicate Page Line for DiD
	char didStatPass[ 11 ];		//  Password to indicate status line for did
	char vmPassword[ 11 ];		//	Password for transfer to voice mail
    char VoiceQues[ 41 ];
    char VoiceQues2[ 41 ];
    char CodeQues[ 41 ];
    char CodeVoiceQues[ 41 ];
    char MultiPageQues[ 41 ];
    char MultiPageQues2[ 41 ];
    char CrtclPrint[ 2 ];				// Indicates if critical error messeges are 
										// sent to a local printer.                
										// N - NO                                   
										// C - Cancelled pages after max attempts    
										// A - Aborted pages check data base       
										// B - both cancelled and aborted            
    char SupperExt[13];
    char UserMsg[ 7 ];
    char MidExtension[11];				// Extension to queue at midnight           
    char MidExtInfo[ 16 ];				// Extension info to queue at midnight 
	char stationId[4];					// Identifies Page server station for queue control
	char ConnectStringNetwork[120];		// ADO connection string for normal network operation.
	char ConnectStringFailsafe[120];	// ADO connection string for failsafe operation.
	char vmTransfer1[ 20 ];				// First part of transfer to voicemail string
	char vmTransfer2[ 20 ];				// End of transfer to voicemail string
	char vmFrqType[ 4 ];				// Frq information conatining middle of transfer 
										//  to voice mail string
	char vmPageMsg[ 20 ];				// Data to be displayed on Voice mail page

	struct IVRPort_Rec_Dest IVRPort[MAX_IVRPORTS];
	struct ComPort_Rec_Dest ComPort[MAX_COMMPORTS];
	struct PageType_Rec_Dest PageType[MAX_RCC];
    struct Cats_Rec_Dest Cats[ 4 ];
    struct PFormat_Rec_Dest PFormat[ 15 ];
    struct TwoToneStruct_Dest TwoTone[ 25 ];
    struct ParallelStruct_Dest Lpt;
    struct PhoneStruct_Dest Phone;
    struct GlobalToneDectStruct_Dest GTD;	

	BOOL FlushDigitBuffer;			// flush digit buffer, this will disable or
									//  enable over-dial between states, usually
									//  associated with covered-by pagers. 
									//  see Project Worksheet - Sequence 0049, and 0043

	BOOL bEnableListQueueProcessing;	
	
	BOOL UseImpairmentAnnouncements;
	int ImpairmentCheckInterval;
	char ImpairmentProfileIdPattern[10 + 1];
	char ImpairmentStatusToEnable[1 + 1];

	//char ActionIdForCommFailAndQWarningAndCancel[2 + 1];	// this one to be deprecated

	char ActionIdForCommportFailure[2 + 1];	// replacement for above.
	char ActionIdForQueueWarning[2 + 1];	// new one (unused/unimplemented)
	char ActionIdForCancelledPage[2 + 1];	// new one

	char ActionIdForKeepAlive[2 + 1];
	char ActionIdForApplicationExit[2 + 1];
	char ActionIdForQueueRenameTrouble[2 + 1];

	// added for Ohio State University - 2005-05-17.
	//
	char WriteTapInMessagesToMessageBase[MAX_COMMPORTS][6];

	// added for Ressurection Hospital - 2005-11-09
	//
	BOOL ExitForRestartEnable;				// 
	int ExitForRestartHour;					// 
	int ExitForRestartMinute;				// 

	//  for Boston Medical Center (BMC) - 2006-04-21
	//
	char ActionIdForQueueAccessError[2 + 1];		// 

	// 2009-12-21 kgill.
	// Added for Mobile Enablement - SmsFlag for each page type on each comm port. 
	//  This can only be used if it is an IN type of port. (%r4 is the keyword in NoPaging)
	//  
	//  When PageServer is instructed to add a page to the queue, there are now 2 ways
	//   that may be appropriate. 
	//   
	//   1 - if the page type is listed on one of the comm ports as an IN port
	//       and the SmsFlag is set, this indicates that a page request should
	//       be added to the DeliverySchedule table as an SMS page requeust.
	//
	//   2 - the regular old way, writing to the flat file NoPgXX.que.
	//
	//  The SmsFlag is set into the configuration XgPrm1.xtd and PgPrm1.xtd by the PageCfg.exe 
	//   when it reads NoPaging. PageCfg.exe does a "special" lookup on all page types listed 
	//   on all comm IN ports looking for the %r4 flag. If found, the SmsFlag is set to true.
	//
	//
	//BOOL SmsFlag[MAX_COMMPORTS][MAX_COMMPORT_QUEUE_ITEMS];

	} PARAM_STRUCT_XG;



