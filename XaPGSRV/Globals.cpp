
// globals.cpp


#include "common.h"
#include "Impairment.h"
#include "Append2File.h"

#include "ErrorLog.h"

char szAppName[100];			// contains info from string table for example:
								//  "Xtend Page Server"
char szAppVersion[100];			// contains text information extracted from version block
								//  and then formatted appropriately.
								//  "Version 4-10-0030-X"

char szApplicationPathAndFileName[MAX_PATH];	// the drive, path, and filename from __argv[0].
												//  e.g. c:\pagesrv\xapgsrv.exe	

char szApplicationFileName[MAX_PATH];
char szApplicationPathName[MAX_PATH];		

bool Respect_Paging_Status;		//2012-02-20 LCD , Respect_Paging_Status is used for respecting page status for Masco
bool ListMember_Covered;         //2012-03-07 Added by LC, to follow the search

//SOguz - 6/3/2016 - DE20747
CString g_csYNFlagNameToPageAllDevices;	// constains the YNFLAG Field Name so we can use it to check if page all devices turned ON
//SOguz - 6/15/2016 - This will will switch to true when YNFlag for Page all devices is set to Y for the profile
bool isPageAllDevicesOn = false;
// SOguz - 5/4/2017 - DE24153
bool FollowPageAllDevicesCoverage; //TRUE or FALSE flag to decide whether to follow coverage when paging all devices

bool ignoreCoverageWhenPageAllDevices;
bool ignoreStatusWhenPageAllDevices;

CString g_csPageTypesWithSmsFlag;


int g_iImpairmentFormatNumber;

// keep in sync with enums.h
	char
szAppExitReason[][40] =
	{
	"APP_EXIT_USER_REQUEST_SYSTEM",
	"APP_EXIT_USER_REQUEST_MENU",
	"APP_EXIT_INIT_APPLICATION_FAILED",
	"APP_EXIT_INIT_INSTANCE_FAILED",
	"APP_EXIT_BAD_COMMAND_LINE_OPTION",
	"APP_EXIT_ACCESS_EXCEPTION",
	"APP_EXIT_EXCEPTION_INT_DIVIDE_BY_ZERO",
	"APP_EXIT_PRIV_INSTRUCTION_EXCEPTION",
	"APP_EXIT_UNKNOWN_EXCEPTION",
	"APP_EXIT_NEW_EXE_DETECTED",
	"APP_EXIT_UNEXPECTED_EXIT",
	"APP_EXIT_SOFTWARE_EXCEPTION",
	"APP_EXIT_BAD_PARAMETER_FILE",
	"APP_EXIT_EXIT_FOR_RESTART",
	"APP_EXIT_OPERATING_SYSTEM_VERSION_NG",
	"APP_EXIT_CANT_CREATE_LOG_SUBDIRECTORY",
	};
// keep in sync with enums.h


	char *
pszBooleanText[] =
	{
	"FALSE", 
	"TRUE"
	};


	char *
pszDataSourceText[] =
	{
	"DATA_SOURCE_NONE", 
	"DATA_SOURCE_NETWORK", 
	"DATA_SOURCE_LOCAL"
	};


	char *
pszActionReminderCommandText[] =
	{
	"REMINDER_ADD", 
	"REMINDER_UPDATE", 
	"REMINDER_DELETE"
	};


int iAppExitReason = APP_EXIT_UNEXPECTED_EXIT;
char ConfigFileName[] = "XGPRM1";	// extension is not always ".XTD"

BOOL bCmdLineCheckForNewEXE = FALSE;
BOOL bCmdLineUseProgrammerMenu = FALSE;
BOOL bCmdLineUseAlternateMenu = FALSE;
BOOL bCmdLineFailSafeMode = FALSE;
BOOL bCmdLineUseExitDialog = TRUE;
BOOL bCmdLineEnableBeepSound = FALSE;
BOOL bCmdLineEnableDebugLogging = FALSE;
BOOL bCmdLineEnableStandardLogging = TRUE;
BOOL d4xDiags = FALSE;
BOOL bCmdLineMinimizeWindowAtStartup = FALSE;
BOOL bCmdLineEnableExperimentThread = FALSE;
BOOL bCmdLineEnableLogCommportTraffic = FALSE;



BOOL g_bOkToRunApplication = FALSE;		// based on operating system, and version
										//  allow the application to run, or not.

// exception handler test variables
BOOL bActivateIntentionalAccessException = FALSE;
BOOL bActivateIntentionalZeroDivideException = FALSE;

//BOOL bActivateIntentionalStuckInLoop1 = FALSE;
//BOOL bActivateIntentionalStuckInLoop2 = FALSE;

// mutual exclusion handles for controlling
//  access to the comm port data buffers and pointers

HANDLE hMutexComm[MAX_COMMPORTS];
int MessageEnableMask;			// message enable settings will be set at application start.
HINSTANCE hInst;                // Program instance handle

HWND hwndGeneralMsg = NULL;		// Handle to general messages			
HWND hwndD4xMsg	 = NULL;		// Handle for dialogic messages			
HWND hwndComMsg	 = NULL;		// Handle for com port messages	
HWND hwndDatabaseMsg = NULL;		// Handle for database messages
		
HWND hwndD4xStatus = NULL;		// handle to phone line status list
HWND hwndQueStatus = NULL;		// Handle for queue status list
HWND hwndComStatus = NULL;		// Handle for Com status list

HWND hwndDataScopeDlg = NULL;	// Handle for datascope dialog
HWND hwndDataScopeLB = NULL;	// Handle for DataScope Dialog List Box

HWND hStatusWindow = NULL;		// Handle for Status Bar
HWND hwndFrame = NULL;			// Handle to main window


//dummy buffer padding for wild write!!!!
char paddingBuffer[1024 * 16];

PARAM_STRUCT_XG param;
struct Bkp_Rec BkpBuf;
struct PageableDevicesInfo AllPageableDevices[ 20 ]; //SOguz - 6/21/2016 - array of pageable all devices

PAGE_STRUCT page[PAGE_ITEMS_TO_ALLOCATE];	// Structure to store standard page information

IVR_PORT_STRUCT	port[MAX_IVRPORTS];
PageTypes PageType[MAX_RCC];
StatusStruct Status[30];
CatsStruct Cats[4];
ListStruct ListQue;
MultiPageStruct MultiQue;

// the first element of this array of 31 elements
//  is used exclusively (hardcoded) as the list queue.
QueueStruct	Queue[MAX_QUEUE_FILES];

ComPorts ComPort[MAX_COMMPORTS];

int	QPause = 200;   // 500;					// time delay between successive queue checking,
											//  applies to list, and comm port, (others ?).
											//  used by multi-queues.
//int DirStr = TRUE;
//int PageId = TRUE;
//int AlphaMsgFh;								// File handle for preformatted alpha messages

DATA_SOURCE DataSource = DATA_SOURCE_NETWORK;		// unless a command line option is specified,
											//  use the network database.

bool bExternalControl = false;				// flag to indicate whether an external signal,
											//  from a serial, or parallel port
											//  will be used to control the active/standby
											//  state of the page server.
											//  always startup as:
											//  NOT externally controlled.,
											//  because, if, and which signal 
											//  to be sampled must be read from the 
											//  page server configuration file.
											//
											// also, if not externally controlled by any method,
											//  the pageserver will be activated at startup, 
											//  regardless of any control input.

int g_iExternalControlParallelPortInputBit = -1;	// contains which bit to use as the 
													//  parallel port input bit.
int g_iExternalControlCommPortToUse = -1;			// contains which comm port to use 
													//  for external control input.
													//  0 == COM1:

int PsOpMode = MODE_IDLE;

int StatusOn = FALSE;				// Indicates if end user is to be prompted
									// with current page status information
									//  0 - do not prompt status
									//  1 - prompt status

//int SerialAccept = FALSE;			// Indicates info accepted via serial port

int	WeekNumber;						// Week number for billing purposes.
int	MonthNumber;					// Month number for billing purposes.

int PageKbd = 0;				// Manual Paging Flag
                                // 0 - inactive
                                // 1 - page unsing pager ids
                                // 3 - enter display info for pager id

BOOL bCmdLineUseDialogicHardware = TRUE;	// enable or disable the dialogic subsystem.
											//  if disabled, all ivr, including 
											//  misc rcc dialout paging cannot be configured.

int totalD4xChannels;			// Total number of dialogic channels.

int promptfh;                   // file handle for the Prompt File - "XTPG2.VAP"
int Digitsfh;                   // file handle for the Digits File - always "DIGITS.VAP"
int StatusFh;                   // file handle for the Status File - "XTPGSTAT.VAP"

int VoiceFileCounter;           // File number for voice file   6/14/96
int CatsCall = FALSE;

int MidnightState = MIDNIGHT_STATE_IDLE;

int LptStatus = TRUE;           // Error LEDS not on
int DataScopeChar = 0;			// Position of next character in datascpoe
int dataScopeFh = -1;			// File Handle to log datascope data
unsigned long dataScopeMesgByte = 0;	// Bytes written in message file

BOOL PagerId = TRUE;

BOOL dataScopeHex = FALSE;			// True - Datascope display in hex mode
BOOL dataScopeScroll = TRUE;		// True - scroll data on datascope
BOOL bKillThreads = FALSE;			// Terminate all threads when TRUE


SYSTEMTIME LptTime;					// Timer for checking parralel port
SYSTEMTIME MultiQTimer1;			// Multi Que 1 Timer
SYSTEMTIME MultiQTimer2;			// Multi Que 2 Timer
SYSTEMTIME keepAliveTimer;			// Timer for scheduler keep alive
SYSTEMTIME MidnightTimer;			// Timer for midnight routine

SYSTEMTIME QueueDisplayUpdateTime;	// Timer for Queue Status Window Update.

char DateTimeISO8601[21] = "9999-12-31  23:59:59";	// note 2 spaces between date and time.
char xnSystemTime[21];				// System Time: MM/DD/YYYY..HH:MM:SS

char Yesterday[10];
char HourStart[3];
char pageFileDir[100];				//	page file direcorty:	param.AutoDir in network mode
									//							param.BkupDir in failsafe mode

unsigned pstate[MAX_COMMPORTS];			// initialized at startup.
unsigned pageerror[MAX_COMMPORTS];		// initialized at startup.
unsigned portactive[MAX_COMMPORTS];		// initialized at startup.
unsigned pageactive[MAX_COMMPORTS];		// initialized at startup.

char holdbuff[MAX_COMMPORTS][RXBUFFLEN + 1];	// received hold work buffers
char BlockText[MAX_COMMPORTS][RXBUFFLEN + 1];   // Tap block from STX thru ETX or ETB or US.
char ChecksumText[MAX_COMMPORTS][RXBUFFLEN + 1];// 3 Tap checksum characters, and the following CR.
char pcapcode[MAX_COMMPORTS][100];				// received hold work buffers
char pauthcode[MAX_COMMPORTS][15];				// received hold work buffers
char pmessage[MAX_COMMPORTS][255];				// received hold work buffers
char pvoice[MAX_COMMPORTS][5];					// received voice line
char pstatus[MAX_COMMPORTS][80];
char sndDataScope[80];


CString TapMessageText[MAX_COMMPORTS];			// an unlimited place to store the incoming TAP message string.

std::string messageString[MAX_COMMPORTS];


BTRLSTUNION lstBtrvBuf;
BTRIEVEUNION OnCallBtrvBuf;

//char LST_POS_BLK[512];


struct vbase40 prompts[MAXPROMPTS];
struct vbase40 DigitPrompts[MAXDIGITS];
struct vbase40 StatusPrompts[MAXSTATUS];

// static allocation for all channels, outside any function
//  these must remain in-scope at all times.
DX_IOTT indexplay[MAX_IVRPORTS + 1][50];

DLG_CTRLS * dlgctrls;
int d4xBoardCount;
int bddev;				// dialogic board device handle, needs to be global
						//  so that error routine can access it to retrieve
						//  error information.
int activeChdev;

HANDLE hD4xWorkerThread;


// JPH Added for ADO wrapper support
    //hwndGeneralMsg = XnCreateChild( "General Messges" );
    //hwndD4xMsg	   = XnCreateChild( "Phone Line Messges" );
    //hwndComMsg	   = XnCreateChild( "Comm Port Messges" );
    //hwndDatabaseMsg	   = XnCreateChild( "Database Messges" );


//LogBuffer errBuffer(NULL);
//ostream errorLogStream(&errBuffer);

//LogBuffer generalBuffer(NULL);
//ostream generalLogStream(&generalBuffer);
//
//LogBuffer phoneBuffer(NULL);
//ostream phoneLogStream(&phoneBuffer);
//
//LogBuffer commBuffer(NULL);
//ostream commLogStream(&commBuffer);

//SO - 03/27/2013 - Jim H and I changed this to be pointer to have control over memory allocation.
//This fixes crash during exit.
ostream *dbLogStream;
//LogBuffer dataBaseBuffer(NULL);

string globalDbConnectionStr;




DX_CAP ChannelParameter[MAX_RCC];	// rcc script numbers index this array, and are 0 thru 9.


	char *
DigitsText[200] =
	{
	"VOX_DIGITS_zero",
	"VOX_DIGITS_one",					
    "VOX_DIGITS_two",						
    "VOX_DIGITS_three",					
    "VOX_DIGITS_four",						
    "VOX_DIGITS_five",						
    "VOX_DIGITS_six",						
    "VOX_DIGITS_seven",					
    "VOX_DIGITS_eight",				
    "VOX_DIGITS_nine",					
    "VOX_DIGITS_ten",						
    "VOX_DIGITS_eleven",					
    "VOX_DIGITS_twelve",					
    "VOX_DIGITS_thirteen",				
    "VOX_DIGITS_fourteen",					
    "VOX_DIGITS_fifteen",					
    "VOX_DIGITS_sixteen",					
    "VOX_DIGITS_seventeen",				
    "VOX_DIGITS_eightteen",				
    "VOX_DIGITS_nineteen",					
    "VOX_DIGITS_twenty",					
    "VOX_DIGITS_thirty",					
    "VOX_DIGITS_forty",					
    "VOX_DIGITS_fifty",					
    "VOX_DIGITS_sixty",					
    "VOX_DIGITS_seventy",					
    "VOX_DIGITS_eighty",					
    "VOX_DIGITS_ninety",					
    "VOX_DIGITS_hundred",					
    "VOX_DIGITS_thousand",				
    "VOX_DIGITS_million",				
    "VOX_DIGITS_and",					
    "VOX_DIGITS_a_m",						
    "VOX_DIGITS_p_m",					
    "VOX_DIGITS_sunday",				
    "VOX_DIGITS_monday",				
    "VOX_DIGITS_tuesday",					
    "VOX_DIGITS_wednesday",				
    "VOX_DIGITS_thursday",					
    "VOX_DIGITS_friday",				
    "VOX_DIGITS_saturday",				
    "VOX_DIGITS_Sunday",					
    "VOX_DIGITS_january",					
    "VOX_DIGITS_february",				
    "VOX_DIGITS_march",					
    "VOX_DIGITS_april",				
    "VOX_DIGITS_may",					
    "VOX_DIGITS_june",						
    "VOX_DIGITS_july",						
    "VOX_DIGITS_august",				
    "VOX_DIGITS_september",			
    "VOX_DIGITS_october",					
    "VOX_DIGITS_november",					
    "VOX_DIGITS_december",					
    "VOX_DIGITS_dollars",					
    "VOX_DIGITS_cents",					
    "VOX_DIGITS_message_arrived",			
    "VOX_DIGITS_at",						
    "VOX_DIGITS_O",						
    "VOX_DIGITS_thank_you_goodbye",
    "VOX_DIGITS_PROMPT_061",	
	"VOX_DIGITS_PROMPT_062",				
	"VOX_DIGITS_PROMPT_063",				
	"VOX_DIGITS_PROMPT_064",				
	"VOX_DIGITS_PROMPT_065",				
	"VOX_DIGITS_PROMPT_066",				
	"VOX_DIGITS_PROMPT_067",				
	"VOX_DIGITS_PROMPT_068",				
	"VOX_DIGITS_PROMPT_069",				
	"VOX_DIGITS_PROMPT_070",				
	"VOX_DIGITS_PROMPT_071",				
	"VOX_DIGITS_PROMPT_072",				
	"VOX_DIGITS_PROMPT_073",				
	"VOX_DIGITS_PROMPT_074",				
	"VOX_DIGITS_PROMPT_075",				
	"VOX_DIGITS_PROMPT_076",				
	"VOX_DIGITS_PROMPT_077",				
	"VOX_DIGITS_PROMPT_078",				
	"VOX_DIGITS_PROMPT_079",				
	"VOX_DIGITS_PROMPT_080",				
	"VOX_DIGITS_PROMPT_081",				
	"VOX_DIGITS_PROMPT_082",				
	"VOX_DIGITS_PROMPT_083",				
	"VOX_DIGITS_PROMPT_084",				
	"VOX_DIGITS_PROMPT_085",				
	"VOX_DIGITS_PROMPT_086",				
	"VOX_DIGITS_PROMPT_087",				
	"VOX_DIGITS_PROMPT_088",				
	"VOX_DIGITS_PROMPT_089",				
	"VOX_DIGITS_PROMPT_090",				
	"VOX_DIGITS_PROMPT_091",				
	"VOX_DIGITS_PROMPT_092",				
	"VOX_DIGITS_PROMPT_093",				
	"VOX_DIGITS_PROMPT_094",				
	"VOX_DIGITS_PROMPT_095",				
	"VOX_DIGITS_PROMPT_096",				
	"VOX_DIGITS_PROMPT_097",				
	"VOX_DIGITS_PROMPT_098",				
	"VOX_DIGITS_PROMPT_099",
	"VOX_DIGITS_PROMPT_100",
	"VOX_DIGITS_PROMPT_101",
	"VOX_DIGITS_PROMPT_102",
	"VOX_DIGITS_PROMPT_103",
	"VOX_DIGITS_PROMPT_104",
	"VOX_DIGITS_PROMPT_105",
	"VOX_DIGITS_PROMPT_106",
	"VOX_DIGITS_PROMPT_107",
	"VOX_DIGITS_PROMPT_108",
	"VOX_DIGITS_PROMPT_109",
	"VOX_DIGITS_PROMPT_110",
	"VOX_DIGITS_PROMPT_111",
	"VOX_DIGITS_PROMPT_112",
	"VOX_DIGITS_PROMPT_113",
	"VOX_DIGITS_PROMPT_114",
	"VOX_DIGITS_PROMPT_115",
	"VOX_DIGITS_PROMPT_116",
	"VOX_DIGITS_PROMPT_117",
	"VOX_DIGITS_PROMPT_118",
	"VOX_DIGITS_PROMPT_119",
	"VOX_DIGITS_PROMPT_120",
	"VOX_DIGITS_PROMPT_121",
	"VOX_DIGITS_PROMPT_122",
	"VOX_DIGITS_PROMPT_123",
	"VOX_DIGITS_PROMPT_124",
	"VOX_DIGITS_PROMPT_125",
	"VOX_DIGITS_PROMPT_126",
	"VOX_DIGITS_PROMPT_127",
	"VOX_DIGITS_PROMPT_128",
	"VOX_DIGITS_PROMPT_129",
	"VOX_DIGITS_PROMPT_130",
	"VOX_DIGITS_PROMPT_131",
	"VOX_DIGITS_PROMPT_132",
	"VOX_DIGITS_PROMPT_133",
	"VOX_DIGITS_PROMPT_134",
	"VOX_DIGITS_PROMPT_135",				
	"VOX_DIGITS_PROMPT_136",				
	"VOX_DIGITS_PROMPT_137",				
	"VOX_DIGITS_PROMPT_138",				
	"VOX_DIGITS_PROMPT_139",				
	"VOX_DIGITS_PROMPT_140",				
	"VOX_DIGITS_PROMPT_141",				
	"VOX_DIGITS_PROMPT_142",				
	"VOX_DIGITS_PROMPT_143",				
	"VOX_DIGITS_PROMPT_144",				
	"VOX_DIGITS_PROMPT_145",				
	"VOX_DIGITS_PROMPT_146",				
	"VOX_DIGITS_PROMPT_147",				
	"VOX_DIGITS_PROMPT_148",				
	"VOX_DIGITS_PROMPT_149",				
	"VOX_DIGITS_PROMPT_150",				
	"VOX_DIGITS_PROMPT_151",				
	"VOX_DIGITS_PROMPT_152",				
	"VOX_DIGITS_PROMPT_153",				
	"VOX_DIGITS_PROMPT_154",				
	"VOX_DIGITS_PROMPT_155",				
	"VOX_DIGITS_PROMPT_156",				
	"VOX_DIGITS_PROMPT_157",				
	"VOX_DIGITS_PROMPT_158",				
	"VOX_DIGITS_PROMPT_159",				
	"VOX_DIGITS_PROMPT_160",				
	"VOX_DIGITS_PROMPT_161",				
	"VOX_DIGITS_PROMPT_162",				
	"VOX_DIGITS_PROMPT_163",				
	"VOX_DIGITS_PROMPT_164",				
	"VOX_DIGITS_PROMPT_165",				
	"VOX_DIGITS_PROMPT_166",				
	"VOX_DIGITS_PROMPT_167",				
	"VOX_DIGITS_PROMPT_168",				
	"VOX_DIGITS_PROMPT_169",				
	"VOX_DIGITS_PROMPT_170",				
	"VOX_DIGITS_PROMPT_171",				
	"VOX_DIGITS_PROMPT_172",				
	"VOX_DIGITS_PROMPT_173",				
	"VOX_DIGITS_PROMPT_174",				
	"VOX_DIGITS_PROMPT_175",				
	"VOX_DIGITS_PROMPT_176",				
	"VOX_DIGITS_PROMPT_177",				
	"VOX_DIGITS_PROMPT_178",				
	"VOX_DIGITS_PROMPT_179",				
	"VOX_DIGITS_PROMPT_180",				
	"VOX_DIGITS_PROMPT_181",				
	"VOX_DIGITS_PROMPT_182",				
	"VOX_DIGITS_PROMPT_183",				
	"VOX_DIGITS_PROMPT_184",				
	"VOX_DIGITS_PROMPT_185",				
	"VOX_DIGITS_PROMPT_186",				
	"VOX_DIGITS_PROMPT_187",				
	"VOX_DIGITS_PROMPT_188",				
	"VOX_DIGITS_PROMPT_189",				
	"VOX_DIGITS_PROMPT_190",				
	"VOX_DIGITS_PROMPT_191",				
	"VOX_DIGITS_PROMPT_192",				
	"VOX_DIGITS_PROMPT_193",				
	"VOX_DIGITS_PROMPT_194",				
	"VOX_DIGITS_PROMPT_195",				
	"VOX_DIGITS_PROMPT_196",				
	"VOX_DIGITS_PROMPT_197",				
	"VOX_DIGITS_PROMPT_198",				
	"VOX_DIGITS_PROMPT_199",
    "VOX_DIGITS_PROMPT_200"
	};


	char *
PromptText[150] =
	{
    "VOX_PROMPT_zero",					
    "VOX_PROMPT_one",						
    "VOX_PROMPT_two",						
    "VOX_PROMPT_three",					
    "VOX_PROMPT_four",					
    "VOX_PROMPT_five",					
    "VOX_PROMPT_six",						
    "VOX_PROMPT_seven",					
    "VOX_PROMPT_eight",					
    "VOX_PROMPT_nine",					
    "VOX_PROMPT_star",					
    "VOX_PROMPT_pound_sign",				
    "VOX_PROMPT_pager_I_D",				
    "VOX_PROMPT_please_dial_your_",		
    "VOX_PROMPT_thank_you_good_bye",		
    "VOX_PROMPT_voice_paging_unavailable",
    "VOX_PROMPT_followed_by_pound_sign",	
    "VOX_PROMPT_is_invalid_please",		
    "VOX_PROMPT_invalid",				
    "VOX_PROMPT_i_d_number",				
    "VOX_PROMPT_please_enter",			
    "VOX_PROMPT_enter_month_and_day_",	
    "VOX_PROMPT_to_page",					
    "VOX_PROMPT_press_star_to_change",	
    "VOX_PROMPT_or_for_the_operator",		
    "VOX_PROMPT_you_do_not_have_a_security_code",				
    "VOX_PROMPT_or_press_",				
    "VOX_PROMPT_operator_page",			
    "VOX_PROMPT_text_message",			
    "VOX_PROMPT_enter_security_code_followed_by_pound_sign",				
    "VOX_PROMPT_incorrect_security_code",		
    "VOX_PROMPT_unavailable",			
    "VOX_PROMPT_coverage_change",			
    "VOX_PROMPT_4_seconds_of_beep_tone",				
    "VOX_PROMPT_enter_time_followed_by_pound_sign",	
    "VOX_PROMPT_critical_alert_line_",	
    "VOX_PROMPT_speak_now",				
    "VOX_PROMPT_paging",					
    "VOX_PROMPT_has_no_pager_at_this_time",			
    "VOX_PROMPT_press_one_to_play_previous_message",		
    "VOX_PROMPT_press_two_to_repeat_current_message",	
    "VOX_PROMPT_press_three to play_",	
    "VOX_PROMPT_status_is_out_",			
    "VOX_PROMPT_room_number",			
    "VOX_PROMPT_to_remain_on_the_line_",	
    "VOX_PROMPT_current",				
    "VOX_PROMPT_to_leave_a_voice_mail_",	
    "VOX_PROMPT_transferring_to_",			
    "VOX_PROMPT_extension",				
    "VOX_PROMPT_code_aborted",				
    "VOX_PROMPT_code_completed",			
    "VOX_PROMPT_call",						
    "VOX_PROMPT_unable_to_set_schedule",	
    "VOX_PROMPT_press",				
    "VOX_PROMPT_to_change",				
    "VOX_PROMPT_to_page_by_name",				
    "VOX_PROMPT_page_status",			
    "VOX_PROMPT_greeting",					
    "VOX_PROMPT_covered_by",				
    "VOX_PROMPT_is",				
    "VOX_PROMPT_list",						
    "VOX_PROMPT_please_call",				
    "VOX_PROMPT_enter_covering",			
    "VOX_PROMPT_already_covered_by",		
    "VOX_PROMPT_coverage",					
    "VOX_PROMPT_recursive_coverage",		
    "VOX_PROMPT_to_repeat_menu",			
    "VOX_PROMPT_enter_new_security_",	
    "VOX_PROMPT_press",					
    "VOX_PROMPT_operator",					
    "VOX_PROMPT_please_dial_your",			
    "VOX_PROMPT_hello",					
    "VOX_PROMPT_to_try_another_pager_i_d_press",			
    "VOX_PROMPT_press_one_to_retrieve_undelivered_messages",	
    "VOX_PROMPT_press_two_to_retrieve_all_messages",	
    "VOX_PROMPT_no_messages",				
    "VOX_PROMPT_no_more_messages",			
    "VOX_PROMPT_to_retrieve_messages",	
    "VOX_PROMPT_security_code",			
    "VOX_PROMPT_please_wait_retrieving_messages",	
    "VOX_PROMPT_text_messages_available_dial_zero",			
    "VOX_PROMPT_press_pound_sign_to_return_to_main_menu",	
    "VOX_PROMPT_press_pound_sign_quit",	
    "VOX_PROMPT_speak_now_then",			
    "VOX_PROMPT_repeat_menu",				
    "VOX_PROMPT_press_star_to_page_your_party_and_remain_on_the_line",		
    "VOX_PROMPT_please_remain_on_the_",	
    "VOX_PROMPT_sorry_unable_to_connect_",	
    "VOX_PROMPT_sorry_unable_to_",			
    "VOX_PROMPT_enter_one_for_A_M_",		
    "VOX_PROMPT_home",						
    "VOX_PROMPT_back",						
    "VOX_PROMPT_emergency",				
    "VOX_PROMPT_number",					
    "VOX_PROMPT_operating_room",			
    "VOX_PROMPT_stat",						
    "VOX_PROMPT_page",						
    "VOX_PROMPT_press_the_first",			
    "VOX_PROMPT_transferring_to_operator",	
    "VOX_PROMPT_silence_for_half_a_second",				
    "VOX_PROMPT_to_connect_to",		
    "VOX_PROMPT_forwarding_number",	
    "VOX_PROMPT_digits_of_the_persons_last_name_for_Q_or_Z_press_zero",				
    "VOX_PROMPT_PROMPT_104",				
    "VOX_PROMPT_PROMPT_105",				
    "VOX_PROMPT_PROMPT_106",				
    "VOX_PROMPT_PROMPT_107",				
    "VOX_PROMPT_enter_pager_I_D_",		
    "VOX_PROMPT_for_page_by_name_",		
	"VOX_PROMPT_PROMPT_110",				
    "VOX_PROMPT_to_try_another_extension_press",			
	"VOX_PROMPT_PROMPT_please_call_page_operator_for_assistance",				
	"VOX_PROMPT_PROMPT_113",				
	"VOX_PROMPT_PROMPT_114",				
	"VOX_PROMPT_PROMPT_115",				
	"VOX_PROMPT_PROMPT_116",				
	"VOX_PROMPT_PROMPT_117",				
	"VOX_PROMPT_PROMPT_118",				
	"VOX_PROMPT_PROMPT_119",	
	"VOX_PROMPT_PROMPT_120",	
	"VOX_PROMPT_PROMPT_121",	
	"VOX_PROMPT_PROMPT_122",	
	"VOX_PROMPT_PROMPT_123",	
	"VOX_PROMPT_PROMPT_124",	
	"VOX_PROMPT_PROMPT_125",	
	"VOX_PROMPT_PROMPT_126",	
	"VOX_PROMPT_PROMPT_127",	
	"VOX_PROMPT_PROMPT_128",	
	"VOX_PROMPT_PROMPT_129",	
	"VOX_PROMPT_PROMPT_130",	
	"VOX_PROMPT_PROMPT_131",	
	"VOX_PROMPT_PROMPT_132",	
	"VOX_PROMPT_PROMPT_133",	
	"VOX_PROMPT_PROMPT_134",	
	"VOX_PROMPT_PROMPT_135",	
	"VOX_PROMPT_PROMPT_136",	
	"VOX_PROMPT_PROMPT_137",	
	"VOX_PROMPT_PROMPT_138",	
	"VOX_PROMPT_PROMPT_139",	
	"VOX_PROMPT_PROMPT_140",	
	"VOX_PROMPT_PROMPT_141",	
	"VOX_PROMPT_PROMPT_142",	
	"VOX_PROMPT_PROMPT_143",	
	"VOX_PROMPT_PROMPT_144",	
	"VOX_PROMPT_PROMPT_145",	
	"VOX_PROMPT_PROMPT_146",	
	"VOX_PROMPT_PROMPT_147",	
	"VOX_PROMPT_PROMPT_148",	
	"VOX_PROMPT_PROMPT_149",	
    "VOX_PROMPT_PROMPT_150"			
	};


	char *
StatusText[150] =
	{
	"VOX_STATUS_zero",						
	"VOX_STATUS_one",						
	"VOX_STATUS_two",					
	"VOX_STATUS_three",					
	"VOX_STATUS_four",						
	"VOX_STATUS_five",						
	"VOX_STATUS_six",						
	"VOX_STATUS_seven",					
	"VOX_STATUS_eight",					
	"VOX_STATUS_nine",	
	"VOX_STATUS_available_statuses_are",	
	"VOX_STATUS_enter_new_status_number_",
	"VOX_STATUS_current_status_is",		
	"VOX_STATUS_in_hospital_page",			
	"VOX_STATUS_in_hospital_not",			
	"VOX_STATUS_out_of_hospital_on",		
	"VOX_STATUS_out_of_hospital_page",		
	"VOX_STATUS_out_of_hospital_not",		
	"VOX_STATUS_referred_to_other",		
	"VOX_STATUS_referred_to_office",		
	"VOX_STATUS_unavailable_hold",			
	"VOX_STATUS_unavailable_call_page_",	
	"VOX_STATUS_refer_calls_to_office",	
	"VOX_STATUS_refer_calls_to_answering_",
	"VOX_STATUS_available_for_page",		
	"VOX_STATUS_to_repeat_choices",		
	"VOX_STATUS_not_available",			
	"VOX_STATUS_avaialable_wide_area_",	
	"VOX_STATUS_unavailable_messages_",	
	"VOX_STATUS_available_emergency_only",	
	"VOX_STATUS_not_avaialabe_messages_being",				
	"VOX_STATUS_invalid",					
	"VOX_STATUS_unavailable",
	"VOX_STATUS_in_hospital_on_page",				
	"VOX_STATUS_in_surgery_messages_being",		
	"VOX_STATUS_available_on_pager",				
	"VOX_STATUS_unavailable_on_pager",				
	"VOX_STATUS_reachable_at",						
	"VOX_STATUS_calls_being_taken_by",				
	"VOX_STATUS_available_by_overhead_paging",		
	"VOX_STATUS_silence_for_half_a_second",		
	"VOX_STATUS_in_office_on_extension",			
	"VOX_STATUS_in_office_calls_to_voice_mail",	
	"VOX_STATUS_in_office_calls_to_operator",		
	"VOX_STATUS_unavailable_calls_to_voice_mail",	
	"VOX_STATUS_special_all_calls_to_operator",	
	"VOX_STATUS_home_office",						
	"VOX_STATUS_press",							
	"VOX_STATUS_to_change_security_code",			
	"VOX_STATUS_in_O_R_holding_messages",			
	"VOX_STATUS_on_vacation",						
	"VOX_STATUS_referred_to_another_number",		
	"VOX_STATUS_available_at_the_V_A_hospital",	
	"VOX_STATUS_status_change",					
	"VOX_STATUS_press_pound_sign_to_leave",		
	"VOX_STATUS_in clinic_on_page",					
	"VOX_STATUS_refer_to_phone_number",			
	"VOX_STATUS_out_of_hospital_at_home",			
	"VOX_STATUS_in_surgery_not_available",			
	"VOX_STATUS_out_of_hospital_wide_area",		
	"VOX_STATUS_in_hospital_no_pager",				
	"VOX_STATUS_in_meeting_until_further_notice",	
	"VOX_STATUS_unavailable_call_page_operator_",	
	"VOX_STATUS_in_hospital_available_overhead",	
	"VOX_STATUS_in_page_emergency_only",			
	"VOX_STATUS_out_on_page",						
	"VOX_STATUS_out_page_emergency_only",			
	"VOX_STATUS_in_on_page",						
	"VOX_STATUS_available_for_paging_at_U_M_M_S",	
	"VOX_STATUS_available_overhead_in_general_",	
	"VOX_STATUS_available_overhead_in_shock",		
	"VOX_STATUS_available_overhead_in_V_A_",		
	"VOX_STATUS_unavailable_until",				
	"VOX_STATUS_not_on_call",						
	"VOX_STATUS_available_for_paging_at_the_V_A_",	
	"VOX_STATUS_being_covered_by",					
	"VOX_STATUS_available_overhead_in_radiology",	
	"VOX_STATUS_in_at_Weiss",						
	"VOX_STATUS_page_emergency_only__stored",		
	"VOX_STATUS_in_at_Darien_or_Bolingbrook",		
	"VOX_STATUS_out_long_range",					
	"VOX_STATUS_messages_being_stored_not",		
	"VOX_STATUS_page_emergency_only_message_",		
	"VOX_STATUS_not_available_no_messages",		
	"VOX_STATUS_calls_are_being_referred_to",		
	"VOX_STATUS_referred_to",						
	"VOX_STATUS_page_emergency_only",				
	"VOX_STATUS_covered_by",						
	"VOX_STATUS_not_on_hospital_page",				
	"VOX_STATUS_available_at_Fairfax_hospital",	
	"VOX_STATUS_available_at_Fair_Oaks_hospital",	
	"VOX_STATUS_available_at_Mount_Vernon",		
	"VOX_STATUS_in_hospital_on_page_or__",			
	"VOX_STATUS_not_a_hospital_pager",				
	"VOX_STATUS_page_emergency_only_messages__",	
	"VOX_STATUS_PROMPT_096",						
	"VOX_STATUS_PROMPT_097",						
	"VOX_STATUS_PROMPT_098",						
	"VOX_STATUS_PROMPT_099",					
	"VOX_STATUS_ten",								
	"VOX_STATUS_eleven",					
	"VOX_STATUS_twelve",					
	"VOX_STATUS_thirteen",					
	"VOX_STATUS_fourteen",					
	"VOX_STATUS_fifteen",					
	"VOX_STATUS_sixteen",					
	"VOX_STATUS_seventeen",				
	"VOX_STATUS_eighteen",					
	"VOX_STATUS_nineteen",					
	"VOX_STATUS_twenty",					
	"VOX_STATUS_star",				
	"VOX_STATUS_PROMPT_112",				
	"VOX_STATUS_PROMPT_113",				
	"VOX_STATUS_PROMPT_114",				
	"VOX_STATUS_PROMPT_115",				
	"VOX_STATUS_PROMPT_116",				
	"VOX_STATUS_PROMPT_117",				
	"VOX_STATUS_PROMPT_118",				
	"VOX_STATUS_in_at_larabida",	
	"VOX_STATUS_Available_at_Inova_Alexandria_Hospital",	
	"VOX_STATUS_unavailable_on_pager_2",				
	"VOX_STATUS_Available_for_medical_staff_only",	
	"VOX_STATUS_Available_at_Inova_Loudon_Hosp",	
	"VOX_STATUS_PROMPT_124",	
	"VOX_STATUS_PROMPT_125",	
	"VOX_STATUS_PROMPT_126",	
	"VOX_STATUS_PROMPT_127",	
	"VOX_STATUS_PROMPT_128",	
	"VOX_STATUS_PROMPT_129",	
	"VOX_STATUS_PROMPT_130",	
	"VOX_STATUS_PROMPT_131",	
	"VOX_STATUS_PROMPT_132",	
	"VOX_STATUS_PROMPT_133",	
	"VOX_STATUS_PROMPT_134",	
	"VOX_STATUS_PROMPT_135",	
	"VOX_STATUS_PROMPT_136",	
	"VOX_STATUS_PROMPT_137",	
	"VOX_STATUS_PROMPT_138",	
	"VOX_STATUS_PROMPT_139",	
	"VOX_STATUS_PROMPT_140",	
	"VOX_STATUS_PROMPT_141",	
	"VOX_STATUS_PROMPT_142",	
	"VOX_STATUS_PROMPT_143",	
	"VOX_STATUS_PROMPT_144",	
	"VOX_STATUS_PROMPT_145",	
	"VOX_STATUS_PROMPT_146",	
	"VOX_STATUS_PROMPT_147",	
	"VOX_STATUS_PROMPT_148",	
	"VOX_STATUS_PROMPT_149",	
	"VOX_STATUS_PROMPT_150"
	};


	char *
MessageRetrieveDispositionText[11] =
	{
	"MSG_FIND_MSGS_UNAVAILABLE",
	"MSG_FIND_NO_MSGS",
	"MSG_FIND_NO_MORE_MSGS",
	"MSG_FIND_NUMERIC_MSG_FOUND",
	"MSG_FIND_STILL_LOOKING",
	"MSG_FIND_DIAL_0_TEXT_MSG",
	"MSG_FIND_VOICE_MSG",
	"MSG_FIND_TEXT_MSG",
	"MSG_FIND_XN_NUMERIC_MSG",
	"MSG_FIND_STATUS_CHANGE_MSG",
	"MSG_FIND_COVERAGE_CHANGE_MSG"
	};


	char *
CommPortDescriptionText[] =
	{
	"---",
	"MTR",
	"ZTN",
	"16Z",
	"IPT",
	"XOM",
	"TAP",
	"IXO",
	"PAG",
	"NUL",
	"GRP",
	"UVA",
	"IXD",
	"IXX",
	"TA2",
	"TA3",
	"XXX"
	};


	char *
IVRPortDescriptionText[] =
	{
	"---",
	"IN ",
	"OUT",
	"B/O",
	"CAT",
	"MET",
	"VAO",
	"007",
	"008",
	"009",
	"010",
	"011",
	"012",
	"013",
	"014",
	"015",
	"RST",
	"STA",
	"GRP",
	"019",
	"KPG",
	"DPG",	
	"RS2",
	"023",
	"024",
	"025",
	"I26",
	"DST",
	"XXX"
	};


	char *
MidnightStateText[] =
	{
	"MIDNIGHT_STATE_IDLE",

	"MIDNIGHT_STATE_STEP1_INITIALIZE",
	"MIDNIGHT_STATE_STEP1_RUN",
	"MIDNIGHT_STATE_STEP1_CLEANUP",

	"MIDNIGHT_STATE_STEP2_INITIALIZE",
	"MIDNIGHT_STATE_STEP2_RUN",
	"MIDNIGHT_STATE_STEP2_CLEANUP",

	"MIDNIGHT_STATE_STEP3_INITIALIZE",
	"MIDNIGHT_STATE_STEP3_RUN",
	"MIDNIGHT_STATE_STEP3_CLEANUP",

	"MIDNIGHT_STATE_STEP4_INITIALIZE",
	"MIDNIGHT_STATE_STEP4_RUN",
	"MIDNIGHT_STATE_STEP4_CLEANUP",

	"MIDNIGHT_STATE_STEP5_INITIALIZE",
	"MIDNIGHT_STATE_STEP5_RUN",
	"MIDNIGHT_STATE_STEP5_CLEANUP",

	"MIDNIGHT_STATE_FINAL"
	};


BOOL g_bUseAlternateConfigFile = FALSE;
char g_ConfigFileExtension[4];


// logging file objects
class Append2File g_LogAll;
class Append2File g_LogSys;
class Append2File g_LogDbg;
class Append2File g_LogDbs;
class Append2File g_LogCom;
class Append2File g_LogIvr;
class Append2File g_LogStart;

class Impairment g_IMP;


COMMPORT_NAME CommportName[] =
	{
		{"\\\\.\\COM1", "COM1"},
		{"\\\\.\\COM2", "COM2"},
		{"\\\\.\\COM3", "COM3"},
		{"\\\\.\\COM4", "COM4"},
		{"\\\\.\\COM5", "COM5"},
		{"\\\\.\\COM6", "COM6"},
		{"\\\\.\\COM7", "COM7"},
		{"\\\\.\\COM8", "COM8"},
		{"\\\\.\\COM9", "COM9"},
		{"\\\\.\\COM10", "COM10"},
		{"\\\\.\\COM11", "COM11"},
		{"\\\\.\\COM12", "COM12"},
		{"\\\\.\\COM13", "COM13"},
		{"\\\\.\\COM14", "COM14"},
		{"\\\\.\\COM15", "COM15"},
		{"\\\\.\\COM16", "COM16"},
		{"\\\\.\\COM17", "COM17"},
		{"\\\\.\\COM18", "COM18"},
		{"\\\\.\\COM19", "COM19"},
		{"\\\\.\\COM20", "COM20"},
		{"\\\\.\\COM21", "COM21"},
		{"\\\\.\\COM22", "COM22"},
		{"\\\\.\\COM23", "COM23"},
		{"\\\\.\\COM24", "COM24"},
		{"\\\\.\\COM25", "COM25"},
		{"\\\\.\\COM26", "COM26"},
		{"\\\\.\\COM27", "COM27"},
		{"\\\\.\\COM28", "COM28"},
		{"\\\\.\\COM29", "COM29"},
		{"\\\\.\\COM30", "COM30"},
		{"\\\\.\\COM31", "COM31"},
		{"\\\\.\\COM32", "COM32"},
		{"\\\\.\\COM33", "COM33"},
		{"\\\\.\\COM34", "COM34"}
	};

char voiceMessageTypeStr[3];