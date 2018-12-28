
// globals.h
#pragma once

// temporarily reduce to compiler warning level 3
#pragma warning(push, 3)

// standard template library STL stuff.
#include <vector>
#include <map>
#include <string>
#include <list>
#include <atlstr.h>
#include "DataBaseAccessor.h"
#include "SpoolCollection.h"
#include "ErrorLog.h"


// restore to previous compiler warning level
#pragma warning(pop) 


using namespace std;

extern BOOL g_bOkToRunApplication;

// boolean flags as a result of command line options.
extern BOOL bCmdLineCheckForNewEXE;
extern BOOL bCmdLineUseProgrammerMenu;
extern BOOL bCmdLineUseAlternateMenu;
extern BOOL bCmdLineFailSafeMode;
extern BOOL bCmdLineUseExitDialog;
extern BOOL bCmdLineEnableBeepSound;
extern BOOL bCmdLineEnableDebugLogging;
extern BOOL bCmdLineEnableStandardLogging;
extern BOOL bCmdLineUseDialogicHardware;
extern BOOL bCmdLineMinimizeWindowAtStartup;
extern BOOL bCmdLineEnableExperimentThread;
extern BOOL bCmdLineEnableLogCommportTraffic;

extern HINSTANCE hInst; 
extern PAGE_STRUCT page[PAGE_ITEMS_TO_ALLOCATE];
extern HWND hwndFrame;
extern HWND hwndGeneralMsg;
extern HWND hwndD4xMsg;
extern HWND hwndComMsg;
extern HWND hwndDatabaseMsg;
extern HWND hwndD4xStatus;
extern HWND hwndQueStatus;
extern HWND hwndComStatus;
extern char szAppName[100];	
extern char szAppVersion[];
extern char szApplicationPathAndFileName[];
extern char szApplicationPathName[];
extern char szApplicationFileName[];
extern char szAppExitReason[][40];
extern int iAppExitReason;
extern char ConfigFileName[];
extern BOOL d4xDiags;
void SetExitCode(int ExitCode);

// exception handler test variables
extern BOOL bActivateIntentionalAccessException;
extern BOOL bActivateIntentionalZeroDivideException;

// extern BOOL bActivateIntentionalStuckInLoop1;
// extern BOOL bActivateIntentionalStuckInLoop2;

extern BOOL CauseException;
extern int ThreadIDToCrash;
extern HANDLE hMutexComm[];
extern int totalD4xChannels;
extern PARAM_STRUCT_XG param;
extern char DialogicStateName[][30];

// test data structure
extern LPTX ListPageTextA[];
extern int bddev;

// 2012-02-22 LD added this parameter

extern bool Respect_Paging_Status;	
//2012-03-07 added by LD
extern bool ListMember_Covered;	

extern CString g_csYNFlagNameToPageAllDevices;	// SOguz - 6/13/2016 - contains the YNFLAG Field Name so we can use it to check if page all devices turned ON
extern bool FollowPageAllDevicesCoverage;	//Soguz - 5/4/2017 - TRUE or FALSE flag to decide whether to follow coverage when paging all devices

extern CString g_csPageTypesWithSmsFlag;
extern int g_iImpairmentFormatNumber;

// functions
extern char * GetFunctionName(void);
extern void DumpHexBytes(char *, int);
extern void ShowListData(void);
extern void ShowConfigData(int);
extern BOOL ProcessCommandLineOptions(void);
extern void ShowHelpScreen(void);
extern void Set_Null(char *, int);
extern void SelectListPageText(LPTX *ListPageText);
extern void BeepSequence(int);
extern BOOL bKillThreads;
extern void CheckForNewEXE(void);
extern void CheckForAppRestart(int, int);
extern unsigned __stdcall CheckForNewExeThread(void *);
extern unsigned __stdcall ExperimentThread(void *);
extern unsigned __stdcall CheckForAppRestartThread(void *);
extern unsigned long UpdateCRC(unsigned long, char *, int);
extern void XnSaveWindowPosition(HWND, char *);
extern void XnRestoreWindowPosition(HWND, const char *);
extern void PrintMessage(int, HWND, char *, ...);
extern int MessageEnableMask;
extern int XnGetSetUp(void);
extern int CreateMsgFile(unsigned long *, const char *, const char *);
extern void pageit(int);
extern void MSGTimeStamp(char *, char *);
extern void XnD4xInitialize(void);
extern void XnInitialize(void);
extern BOOL Wait(SYSTEMTIME *, long); 
extern void CheckPortBits(void);
//extern void SetResetBit(void);
extern int WriteLog(const char *, char *);
extern void XnOpenDb(void); 
extern void PageThePageSupervisor(int);
extern void XnCloseDb(void);
extern void D4xWorkerThread(void);
extern void scanaport(void);
extern void CheckCommState(void);
extern void SerialIn(void);
extern void CheckD4xMessages(int, char *);
extern void RCC_Check(int);
extern void Check_D4xHookState(int);
extern void Check_D4x_Add_Que(int);
extern void Check_RCC_Queues(int);
extern void CheckMultiQue(int, SYSTEMTIME *, char *);
extern void CheckCallSign(void);
extern void CheckCatsCall(void);
extern void CheckKeepAlive(void);
extern void CheckQueueErrorMonitor(void);
extern int CheckIfAllPageFlagIsOn(int); //SOguz - 6/15/2016 - Checks if YNFlag for profile is set to Y
extern int GetAllPagingDevicesFromProfile(int); //Soguz - 6/17/2016 - Gets list of all pageable devices belong to profile
extern bool isPageAllDevicesOn; //SOguz - 6/15/2016 - Check if page all devices is on

// Ability to ignore Profile Coverage when Page All Devices flag for a Profile is ON
extern bool ignoreCoverageWhenPageAllDevices;

// Ability to ignore Profile "Not Available" and "Forward to another pager" Statuses when Page All Devices flag for a Profile is ON
extern bool ignoreStatusWhenPageAllDevices;

extern void HourMidnight(void);
extern void XnD4xClose(void);
extern void CloseMiscFlatFiles(void);
extern void Log_Message(int, unsigned long *, char *);
extern void SetActionReminder(char *, char *, int, int, int, const char *);
extern int XnSetHook(int, int);
extern int GetPagerId(char *, int, int);
extern int LogPager(char *, int);
extern int Get_Frq(int, char *, char *, int index = -1); // 6/27/2016 - DE20747 - SOguz - Added extra optional parameter
extern int GetPageType(int, char *, char*, int);
extern void Change_Log2( int);
extern int CoverLog(char *, int);
extern int CoverDoubleCheck(int);
extern void AddSetUp(int, char *);
extern void SelectListPageText(LPTX *);
extern int GetFirstOnCall(long *, char *, char *);
extern int GetNextOnCall(long *, char *);
extern void XnResetDb(short int, int, char *);
extern int ListGetFirst( long *, char * );
extern int ListGetNext(long *, int, char *);
extern void AddSetUp(int, char *);
extern int GetQuePointer(int);
extern int CheckQueue(int);
extern bool ClaimQueueItem(int, int);
extern void	GetQueue(int, int, int, bool, char*, int);
extern bool AddQueue(int, int);
extern bool AddQueueEx(int, int);
extern int WriteUserLog(int, int, int);
extern int WriteUserLogEx(int, int);

extern int GetNumberOfTitleLinesInPhoneLineStatusWindow(void);
extern int GetNumberOfTitleLinesInQueueStatusWindow(void);
extern int GetNumberOfTitleLinesInCommPortStatusWindow(void);
extern void UpdateQueueStatusWindow(int);
extern void InitializeQueueStatusWindow(int);
extern void InitializePhoneLineStatusWindow(int);
extern void InitializeCommPortStatusWindow(int);
extern int RenameFile(int);
extern void ResetBuffer(int);
extern BOOL Wait(SYSTEMTIME *, long);
extern void Set_Null(char *, int);
extern void ListPagePrinting(int, char *, ...);
extern int WriteLog(const char *, char *);
extern void Remove_CrLf(char *, int);
extern HWND XnCreateStatus(LPSTR , int, int, int, int);
extern void SetComportState(int);
extern unsigned __stdcall ReadComport(void *);
extern void QueryService(void);
extern void UpdatePhoneLineStatusWindow(int, LPSTR, ...);
extern void SetNextD4xState(int, DX_CST *);
extern int PlayFil(int, char *, int *, int);
extern int vb40_play(int, int, struct vbase40 *, int, char *);
extern int vb41_play(int, int, struct vbase40 *, int, char *);
extern int process_event(int, int, int, DX_CST *);
extern int next_state(int, int);
extern unsigned long TimeDiff(unsigned long, unsigned long);
extern int WriteMessage(int, char *, char *, char *, char *);
extern void MarkLogPageMsg(int, char *);
extern void CopyLogPageMsg(int, char *, char *);
extern int Parse_Frq(int, char *, char *, char *, char *);
extern int CheckFrqOptions(int, char *, char *, char *);
extern void CheckCodePriorityListQueue(void);
extern void CheckStandardPriorityListQueue(void);
extern void XnCopy(char *, char *, int); 
extern bool FileService(int);
extern int CheckScript(int);
extern void SetQueueError(int, int, int);
extern void cpcinit(int);
extern void WriteSuppLog_Cancelled(int, int, char *);
extern void CancelPrint(int);
extern void CancelledPageProcessing(int);
extern void ProcessIvrQueue(int, int);
extern void GroupCatCall(int);
extern int OpenComport(int);
extern int CloseComPort(int);
extern void UpdateCommPortStatusWindow(int);
extern void Motorola(void);
// extern void Zetron(void);
extern void Zetron16(void);
extern void Zetron2K(void);
extern void Tap(void);
extern void Tap2(void);
extern void Tap3(void);
extern void TapArchUVA(void);
extern void IxoModem(void);
extern void PronetIpt(void);
extern void Ixo_Accept_Direct(void);
extern void GroupCall(void);
extern void SetDTR(int, int);
extern void zpagechk(int, unsigned, unsigned, unsigned, int, char *);
extern void WritePort(int, int, const char *);
extern BOOL getbuff(int, char *);
extern void CheckBuffer(int, unsigned, int, char *);
extern void Remove_CtrlChars(char *, int);
extern void BlockBeginPageCheck(int, unsigned, unsigned, unsigned);
extern void BlockEndPageCheck(int, unsigned, unsigned, unsigned);
extern void pagechk(int, unsigned, unsigned, unsigned, char *);
extern void IPTPageChk(int, unsigned);
extern BOOL WriteComport(int, const char *);
extern void DoState(int, unsigned, char *);
extern void WriteAlarmLog_CommportFailure(int, int);
extern void AbortComPage(int);
extern void ProcessCommportQueue(int, int);
extern void ProcessStack(int);
extern void TwoToneConvert(int);
extern int IdentifyComType(int, char *);
extern void CheckCatsState(void);
extern int T1EventHdlr(int, int, DX_CST *);
extern int Exit_State(int, int, int chdev, DX_CST *);
extern int curr_state(int, int, int chdev, DX_CST *);
extern void GetDigitBuffer(int);
extern int SchedulePage(int, int);
extern int CheckMenu(void);
extern void AddTime(char *);
extern int SetSchedule(int);
extern void WriteStatLog(int, int);
extern int NameId(char *, int);
extern int record(int, int, int);
extern int get_digits(int, short);
extern int ConvertVoiceTime2(char *, char *);
extern int XnTransfer(int, char *, DX_CAP *);
extern void Change_Log3(int);
extern void XnAutoFormat(int);
extern int Voice_Record(int);
extern void AddTimeSeconds(SYSTEMTIME &, int);
extern int GetLogPageMsg(int, char *);
extern void SetStatusText(int, char *);
extern void NoPaging(void);
extern int GetQuePos(char *);
extern int CreateQueue(int);
extern int vbinit(char *, struct vbase40 vbtable[], int);
extern int Check_QControlFile(int);
extern void UpdateFrqBtrv(int);
extern int IdentifyDlType(int pType, char *);
extern HWND hwndFrame;          // main window handle                     
extern HWND hwndMDIClient;      // handle of MDI Client window            
extern HWND hwndActive;         // handle of current active MDI child     
extern HWND hwndActiveEdit;     // handle of edit control in active child 
extern CHAR szChild[];          // class of child                         
extern CHAR szStatus[];			// class of child status
extern CHAR szSearch[];         // search string                          
extern CHAR *szDriver;          // name of printer driver                 
extern CHAR szPropertyName[];   // filename property for dialog box       
extern INT iPrinter;            // level of printing capability           
extern BOOL fCase;              // searches case sensitive                
extern WORD cFonts;             // number of fonts enumerated 
extern int XnSetMenuText(int, const LPTSTR);
extern void InitializeMiscWindow(void);
extern unsigned __stdcall MainPageThread(void *);
extern VOID InitializeMenu(HANDLE);
extern BOOL CALLBACK DlgLineProc(HWND, UINT, WPARAM, LPARAM);
extern BOOL CALLBACK QueueStatusProc(HWND, UINT, WPARAM, LPARAM);
extern BOOL CALLBACK DataScopeDlgProc(HWND, UINT, WPARAM, LPARAM);
extern void DataScopeShowState(int, HWND);
extern BOOL CALLBACK PageDlgProc(HWND, UINT, WPARAM, LPARAM);
extern DLG_CTRLS * dlgctrls;
extern int d4xBoardCount;
extern int bddev;							
extern int activeChdev;
extern HANDLE hD4xWorkerThread;
extern DX_CAP ChannelParameter[];	
extern void DlgLineInfo(HWND);
extern BOOL CALLBACK TngenDlgProc(HWND, UINT, WPARAM, LPARAM);
extern HWND hwndGeneralMsg;
extern HWND hwndD4xMsg;
extern HWND hwndComMsg;
extern HWND hwndDatabaseMsg;
extern HWND hwndD4xStatus;
extern HWND hwndQueStatus;
extern HWND hwndComStatus;
extern HWND hwndDataScopeDlg;
extern HWND hwndDataScopeLB;
extern HWND hStatusWindow;
extern HINSTANCE hInst;
extern HWND hwndFrame;
extern struct Bkp_Rec BkpBuf;
extern struct PageableDevicesInfo AllPageableDevices[ 20 ]; //SOguz - 6/21/2016 - array of pageable all devices
//JPH 12/18/12 -- already define extern above
//extern PAGE_STRUCT page[PAGE_ITEMS_TO_ALLOCATE];
extern IVR_PORT_STRUCT port[MAX_IVRPORTS];
extern PageTypes PageType[MAX_RCC];
extern StatusStruct Status[30];
extern CatsStruct Cats[4];
extern ListStruct ListQue;
extern MultiPageStruct MultiQue;
extern QueueStruct Queue[MAX_QUEUE_FILES];
extern ComPorts ComPort[MAX_COMMPORTS];
extern int QPause;
extern DATA_SOURCE DataSource;
extern bool  bExternalControl;
extern int g_iExternalControlCommPortToUse;
extern int g_iExternalControlParallelPortInputBit;
extern int PsOpMode;
extern int StatusOn;
//extern int  SerialAccept;
extern int	WeekNumber;          
extern int	MonthNumber;         
extern int  PageKbd;
extern int	promptfh;                   
extern int	Digitsfh;                   
extern int	StatusFh;                
extern int	VoiceFileCounter;
extern int  CatsCall;
extern int  Abort;
extern int  WatchAlive;
extern int	MidnightState;
extern int LptStatus;
extern int iSelectedItem;
extern int DataScopeChar;
extern int dataScopeFh;
extern unsigned long dataScopeMesgByte;
extern BOOL PagerId;
extern BOOL dataScopeHex;
extern BOOL dataScopeScroll;
extern BOOL bKillThreads;
extern SYSTEMTIME LptTime;
extern SYSTEMTIME MultiQTimer1;
extern SYSTEMTIME MultiQTimer2;
extern SYSTEMTIME keepAliveTimer;
extern SYSTEMTIME MidnightTimer;
extern SYSTEMTIME QueueDisplayUpdateTime;
extern char DateTimeISO8601[21];
extern char xnSystemTime[21];
extern char Yesterday[10];
extern char HourStart[3];
extern char pageFileDir[100];
extern unsigned pstate[MAX_COMMPORTS];
extern unsigned pageerror[MAX_COMMPORTS];
extern unsigned portactive[MAX_COMMPORTS];
extern unsigned pageactive[MAX_COMMPORTS];
extern char holdbuff[MAX_COMMPORTS][RXBUFFLEN + 1];   
extern char BlockText[MAX_COMMPORTS][RXBUFFLEN + 1];   
extern char ChecksumText[MAX_COMMPORTS][RXBUFFLEN + 1];   
extern char pcapcode[MAX_COMMPORTS][100];                 
extern char pauthcode[MAX_COMMPORTS][15];               
extern char pmessage[MAX_COMMPORTS][255]; 
extern CString TapMessageText[MAX_COMMPORTS];
extern std::string messageString[MAX_COMMPORTS];              
extern char pvoice[MAX_COMMPORTS ][5];                      
extern char pstatus[MAX_COMMPORTS][80];
//extern char	LST_POS_BLK[512];
extern struct vbase40 prompts[MAXPROMPTS];
extern struct vbase40 DigitPrompts[MAXDIGITS];
extern struct vbase40 StatusPrompts[MAXSTATUS];
extern DX_IOTT indexplay[MAX_IVRPORTS + 1][50];
extern int MessageEnableMask;
extern BTRLSTUNION lstBtrvBuf;
extern BTRIEVEUNION OnCallBtrvBuf;
extern DataBaseAccessor * pDBA;
extern SpoolCollection * pscListPage;
//extern char * AAssign(char *, string);

// unused code removed JPH 06/12/12
//extern void SQLSequence3(void);
//extern void SQLSequence4(void);
//extern void SQLSequence5(void);

extern char * DigitsText[];
extern char * StatusText[];
extern char * PromptText[];
extern char * MessageRetrieveDispositionText[]; 
extern char * CommPortDescriptionText[];
extern char * IVRPortDescriptionText[];
extern char * MidnightStateText[];
extern void TestSendAlphaPage1(void);
extern void TestSendAlphaPage2(void);
extern void TestSendScheduledAlphaPage(void);
extern void TestSendScheduledVoicePage(void);
extern void TestSendScheduledOverheadPage(void);
extern void TestSendOverheadPage(void);
extern void TestSendNumericPage(void);
extern void TestSendVoicePage1(void);
extern void TestSendVoicePage2(void);
extern void TestSendOnCallPage(void);
extern void TestSend3AlphaPages(void);
extern void TestSendBadPage(void);
extern void TestSendTalkPage1(void);
extern void TestSendTalkPage2(void);
extern void TestSendTalkPage3(void);
extern void TestListPagePrintout(void);
extern void TestCancelledPagePrintout(void);
extern void ShowAllImpairedProfiles(void);
extern void ShowActiveImpairedProfiles(void);
extern unsigned ccounter[];
extern unsigned countA[];
extern void ShowInfoGeneral(void);
extern void ShowInfoDatabase(void);
extern void ShowInfoCustomer(void);
extern void ShowInfoParameters(void);
extern void ShowImpairedVendors(void);
extern char * pszDBMSInUseText[];
extern char * pszBooleanText[];
extern char * pszDataSourceText[];
extern char * pszActionReminderCommandText[];
extern BOOL g_bUseAlternateConfigFile;
extern char g_ConfigFileExtension[];
extern class Append2File g_LogAll;
extern class Append2File g_LogSys;
extern class Append2File g_LogDbg;
extern class Append2File g_LogDbs;
extern class Append2File g_LogCom;
extern class Append2File g_LogIvr;
extern class Append2File g_LogStart;
extern class Impairment g_IMP;
extern void WriteToPhoneLineStatusWindow(int, LPCSTR);
extern void WriteToQueueStatusWindow(int, LPCSTR);
extern void WriteToCommPortStatusWindow(int, LPCSTR);
extern int GetExternalControlEvent(void);
extern BOOL CheckLeapYear(int);
extern void SetQueueCancel(int, int);
extern COMMPORT_NAME CommportName[];
extern char voiceMessageTypeStr[3];
extern void SendCancelledPageEmail(int);
extern int WriteToScheduleTable(int, int);
extern CString EscapeSqlText(CString);

extern bool IsSmsType(int);
extern bool IsSmsType(char *); //7/1/2016 - SOguz - Added the method to be able to pass pagetype as parameter

extern void ProcessOnCall(const string& profileId, const string& date);

//extern ostream errorLogStream;

//SO - 03/27/2013 - As per Jim H., commented following statement since they are not used.
//extern LogBuffer generalBuffer;
//extern ostream generalLogStream;
//
//extern LogBuffer phoneBuffer;
//extern ostream phoneLogStream;
//
//extern LogBuffer commBuffer;
//extern ostream commLogStream;

//extern LogBuffer dataBaseBuffer;
extern ostream *dbLogStream;

extern string globalDbConnectionStr;

