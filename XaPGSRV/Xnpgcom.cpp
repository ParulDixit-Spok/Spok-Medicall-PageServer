
// xnpgcom.cpp


#include "common.h"
#include <stdlib.h>
#include "Transaction.h"
#include <string.h>
#include <memory>
#include <string>
#include <sstream>
#include <list>

#include "stringUtil.h"
#include "AdoWrap.h"

// disable deprecation
//#pragma warning(disable:4996)

//auth code     =  15
//message       = 255
//control chars =   5
//checksum      =   3
//				--------
//				  288
//
const int SendBufferSize = 288;	
using namespace std;
HICON hIcon;

char Nul[]			= { 0x00, 0 };
char Etx[]			= { 0x03, 0 };
char cr[]			= { 0x0d, 0 };
char ctrlc[]		= { 0x03, 0 };
char CtrlK[]		= { 0x0b, 0 };
char Stx[]			= { 0x02, 0 };
char EotCr[]		= { 0x04, 0x0d, 0 };
char Ack2[]			= { 0x06, 0 };
char Ack3[]			= { 0x06, 0x0d, 0 };								// was <CR> following 1.8-
char CrAckCr[]		= { 0x0d, 0x06, 0x0d, 0 };							// use this "ack" sequence on blocks that are not the "last one".
char abandon[]		= { 0x0d, 0x1e, 0x0d, 0 };							// ascii RS usually means invalid pager, go on to next request.
char abandon2[]		= { 0x1e, 0 };
char Nack[]			= { 0x15, 0x0d, 0 };								// ascii NAK usually means bad checksum, send again.
char Nack2[]		= { 0x15, 0 };
char EscPG1Cr[]		= { 0x1b, 'P',  'G',  '1',  0x0d, 0 };
char EscPCr[]		= { 0x06, 0x0d, 0x1b, '[',  'p',  0x0d, 0 };		// for IXO protocol
char EscPG8Cr[]		= { 0x1b, 'P',  'G',  '8',  0x0d, 0 };
char EscP[]			= { 0x1b, '[',  'p',  0x0d, 0 };
char EscPCrCr[]		= { 0x06, 0x0d, 0x1b, '[',  'p',  0x0d, 0x0d, 0 };	// for IXO protocol
char IDEQ[]			= { 'I',  'D',  '=',  0x0d, 0x0a, 0 };
char ModemOnHk[]	= { 'A',  'T',  'H',  '0',  0x0d, 0 };
char IPTUser[]		= { 'P',  'A',  'G',  'E',  0x0d, 0 };
char CATSInit2[]	= { 0x41, 0x54, 0x54, 0x53, 0x50, 0x21, 0x0d, 0 };   // ATTSP!cr
char CATSInit3[]	= { 0x51, 0x19, 0x20, 0 };
char CATSKeyHit[]	= { 0x51, 0x11, 0 };
char Eot[]         = { 0x04, 0 };
char EscEotCr[]    = { 0x1b, 0x04, 0x0d, 0 };

int pport;	// index to com port, usually used in a for-next loop

SYSTEMTIME starttime[MAX_COMMPORTS];
unsigned ccounter[MAX_COMMPORTS];
unsigned countA[MAX_COMMPORTS];
char pchar[MAX_COMMPORTS][2];
unsigned ccount[MAX_COMMPORTS];

// these are used to keep track of multiple TAP blocks.
//  they are allocated per comm port, as there could be multiple comm ports 
//  having the same protocol.
unsigned Multi_TAP_Block_Count[MAX_COMMPORTS];
unsigned Multi_TAP_Block_Index[MAX_COMMPORTS];
char TAP_Block_Data[MAX_COMMPORTS][10][350];		

// forward references.
void xpagechk(int pgport, unsigned nextstate, unsigned errorstate, unsigned ptimeout, char * prompt);
void pagerLoginCheck(int pgport, unsigned nextstate, unsigned errorstate, unsigned ptimeout);

CString ReduceMultipleSpaces(CString);

	char * 
CPortMessage[] = 
	{ 
	"RESET",
	"RESET1", 
	"RESET2", 
	"RESET3",
	"RESET4", 
	"RESET5", 
	"RESET6", 
	"RESET7",
	"RESET8", 
	"RESET9", 

	"RESETA", 
	"RESETB",
	"RESETC", 
	"RESETD", 
	"NOTREADY", 
	"NOTREADYTOUT",
	"DTRLOW", 
	"DTRPAUSE", 
	"DTRHIGH",
	"READY", 

	"SEND",
	"MESSAGE",
	"RESETJ",
	"TALKNOW", 
	"MESSAGETOUT", 
	"PAGINGTOUT", 
	"TALKNOWTOUT",
	"RESETTIMEOUT", 
	"SEND2", 
	"SEND3", 

	"SEND4",
	"SEND5", 
	"SEND6", 
	"SEND7", 
	"SEND8",
	"SEND9", 
	"SENDTIMEOUT", 
	"WAIT1",
	"WAIT2", 
	"WAIT3",
	
	"ABORT",
	"ABORT2", 
	"ABORT3", 
	"ABORT4", 
	"ABORT5",
	"ABORT6", 
	"TALKING", 
	"SENDA", 
	"SENDB",
	"PAGINGTOUT2", 

	"xxxUNKNOWN", 
	"PAGINGBUSY", 
	"TALKBUSY",
	"HANGUP", 
	"HANG2", 
	"HANG3",
	"HANG4", 
	"HANG5", 
	"HANG6",
	"READY2",

	"DELAYB4DISC",
	"DISCSEQ",
	"DELAYB4DTRLOW",
	"ST_BEGIN_TAP_MESSAGE",
	"ST_GET_TAP_BLOCK",
	"ST_CHECK_ERROR_COUNTS",
	"ST_WAIT_FOR_BLOCK_BEGIN",
	"ST_WAIT_FOR_BLOCK_END",
	"ST_WAIT_FOR_CHECKSUM",
	"ST_ANALYZE_CHECKSUM",

	"ST_ANALYZE_BLOCK",
	"ST_BEGIN_SIGNON",			
	"ST_SEND_GOAHEAD",
	"ST_WAIT_FOR_LOGIN",
	"ST_BAD_CHECKSUM",
	"ST_GOOD_PAGERID",
	"ST_BAD_PAGERID",
	"ST_ANALYZE_TAP_MESSAGE",
	"ST_DISCONNECT_AND_START_OVER",
	"ST_ACK_AND_GET_NEXT_BLOCK",

	"ST_NEW_1",
	"ST_NEW_2",
	"ST_NEW_3",
	"ST_NEW_4",

	"LAST_ONE"
	};


	unsigned int 
AddString(const char* ptr)
	{
	unsigned int sum = 0;

	for(; *ptr; ++ptr)
		{
		sum += (static_cast<unsigned int>(*ptr) & 0x7F); 
		}

	return sum;
	}


	void 
CalculateTAPChecksum(const char * pCapcode,
						const char * pMessage,
						char * pCheckSum)
	{
	unsigned int checksum;

	checksum = 31;			//stx + cr + cr + etx = 31

	checksum += AddString(pCapcode);
	checksum += AddString(pMessage);

	pCheckSum[2] = static_cast<char>(0x30 + (checksum & 0xF));
	checksum >>= 4;
	pCheckSum[1] = static_cast<char>(0x30 + (checksum & 0xF));
	checksum >>= 4;
	pCheckSum[0] = static_cast<char>(0x30 + (checksum & 0xF));

	pCheckSum[3] = 0;
	}


	// initialize a port, activate, deactivate
	//  comm port data structures, and open the ports
	//
	void
pageit(int command)
	{	
	switch(command)
		{
		case SETUP:
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndComMsg, 
							"Initializing Com port buffers.");

			// Set up each comm port.
			//
			for(pport = 0; pport < MAX_COMMPORTS; pport++)
				{
				if(portactive[pport] == TRUE)
					{
					// initialize to "no errors".
					pageerror[pport] = PNOERROR;
					
					// clear out the holding buffer
					memset(holdbuff[pport], 0, RXBUFFLEN);

					GetLocalTime(&starttime[pport]);
					}
				}

			PrintMessage(MSGTYPE_SYSTEM,  
							hwndComMsg, 
							"Com Ports initialized.");
			break;
			
		case ACTIVATE:
			for(pport = 0; pport < MAX_COMMPORTS; pport++)
				{
				// if the port is configured/enabled in the parameter file.
				//
				if(portactive[pport] == TRUE)
					{
					if(OpenComport(pport))
						{
						//PrintMessage(MSGTYPE_SYSTEM, 
						//				hwndComMsg, 
						//				"[%s]: Opened",  
						//				CommportName[pport].FamiliarName);
						}
					else
						{
						//PrintMessage(MSGTYPE_SYSTEM, 
						//				hwndComMsg, 
						//				"[%s]: Not Opened",  
						//				CommportName[pport].FamiliarName);
						}
					}
				else
					{
					//PrintMessage(MSGTYPE_SYSTEM, 
					//				hwndComMsg, 
					//					"[%s]: not configured",  
					//					CommportName[pport].FamiliarName);
					}
				}
			break;
			
		case DEACTIVATE:
			for(pport = 0; pport < MAX_COMMPORTS; pport++)
				{
				if(portactive[pport] == TRUE)
					{
					CloseComPort(pport);
					}
				}
			break;
			
		default:
			break;
		}
	}


	void
scanaport(void)
	{
	static int iStep = 0;
	WORD wCurrentIconId = IDI_STA1;

	if(hwndDataScopeDlg != NULL)
		{
		// display next sequential icon on
		//  data scope top left border, if the window is open.
		iStep++;
		iStep %= 4;

		switch(iStep)
			{
			case 0:
				wCurrentIconId = IDI_STA2;
				break;

			case 1:
				wCurrentIconId = IDI_STA3;
				break;

			case 2:
				wCurrentIconId = IDI_STA4;
				break;

			case 3:
			default:
				wCurrentIconId = IDI_STA1;
				break;
			}
		
		SendMessage(hwndDataScopeDlg,
					WM_SETICON,
					ICON_SMALL,
					(LPARAM) LoadIcon(hInst, MAKEINTRESOURCE(wCurrentIconId)));		
 		}

    for(pport = 0; pport < MAX_COMMPORTS; pport++)
		{
		
		if(portactive[pport] == TRUE)
			{
			UpdateCommPortStatusWindow(pport);
			switch(pageactive[pport])
				{
				case COMM_NOT_ACTIVE:		// NO
					break;

				case ZETRON15:				// MTR
					Motorola();
					break;

				case ZETRON2000:			// ZTN - Now only Zetron2000
					Zetron2K();
					break;

				case ZETRON16:				// Z16 - split the implementation so as to not disturb Zetron2000
					Zetron16();				//       Zetron16 and Zetron2000 used to be a common implementation
					break;					//       just differentiating for "<esc> PG1" vs "<esc> PG8" 

				case TAP:					// TAP					
					Tap();
					break;

				case TAP_ARCH_UVA:			// UVA
					TapArchUVA();
					break;

				case IXO_MODEM:				// XOM
					IxoModem();
					break;

				case PRONET_IPT:			// IPT
					PronetIpt();
					break;

				case IXO_ACCEPT:			// IXO
					Ixo_Accept_Direct();	// if modem initialization string is defined, use it, and the states that go with it.
					break;

				case IXO_ACCEPT_DIRECT:		// IXD
					Ixo_Accept_Direct();	//  for direct connection.
					break;

				case IXO_DISCARD:			// NUL
					Ixo_Accept_Direct();	//  for testing, at the last minute, don't actually write to the queue.
					break;

				case IXX_DIRECT:			// IXX
					Ixo_Accept_Direct();	//  for looking up in Xkm:PatientId, instead of Pagers:PagerId.
					break;

				case TAP_TYPE2:				// TA2
					Tap2();					// Tap Variation #2 for CHOP.
					break;

				case TAP_TYPE3:				// TA3
					Tap3();					// Tap Variation #3 for Oakwood Hospital.
					break;

				case GROUPCALL:				// GRP
					GroupCall();
					break;
				}
			}
		}
	return;
	}


	// Zetron 15b paging encoder device.
	//
	void
Motorola(void)
	{
	int j, k, l;
	char CapcodeMessage[100];

    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 200, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            ComPort[pport].reset++;
            WritePort(pport, 1, ctrlc);
            pstate[pport] = PRESETTIMEOUT;
            break;

        case PRESETTIMEOUT:
            if(Wait(&starttime[pport], 300L))
				{
                pstate[pport] = PRESET2;
				}
            break;

        case PRESET2:
			// flush buffer after resetting z15b
            while(getbuff(pport, pchar[pport]) == TRUE)
				{
				}
            pstate[pport] = PNOTREADY;
            break;

        case PNOTREADY:
            WritePort(pport, 1, cr);
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
        case PWAIT2:
            zpagechk(pport, PREADY, PRESET, 10 * 100, 1, "Enter capcode: ");
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            while(getbuff(pport, pchar[pport]) == TRUE); // clean up incoming.

            ccounter[pport]=0;
            break;

        case PSEND:
            pageerror[pport] = PNOERROR;
            while(getbuff(pport, pchar[pport]) == TRUE); // clean up incoming.

            GetLocalTime(&starttime[pport]);
            ccounter[pport] = 0;

            pageerror[pport] = WRONGPAGETYPEERR;
            pstate[pport] = PREADY;

			//  at this point, we need the capcode string
			//  with a <cr> appended.
			memset(CapcodeMessage, 0, sizeof(CapcodeMessage));
			//sprintf(CapcodeMessage, "%s\x0d", pcapcode[pport]);
			// jph 06/25/2012
			sprintf_s<sizeof(CapcodeMessage)>(CapcodeMessage, "%s\x0d", pcapcode[pport]);


			// there are 15 "functions" defined in the Zetron 15b.
			//  this pertains to the universal paging software
			//  option defined in section 8 of the Zetron model 15 document:
			//  "Model 15 multi-format encoder operating manual" #025-9003T
            for(j = 0; j < 15; j++)
				{
                if(pcapcode[pport][0] == param.PFormat[j].Id[0])
					{
                    k = pcapcode[pport][param.PFormat[j].Position];

                    for(l = 0; l < (int) strlen(param.PFormat[j].VoiceFuncs); l++)
						{
                        if(k == param.PFormat[j].VoiceFuncs[l])
							{
                            pageerror[pport] = PNOERROR;
                            pstate[pport] = PTALKNOWTIMEOUT;
                            break;
							}
						}

                    for(l = 0; l < (int) strlen(param.PFormat[j].MsgFuncs); l++)
						{
                        if(k == param.PFormat[j].MsgFuncs[l])
							{
                            pageerror[pport] = PNOERROR;
                            pstate[pport] = PMESSAGETIMEOUT;
                            break;
							}
						}

                    for(l = 0; l < (int) strlen(param.PFormat[j].ToneFuncs); l++)
						{
                        if( k == param.PFormat[j].ToneFuncs[l])
							{
                            pageerror[pport] = PNOERROR;
                            pstate[pport] = PPAGINGTIMEOUT;
                            break;
							}
						}

					WritePort(pport, 1, CapcodeMessage); 						
                    break;
					}
				}
            break;

        case PMESSAGE:
			// the <cr> was removed from the message for checksum calculation purpose,
			//  it must be replaced here because it's part of the z15b protocol.
			//strcat(pmessage[pport], cr);
			// jph 06/25/2012
			strcat_s<sizeof(pmessage[0])>(pmessage[pport], cr);

            WritePort(pport, 1, pmessage[pport]);
            pstate[pport] = PPAGINGTIMEOUT;
            GetLocalTime(&starttime[pport]);
            break;

        case PMESSAGETIMEOUT:
            zpagechk(pport, PMESSAGE, PRESET, 500, 1, "Enter message: ");
            break;

        case PPAGINGTIMEOUT:
            zpagechk(pport, PWAIT2, PRESET, 10 * 100, 1, "Paging...");

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
				CheckBuffer(pport, PPAGINGBUSY, 1, "*Busy*");

				if(pstate[pport] == PPAGINGBUSY)
					{
					GetLocalTime(&starttime[pport]);
					}
				}

            if(pstate[pport] == PWAIT2)
				{
                GetLocalTime(&starttime[pport]);
				}
            break;

        case PPAGINGBUSY:
            zpagechk(pport, PWAIT2, PRESET, 9000, 1, "Paging...");

            if(pstate[pport] == PWAIT2)
				{
                ComPort[pport].timer.wHour = 70;
                GetLocalTime(&starttime[pport]);
				}
            break;

        case PTALKNOWTIMEOUT:
            zpagechk(pport, PTALKNOW, PRESET, 1500, 1, "Talk");

            if(pstate[pport] == PTALKNOW)
				{
				GetLocalTime(&starttime[pport]);
				}

            if(pstate[pport] == PTALKNOWTIMEOUT)
				{
				CheckBuffer(pport, PTALKNOWBUSY, 1, "*Busy*");
				if(pstate[pport] == PTALKNOWBUSY)
					{
					GetLocalTime(&starttime[pport]);
					}
				}
            break;

        case PTALKNOWBUSY:
            zpagechk(pport, PTALKNOW, PRESET, 9000, 1, "Talk");

            if(pstate[pport] == PTALKNOW)
				{
                ComPort[pport].timer.wHour = 70;
                GetLocalTime(&starttime[pport]);
				}
            break;

        case PTALKNOW:
            GetLocalTime(&starttime[pport]);
            break;

        case PTALKING:
            zpagechk(pport, PREADY, PRESET, 3500, 1, "Enter capcode:");
           break;

        case PHANGUP:
            if(param.voiceSquelch)
				{
                WritePort(pport, 1, "!");
				}
            else
				{
                WritePort(pport, 1, ctrlc);
				}
            pstate[pport] = PTALKING;
            break;

        case PWAIT1:
            break;

        default:
            break;
		}
	}


	// Zetron model 2000 only.
	//
	void
Zetron2K(void)
	{
	char temp[SendBufferSize];
	char Checksum[4];

    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 200, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
			countA[pport] = 0;
            ComPort[pport].reset++;
            WritePort(pport, 1, EotCr);
            pstate[pport] = PRESET2;
            break;

        case PRESET2:
            zpagechk(pport, PRESET4, PRESET4, 100, 0, "JOE");
            break;

        case PRESET4:
            countA[pport]++;
            ComPort[pport].reset++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET5;
            break;

        case PRESET5:
            zpagechk(pport, PNOTREADY, PRESET4, 200, 0, "ID=");

			if(pstate[pport] == PRESET5)
				{
				CheckBuffer(pport, PREADY, 1, EscP);
				}

            if(countA[pport] > 12 && pstate[pport] == PRESET5)
				{
				pstate[pport] = PRESET;
				}
            break;

        case PNOTREADY:
			// no longer differentiated from Zetron16, now exclusively for Zetron2000
			WritePort(pport, 1, EscPG1Cr);
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
            zpagechk(pport, PREADY, PRESET, 10 * 100, 1, EscP);
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            countA[pport] = 0;
            zpagechk(pport, PRESET4, PREADY, 500, 1, "bye");
            break;

        case PSEND:
			CalculateTAPChecksum(pcapcode[pport], pmessage[pport], Checksum);

			// build up the message piece by piece.
            memset(temp, 0, sizeof(temp));

			// prepare the TAP message
            strcat(temp, "\x02");
            strcat(temp, pcapcode[pport]);
            strcat(temp, "\x0d");
            strcat(temp, pmessage[pport]);
            strcat(temp, "\x0d");
            strcat(temp, "\x03");			
            strcat(temp, Checksum);
            strcat(temp, "\x0d");

			// send the message to the serial port.
            WritePort(pport, 1, temp);
            memset(temp, 0, sizeof(temp));

            pstate[pport] = PPAGINGTIMEOUT;
            break;

        case PPAGINGTIMEOUT:
            zpagechk(pport, PREADY, PRESET, 3000, 1, Ack2);

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, "Batches are Full");
				if(pstate[pport] == PRESET)
					{
		            ComPort[pport].reset = 0;
                    ComPort[pport].greenLight = TRUE;
					}
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, Nack2);
				}

			if(pstate[pport] == PRESET)
				{
				pageerror[pport] = PPAGINGTIMEOUT;
				}

            break;

        default:
            break;
		}
	}


	void
Zetron16(void)
	{
	// CString temp2;
	//char temp[SendBufferSize];
	//char Checksum[4];

    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PRESET");
			//Sleep(1 * 50);

            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 200, 0, "JOE");
            break;

        case PDTRHIGH:

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PDTRHIGH");
			//Sleep(1 * 50);

            SetDTR(pport, TRUE);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
			countA[pport] = 0;
            ComPort[pport].reset++;
            WritePort(pport, 1, EotCr);
            pstate[pport] = PRESET2;

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PRESET1");
			//Sleep(1 * 50);
            break;

        case PRESET2:
            zpagechk(pport, PRESET4, PRESET4, 100, 0, "JOE");

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PRESET2");
			//Sleep(1 * 50);

            break;

        case PRESET4:

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PRESET4");
			//Sleep(1 * 50);

            countA[pport]++;
            ComPort[pport].reset++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET5;
            break;

        case PRESET5:
            zpagechk(pport, PNOTREADY, PRESET4, 200, 0, "ID=");

			if(pstate[pport] == PRESET5)
				{
				CheckBuffer(pport, PREADY, 1, EscP);
				}

            if(countA[pport] > 12 && pstate[pport] == PRESET5)
				{
				pstate[pport] = PRESET;
				}
            break;

        case PNOTREADY:
			// capcode paging mode of to Zetron16
			WritePort(pport, 1, EscPG8Cr);
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
			// 1000 count is 10 seconds.
            zpagechk(pport, PREADY, PRESET, 10 * 100, 1, EscP);
            break;

        case PREADY:
			//  Wait here until flat-file page queue reader finds something for us to do.
			//
            ComPort[pport].reset = 0;
            countA[pport] = 0;
            zpagechk(pport, PRESET4, PREADY, 500, 1, "bye");
            break;

        case PSEND:

			// debug code, remove after debugging.
			///rintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "State Machine MAGIC... At: PSEND");
			//Sleep(1 * 50);

			// the capcode is just that, no cr is present at the end of it.
			char CapcodeMessage[100];

			memset(CapcodeMessage, 0, sizeof(CapcodeMessage));
			sprintf_s<sizeof(CapcodeMessage)>(CapcodeMessage, "%s\x0d", pcapcode[pport]);

			// this is the capcode, which is now has a <cr> appended.
			pstate[pport] = PSEND8;
			break;

        case PSEND8:
			{

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PSEND8");
			//Sleep(1 * 50);

			// prepare to send a message over TAP protocol.
			// initialize variables for multiple TAP blocks.
			Multi_TAP_Block_Count[pport] = 0;
			Multi_TAP_Block_Index[pport] = 0;
		
			// all data from previous operations gets cleared out.
			for(int x = 0; x < 10; x++)
				{
				ZeroMemory(TAP_Block_Data[pport][x], sizeof(TAP_Block_Data[0][0]));
				}

			auto_ptr<Transaction> message(new Transaction());

			message->SetAlternateBlockContinuationChar(true);	// for Z16 use alternate continuation character.

			message->AddField(string(pcapcode[pport]));			// for Z16 use pcapcode instead of pauthcode.
			message->AddField(messageString[pport]);
			message->Blockify();

			Block* bPtr;
			message->ResetBlockList();
			
			{ // scope limit for variable x
			int x = 0;

			while((bPtr = message->GetNextBlock()) != NULL)
				{
				strcpy_s <sizeof(TAP_Block_Data[0][0])> (TAP_Block_Data[pport][x], bPtr->GetBlockBuffer());

				x++;
				Multi_TAP_Block_Count[pport] = x;

				// Moved the serial data "write" operation to a different state, so that the ACK for each block
				//  can be checked as each TAP block is sent.
				}
			}
			}

			// for debug... show the data collected.
			//
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "showing TAP blocks");

			//for(int x = 0; x < Multi_TAP_Block_Count[pport]; x++)
			//	{
			//	PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%d][%s] - ", x, TAP_Block_Data[pport][x]);
			//	}

            pstate[pport] = PSEND9;
            break;


		case PSEND9:
			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "Zetron16 - At: PSEND9");
			//Sleep(1 * 50);

			//send out the message blocks over TAP protocol.

			// if you get here, it is either the first block is to be sent,
			//  or a block of a multi-block page was successfully sent, 
			//  and the next one should be sent, if there is one.

			// if the final block has been sent, successfully, then the job is complete.

			// step thru each element of the array that was captured in PSEND8 state.
			//

			// check the first character of the block, 
			//  if it is a NUL char, the last block has already been sent.
			if(0 == TAP_Block_Data[pport][Multi_TAP_Block_Index[pport]][0])
				{
				// the block is empty, there are no more blocks.
				// all blocks have been sent successfully.
				// this page job is complete,
				// the page job is a success.
				//
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "Page Success - Zetron16 protocol - PSEND9 state.");

				// also, claim no error!
				pageerror[pport] = PNOERROR;

				pstate[pport] = PREADY2;
				}
			else
				{
				// send the block.
				WritePort(pport, 1, TAP_Block_Data[pport][Multi_TAP_Block_Index[pport]]);

				// advance the index to the next block to be sent.
				Multi_TAP_Block_Index[pport]++;

				// then wait for the result.
				pstate[pport] = PPAGINGTIMEOUT;
				}

			break;

        case PPAGINGTIMEOUT:
			// just waiting for something to happen, 
			//  a recognizable message being received, or timeout happens.

			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PPAGINGTIMEOUT");
			// xxxxxxxxxxx Sleep(1 * 50);

			//zpagechk(pport, PREADY, PRESET, 3 * 100, 1, Ack2);  // only 3 seconds??????
			zpagechk(pport, PREADY, PRESET, 30 * 100, 1, Ack2);

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
				// no timeout, no ack received, state has not been changed, 
				//  react to possible message in the buffer.
                CheckBuffer(pport, PRESET, 1, "Batches are full");

				if(pstate[pport] == PRESET)
					{
		            ComPort[pport].reset = 0;
                    ComPort[pport].greenLight = TRUE;
					}
				}

			// best that could have happened, an ACK was found, 
			//  continue at PSEND9 to send antoher block if there is one avaialable.
			//
            if(pstate[pport] == PREADY)
				{
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "Got ACK in PPAGINGTIMEOUT state.");

				pstate[pport] = PSEND9;
				break;
				}

			// maybe negative ack received...
            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, Nack2);
				}

 			if(pstate[pport] == PRESET)
				{
				pageerror[pport] = PPAGINGTIMEOUT;
				}

            break;
			
        default:
            break;
		}
	}


	void
Tap(void)
	{
    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 300, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            countA[pport] = 0;
            ComPort[pport].reset++;
            WritePort(pport, 1, EotCr);
            pstate[pport] = PRESET2;
            break;

        case PRESET2:
            zpagechk(pport, PRESET4, PRESET4, 100, 0, "JOE");
            break;

        case PRESET4:
            countA[pport]++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET5;
            break;

        case PRESET5:
            zpagechk(pport, PNOTREADY, PRESET4, 200, 0, "ID=");

            if(countA[pport] > 12 && pstate[pport] == PRESET5)
				{
				pstate[pport] = PRESET;
				}
            break;

        case PNOTREADY:
            WritePort(pport, 1, EscPG1Cr);
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
            zpagechk(pport, PREADY, PRESET, 500, 0, EscP);

            if(pstate[pport] == PREADY)
				{
				GetLocalTime(&starttime[pport]);
				}
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            countA[pport] = 0;
			ComPort[pport].BlockSendAttempts = 3;		// init to 3, hard error if it goes to 0.

			// "+++ATH" could come from the rcc paging terminal
			//  down the leased line, this is a hangup command
			//  to a hayes compatible dialup modem, but since
			//  this is a leased line and not a dialup connection,
			//  the page server may see this command.
			// the sensible thing to do if this is detected is
			//   to recycle the port, and start a new connection.
			//
            zpagechk(pport, PRESET4, PREADY, 500, 1, "ATH");

			// 2002-10-17 added this for MASCO / ARCH leased line, 
			//  they don't send the larger "DISCONNECT" message
			//
            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, Eot);
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "Good Bye");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "PAGES SENT");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "DISCONNECT");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "TERMINAL FAILED");
				}

			//when "ID=" comes, we are not synchronized nicely, but
			//  advance to PNOTREADY where "<esc>PG1<cr>" will be sent.
			if(pstate[pport] == PREADY )
				{
                CheckBuffer(pport, PNOTREADY, 1, "ID=");
				}

			//when "Thank you" comes,
		    //  (ARCH communications may send this, 2001-07-10)
			//  advance to PRESET4 to sign on again.
			if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "Thank you");
				}

            break;

        case PSEND:
			{
			auto_ptr<Transaction> message(new Transaction());

			message->AddField(string(pcapcode[pport]));
			message->AddField(messageString[pport]);
			message->Blockify();		

			Block* bPtr;
			message->ResetBlockList();

			while((bPtr = message->GetNextBlock()) != NULL)
				{
				WritePort(pport, 1, bPtr->GetBlockBuffer());
				}
			}

        pstate[pport] = PPAGINGTIMEOUT;
        break;

		// waiting for an acknowledgement 0x06, but other 
		//  recognizable "negative" responses will be acted on 
		//  immediately, without the need to timeout.
		//  this reduces unnecessary delays when things are not going well.
		//  
        case PPAGINGTIMEOUT:
			xpagechk(pport, PREADY, PRESET4, (10 * 100), Ack2);	

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 1, "Page OK");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 0, abandon2);
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 0, abandon);
				}

			if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "ATH");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "ID=");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "Good Bye");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
 				{
               CheckBuffer(pport, PRESET4, 1, "Invalid");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, "Bad Line");
				}

 			if(pstate[pport] == PRESET || pstate[pport] == PRESET4)
				{
				pageerror[pport] = PPAGINGTIMEOUT;

				memset(ComPort[pport].errorResponse, 0, sizeof(ComPort[pport].errorResponse));
				//sprintf(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);
				sprintf_s<81>(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);

				Remove_CtrlChars(ComPort[ pport ].errorResponse, 
									sizeof(ComPort[pport].errorResponse));
				Set_Null(ComPort[pport].errorResponse, 
							sizeof(ComPort[pport].errorResponse));

				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"COM%d:, Response [%s]", 
								pport + 1, 
								ComPort[pport].errorResponse);
				}
           break;

        default:
            break;
		}
	}



	void
Tap2(void)
	{
    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 300, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            countA[pport] = 0;
            ComPort[pport].reset++;
            WritePort(pport, 1, EotCr);
            pstate[pport] = PRESET2;
            break;

        case PRESET2:
            zpagechk(pport, PRESET4, PRESET4, 100, 0, "JOE");
            break;

        case PRESET4:
            countA[pport]++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET5;
            break;

        case PRESET5:
            zpagechk(pport, PNOTREADY, PRESET4, 200, 0, "ID=");

            if(countA[pport] > 12 && pstate[pport] == PRESET5)
				{
				pstate[pport] = PRESET;
				}
            break;

        case PNOTREADY:
            WritePort(pport, 1, EscPG1Cr);
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
            zpagechk(pport, PREADY, PRESET, 500, 0, EscP);

            if(pstate[pport] == PREADY)
				{
				GetLocalTime(&starttime[pport]);
				}
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            countA[pport] = 0;
			ComPort[pport].BlockSendAttempts = 3;		// init to 3, hard error if it goes to 0.

			// "+++ATH" could come from the rcc paging terminal
			//  down the leased line, this is a hangup command
			//  to a hayes compatible dialup modem, but since
			//  this is a leased line and not a dialup connection,
			//  the page server may see this command.
			// the sensible thing to do if this is detected is
			//   to recycle the port, and start a new connection.
			//
            zpagechk(pport, PRESET4, PREADY, 500, 1, "ATH");

			// 2002-10-17 added this for MASCO / ARCH leased line, 
			//  they don't send the larger "DISCONNECT" message
			//
            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, Eot);
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "Good Bye");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "PAGES SENT");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "DISCONNECT");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "TERMINAL FAILED");
				}

			//when "ID=" comes, we are not synchronized nicely, but
			//  advance to PNOTREADY where "<esc>PG1<cr>" will be sent.
			if(pstate[pport] == PREADY )
				{
                CheckBuffer(pport, PNOTREADY, 1, "ID=");
				}

			//when "Thank you" comes,
		    //  (ARCH communications may send this, 2001-07-10)
			//  advance to PRESET4 to sign on again.
			if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "Thank you");
				}

            break;

        case PSEND:
			{
			auto_ptr<Transaction> message(new Transaction());

			message->AddField(string(pcapcode[pport]));
			message->AddField(messageString[pport]);
			message->Blockify();		

			Block* bPtr;
			message->ResetBlockList();

			while((bPtr = message->GetNextBlock()) != NULL)
				{
				WritePort(pport, 1, bPtr->GetBlockBuffer());
				}
			}

        pstate[pport] = PPAGINGTIMEOUT;
        break;

		// waiting for an acknowledgement 0x06, but other 
		//  recognizable "negative" responses will be acted on 
		//  immediately, without the need to timeout.
		//  this reduces unnecessary delays when things are not going well.
		//  
        case PPAGINGTIMEOUT:
			xpagechk(pport, PREADY, PRESET4, (10 * 100), Ack2);	

			// if NAK is seen, just advance to the ready state to send the next block.
			//  added 2010-04-20 kgill.
            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 1, Nack2);
				}

			// if EOT is seen, perform "maximum" reset, start over at PRESET state.
			//  added 2010-04-20 kgill.
            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, Eot);
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 1, "Page OK");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 0, abandon2);
				}

			if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "ATH");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "ID=");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "Good Bye");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
 				{
               CheckBuffer(pport, PRESET4, 1, "Invalid");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, "Bad Line");
				}

 			if(pstate[pport] == PRESET || pstate[pport] == PRESET4)
				{
				pageerror[pport] = PPAGINGTIMEOUT;

				memset(ComPort[pport].errorResponse, 0, sizeof(ComPort[pport].errorResponse));
				//sprintf(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);
				sprintf_s<sizeof(ComPort[0].errorResponse)>(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);

				Remove_CtrlChars(ComPort[ pport ].errorResponse, 
									sizeof(ComPort[pport].errorResponse));
				Set_Null(ComPort[pport].errorResponse, 
							sizeof(ComPort[pport].errorResponse));

				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"COM%d:, Response [%s]", 
								pport + 1, 
								ComPort[pport].errorResponse);
				}
           break;

        default:
            break;
		}
	}


	void
Tap3(void)
	{
    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 300, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            countA[pport] = 0;
            ComPort[pport].reset++;
            WritePort(pport, 1, EotCr);
            pstate[pport] = PRESET2;
            break;

        case PRESET2:
            zpagechk(pport, PRESET4, PRESET4, 100, 0, "JOE");
            break;

        case PRESET4:
            countA[pport]++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET5;
            break;

        case PRESET5:
            zpagechk(pport, PNOTREADY, PRESET4, 200, 0, "ID=");

            if(countA[pport] > 12 && pstate[pport] == PRESET5)
				{
				pstate[pport] = PRESET;
				}
            break;

        case PNOTREADY:
            WritePort(pport, 1, EscPG1Cr);
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
            zpagechk(pport, PREADY, PRESET, 500, 0, EscP);

            if(pstate[pport] == PREADY)
				{
				GetLocalTime(&starttime[pport]);
				}
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            countA[pport] = 0;
			ComPort[pport].BlockSendAttempts = 3;		// init to 3, hard error if it goes to 0.

			// "+++ATH" could come from the rcc paging terminal
			//  down the leased line, this is a hangup command
			//  to a hayes compatible dialup modem, but since
			//  this is a leased line and not a dialup connection,
			//  the page server may see this command.
			// the sensible thing to do if this is detected is
			//   to recycle the port, and start a new connection.
			//
            zpagechk(pport, PRESET4, PREADY, 500, 1, "ATH");

			// 2002-10-17 added this for MASCO / ARCH leased line, 
			//  they don't send the larger "DISCONNECT" message
			//
            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, Eot);
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "Good Bye");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "PAGES SENT");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "DISCONNECT");
				}

            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "TERMINAL FAILED");
				}

			//when "ID=" comes, we are not synchronized nicely, but
			//  advance to PNOTREADY where "<esc>PG1<cr>" will be sent.
			if(pstate[pport] == PREADY )
				{
                CheckBuffer(pport, PNOTREADY, 1, "ID=");
				}

			//when "Thank you" comes,
		    //  (ARCH communications may send this, 2001-07-10)
			//  advance to PRESET4 to sign on again.
			if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, "Thank you");
				}

            break;

        case PSEND:
			{
			auto_ptr<Transaction> message(new Transaction());

			message->AddField(string(pcapcode[pport]));
			message->AddField(messageString[pport]);
			message->Blockify();		

			Block* bPtr;
			message->ResetBlockList();

			while((bPtr = message->GetNextBlock()) != NULL)
				{
				WritePort(pport, 1, bPtr->GetBlockBuffer());
				}
			}

        pstate[pport] = PPAGINGTIMEOUT;
        break;

		// waiting for an acknowledgement 0x06, but other 
		//  recognizable "negative" responses will be acted on 
		//  immediately, without the need to timeout.
		//  this reduces unnecessary delays when things are not going well.
		//  
        case PPAGINGTIMEOUT:
			xpagechk(pport, PREADY, PRESET4, (10 * 100), Ack2);	

			// if NAK is seen, just advance to the ready state to send the next block.
			//  added 2010-04-20 kgill.
            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 1, Nack2);
				}

			// if EOT is seen, perform "maximum" reset, start over at PRESET state.
			//  added 2010-04-20 kgill.
            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, Eot);
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 1, "Page OK");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 0, abandon2);
				}

			if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "ATH");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "ID=");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET4, 1, "Good Bye");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
 				{
               CheckBuffer(pport, PRESET4, 1, "Invalid");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PRESET, 1, "Bad Line");
				}

 			if(pstate[pport] == PRESET || pstate[pport] == PRESET4)
				{
				pageerror[pport] = PPAGINGTIMEOUT;

				memset(ComPort[pport].errorResponse, 0, sizeof(ComPort[pport].errorResponse));
				//sprintf(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);
				sprintf_s<sizeof(ComPort[0].errorResponse)>(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);

				Remove_CtrlChars(ComPort[ pport ].errorResponse, 
									sizeof(ComPort[pport].errorResponse));
				Set_Null(ComPort[pport].errorResponse, 
							sizeof(ComPort[pport].errorResponse));

				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"COM%d:, Response [%s]", 
								pport + 1, 
								ComPort[pport].errorResponse);
				}
           break;

        default:
            break;
		}
	}


	// this implementation based on Tap();
	//  to accomodate MASCO and Arch Paging UVA page output.
	//  2002-10-18
	//
	void 
TapArchUVA(void)
	{
	//char temp[255];
	char temp[SendBufferSize];
	char Checksum[4];

    switch( pstate[pport] )
		{
        case PRESET:
            SetDTR( pport, FALSE );
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk( pport, PDTRHIGH, PDTRHIGH, 300, 0, "JOE" );
            break;

        case PDTRHIGH:
            SetDTR( pport, TRUE );
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            countA[ pport ] = 0;
            ComPort[pport].reset++;
            WritePort( pport, 1, EotCr );
            pstate[ pport ] = PRESET2;
            break;

        case PRESET2:
            zpagechk( pport, PRESET4, PRESET4, 100, 0, "JOE" );
            break;

        case PRESET4:
            countA[ pport ]++;
            WritePort( pport, 1, cr );
            pstate[ pport ] = PRESET5;
            break;

        case PRESET5:
            zpagechk( pport, PNOTREADY, PRESET4, 200, 0, "ID=" );
            if( countA[ pport ] > 12 && pstate[ pport ] == PRESET5 ) 
				pstate[ pport ] = PRESET;
            break;

        case PNOTREADY:
            EscPG1Cr[ 3 ] = '1';
            WritePort( pport, 1, EscPG1Cr );
            pstate[pport] = PNOTREADYTIMEOUT;
            break;

        case PNOTREADYTIMEOUT:
            zpagechk(pport, PREADY, PRESET, 500, 0, EscP);
            if(pstate[pport] == PREADY)
				{
				//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - PNOTREADYTIMEOUT.");
				GetLocalTime(&starttime[pport]);
				}
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            countA[pport] = 0;

			// "+++ATH" could come from the rcc paging terminal
			//  down the leased line, this is a hangup command
			//  to a hayes compatible dialup modem, but since 
			//  this is a leased line and not a dialup connection, 
			//  the page server may see this command.
			// the sensible thing to do if this is detected is
			//   to recycle the port, and start a new connection.
			//
            zpagechk(pport, PRESET4, PREADY, 500, 1, "ATH");

			// 2002-10-17 added this for MASCO / ARCH leased line, 
			//  they don't send the larger "DISCONNECT" message
			//
            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET4, 1, Eot);
				}

			// 2002-10-18 added this for MASCO / ARCH leased line, 
			//  respond to Negative Acknowledge by signing off, 
			//  then sign on again.
			//
            if(pstate[pport] == PREADY)
				{
                CheckBuffer(pport, PRESET1, 1, Nack2);
				}

            if(pstate[pport] == PREADY)
                CheckBuffer(pport, PRESET4, 1, "Good Bye");

            if(pstate[pport] == PREADY)
                CheckBuffer(pport, PRESET4, 1, "PAGES SENT");

            if(pstate[pport] == PREADY)
                CheckBuffer(pport, PRESET4, 1, "DISCONNECT");

            if(pstate[pport] == PREADY)
                CheckBuffer(pport, PRESET4, 1, "TERMINAL FAILED");

			//when "ID=" comes, we are not synchronized nicely, but
			//  advance to PNOTREADY where "<esc>PG1<cr>" will be sent.
			if(pstate[pport] == PREADY)
                CheckBuffer( pport, PNOTREADY, 1, "ID=");

			//when "Thank you" comes, 
		    //  (ARCH communications may send this, 2001-07-10)
			//  advance to PRESET4 to sign on again.
			if(pstate[pport] == PREADY)
                CheckBuffer(pport, PRESET4, 1, "Thank you");

            break;

        case PSEND:
			CalculateTAPChecksum(pcapcode[pport], pmessage[pport], Checksum);

 			// build up the message piece by piece.
            memset(temp, 0, sizeof(temp));

			// prepare the TAP message
            strcat(temp, "\x02");
            strcat(temp, pcapcode[pport]);
            strcat(temp, "\x0d");
            strcat(temp, pmessage[pport]);
            strcat(temp, "\x0d");
            strcat(temp, "\x03");			
            strcat(temp, Checksum);
            strcat(temp, "\x0d");

			// send the message to the serial port.
            WritePort(pport, 1, temp);
            memset(temp, 0, sizeof(temp));

            pstate[pport] = PPAGINGTIMEOUT;
            break;

        case PPAGINGTIMEOUT:
			// data block sent... now expecting ACK.
			xpagechk(pport, PREADY, PRESET4, 10 * 100, Ack2);

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
                CheckBuffer(pport, PREADY, 1, "Page OK");
				}

            if( pstate[pport] == PPAGINGTIMEOUT )
                CheckBuffer(pport, PRESET4, 1, Nack2 );

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
				// if <RS> (abandon) (0x1e) is received, max out the error count.
                CheckBuffer(pport, PRESET1, 1, abandon2);

				if(pstate[pport] == PRESET1)
					{
					// they don't have the requested pager number in their database.
					//  no sense to try again, set the reset count to 
					//  the maximum, then enter PRESET1 state.
					ComPort[pport].reset = param.ComPort[pport].Retry + 1;
					}
				}

			if(pstate[pport] == PPAGINGTIMEOUT)
                CheckBuffer( pport, PRESET4, 1, "ATH");

            if(pstate[pport] == PPAGINGTIMEOUT)
                CheckBuffer( pport, PRESET4, 1, "ID=");

            if(pstate[pport] == PPAGINGTIMEOUT)
                CheckBuffer( pport, PRESET4, 1, "Good Bye");

            if(pstate[pport] == PPAGINGTIMEOUT)
                CheckBuffer( pport, PRESET4, 1, "Invalid");

            if(pstate[pport] == PPAGINGTIMEOUT)
                CheckBuffer( pport, PRESET, 1, "Bad Line");
            
 			if(pstate[pport] == PRESET || 
				pstate[pport] == PRESET4 || 
				pstate[pport] == PRESET1)
				{
				// things aren't going so well, save the error response
				//  so that it can be reported.
				pageerror[pport] = PPAGINGTIMEOUT;

				memset(ComPort[pport].errorResponse, 
							0, 
							sizeof(ComPort[pport].errorResponse));

				sprintf_s<sizeof(ComPort[0].errorResponse)>(ComPort[pport].errorResponse, 
						"%-80.80s", holdbuff[pport]);

				Remove_CtrlChars(ComPort[pport].errorResponse, 
									sizeof(ComPort[pport].errorResponse));

				Set_Null(ComPort[pport].errorResponse, 
							sizeof(ComPort[pport].errorResponse));

				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"COM%d:, Response [%s]", 
								pport + 1, 
								ComPort[pport].errorResponse);
				}
           break;

        default:
            break;
		}
	}


	bool 
IsPageCharOk(char c)
	{
	return (((c >= 0x20) && (c <= 0x7E)) || (c == 0x0A));
	}




	bool PageTwo(int iport)
	{
		char SSSstring[20];
		bool IsCoveredBy = false;

		int i, k;
		PAGE_STRUCT* pagePtr = &page[iport + PAGE_COM_START];

		memset(SSSstring, 0, sizeof(SSSstring));
		//sprintf(SSSstring, "%%%s", page[iport + PAGE_COM_START].PageType);
		sprintf_s<20>(SSSstring, "%%%s", page[iport + PAGE_COM_START].PageType);

		//  We must get a TRUE to advance toward AddQueue ().
		//

		i = Get_Frq(iport + PAGE_COM_START, SSSstring, SSSstring);

		if(i == TRUE && pagePtr->ScriptNumber != -1)
		{
			if(strchr(param.PageType[pagePtr->ScriptNumber].Script, 'V') != NULL)
			{
				i = FALSE;
			}
		}

		if(i == TRUE)
		{
			k = strlen(param.PriorityPass);
			i = strlen(pmessage[iport]);

			if(i > k && pagePtr->Flag4[0] == 'Y')
			{
				if(strncmp(pmessage[iport] + i - k, param.PriorityPass, k) == 0)
				{
					if(pagePtr->PriorityQPointer < 9)
						pagePtr->PriorityQPointer++;
				}
			}

			/// xxx
			pmessage[iport][147] = 0;
			k = 0;

			for(i = 0; i < (int) strlen(pmessage[iport]); i++)
			{
				// counting message characters to be in the range of 0 thru 9
				if(pmessage[iport][i] >= '0' && pmessage[iport][i] <= '9')
				{
					pagePtr->DisplayInfo[k] = pmessage[iport][i];
					k++;
				}

				// counting specific characters in the message <minus> and <space>.
				if((pmessage[iport][i] == '-' || pmessage[iport][i] == ' ') && pagePtr->ComNumber >= 0)
				{
					pagePtr->DisplayInfo[k] = pmessage[iport][i];
					k++;
				}
			}

			// recognize input from a comm/serial port.
			if(pagePtr->ComNumber >= 0)
			{
				if(param.ComPort[pagePtr->ComNumber].QueType[pagePtr->ComOffset][1] == '1')
				{
					memset(pagePtr->DisplayInfo, 0, sizeof(page[0].DisplayInfo));
					//strcpy(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
					strcpy_s<sizeof(page[0].DisplayInfo)>(pagePtr->DisplayInfo, pmessage[iport]);
				}
			}

			// something about "list" paging...
			if(pagePtr->PageStatus[0] == 'L')
			{
				memset(pagePtr->DisplayInfo, 0, sizeof(page[0].DisplayInfo));
				//strcpy(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
				strcpy_s<sizeof(page[0].DisplayInfo)>(pagePtr->DisplayInfo, pmessage[iport]);
			}

			// something about "multi-queue" paging...
			if(param.MultiQueNumber2 > 0)
			{
				if(strncmp(pagePtr->PageType, Queue[param.MultiQueNumber2].Id + 1, 2) == 0)
				{
					memset(pagePtr->DisplayInfo, 0, sizeof(page[0].DisplayInfo));
					//strcpy(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
					strcpy_s<sizeof(page[0].DisplayInfo)>(pagePtr->DisplayInfo, pmessage[iport]);
				}
			}

			memset(SSSstring, 0, sizeof(SSSstring));
			//sprintf(SSSstring, "IXO P %i", iport + 1);
			sprintf_s<sizeof(SSSstring)>(SSSstring, "IXO P %i", iport + 1);

			AddSetUp(iport + PAGE_COM_START, SSSstring);

			pagePtr->PriorityQPointer = 0;
			pagePtr->PortNumber = GetQuePointer(iport + PAGE_COM_START);


			if(IXO_DISCARD == pageactive[iport])
			{
				// no AddQueue () is required... NUL protocol swallows incoming IXO type
				//  page requests, quietly.
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "IXO_DISCARD - request accepted, and discarded");
			}
			else
			{
				// other input protocols e.g. IXO_ACCEPT, IXO_ACCEPT_DIRECT, IXX_DIRECT, will "write" to queues.

				AddQueueEx(iport + PAGE_COM_START, pagePtr->PortNumber);
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "this page came from Tap In port.................");
			}

			// check the "enable" option... if TRUE, log a message from IXO, or NUL port (Tap In).
			PrintMessage(MSGTYPE_SYSTEM,
				hwndComMsg, 
				"WriteTapInMessagesToMessageBase true/false value: [%d] [%s]", 
				iport,
				param.WriteTapInMessagesToMessageBase[iport]);

			if(param.WriteTapInMessagesToMessageBase[iport][0] == 'T')		// T means TRUE
			{
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "writing a message to the Spok MessageBase.................");

				WriteUserLogEx(iport + PAGE_COM_START, pagePtr->PortNumber);
			}
			else
			{
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "NO message written to the Spok MessageBase.................");
			}


			//////////////

			{
				return true;	
			}
			//return true;
		}
		else
		{
			PrintMessage(MSGTYPE_SYSTEM,  
				hwndComMsg, 
				"Extension [%s] no pager currently assigned. Check frq file - 2.", 
				page[iport + PAGE_COM_START].Extension);
			return false;
		}

		return true;
	}












		//string profileId = "  W7912213";
		//string date = "2013010318:45";

	//return an on call list

	bool ProcessOnCall(const string& profileId, const string& date, int iport)
	{
		bool writeList = false;

		auto_ptr<AdoWrapConnection> connPtr(AdoWrapConnection::CreateConnection(30, 30, globalDbConnectionStr.c_str(), "", ""));

		auto_ptr<AdoWrapCommand> cmdPtr(new AdoWrapCommand(connPtr.get()));
		stringstream update;

		string profile;
		string period;
		string extension;

		update <<	"SELECT OnCall.ProfileId, Period, MemberExtension, T.PagerId, P.DefaultPager, DepartmentPager "
						"FROM OnCall "
						"INNER JOIN Xkm AS T ON OnCall.MemberExtension = T.ProfileId "
						"LEFT OUTER JOIN Pagers AS P ON T.PagerId = P.PagerId "
						"WHERE Period "
						"BETWEEN (SELECT TOP 1 Period FROM OnCall WHERE Period < '" <<  date << "' ORDER BY Period DESC) AND "
						"(SELECT TOP 1 Period FROM OnCall WHERE Period > '" << date << "' ORDER BY Period ASC) AND "
						"Period > '" << date <<"'" << ends;


		string sqlStatement = update.str();

		auto_ptr<AdoWrapRecordSet> recPtr(cmdPtr->Execute(sqlStatement));

			
		while(!recPtr->IsEOF())
		{
			writeList = true;
			string profile;
			string period;
			string extension;
			string pagerId;
			string defaultPager;

			recPtr->GetFieldValue(0, profile);
			recPtr->GetFieldValue(1, period);
			recPtr->GetFieldValue(2, extension);
			recPtr->GetFieldValue(3, pagerId);
			recPtr->GetFieldValue(4, defaultPager);

			profile = trim(profile);
			pagerId = trim(pagerId);
			extension = trim(extension);

			strcpy_s<11>(page[iport + PAGE_COM_START].Extension, extension.c_str());
			strcpy_s<11>(page[iport + PAGE_COM_START].PagerId, pagerId.c_str());
			strcpy_s<3>(page[iport + PAGE_COM_START].PageType, defaultPager.substr(1).c_str());


			//clear out on call
			page[iport + PAGE_COM_START].FileName[0] = 0;
			PageTwo(iport);


			recPtr->MoveNext();
		}
		return writeList;
	}

	bool DoDatabaseWork(int iport)
	{
	// return true, if the PagerId is known, and the page should be accepted for delivery.
	//  otherwise, return false.
	//
	int i;
    int k;
    char SSSstring[20];

	// for any incoming page request from a TAP input port, 
	//  this should be initialized as follows, otherwise trouble in ParseFrq().
	// kgill 2009-04-16

	page[iport + PAGE_COM_START].ScriptNumber = -1;
	page[iport + PAGE_COM_START].PageStatus[0] = 'I';

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndComMsg,
					"Profile Id:[%s] Message:[%s], CheckSum:[%s]",
					pcapcode[iport],
					pmessage[iport],
					pstatus[iport]);

	memset(page[iport + PAGE_COM_START].Extension,   0, sizeof(page[iport + PAGE_COM_START].Extension));
	memset(page[iport + PAGE_COM_START].PagerNumber, 0, sizeof(page[iport + PAGE_COM_START].PagerNumber));
	memset(page[iport + PAGE_COM_START].DisplayInfo, 0, sizeof(page[iport + PAGE_COM_START].DisplayInfo));
	memset(page[iport + PAGE_COM_START].AutCode,     0, sizeof(page[iport + PAGE_COM_START].AutCode));
	memset(page[iport + PAGE_COM_START].PageStatus,  0, sizeof(page[iport + PAGE_COM_START].PageStatus));

	{
	// for compatibility with Commtech...
	//  they may send leading spaces on the pagerid, and they can send 11 chars.
	//  simply trim off any leading spaces, and cut to a maximum of 10 characters.
	//  later, this will be padded on the left as necessary before database lookup.
	//

	CString csTemp; csTemp = pcapcode[iport];

	csTemp.TrimLeft();
	csTemp.TrimRight();

	// put back to 'C' character array variable.
			//sprintf(pcapcode[iport], "%s", (LPCTSTR) csTemp);
			sprintf_s<sizeof(pcapcode[0])>(pcapcode[iport], "%s", (LPCTSTR) csTemp);
	}

	// extra careful, or superstition.
	pcapcode[iport][10] = 0;

		//strcpy(page[iport + PAGE_COM_START].Extension, pcapcode[iport]);
		strcpy_s<sizeof(page[0].Extension)>(page[iport + PAGE_COM_START].Extension, pcapcode[iport]);
		//strncpy(page[iport + PAGE_COM_START].PagerId, page[iport + PAGE_COM_START].Extension, 10);
		strncpy_s<sizeof(page[0].PagerId)>(page[iport + PAGE_COM_START].PagerId, page[iport + PAGE_COM_START].Extension, 10);


	// step0 -
	//
	// the first-step lookup method is "lookup by Pagers:PagerId" by default.
	//
	int Method = BY_PAGERID;

	if(IXX_DIRECT == pageactive[iport])
		{
		// if protocol is IXX, find the PagerId as received by the 
		//  TAP IN port in the Xkm:PatientId table.
		//
		// lookup method should be lookup by Xkm:PatientId.
		Method = BY_PATIENTID;
		}

	// step1 -
	//
	if(GetPagerId(page[iport + PAGE_COM_START].Extension, iport + PAGE_COM_START, Method))
		{
		// PagerId was found, continue with the evaluation.
		}
	else
		{
		// PagerId could not be found, this request should not be accepted.
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg, 
						"2-Pager id [%s] not found", 
						page[iport + PAGE_COM_START].PagerId);
		return false;	
		}


	// step2 - get misc database items for this profile.
		//
		//if(LogPager(page[iport + PAGE_COM_START].Extension, iport + PAGE_COM_START) == TRUE)
		//{
		//	// additional database items retrieved. Flags, Password, etc.
		//	i = TRUE;
		//}
		//else
		//{
		//	// couldn't get database items.
		//	i = FALSE;
		//}


		i = LogPager(page[iport + PAGE_COM_START].Extension, iport + PAGE_COM_START);

	// step3 - check for coverage.
	if(i == TRUE)
		{
			// COVERED BY ON CALL
		// check the content of the FileName field, this is a general purpose variable.
		//
		if(page[ iport + PAGE_COM_START].FileName[0] != 0)
			{
				//JPH 01/07/13 --  ADDED on call support
				//check for on call
				if(strstr(page[ iport + PAGE_COM_START].FileName, "ON CALL") != NULL)
				{
					//long dummy1;	// I don't even ask anymore...
					//char dummy2;	// don't want to know the answer

					//GetFirstOnCall(&dummy1, page[ iport + PAGE_COM_START].Extension, &dummy2);

					string profileId = page[iport + PAGE_COM_START].Extension;					
					SYSTEMTIME CurrTime;
	
					GetLocalTime(&CurrTime);
					char string2[200];
					sprintf_s<200>(string2,
						"%04.4i%02.2i%02.2i%02.2i:%02.2i",
						CurrTime.wYear,
						CurrTime.wMonth,
						CurrTime.wDay,
						CurrTime.wHour,
						CurrTime.wMinute);

					if(ProcessOnCall(profileId, string2, iport))
					{
						//DONE, exit
						return true;
					}
					i = FALSE;

				}
				else
				{
			// check for coverage.
			memset(page[iport + PAGE_COM_START].PagerNumber, 0, sizeof(page[0].PagerNumber));
			//format the pagerID right justifiy
			sprintf_s<sizeof(page[0].PagerNumber)>(page[iport + PAGE_COM_START].PagerNumber, "%10.10s", page[iport + PAGE_COM_START].Extension);

			//get profile id into filename
			i = CoverDoubleCheck(iport + PAGE_COM_START);
			
			if( i )
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"%s covered by extension %s ", 
								page[iport + PAGE_COM_START].Extension, 
								page[PAGE_COV_POS].Extension);

				memset(page[iport + PAGE_COM_START].Extension, 0, sizeof(page[iport + PAGE_COM_START].Extension));
				//strcpy(page[iport + PAGE_COM_START].Extension, page[PAGE_COV_POS].Extension);
				strcpy_s<sizeof(page[0].Extension)>(page[iport + PAGE_COM_START].Extension, page[PAGE_COV_POS].Extension);

				i = GetPagerId(page[iport + PAGE_COM_START].Extension, iport + PAGE_COM_START, BY_PROFILEID);

				if(i == FALSE)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndComMsg, 
									"3-Pager id %s not found", 
									page[iport + PAGE_COM_START].PagerId);
					return false;	
					}

						//i = FALSE;

						//if(LogPager(page[iport + PAGE_COM_START].Extension, iport + PAGE_COM_START) == TRUE)
						//{
						//	i = TRUE;
						//}

						i = LogPager(page[iport + PAGE_COM_START].Extension, iport + PAGE_COM_START);
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"%s Recursively covered", 
								page[iport + PAGE_COM_START].PagerId);
				}
				}
			}
		}

	// step 4 - check if pager assigned 
	//
	if(i == FALSE)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg, 
						"Extension [%s] pager not assigned, check log file 1.", 
						page[iport + PAGE_COM_START].Extension);
		return false;
		}					
	else
		{
        //check pager and its coverage. Everything looks good so far, moving on

		//DE27916 - SO - 11/13/2018 - Added the following code to fix the issue where page status is ignored 
		//when page sent using TAP protocol like Messenger Application
		if (Respect_Paging_Status)
		{
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
				" DoDatabaseWork(): Respect_Paging_Status is set to TRUE in the Xapgsrv.ini file. ");
			
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg,
				" DoDatabaseWork(): Page Status for this profile is [%c]",
				Status[page[iport + PAGE_COM_START].ACKMin].Page[0]);

			if (page[iport + PAGE_COM_START].ACKMin != -1 && page[iport + PAGE_COM_START].ACKMin <=29 && (Status[page[iport + PAGE_COM_START].ACKMin].Page[0]!='Y'))
			{
				
				PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg,
					" DoDatabaseWork(): This profile won't be paged since page status is [%c] and Respect_Paging_Status is set to TRUE. ",
					Status[page[iport + PAGE_COM_START].ACKMin].Page[0]);

				WriteMessage((iport + PAGE_COM_START), page[iport + PAGE_COM_START].Extension, "PG", "This profile will not be paged since page status is not set to available and Respect_Paging_Status is set in the configuration.", "");
				
				return false;
			}
		}
				
		memset(SSSstring, 0, sizeof(SSSstring));
			//sprintf(SSSstring, "%%%s", page[iport + PAGE_COM_START].PageType);
			sprintf_s<20>(SSSstring, "%%%s", page[iport + PAGE_COM_START].PageType);

		//  We must get a TRUE to advance toward AddQueue ().
		//
		i = Get_Frq(iport + PAGE_COM_START, SSSstring, SSSstring);

		if(i == TRUE && page[iport + PAGE_COM_START].ScriptNumber != -1)
			{
			if(strchr(param.PageType[page[iport + PAGE_COM_START].ScriptNumber].Script, 'V') != NULL)
				{
				i = FALSE;
				}
			}

		if(i == TRUE)
			{
			k = strlen(param.PriorityPass);
			i = strlen(pmessage[iport]);

			if(i > k && page[iport + PAGE_COM_START].Flag4[0] == 'Y')
				{
				if(strncmp(pmessage[iport] + i - k, param.PriorityPass, k) == 0)
					{
					if(page[iport + PAGE_COM_START].PriorityQPointer < 9)
						page[iport + PAGE_COM_START].PriorityQPointer++;
					}
				}

			/// xxx
			pmessage[iport][147] = 0;
			k = 0;

			for(i = 0; i < (int) strlen(pmessage[iport]); i++)
				{
				// counting message characters to be in the range of 0 thru 9
				if(pmessage[iport][i] >= '0' && pmessage[iport][i] <= '9')
					{
					page[iport + PAGE_COM_START].DisplayInfo[k] = pmessage[iport][i];
					k++;
					}

				// counting specific characters in the message <minus> and <space>.
				if((pmessage[iport][i] == '-' || pmessage[iport][i] == ' ') && page[iport + PAGE_COM_START].ComNumber >= 0)
					{
					page[iport + PAGE_COM_START].DisplayInfo[k] = pmessage[iport][i];
					k++;
					}
				}

			// recognize input from a comm/serial port.
			if(page[iport + PAGE_COM_START].ComNumber >= 0)
				{
				if(param.ComPort[page[iport + PAGE_COM_START].ComNumber].QueType[page[iport + PAGE_COM_START].ComOffset][1] == '1')
					{
						memset(page[iport + PAGE_COM_START].DisplayInfo, 0, sizeof(page[iport + PAGE_COM_START].DisplayInfo));
						//strcpy(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
						strcpy_s<sizeof(page[0].DisplayInfo)>(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
					}
				}

			// something about "list" paging...
			if(page[iport + PAGE_COM_START].PageStatus[0] == 'L')
				{
					memset(page[iport + PAGE_COM_START].DisplayInfo, 0, sizeof(page[iport + PAGE_COM_START].DisplayInfo));
					//strcpy(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
					strcpy_s<sizeof(page[0].DisplayInfo)>(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
				}

			// something about "multi-queue" paging...
			if(param.MultiQueNumber2 > 0)
				{
				if(strncmp(page[iport + PAGE_COM_START].PageType, Queue[param.MultiQueNumber2].Id + 1, 2) == 0)
					{
						memset(page[iport + PAGE_COM_START].DisplayInfo, 0, sizeof(page[iport + PAGE_COM_START].DisplayInfo));
						//strcpy(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
						strcpy_s<sizeof(page[0].DisplayInfo)>(page[iport + PAGE_COM_START].DisplayInfo, pmessage[iport]);
					}
				}

			memset(SSSstring, 0, sizeof(SSSstring));
				//sprintf(SSSstring, "IXO P %i", iport + 1);
				sprintf_s<sizeof(SSSstring)>(SSSstring, "IXO P %i", iport + 1);

			AddSetUp(iport + PAGE_COM_START, SSSstring);

			page[iport + PAGE_COM_START].PriorityQPointer = 0;
			page[iport + PAGE_COM_START].PortNumber = GetQuePointer(iport + PAGE_COM_START);


			if(IXO_DISCARD == pageactive[iport])
				{
				// no AddQueue () is required... NUL protocol swallows incoming IXO type
				//  page requests, quietly.
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "IXO_DISCARD - request accepted, and discarded");
				}
			else
				{
				// other input protocols e.g. IXO_ACCEPT, IXO_ACCEPT_DIRECT, IXX_DIRECT, will "write" to queues.

				AddQueueEx(iport + PAGE_COM_START, page[iport + PAGE_COM_START].PortNumber);
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "this page came from Tap In port.................");
				}

			// check the "enable" option... if TRUE, log a message from IXO, or NUL port (Tap In).
			PrintMessage(MSGTYPE_SYSTEM,
							hwndComMsg, 
							"WriteTapInMessagesToMessageBase true/false value: [%d] [%s]", 
							iport,
							param.WriteTapInMessagesToMessageBase[iport]);

			if(param.WriteTapInMessagesToMessageBase[iport][0] == 'T')		// T means TRUE
				{
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "writing a message to the Spok MessageBase.................");

				WriteUserLogEx(iport + PAGE_COM_START, page[iport + PAGE_COM_START].PortNumber);
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "NO message written to the Spok MessageBase.................");
				}				
			return true;

			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndComMsg, 
							"Extension [%s] no pager currently assigned. Check frq file - 2.", 
							page[iport + PAGE_COM_START].Extension);
			return false;
			}
		}
	}


	bool 
CheckTheChecksum(char * BlockText, char * ChecksumText)
	{
	// at entry, block is a null terminated character array, 
	//  containing the contents of a TAP protocol block.
	//  the checksum is a pointer to the received checksum characters. 
	//  only the first 3 need to be examined.
	unsigned int Sum = 0;

	for(int i = 0; (BlockText[i] != 0); i++)
		{
		Sum += (BlockText[i] & 0x7f);
		}

	if( (0x30 + ((Sum >> 8) & 0xf) == ChecksumText[0]) && 
		(0x30 + ((Sum >> 4) & 0xf) == ChecksumText[1]) && 
		(0x30 + ((Sum >> 0) & 0xf) == ChecksumText[2]))
		{
		return true;
		}
	else
		{
		return false;
		}
	}
	
	// SOguz - US31685 - I changed the method to use Replace 
	// function. This method replaces whitespaces in the message
	// with single space.  See US31685 for more info.
	CString 
ReduceMultipleSpaces(CString SourceText)
	{
		while(SourceText.Replace("  "," "));
		return SourceText;
	}

// Commented out by SOguz because "SetAt" function is no longer supported in current MFC
/************************************************************************************
//	CString 
//ReduceMultipleSpaces(CString SourceText)
//	{
//	// reduce multiple adjacent <space> characters to a single <space> character.
//	CString TargetText = "";
//	
//	// The most straight forward method is to iterate the whole string. 
//	//  examine the current character and the previous one,
//	//  don't copy if the previous char WAS a <space>,
//	//  AND the current char IS a <space>.
//	//
//	int TargetIndex = 0;
//	char PreviousChar;
//	
//	PreviousChar = 'A';	// initialize "previous" to any non-space character.
//
//	for(int SourceIndex = 0; SourceIndex < SourceText.GetLength(); SourceIndex++)
//		{
//		if((' ' == PreviousChar) && (' ' == SourceText[SourceIndex]))
//			{
//			// don't copy the adjacent <space> character.
//			}
//		else
//			{
//			// transfer source character to target, 
//			//  only if the previous character was not a <space>.
//			TargetText.SetAt(TargetIndex++, SourceText[SourceIndex]);
//			TargetText.SetAt(TargetIndex, (char) 0);
//
//			PreviousChar = char(SourceText[SourceIndex]);
//			}
//		}
//
//	return TargetText;
//	}
//
*******************************************************************************************/
	void
FlushHoldBuffer(int pport)
	{
	ZeroMemory(holdbuff[pport], sizeof(holdbuff[0]));
	ccount[pport] = 0; 
	}


	// IXO Accept notes:
	//  Better to use the "E0" - command echo option of AT command set, 
	//  we don't want the "Connect 1200" type of messages in the data stream. 
	//  Don't forget to use the S0=1 - auto answer.
	//
	void
Ixo_Accept_Direct(void)
	{
    switch(pstate[pport])
		{
		case ST_DISCONNECT_AND_START_OVER:	// this is a better name for this state.
		case PRESET:						// when pageserver starts, PRESET is the 
			{								// startup state, don't delete this.
			FlushHoldBuffer(pport);
			WritePort(pport, 1, "Good Bye\r");

			pstate[pport] = P_DELAY_BEFORE_DISCONNECT;			
			GetLocalTime(&starttime[pport]);		// initialize the time reference.
			}
			break;

		case P_DELAY_BEFORE_DISCONNECT:
			zpagechk(pport, P_DISCONNECT_SEQUENCE, P_DISCONNECT_SEQUENCE, (2 * 100), 0, "JOE");
			break;

		case P_DISCONNECT_SEQUENCE:
			WritePort(pport, 1, EscEotCr);
			pstate[pport] = P_DELAY_BEFORE_DTR_LOW;
			break;

		case P_DELAY_BEFORE_DTR_LOW:
			zpagechk(pport, PDTRLOW, PDTRLOW, (2 * 100), 0, "JOE");
			break;

        case PDTRLOW:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
			zpagechk(pport, PDTRHIGH, PDTRHIGH, (2 * 100), 0, "JOE");
			break;

        case PDTRHIGH:
			SetDTR(pport, TRUE);

			// check for empty initialization string
			if(param.ComPort[pport].Init1[0] == 0)
				{
				GetLocalTime(&starttime[pport]);

				pstate[pport] = PREADY;
				}
			else
				{
				// proceed with steps to initialize the modem.
	            pstate[pport] = PRESETJ;
				}
           break;

		// init modem steps are: PRESETJ, PRESETA, PRESET1, PRESET2, PRESET3, 
		//  PRESET4, PRESET5, PRESET6, PRESETB, PRESETC, and PRESETD.
		case PRESETJ:
			// a small delay between DTR signal going high, and sending <cr>.
			zpagechk(pport, PRESETA, PRESETA, 50, 0, "JOE");
			break;

        case PRESETA:
            WritePort(pport, 1, cr);
			pstate[pport] = PRESET1;
            break;

        case PRESET1:
            zpagechk(pport, PRESET2, PRESET2, (2 * 100), 0, "JOE");
            break;

        case PRESET2:
            WritePort(pport, 1, "+++");
            pstate[pport] = PRESET3;
            break;

        case PRESET3:
            zpagechk(pport, PRESET4, PRESET4, (2 * 100), 0, "JOE");
            break;

        case PRESET4:
            WritePort(pport, 1, ModemOnHk);
            pstate[pport] = PRESET5;
			break;

        case PRESET5:
			zpagechk(pport, PRESET6, PRESET6, (3 * 100), 0, "OK");	
            break;

        case PRESET6:
            WritePort(pport, 1, param.ComPort[pport].Init1);
            WritePort(pport, 1, cr);
            pstate[pport] = PRESETB;
            break;

        case PRESETB:
            zpagechk(pport, PRESETC, PRESETC, 150, 1, "OK");	
            break;

        case PRESETC:
            WritePort(pport, 1, param.ComPort[pport].Init2);
            WritePort(pport, 1, cr);
            pstate[pport] = PRESETD;
            break;

        case PRESETD:
			// continue at PREADY when modem says: "OK".
            zpagechk(pport, PREADY, PREADY, (5 * 100), 1, "OK");

			if(PRESETD != pstate[pport])
				{
				// init steps before entering PREADY.

				// flush any characters from the input stream, 
				//  this could be the echo of the modem initialization string, 
				//  which ends in a <cr>, and coming from the modem could be
				//  mis-interpreted as a <cr> from the remote application, 
				//  to which we will respond too early with "ID=<cr><lf>"
				//  invitation to log on.
				//
				// flush the input buffer.
				FlushHoldBuffer(pport);
				GetLocalTime(&starttime[pport]);
				}
			break;

		// PREADY is really the "idle" state, waiting for the first incoming <cr>.
		//  this state now has a timeout.
		//
		// prepare a long duration timeout before entering the PREADY state.
		//  this state should timeout at some point. For instance, if the modem
		//  never hangs up, and never sends any recognizable characters, 
		//  the port would be "hung" until application restart.

        case PREADY:
            ComPort[pport].reset = 0;

			// the following zpagechk() is just a timeout that will re-cycle the port
			//  if this state is not exited in a long while. 

			zpagechk(pport, PREADY, ST_DISCONNECT_AND_START_OVER, (1 * 60 * 100), 0, "JOE");
 
			// respond to cr if received, GOOD.		
            CheckBuffer(pport, ST_BEGIN_SIGNON, 1, cr);

			// respond to EOT if received, BAD.	
			CheckBuffer(pport, ST_DISCONNECT_AND_START_OVER, 1, Eot);
            break;

		// after seeing a <CR>, begin signon sequence, by sending "ID="
        case ST_BEGIN_SIGNON:
			WritePort(pport, 1, IDEQ);
            pstate[pport] = ST_WAIT_FOR_LOGIN;

			GetLocalTime(&starttime[pport]);
            break;

        case ST_WAIT_FOR_LOGIN:
			// expect the TAP "login sequence".
 			pagerLoginCheck(pport, ST_SEND_GOAHEAD, PREADY, (10 * 100));

			// respond to EOT if received.
            CheckBuffer(pport, ST_DISCONNECT_AND_START_OVER, 0, Eot);

  			if(ST_WAIT_FOR_LOGIN != pstate[pport])
				{
 				GetLocalTime(&starttime[pport]);

				if(ST_SEND_GOAHEAD == pstate[pport])
					{
					// bytes received have been interpreted, set count to 0, 
					//  so that old bytes will not be used again.
 					ccounter[pport]=0;
					}
				}
           break;

        case ST_SEND_GOAHEAD:
			// send the "go ahead" sequence: "0x06, 0x0d, 0x1b, '[',  'p',  0x0d, 0" 
			WritePort(pport, 1, EscPCr);
			pstate[pport] = ST_BEGIN_TAP_MESSAGE;

			GetLocalTime(&starttime[pport]);
			break;

		case ST_BEGIN_TAP_MESSAGE:
			// clear out some data items at the beginning of TAP message receive process.
			//
 			ZeroMemory(pcapcode[pport], sizeof(pcapcode[0]));
			ComPort[pport].reset = 0;

			TapMessageText[pport].Empty();

			// don't send any ACK, this is the first time thru.
			pstate[pport] = ST_GET_NEXT_TAP_BLOCK;
			GetLocalTime(&starttime[pport]);
			break;

		case ST_ACK_AND_GET_NEXT_BLOCK:
 			// good checksum on the block just received, but this is NOT the final block. 
            WritePort(pport, 1, CrAckCr);	
            pstate[pport] = ST_GET_NEXT_TAP_BLOCK;
 
			GetLocalTime(&starttime[pport]);

			PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Good Tap Block, not the last block, sending Ack sequence.");
			break;


		case ST_GET_NEXT_TAP_BLOCK:
			// state machine collection point, 
			//  advance to check error counts, etc.

			// initialize items that must be cleared for starting a new block.
			ZeroMemory(holdbuff[pport], sizeof(holdbuff[0]));
			ZeroMemory(BlockText[pport], sizeof(BlockText[0]));
			ZeroMemory(ChecksumText[pport], sizeof(ChecksumText[0]));

			ZeroMemory(pmessage[pport], sizeof(pmessage[0]));
			ZeroMemory(pstatus[pport], sizeof(pstatus[0]));

			pstate[pport] = ST_CHECK_ERROR_COUNTS;
			GetLocalTime(&starttime[pport]);
 			break;

 		case ST_CHECK_ERROR_COUNTS:
			// if error counts are too high, signoff, hangup, recycle.
			//  error counts on input are used for rejecting too many 
			//  tries with bad checksum.
		
			if(ComPort[pport].reset > param.ComPort[pport].Retry)	// better logic
				{
				pstate[pport] = ST_DISCONNECT_AND_START_OVER;
				}
			 else
				{
				pstate[pport] = ST_WAIT_FOR_BLOCK_BEGIN;
		
				// bytes received have been interpreted, set count to 0, 
				//  so that old bytes will not be used again.
 				ccounter[pport]=0;
				}
		
			GetLocalTime(&starttime[pport]);
			break;

		case ST_WAIT_FOR_BLOCK_BEGIN:
			// the only thing I want to see is STX, 
			//  I will wait a reasonable time for the next block, before timeout, and expect a new signon.
			//
            BlockBeginPageCheck(pport, ST_WAIT_FOR_BLOCK_END, ST_DISCONNECT_AND_START_OVER, (20 * 100));	

			// respond to EOT if received.
            CheckBuffer(pport, ST_DISCONNECT_AND_START_OVER, 0, Eot);

			// when stx is found.
			if(ST_WAIT_FOR_BLOCK_BEGIN != pstate[pport])
				{
				// start timer.
  				GetLocalTime(&starttime[pport]);
				}
            break;

		case ST_WAIT_FOR_BLOCK_END:
			{
			// collect all characters up-to the ETX, ETB, US.
			BlockEndPageCheck(pport, ST_WAIT_FOR_CHECKSUM, ST_DISCONNECT_AND_START_OVER, (15 * 100));

			// check for the unexpected error character sequence.
			//  if it comes, we will recover quickly.
            CheckBuffer(pport, ST_DISCONNECT_AND_START_OVER, 0, Eot);

			// when ETX, ETB, US. is found.
			if(ST_WAIT_FOR_BLOCK_END != pstate[pport])
				{
				if(ST_WAIT_FOR_CHECKSUM == pstate[pport])
					{
					// proceeding as expected...

					// store received information in yet another buffer 
					//  for future analysis.
					for(size_t i = 0; i < strlen(holdbuff[pport]); i++)
						{
						BlockText[pport][i] = holdbuff[pport][i];
						}

					// clear out buffer contents, in preparation to receive checksum.
					ccounter[pport] = 0;
					memset(holdbuff[pport], 0,  RXBUFFLEN);
					}

				// start timer.
  				GetLocalTime(&starttime[pport]);
				}
			}
			break;

		case ST_WAIT_FOR_CHECKSUM:
			// collect all characters up-to the cr.
			xpagechk(pport, ST_ANALYZE_CHECKSUM, ST_DISCONNECT_AND_START_OVER, (3 * 100), cr);

			// check for the unexpected error character sequence.
			//  if it comes, we will recover quickly.
            CheckBuffer(pport, ST_DISCONNECT_AND_START_OVER, 0, Eot);

			// when CR is found.
			if(ST_WAIT_FOR_CHECKSUM != pstate[pport])
				{
				if(ST_ANALYZE_CHECKSUM == pstate[pport])
					{
					// proceeding as expected...

					// store received information in yet another buffer 
					//  for future analysis.
					for(int i = 0; i < (int)strlen(holdbuff[pport]); i++)
						{
						ChecksumText[pport][i] = holdbuff[pport][i];
						}

					// clear out buffer contents
					ccounter[pport] = 0;
					memset(holdbuff[pport], 0,  RXBUFFLEN);
					}
  				GetLocalTime(&starttime[pport]);
				}
			break;

		case ST_ANALYZE_CHECKSUM:
			// if checksum is ok, analyze the data received, 
			//  otherwise send "NAK".

			if(true == CheckTheChecksum(BlockText[pport], ChecksumText[pport]))
				{
				// checksum is ok.
				pstate[pport] = ST_ANALYZE_BLOCK;
  				GetLocalTime(&starttime[pport]);
				}
			else
				{
				// checksum is ng.
				pstate[pport] = ST_BAD_CHECKSUM;
  				GetLocalTime(&starttime[pport]);
				}
			break;

		case ST_ANALYZE_BLOCK:
			// 2 determinations are required.
			//  * is this the first block received - determined if PagerId is empty.
			//  * is this the final block - determined by reception of ETX character.
			//
			{
			// extract some information from the block to easier to use variables.
			//
			CString csPagerId, csMessage;

			// convert to more convenient CString.
			CString csBlockText; csBlockText = BlockText[pport];

			// extract content by cutting off the first, and last characters.
			//  these are the Start: (STX) and Terminator: (can be 1 of: ETX, ETB, or US).
			CString csBlockContent; csBlockContent = csBlockText.Mid(1, csBlockText.GetLength() - 2);

			if(0 == strlen(pcapcode[pport]))
				{
				// this is the first Tap block, containing the PagerId, and some or all of the MessageText.
				//  extract and store PagerId, and MessageText.
				//

				// between the 02 and the <cr> is the PagerId.
				// copy all after the <cr> up to but not including the block terminator as the MessageText.
				//
				int PositionOfCr = csBlockContent.Find("\r", 0);

                //SO - 4/8/2013 - xxxx - Check if <CR> is present
				csPagerId = csBlockContent.Mid(0, PositionOfCr);
				csMessage = csBlockContent.Mid(PositionOfCr + 1);

				// remove the <cr> at the end if it is found.
				csMessage.TrimRight("\r");

				// store the message text.
				TapMessageText[pport] = csMessage;

				// store the PagerId into the standard pcapcode field.
				//strcpy(pcapcode[pport], csPagerId);
				strcpy_s<sizeof(pcapcode[0])>(pcapcode[pport], csPagerId);
				}
			else
				{
				// this is a second or subsequent Tap block.
				//
				csMessage = csBlockContent;

				// remove the <cr> at the end if it is found.
				csMessage.TrimRight("\r");

				// append the message text.
				TapMessageText[pport] += csMessage;
				}

			// if the End-Of-Block character is ETX, the message is complete.
			//  otherwise get more blocks.

			int EndOfBlockCharValue = csBlockText.Right(1).GetAt(0);

			switch(EndOfBlockCharValue)
				{
				case 0x03:
					{
					// this is the final message block.

					{
					// MESSAGE FIXUP - BEGIN.
					PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Complete Unfiltered TapMessageText:[%s].", TapMessageText[pport]);

					// cleanup the message content once the 
					//  complete message has been received;

					//  remove leading and trailing spaces.
					TapMessageText[pport].TrimLeft();
					TapMessageText[pport].TrimRight();

					// SOguz - US31685 -  the following code removed by kgill on 2010-07-01
					// because the "SetAt" function in ReduceMultipleSpaces method is no  
					// longer supported with the new MFC.  I changed the method to use Replace 
					// function.  Please see ReduceMultipleSpaces for more info.
					TapMessageText[pport] = ReduceMultipleSpaces(TapMessageText[pport]);
					//

					PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Complete Filtered TapMessageText:[%s].", TapMessageText[pport]);
					// MESSAGE FIXUP - COMPLETE.
					}

					// put the page request content into the expected "interface" locations.
					// the message is expected in the STL longMessage variable.
					//
					page[PAGE_COM_START + pport].longMessage.assign((LPCTSTR) TapMessageText[pport]);
					PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "longmessage content:[%s].", page[PAGE_COM_START + pport].longMessage.c_str());
					pstate[pport] = ST_ANALYZE_TAP_MESSAGE;
					}
					break;

				case 0x17:
					// this is NOT the final message block, continue at Get-Another-Block.
					// Append to MessageText.
					pstate[pport] = ST_ACK_AND_GET_NEXT_BLOCK;
					break;

				case 0x1f:
					// this is NOT the final message block, continue at Get-Another-Block.
					// Append to MessageText.
					pstate[pport] = ST_ACK_AND_GET_NEXT_BLOCK;
					break;

				default:
					// this should never happen, but if it does, send an ACK and get on with the process.
					pstate[pport] = ST_ACK_AND_GET_NEXT_BLOCK;
					break;
				}
			}

  			GetLocalTime(&starttime[pport]);
			break;

		case ST_ANALYZE_TAP_MESSAGE:
			// do database work to determine if the PagerId is known, 
			//  if known, write the request to the page queue, and write all appropriate 
			//  messages to the database.

			// special feature for East Maine, and IXX protocol. 2009-12-10 kgill.
			//
			//  Parse the tap message extracting the PagerId from the Page Message Text.
			//  we are looking for the specific keyword text "Subject:<space>DDDD<space>
			//  where DDDD is the numeric PagerId to be looked up in the Xkm:PatientId
			//  field of the database. The DDDD is expected to be a number from 1 to 10 
			//  digits in length.
			//
			if(IXX_DIRECT == pageactive[pport])
				{
				// IXX protocol, do this extra parsing and extraction of the PagerId.
				CString csMessageBody = page[PAGE_COM_START + pport].longMessage.c_str();
				CString csTextToMatch = "Subject: ";

				int iKeyStart = csMessageBody.Find(csTextToMatch);

				if(iKeyStart != -1)
					{
					// keyword found, now look for a space char after that, 
					//  and extract the PagerId, and MesageText parts.
					int iLocationOfSpace = csMessageBody.Find(" ", iKeyStart + csTextToMatch.GetLength());
					int iStartOfPagerId = iKeyStart + csTextToMatch.GetLength();

					CString csPagerId = csMessageBody.Mid(iStartOfPagerId, (iLocationOfSpace - iStartOfPagerId));

					// limit the length of the PagerId to 10 characters or less.
					csPagerId = csPagerId.Right(10);

					CString csMessageText = csMessageBody.Mid(iLocationOfSpace + 1);

					// now put the extracted info back into the variables where it is 
					//  expected to be by the rest of the process.
					page[PAGE_COM_START + pport].longMessage.assign((LPCTSTR) csMessageText);
					//sprintf(pcapcode[pport], "%s", (LPCTSTR) csPagerId);					
					sprintf_s<sizeof(pcapcode[0])>(pcapcode[pport], "%s", (LPCTSTR) csPagerId);					
					}
				else
					{
					// keyword not found.
					//sprintf(page[PAGE_COM_START + pport].PagerId, "          ");
					sprintf_s<sizeof(page[0].PagerId)>(page[PAGE_COM_START + pport].PagerId, "          ");
					}				
				}

			if(DoDatabaseWork(pport))
				{
				// PagerId was found, and all database work was successful.
				pstate[pport] = ST_GOOD_PAGERID;
				}
			else
				{
				// PagerId was not found, can't accept the request.
				pstate[pport] = ST_BAD_PAGERID;
				}
 
			GetLocalTime(&starttime[pport]);
			break;

		case ST_GOOD_PAGERID:
 			// everything went well, 
			//  send a "positive" ACK,
			//  and prepare to receive a new request.
			//
            WritePort(pport, 1, CrAckCr);	// ACK, 0x06, means all ok, get next page request.
											// Changed Ack3 to CrAckCr for customer CHOP KG, LD, 2011-01-31
            pstate[pport] = ST_BEGIN_TAP_MESSAGE;
 
			GetLocalTime(&starttime[pport]);

			PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Good PagerId");

			// done at this point, remember that the DoDatabaseWork() procedure 
			//  looks up the PagerId, writes to the flat-file queue, and writes 
			//  to the Xtend "message base" database files.

			// the page is ALREADY WRITTEN to the page queue.
			break;

		case ST_BAD_PAGERID: //case PABORT:
			// unknown PagerId, no point to send that block again.
			//  give a NAK response to signal sender to please send NEXT request if there is one.
			//
			WritePort(pport, 1, abandon);	// RS, 0x1E, means invalid pager.
			pstate[pport] = ST_BEGIN_TAP_MESSAGE;

			GetLocalTime(&starttime[pport]);

 			PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Bad PagerId or Status");
			break;

		case ST_BAD_CHECKSUM: //case PABORT2:
			// Bad checksum... request retransmit the block.
			//
			// increase error count.
			ComPort[pport].reset++;

			WritePort(pport, 1, Nack);		// NAK, 0x15, means send block again please.
			pstate[pport] = ST_GET_NEXT_TAP_BLOCK;

			GetLocalTime(&starttime[pport]);

			PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Bad Checksum");
            break;

        default:
			// this will never happen, unless it is an application bug.
			pstate[pport] = ST_DISCONNECT_AND_START_OVER;
			GetLocalTime(&starttime[pport]);
            break;
		}
	}


	// XOM Protocol
	void
IxoModem(void)		
	{
    switch(pstate[pport])
		{
        case PRESET:
			SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            break;

        case PDTRPAUSE:
            zpagechk(pport, PDTRHIGH, PDTRHIGH, 300, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESETA;
            break;

        case PRESETA:
            ComPort[pport].reset++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
			// establish guard time, 2 seconds, before sending "+++" sequence
			//  to enter modem on-line command mode.
			//  see (Hayes U.S. Patent #4,549,302).
            zpagechk(pport, PRESET2, PRESET2, (2 * 100), 0, "JOE");
            break;

        case PRESET2:
            WritePort(pport, 1, "+++");
            pstate[pport] = PRESET3;
            break;

        case PRESET3:
            zpagechk(pport, PRESET4, PRESET4, 200, 0, "JOE");
            break;

        case PRESET4:
            WritePort(pport, 1, ModemOnHk);
            pstate[pport] = PRESET5;
            break;

        case PRESET5:
            zpagechk(pport, PRESET6, PRESET6, 500, 0, "OK");
            break;

        case PRESET6:
            zpagechk(pport, PRESET7, PRESET7, 100, 0, "JOE");
			break;

		case PRESET7:
			WritePort(pport, 1, param.ComPort[pport].Init1);
            WritePort(pport, 1, cr);
			GetLocalTime(&starttime[pport]);
            pstate[pport] = PRESET8;
            break;

        case PRESET8:
            zpagechk(pport, PRESET9, PRESET9, 300, 0, "NOTHING");
			break;

        case PRESET9:
			WritePort(pport, 1, param.ComPort[pport].Init2);
            WritePort(pport, 1, cr);
			GetLocalTime(&starttime[pport]);
            pstate[pport] = PRESETB;
            break;

        case PRESETB:
			pagerLoginCheck(pport, PRESETC, PREADY, 500);
            break;			

        case PRESETC:
            zpagechk(pport, PRESETD, PRESETD, 500, 0, "OK");
            break;

        case PRESETD:
            zpagechk(pport, PREADY, PREADY, 100, 0, "JOE");
			break;

        case PREADY:
        case PREADY2:
            ComPort[pport].reset = 0;
            while(getbuff( pport, pchar[pport]) == TRUE); // clean up incoming.
            ccounter[pport] = 0;
            break;

        case PSEND:
            WritePort( pport, 1, "ATDT" );
            pstate[pport] = PSEND2;
            break;

        case PSEND2:
			// the capcode is just that, no cr is present at the end of it.
			char CapcodeMessage[100];
			memset(CapcodeMessage, 0, sizeof(CapcodeMessage));

			sprintf_s<sizeof(CapcodeMessage)>(CapcodeMessage, "%s\x0d", pcapcode[pport]);

			// this is the dial string, which is now has a <cr> appended.
            WritePort(pport, 1, CapcodeMessage);
            pstate[pport] = PSEND3;
            break;

        case PSEND3:
            pagechk(pport, PWAIT1, PRESET, (12 * 1000), "CONNECT");

            if(pstate[pport] == PSEND3)
                CheckBuffer(pport, PRESET, 1, "NO ");

            if(pstate[pport] == PSEND3)
                CheckBuffer(pport, PRESET, 1, "ERROR");

            if(pstate[pport] == PSEND3)
                CheckBuffer(pport, PRESET, 1, "BUSY");

            if(pstate[pport] == PWAIT1)
				{
				GetLocalTime(&starttime[pport]);
				}

			if(pstate[pport] == PRESET)
				{
				pageerror[pport] = PSEND3;

				memset(ComPort[pport].errorResponse, 0, sizeof(ComPort[pport].errorResponse));
				sprintf_s<sizeof(ComPort[0].errorResponse)>(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);

				Remove_CtrlChars(ComPort[pport].errorResponse, sizeof(ComPort[pport].errorResponse));
				Set_Null(ComPort[pport].errorResponse, sizeof(ComPort[pport].errorResponse));

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndComMsg,
								"COM%d:, Response [%s]",
								pport + 1,
								ComPort[pport].errorResponse);
				}
            break;

        case PWAIT1:
            zpagechk( pport, PSEND4, PSEND4, 100, 0, "JOE" );
            countA[ pport ] = 0;
            break;

        case PSEND4:
            countA[ pport ]++;
            WritePort( pport, 1, cr );
            pstate[ pport ] = PSEND5;
            break;

        case PSEND5:
            zpagechk(pport, PSEND6, PSEND4, 200, 1, "ID=");
            if(countA[pport] > 12 && pstate[pport] == PSEND4)
				{
				pstate[pport] = PRESET;
				memset(ComPort[pport].errorResponse, 
						0, 
						sizeof(ComPort[pport].errorResponse));

				strcpy_s<sizeof(ComPort[pport].errorResponse)>(ComPort[pport].errorResponse, 
						"Paging Server unable to log in");

				PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"COM%d:, Response [%s]", 
								pport + 1, 
								ComPort[pport].errorResponse);
				}
            break;

        case PSEND6:
			// send the TAP go-ahead message.
			WritePort(pport, 1, EscPG1Cr);
            pstate[pport] = PSEND7;
            break;

        case PSEND7:
            zpagechk(pport, PSEND8, PSEND4, 3000, 1, "[p");

            if(pstate[pport] == PSEND7)
                CheckBuffer( pport, PSEND8, 1, EscP);

            if(pstate[pport] == PSEND7)
                CheckBuffer(pport, PSEND8, 1, "LOGIN ACCEPTED");
            break;

        case PSEND8:
			{
			// prepare to send a message over TAP protocol.
			// initialize variables for multiple TAP blocks.
			Multi_TAP_Block_Count[pport] = 0;
			Multi_TAP_Block_Index[pport] = 0;
		
			// all data from previous operations gets cleared out.
			for(int x = 0; x < 10; x++)
				{
				ZeroMemory(TAP_Block_Data[pport][x], sizeof(TAP_Block_Data[0][0]));
				}

			auto_ptr<Transaction> message(new Transaction());

			message->AddField(string(pauthcode[pport]));
			message->AddField(messageString[pport]);
			message->Blockify();

			Block* bPtr;
			message->ResetBlockList();
			
			{ // scope limit for variable x
			int x = 0;

			while((bPtr = message->GetNextBlock()) != NULL)
				{
				// capture TAP block data into simple 3 dimension array.
				strcpy_s <sizeof(TAP_Block_Data[0][0])> (TAP_Block_Data[pport][x], bPtr->GetBlockBuffer());

				x++;
				Multi_TAP_Block_Count[pport] = x;

				// Moved the serial data "write" operation to a different state, so that the ACK for each block
				//  can be checked as each TAP block is sent.
				}
			}
			}

            pstate[pport] = PSEND9;

			// PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "no data sent yet, but data is now in the simple array");
            break;

		case PSEND9:
			// check the first character of the block, 
			//  if it is a NUL char, the last block has already been sent.

			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "IxoModem - At: PSEND9");

			if(0 == TAP_Block_Data[pport][Multi_TAP_Block_Index[pport]][0])
				{
				// the block is empty, there are no more blocks.
				// all blocks have been sent successfully.
				// this page job is complete,
				// the page job is a success.
				//
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "Page Success - IxoModem protocol - PSEND9 state.");

				// also, claim no error!
				pageerror[pport] = PNOERROR;

				pstate[pport] = PREADY2;
				}
			else
				{
				// send the block.
				WritePort(pport, 1, TAP_Block_Data[pport][Multi_TAP_Block_Index[pport]]);

				// advance the index to the next block to be sent.
				Multi_TAP_Block_Index[pport]++;

				// then wait for the result.
				pstate[pport] = PPAGINGTIMEOUT;
				}

			break;

        case PPAGINGTIMEOUT:
			// just waiting for something to happen, 
			//  a recognizable message being received, or timeout happens.

			// note that the "PAGE SENT" message is seldom seen in as a success message in the data stream.

			// why ???
			// also note that this is xpagechk, all other "checks" are zpagechk.
			//
			xpagechk(pport, PWAIT2, PRESET, 45 * 100, "PAGE SENT");

			// information only to log file.
			if(pstate[pport] == PPAGINGTIMEOUT)
				{
				// PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "PPAGINGTIMEOUT - no state change.");
				}
			else if(pstate[pport] == PWAIT2)
				{
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "PPAGINGTIMEOUT - state change to PWAIT2.");
				}
			else if(pstate[pport] == PRESET)
				{
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "PPAGINGTIMEOUT - state change to PRESET.");
				}
			else
				{
				// PPAGINGTIMEOUT - unknown state change.
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "PPAGINGTIMEOUT - state change to Unknown state.");
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				{
				// no timeout, state has not been changed, 
				//  see if there is a positive ack in the buffer.
                CheckBuffer(pport, PWAIT2, 0, Ack2);
				}

			// best that could have happened, an ACK was found, 
			//  continue at PSEND9 to send another block if there is one avaialable.
			//
            if(pstate[pport] == PWAIT2)
				{
				//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "Got ACK in PPAGINGTIMEOUT state.");

				pstate[pport] = PSEND9;
				break;
				}

            if(pstate[pport] == PPAGINGTIMEOUT)
				CheckBuffer(pport, PRESET, 0, Nack);

            if(pstate[pport] == PPAGINGTIMEOUT)
                CheckBuffer(pport, PRESET, 0, abandon2);

			if(pstate[pport] == PRESET)
				{
				pageerror[ pport ] = PPAGINGTIMEOUT;

				memset(ComPort[pport].errorResponse, 0, sizeof(ComPort[pport].errorResponse));

				sprintf_s<sizeof(ComPort[0].errorResponse)>(ComPort[pport].errorResponse, "%-80.80s", holdbuff[pport]);

				Remove_CtrlChars(ComPort[pport].errorResponse, sizeof(ComPort[pport].errorResponse));
				Set_Null(ComPort[pport].errorResponse, sizeof(ComPort[pport].errorResponse));

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndComMsg,
								"COM%d:, Response [%s]",
								pport + 1,
								ComPort[pport].errorResponse);
				}
            break;

        case PWAIT2:
			{
			// debug code, remove after debugging.
			//PrintMessage(MSGTYPE_COMM_DTR, hwndComMsg, "[%s]", "At: PWAIT2");
			}

			// just a delay.
            zpagechk(pport, PREADY2, PREADY2, 100, 0, "JOE");

			// then claim no error!
            pageerror[pport] = PNOERROR;

			// always exit to PREADY2 state.
            break;

        case PHANGUP:
            WritePort(pport, 1, cr);
            pstate[pport] = PHANG2;
            break;

        case PHANG2:
            zpagechk(pport, PHANG3, PHANG3, 200, 0, "JOE");
            break;

        case PHANG3:
            WritePort(pport, 1, "+++");
            pstate[pport] = PHANG4;
            break;

        case PHANG4:
            zpagechk(pport, PHANG5, PHANG5, 200, 0, "JOE");
            break;

        case PHANG5:
            pageerror[pport] = PNOERROR;
            WritePort(pport, 1, ModemOnHk);

            pstate[pport] = PHANG6;
            break;

        case PHANG6:
            zpagechk(pport, PWAIT3, PWAIT3, 1500, 0, "OK");

            if(pstate[pport] == PWAIT3)
				{
				GetLocalTime(&starttime[pport]);
				}
            break;

        case PWAIT3:
            zpagechk(pport, PSEND, PSEND, 300, 0, "JOE");
            pageerror[pport] = PNOERROR;
            break;

        default:
            break;
		}
	}


	void 
PronetIpt(void)
	{
	char CapcodeMessage[100];

    switch(pstate[pport])
		{
        case PRESET:
            SetDTR(pport, FALSE);
            pstate[pport] = PDTRPAUSE;
            zpagechk( pport, PDTRHIGH, PDTRHIGH, 200, 0, "JOE");
            break;

        case PDTRHIGH:
            SetDTR(pport, TRUE);
            pstate[pport] = PRESETA;
            break;

        case PRESETA:
            ComPort[pport].reset++;
            WritePort(pport, 1, cr);
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            IPTPageChk(pport, 500);
            break;

        case PRESET2:
            WritePort(pport, 1, IPTUser);
            pstate[pport] = PRESET3;
            break;

        case PRESET3:
            zpagechk(pport, PREADY, PRESET, 500, 1, "Pager ID?");
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            while(getbuff(pport, pchar[pport]) == TRUE); // clean up incoming.
            ccounter[pport] = 0;
            break;

        case PSEND:
			// need to append a <cr> to the capcode.
			memset(CapcodeMessage, 0, sizeof(CapcodeMessage));
			//sprintf(CapcodeMessage, "%s\x0d", pcapcode[pport]);
			sprintf_s<sizeof(CapcodeMessage)>(CapcodeMessage, "%s\x0d", pcapcode[pport]);
			
            WritePort(pport, 1, CapcodeMessage);

            pstate[pport] = PSEND2;
            break;

        case PSEND2:
            zpagechk(pport, PSEND5, PRESET, 500, 1, "Numeric message:");
            if( pstate[pport] == PSEND2 )
                pstate[ pport ] = PSEND3;
            break;

        case PSEND3:
            zpagechk(pport, PSEND5, PRESET, 500, 1, "Message:");
            if( pstate[pport] == PSEND3 )
                pstate[ pport ] = PSEND4;
            break;

        case PSEND4:
            zpagechk(pport, PRESET, PRESET, 500, 1, "Goodbye");
            if( pstate[pport] == PSEND4 )
                pstate[ pport ] = PSEND2;
            break;

        case PSEND5:
			// the <cr> was removed from the message for checksum calculation purpose,
			//  it must be replaced here because it's part of the pronet ipt protocol.
			//strcat(pmessage[pport], cr);
			strcat_s<sizeof(pmessage[0])>(pmessage[pport], cr);

            WritePort(pport, 1, pmessage[pport]);
            pstate[pport] = PSEND6;
            break;

        case PSEND6:
            zpagechk( pport, PREADY, PRESET, 500, 1, "Page accepted." );
            break;

        default:
            break;
            
		}
	}


	void
GroupCall(void)
	{
	char string[40];

    switch( pstate[ pport ] ){
        case PRESET:
            ComPort[pport].reset++;
            SetDTR( pport, FALSE );
            pstate[pport] = PRESET1;
            break;

        case PRESET1:
            zpagechk( pport, PRESET2, PRESET2, 500, 1, "JOE" );
            break;

        case PRESET2:
            pageerror[ pport ] = PNOERROR;
            SetDTR( pport, TRUE );
            pstate[pport] = PRESETA;
            break;

        case PRESET3:
            zpagechk( pport, PRESETA, PRESETA, 100, 1, "JOE" );
            break;

        case PRESETA:
            WritePort( pport, 0, cr );
            pstate[pport] = PRESETB;
            break;

        case PRESETB:
            zpagechk( pport, PRESET8, PRESET8, 200, 1, "JOE" );
            break;

        case PRESET8:
            WritePort( pport, 0, CATSInit2 );
            pstate[pport] = PRESET9;
            break;

        case PRESET9:
            zpagechk( pport, PREADY, PRESET, 1500, 1, CATSInit3 );
            break;

        case PREADY:
            ComPort[pport].reset = 0;
            while( getbuff( pport, pchar[pport] ) == TRUE ); // clean up incoming.
            ccounter[pport] = 0;
            break;

        case PSEND:
            memset(string, 0, sizeof(string));
            //strncpy( string, pcapcode[pport], 3 );
			strncpy_s<sizeof(string)>(string, pcapcode[pport], 3 );
            string[ 3 ] = 0;
            WritePort( pport, 0, string );
            pstate[pport] = PWAIT1;
            break;

        case PWAIT1:
            zpagechk( pport, PSEND2, PSEND2, 200, 1, CATSKeyHit );
            break;

        case PSEND2:
            pageerror[ pport ] = PNOERROR;
            memset( string, 0, sizeof(string));
            //strncpy( string, pcapcode[pport]+3, 3 );
            strncpy_s<sizeof(string)>(string, pcapcode[pport]+3, 3 );
            string[ 3 ] = 0;
            WritePort( pport, 1, string );
            pstate[pport] = PSENDTIMEOUT;
            break;

        case PSENDTIMEOUT:
            zpagechk( pport, PPAGINGTIMEOUT, PPAGINGTIMEOUT, 500, 1, CATSKeyHit );
            break;

        case PPAGINGTIMEOUT:
            zpagechk( pport, PHANGUP, PHANGUP, 7000, 1, "JOE" );
            pageerror[ pport ] = PNOERROR;
            break;

        case PHANGUP:
            pageerror[ pport ] = PNOERROR;
            memset( string, 0, sizeof(string));
            //strncpy( string, pmessage[ pport ], 3 );
            strncpy_s<sizeof(string)>(string, pmessage[ pport ], 3 );
            string[ 3 ] = 0;
            WritePort( pport, 1, string );
            pstate[pport] = PHANG2;
            break;

        case PHANG2:
            zpagechk( pport, PHANG3, PHANG3, 200, 1, CATSKeyHit );
            pageerror[ pport ] = PNOERROR;
            break;

        case PHANG3:
            pageerror[ pport ] = PNOERROR;
            memset( string, 0, sizeof(string));
            //strncpy( string, pmessage[ pport ]+3, 3 );
            strncpy_s<sizeof(string)>( string, pmessage[ pport ]+3, 3 );
            string[ 3 ] = 0;
            WritePort( pport, 1, string );
            pstate[pport] = PHANG4;
            break;

        case PHANG4:
            zpagechk( pport, PREADY, PREADY, 200, 1, CATSKeyHit );
            pageerror[ pport ] = PNOERROR;
            break;

        case PABORT:
            pageerror[ pport ] = PNOERROR;
            memset( string, 0, sizeof(string));
            //strncpy( string, pmessage[ pport ], 3 );
            strncpy_s<sizeof(string)>(string, pmessage[ pport ], 3 );
            string[ 3 ] = 0;
            WritePort( pport, 1, string );
            pstate[pport] = PABORT2;
            break;

        case PABORT2:
            zpagechk( pport, PABORT3, PABORT3, 200, 1, CATSKeyHit );
            break;

        case PABORT3:
            pageerror[ pport ] = PNOERROR;
            memset( string, 0, sizeof(string));
            //strncpy( string, pmessage[ pport ]+3, 3 );
            strncpy_s<sizeof(string)>(string, pmessage[ pport ]+3, 3 );
            string[ 3 ] = 0;
            WritePort( pport, 1, string );
            pstate[pport] = PABORT4;
            break;

        case PABORT4:
            zpagechk( pport, PRESET, PRESET, 50, 1, CATSKeyHit );
            break;
		}
	}


	void
WritePort(int Port, int RS, const char * Text)
	{
    while(getbuff(Port, pchar[Port]) == TRUE)
		{
        holdbuff[Port][ccounter[Port]] = (char) pchar[Port][0];
        ++ccounter[Port];
        holdbuff[Port][ccounter[Port]] = 0;

        if((pchar[Port][0] == 13) || (ccounter[Port] > RXBUFFLEN))
			break;
		}

	PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "[COM%d] Begin Sending...", Port + 1);

	WriteComport(Port, Text);
    GetLocalTime(&starttime[Port]);

	// RS means, reset the port buffer????
    if(RS == 1)
		{
        ccounter[Port] = 0;
        memset(holdbuff[Port], 0,  RXBUFFLEN);
		}

	{
	//
	// log each and every serial string sent to the comm port to the logfile,
	//
	// this option is controlled by an application command line option. 
	//  -T representing (log comm port Traffic).
	//
	if(bCmdLineEnableLogCommportTraffic)
		{
		// for each character in the string, prepare a "printable" 
		//  representation for logging purposes.
		//
		CString csOutputString = "";
		CString csCharacter;

		for(int i = 0; i < (int) strlen(Text); i++)
			{
			// limit to 7 bit ascii.
			int c = Text[i] & 0x7f;

			if((c > 0x20) && (c <= 0x7e))
				{
				// a printable character.
				csCharacter.Format("%c ", (char) c);
				}
			else
				{
				// non-printable, show it as ascii-hex representation.
				csCharacter.Format("0x%02x ", c);
				}

			// add the formatted or unformatted char to the "output" string.
			//
			csOutputString += csCharacter;
			}

			PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "[COM%d] sent: [%s]", Port + 1, csOutputString);
			}
		}

	}


	void
SetDTR(int comNumber, int dtrFlag)
	{
	BOOL fSuccess;
	DWORD dwFunc;

    if(param.ComPort[comNumber].toggleDTR)
		{
        while(getbuff(comNumber, pchar[comNumber]) == TRUE);	// clean up incoming.

	    if(dtrFlag)
			{
			dwFunc = SETDTR;
			}
		else
			{
			dwFunc = CLRDTR;
			}

		fSuccess = EscapeCommFunction(ComPort[comNumber].hComm, dwFunc);

		if(fSuccess)
			{
			PrintMessage(MSGTYPE_COMM_DTR, 
							hwndComMsg, 
							"[%s]:  DTR Set %d", 
							CommportName[comNumber].FamiliarName,
							dtrFlag);
			}
		else
			{
			PrintMessage(MSGTYPE_COMM_ERROR, 
							hwndComMsg, 
							"[%s]:  Error SetDTR", 
							CommportName[comNumber].FamiliarName);
			}
		}

    GetLocalTime(&starttime[comNumber]);
	}


	// return a single character from com port buffer
	//  of the specified com port, if one is available.
	//
	// returns TRUE if A new character was received,
	//  returns FALSE if no new character was received
	//  the character is returned at the location pointed to by comChar.
	BOOL
getbuff(int comNumber, char *comChar)
	{
	BOOL NewCharacterReceived = FALSE;

	try
		{
		WaitForSingleObject(hMutexComm[comNumber], INFINITE);

		if(ComPort[comNumber].nextRead != ComPort[comNumber].nextWrite)
			{
			comChar[0] = ComPort[comNumber].buffer[ComPort[comNumber].nextRead];
			ComPort[comNumber].nextRead++;

			if(ComPort[comNumber].nextRead > sizeof(ComPort[comNumber].buffer) - 1)
				{
				ComPort[comNumber].nextRead = 0;
				}
			NewCharacterReceived = TRUE;
			}
		}
	catch(...)
		{
		// swallow any exception
		}

	// finally, release the mutex.
	//
	ReleaseMutex(hMutexComm[comNumber]);

	{
	//
	// log each and every character received from the serial port to the logfile,
	//
	// this option is controlled by an application command line option. 
	//  -T representing (log comm port Traffic).
	//
	if(bCmdLineEnableLogCommportTraffic && NewCharacterReceived)
		{
		CString csSerialCharacterLog;
		CString csCharacter;

		// limit to 7 bit ascii.
		int c = comChar[0] & 0x7f;

		if((c > 0x20) && (c <= 0x7e))
			{
			// a printable character.
			csCharacter.Format("%c", (char) c);
			}
		else
			{
			// non-printable, show it as ascii-hex representation.
			csCharacter.Format("0x%02x", c);
			}
		
		csSerialCharacterLog.Format("[COM%d] received char: [%s]",
										comNumber + 1,
										csCharacter);

		PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "%s", csSerialCharacterLog);
		}
	}

	return(NewCharacterReceived);
	}


	// Check received text,
	// If text matches:
	//  set next state,
	//  and clear out the buffer.
	//  no timeout available.
	//	advance to error state option available.
	//
	void
CheckBuffer(int pgport, unsigned nextstate, int resetBuf, char * prompt)
	{
	if(strstr(holdbuff[pgport], prompt) != NULL)
		{
		// found the text we were expecting,
		//  advance to next state.
        pstate[pgport] = nextstate;

		// clear out the rx buffer if requested.
		if(resetBuf)
			{
			ccounter[pgport]=0;
			memset(holdbuff[pgport], 0,  RXBUFFLEN);
			}

		GetLocalTime(&starttime[pgport]);
		}
	}


	//  pagechk: - read incomming buffer
	//           - Stop on carriage return or line feed
	//           - Check prompt. If matches reset buffer, set next state
	//           - If time expires set error level, next state
	void
pagechk(int pgport, unsigned nextstate, unsigned errorstate, unsigned ptimeout, char * prompt)
	{
	// only used in XOM protocol.
    while(getbuff(pgport, pchar[pgport]) == TRUE)
		{
        holdbuff[pgport][ccounter[pgport]] = (char) pchar[pgport][0];
        ++ccounter[pgport];
        holdbuff[pgport][ccounter[pgport]] = 0;

		// if <cr> or <lf>, skip ahead and compare the text
        if(pchar[pgport][0] == 13 ||
			pchar[pgport][0] == 10)
			{
			break;
			}
		}

    if(ccounter[pgport] >= strlen(prompt))
		{
		// enough chars have been received to possibly
		//  match the expected text, go ahead and compare.
        if(strstr(holdbuff[pgport], prompt) !=NULL)
			{
			// expected text received.
			pstate[pgport] = nextstate;

            while(getbuff(pgport, pchar[pgport]) == TRUE);	// clean up incoming.
			ccounter[pgport]=0;
            memset(holdbuff[pgport], 0,  RXBUFFLEN);
			return;
			}
		}

	if(Wait( &starttime[pgport], (long) ptimeout))
		{
		pageerror[pgport] = pstate[pgport];
		pstate[pgport] = errorstate;
		}

	if(ccounter[pgport] >= RXBUFFLEN)
		ccounter[pgport]=0;
	}


	//  read incomming buffer.
	//  Stop on carriage return or buffer overflow.
	//  Check prompt. If matches reset buffer, set next state.
	//  If time expires set error level, next state.
	//  Capture "Current Time" if timeout, or recognition of "text".
	//
	void
zpagechk(int pgport,
		 unsigned nextstate,
		 unsigned errorstate,
		 unsigned ptimeout,
		 int seterr,
		 char * prompt)
	{
	// always transfer characters to a local buffer, and update character counter.
    while(getbuff(pgport, pchar[pgport]) == TRUE)
		{
		holdbuff[pgport][ccounter[pgport]] = (char) pchar[pgport][0];
		++ccounter[pgport];
		holdbuff[pgport][ccounter[pgport]] = 0;

		if((pchar[pgport][0] == 13) || (ccounter[pgport] >RXBUFFLEN))
			{
			ccounter[pgport] = 0;
			break;
			}
		}

	// check for "expected" text.
	if(strstr(holdbuff[pgport], prompt) != NULL)
		{
		// received expected text.
		//  cleanup input buffer, and advance to success state. 
        while(getbuff(pgport, pchar[pgport]) == TRUE)
			{
			// clean up incoming.
			}

        ccounter[pgport] = 0;
        memset(holdbuff[pgport], 0,  RXBUFFLEN);

		pstate[pgport] = nextstate;
		GetLocalTime(&starttime[pgport]); 
		return;
		}

	// check for timeout waiting for expected text.
	if(Wait(&starttime[pgport], (long) ptimeout))
		{
        if(seterr)
			{
			pageerror[pgport] = pstate[pgport];
			}

        pstate[pgport] = errorstate;
		GetLocalTime(&starttime[pgport]);
		return;
		}
	}


	void
IPTPageChk(int pgport, unsigned ptimeout)
	{
    while(getbuff(pgport, pchar[pgport]) == TRUE)
		{
        holdbuff[pgport][ccounter[pgport]] = (char) pchar[pgport][0];
        ++ccounter[pgport];
        holdbuff[pgport][ccounter[pgport]] = 0;

        if((pchar[pgport][0] == 13) || (pchar[pgport][0] == 0) || (ccounter[pgport] >RXBUFFLEN))
			{
            ccounter[pgport] = 0;
            break;
			}
		}

    DoState(pgport, PREADY, "Pager ID?");
    DoState(pgport, PRESET2, "Passcode ID");
    DoState(pgport, PRESET2, "ID=");
    DoState(pgport, PRESET, "Goodbye.");

	if(Wait(&starttime[pgport], (long) ptimeout))
		{
        pageerror[pgport] = pstate[pgport];
        pstate[pgport] = PRESET;
		}
	}


	void
DoState(int pgport, unsigned nextstate, char * prompt)
	{
	// this is only used by "IPT".
    if(strstr(holdbuff[pgport], prompt) != NULL)
		{
        pstate[pgport] = nextstate;
        while( getbuff(pgport, pchar[pgport]) == TRUE); // clean up incoming.
        ccounter[pgport] = 0;
        memset(holdbuff[pgport], 0,  RXBUFFLEN);
        return;
		}
	}


	typedef bool (*delimterTest)(char);


	static bool 
TestCR(char c) 
	{
	if(c == 0x0d)
		{
		return true;
		}
	return false;
	}


	static bool 
TestSTX(char c) 
	{
	if(c == 0x02)
		{
		return true;
		}
	return false;
	}


	static bool 
TestBlockEnd(char c)
	{
	if(c == 0x03) 
		{
		return true;	// <ETX>
		}

	if(c == 0x17) 
		{
		return true;	// <ETB>
		}

	if(c == 0x1f) 
		{
		return true;	// <US>
		}

	return false;
	}


	void 
commonPageCheck(int pgport,
				unsigned nextstate,
				unsigned errorstate,
				unsigned ptimeout,
				char * prompt,
				delimterTest delimiter)
	{
	// make a pointer to the end of what's in the per-serial-port buffer.
	char* holdPtr = &holdbuff[pgport][ccounter[pgport]];

	while(getbuff(pgport, pchar[pgport]) == TRUE)
		{
		// check for too many characters.
		if(ccounter[pgport] > RXBUFFLEN) 
			{
			// too many characters... new characters will not be transferred.
			//  timeout will most likely occur.
			break;
			}

		*holdPtr = *pchar[pgport];
		*holdPtr++;

		// ensure null termination of the holding buffer after each char is added.
		*holdPtr = 0;

		// increase the character count.
		ccounter[pgport]++;

		if(strlen(prompt) == 0)
			{
			// a "prompt" was not specified, 
			//  apparently we are looking for a terminator character only.
			if(delimiter(*pchar[pgport])) 
				{
				pstate[pgport] = nextstate;
				return;
				}
			}
		else
			{
			// attempt to match the "prompt" string.
			if(strstr(holdbuff[pgport], prompt) != NULL)
				{
				pstate[pgport] = nextstate;
				return;
				}
			}
		}

	// timeout check always applies, but does not update "reference" time.
	if(Wait(&starttime[pgport], (long) ptimeout))
		{
		pageerror[pgport] = pstate[pgport];
		pstate[pgport] = errorstate;
		return;
		}
	}


	void 
xpagechk(int pgport,
			unsigned nextstate,
			unsigned errorstate,
			unsigned ptimeout,
			char * prompt)
	{
	commonPageCheck(pgport, nextstate, errorstate, ptimeout, prompt, TestCR);
	}


	void 
BlockBeginPageCheck(int pgport,
					unsigned nextstate,
					unsigned errorstate,
					unsigned ptimeout)
	{
	// specify the end-of-block character function, and an empty "prompt" to recognize.
	commonPageCheck(pgport, nextstate, errorstate, ptimeout, "", TestSTX);
	}


	void		
BlockEndPageCheck(int pgport,
					unsigned nextstate,
					unsigned errorstate,
					unsigned ptimeout)
	{
	// specify the end-of-block character function, and an empty "prompt" to recognize.
	commonPageCheck(pgport, nextstate, errorstate, ptimeout, "", TestBlockEnd);
	}
	

	//check for pager login PG1 and accept ANY password given.
	//<ESC>PG1pppppp<CR>
	void 
pagerLoginCheck(int pgport,
				unsigned nextstate,
				unsigned errorstate,
				unsigned ptimeout)
	{
	char* holdPtr = &holdbuff[pgport][ccounter[pgport]];
		
	while(getbuff(pgport, pchar[pgport]) == TRUE)
		{
		*holdPtr++ = *pchar[pgport];
		*holdPtr = 0;
		ccounter[pgport]++;

		if((*pchar[pgport] == '\r') || (ccounter[pgport] >= RXBUFFLEN)) 
			{
			break;
			}
		}
				
	if((strstr(holdbuff[pgport], "\x1bPG1") != NULL) && (strstr(holdbuff[pgport], "\r")))
		{
		pstate[pgport] = nextstate;
		return;
		}
	
	if(Wait(&starttime[pgport], (long) ptimeout))
		{
		pageerror[pgport] = pstate[pgport];
		pstate[pgport] = errorstate;
		}
	}	


	int
GetNumberOfTitleLinesInCommPortStatusWindow(void)
	{
	const int HeaderLines = 3;

	return (int) HeaderLines;
	}


	void
WriteToCommPortStatusWindow(int Line, LPCSTR Text)
	{
	// disable re-draw temporarily.
	SendMessage(hwndComStatus, WM_SETREDRAW, (WPARAM) FALSE, (LPARAM) 0);

	// update line item.
	SendMessage(hwndComStatus, LB_DELETESTRING, (WPARAM) Line, (LPARAM) 0L);
	SendMessage(hwndComStatus, LB_INSERTSTRING, (WPARAM) Line, (LPARAM) (LPSTR) Text);

	// allow re-draw now.
	SendMessage(hwndComStatus, WM_SETREDRAW, (WPARAM) TRUE, (LPARAM) 0);
	}


	void
InitializeCommPortStatusWindow(int IsStandby)
	{
	int L;

	WriteToCommPortStatusWindow(0, "Port    Port    Device        Port           ");
	WriteToCommPortStatusWindow(1, "Number  Type    Description   Status         ");
	WriteToCommPortStatusWindow(2, "------  ------  ------------  ---------------");

	// write empty info to each non-title line item.
	for(L = 0; L < MAX_COMMPORTS; L++)
		{
		WriteToCommPortStatusWindow(L + GetNumberOfTitleLinesInCommPortStatusWindow(), 
									"");
		}

	// post a standby message if requested.
	if(IsStandby)
		{
		WriteToCommPortStatusWindow(4, "  Standing By ...");
		}
	}


	// display status line in "Comm Port Status" window.
	// 
	void
UpdateCommPortStatusWindow(int comNumber)
	{
    char message[80];
	char temp[255];

	// if the window is not open, or has been closed,
	//  the handle should be NULL,
	//  skip any window access/update.
	if(hwndComStatus == NULL)
		{
		return;
		}

	// decide where to put the information inside the listbox,
	//  leaving header lines undisturbed.
	int ListboxLineItemToUpdate = GetNumberOfTitleLinesInCommPortStatusWindow() + comNumber;

    memset(message, 0, sizeof(message));

   // sprintf(message,
			//"  %-2.2i     %-3.3s    %-12.12s  %s",
			//comNumber + 1,
			//CommPortDescriptionText[param.ComPort[comNumber].PageActive],
			//param.ComPort[comNumber].UserDefinedShortDesignation,
			//CPortMessage[pstate[comNumber]]);

    sprintf_s<sizeof(message)>(message,
			"  %-2.2i     %-3.3s    %-12.12s  %s",
			comNumber + 1,
			CommPortDescriptionText[param.ComPort[comNumber].PageActive],
			param.ComPort[comNumber].UserDefinedShortDesignation,
			CPortMessage[pstate[comNumber]]);

	memset(temp, 0, sizeof(temp));

	SendMessage(hwndComStatus, 
				LB_GETTEXT, 
				(WPARAM) ListboxLineItemToUpdate, 
				(LPARAM) temp);

	if(strcmp(temp, message) == 0)
		{
		return;
		}
	else
		{
		WriteToCommPortStatusWindow(ListboxLineItemToUpdate, message);
		}
	}


	void
CheckCommState(void)
	{
    int	iport;
    int	j;
    int	k;
    int	l;
	int	sendPage;
	unsigned char cByte;
    char string3[256+1];
    char string4[256+1];
    char Messages[256+1];
	int i;

    for(iport = 0; iport < MAX_COMMPORTS; iport++)
		{
        if(pageactive[iport] == FALSE || portactive[iport] == FALSE)
			{
			// skip ports that are not configured.
			continue;
			}

        if(param.Lpt.ResetLevel > 0 &&
			ComPort[iport].reset > param.Lpt.ResetLevel &&
			ComPort[iport].alarm == FALSE)
			{
            ComPort[ iport ].alarm = TRUE;
            WriteAlarmLog_CommportFailure(iport, 0);
			}

        if(ComPort[iport].reset < param.Lpt.ResetLevel)
			{
            ComPort[iport].alarm = FALSE;
			}

		// this code does NOT apply to "serial-in" type of protocols.
        if(pageactive[iport] == GROUPCALL ||
			pageactive[iport] == IXO_ACCEPT ||
			pageactive[iport] == IXO_ACCEPT_DIRECT ||
			pageactive[iport] == IXX_DIRECT ||
			pageactive[iport] == IXO_DISCARD)
			{
            continue;
			}

        switch(ComPort[iport].state)
			{
            case RPNOTREADY:
				// comm port is in not-ready state...
               if(pstate[iport] == PREADY || pstate[iport] == PREADY2)
					{
                    ComPort[iport].state = RPREADY;
                    memset(ComPort[iport].oldCapcode, 0, sizeof(ComPort[iport].oldCapcode));
					}
                else
					{
					//  if timeout force psate to preset
                    if(ComPort[iport].timer.wHour == 70)
						{
						GetLocalTime(&ComPort[iport].timer);
						}

                    k = Wait(&ComPort[iport].timer, 3000L);

                    if(k == 1)
						{
                        ComPort[iport].state = RPNOTREADY;
                        pstate[iport] = PRESET;
                        ComPort[iport].timer.wHour = 70;
                        PrintMessage(MSGTYPE_COMM_RESET, 
										hwndComMsg, 
										"Resetting Serial Port COM%i:.", 
										iport + 1);
						}

                    if(ComPort[iport].QTimer.wHour == 70)
						{
						GetLocalTime(&ComPort[iport].QTimer);
						}

                    k = Wait(&ComPort[iport].QTimer, (long) QPause);

                    if(k)
						{
                        ComPort[iport].QTimer.wHour = 70;
						}
					}
                break;

            case RPREADY:
				sendPage = TRUE;

				// coordinate "shared" ports.
                if(param.ComPort[iport].SharedPort > -1)
                    if(param.ComPort[iport].SharedPsId[0] == 0)
                        if(pstate[param.ComPort[iport].SharedPort] > PREADY)
							sendPage = FALSE;

				// check parallel port bit enable signals.
                if(ComPort[iport].PortBit != -1)
					{
					j = ComPort[ iport ].messageOpt;
					ComPort[ iport ].messageOpt = 0;

					cByte = 0;

                    memset( string3, 0, sizeof( string3 ) );
					//itoa( (int)cByte, string3, 2 );
					_itoa_s<sizeof(string3)>( static_cast<int>(cByte), string3, 2 );

					memset( string4, 0, sizeof( string4 ) );
					//sprintf( string4, "%8.8s", string3 );
					sprintf_s<sizeof(string4)>(string4, "%8.8s", string3 );

					for( i=0; i<8; i++ )
						if( string4[ i ] == ' ' )
							string4[ i ] = '0';

                    if( string4[ 7 - ComPort[ iport ].PortBit ] == '0' )
						{
						sendPage = FALSE;
						ComPort[ iport ].messageOpt = 1;
						}

					if( j != ComPort[ iport ].messageOpt )
						{
						PrintMessage(MSGTYPE_SYSTEM,  
										hwndComMsg,
										"COM%d: Holdbit %d, Bits:%s.",
										iport+1,
										ComPort[ iport ].PortBit,
										string4 );
						}
					}

                if(ComPort[iport].greenLight != TRUE)
					{
					sendPage = FALSE;
					}

                if(pstate[iport] == PREADY || pstate[iport] == PREADY2)
					{
                    if(page[iport + PAGE_COM_START].PageStatus[0] == 'A' && sendPage == TRUE)
						{
                        if(page[iport + PAGE_COM_START].attempts + 1 <= param.ComPort[iport].Retry)
							{
							// number of tries/retries is not exhausted yet.
                            page[iport + PAGE_COM_START].attempts++;
                            pageerror[iport] = PNOERROR;
                            page[iport + PAGE_COM_START].PageStatus[0] = 'B';

                            memset(string3, 0, sizeof(string3));
                            memset(string4, 0, sizeof(string4));
                            memset(pstatus[iport], 0, sizeof(pstatus[iport]));

                            //strcpy(string3, pcapcode[iport]);
                            strcpy_s<sizeof(string3)>(string3, pcapcode[iport]);
                            //strcpy(string4, pmessage[iport]);
                            strcpy_s<sizeof(string4)>(string4, pmessage[iport]);

                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg,
											"Repaging on COM%i: CapCode [%s], Message [%s].",
											iport+1,
											string3,
											string4);

                            ComPort[iport].state = RPPAGING;

							// zetron 15 device requires....
                            if(pageactive[iport] == ZETRON15)
								{
                                for(j = 0; j < 15; j++)
									{
                                    if(pcapcode[iport][0] == param.PFormat[j].Id[0])
										{
                                        k = pcapcode[iport][param.PFormat[j].Position];

                                        for(int l = 0; l < (int) strlen(param.PFormat[j].VoiceFuncs); l++)
											{
                                            if(k == param.PFormat[j].VoiceFuncs[l])
												{
                                                ComPort[iport].state = RPVOICE;
												}
											}
                                        break;
										}
									}
								}
                            GetLocalTime(&ComPort[iport].timer);
                            pstate[iport] = PSEND;
							}
                        else
							{
							// too many retries, give up.
                            AbortComPage(iport);
							}

                        break;
						}

					l = 0;

                    if(ComPort[iport].QTimer.wHour == 70)
						{
						GetLocalTime(&ComPort[iport].QTimer);
						}

                    k = Wait(&ComPort[iport].QTimer, (long) QPause);

                    if(k)
						{
						for(j = 0; j < 20; j++)
							{
							if(ComPort[iport].InQueuePointer[j] < 0)
								{
								break;
								}

							l = CheckQueue(ComPort[iport].InQueuePointer[j]);
							
							if(l > 0)
								{
								// there is at least one item in the page queue to claim.
								//
								// check the "control port", see config utility COMX port screen.
								if(((param.ComPort[iport].ControlPort[j] - 1) == iport) && (sendPage == TRUE))
									{
									// this pageserver "owns" this comm port.
									ProcessCommportQueue(iport, j);
									}
								else
									{
									ComPort[iport].QTimer.wHour = 70;
									}
								break;
								}
							else
								{
								// no "claimable" records are in the page queue.
								//
								// if the protocol type is TA3 (TAP_TYPE3), this is a signal to end the session immediately.
								//
								if(pageactive[iport] == TAP_TYPE3)
									{
									// advance to end session immediately.
									pstate[iport] = PRESET1;
									}
								}
							}

                        if(pstate[iport] == PREADY2)
							{
                            pstate[iport] = PRESET;
                            ComPort[iport].state = RPNOTREADY;
							}

                        if(l <= 0) 
							ComPort[iport].QTimer.wHour = 70;
						}
					}
                else
					{
                    ComPort[iport].state = RPNOTREADY;
					}
                break;

            case RPWAITING:
                if( ComPort[ iport ].timer.wHour == 70 )
					{
					GetLocalTime(&ComPort[iport].timer);
					}

                k = Wait( &ComPort[ iport ].timer, 3000L );

                if( k == 1 )
					{
                    page[ iport + PAGE_COM_START ].PageStatus[ 0 ] = 'I';
                    ComPort[ iport ].state = RPNOTREADY;
                    ComPort[ iport ].Errors++;
                    SetQueueError( iport, -1, 1 );
                    pstate[ iport ] = PRESET;
					}
                break;

            case RPPAGING:
                if( pstate[ iport ] == PREADY || pstate[ iport ] == PREADY2 )
					{
                    if( pageerror[ iport ] == PNOERROR )
						{
                        page[ iport + PAGE_COM_START ].PageStatus[ 0 ] = 'I';
                        ComPort[ iport ].state = RPREADY;

						if( strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "Code List" ) == 0 ||
							strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "CATS PG" ) == 0 )
							{
							//sprintf( string3, "XPCD%2.2s%2.2s.xnl", xnSystemTime, xnSystemTime+3 );
							sprintf_s<sizeof(string3)>( string3, "XPCD%2.2s%2.2s.xnl", xnSystemTime, xnSystemTime+3 );
							memset(string4, 0, sizeof(string4));

							//sprintf(string4,
							//		"                Page Completed: Profile Id: %10.10s, Id: %10.10s, Initiated: %8.8s, PageType : %%%2.2s",
							//		page[ iport + PAGE_COM_START ].Extension,
							//		page[ iport + PAGE_COM_START ].PagerId,
							//		page[ iport + PAGE_COM_START ].InitiatedTime,
							//		page[ iport + PAGE_COM_START ].PageType );
							sprintf_s<sizeof(string4)>(string4,
									"                Page Completed: Profile Id: %10.10s, Id: %10.10s, Initiated: %8.8s, PageType : %%%2.2s",
									page[ iport + PAGE_COM_START ].Extension,
									page[ iport + PAGE_COM_START ].PagerId,
									page[ iport + PAGE_COM_START ].InitiatedTime,
									page[ iport + PAGE_COM_START ].PageType );
							WriteLog(string3, string4 );

							//sprintf(string4,
							//		"                Number Sent: %.15s, Id Number: %.15s, Message Sent: %.40s, Voice File %.40s",
							//		page[iport + PAGE_COM_START].PagerNumber,
							//		page[ iport + PAGE_COM_START ].AutCode,
							//		page[ iport + PAGE_COM_START ].DisplayInfo,
							//		page[ iport + PAGE_COM_START ].FileName );
							sprintf_s<sizeof(string4)>(string4,
									"                Number Sent: %.15s, Id Number: %.15s, Message Sent: %.40s, Voice File %.40s",
									page[iport + PAGE_COM_START].PagerNumber,
									page[ iport + PAGE_COM_START ].AutCode,
									page[ iport + PAGE_COM_START ].DisplayInfo,
									page[ iport + PAGE_COM_START ].FileName );
							WriteLog(string3, string4 );
							}
						}
                    else
						{
                        page[iport + PAGE_COM_START].PageStatus[0] = 'A';
                        ComPort[iport].state = RPNOTREADY;
                        ComPort[iport].Errors++;
                        SetQueueError(iport, -1, 1);

                        PrintMessage(MSGTYPE_SYSTEM, 
										hwndComMsg, 
										"Paging device status: %i", 
										pageerror[iport]);
						}
					}
                else
					{
                    if(ComPort[iport].timer.wHour == 70)
						{
						GetLocalTime(&ComPort[iport].timer);
						}

                    if(pageactive[iport] == IXO_MODEM)
						{
						k = Wait(&ComPort[iport].timer, 90 * 100L);
						}
                    else
						{
						// this time interval controls the time to wait 
						//  before the TAP [0x04] is delivered, this was lengthened to
						//  support the backup mode of the Hark ISI box. 
						//  original time interval was 20 sec. 2005-11-02.
                        k = Wait(&ComPort[iport].timer, 120 * 100L);
						}

                    if(k == 1 && pstate[iport] != PPAGINGBUSY)
						{
						PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Timing out on: ComPort[iport].timer...");

                        page[iport + PAGE_COM_START].PageStatus[0] = 'A';
                        ComPort[iport].state = RPNOTREADY;
                        ComPort[iport].Errors++;
                        SetQueueError(iport, -1, 1);
                        pstate[iport] = PRESET;
						}
					}
                break;

            case RPVOICE:
                switch( pstate[ iport ] )
					{
                    case PREADY:
                        if( pageerror[ iport ] == PNOERROR )
							{
                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											"Voice Page Terminated");

							if( strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "Code List" ) == 0 ||
								strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "CATS PG" ) == 0 )
								{
								//sprintf( string3, "XPCD%2.2s%2.2s.xnl", xnSystemTime, xnSystemTime+3 );
								sprintf_s<sizeof(string3)>( string3, "XPCD%2.2s%2.2s.xnl", xnSystemTime, xnSystemTime+3 );
								memset( string4, 0, sizeof( string4 ) );
								//sprintf(string4,
								//		"Page Completed: Profile Id: %10.10s, Id: %10.10s, Initiated: %8.8s, PageType : %%%2.2s",
								//		page[ iport + PAGE_COM_START ].Extension,
								//		page[ iport + PAGE_COM_START ].PagerId,
								//		page[ iport + PAGE_COM_START ].InitiatedTime,
								//		page[ iport + PAGE_COM_START ].PageType );
								sprintf_s<sizeof(string4)>(string4,
										"Page Completed: Profile Id: %10.10s, Id: %10.10s, Initiated: %8.8s, PageType : %%%2.2s",
										page[ iport + PAGE_COM_START ].Extension,
										page[ iport + PAGE_COM_START ].PagerId,
										page[ iport + PAGE_COM_START ].InitiatedTime,
										page[ iport + PAGE_COM_START ].PageType );
								WriteLog( string3, string4 );
								//sprintf( string4,
								//	"                Number Sent: %.15s, Id Number: %.15s, Message Sent: %.40s, Voice File %.40s",
								//	page[iport + PAGE_COM_START].PagerNumber,
								//	page[ iport + PAGE_COM_START ].AutCode,
								//	page[ iport + PAGE_COM_START ].DisplayInfo,
								//	page[ iport + PAGE_COM_START ].FileName );
								sprintf_s<sizeof(string4)>( string4,
									"                Number Sent: %.15s, Id Number: %.15s, Message Sent: %.40s, Voice File %.40s",
									page[iport + PAGE_COM_START].PagerNumber,
									page[ iport + PAGE_COM_START ].AutCode,
									page[ iport + PAGE_COM_START ].DisplayInfo,
									page[ iport + PAGE_COM_START ].FileName );
								WriteLog( string3, string4 );
								}

                            if(port[page[iport + PAGE_COM_START].PortNumber].file_handle > 0)
								{
                                dx_fileclose(port[page[iport + PAGE_COM_START].PortNumber].file_handle);
                                port[page[iport + PAGE_COM_START].PortNumber].file_handle = 0;
								}

                            port[ page[ iport + PAGE_COM_START ].PortNumber ].state = ST_VP_IDLE;
                            port[ page[ iport + PAGE_COM_START ].PortNumber ].timer.wHour = 70;
                            k = page[ iport + PAGE_COM_START ].PortNumber;
							XnSetHook( k, DX_ONHOOK );

                            UpdatePhoneLineStatusWindow(page[iport + PAGE_COM_START].PortNumber, 
														"Idle");

                            if( param.VoiceDelete && page[ iport + PAGE_COM_START ].FileName[ 0 ] != 0 )
								{
                                if( strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "Code List" ) != 0 &&
									strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "XP CS" ) != 0 )
									{
									SetFileAttributes( page[ iport + PAGE_COM_START ].FileName, FILE_ATTRIBUTE_NORMAL );
                                    if( remove( page[ iport + PAGE_COM_START ].FileName ) == 0 )
										{
                                        PrintMessage(MSGTYPE_SYSTEM,  
														hwndComMsg,
														"Voice file: %s removed.",
														page[ iport + PAGE_COM_START ].FileName );
										}
                                    else
										{
                                        PrintMessage(MSGTYPE_SYSTEM,  
														hwndComMsg,
														"Unable to remove voice file: %s.",
														page[ iport + PAGE_COM_START ].FileName );
										}
									}
								}
                            page[ iport + PAGE_COM_START ].PageStatus[ 0 ] = 'I';
                            ComPort[ iport ].state = RPREADY;
                            break;
                        }
                        else{
                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											"Voice Page unsuccessful, error %i.", 
											pageerror[iport]);

                            page[ iport + PAGE_COM_START ].PageStatus[ 0 ] = 'A';
                            ComPort[ iport ].Errors++;
                            SetQueueError( iport, -1, 1 );
                            ComPort[ iport ].state = RPREADY;
                        }
                        break;

                    case PTALKNOW:
                        // Tell notis to talk now
                        port[ page[ iport + PAGE_COM_START ].PortNumber ].ComNumber = iport;
                        port[ page[ iport + PAGE_COM_START ].PortNumber ].state = ST_VPAGE_TALK;

                        UpdatePhoneLineStatusWindow(page[iport + PAGE_COM_START].PortNumber, 
													"Voice Page Talk");

						k = page[ iport + PAGE_COM_START ].PortNumber;
						dx_clrdigbuf( port[ k ].d4xHandle );
                        port[ page[ iport + PAGE_COM_START ].PortNumber ].Attempts++;
                        ComPort[ iport ].timer.wHour = 70;

                        if( port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle > 0 )
							{
                            dx_fileclose( port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle );
                            port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle = 0;
							}

                        switch( param.ComPort[ iport ].QueType[ page[ iport + PAGE_COM_START ].ComOffset ][ 0 ] )
							{
                            case 'T':
                                if( page[ iport + PAGE_COM_START ].VoiceFile == 0 ){
                                    k = 0;
                                    for( l=0; l<param.TalkPageRpts; l++ ){
                                        if( strncmp( page[ iport + PAGE_COM_START ].InitiatorId, "External", 8 ) == 0 || param.VoiceConv ){
                                            Messages[ k ] = 51;
                                            k++;
                                        }
                                        for( j=0; j<sizeof( pmessage[ iport ] ) && pmessage[ iport ][ j ] != 0; j++ ){
                                            if( pmessage[ iport ][ j ] > 47 && pmessage[ iport ][ j ] < 58 ){
                                                Messages[ k ] = (char)( pmessage[ iport ][ j ] - 48 );
                                                k++;
                                            }
                                            else if( param.VoiceConv ){
                                                switch( pmessage[ iport ][ j ] ){
                                                    case '*':
                                                    case 'X':
                                                        if(param.StarPrompt > 1 )
															{
                                                            Messages[ k ] = (char) (param.StarPrompt - 1); k++;
															}
                                                        break;
                                                    case 'C':
                                                        Messages[ k ] = 51; k++;
                                                        break;
                                                    case 'H':
                                                        Messages[ k ] = 90; k++;
                                                        break;
                                                    case 'B':
                                                        Messages[ k ] = 91; k++;
                                                        break;
                                                    case 'E':
                                                        Messages[ k ] = 92; k++;
                                                        break;
                                                    case 'N':
                                                        Messages[ k ] = 93; k++;
                                                        break;
                                                    case 'R':
                                                        Messages[ k ] = 94; k++;
                                                        break;
                                                    case 'S':
                                                        Messages[ k ] = 95; k++;
                                                        break;
                                                    case 'P':
                                                        Messages[ k ] = 96; k++;
                                                        break;
                                                    case 'O':
                                                        Messages[ k ] = 69; k++;
                                                        break;
                                                    default:
                                                        break;
                                                }
                                            }
                                        }
                                    }
                                    k = vb40_play( promptfh, page[ iport + PAGE_COM_START ].PortNumber, prompts, k, Messages );

                                    //strcpy( string3, pcapcode[ iport ] );
                                    strcpy_s<sizeof(string3)>( string3, pcapcode[ iport ] );
                                    //strcpy( string4, pmessage[ iport ] );
                                    strcpy_s<sizeof(string4)>(string4, pmessage[ iport ] );

                                    string3[ strlen( string3 ) - 1 ] = 0;
                                    string4[ strlen( string4 ) - 1 ] = 0;

                                    PrintMessage(MSGTYPE_SYSTEM, 
													hwndComMsg,
													"T - Playing voice capcode: %s, message: %s, Port %i.",
													string3,
													string4,
													page[iport + PAGE_COM_START].PortNumber);
                                }
                                else{
									k = page[ iport + PAGE_COM_START ].PortNumber;
									dx_clrdigbuf( port[ k ].d4xHandle );

                                    k = PlayFil(page[ iport + PAGE_COM_START ].PortNumber,
												page[ iport + PAGE_COM_START ].FileName,
												&port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle, 0 );

                                    PrintMessage(MSGTYPE_SYSTEM, 
													hwndComMsg,
													"Playing voice file 1: [%s], File Handle: [%i].",
													page[ iport + PAGE_COM_START ].FileName,
													port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle );
                                }
                                break;

                            case 'B':
                                pstate[ iport ] = PHANGUP;
                                PrintMessage(MSGTYPE_SYSTEM,  
												hwndComMsg, 
												"Tone Only Page");
                                break;

                            default:
								k = page[ iport + PAGE_COM_START ].PortNumber;
								dx_clrdigbuf( port[ k ].d4xHandle );

                                k = PlayFil(page[ iport + PAGE_COM_START ].PortNumber,
											page[ iport + PAGE_COM_START ].FileName,
											&port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle, 0);

                                PrintMessage(MSGTYPE_SYSTEM, 
												hwndComMsg,
												"Playing voice file 2: [%s], File Handle: [%i].",
												page[ iport + PAGE_COM_START ].FileName,
												port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle);
                                break;
                        }

                        if( pstate[ iport ] != PHANGUP )
							{
                            pstate[ iport ] = PTALKING;
							}

                        if(ComPort[iport].timer.wHour == 70) 
							{
							GetLocalTime(&ComPort[iport].timer);
							}

                        k = Wait( &ComPort[ iport ].timer, (long)param.voicePageTime );

                        PrintMessage(MSGTYPE_SYSTEM,  
										hwndComMsg, 
										"Voice page talking");
                        if( k == 1 )
							{
                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											"Voice Page Timeout");

                            if( port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle > 0 )
								{
                                dx_fileclose( port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle );
                                port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle = 0;
								}

                            port[ page[ iport + PAGE_COM_START ].PortNumber ].state = ST_VP_IDLE;
                            port[ page[ iport + PAGE_COM_START ].PortNumber ].timer.wHour = 70;
                            k = page[ iport + PAGE_COM_START ].PortNumber;
							XnSetHook( k, DX_ONHOOK );
                            UpdatePhoneLineStatusWindow( k, "V Page Completed" );
                            break;
                        }
                        break;

                    default:
                        if(ComPort[iport].timer.wHour == 70)
							{
							GetLocalTime(&ComPort[iport].timer);
							}

                        k = Wait( &ComPort[ iport ].timer, 4500L );

                        if( k == 1 && pstate[ iport ] != PTALKNOWBUSY )
							{
                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											"Voice page timer expired");

                            page[ iport + PAGE_COM_START ].PageStatus[ 0 ] = 'A';
                            ComPort[ iport ].Errors++;
                            SetQueueError( iport, -1, 1 );
                            ComPort[ iport ].state = RPNOTREADY;
                            pstate[ iport ] = PRESET;
							}
                        break;
					}
                break;

            default:
                break ;
			}
		}
	}


	void
AbortComPage(int iport)
	{
    int j;
    int k;
    int l;
	int m;
	char string2[400];	//nothing a few more bytes can't fix
	char string3[80];

    // Maximum number of retries exceeded for this page
	//

	// notify the page supervisor.
	//
    if(param.SupperExt[0] != 0)
		{
		WriteSuppLog_Cancelled(iport + PAGE_COM_START, 
								FALSE, 
								param.SupperExt);
		}

	// leave a "user" message, if configured to do so.
	//
    if(param.UserMsg[0] != 'N')
		{
		WriteSuppLog_Cancelled(iport + PAGE_COM_START, 
								TRUE, 
								page[iport + PAGE_COM_START].Extension);
		}

   // sprintf(string2,
			//"CANCELLED Serial page - Number [%s], Display [%s], ProfileId [%s], attempts [%i], queue [%s].",
			//page[iport + PAGE_COM_START].PagerNumber,
			//page[iport + PAGE_COM_START].DisplayInfo,
			//page[iport + PAGE_COM_START].Extension,
			//page[iport + PAGE_COM_START].attempts,
			//page[iport + PAGE_COM_START].PageType);

	sprintf_s<sizeof(string2)>(string2,
		"CANCELLED Serial page - Number [%s], Display [%s], ProfileId [%s], attempts [%i], queue [%s].",
		page[iport + PAGE_COM_START].PagerNumber,
		page[iport + PAGE_COM_START].DisplayInfo,
		page[iport + PAGE_COM_START].Extension,
		page[iport + PAGE_COM_START].attempts,
		page[iport + PAGE_COM_START].PageType);


	page[iport + PAGE_COM_START].attempts = 0;
    page[iport + PAGE_COM_START].PageStatus[0] = 'I';

    ComPort[iport].Cancels++;
	SetQueueCancel(iport, -1);

    k = page[iport + PAGE_COM_START].ComOffset;  // Page Id

    PrintMessage(MSGTYPE_SYSTEM,  
					hwndComMsg, 
					string2);

    WriteLog("NOPGERR", string2);

	if(ComPort[iport].errorResponse[0] != 0)
		{
		//sprintf(string2, "     Response : %s\r\n\r\n", ComPort[iport].errorResponse);
		sprintf_s<sizeof(string2)>(string2, "     Response : %s\r\n\r\n", ComPort[iport].errorResponse);
	    WriteLog("NOPGERR", string2);
		}

	if(strcmp(page[iport + PAGE_COM_START].InitiatorId, "Code List") == 0 ||
		strcmp(page[iport + PAGE_COM_START].InitiatorId, "CATS PG") == 0)
		{
		//sprintf(string3, "XPCD%2.2s%2.2s.xnl", xnSystemTime, xnSystemTime + 3);
		sprintf_s<sizeof(string3)>(string3, "XPCD%2.2s%2.2s.xnl", xnSystemTime, xnSystemTime + 3);

		memset(string2, 0, sizeof(string2));

		//sprintf(string2,
		//		"Page Cancelled : Profile Id: %10.10s, Pager Id: %10.10s, Initiated: %8.8s, PageType : %%%2.2s",
		//		page[iport + PAGE_COM_START].Extension,
		//		page[iport + PAGE_COM_START].PagerId,
		//		page[iport + PAGE_COM_START].InitiatedTime,
		//		page[iport + PAGE_COM_START].PageType);

		sprintf_s<sizeof(string2)>(string2,
				"Page Cancelled : Profile Id: %10.10s, Pager Id: %10.10s, Initiated: %8.8s, PageType : %%%2.2s",
				page[iport + PAGE_COM_START].Extension,
				page[iport + PAGE_COM_START].PagerId,
				page[iport + PAGE_COM_START].InitiatedTime,
				page[iport + PAGE_COM_START].PageType);

		WriteLog(string3, string2);

		//sprintf(string2,
		//		"                Number Sent: %.15s, Id Number: %.15s, Message Sent: %.40s, Voice File %.40s",
		//		page[iport + PAGE_COM_START].PagerNumber,
		//		page[iport + PAGE_COM_START].AutCode,
		//		page[iport + PAGE_COM_START].DisplayInfo,
		//		page[iport + PAGE_COM_START].FileName);

		sprintf_s<sizeof(string2)>(string2,
				"                Number Sent: %.15s, Id Number: %.15s, Message Sent: %.40s, Voice File %.40s",
				page[iport + PAGE_COM_START].PagerNumber,
				page[iport + PAGE_COM_START].AutCode,
				page[iport + PAGE_COM_START].DisplayInfo,
				page[iport + PAGE_COM_START].FileName);

		WriteLog(string3, string2);
		}

	CancelledPageProcessing(iport + PAGE_COM_START);

    SetQueueError(iport, -1, 2);

	if(pageactive[iport] == ZETRON15)
		{
		for(j = 0; j < 15; j++)
			{
			if(pcapcode[iport][0] == param.PFormat[j].Id[0])
				{
				k = pcapcode[ iport ][ param.PFormat[ j ].Position ];
				for( l=0; l<( int )strlen( param.PFormat[ j ].VoiceFuncs ); l++ )
					{
					if( k == param.PFormat[ j ].VoiceFuncs[ l ] )
						{
						if( port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle > 0 )
							{
							dx_fileclose( port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle );
							port[ page[ iport + PAGE_COM_START ].PortNumber ].file_handle = 0;
							}

						port[ page[ iport + PAGE_COM_START ].PortNumber ].state = ST_VP_IDLE;
						port[ page[ iport + PAGE_COM_START ].PortNumber ].timer.wHour = 70;
						m = page[ iport + PAGE_COM_START ].PortNumber;
						XnSetHook( m, DX_ONHOOK );

						if( param.VoiceDelete && page[ iport + PAGE_COM_START ].FileName[ 0 ] != 0 )
							{
							if( strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "Code List" ) != 0 &&
								strcmp( page[ iport + PAGE_COM_START ].InitiatorId, "XP CS" ) != 0 )
								{
								SetFileAttributes( page[ iport + PAGE_COM_START ].FileName, FILE_ATTRIBUTE_NORMAL );
								if( remove( page[ iport + PAGE_COM_START ].FileName ) == 0 )
									{
									PrintMessage(MSGTYPE_SYSTEM,  
													hwndComMsg,
													"Voice file: %s removed.",
													page[ iport + PAGE_COM_START ].FileName );
									}
								else
									{
									PrintMessage(MSGTYPE_SYSTEM,  
													hwndComMsg,
													"Unable to remove voice file: %s.",
													page[ iport + PAGE_COM_START ].FileName );
									}
								}
							}
						UpdatePhoneLineStatusWindow(page[ iport + PAGE_COM_START].PortNumber,
													"V Page Cancelled");
						break;
						}
					}
				break;
				}
			}
		}
	}


	// ProcessCommportQueue() - process comm port queue.
	//
	// the first parameter QueueUse, is the comm port (0 thru 33) to use.
	//  the comm port offset must be added, to correctly index into the block of 
	//  page[] data structures (0 thru 135).
	//
	// the second parameter j, is the offset into the comm port parameter block:
	//  ComPort[QueUse].InQueuePointer[j], these are the 20 items (0 thru 19) 
	//  seen in the Page Configuration Utility that defines information 
	//  about each page device type serviced by that comm port.
	//
	void
ProcessCommportQueue(int QueUse, int j)
	{
    int k;
    int n;
    int m;
    int i;
	char string2[40];

 	PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "ProcessCommportQueue() - begin:");

	bool want3 = (pageactive[QueUse] == IXO_MODEM); 

	int MessageSegements = want3 ? 3 : 2;

	if(ClaimQueueItem(ComPort[QueUse].InQueuePointer[j], MessageSegements))
		{
		GetQueue(QueUse + PAGE_COM_START, QueUse, ComPort[QueUse].InQueuePointer[j], want3, __FILE__, __LINE__);

		if(strlen(page[QueUse + PAGE_COM_START].PagerNumber) > 0)
			{
			ComPort[QueUse].Attempts++;

			if(strncmp(page[QueUse + PAGE_COM_START].InitiatorId, "External", 8) == 0)
				{
				ComPort[QueUse].External++;
				}

			memset(pstatus[QueUse], 0, sizeof(pstatus[QueUse]));
			memset(pcapcode[QueUse], 0, sizeof(pcapcode[QueUse]));
			memset(pmessage[QueUse], 0, sizeof(pmessage[QueUse]));
			memset(pauthcode[QueUse], 0, sizeof(pauthcode[QueUse]));
			memset(ComPort[QueUse].errorResponse, 0, sizeof(ComPort[QueUse].errorResponse));

			//strncpy(pcapcode[QueUse], 
			//		page[QueUse + PAGE_COM_START].PagerNumber, 
			//		sizeof(page[QueUse + PAGE_COM_START].PagerNumber));


			strncpy_s<sizeof(pcapcode[0])>(pcapcode[QueUse], 
					page[QueUse + PAGE_COM_START].PagerNumber, 
					sizeof(page[QueUse + PAGE_COM_START].PagerNumber));

			//strncpy(pmessage[QueUse],
			//page[QueUse + PAGE_COM_START].DisplayInfo,
			//sizeof(page[QueUse].DisplayInfo));

			strncpy_s<sizeof(pmessage[0])>(pmessage[QueUse],
			page[QueUse + PAGE_COM_START].DisplayInfo,
			sizeof(page[0].DisplayInfo));


			//copy long message
			messageString[QueUse] = page[QueUse + PAGE_COM_START].longMessage;

			//strncpy(pauthcode[QueUse], page[QueUse + PAGE_COM_START].AutCode, 15);
			strncpy_s<sizeof(pauthcode[0])>(pauthcode[QueUse], page[QueUse + PAGE_COM_START].AutCode, 15);

			if(page[QueUse + PAGE_COM_START].PageType[0] != 0)
				{
				if(strstr(param.TwoToneQues,
							page[QueUse + PAGE_COM_START].PageType) != NULL &&
							param.TwoToneQues[0] != 0)
					{
					TwoToneConvert(QueUse);
					}
				else if(strstr(param.MidFiles,
								page[QueUse + PAGE_COM_START].PageType) != NULL &&
								param.MidFiles[0] != 0)
					{
					TwoToneConvert(QueUse);
					}
				}

			page[QueUse + PAGE_COM_START].ScriptFlag = -99;
			page[QueUse + PAGE_COM_START].ScriptNumber = -1;
			page[QueUse + PAGE_COM_START].PortNumber = -1;
			page[QueUse + PAGE_COM_START].ComNumber = QueUse;

			memset(string2, 0, sizeof(string2));
			//sprintf(string2, "%%%2s", page[QueUse + PAGE_COM_START].PageType);
			sprintf_s<sizeof(string2)>(string2, "%%%2s", page[QueUse + PAGE_COM_START].PageType);
			page[QueUse + PAGE_COM_START].ComOffset = IdentifyComType(QueUse, string2);

			page[QueUse + PAGE_COM_START].PageStatus[0] = 'B';

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndComMsg,
							"Page on COM%i: [%i], CapCode [%s], Message [%s].",
							QueUse + 1,
							j,
							pcapcode[QueUse],
							page[QueUse + PAGE_COM_START].longMessage.c_str());

			page[QueUse + PAGE_COM_START].attempts = 1;
			pageerror[QueUse] = PNOERROR;

			if(pageactive[ QueUse ] == ZETRON15 )
				{
				ComPort[ QueUse ].state = RPPAGING;

				for(n = 0; n < 15; n++)
					{
					if( pcapcode[ QueUse ][ 0 ] == param.PFormat[ n ].Id[ 0 ] )
						{
						m = pcapcode[ QueUse ][ param.PFormat[ n ].Position ];
						for( i=0; i<( int )strlen( param.PFormat[ n ].VoiceFuncs ); i++ )
							{
							if( m == param.PFormat[ n ].VoiceFuncs[ i ] )
								{
								k = param.ComPort[ QueUse ].VoiceLine-1;
								//if( port[ k ].PortType == 6 )
								if(param.IVRPort[k].iType == 6 )
									{
									page[ QueUse + PAGE_COM_START ].PortNumber = k;
									UpdatePhoneLineStatusWindow( k, "Voice Page Ready" );
									port[ k ].state = ST_VP_PAGE;
									port[ k ].LoopCurrent = TRUE;
									XnSetHook( k, DX_OFFHOOK );
									}
								ComPort[ QueUse ].state = RPVOICE;
								break;
								}
							}
						break;
						}
					}
				if(pmessage[QueUse][0] == 'E' &&
					pcapcode[QueUse][0] == '8' &&
					pcapcode[QueUse][7] == '7')
					{
					pcapcode[QueUse ][7] = '8';
					}
				}
			else
				{
				ComPort[QueUse].state = RPPAGING;
				}

			GetLocalTime(&ComPort[QueUse].timer);

			if(ComPort[QueUse].state != RPWAITING)
				{
				// for the dial-up modem type, if the phone number for the most recently
				//  dialed paging service is the same as the one for the current request,
				//  no-sense to hangup, just to dial the exact same number, 
				//  remain online, and send the next page request.
				//
				if(pageactive[QueUse] == IXO_MODEM)
					{
					if(pstate[QueUse] == PREADY2)
						{
						if(strncmp(pcapcode[QueUse],
									ComPort[QueUse].oldCapcode,
									strlen(pcapcode[QueUse])) == 0)
							{
							// if the phone number is the same for the next page request, no need to redial
							//  just jump into PSEND8, to prepare the next TAP page request message.
							pstate[QueUse] = PSEND8;
							}
						else
							{
							// different phone number, must hangup and start from the "beginning".
							pstate[QueUse] = PHANGUP;
							}
						}
					else
						{
						pstate[QueUse] = PSEND;
						}
					memset(ComPort[QueUse].oldCapcode, 0, sizeof(ComPort[QueUse].oldCapcode));
					//sprintf(ComPort[QueUse].oldCapcode, "%-29.29s", pcapcode[QueUse]);
					sprintf_s<sizeof(ComPort[0].oldCapcode)>(ComPort[QueUse].oldCapcode, "%-29.29s", pcapcode[QueUse]);
					}
				else
					{
					pstate[QueUse] = PSEND;
					}
				}
			else
				{
				pstate[QueUse] = PWAIT1;
				}
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "Null Page.");
			page[QueUse + PAGE_COM_START].PageStatus[0] = 'I';
			}
		}
	}




	void
TwoToneConvert(int Cport)
	{
    int i;
    int j;

    PrintMessage(MSGTYPE_SYSTEM, 
					hwndComMsg, 
					"Two tone conversion. Cport [%d] input string [%s]", 
					Cport, 
					pcapcode[Cport]);

	// copy the "2 tone string" e.g. "B0101" to be converted 
	//  into a temporary string.
	char string2[80];
    memset(string2, 0, sizeof(string2));
    //strcpy(string2, pcapcode[Cport]);
    strcpy_s<sizeof(string2)>(string2, pcapcode[Cport]);

	// clear out the original string, 
	//  it is also the destination storage location.
    memset(pcapcode[Cport], 0, sizeof(pcapcode[Cport]));

    pcapcode[Cport][0] = param.TwoToneId[0];

    j = string2[strlen(string2) - 3] - 48;

    pcapcode[Cport][1] = string2[strlen(string2) - 4];

    for(i = 0; i < 25; i++)
		{
        if(strncmp(string2, param.TwoTone[i].Id, strlen(string2) - 4) == 0)
			{
            //strcat(pcapcode[Cport], param.TwoTone[i].Array[j]);
            strcat_s<sizeof(pcapcode[0])>(pcapcode[Cport], param.TwoTone[i].Array[j]);

			//strcat(pcapcode[Cport], string2 + strlen(string2) - 2);
			strcat_s<sizeof(pcapcode[0])>(pcapcode[Cport], string2 + strlen(string2) - 2);
            break;
			}
		}

    PrintMessage(MSGTYPE_SYSTEM, 
					hwndComMsg, 
					"output string [%s]", 
					pcapcode[Cport]);
	}


	void
GroupCatCall( int CatsPointer )
	{
    int i;
    int k;
	char string2[ 300 ];

	for(i = 0; i < MAX_COMMPORTS; i++)
		{
        if( pageactive[i] == GROUPCALL )
			{
            switch( ComPort[ i ].state )
				{
                case RPNOTREADY:
                    if( pstate[ i ] == PREADY )
						{
                        ComPort[ i ].state = RPREADY;
						}
                    else
						{
                       // if timeout force psate to preset 
                        if( ComPort[ i ].timer.wHour == 70 )
							{
							//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPNOTREADY - GroupCatCall.");
							GetLocalTime( &ComPort[ i ].timer );
							}

                        k = Wait( &ComPort[ i ].timer, 3000L );

                        if( k == 1 )
							{
                            if( page[ i + PAGE_COM_START ].PageStatus[ 0 ] != 'A' )
                                page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'I';
                            ComPort[ i ].state = RPNOTREADY;
                            pstate[ i ] = PRESET;
                            ComPort[ i ].timer.wHour = 70;
                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg,
											"Resetting CATS Serial Port COM%i: .", 
											i+1);
							}
						}
                    break;

                case RPABORT:
                    if( pstate[ i ] == PREADY ){
                        ComPort[ i ].state = RPREADY;
                    }
                    else if( pstate[ i ] == PRESET )
                        ComPort[ i ].state = RPNOTREADY;
                    else{
                       /*  if timeout force psate to preset  */
                        if( ComPort[ i ].timer.wHour == 70 )
							{
							//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPABORT - GroupCatCall.");
							GetLocalTime( &ComPort[ i ].timer );
							}

                        k = Wait( &ComPort[ i ].timer, 1000L );

                        if( k == 1 ){
                            if( page[ i + PAGE_COM_START ].PageStatus[ 0 ] != 'A' )
                                page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'I';
                            ComPort[ i ].state = RPNOTREADY;
                            pstate[ i ] = PRESET;
                            ComPort[ i ].timer.wHour = 70;

                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											"Resetting CATS Serial Port COM%i: .", 
											i+1);
                        }
                    }
                    break;

                case RPREADY:
                    if(pstate[i] == PREADY)
						{
                        if( page[ i + PAGE_COM_START ].PageStatus[ 0 ] == 'A' )
							{
                            if( page[ i + PAGE_COM_START ].attempts+1 <=  param.ComPort[ i ].Retry )
								{
                                if(Cats[CatsPointer].timer.wHour == 70) 
									{
									//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPREADY 1 - GroupCatCall.");
									GetLocalTime(&Cats[CatsPointer].timer);
									}

                                k = Wait( &Cats[ CatsPointer ].timer, 300L );

                                if( k == 1 )
									{
                                    Cats[ CatsPointer ].timer.wHour = 70;
                                    pageerror[ i ] = PNOERROR;
                                    ComPort[ i ].state = RPPAGING;
                                    page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'B';
                                    page[ i + PAGE_COM_START ].attempts++;

           //                         sprintf(string2, 
											//"Retrying CATS group call on COM%i: Button %s.", 
											//i + 1, 
											//pcapcode[i]);

                                    sprintf_s<sizeof(string2)>(string2, 
											"Retrying CATS group call on COM%i: Button %s.", 
											i + 1, 
											pcapcode[i]);

                                    PrintMessage(MSGTYPE_SYSTEM,  
													hwndComMsg, 
													string2);

                                    WriteLog( "XTPGCATS.MSG", string2 );

									//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPREADY 2 - GroupCatCall.");
                                    GetLocalTime( &ComPort[ i ].timer );

                                    pstate[ i ] = PSEND;
									}
								}
                            else
								{
                                // Maximum number of retries exceeded for this page
                                if(param.SupperExt[0] != 0)
									{
									WriteSuppLog_Cancelled(i + PAGE_COM_START, 
															TRUE, 
															param.SupperExt);
									}

          //                      sprintf(string2, 
										//"CATS Call - %s, cancelled after %i attempts.", 
										//Cats[CatsPointer ].FileName, 
										//page[i + PAGE_COM_START].attempts);

                                sprintf_s<sizeof(string2)>(string2, 
										"CATS Call - %s, cancelled after %i attempts.", 
										Cats[CatsPointer ].FileName, 
										page[i + PAGE_COM_START].attempts);

                                page[ i + PAGE_COM_START ].attempts = 0;
                                page[ i + PAGE_COM_START ].PageStatus[0] = 'I';

                                ComPort[ i ].Cancels++;
								SetQueueCancel(i, -1);

                                PrintMessage(MSGTYPE_SYSTEM,  
												hwndComMsg, 
												string2);

                                WriteLog( "XTPGCATS.MSG", string2 );
                                memset( pcapcode[i], 0, sizeof(pcapcode[i]));
                                memset( pmessage[i], 0, sizeof(pmessage[i]));
                                Cats[ CatsPointer ].GroupCallWaiting--;
                                CheckCatsState();
                            }
                            break;
                        }

						// Check and start processing outging page ques for radio interface,
						// or initialize a keyboard page.
                        if( Cats[ CatsPointer ].GroupCallWaiting && pcapcode[ i ][ 0 ] != 0 )
							{
                            if( Cats[ CatsPointer ].timer.wHour == 70 )
								{
								//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPREADY 3 - GroupCatCall.");
								GetLocalTime( &Cats[ CatsPointer ].timer );
								}

                            k = Wait( &Cats[ CatsPointer ].timer, 200L );

                            if( k == 1 )
								{
                                Cats[ CatsPointer ].timer.wHour = 70;
                                ComPort[ i ].Attempts++;

          //                      sprintf(string2, 
										//"CATS group call on COM%i: , Button %s.", 
										//i + 1, 
										//pcapcode[i]);
                                sprintf_s<sizeof(string2)>(string2, 
										"CATS group call on COM%i: , Button %s.", 
										i + 1, 
										pcapcode[i]);

                                PrintMessage(MSGTYPE_SYSTEM,  
												hwndComMsg, 
												string2);

                                WriteLog( "XTPGCATS.MSG", string2 );
                                page[ i + PAGE_COM_START ].ScriptFlag = -99;
                                page[ i + PAGE_COM_START ].ScriptNumber = -1;
                                page[ i + PAGE_COM_START ].PortNumber = -1;
                                page[ i + PAGE_COM_START ].ComNumber = i;
                                page[ i + PAGE_COM_START ].ComOffset = 18;
                                page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'B';
                                page[ i + PAGE_COM_START ].attempts = 1;
                                pageerror[ i ] = PNOERROR;
                                ComPort[ i ].state = RPPAGING;

								//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPREADY 4 - GroupCatCall.");
                                GetLocalTime( &ComPort[ i ].timer );

                                pstate[ i ] = PSEND;
								}
                            break;
							}
						}
                    else
						{
                        ComPort[ i ].state = RPNOTREADY;
						}

                    break;

                case RPPAGING:
                    if( pstate[ i ] == PREADY ){
                        if( pageerror[ i ] == PNOERROR )
							{
                            Cats[ CatsPointer ].GroupCallWaiting--;
                            ComPort[ i ].state = RPREADY;
                            page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'I';
                            memset( pcapcode[ i ], 0, sizeof(pcapcode[i]));
                            memset( pmessage[ i ], 0, sizeof(pmessage[i]));
                            CheckCatsState( );
							}
                        else
							{
                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											"Group reset due to error condition");

                            page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'A';
                            ComPort[ i ].state = RPNOTREADY;
                            ComPort[ i ].Errors++;
                            pstate[ i ] = PABORT;
							}
						}
                    else
						{
                        if( ComPort[ i ].timer.wHour == 70 )
							{
							//PrintMessage(MSGTYPE_SYSTEM, hwndComMsg, "begin timeout - RPPAGING - GroupCatCall.");
							GetLocalTime( &ComPort[ i ].timer );
							}

                        k = Wait( &ComPort[ i ].timer, 1000L );

                        if( k == 1 )
							{
                            k = param.ComPort[ param.Cats[ CatsPointer ].Group1Port-1 ].VoiceLine;
                            if( Cats[ CatsPointer ].Group1On == 0 )
								{
                                if( param.Cats[ CatsPointer ].Group1Port-1 == i && Cats[ CatsPointer  ].Group1On == 0 )
									{
                                    PrintMessage(MSGTYPE_SYSTEM,  
													hwndComMsg, 
													"Group Call phone 1 not completed");

                                    page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'A';
                                    ComPort[ i ].state = RPABORT;
                                    ComPort[ i ].timer.wHour =  70;
                                    ComPort[ i ].Errors++;
                                    pstate[ i ] = PABORT;
                                    port[ k ].timer.wHour = 70;
                                    port[ k ].PortStatus = 0;
                                    port[ k ].state = ST_ONHK_DELAY;
									XnSetHook( k, DX_ONHOOK );
									}
								}
                            if( Cats[ CatsPointer ].Group2On == 0 )
								{
                                k = param.ComPort[ param.Cats[ CatsPointer ].Group2Port-1 ].VoiceLine;
                                if( param.Cats[ CatsPointer ].Group2Port-1 == i && Cats[ CatsPointer  ].Group2On == 0 )
									{
                                    PrintMessage(MSGTYPE_SYSTEM,  
													hwndComMsg, 
													"Group Call phone 2 not completed");

                                    page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'A';
                                    ComPort[ i ].state = RPABORT;
                                    ComPort[ i ].timer.wHour = 70;
                                    ComPort[ i ].Errors++;
                                    pstate[ i ] = PABORT;
                                    port[ k ].PortStatus = 0;
                                    port[ k ].timer.wHour = 70;
                                    port[ k ].state = ST_ONHK_DELAY;
									XnSetHook( k, DX_ONHOOK );
									}
								}
							}

						if(pstate[ i ] != PHANGUP && 
							pstate[ i ] != PHANG2 && 
							pstate[ i ] != PHANG3 && 
							pstate[ i ] != PHANG4 && 
							pstate[ i ] != PABORT )
							{
							k = Wait( &ComPort[ i ].timer, 8000L );
							if( k == 1 )
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndComMsg, 
												"Group call timeout");

								page[ i + PAGE_COM_START ].PageStatus[ 0 ] = 'A';
								ComPort[ i ].state = RPNOTREADY;
								ComPort[ i ].timer.wHour = 70;
								ComPort[ i ].Errors++;
								pstate[ i ] = PRESET;
								}
							}
						}
                    break;

                default:
                    break ;
				}
			}
		}
	}


	void
CheckCatsState(void)
	{
    int	j;
    int k;
    int l;
	char string2[40];

    k = 0;
    for( j=0; j<4; j++ )
		if( Cats[ j ].GroupCallWaiting )
			k = 1;

    if( k == 0 )
		{
        for( l=0; l<4; l++ )
			{
            if( param.Cats[ l ].Line > 0 )
				{
                port[ param.Cats[ l ].Line ].timer.wHour = 70;
                memset( string2, 0, sizeof( string2 ) );
                //sprintf(string2, "%Idle");
                sprintf_s<sizeof(string2)>(string2, "%Idle");

                UpdatePhoneLineStatusWindow(param.Cats[l].Line, string2);
				}
			}
        CatsCall = FALSE;
        //sprintf( string2, "Cat call completed.\r\n" );
        sprintf_s<sizeof(string2)>( string2, "Cat call completed.\r\n" );
        WriteLog( "XTPGCATS.MSG", string2 );
		}
	}

