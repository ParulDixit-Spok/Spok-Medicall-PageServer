
#include "common.h"
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <time.h>



#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "List2.h"
#include "Impairment.h"


// xnpgd4x.cpp
#pragma warning( disable : 4786 )
#pragma warning( disable : 4996 )


using namespace std;

#define SIGEVTCHK       0x1010  /* Check for type of signaling event*/
#define SIGBITCHK       0x1111  /* Check for signalling bit change  */
int Exit_State(int, int, int, DX_CST *);
int curr_state(int, int, int, DX_CST *);
int T1EventHdlr(int, int, DX_CST *);


	void
CatsFinalize(int channel)
	{
	int i;

	PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "CatsFinalize(): ");

    for(i = 0; i < 4; i++)
		{
        Cats[i].GroupCallWaiting = 0;
        if(param.Cats[i].Line > 0)
			{
            port[param.Cats[i].Line].timer.wHour = 70;
            port[param.Cats[i].Line].state = ST_IDLE;

			UpdatePhoneLineStatusWindow(param.Cats[i].Line, "Idle");
			}
		}
    CatsCall = FALSE;

    page[channel].PageStatus[0] = 'I';

    if(port[channel].file_handle > 0)
		{
        dx_fileclose(port[channel].file_handle);
        port[channel].file_handle = 0;
		}

	port[channel].timer.wHour = 70;
	}


	bool GetMaxScheduleCount(int* count)
	{
		CString csSQLString;
		CString csFunctionName("GetScheduleCount");
		CString csTableName("Schedule");
		int iStatus;
		int max = 1;
		
		*count=0;
		csSQLString.Format(
			"SELECT COUNT(1) AS scount FROM Schedule WHERE RecordType = 7");
		
		iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
		
		if(iStatus == DBA_RECORD_ACCESS_ERROR) return false;
		
		if(iStatus != DBA_RECORD_NOT_FOUND)
		{
			pDBA->ADOGetNumeric("scount", count);
			csSQLString.Format(
				"SELECT MAX(RecordCount) AS max FROM Schedule WHERE RecordType = 7");
			
			iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
			if((iStatus == DBA_RECORD_NOT_FOUND) || (iStatus == DBA_RECORD_ACCESS_ERROR)) return false;
			pDBA->ADOGetNumeric("max", &max);
		}		
		
		if(*count > max) *count = max;
		return true;
	}


	bool GetMaxMessageNumber(char* profileId, int* max)
	{
		CString csSQLString;
		CString csFunctionName("MaxMessageNumber");
		CString csTableName("Messages");
		int iStatus;
		
		*max=0;

		csSQLString.Format(
			"SELECT MAX(MessageNumber) AS max FROM Messages WHERE ProFileId = '%10.10s'",
			profileId);
		
		iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
		if((iStatus == DBA_RECORD_NOT_FOUND) || (iStatus == DBA_RECORD_ACCESS_ERROR)) return false;

		pDBA->ADOGetNumeric("max", max);
		return true;
	}

	bool LookupProfile(char* inp, char* profileId, int sizeProfileId)
	{
		CString csSQLString;
		CString csFunctionName("LookupProfile");
		CString csTableName("Pagers");
		int iStatus;

		csSQLString.Format(
			"SELECT ProfileId FROM Pagers WHERE "
			"PagerId ='%10.10s'", inp);

		iStatus = pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);
		if((iStatus == DBA_RECORD_NOT_FOUND) || (iStatus == DBA_RECORD_ACCESS_ERROR)) return false;
		
		pDBA->ADOGetString("ProfileId", profileId, sizeProfileId);
		return true;
	}

	CString GetHeaderDate()
	{
        struct tm *newtime;
        time_t long_time;

        time( &long_time );                /* Get time as long integer. */
        newtime = localtime( &long_time ); /* Convert to local time. */
		
		CString date;
		date.Format("%04d%02d%02d%02d%02d",
		newtime->tm_year + 1900, newtime->tm_mon + 1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min);

		return date;
	}

	bool ForwardEmail(const char* inp, int msgNumber)
	{
		
		if(*inp == 0) return false;
		CString csFunctionName("ForwardEmail");
		CString csTableName;
		CString csSQLString;
		char profileId[11];
		char pagerId[11];
		int count;
		bool success;

		if(strlen(inp) > 10) return false;
		strncpy(pagerId, inp, sizeof(pagerId));

		//remove # sign
		char* pound = strchr(pagerId, '#');
		if(*pound) *pound = 0;


		success = LookupProfile(pagerId, profileId, sizeof(profileId));
		if(!success) return false;

		CString date;

		date = GetHeaderDate();

		GetMaxScheduleCount(&count);
		count++;
		csTableName.Format("Schedule");


		CString generalInfo;
		generalInfo.Format("SC%10d    PageServer%s", msgNumber, "%DE");

		csSQLString.Format(
			"INSERT INTO Schedule"
			"(Header, RecordType, Extension, RecordCount, GeneralInformation) "
			"VALUES ('%12.12s', 7, '%10.10s', %d, '%s') "
			"--SQL-3-0000",
			static_cast<LPCTSTR>(date),
			profileId,
			count,
			static_cast<LPCTSTR>(generalInfo));
	
		pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
		return true;
	}



	bool CopyToDest(const string& source, const string& destDir, const string& fname)
	{
		BOOL fSuccess;
		string fullDest(destDir);

		fullDest += fname;
		fSuccess = CopyFile(source.c_str(), fullDest.c_str(), FALSE);
		if(!fSuccess)
		{
			//create directory
			CreateDirectory(destDir.c_str(), NULL);
			fSuccess = CopyFile(source.c_str(), fullDest.c_str(), FALSE);
		}
		return (fSuccess?true:false);
	}




	class LineMessage
	{
	public:
		LineMessage(string msg, int lineNo) : message(msg), lineNumber(lineNo)
		{}
		string GetMessage() {return message;}
		int GetLineNumber(){return lineNumber;}

	protected:
		string message;
		int lineNumber;
	};



	bool ForwardPage(const char* inp, int msgNumber, char* fromProfile)
	{
		if(*inp == 0) return false;
		CString csFunctionName("ForwardPage");
		CString csTableName;
		CString csSQLString;
		char profileId[11];
		char pagerId[11];
		int maxMsg;
		bool success;
		
		strncpy(pagerId, inp, sizeof(pagerId));
		//remove # sign
		char* pound = strchr(pagerId, '#');
		if(*pound) *pound = 0;
		
		
		success = LookupProfile(pagerId, profileId, sizeof(profileId));
		if(!success) return false;
		

		if(!GetMaxMessageNumber(profileId, &maxMsg)) return false;
		++maxMsg;
		//update message
		csTableName = "Messages";

		CString date;
		date = GetHeaderDate();



		csSQLString.Format(
			"INSERT INTO Messages"
			"(ProfileId, Header, MessageType, MessageNumber, MessageFrom, Delivered, Operator, CarbonCopy) "
			"VALUES ('%10.10s', '%12.12s', '%2.2s', %d, 'XN Paging Server    ', '0', 'XN PS     ', '0') "
			"--SQL-3-0001",
			fromProfile, 
			static_cast<LPCTSTR>(date), 
			voiceMessageTypeStr,
			maxMsg);
		
		pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);


		
		//need to insert into messagelines

		csSQLString.Format(
			"SELECT LineNumber, Line FROM MessageLines "
			"WHERE ProfileId = '%10.10s' AND MessageNumber = %d",
			fromProfile, msgNumber);



		pDBA->ADORsSelect(csFunctionName, csTableName, csSQLString);

		vector<LineMessage> message;
		char charMessage[256];		//line of text from DB
		int lineNo;

		//collect lines for message
		do
		{
			pDBA->ADOGetString("Line", charMessage, sizeof(charMessage));
			pDBA->ADOGetNumeric("LineNumber", &lineNo);
			message.push_back(LineMessage(string(charMessage), lineNo));
		}while(pDBA->ADOStep() != FAIL);

		

		//forward to new profile
		vector<LineMessage>::iterator msgIter;
		msgIter = message.begin();
		for(; msgIter != message.end(); ++msgIter)
		{
			//write to DB
				
			csSQLString.Format(
				"INSERT INTO MessageLines"
				"(ProfileId, MessageNumber, LineNumber, Line) "
				"VALUES ('%10.10s', %d, %d, '%s') "
				"--SQL-3-0001",
				profileId,
				maxMsg,
				msgIter->GetLineNumber(),
				static_cast<const char*>(msgIter->GetMessage().c_str())
				);
			
			pDBA->ADORsInsert(csFunctionName, csTableName, csSQLString);
			
		}

		string toFileName(profileId);

		//trim left
		toFileName.erase (0, toFileName.find_first_not_of (" \r\n\t")); 

		//copy file
		//from, to
		ostringstream destDir;
		ostringstream sourceFile;
		ostringstream destFile;

		sourceFile << pageFileDir << GetLookupDir(fromProfile) << fromProfile << '\\' << msgNumber << '.' << "wav" << ends;
		destDir  << pageFileDir << GetLookupDir(toFileName) << toFileName << '\\' << "EXTRA\\" << ends;

		destFile << maxMsg << '.' << "wav" << ends;

		//copy file
		CopyToDest(sourceFile.str().c_str(), destDir.str().c_str(), destFile.str().c_str());

		return true;
	}


/****************************************************************
*        NAME : process_event(channel,evtcode,chdev)
* DESCRIPTION : begin initial processing of the event code
*       INPUT : channel = channel number
*             : evtcode = termination event code
*             : chdev   = dialogic channel handle
*      OUTPUT : none
*     RETURNS : new channel state
****************************************************************/
	int
process_event(int channel, int evtcode, int chdev, DX_CST * cstp)
	{
	char string2[300];
    int i;

	// check for certain termination events,
	// that can be handled regardless of the current state
	switch( evtcode )
		{
		case DTEV_SIG:
			port[channel].state = T1EventHdlr(channel, evtcode, cstp);
			break;

		case TDX_CST:
			switch( cstp->cst_event )
				{
				case DE_RINGS:		// Rings Recieved
				case DE_LCON:       // loop current on (cst event)
					PrintMessage(MSGTYPE_IVR, 
									hwndD4xMsg,
									"Line [%i] Ringing,  State [%s]",
									channel,
									DialogicStateName[port[channel].state]);

					if( port[channel].state == ST_IDLE )
						{
						port[ channel ].LoopCurrent = TRUE;
						UpdatePhoneLineStatusWindow( channel, "Ringing..." );

						if((param.IVRPort[channel].iType == 21 ||
							param.IVRPort[channel].iType == 27 ) &&
							port[channel].t1 == FALSE )
							{
        					// 21 == line type DPG == Dialogic DID-40  page in line
        					// 27 == line type DST == Dialogic DID-40  page status line
							if( page[ channel ].PageStatus[ 0 ] == 'I' )    // idle ??
								{
								page[ channel ].PageStatus[ 0 ] = 'B';		// busy ??
								page[ channel ].PortNumber = channel;

								ResetBuffer(channel);

								port[ channel ].PageNumber = channel;
								return( ST_DID_GET_EXT );
								}
							return( ST_IDLE );
							}

						if(bExternalControl && (PsOpMode == MODE_STANDBY))
							{
							return ST_IDLE;
							}

						if(param.IVRPort[channel].iType == 4 && CatsCall )
							{
							port[ channel ].PortStatus = 0;
							sprintf( string2, "1 Line %i CATS ringing.", channel );
							WriteLog( "XTPGCATS.MSG", string2 );

							PrintMessage(MSGTYPE_IVR,  
											hwndD4xMsg,
											string2);

							return( ST_IDLE );
							}

						if(param.IVRPort[channel].iType == 4 )
							CatsCall = TRUE;

						return( ST_OFFHK );
						}

				case DE_TONEOFF:             /* Global Tone Detection                */
					PrintMessage(MSGTYPE_IVR,  
									hwndD4xMsg, 
									"Line %i, tone event 0x%0x.", 
									channel, 
									evtcode);
					break;

				case DE_TONEON:              /* Global Tone Detection                */
					PrintMessage(MSGTYPE_IVR,  
									hwndD4xMsg, 
									"Line %i, tone event 0x%0x.", 
									channel, 
									evtcode);

					dx_stopch(chdev, EV_ASYNC);
					Sleep(100);

					i = dx_distone( chdev, param.GTD.tid, DM_TONEON|DM_TONEOFF );
					if( i != 0 )
						PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
										"Line %i ERROR.1!! Tone not disabled, CST event %i, return %i.",
										channel,
										cstp->cst_event,
										i );
					else
						PrintMessage(MSGTYPE_IVR,  
										hwndD4xMsg,
										"Line %i Tone disabled, CST event %i, return %i.",
										channel,
										cstp->cst_event,
										i );
					// fall into DE_LCOFF:

				case DE_LCOFF:         /* loop current off (cst event)                  */
					/*
					* loss of loop current,
					* (disconnect)
					*/
					if( port[ channel ].state != ST_IDLE && port[ channel ].state != ST_ONHK )
						{
						port[ channel ].LoopCurrent = FALSE;
						UpdatePhoneLineStatusWindow( channel, "Disconnect" );

						PrintMessage(MSGTYPE_IVR,  
										hwndD4xMsg, 
										"Line %i Analog Disconnect Detected", 
										channel);

						PrintMessage(MSGTYPE_IVR,  
										hwndD4xMsg, 
										"process_event() - state %d", 
										port[ channel ].state);

						PrintMessage(MSGTYPE_IVR,  
										hwndD4xMsg, 
										"DE_LCOFF - dx_stopch() and Exit_State()");

						dx_stopch(chdev, EV_ASYNC);
						// simple fix to give the dx_stopch() function a chance
						//  to clear out any subsequent messages regarding
						//  the channel that has just been stopped. these messages
						//  could be retrieved, and are unexpected by the state machine
						//  unfortunately, any unexpected messages are treated as errors
						//  when they really should be ignored.
						Sleep(100);
					//		PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "after dx_stopch()");

						return( Exit_State( channel, evtcode, chdev, cstp ) );
						}
					break;
			}

		case TDX_PLAY:
		case TDX_CALLP:		 	// termination of call progress analysis
		case TDX_RECORD:
		case TDX_PLAYTONE:
		case TDX_DIAL:			// termination of dialing digits
		case TDX_GETDIG:
		case TDX_SETHOOK:		// on/off hook complete
		case DTEV_WINKCPLT:	// Wink complete on T1 Line
			if( port[channel].state != ST_IDLE )
				return( curr_state( channel, evtcode, chdev, cstp ) );
			break;

		case DTEV_ERREVT:
            PrintMessage(MSGTYPE_IVR,  
							hwndD4xMsg,
							"Line %i ERROR Event Code 0x%0x, %s",
							channel,
							evtcode,
							ATDV_ERRMSGP( port[channel].d4xT1Handle ) );

            return( curr_state( channel, evtcode, chdev, cstp ) );

		case TDX_ERROR:
			PrintMessage(MSGTYPE_IVR,  
							hwndD4xMsg,
							"Dialogic - TDX_ERROR:  Channel %i _LASTERR %i _ERRMSGP:[%s] _NAMEP: [%s]",
							channel,
							ATDV_LASTERR( port[channel].d4xHandle ),
							ATDV_ERRMSGP( port[channel].d4xHandle ),
							ATDV_NAMEP( port[channel].d4xHandle ) );

			// on error events, give this info regardless of the d4xDiags flag.
			PrintMessage(MSGTYPE_IVR, 
						hwndD4xMsg,
						"process_event(): channel [%d]  state [%d]  event [%d]",
						channel,
						port[channel].state,
						evtcode);

			return( curr_state( channel, evtcode, chdev, cstp ) );

		default:
            PrintMessage(MSGTYPE_IVR,  
							hwndD4xMsg,
							"Line %i WARNING Unexpected Event, Event Code 0x%0x",
							channel,
							evtcode );

            return( curr_state( channel, evtcode, chdev, cstp ) );
		}
	return( port[channel].state );
	}


	int
T1EventHdlr(int channel, int evtcode, DX_CST *cstp)
	{
	unsigned short sig;
	unsigned short eventBit;
	short indx;
	char string2[300];

	// take first data item from inside the structure. kgill 2001-10-30
	sig = cstp->cst_event;

	for (indx = 0; indx < 4; indx++)
		{
		// Check if bit in change mask (upper nibble - lower byte) is set or
		//  if this is a WINK (upper nibble - upper byte) event
		if (!(sig & (SIGEVTCHK << indx)))
			{
			continue;
			}

		eventBit = (unsigned short) (sig & (SIGBITCHK << indx));
		switch( eventBit )
			{
			case DTMM_AOFF:
			case DTMM_AON:
			case DTMM_BOFF:
			case DTMM_BON:
				if( eventBit == param.t1EvtRing )
					{
					//	Incoming Rings Event
					//
					//		Set Channel to Off-Hook
					//
					if( port[channel].state == ST_IDLE )
						{
						port[ channel ].LoopCurrent = TRUE;
						UpdatePhoneLineStatusWindow( channel, "Ringing..." );

						if(bExternalControl && (PsOpMode == MODE_STANDBY))
							{
							return( ST_IDLE );
							}

						if(param.IVRPort[channel].iType == 4 && CatsCall )
							{
							port[ channel ].PortStatus = 0;
							sprintf( string2, "2 Line %i CATS ringing.", channel );
							WriteLog( "XTPGCATS.MSG", string2 );

							PrintMessage(MSGTYPE_IVR,  
											hwndD4xMsg, 
											string2);

							return( ST_IDLE );
							}

						if(param.IVRPort[channel].iType == 4 )
							CatsCall = TRUE;

						return( ST_OFFHK );
						}
					}
				else if( eventBit == param.t1EvtOnHook )
					{
					// Caller hangup, set state to ONHOOK and set channel to ONHOOK
					if( port[ channel ].state != ST_IDLE && port[ channel ].state != ST_ONHK )
						{
						port[ channel ].LoopCurrent = FALSE;
						UpdatePhoneLineStatusWindow( channel, "Disconnect" );

						PrintMessage(MSGTYPE_IVR,  
										hwndD4xMsg, 
										"Line %i T1 Disconnect Detected", 
										channel);

						dx_stopch(port[channel].d4xHandle, EV_ASYNC);

						// delay after dx_stopch(), it takes some time
						//  for the chanel to become idle.
						//  any commands issued before the channel is actually stopped
						//  will precipitate an error.
						// I chose 100ms for the delay, as by experimentation, values less than
						//  32ms do not prevent the problem.
						Sleep(100);
						// PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "delay after dx_stopch()");

						return( Exit_State( channel, evtcode, port[channel].d4xHandle, cstp ) );
						}
					}
				break;

			case DTMM_COFF:
			case DTMM_CON:
			case DTMM_DOFF:
			case DTMM_DON:
			case DTMM_WINK:
				break;

			default:
				PrintMessage(MSGTYPE_IVR, 
								hwndD4xMsg,
								"Line %i Unknown DTEV_SIG Event 0x%hx",
								channel,
								sig );
			}
		}
	return port[channel].state;
	}



static int GetMenuIndex(char c)
{
	static const int lookup[] =
	{//              #                           *
		-1, -1, -1, 11, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1,
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1
	};

	unsigned int idx = c - 0x20;
	if(idx & 0xFFFFFFE0) return -1;

	return lookup[idx];
}



/****************************************************************
*        NAME : curr_state( channel, evtcode, chdev )
* DESCRIPTION : complete processing of the current state
*             : identify the next state to be entered
*             : DIAL T : chanel = channel number
*             : evtcode = termination event code
*             : chdev = termination status data
*      OUTPUT : none
*     RETURNS : next channel state
****************************************************************/

	int
curr_state(int channel, int evtcode, int chdev, DX_CST *cstp)
	{
	int i, fc_status;
    int j;
    int k;
    int PagePointer;
	long rvl;
    DWORD rv;
	char string[ 20 ];
    size_t result1;
	char string2[85];


	if(d4xDiags == TRUE)
		{
		PrintMessage(MSGTYPE_IVR, 
						hwndD4xMsg,
						"curr_state(): Line[%i] evtcode[0x%0x] State[%s]",
						channel,
						evtcode,
						DialogicStateName[port[channel].state]);
		}

    PagePointer = channel;

    switch(port[channel].state)
		{
        case ST_IDLE:
            port[ channel ].LoopCurrent = TRUE;
            return( ST_OFFHK );

        case ST_OFFHK:
            port[ channel ].Attempts++;

             if(param.IVRPort[channel].iType == IVR_PAGE_IN ||
				param.IVRPort[channel].iType == IVR_CATS ||
				param.IVRPort[channel].iType == IVR_PAGE_STATUS ||
				param.IVRPort[channel].iType == IVR_GROUP_CALL ||
				param.IVRPort[channel].iType == IVR_TYPE_19 ||
				param.IVRPort[channel].iType == IVR_SELF_PARK_AND_PAGE )
				{
				page[ channel ].PageStatus[ 0 ] = 'I';

                if( page[ channel ].PageStatus[ 0 ] == 'I' )
					{
                    page[ channel ].attempts = 0;
                    page[ channel ].PageStatus[ 0 ] = 'B';
                    page[ channel ].PortNumber = channel;
                    port[ channel ].PageNumber = channel;

                    ResetBuffer(channel);

					i = channel;

                    if( param.GTD.freq1 > 0 )
						{
                        rv = dx_enbtone( chdev, param.GTD.tid, DM_TONEON|DM_TONEOFF );
                        if( rv != 0 )
                            PrintMessage(MSGTYPE_IVR, 
											hwndD4xMsg,
											"Line %i ERROR.2!! Tone not enabled return %i.",
											channel,
											rv);
						}

                    //switch( port[ channel ].PortType )
                    switch(param.IVRPort[channel].iType)
						{
						case IVR_DPG:
						case IVR_DST:
                            return( ST_DID_GET_EXT );

							
                        case IVR_PAGE_IN:
                            if(param.GreetingExt2[0] != 0)
								{
                                memset(string2, 0, sizeof(string2));
                                strcat(string2, param.GreetingExt2);
                                getfile(string2, "VCE", sizeof(string2));
                                rv = GetFileAttributes( string2 );

                                if(rv != 0xFFFFFFFF )
									return(ST_GREETING2_MSG);
								}
                            return( ST_HELLO_MSG );

                        case IVR_SELF_PARK_AND_PAGE:
                            if( param.greetingExtPark[ 0 ] != 0 )
								{
                                memset(string2, 0, sizeof(string2));
                                strcat(string2, param.greetingExtPark);
                                getfile(string2, "VCE", sizeof(string2));
                                rv = GetFileAttributes( string2 );

                                if( rv != 0xFFFFFFFF )
									return( ST_GREETING2_MSG );
								}
                            return( ST_HELLO_MSG );
							
                        case IVR_PAGE_STATUS:	
                            if(param.GreetingExt[0] != 0)
								{
								// custom greeting for status line is defined...
                                memset(string2, 0, sizeof(string2));
                                strcat(string2, param.GreetingExt);
                                getfile(string2, "VCE", sizeof(string2));
                                rv = GetFileAttributes(string2);

                                if(rv != 0xFFFFFFFF)
									{
									// the referenced greeting file was found.
									return(ST_GREETING1_MSG);
									}
								}
                            return(ST_HELLO_MSG);

                        case 18:
                            for( j=0; j<4; j++ )
								{
                                if( Cats[ j ].GroupCallWaiting > 0 ){
                                    if( param.ComPort[ param.Cats[ j ].Group1Port-1 ].VoiceLine-1 == channel )
										Cats[ j ].Group1On = 1;

                                    if( param.ComPort[ param.Cats[ j ].Group2Port-1 ].VoiceLine-1 == channel )
										Cats[ j ].Group2On = 1;

                                    if( Cats[ j ].Group1On || Cats[ j ].Group2On )
										{
                                        port[ channel ].DataPointer = j;
                                        port[ channel ].timer.wHour = 70;
                                        sprintf( string2, "Group call line %i answered.", channel );
                                        WriteLog( "XTPGCATS.MSG", string2 );
                                        return( ST_CATS_PLAY );
										}
									}
								}
                            page[ channel ].PageStatus[ 0 ] = 'I';

                            port[ channel ].Cancels++;
							SetQueueCancel(-1, channel);

                            return( ST_ONHK );

                        case 4:
                            return( ST_CATS_MSG );

                        default:
                            page[ channel ].PageStatus[ 0 ] = 'I';

                            port[ channel ].Cancels++;
							SetQueueCancel(-1, channel);

                            return( ST_ONHK );
                    }
                    break;
                }
            }

            port[ channel ].Cancels++;
			SetQueueCancel(-1, channel);

			return( ST_ONHK );

        case ST_GREETING1_MSG:
        case ST_GREETING2_MSG:
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            if( param.GreetingOnly )
				return( ST_GET_EXT );
            return( ST_HELLO_MSG );

        case ST_VP_PAGE:
            return ST_VP_PAGE;

		// curr_state();
		//  note that this state is setup using vb41_play() function
		//  not the standard one vb40_play().
        case ST_CATS_MSG:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_MSG");
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, " eventcode       [%d]", evtcode);
			//PrintMessage(hwndD4xMsg, " cstp->cst_event [%d]", cstp->cst_event);

			// I don't understand this event / condition,
			//  but I believe that it never happens,
			//  no problem.   kgill 2003-07-30
			//if(evtcode == TDX_CST && cstp->cst_event == DE_DIGITS)
			//	{
			//	PrintMessage(hwndD4xMsg, "advance to: ST_CATS_GET_DT");
			//
			//	return ST_CATS_GET_DT;
			//	}

			// expected digit received
			if((evtcode == TDX_PLAY) && (ATDX_TERMMSK(chdev) == TM_DIGIT))
				{
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "0 thru 9 or * rec'd - advance to: ST_CATS_REC_NG");
				return ST_CATS_REC_NG;
				}

			// end of data (normal play complete)
			if((evtcode == TDX_PLAY) && (ATDX_TERMMSK(chdev) == TM_EOD))
				{
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "end of data - advance to: ST_CATS_REC_ON");
				return ST_CATS_REC_ON;
				}

			// something unexpected, go ahead and record.
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "unexpected event/mask - advance to: ST_CATS_REC_ON");
			return ST_CATS_REC_ON;

		case ST_CATS_REC_ON:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_REC_ON");
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "evtcode [%d]", evtcode);

			PrintMessage(MSGTYPE_IVR, 
							hwndD4xMsg,
							"ATDX_TERMMSK(chdev) [%d]",
							ATDX_TERMMSK(port[channel].d4xHandle));

			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "param.CatsConfirm [%d]", param.CatsConfirm);
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "FALSE value [%d]", FALSE);
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "TRUE value [%d]", TRUE);

			// during the first 4 seconds, any DTMF digit pressed will
			//  cancel the "cats greeting" announcement, this allows
			//  the announcement to be "cancelled", and the process is speeded up.
            if(evtcode == TDX_CST && cstp->cst_event == DE_DIGITS)
				{
                i = Wait(&Cats[port[channel].DataPointer].timer, 400L);

                if((i == FALSE) || (param.CatsConfirm == TRUE))
					{
					// necessary conditions met to abort the cats page.
					//
					// digits received.
					// within the first 4 seconds of the start of recording,
					//  or the confirmation parameter set to not required.
                    page[PagePointer].PageStatus[0] = 'I';

					if(param.CatsConfirm)
						{
						PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "cats confirm parameter option is TRUE");
						}
					if(i == FALSE)
						{
						PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "first 4 seconds of recording not completed");
						}

					PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "advance to: ST_CATS_REC_NG");

                    return ST_CATS_REC_NG;
					}

				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "advance to: ST_CATS_REC_OK");

                return ST_CATS_REC_OK;
				}

            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

			// show exactly what is going on...................
			/*
            if(FALSE == param.CatsConfirm)
				{
				PrintMessage(hwndD4xMsg, "CatsConfirm IS FALSE");
				}

            if(evtcode == TDX_RECORD)
				{
				PrintMessage(hwndD4xMsg, "THE EVENT == TDX_RECORD");
				}

            if(ATDX_TERMMSK(port[channel].d4xHandle) == TM_NORMTERM)
				{
				PrintMessage(hwndD4xMsg, "MASK IS == TM_NORMTERM");
				}
            if(ATDX_TERMMSK(port[channel].d4xHandle) == TM_MAXSIL)
				{
				PrintMessage(hwndD4xMsg, "MASK IS == TM_MAXSIL");
				}
            if(ATDX_TERMMSK(port[channel].d4xHandle) == TM_MAXTIME)
				{
				PrintMessage(hwndD4xMsg, "MASK IS == TM_MAXTIME");
				}
			*/

			// respond to "record complete" message, due to ...
            if((FALSE == param.CatsConfirm) &&
				((evtcode == TDX_RECORD) &&
				 (ATDX_TERMMSK(port[channel].d4xHandle) == TM_NORMTERM ||
				  ATDX_TERMMSK(port[channel].d4xHandle) == TM_MAXSIL ||
				  ATDX_TERMMSK(port[channel].d4xHandle) == TM_MAXTIME )) )
				{
				// send the request.
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "CATS confirm not required");
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, " and record completed, via NORMTERM, MAXSIL, OR MAXTIME");
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, " advance to: ST_CATS_REC_OK");

				CatsFinalize(channel);

                return ST_CATS_REC_OK;
				}

			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "advance to: ST_CATS_GET_DT");
            return ST_CATS_GET_DT;

        case ST_CATS_GET_DT:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_GET_DT");

			// retrieve received DTMF digit
			GetDigitBuffer(channel);

            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

            if(strncmp(port[channel].digsave, "7", 1) == 0)	// challenger or caller pressed 7 to
				{											//  voluntarily delete the current
															//  voice recording and start a new recording.
				SetFileAttributes(Cats[ port[channel].DataPointer].FileName, FILE_ATTRIBUTE_NORMAL);
				remove(Cats[port[channel].DataPointer].FileName);

                port[channel].file_handle = dx_fileopen(Cats[port[channel].DataPointer].FileName,
															O_CREAT | O_BINARY | O_TRUNC | O_RDWR,
															S_IREAD | S_IWRITE);
                PrintMessage(MSGTYPE_IVR, 
								hwndD4xMsg,
								"Line %i REWIND: CATS file %s open, FH: %i",
								channel,
								Cats[port[channel].DataPointer].FileName,
								port[channel].file_handle);

				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "someone pressed '7'");

                return ST_CATS_REC_ON;
				}

            if(strncmp(port[channel].digsave, "*", 1) == 0)	// challenger or caller pressed * to abort
				{
                page[PagePointer].PageStatus[0] = 'I';

				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "someone pressed '*'");

                return ST_CATS_REC_NG;
				}

            if(strncmp(port[channel].digsave, "#", 1) == 0)	// challenger or caller pressed # to proceed
				{
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "someone pressed '#'");

                return ST_CATS_REC_OF;
                //return ST_CATS_REC_OK;	// changed 2003-04-24 kgill.
				}

            page[PagePointer].PageStatus[0] = 'I';

			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "advance to: ST_CATS_REC_NG");
            return ST_CATS_REC_NG;

        case ST_CATS_REC_OF:
            // end of play or record, close the message file
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_REC_OK");

            return ST_CATS_V_ON;

        case ST_CATS_REC_OK:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_REC_OK");

            if(evtcode == TDX_CST && cstp->cst_event == DE_DIGITS)
				{
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "advance to: ST_CATS_GET_DT");
				return ST_CATS_GET_DT;
				}

			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "advance to: ST_CATS_REC_OF");
			return ST_CATS_REC_OF;

        case ST_CATS_REC_NG:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_REC_NG");
            return ST_CATS_ABORT;

        case ST_CATS_ABORT:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_ABORT");
            return ST_IDLE;

        case ST_CATS_V_ON:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_V_ON");

            page[PagePointer].PageStatus[0] = 'I';
            return ST_ONHK_DELAY;

        case ST_CATS_CALL:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_CALL");
            return ST_CATS_CALL;

        case ST_CATS_PLAY:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "curr_state() at: ST_CATS_PLAY");
            if(port[channel].timer.wHour == 70)
				{
				GetLocalTime(&port[channel].timer);
				}

            i = Wait(&port[channel].timer, 6000L);

            if(i == FALSE)
				{
                return ST_CATS_PLAY;
				}
            else
				{
                page[PagePointer].PageStatus[0] = 'I';
                return ST_CATS_ONHK;
				}

        case ST_CATS_ONHK:
            sprintf( string2, "Group call line %i completed", channel );
            WriteLog( "XTPGCATS.MSG", string2 );
            port[ channel ].timer.wHour = 70;
            return( ST_ONHK_DELAY );

        case ST_HELLO_MSG:
            return( ST_GET_EXT );

        case ST_CVR_RECUR:
            return( ST_GOODBYE2_MSG );

        case ST_DID_GET_EXT:
            port[ channel ].Attempts++;
			page[ PagePointer ].pageAdded = FALSE;
			page[ PagePointer ].pageFormatted = FALSE;
			page[ PagePointer ].tempInt = 0;
            return( ST_DID_HKFLSH );

        case ST_GET_CVR_RES:
			// Process extension for covering doctor. Pass thru next state too.

			// collect any dtmf digits that may have been entered,
			//  and clear them out.
			GetDigitBuffer(channel);
            memset(port[channel].digsave, 0, sizeof(port[channel].digsave));

			// indicate that the channel is 'B'usy.
            page[ PagePointer ].PageStatus[0] = 'B';

            memset(page[PagePointer].PagerId, 0, sizeof(page[PagePointer].PagerId));

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndDatabaseMsg, 
							"GetPagerId() called from ST_GET_CVR_RES");

            i = GetPagerId(page[PagePointer].FileName, PagePointer, BY_PROFILEID);

			if( i == TRUE )
				{
                strcpy( port[ channel ].digsave, page[ PagePointer ].PagerId );
				}

			PrintMessage(MSGTYPE_IVR,  
							hwndGeneralMsg, 
							"Covered By: %s", port[ channel ].digsave );

            memset( page[ PagePointer ].InitiatorId, 0, sizeof(page[ PagePointer ].InitiatorId ) );
            memset( page[ PagePointer ].FileName, 0, sizeof(page[ PagePointer ].FileName ) );
            memset( page[ PagePointer ].PageInfo, 0, sizeof(page[ PagePointer ].PageInfo ) );

            if( page[ PagePointer ].attempts > 24 )
				return( ST_CVR_RECUR );

        case ST_NAME_MSG2:
        case ST_DID_HKFLSH:
        case ST_GET_EXT:
			// response to getdtmfs() function
			if( port[ channel ].state == ST_DID_HKFLSH ||
				port[ channel ].state == ST_GET_EXT )
				{
				GetDigitBuffer( channel );
				}

            page[ PagePointer ].attempts++;
			page[ PagePointer ].pageAdded = FALSE;
			page[ PagePointer ].pageFormatted = FALSE;
            k = 0;
            if( port[channel].digsave[0] == '#' || port[channel].digsave[0] == 0 )
				{
                if( param.TimeOutTransfer && param.OpTransfer[ 0 ] != 0 && port[ channel ].digsave[ 0 ] == 0 )
					return( ST_OP_TRANSFER );

                if( param.timeoutRetry && page[ PagePointer ].attempts < 4 )
					return( ST_HELLO_MSG );

                page[ PagePointer ].PageStatus[ 0 ] = 'I';
                port[ channel ].Errors++;
				port[ channel ].timer.wHour = 70;
                return ( ST_ONHK );
				}

            if( port[ channel ].digsave[ 0 ] == '0' &&
				( port[ channel ].digsave[ 1 ] == 0 || port[ channel ].digsave[ 1 ] == '#' ) &&
				param.OpTransfer[ 0 ] != 0 )
				{
				return( ST_OP_TRANSFER );
				}

            if( strncmp( port[channel].digsave, param.PNamePass, strlen( param.PNamePass ) ) == 0 )
				{
                if( strlen( port[ channel ].digsave ) <= strlen( param.PNamePass ) + 1 )
                    return( ST_NAME_MSG );
				}

			if( param.didPagePass[ 0 ] != 0 &&
				strncmp( port[channel].digsave, param.didPagePass, strlen( param.didPagePass ) ) == 0  )
				{
				return( ST_HELLO_MSG );
				}

			if( param.didStatPass[ 0 ] != 0 &&
				strncmp( port[channel].digsave, param.didStatPass, strlen( param.didStatPass ) ) == 0  )
				{
				page[ PagePointer ].tempInt = 1;
				return( ST_HELLO_MSG );
				}

            page[ PagePointer ].VoiceFile = FALSE; // Indicates not a voice page

            memset(string2, 0, sizeof(string2));
            strcpy(string2, port[channel].digsave);

            if( strncmp( param.VoicePass, string2, strlen( param.VoicePass ) ) == 0 )
				{
                page[ PagePointer ].VoiceFile = TRUE;
                memset(string2, 0, sizeof(string2));
                strcpy(string2, port[ channel ].digsave + strlen( param.VoicePass ) );

                PrintMessage(MSGTYPE_IVR,  
								hwndD4xMsg, 
								"Line %i Voice password accepted.", 
								channel);
				}

            if( strncmp( param.ListPass, string2, strlen( param.ListPass ) ) == 0 ||
				strncmp( param.ListPassN, string2, strlen( param.ListPassN ) ) == 0 )
				{
                page[ PagePointer ].VoiceFile = 1;
                if( strncmp( param.ListPassN, string2, strlen( param.ListPassN ) ) == 0 )
					{
					page[ PagePointer ].VoiceFile = 2;
					}

                memset(string2, 0, sizeof(string2));
                strcpy(string2, port[ channel ].digsave + strlen( param.ListPass ) );
                memset(page[ PagePointer ].Extension, 0, sizeof( page[ PagePointer ].Extension ) );
                memset(page[ PagePointer ].PagerId, 0, sizeof( page[ PagePointer ].PagerId ) );

                strncpy(page[ PagePointer ].PageType, Queue[ 0 ].Id + 1, 2 );

                for(i = 0; i < 10; i++)
					{
                    if( string2[ i ] == '#' || string2[ i ] == '*' || string2[ i ] == 0 )
						break;
                    page[ PagePointer ].Extension[ i ] = string2[ i ];
					}

                strncpy( page[ PagePointer ].PagerId, page[ PagePointer ].Extension, 10 );

 				i = GetPagerId( page[ PagePointer ].Extension, PagePointer, BY_PAGERID);
				if( i == FALSE )
					{
					PrintMessage(MSGTYPE_IVR, 
									hwndD4xMsg,
									"1-Line %i Pager id %s not found",
									channel,
									page[PagePointer].PagerId);

					port[channel].Errors++;
					return(ST_LSTNG_MSG);
					}

                PrintMessage(MSGTYPE_IVR, 
								hwndD4xMsg,
								"Line %i Password accepted for list %s, id %s.",
								channel,
								page[ PagePointer ].Extension,
								page[ PagePointer ].PagerId );

				// now see if the list exists. use ListGetFirst() to
				//  retrieve the first element, if successful then the list exists.
				//  the data from this call is not used.
				//
				rv = SUCCESS;
                if(SUCCESS == rv)
					{
                    if(param.CustPrompt)
						{
                        memset(string2, 0, sizeof(string2));
                        strcpy(string2, page[ PagePointer ].Extension);

                        getfile(string2, "VCE", sizeof(string2));
                        rv = GetFileAttributes(string2);

                        if(rv != 0xFFFFFFFF)
							{
							return(ST_LSTOK_VCE);
							}
						}
                    return(ST_LSTOK_MSG);
					}
                port[ channel ].Errors++;
                return(ST_LSTNG_MSG);
				}
            else if( strncmp( param.MenuPass, string2, strlen( param.MenuPass ) ) == 0)
				{
                memset(string2, 0, sizeof(string2));
                strncpy(string2, port[ channel ].digsave + strlen( param.MenuPass ), 10);
                k= -1;
				page[ PagePointer ].tempInt = 1;
				PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i, Menu password accepted.", channel );
				}

			// validate the digits for range 0-9
            memset( page[ PagePointer ].Extension, 0, sizeof( page[ PagePointer ].Extension ) );
            memset( page[ PagePointer ].PagerId, 0, sizeof( page[ PagePointer ].PagerId ) );

            for(i = 0; i < 10; i++)
				{
                if( string2[ i ] == '#' || string2[ i ] == '*' || string2[ i ] == 0 ) break;
                page[ PagePointer ].Extension[ i ] = string2[ i ];
				}

            if( page[ PagePointer ].InitialPagerId[ 0 ] == 0 )
                strncpy( page[ PagePointer ].InitialPagerId, page[ PagePointer ].Extension, 10 );

            strncpy( page[ PagePointer ].PagerId, page[ PagePointer ].Extension, 10 );

            i = GetPagerId( page[ PagePointer ].Extension, PagePointer, BY_PAGERID);
			
			// SOguz - DE20747 - 6/15/2016 - In order to page all devices, 
			// the YNFlag parameter value (e.g. YNFlag23) in XaPGSRV.INI file 
			// for the profile must be set to Y

			// SOguz - DE20747 - 6/15/2016 - Initialize global variable "isPageAllDevicesOn" to FALSE
			// and StatusOn to TRUE
			// These variablse will be used through out the code so we don't constantly check the database
			isPageAllDevicesOn = FALSE;
			StatusOn = TRUE; 

			// SOguz - DE20747 - 6/15/2016 - check if page all devices YNFlag is set to Y for the profile
			if (CheckIfAllPageFlagIsOn(PagePointer) == SUCCESS)
			{				
				PrintMessage(MSGTYPE_SYSTEM,
								hwndGeneralMsg,
								"PageAllDevices is ON for profileid = %s",
								page[ PagePointer ].Extension);
				isPageAllDevicesOn = TRUE; // YNFlag is set to Y; therefore isPageAllDevicesOn to TRUE
				StatusOn = FALSE; // do not prompt current page status to user
				return ST_PLAY_ID; // Move to next state to play the id and ask for call back number
			}

			//checking if pagerid is found
            if(i == FALSE)
				{
                PrintMessage(MSGTYPE_IVR, hwndD4xMsg,
								"2-Line %i Pager id %s not found",
								channel,
								page[PagePointer].PagerId);

                port[ channel ].Errors++;
                return(ST_NO_LOG_MSG);
				}

			//	}
			//copying Extension (profileid) to .InitialExtension
            if( page[ PagePointer ].InitialExtension[ 0 ] == 0 )
                strncpy( page[ PagePointer ].InitialExtension, page[ PagePointer ].Extension, 10 );

            // check for the .log file present
			if( LogPager( page[ PagePointer ].Extension, PagePointer ) == FALSE )
				{
				PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
								"Line %i Pager not assigned: log file %s.",
								channel,
								page[ PagePointer ].Extension );

				if( page[ PagePointer ].PageStatus[ 0 ] == 'N' || page[ PagePointer ].PageStatus[ 0 ] == ' ' )
					return( ST_NO_LOG_MSG );

				page[ PagePointer ].PageStatus[ 0 ] = 'M';
				memset( string, 0, sizeof( string ) );
				sprintf( string, "%%%s", page[ PagePointer ].PageType );
				Get_Frq( PagePointer, string, string );

				if(param.IVRPort[channel].iType != 17 &&
					page[ PagePointer ].tempInt != 1 )
					{
					if( page[ PagePointer ].ACKMin != -1 )
						{
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] != 'Y' )
							{
							if( param.CustPrompt )
								{
								memset(string2, 0, sizeof(string2));
								strcat(string2, page[ PagePointer ].Extension);
								getfile(string2, "VCE", sizeof(string2));
								rv = GetFileAttributes( string2 );

								if( rv != 0xFFFFFFFF )
									return( ST_PLAY_PROMPT );

								if(port[channel].state != ST_NAME_MSG2 && param.PromptId)
									{

									PrintMessage(MSGTYPE_IVR,  
													hwndD4xMsg, 
													"current state is: [%s], ", 
													DialogicStateName[port[channel].state]);
									PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "entering ST_PLAY_ID - 1");

									return ST_PLAY_ID;
									}
								}
							return ST_CB_MSG;
							}
						}
					else
						{
						port[channel].Errors++;
						return ST_NO_LOG_MSG;
						}
					}
				}

			if( page[ PagePointer ].VoiceFile == TRUE && page[ PagePointer ].tempInt == 0 )
				{
				if( Get_Frq( PagePointer, param.VoiceQues, param.VoiceQues2 ) == TRUE )
					{
					return( ST_CB_VOICE );
					}
				else
					{
					PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
									"Line %i Voice pager not assigned. Check frq %s.",
									channel,
									page[ PagePointer ].Extension );

					port[ channel ].Errors++;
					return( ST_NO_VOICE );
					}
				}
			else
				{
				memset( string, 0, sizeof( string ) );
				sprintf( string, "%%%s", page[ PagePointer ].PageType );
				///Get FrqNumber, FrqType, and AuthCode (FrqNumber) 
				if( Get_Frq( PagePointer, string, string ) == TRUE ||
					param.IVRPort[channel].iType == 17 ||
					k == -1 ||
					page[PagePointer].tempInt == 1)
					{
					if(param.IVRPort[channel].iType == 17 ||
						k == -1 ||
						page[ PagePointer ].tempInt == 1)
						{
						j = 0;
						i = ST_ONHK;
						for( k=0; k<12; k++ )
							{
							switch( param.StatPress[ k ][ 0 ] )
								{
								case 'S':
									i = ST_STAT_MSG;
									j++;
									if( param.CustPrompt ) i = ST_STAT_HELLO;
									break;
								case 'V':
									i = ST_VCE_MSG;
									j++;
									break;
								case 'A':
									i = ST_PASS_MSG;
									j++;
									break;
								case 'M':
									if( page[ PagePointer ].Password[ 0 ] == 0 ||
										page[ PagePointer ].Printed[ 0 ] == 'P' )
										i = ST_STAT_M_MSG;
									else
										i = ST_STAT_M_PASS;
									j++;
									break;
								}
							}
						if( j > 1 ){
							if( param.StPromptStat )
								i = ST_MENU_STAT;
							else
								i = ST_MENU_MSG;
							if( param.CustPrompt ) i = ST_MENU_HELLO;
						}
						if( i == ST_ONHK ){
							port[ channel ].timer.wHour = 70;
							page[ PagePointer ].PageStatus[ 0 ] = 'I';
						}
						page[ PagePointer ].Printed[ 0 ] = 0;
						return( i );
					}
					if( param.CustPrompt )
						{
						memset(string2, 0, sizeof(string2));
						strcat(string2, page[ PagePointer ].Extension);
						getfile(string2, "VCE", sizeof(string2));
						rv = GetFileAttributes( string2 );

						if( rv != 0xFFFFFFFF )
							return( ST_PLAY_PROMPT );
						}

					if(port[channel].state != ST_NAME_MSG2 &&
						(param.PromptId || page[PagePointer].attempts > 1))
						{

						PrintMessage(MSGTYPE_IVR,  
										hwndD4xMsg, 
										"current state is: [%s], ", 
										DialogicStateName[port[channel].state]);
						PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "entering ST_PLAY_ID - 2");

						return ST_PLAY_ID;
						}

					if(param.IVRPort[channel].iType == 20 )
						return( ST_PARK_MSG );

					memset(string2, 0, sizeof(string2));
					strncpy(string2, page[ PagePointer ].PageType, 2);
					string2[2] = 0;
					return( ST_CB_MSG );
					}
				else
					{
					PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
									"Line %i Pager not assigned. Check frq %s.",
									channel,
									page[ PagePointer ].Extension );

					if( page[ PagePointer ].ACKMin != -1 )
						{
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'F' ) return( ST_CB_MSG );
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'O' ) return( ST_CB_MSG );
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'C' ) return( ST_CB_MSG );
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'M' ) return( ST_CB_MSG );
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'U' ) return( ST_CB_MSG );
						if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'T' ) return( ST_CB_MSG );
						}
					port[ channel ].Errors++;
					return ST_NO_LOG_MSG;
					}
				}
			break;	

        case ST_CHECK_IMPAIRED_VENDOR_ANN:
			// check to see if impairment announcement feature is enabled.

			if(param.UseImpairmentAnnouncements)
				{
				// check to see if vendor for this page type is impaired.
				//  check to see if this page type is listed as "Impaired"
				//  and if it is impaired, check to see if the custom greeting
				//  prompt that will be used exists. If all conditions
				//  are met... then enter the state that plays the impairment message.
				// 
				// check if "impaired message" should be announced.
				//
				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "Checking Paging Vendor Impairment.");
				{
				CString PageDeviceType;
				PageDeviceType.Format("%%%s", page[PagePointer].PageType);

				CString csProfileId;

				csProfileId = g_IMP.GetCustomGreetingProfileId(PageDeviceType);

				if(csProfileId.GetLength() > 0)
					{
					PrintMessage(MSGTYPE_IVR, 
									hwndD4xMsg, 
									"Profile Info from Impairment map: [%s]", 
									(LPTSTR) (LPCTSTR) csProfileId);

					// ensure that the Impairment custom greeting exists.
					//
					{
					CString csFileName;
					CString PageDeviceType;
					PageDeviceType.Format("%%%s", page[PagePointer].PageType);

					csFileName = g_IMP.GetCustomGreetingProfileId(PageDeviceType);

					char FileName[MAX_PATH];
					sprintf(FileName, "%s", (LPTSTR) (LPCTSTR) csFileName);
					getfile(FileName, "VCE", MAX_PATH);

					rv = GetFileAttributes(FileName);
					}

					if(rv == 0xFFFFFFFF)
						{
						// unable to get file attributes.
						//  interpret this as file not found.
						}
					else
						{
						// file attributes were found
						//  implies that the file exists.
						//  go ahead into the state that plays the message.
						return ST_IMPAIRMENT_MSG;
						}				
					}
				}
				}

			// normal course of events, 
			//  with no impairment feature, or vendor in impaired condition.
			return ST_PLAY_ID;
			break;

        case ST_PLAY_PROMPT:
        case ST_PLAY_ID:
            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

             if(param.IVRPort[channel].iType == 20)
				{
				return ST_PARK_MSG;
				}
			 //01/14/2016 - DE20568 - SOguz - If the page server is in 
			//FAILSAFE mode and the page type is SMS type then go into 
			//ST_SMS_FS_MSG state to play message to caller "sorry 
			//unable to page" since we cannot send page out. This is mainly 
			//used by Spok Mobile type pagers in FailSafe.
			//1/15/2016 - SOguz - FileName is used for checking coverage
			// 7/1/2016 - SOguz - Added isPageAllDevicesOn condition to the if statement so that 
			// we can respect coverage based on whether page all devices ON or OFF. If page all devices
			// ON then we don't respect coverage otherwise we do. (page[PagePointer].FileName[0] == 0
			// checks for coverage.
			if((DATA_SOURCE_LOCAL == DataSource && IsSmsType(PagePointer))
				&& ((page[PagePointer].FileName[0] == 0) || isPageAllDevicesOn))
			{	
					// 7/1/2016 - DE20747 - SOguz
					// if page all devices feature is ON then get the list of pagers for that profile.
					// if the profile has only SMS pager then log and play a message that we cannot  
					// page this pager at this time. If the profile has more than one pager then go 
					// ahead and ask for the call back number because we will page the non-SMS pagers
					if (isPageAllDevicesOn)
					{
						int counter = GetAllPagingDevicesFromProfile (PagePointer);	
						// 7/1/2016 - SOguz - if there are more than one pagers for this profile then 
						// ask for call back number
						if (counter > 1) 
						{
							return ST_CB_MSG;
						}

						// 7/1/2016 - SOguz - if SMS type pager is the only one pager type for this profile 
						// then log message and play the message that we cannot page at this time.
						if (counter == 1)
						{
							char msg1[250];
							char msg2[250];

							sprintf(msg1,"Page Server is in failsafe mode. Therefore, it is skipping SMS pager %s for profile %s.",
							string,
							page[PagePointer].Extension);

							sprintf(msg2, "");

							PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg,								
							"PageAllDevices: %s",
							msg1);

							WriteMessage(PagePointer, page[PagePointer].Extension, "PG", msg1, msg2);	
						}
					}
				
					PrintMessage(MSGTYPE_IVR,  
									hwndD4xMsg, 
									"current state is: [%s], ", 
									DialogicStateName[port[channel].state]);
					PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "entering ST_SMS_FS_MSG - 261");

					return ST_SMS_FS_MSG;				
			}
			else
			{
				return ST_CB_MSG;
			}
			break;

        case ST_LSTOK_MSG:
        case ST_LST_SPK_MSG:
            if( page[ PagePointer ].VoiceFile == 2 )
				return( ST_LST_GET_DIS );

            return( ST_LST_RECORD );
			break;

        case ST_LSTOK_VCE:
            if( page[ PagePointer ].VoiceFile == 2 )
				return( ST_LST_GET_DIS );

            return( ST_LST_SPK_MSG );
			break;

        case ST_LST_GET_DIS:
			GetDigitBuffer( channel );
            if( ( strlen( port[channel].digsave ) == 0 ) || ( port[channel].digsave[0] == '#' ) )
				{
                page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}
            result1 = strlen( port[ channel ].digsave );

            if( port[ channel ].digsave [ result1 - 1 ] == '#' )
				port[ channel ].digsave [ result1 - 1 ] = 0;
            strncpy( page[ PagePointer ].DisplayInfo, port[channel].digsave, 30 );
            page[ PagePointer ].VoiceFile = 0;
            page[ PagePointer ].PageStatus[ 0 ] = 'L';

            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
					"Line %i Display information %s for list %4s received.",
					channel,
					page[ PagePointer ].DisplayInfo,
					page[ PagePointer ].Extension );

            if( port[ channel ].LoopCurrent == FALSE )
				{
				port[ channel ].timer.wHour = 70;
				return( ST_ADD_PAGE );
				}
            return( ST_LST_REC_OK );
			break;

        case ST_LST_RECORD:
            page[ PagePointer ].PageStatus[ 0 ] = 'L';

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            if( port[ channel ].LoopCurrent == FALSE )
				{
				port[ channel ].timer.wHour = 70;
				return( ST_ADD_PAGE );
				}
            return( ST_LST_REC_OK );
			break;

        case ST_GOODBYE2_MSG:
        case ST_LSTNG_MSG:
        case ST_NO_VOICE:
            page[ PagePointer ].PageStatus[ 0 ] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

        case ST_CB_VOICE:
            if( param.voicePagerDtmfPass &&
				evtcode == TDX_CST &&
				cstp->cst_event == DE_DIGITS )
				{
				return( ST_VOICE_DTMF );
				}
            return( ST_GET_VOICE );

        case ST_GET_VOICE:
			// if open, close voice page audio message file
            if(port[channel].file_handle > 0)
				{
                fc_status = dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;

 				PrintMessage(MSGTYPE_IVR, hwndD4xMsg,
								"curr_state() state [%d] close voice record channel[%d]  close status[%d]",
								port[channel].state,
								channel,
								fc_status);
				}

			// status 'M' is ????
            if(Status[page[PagePointer].ACKMin].Page[0] == 'M')
				{
                WriteUserLogEx(PagePointer, -1);
                return(ST_GOODBYE2_MSG);
				}
            return ST_GOODBYE_MSG;

        case ST_NO_LOG_MSG:
			memset( page[ PagePointer ].InitialPagerId, 0, sizeof(page[ PagePointer ].InitialPagerId ) );
			memset( page[ PagePointer ].InitialExtension, 0, sizeof(page[ PagePointer ].InitialExtension ) );
            if( page[ PagePointer ].attempts > 3 )
				{
                page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}

            if( page[ PagePointer ].PageStatus[ 0 ] == 'N' || page[ PagePointer ].PageStatus[ 0 ] == ' ' )
				{
                if( param.RetryId )
					{
                    page[ PagePointer ].PageStatus[ 0 ] = 'B';
                    return( ST_GET_EXT );
					}
                else
					{
                    page[ PagePointer ].PageStatus[ 0 ] = 'I';
					port[ channel ].timer.wHour = 70;
                    return ( ST_ONHK );
					}
				}

            if( param.RetryId )
				{
                page[ PagePointer ].PageStatus[ 0 ] = 'B';
                return( ST_GET_EXT );
				}
            return( ST_GET_DIS );

        case ST_GOODBYE_MSG:
        case ST_LST_REC_OK:

			// 6/27/2016 - DE20747 - SOguz
			// Make sure that pagealldevices is not ON since 
			// we are not suppose to respect page status
            if( isPageAllDevicesOn == FALSE && 
				(page[ PagePointer ].PageStatus[0] == 'N' ||
				page[ PagePointer ].PageStatus[0] == ' ' ||
				page[ PagePointer ].PageStatus[ 0 ] == 'M' ))
				{
                page[ PagePointer ].PageStatus[0] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}

            if( port[ channel ].LoopCurrent == FALSE )
				{
				port[ channel ].timer.wHour = 70;
				// 6/24/2016 - DE20747 - SOguz 
				// the caller entered the call back number
				// Move to next state and add each devices to queue
				if (isPageAllDevicesOn) 
				{ 
					//next state to add the each paging device to queue	
					return ST_ADD_ALL_DVCS_Q; 
				}

				return( ST_ADD_PAGE );
				}

            if( param.CustPrompt &&
				port[ channel ].state != ST_LST_REC_OK &&
				param.PromptId )
				{
                memset(string2, 0, sizeof(string2));
                strcat(string2, page[ PagePointer ].Extension);
                getfile(string2, "VCE", sizeof(string2));
                rv = GetFileAttributes( string2 );

                if( rv != 0xFFFFFFFF )
					return( ST_GOODBYE_PROMPT );

                return( ST_GOODBYE_ID );
				}
			port[ channel ].timer.wHour = 70;

			// 6/24/2016 - DE20747 - SOguz 
			// the caller entered the call back number
			// Move to next state and add each devices to queue
			if (isPageAllDevicesOn) 
			{ 
				//next state to add the each paging device to queue	
				return ST_ADD_ALL_DVCS_Q; 
			}

            return( ST_ADD_PAGE );

		case ST_VM_TR_MSG:
			page[ channel ].tempInt = 0;
			return( ST_VM_TRANSFER );

		case ST_VM_TRANSFER:
			if(page[ channel ].tempInt == -1)
				{
				strcpy(page[PagePointer].DisplayInfo, param.vmPageMsg);

				// schedule it to happen 5 minutes from now.
				SchedulePage(PagePointer, 5);
				return( ST_ONHK );
				}
			else
				{
				return( ST_VM_TRANSFER );
				}

        case ST_PARK_MSG:
            if( page[ PagePointer ].ACKMin != -1 || param.IgnoreStatus )
				{
                switch( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] )
					{
                    case 'N':
                    case ' ':
                    case 'F':
                    case 'O':
                    case '0':
                    case 'V':
                    case '#':
                    case 'U':
					case 'T':
                        page[ PagePointer ].PageStatus[0] = 'I';
                        return( ST_PARK_NG_MSG );
                        break;
                    default:
                        break;
					}
				}
            i = GetParkNumber( PagePointer, channel );

            if( i == ST_PARK_GET )
				{
                if( page[ PagePointer ].PriorityQPointer < 9 )
                    page[ PagePointer ].PriorityQPointer++;
				}
            return( i );

        case ST_PARK_GET:
			GetDigitBuffer( channel );
            if( port[ channel ].digsave[ 0 ] == '*' ) return( ST_PARK_TMSG );
            return( ST_GOODBYE2_MSG );

        case ST_PARK_TMSG:
			page[ channel ].tempInt = 0;
            return( ST_PARK_TRSFR );

        case ST_PARK_TRSFR:
			if( page[ channel ].tempInt == -1 )
				{
				rvl = ATDX_CPTERM( chdev );

				if( rvl != CR_BUSY && rvl != CR_CEPT )
					{
					i = FALSE;
					memset( string, 0, sizeof( string ) );
					sprintf( string, "%%%s", page[ PagePointer ].PageType );

					if( param.CodeQues[ 0 ] != 0 )
						{
						page[ PagePointer ].ComNumber = -1;
						page[ PagePointer ].ScriptNumber = -1;
						i = Get_Frq( PagePointer, param.CodeQues, string );
						}

					if( i == FALSE )
						{
						Get_Frq( PagePointer, string, string );
						}

					AddSetUp( PagePointer, "External" );
					page[ PagePointer ].PortNumber = GetQuePointer( PagePointer );

					if(AddQueueEx(PagePointer, page[PagePointer].PortNumber))
						{
						page[PagePointer].pageAdded = TRUE;
						}

					page[ PagePointer ].PageStatus[0] = 'I';

					if( param.UserMsg[0] == 'E' )
						{
						WriteUserLogEx(PagePointer, page[ PagePointer ].PortNumber);
						}

					port[ channel ].timer.wHour = 70;
					return( ST_ONHK );
					}
				else
					{
					PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i, Unexpected termination %l", channel, rvl );
					page[ channel ].tempInt = 0;
					return( ST_PARK_RECALL );
					}
				}
			else
				{
				return( ST_PARK_TRSFR );
				}

        case ST_PARK_RECALL:
			if( page[ channel ].tempInt == -1 )
				return( ST_PARK_NG_MSG );
			else
				return( ST_PARK_RECALL );

        case ST_PARK_NG_MSG:
            if( param.OpTransfer[ 0 ] != 0 ) return( ST_OP_TRANSFER );
            page[ PagePointer ].PageStatus[ 0 ] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

        case ST_GOODBYE_ID:
        case ST_GOODBYE_PROMPT:
            return( ST_GOODBYE3_MSG );

        case ST_GOODBYE3_MSG:
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}
			port[ channel ].timer.wHour = 70;

			// 6/24/2016 - - DE20747 - SOguz
			// We received the call back number
			// Move to next state and add each devices to queue
			if (isPageAllDevicesOn) 
			{ 
				return ST_ADD_ALL_DVCS_Q; 
			}
            return( ST_ADD_PAGE );

        case ST_CB_MSG2:
            if(Status[page[PagePointer].ACKMin].Page[0] == 'U')
				{
				return(ST_EXCP_TIME);
				}

			if(Status[page[PagePointer].ACKMin].Page[0] == 'Y')
				{
				if(param.UseImpairmentAnnouncements)
					{
					CString csProfileId;
					CString PageDeviceType;

					// check to see if vendor for this page type is impaired.
					//  check to see if this page type is listed as "Impaired"
					//  and if it is impaired, check to see if the custom greeting
					//  prompt that will be used exists. If all conditions
					//  are met... then enter the state that plays the impairment message.
					// 
					// check if "impaired message" should be announced.
					//
					PrintMessage(MSGTYPE_IVR, 
									hwndD4xMsg, 
									"Vendor Impairment feature IS in use.");

					PageDeviceType.Format("%%%s", page[PagePointer].PageType);

					csProfileId = g_IMP.GetCustomGreetingProfileId(PageDeviceType);

					if(csProfileId.GetLength() > 0)
						{
						PrintMessage(MSGTYPE_IVR, 
										hwndD4xMsg, 
										"Profile Info from Impairment map: [%s]", 
										(LPTSTR) (LPCTSTR) csProfileId);

						// ensure that the Impairment custom greeting exists.
						//
						{
						CString csFileName;
						CString PageDeviceType;
						PageDeviceType.Format("%%%s", page[PagePointer].PageType);

						csFileName = g_IMP.GetCustomGreetingProfileId(PageDeviceType);

						char FileName[MAX_PATH];
						sprintf(FileName, "%s", (LPTSTR) (LPCTSTR) csFileName);
						getfile(FileName, "VCE", MAX_PATH);

						rv = GetFileAttributes(FileName);
						}

						if(rv == 0xFFFFFFFF)
							{
							// unable to get file attributes.
							//  interpret this as file not found.
							}
						else
							{
							// file attributes were found
							//  implies that the file exists.
							//  go ahead into the state that plays the message.
							return ST_IMPAIRMENT_MSG;
							}				
						}
					}
				}


            if(Status[page[PagePointer].ACKMin].Page[0] == 'T')
				{
				return(ST_OP_TRANSFER);
				}

            return ST_CB_MSG3;

        case ST_CB_MSG:
            return( ST_GET_DIS );

        case ST_CB_MSG3:
            if(page[PagePointer].PageStatus[0] == 'M')
				{
                if(param.AnotherPage[0] != 0 &&
					param.AnotherPage[0] != ' ')
					{
					return ST_ANOTHER_GET;
					}
				}

            if((param.AnotherPage[0] != 0 &&
				param.AnotherPage[0] != ' ') &&
				param.IgnoreStatus == FALSE)
				{
                switch(Status[page[PagePointer].ACKMin].Page[0])
					{
                    case 'N':
                    case ' ':
                    case '#':
                    case '0':
                    case 'V':
                    case 'U':
                        return ST_ANOTHER_GET;

                    default:
						//if( port[ channel ].PortType == 20 )
						if(param.IVRPort[channel].iType == 20 )
							{
							i = GetParkNumber( PagePointer, channel);

							if(i == ST_PARK_GET)
								{
								if(page[PagePointer].PriorityQPointer < 9)
									{
									page[PagePointer].PriorityQPointer++;
									}
								}
							return i;
							}

						return ST_GET_DIS;
					}
				}

			//if( port[ channel ].PortType == 20 )

			if(param.IVRPort[channel].iType == 20)
				{
				i = GetParkNumber(PagePointer, channel);
				if(i == ST_PARK_GET)
					{
					if(page[PagePointer].PriorityQPointer < 9)
						{
						page[PagePointer].PriorityQPointer++;
						}
					}
				return i;
				}
			return ST_GET_DIS;


		// announce page vendor impairment message.
		//
		case ST_IMPAIRMENT_MSG:
            PrintMessage(MSGTYPE_IVR, 
							hwndD4xMsg, 
							"ST_IMPAIRMENT_MSG - hanging up on caller, "
							"after impairment announcement has played.");
			// hang up on caller, after the announcement has played.
			return(ST_ONHK);
 
        case ST_GET_DIS:
        case ST_VOICE_DTMF:
			GetDigitBuffer( channel );		

			k = TRUE;
            if( page[ PagePointer ].ACKMin != -1 && StatusOn && param.IgnoreStatus == FALSE )
                if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] != 'Y' )
					k = FALSE;

            if(strncmp(param.VoicePass, port[channel].digsave, strlen(param.VoicePass)) == 0  
				&& k == TRUE 
				&& isPageAllDevicesOn == FALSE) //Don't enter if PageAllDevices is ON
				{
                if(strlen(param.VoicePass) == strlen(port[channel].digsave) - 1)
					{
                    if(strncmp(page[PagePointer].FileName, "ON CALL", 7) != 0 &&
						page[PagePointer].PageStatus[0] != 'L')
						{
                        if(Get_Frq(PagePointer, param.VoiceQues, param.VoiceQues2) == TRUE)
							{
                            page[PagePointer].VoiceFile = TRUE;
                            PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "Line %i Voice password accepted.", channel);
                            return(ST_CB_VOICE);
							}
                        else
							{
                            PrintMessage(MSGTYPE_IVR, hwndD4xMsg,
											"Line %i Voice pager not assigned. Check frq %s.",
											channel,
											page[ PagePointer ].Extension);
                            port[channel].Errors++;
                            return(ST_NO_VOICE);
							}
						}
                    else
						{
                        page[PagePointer].PageStatus[0] = 'L';
                        page[PagePointer].VoiceFile = TRUE;
                        PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "Line %i On Call voice password accepted.", channel);
                        return( ST_CB_VOICE );
						}
					}
				}
            else if(strncmp( param.ListPass, port[channel].digsave, strlen( param.ListPass ) ) == 0 ||
					strncmp( param.ListPassN, port[channel].digsave, strlen( param.ListPassN ) ) == 0 )
				{
                page[ PagePointer ].VoiceFile = 1;
                if( strncmp( param.ListPassN, port[channel].digsave, strlen( param.ListPassN ) ) == 0 )
					{
					page[ PagePointer ].VoiceFile = 2;
					}
                memset(string2, 0, sizeof(string2));
                strcpy(string2, port[ channel ].digsave + 5);

                strncpy(page[ PagePointer ].PageType, Queue[ 0 ].Id + 1, 2);

				// now see if the list exists. use ListGetFirst() to
				//  retrieve the first element, if successful then the list exists.
				//  the data from this call is not used.
				//
				rv = SUCCESS;
                if(SUCCESS == rv)
					{
                    if(param.CustPrompt)
						{
                        memset(string2, 0, sizeof(string2));
                        strcat(string2, page[PagePointer].Extension);
                        getfile(string2, "VCE", sizeof(string2));
                        rv = GetFileAttributes(string2);

                        if(rv != 0xFFFFFFFF)
							{
							return ST_LSTOK_VCE;
							}
						}
                    return ST_LSTOK_MSG;
					}
                port[channel].Errors++;
                return ST_LSTNG_MSG;
				}

            if( strncmp( port[channel].digsave, param.VPromptPass, strlen( param.VPromptPass ) ) == 0 )
				{
				return( ST_VCE_MSG );
				}

            if(port[channel].digsave[0] == '0' &&
				(port[channel].digsave[1] == 0 || port[channel].digsave[1] == '#') &&
				param.OpTransfer[0] != 0)
				{
				return ST_OP_TRANSFER;
				}

            if(strncmp(port[channel].digsave, param.ParkPass, strlen(param.ParkPass)) == 0)
				{
                if(param.FlagParkOffset == 0  || 
					(page[PagePointer].Flag1[0] == 'Y' && 
					 (Status[page[PagePointer].ACKMin].Page[0] == 'Y' || param.IgnoreStatus)))
					{
                    i = GetParkNumber(PagePointer, channel);

                    if(i == ST_PARK_GET)
						{
                        if(page[PagePointer].PriorityQPointer < 9)
							{
                            page[PagePointer].PriorityQPointer++;
							}
                        return ST_PARK_TMSG;
						}
                    return i;
					}
				}

            if( strncmp( port[ channel ].digsave, param.vmPassword, strlen( param.vmPassword ) ) == 0 )
				{
                if( page[ PagePointer ].Flag5[ 0 ] == 'Y' )
					{
					if( Get_Frq( PagePointer, param.vmFrqType, param.vmFrqType ) == TRUE )
						{
	                    return( ST_VM_TR_MSG );
						}
					else
						{
						if( param.OpTransfer[ 0 ] != 0 )
							{
							return( ST_OP_TRANSFER );
							}
						}
					}
				}

            if( strncmp( port[channel].digsave, param.MenuPass, strlen( param.MenuPass ) ) == 0 )
				{
                if(DATA_SOURCE_LOCAL == DataSource)
					{
                    page[ PagePointer ].PageStatus[ 0 ] = 'I';
					port[ channel ].timer.wHour = 70;
                    return( ST_ONHK );
					}

                if( strcmp( page[ PagePointer ].PagerId, page[ PagePointer ].InitialPagerId ) != 0 )
					{
                    strcpy( page[ PagePointer ].PagerId, page[ PagePointer ].InitialPagerId );
                    strcpy( page[ PagePointer ].Extension, page[ PagePointer ].InitialExtension );

					GetPagerId( page[ PagePointer ].PagerId, PagePointer, BY_PAGERID);

                    LogPager( page[ PagePointer ].Extension, PagePointer );
                    memset( string, 0, sizeof( string ) );
                    sprintf( string, "%%%s", page[ PagePointer ].PageType );

                    Get_Frq(PagePointer, string, string);
					}
                j = 0;
                i = ST_ONHK;

                for( k=0; k<12; k++ )
					{
                    switch( param.StatPress[ k ][ 0 ] )
						{
                        case 'S':
                            i = ST_STAT_MSG;
                            j++;
                            break;
                        case 'V':
                            i = ST_VCE_MSG;
                            j++;
                            break;
                        case 'A':
                            i = ST_PASS_MSG;
                            j++;
                            break;
                        case 'M':
                            if( page[ PagePointer ].Password[ 0 ] == 0 )
								{
                                page[ PagePointer ].Printed[ 0 ] = 'P';
                                i = ST_STAT_M_MSG;
								}
                            else
								{
                                page[ PagePointer ].Printed[ 0 ] = 0;
                                i = ST_STAT_M_PASS;
								}
                            j++;
                            break;
						}
					}





                if( j > 1 )
					{
                    if( page[ PagePointer ].Password[ 0 ] == 0 )
						{
                        if( param.StPromptStat )
                            i = ST_MENU_STAT;
                        else
                            i = ST_MENU_MSG;
                        page[ PagePointer ].Printed[ 0 ] = 'P';
						}
                    else
						{
                        i = ST_MENU_PASS;
                        page[ PagePointer ].Printed[ 0 ] = 0;
						}
					}

                if( i == ST_ONHK )
					{
					page[ PagePointer ].PageStatus[ 0 ] = 'I';
					port[ channel ].timer.wHour = 70;
					}
                return( i );
				}

            if( ( strlen( port[channel].digsave ) == 0 ) || ( port[channel].digsave[0] == '#' ) )
				{
                if( page[ PagePointer ].ComNumber == -1 )
					{
                    page[ PagePointer ].PageStatus[ 0 ] = 'I';
					port[ channel ].timer.wHour = 70;
                    return( ST_ONHK );
					}
                if( param.ComPort[ page[ PagePointer ].ComNumber ].QueType[ page[ PagePointer ].ComOffset ][ 0 ] != 'B' )
					{
                    page[ PagePointer ].PageStatus[ 0 ] = 'I';
					port[ channel ].timer.wHour = 70;
                    return( ST_ONHK );
					}
				}

            if( port[ channel ].state == ST_VOICE_DTMF )
				{
                page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}

            j = strlen( param.PriorityPass );
            i = strlen( port[ channel ].digsave );

            if( i>j && page[ PagePointer ].Flag4[ 0 ] == 'Y' )
				{
                if( strncmp( port[ channel ].digsave + i - j, param.PriorityPass, j ) == 0 )
					{
                    if( page[ PagePointer ].PriorityQPointer < 9 )
						{
                        page[ PagePointer ].PriorityQPointer++;
						}
                    port[ channel ].digsave[ i - j ] = 0;
					}
				}

            result1 = strlen( port[ channel ].digsave );
            if( port[ channel ].digsave [ result1 - 1 ] == '#' )
				{
				port[ channel ].digsave [ result1 - 1 ] = 0;
				}

            if( ( int )strlen( port[channel].digsave ) < param.MinMessage )
				{
                if( page[ PagePointer ].ComNumber == -1 )
                    return( ST_CB_MSG );

                if( param.ComPort[ page[ PagePointer ].ComNumber ].QueType[ page[ PagePointer ].ComOffset ][ 0 ] != 'B' )
                    return( ST_CB_MSG );
				}

            j = 0;

            for( i=0; i<(int)strlen( port[ channel ].digsave ) && i<30; i++ )
				{
                page[ PagePointer ].DisplayInfo[ j ] = port[ channel ].digsave[ i ];
                j++;
                if( strncmp( port[ channel ].digsave+i, param.ReenterPass, strlen( param.ReenterPass ) ) == 0 )
					{
                    return( ST_CB_MSG );
					}
				}

            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
					"Line [%i] Display [%s] for extension [%s] received.",
					channel,
					page[PagePointer].DisplayInfo,
					page[PagePointer].Extension);

            if( page[ PagePointer ].ACKMin != -1 && StatusOn && param.IgnoreStatus == FALSE )
				{
                switch( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] )
					{
                    case 'N':
                    case ' ':
                    case 'O':
                    case '#':
                    case '0':
                    case 'V':
					case 'U':
                        page[ PagePointer ].PageStatus[0] = 'I';
                        port[ channel ].External++;
						port[ channel ].timer.wHour = 70;
                        return( ST_ONHK );
                        break;

                    case 'M':
                        WriteUserLogEx(PagePointer, -1);
                        return( ST_GOODBYE2_MSG );
                        break;

                    case 'F':
                        if( Status[ page[ PagePointer ].ACKMin ].Page[ 2 ] != 'P' )
							{
                            page[ PagePointer ].PageStatus[0] = 'I';
                            port[ channel ].External++;
							port[ channel ].timer.wHour = 70;
                            return( ST_ONHK );
							}
                        break;

					case 'T':
						if( param.OpTransfer[ 0 ] != 0 ) return( ST_OP_TRANSFER );
						return( ST_ONHK );
						break;

                    default:
                        break;
					}
				}
			// 7/12/2016 - SOguz - DE20747 - Incase of incomplete pager, do not enter this 
			// if statement when PageAllDevicesOn since there may be other pagers to be paged.
            if( page[ PagePointer ].PageType[ 0 ] == 0 && isPageAllDevicesOn == FALSE)
				{
                page[ PagePointer ].PageStatus[0] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}

            if( port[ channel ].LoopCurrent == FALSE )
				{
				port[ channel ].timer.wHour = 70;

				// 6/24/2016 - DE20747 - SOguz - we received the call back number
				// Move to next state and add each devices to queue
				if (isPageAllDevicesOn) 
				{ 
					return ST_ADD_ALL_DVCS_Q; 
				}

				return( ST_ADD_PAGE );
				}

            return( ST_GOODBYE_MSG );

		case ST_EXCP_TIME:
			return( ST_GET_DIS );


        /*                                                                  */
        /*      This section is the main menu for changing user info.       */
        /*                                                                  */
        case ST_MENU_PASS:
            return( ST_MENU_PSCHK );

        case ST_MENU_PSCHK:
			GetDigitBuffer( channel );

            PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
							"Line [%i], Main menu password [%s], Entered [%s].",
							channel,
							page[ PagePointer ].Password,
							port[ channel ].digsave );

            if(strncmp(port[channel].digsave,
				page[PagePointer].Password,
				strlen(page[PagePointer].Password)) == 0 )
				{
                page[ PagePointer ].Printed[ 0 ] = 'P';

                if( param.StPromptStat )
                    return( ST_MENU_STAT );
                else
                    return( ST_MENU_MSG );
				}
            return( ST_MENU_PSERR );

        case ST_MENU_PSERR:
        case ST_STAT_M_PSERR:
        case ST_VCE_PSERR:
        case ST_STAT_PSERR:
        case ST_PASS_PSERR:
            if( page[ PagePointer ].attempts < 4 ){
                switch( port[ channel ].state ){
                    case ST_MENU_PSERR:
                        return( ST_MENU_PASS );
                    case ST_STAT_M_PSERR:
                        return( ST_STAT_M_PASS );
                    case ST_VCE_PSERR:
                        return( ST_VCE_PASS );
                    case ST_STAT_PSERR:
                        return( ST_STAT_PASS );
                    case ST_PASS_PSERR:
                        return( ST_PASS_PASS );
                    default:
                        break;
                }
            }
            page[ PagePointer ].PageStatus[ 0 ] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

        case ST_MENU_STAT:
        case ST_STAT_WRITE2:
			if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'U' )
				return( ST_MENU_STAT_2 );
        case ST_MENU_STAT_2:
            return( ST_MENU_MSG );


        case ST_MENU_MSG:
            return( ST_MENU_CHK );

        case ST_MENU_CHK:
			GetDigitBuffer( channel );
			
            if( port[channel].digsave[0] == 0 )
			{
				// if( evtcode != TDX_CST || cstp->cst_event != DE_DIGITS ){
                PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i not enough digits selecting menu.", channel );
                return( ST_GOODBYE2_MSG );
			}

			i = GetMenuIndex(port[channel].digsave[0]);
			
			if( i == 0)
			{
				if(param.OpTransfer[ 0 ] != 0) return(ST_OP_TRANSFER);
			}
			else if(i > 0)
			{
				int retVal = -1;

				switch( param.StatPress[ i ][ 0 ] )
				{
				case 'S':
					if( param.StPromptStat ) return( ST_STAT_TYPES );
					retVal= ST_STAT_MSG;
					break;

				case 'V':
					retVal = ST_VCE_MSG; 
					break;

				case 'M':
					if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' )
						retVal = ST_STAT_M_MSG;
					else
						retVal = ST_STAT_M_PASS; 
					break;

				case 'A':
					retVal = ST_PASS_MSG; 
					break;

				case 'R':
					retVal = ST_MENU_MSG; 
					break;

				default:
					// # pressed, return
					if (i == 11)  retVal = ST_GOODBYE2_MSG;
					else if(i == 10)
					{
						retVal = ST_STAT_MSG7;//play a wave file
						strcpy(page[PagePointer].FileName, "SUPER");
					}
					break;
				}

				if(retVal > 0) return retVal;
			}
			

            if( param.StPromptStat )
                return( ST_MENU_STAT );
            else
                return( ST_MENU_MSG );
			

        case ST_MENU_HELLO:
            memset(string2, 0, sizeof(string2));
            strcpy(string2, page[ PagePointer ].Extension);
            //getfile(string2, "VCE");
			getfile(string2, "VCE", sizeof(string2));

            rv = GetFileAttributes( string2 );

            if( rv != 0xFFFFFFFF )
				return( ST_MENU_V_PRMPT);

            return( ST_MENU_I_PRMPT );

        case ST_MENU_V_PRMPT:
        case ST_MENU_I_PRMPT:
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' ){
                if( param.StPromptStat )
                    return( ST_MENU_STAT );
                else
                    return( ST_MENU_MSG );
            }
            return( ST_MENU_PASS );

        case ST_OP_TRANSFER:
			page[ channel ].tempInt = 0;
            return( ST_OP_TRANSFER2 );

        case ST_OP_TRANSFER2:
			if( page[ channel ].tempInt == -1 ){
				page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
				return( ST_ONHK );
			}
			else{
				return( ST_OP_TRANSFER2 );
			}


        /*                                                                  */
        /*      This section is used to check/change pageing status         */
        /*                                                                  */
        case ST_STAT_HELLO:
            memset(string2, 0, sizeof(string2));
            strcat(string2, page[ PagePointer ].Extension);
            getfile(string2, "VCE", sizeof(string2));
            rv = GetFileAttributes( string2 );

            if( rv != 0xFFFFFFFF )
				return( ST_STAT_V_PRMPT );

            return( ST_STAT_I_PRMPT );

        case ST_STAT_V_PRMPT:
        case ST_STAT_I_PRMPT:
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}
            return( ST_STAT_MSG );

        case ST_STAT_MSG2:
            return( ST_STAT_MSG3 );

        case ST_STAT_MSG6:
            return( ST_STAT_MSG7 );

        case ST_STAT_MSG7:
            if( param.ConfirmChange ) return( ST_STAT_MSG3 );
            if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' )
                return( ST_STAT_TYPES );
            else
                return( ST_STAT_PASS );

        case ST_STAT_MSG:
        case ST_STAT_MSG3:
            if( param.ConfirmChange ) return( ST_STAT_NEXT );
            if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' )
                return( ST_STAT_TYPES );
            else
                return( ST_STAT_PASS );

        case ST_STAT_NEXT:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == 0 || port[ channel ].digsave[ 0 ] == '#' ){
				// did not get the digits, re-prompt the user

                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i No response while checking status.",
								channel );

                if( CheckMenu( ) ){
                    if( param.StPromptStat )
                        return( ST_MENU_STAT );
                    else
                        return( ST_MENU_MSG );
                }
                return( ST_GOODBYE2_MSG );
			}
            if( port[channel].digsave[0] == '*' ){
                if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' )
                    return( ST_STAT_TYPES );
                else
                    return( ST_STAT_PASS );
            }
            if( port[ channel ].digsave[ 0 ] == '0' && param.OpTransfer[ 0 ] != 0 ) return( ST_OP_TRANSFER );
            return( ST_GOODBYE2_MSG );

        case ST_STAT_M_PASS:
            return( ST_STAT_M_PSCHK );

        case ST_STAT_M_PSCHK:
			GetDigitBuffer( channel );

            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
							"Line %i Password %s, Entered %s.",
							channel,
							page[ PagePointer ].Password,
							port[ channel ].digsave );

            if( strncmp( port[ channel ].digsave,
				page[ PagePointer ].Password,
				strlen( page[ PagePointer ].Password ) ) == 0 )
				{
                page[ PagePointer ].Printed[ 0 ] = 'P';
                return( ST_STAT_M_MSG );
				}
            return( ST_STAT_M_PSERR );

        case ST_STAT_M_MSG:
            return( ST_STAT_M_GET );

        case ST_STAT_M_GET:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == 0 || port[ channel ].digsave[ 0 ] == '#' )
				{
                if( CheckMenu( ) )
					{
                    if( param.StPromptStat )
                        return( ST_MENU_STAT );
                    else
                        return( ST_MENU_MSG );
					}
                return( ST_STAT_MSG );
				}

            if( port[ channel ].digsave[ 0 ] == '1' )
				{
				// caller selected - "undelivered messages"
                page[ PagePointer ].VoiceFile = 1;
                return( ST_STAT_M_MSG2 );
				}
            if( port[ channel ].digsave[ 0 ] == '2' )
				{
				// caller selected - "all messages"
                page[ PagePointer ].VoiceFile = 2;
                return( ST_STAT_M_MSG2 );
				}
				// add state to forward and email voice messages
				// conditional on message type??
			if(page[PagePointer].voiceMsg)
				{
				// add to more options
				}

            return( ST_STAT_M_MSG );

        case ST_STAT_M_DATE:
            return( ST_STAT_M_TALK );

        case ST_STAT_M_DATE2:
            return( ST_STAT_M_VOICE );

        case ST_STAT_M_DATE3:
            return( ST_STAT_M_TEXT );

        case ST_STAT_M_DATE4:
            return( ST_STAT_M_TALK2 );

        case ST_STAT_M_DATE5:
            return( ST_STAT_M_STAT );

        case ST_STAT_M_DATE6:
            return( ST_STAT_M_CVR );


        case ST_STAT_M_MSG2:
			// signal the message search routine that a new
			//  IVR search session has started.
			page[PagePointer].bMessageLimitsDefined = FALSE;
            return(ST_STAT_M_SRCH);

        case ST_STAT_M_SRCH:
        case ST_STAT_M_TALK:
        case ST_STAT_M_TALK2:
        case ST_STAT_M_TEXT:
        case ST_STAT_M_STAT:
        case ST_STAT_M_CVR:
			// if MessageRtrOpt is non-zero, then this is the
			//  touchtone controlled mode of retrieval, where
			//  the user presses 1, 2, or 3 for:
			//                   previous, repeat current, or next message.
            if(param.MessageRtrOpt)
				{
				return ST_STAT_M_MSG3;
				}

			// it is not the user controlled message retrieval mode,
			//  simply request the next previous (older message).
			page[PagePointer].iMessageRetrieveRequest = REQUEST_PLAY_OLDER_MESSAGE;

			// and continue message searching.
            return ST_STAT_M_SRCH;

        case ST_STAT_M_MSG3:
            return ST_STAT_M_GET3;

        case ST_STAT_M_GET3:
			// process the response to retrieval question
			//  1 for previous, 2 for current, 3 for next message.
			GetDigitBuffer(channel);
			dx_clrdigbuf(chdev);

            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
							"Line %i Message Current %i, Last %i",
							channel,
							page[ PagePointer ].tempInt,
							page[ PagePointer ].ScriptFlag );

            page[PagePointer].Flag1[0] = 'N';

            if(port[channel].digsave[0] == 0 || port[channel].digsave[0] == '#')
				{
				// entry of '0' or '#' exits the message retrieval process.
                if(CheckMenu())
					{
                    if(param.StPromptStat)
						{
                        return ST_MENU_STAT;
						}
                    else
						{
                        return ST_MENU_MSG;
						}
					}
                return ST_STAT_MSG;
				}

			// Play Previous Message
            if(port[channel].digsave[0] == '1')
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND, hwndDatabaseMsg, "REQUEST_PLAY_OLDER_MESSAGE");
				page[PagePointer].iMessageRetrieveRequest = REQUEST_PLAY_OLDER_MESSAGE;
				return ST_STAT_M_SRCH;
				}

			// Repeat Current Message
            if(port[channel].digsave[0] == '2')
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND, hwndDatabaseMsg, "REQUEST_REPEAT_MESSAGE");
				page[PagePointer].iMessageRetrieveRequest = REQUEST_REPEAT_MESSAGE;
				return ST_STAT_M_SRCH;
				}

			// Play Next Message
            if(port[channel].digsave[0] == '3')
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND, hwndDatabaseMsg, "REQUEST_PLAY_NEWER_MESSAGE");
				page[PagePointer].iMessageRetrieveRequest = REQUEST_PLAY_NEWER_MESSAGE;
				return ST_STAT_M_SRCH;
				}

			// add check for forward and email options
			if(page[PagePointer].voiceMsg)
				{
				if(port[channel].digsave[0] == '4')
					{
					// only when state is ST_STAT_M_SRCH
					//  start playing message here 
					return ST_STAT_M_FORWARD;
					}
				if(port[channel].digsave[0] == '5')
					{
					return ST_STAT_M_EMAIL;
					}
				}

			// check for zero pressed...
			if(port[channel].digsave[0] == '0')
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND, hwndDatabaseMsg, "User Pressed ZERO ..........................");

				// if dial operator string is defined, go ahead and use it, 
				//  otherwise... wait for better response or timeout.
				//
				if(param.OpTransfer[0] != 0)
					{
					// a transfer string is defined... ok to transfer.
					return ST_OP_TRANSFER;
					}
				}

            return ST_STAT_M_MSG3;

        case ST_STAT_M_VOICE:
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            if(param.MessageRtrOpt)
				{
				return ST_STAT_M_MSG3;
				}

            page[ PagePointer ].tempInt--;

            return ST_STAT_M_SRCH;

        case ST_STAT_M_NO2:
            if(param.MessageRtrOpt)
				{
				return ST_STAT_M_MSG3;
				}

        case ST_STAT_M_NO:
            if( CheckMenu( ) ){
                if( param.StPromptStat )
                    return( ST_MENU_STAT );
                else
                    return( ST_MENU_MSG );
            }
            return( ST_GOODBYE2_MSG );

        case ST_STAT_PASS:
            return( ST_STAT_PSCHK );

        case ST_STAT_PSCHK:
			GetDigitBuffer( channel );
            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
							"Line %i Password %s, Entered %s.",
							channel,
							page[ PagePointer ].Password,
							port[ channel ].digsave );

            if( strncmp(port[ channel ].digsave,
						page[ PagePointer ].Password,
						strlen( page[ PagePointer ].Password ) ) == 0 )
				{
				// memset( page[ PagePointer ].Password, 0, 10 );
                page[ PagePointer ].Printed[ 0 ] = 'P';
                return( ST_STAT_TYPES );
				}
            return( ST_STAT_PSERR );

        case ST_STAT_TYPES:
        case ST_STAT_MSG5:
            return( ST_STAT_CHANGE );

        case ST_STAT_MSG4:
            return( ST_STAT_MSG5 );

        case ST_STAT_CHANGE: 
			page[PagePointer].NextAckPtr = page[PagePointer].ACKMin;

        case ST_STAT_CHANGE2:
			GetDigitBuffer(channel);

            if(param.StatPoundOpt[0] != 0 && port[channel].digsave[0] == '#')
				{
                switch(param.StatPoundOpt[0])
					{
                    case 'A':
						return ST_PASS_MSG;

                    case 'M':
						return ST_STAT_M_MSG;

                    case 'R':
						return ST_STAT_TYPES;

                    case 'V':
                        return ST_VCE_MSG;

                    default:
                        break;
					}
				}

            if( ( port[channel].digsave[0] == 0 || port[channel].digsave[0] == '#')  &&
				port[channel].state == ST_STAT_CHANGE)
				{
                PrintMessage(MSGTYPE_IVR, hwndD4xMsg,
								"Line %i Not enough digits while changing status.",
								channel);
                if(CheckMenu())
					{
                    if(param.StPromptStat)
						{
 						return ST_MENU_STAT;
						}
                    else
						{
                       return ST_MENU_MSG;
						}
					}

               return ST_GOODBYE2_MSG;
				}

            i = atoi(port[channel].digsave);

            if(port[channel].state == ST_STAT_CHANGE2)
				{
                if(port[channel].digsave[0] == '#' || port[channel].digsave[0] == 0)
					{
                    i = page[PagePointer].ACKMin;
					}
                else
					{
                    i = (page[PagePointer].ACKMin * 10) + i;
					}
				}

            if(param.Status10s && i == 1 && port[channel].state == ST_STAT_CHANGE)
				{
                page[PagePointer].ACKMin = i;
               return ST_STAT_CHANGE2;
				}

            if(param.Status20s && i == 2 && port[channel].state == ST_STAT_CHANGE)
				{
                page[PagePointer].ACKMin = i;

 				return ST_STAT_CHANGE2;
				}

            if(Status[i].Page[0] == ' ')
				{
				return ST_STAT_TYPES;
				}

            if(Status[i].Page[0] == 'P')
				{
				return ST_PASS_MSG;
				}

            if(Status[i].Page[0] == 'C' && param.CoverPage)
				{
                page[PagePointer].ACKMin = i;
 				return ST_STAT_C_MSG;
				}

            if( Status[i].Page[0] == '#' ||
				Status[i].Page[0] == 'V' ||
				Status[i].Page[0] == '0')
				{
                if(param.CForwardQueue[0] == 0)
					{
					return ST_STAT_TYPES;
					}

                page[PagePointer].ACKMin = i;
                page[PagePointer].ScriptNumber = -1;
                page[PagePointer].ComNumber = -1;

                for(j = 0; j < param.PgTypsNum; j++)
					{
                    if((strncmp(param.CForwardQueue, param.PageType[j].pageid, 3) == 0) ||
					   (strncmp(param.CForwardQueue, param.PageType[j].alias, 3) == 0))
						{
                        page[PagePointer].ScriptNumber = j;
						}
					}

                if(Get_Frq(PagePointer, param.CForwardQueue, param.CForwardQueue) == TRUE &&
					page[PagePointer].ScriptNumber != -1)
					{
                    if(Status[i].Page[0] == '#') 
						{
						return ST_STAT_RFD_MSG;
						}

                    return ST_STAT_WRITE;
					}
                else
					{
                    PrintMessage(MSGTYPE_IVR,  hwndD4xMsg,
									"Line %i Remote call forwarding %s not allowed. Check frq %s.",
									channel,
									param.CForwardQueue,
									page[PagePointer].Extension);

                    port[channel].Errors++;

					return ST_STAT_RFD_NG;
					}
				}

            if(port[channel].digsave[0] >= '0' || port[channel].digsave[0] <= '9')
				{
                port[channel].digsave[1] = 0;

                if(Status[i].Prompt != -1)
					{
                    if(Status[i].Page[0] == 'R')
						{
						return ST_STAT_TYPES;
						}

                    k = FALSE;

                    if(param.MultiQueNumber > 0)
						{
                        if(strncmp(page[PagePointer].PageType, Queue[param.MultiQueNumber].Id + 1, 2) == 0)
							{
                            k = TRUE;
							}
						}

                    if(param.MultiQueNumber2 > 0)
						{
                        if(strncmp(page[PagePointer].PageType, Queue[param.MultiQueNumber2].Id + 1, 2) == 0)
							{
                            k = TRUE;
							}
						}

                    if(page[PagePointer].ComNumber == -1 &&
						page[PagePointer].ScriptNumber == -1 &&
						Status[i].Page[0] == 'Y' &&
						k == FALSE)
						{
						return ST_STAT_TYPES;
						}

                    if(page[PagePointer].Flag3[0] == 'Y' && Status[i].Page[0] != 'Y')
						{
						return ST_STAT_TYPES;
						}

                    page[PagePointer].ACKMin = i;

                    if(Status[i].Page[0] == 'F' || Status[i].Page[0] == 'O')
						{
						return ST_STAT_F_MSG;
						}

                    if(Status[i].Page[0] == 'U')
						{
						return ST_STAT_EX_MSG1;
						}

                    memset(page[PagePointer].PageInfo, 0, sizeof(page[PagePointer].PageInfo));
                    memset(page[PagePointer].FileName, 0, sizeof(page[PagePointer].FileName));
                    memset(page[PagePointer].InitiatorId, 0, sizeof(page[PagePointer].InitiatorId));

                    if(param.StPromptStat)
						{
						return ST_STAT_WRITE2;
						}

					return ST_STAT_WRITE;
                }
            }

            if(port[channel].digsave[0] == '0' && param.OpTransfer[0] != 0)
				{
				return ST_OP_TRANSFER;
				}

            return ST_STAT_TYPES;

        case ST_STAT_WRITE:
            if( param.ConfirmChange ) return( ST_STAT_NEXT );
            return( ST_STAT_TYPES );

        case ST_STAT_F_MSG:
            return ST_STAT_F_GET;

        case ST_STAT_F_GET:
			GetDigitBuffer(channel);

            if(port[channel].digsave[0] == '#' || port[channel].digsave[0] == 0)
				{
                page[PagePointer].ACKMin = page[PagePointer].NextAckPtr;
                return ST_STAT_MSG;
				}

            if(port[channel].digsave[0] == '0' &&
				(port[channel].digsave[1] == 0 || port[channel].digsave[1] == '#') &&
				param.OpTransfer[0] != 0)
				{
				return ST_OP_TRANSFER;
				}

            memset(string2, 0, sizeof(string2));
            strcpy(string2, port[channel].digsave);
            memset(page[PagePointer].PageInfo, 0, sizeof(page[PagePointer].PageInfo));

            for(i = 0; i < 30; i++)
				{
                if(string2[i] == '#') 
					{
					break;
					}
                page[PagePointer].PageInfo[i] = string2[i];
				}

            if(page[PagePointer].PageInfo[0] == 0)
				{
                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i Not enough digits while getting forwarding number.",
								channel );

                if(CheckMenu())
					{
                    if(param.StPromptStat)
                        return(ST_MENU_STAT);
                    else
                        return(ST_MENU_MSG);
					}
                return(ST_STAT_MSG);
				}

            memset(page[PagePointer].FileName, 0, sizeof(page[PagePointer].FileName));
            memset(page[PagePointer].InitiatorId, 0, sizeof(page[PagePointer].InitiatorId));

            if( param.StPromptStat )
				return( ST_STAT_WRITE2 );
            return( ST_STAT_WRITE );

		case ST_STAT_EX_MSG1:
            return( ST_STAT_EX_GET1 );

		case ST_STAT_EX_GET1:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == 0 )
				{
                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
                return( ST_STAT_MSG );
				}
            if( port[ channel ].digsave[ 0 ] == '0' &&
				( port[ channel ].digsave[ 1 ] == 0 ||
				port[ channel ].digsave[ 1 ] == '#' ) &&
				param.OpTransfer[ 0 ] != 0 )
				{
				return( ST_OP_TRANSFER );
				}
            memset( page[ PagePointer ].tempStr, 0, sizeof( page[ PagePointer ].tempStr ) );
            for( i=0; i<5; i++ )
				{
                if( port[ channel ].digsave[ i ] == '#' )
					{
	                port[ channel ].digsave[ i ] = 0;
					break;
					}
				}
			i = strlen( port[ channel ].digsave );
			memset( string, 0, sizeof( string ) );

			switch(i)
				{
				case 0:
		            memset(string2, 0, sizeof(string2));

					// get tomorrows date and time.
					//  (24 hours from now);
					AddTime(string2);

		            sprintf(string, "%2.2s %2.2s", string2, string2 + 3);
					break;

				case 1:
					if( port[ channel ].digsave[ 0 ] == '*' )
		            	sprintf( string, "%2.2s %2.2s", xnSystemTime, xnSystemTime+3 );
					else{
		                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		                return( ST_STAT_MSG );
					}
					break;

				case 2:
					if( port[ channel ].digsave[ 0 ] == '*' || port[ channel ].digsave[ 1 ] == '*' )
						{
		                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		                return( ST_STAT_MSG );
						}
					sprintf( string, "0%1.1s 0%1.1s", port[ channel ].digsave, port[ channel ].digsave + 1 );
					break;

				case 3:
					for( i=0; i<3; i++ )
						{
						if( port[ channel ].digsave[ i ] == '*' )
							{
			                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		                	return( ST_STAT_MSG );
							}
						}

					switch( port[ channel ].digsave[ 0 ] )
						{
						case '1':
							if( port[ channel ].digsave[ 1 ] < '3' && port[ channel ].digsave[ 2 ] != '0' )
								{
								sprintf( string2,
										"0%1.1s%2.2s",
										port[ channel ].digsave, port[ channel ].digsave + 1 );

								i = atoi( string2 );     /* date as 0111 */
								sprintf( string2,
										"%2.2s0%1.1s",
										port[ channel ].digsave,
										port[ channel ].digsave + 2 );

								j = atoi( string2 );     /* date as 1101 */
				            	sprintf( string2,
										"%2.2s%2.2s",
										xnSystemTime,
										xnSystemTime+3 );
								k = atoi( string2 );	/* system date   */
								if( k < i )
									{
									sprintf( string,
											"0%1.1s %2.2s",
											port[ channel ].digsave,
											port[ channel ].digsave + 1 );
									}
								else
									{
									if( k < j )
										{
										sprintf( string,
												"%2.2s 0%1.1s",
												port[ channel ].digsave,
												port[ channel ].digsave + 2 );
										}
									else
										{
										sprintf( string,
												"0%1.1s %2.2s",
												port[ channel ].digsave,
												port[ channel ].digsave + 1 );
										}
									}
								}
							else
								{
								sprintf( string,
										"0%1.1s %2.2s",
										port[ channel ].digsave,
										port[ channel ].digsave + 1 );
								}
							break;
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							sprintf( string,
									"0%1.1s %2.2s",
									port[ channel ].digsave,
									port[ channel ].digsave + 1 );
							break;
						case '0':
						default:
            			    page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		                	return( ST_STAT_MSG );
					}
					break;

				case 4:
					for( i=0; i<4; i++ )
						{
						if( port[ channel ].digsave[ i ] == '*' )
							{
			                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		                	return( ST_STAT_MSG );
							}
						}
					sprintf( string,
							"%2.2s %2.2s",
							port[ channel ].digsave, port[ channel ].digsave + 2 );
					break;
				default:
	                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
	                return( ST_STAT_MSG );
			}
			i = atoi( string );
			j = atoi( string + 3 );
			switch( i )
				{
				case 4:
				case 6:
				case 9:
				case 11:
					k = 30;
					break;

				// february has 29 days in leap years.
				case 2:
					{
					int Year = atoi(xnSystemTime + 6);

					if(CheckLeapYear(Year))
						{
						k = 29;
						}
					else
						{
						k = 28;
						}
					}
					break;

				default:
					k = 31;
				}

			if( j > k || j == 0 )
				{
	            page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
	            return( ST_STAT_MSG );
				}

			sprintf(string2, "%2.2s%2.2s", xnSystemTime, xnSystemTime + 3);
			i = atoi(string2);	// system date 
			sprintf(string2, "%2.2s%2.2s", string, string + 3);
			j = atoi(string2);	// system date 

			if( j >= i )
				k = 0;
			else
				k = 1;
			j = atoi( xnSystemTime + 6 ) + k;
			sprintf( page[ PagePointer ].tempStr, "%5.5s %4.4d", string, j );

			PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Entered Exception Date %s.",
							channel,
							page[ PagePointer ].tempStr );

            return( ST_STAT_EX_MSG2 );

		case ST_STAT_EX_MSG2:
            return( ST_STAT_EX_GET2 );

		case ST_STAT_EX_GET2:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == 0 )
				{
                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
                return( ST_STAT_MSG );
				}
            if( port[ channel ].digsave[ 0 ] == '0' &&
				( port[ channel ].digsave[ 1 ] == 0 ||
				port[ channel ].digsave[ 1 ] == '#' ) &&
				param.OpTransfer[ 0 ] != 0 )
				{
				return( ST_OP_TRANSFER );
				}

			for( i=0; i<5; i++ )
				{
                if( port[ channel ].digsave[ i ] == '#' )
					{
	                port[ channel ].digsave[ i ] = 0;
					break;
					}
				}

			for( i=0; i<4; i++ )
				if( port[ channel ].digsave[ i ] == '*' )
					{
			        page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		            return( ST_STAT_MSG );
					}
			i = strlen( port[ channel ].digsave );
			memset( string, 0, sizeof( string ) );
			switch( i )
				{
				case 0:
		            page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
		            return( ST_STAT_MSG );
					break;
				case 1:
		            sprintf( string, "0%1.1s00", port[ channel ].digsave );
					break;
				case 2:
					sprintf( string, "%2.2s00", port[ channel ].digsave );
					break;
				case 3:
					sprintf( string, "0%3.3s", port[ channel ].digsave );
					break;
				case 4:
					sprintf( string, "%4.4s", port[ channel ].digsave );
					break;
				default:
	                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
	                return( ST_STAT_MSG );
			}

			// time must be less than 24:00
			i = atoi( string );
			if( i > 2400 )
				{
	            page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
	            return( ST_STAT_MSG );
				}

			sprintf( page[ PagePointer ].tempStr + 10, " %4.4s", string );

			PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Entered Exception Time %s.",
							channel,
							page[ PagePointer ].tempStr );

			// check for after noon 12:00
			if( i > 1200 )
				{
				if( SetSchedule( PagePointer ) )
					{
					memset( page[ PagePointer ].PageInfo, 0, sizeof( page[ PagePointer ].PageInfo ) );
					strcpy( page[ PagePointer ].PageInfo, page[ PagePointer ].tempStr );
            		if( param.StPromptStat )
						return( ST_STAT_WRITE2 );
            		return( ST_STAT_WRITE );
					}
				return( ST_STAT_EX_ERR );
				}
		    return( ST_STAT_EX_AMPM );

		case ST_STAT_EX_AMPM:
            return( ST_STAT_EX_GET3 );

        case ST_STAT_EX_GET3:
			GetDigitBuffer(channel);

			i = atoi(page[PagePointer].tempStr + 11);

			if(port[channel].digsave[0] == '2')
				{
				i += 1200;
				}

			sprintf(page[PagePointer].tempStr + 10, " %4.4i", i);

			PrintMessage(MSGTYPE_IVR, hwndD4xMsg,
							"Line %i Adjusted Exception Time %s.",
							channel,
							page[PagePointer].tempStr);

			if(SetSchedule(PagePointer))
				{
				memset(page[PagePointer].PageInfo, 0, sizeof(page[PagePointer].PageInfo));
				strcpy(page[PagePointer].PageInfo, page[PagePointer].tempStr);

				if(param.StPromptStat)
					{
					return(ST_STAT_WRITE2);
					}

            	return(ST_STAT_WRITE);
				}

			return(ST_STAT_EX_ERR);

		case ST_STAT_EX_ERR:
	        page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
	        return( ST_STAT_MSG );

        case ST_STAT_RFD_MSG:
            return ST_STAT_RFD_GET;

        case ST_STAT_RFD_GET:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == '#' || port[channel].digsave[0] == 0 )
				{
                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
                return( ST_STAT_MSG );
				}

            if(page[PagePointer].Flag2[0] != 'Y')
				{
                if(strchr(param.CForwardDigits, port[channel].digsave[0]) == NULL)
					{
					return ST_STAT_RFD_MSG;
					}
				}

            if( port[ channel ].digsave[ 0 ] == '0' &&
				( port[ channel ].digsave[ 1 ] == 0 || port[ channel ].digsave[ 1 ] == '#' ) &&
				param.OpTransfer[ 0 ] != 0 )
				{
				return( ST_OP_TRANSFER );
				}
            memset(string2, 0, sizeof(string2));
            strcpy( string2, port[ channel ].digsave );
            memset( page[ PagePointer ].PageInfo, 0, sizeof( page[ PagePointer ].PageInfo ) );

			// xxx
            for(i = 0; i < 15; i++)
				{
                if(string2[i] == '#')
					break;

                page[PagePointer].PageInfo[i] = string2[i];
				}

            if(page[PagePointer].PageInfo[0] == 0)
				{
                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i Not enough digits while changing status.",
								channel);

                if(CheckMenu())
					{
                    if( param.StPromptStat )
                        return( ST_MENU_STAT );
                    else
                        return( ST_MENU_MSG );
					}
                return( ST_STAT_MSG );
				}
            return( ST_STAT_WRITE );

        case ST_STAT_RFD_NG:
            return( ST_STAT_TYPES );

        case ST_STAT_C_MSG:
            return(ST_STAT_C_GET);

        case ST_STAT_C_GET:
			GetDigitBuffer(channel);
            if(port[channel].digsave[0] == '#' || port[channel].digsave[0] == 0)
				{
                page[ PagePointer ].ACKMin = page[ PagePointer ].NextAckPtr;
                return( ST_STAT_MSG );
				}

            if(port[channel].digsave[0] == '0' &&
			   (port[channel].digsave[1] == 0 || port[channel].digsave[1] == '#') &&
				param.OpTransfer[0] != 0)
				{
				return ST_OP_TRANSFER;
				}

            memset(string2, 0, sizeof(string2));
            strcpy(string2, port[channel].digsave);
            memset(page[PagePointer].AutCode, 0, sizeof(page[PagePointer].AutCode));

            for(i = 0; i < 10; i++)
				{
                if(string2[i] == '#') 
					break;

                page[PagePointer].AutCode[i] = string2[i];
				}

            if(page[PagePointer].AutCode[0] == 0)
				{
                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i Not enough digits while changing status.",
								channel);

                if(CheckMenu())
					{
                    if(param.StPromptStat)
						{
                        return ST_MENU_STAT;
						}
                    else
						{
                        return ST_MENU_MSG;
						}
					}
                return ST_STAT_MSG;
				}

            memset(page[PagePointer].InitiatorId, 0, sizeof(page[PagePointer].InitiatorId));

			//if(true == true)
            ////if(PageId)
			//	{

            i = GetPagerId(page[PagePointer].AutCode, PAGE_COV_POS, BY_PAGERID);
            if(i == TRUE)
				{
                memset(page[PagePointer].AutCode, 
						0,
						sizeof(page[PagePointer].AutCode));
                strcpy(page[PagePointer].AutCode, page[PAGE_COV_POS].Extension);
				}
            else
				{
				return ST_STAT_C_NG2;
				}

			//	}

            i = CoverLog(page[PagePointer].AutCode, PagePointer);

            page[PagePointer].ScriptNumber = page[PAGE_COV_POS].ACKMin;
            strncpy(page[PagePointer].PageInfo, page[PAGE_COV_POS].PageInfo, 25);			//??This only copies 25 of 148 bytes?? JPH 12/21/12  -EOW (End of world)
            strncpy(page[PagePointer].InitiatorId, page[PAGE_COV_POS].InitiatorId, 22);

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Cover log: %i",
							channel,
							i );
           switch(i)
				{
                case 0: // all tests for the proposed coverage were ok.
					if(strncmp(page[PagePointer].Extension, page[PagePointer].AutCode,
								strlen(page[PagePointer].Extension)) == 0)
						{
                        return ST_STAT_C_NG1;
						}

                    if(page[PAGE_COV_POS].ACKMin != -1)
						{
                        if(Status[page[PAGE_COV_POS].ACKMin].Page[0] == 'Y')
							{
                            page[PagePointer].PageStatus[0] = 'S';
                            strcpy(page[PagePointer].FileName, page[PagePointer].AutCode);

							// patch point.

							// a final check before the coverage is written.
							//  see if there is "page vendor impairment", if so
							//  don't allow the coverage.  recycle with an informative message, 
							//  detailing the current situation.
							//  for now, just use an existing message state machine path 
							//  leading to recycle.
							//
							{
							CString PageDeviceType;
							PageDeviceType.Format("%%%s", page[PAGE_COV_POS].PageType);

							if(g_IMP.IsImpairedVendor(PageDeviceType))
								{
								PrintMessage(MSGTYPE_IVR,
											hwndD4xMsg, 
											"case 0: vendor IS impaired. PageType [%s]",
											PageDeviceType);

								// return different value, to announce impairment as the reason
								//  for denial of status change.
								//
								return ST_STAT_C_NG5;
								}
							else
								{
								PrintMessage(MSGTYPE_IVR,
											hwndD4xMsg, 
											"case 0: vendor NOT impaired. PageType [%s] [%s]",
											PageDeviceType,
											page[PAGE_COV_POS].PageType);

								}
							}
							// end patch point.

                            if(param.StPromptStat)
								{
								return ST_STAT_WRITE2;
								}
                            return ST_STAT_WRITE;
							}
                        else
							{
							// status does not represent a "pageable" staus 'Y' in nopgstat.
							PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "NG3/NG6 - case 1");
							//return ST_STAT_C_NG3;
							return ST_STAT_C_NG6;
							}
						}

                case -2: // multiple coverage, and multiple coverage IS permitted.

					i = CoverDoubleCheck(PagePointer);

                    if(i == TRUE)
						{
                        if(Status[page[PAGE_COV_POS].ACKMin].Page[0] == 'Y' ||
							Status[page[PAGE_COV_POS].ACKMin].Page[0] == 'C')
							{
							// the "ultimate" profile has been found.
							//
                            page[PagePointer].PageStatus[0] = 'S';
                            strcpy(page[PagePointer].FileName, page[PagePointer].AutCode);

							// patch point.

							// a final check before the coverage is written.
							//  see if there is "page vendor impairment", if so
							//  don't allow the coverage.  recycle with an informative message, 
							//  detailing the current situation.
							//  for now, just use an existing message state machine path 
							//  leading to recycle.
							//
							{
							CString PageDeviceType;
							PageDeviceType.Format("%%%s", page[PAGE_COV_POS].PageType);

							if(g_IMP.IsImpairedVendor(page[PagePointer].PageType))
								{
								PrintMessage(MSGTYPE_IVR,
											hwndD4xMsg, 
											"case -2: vendor IS impaired.");

								// return different value, to announce impairment as the reason
								//  for denial of status change.
								//
								return ST_STAT_C_NG5;
								}
							else
								{
								PrintMessage(MSGTYPE_IVR,
											hwndD4xMsg, 
											"case -2: vendor NOT impaired.");

								}
							}
							// end patch point.


                            if(param.StPromptStat) 
								{
								return ST_STAT_WRITE2;
								}
                            return ST_STAT_WRITE;
							}
                        else
							{
							// multiple coverage is allowed, but the target that we have found
							//  is not 'C' or 'Y', so this is not a possibility that we can
							//  allow to proceed.
							//
							PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "NG3/NG6 - case 2");
                            //return ST_STAT_C_NG3;
                            return ST_STAT_C_NG6;
							}
						}
                    return ST_STAT_C_NG1;

                case -6: // multiple coverage, but multiple coverage is NOT permitted
                    return ST_STAT_C_NG1;

                case -3: // illegal value for page status.
                case -4: // unable to find pagers table record for this profile.
                    return ST_STAT_C_NG2;

                case -5: // DefaultPager type is not present in the PageServer config.
 					PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "NG3/NG6 - case 3");
					return ST_STAT_C_NG3;

                default:
                    return ST_STAT_TYPES;
				}

            return ST_STAT_C_NG4;	
			break;					

        case ST_STAT_C_NG3:
            return ST_STAT_C_NG4;

        case ST_STAT_C_NG6:
            return ST_STAT_C_NG4;

		case ST_STAT_C_NG5:
			// after impairment announcement, 
			//  continue at... "request convering info" menu
        case ST_STAT_C_NG1:
        case ST_STAT_C_NG2:
        case ST_STAT_C_NG4:
            memset(page[PagePointer].FileName, 
					0, 
					sizeof(page[PagePointer].FileName));

            memset(page[PagePointer].InitiatorId, 
					0, 
					sizeof(page[PagePointer].InitiatorId));

            return ST_STAT_C_MSG;

        // check/change voice prompt message
        case ST_VCE_MSG:
            rv = GetFileAttributes(page[PagePointer].PageInfo);

            if(rv != 0xFFFFFFFF) 
				return ST_VCE_MSG2;

            return ST_VCE_MSG3;

        case ST_VCE_MSG2:
        case ST_VCE_MSG3:
            if(param.ConfirmChange)
				{
		            PrintMessage(MSGTYPE_IVR,
					hwndD4xMsg,
					"confirm change true in ST_VCE_MSG3 or MSG2"
					);

				return ST_VCE_MSG4;
				}




            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_VCE_MSG2: or ST_VCE_MSG3: PasswordFlag [%s], PrintedFlag [%c]",
							page[PagePointer].Password[0],
							page[PagePointer].Printed[0]
							);




			if(page[PagePointer].Password[0] == 0 || page[PagePointer].Printed[0] == 'P')
				{
                return ST_VCE_CHG_MSG;
				}
            else
				{
                return ST_VCE_PASS;
				}

        case ST_VCE_MSG4:
            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

            if(param.ConfirmChange) 
				return ST_VCE_NEXT;


            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_VCE_MSG4: PasswordFlag [%s]  PrintedFlag [%c]",
							page[PagePointer].Password[0],
							page[PagePointer].Printed[0]
							);

            if(page[PagePointer].Password[0] == 0 || page[PagePointer].Printed[0] == 'P')
				{
                return ST_VCE_CHG_MSG;
				}
            else
				{
                return ST_VCE_PASS;
				}

        case ST_VCE_NEXT:
			GetDigitBuffer(channel);
            if(port[channel].digsave[0] == 0 || port[channel].digsave[0] == '#')
				{
				// did not get the digits, re-prompt the user 

                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i No response while checking voice prompt.",
								channel );

                if(CheckMenu())
					{
                    if(param.StPromptStat)
                        return ST_MENU_STAT;
                    else
                        return ST_MENU_MSG;
					}
                page[PagePointer].PageStatus[0] = 'I';
				port[channel].timer.wHour = 70;
                return ST_ONHK;
				}

            if(port[channel].digsave[0] == '*')
				{
                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"ST_VCE_NEXT: PasswordFlag [%s]  PrintedFlag [%c]",
								page[PagePointer].Password[0],
								page[PagePointer].Printed[0]
								);

                if(page[PagePointer].Password[0] == 0 || page[PagePointer].Printed[0] == 'P')
					{
                    return ST_VCE_CHG_MSG;
					}
                else
					{
                    return ST_VCE_PASS;
					}
				}

            if(port[channel].digsave[0] == '0' && param.OpTransfer[0] != 0) 
				return ST_OP_TRANSFER;

            return ST_HELLO_MSG; 
			
        case ST_VCE_PASS:
            return ST_VCE_PSCHK;

        case ST_VCE_PSCHK:
			GetDigitBuffer( channel );
            PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
							"Line %i Password %s, Entered %s.",
							channel, page[ PagePointer ].Password,
							port[ channel ].digsave );

            if(strncmp( port[ channel ].digsave,
				page[ PagePointer ].Password,
				strlen( page[ PagePointer ].Password ) ) == 0 )
				{
                page[ PagePointer ].Printed[ 0 ] = 'P';
                return( ST_VCE_CHG_MSG );
				}
            return( ST_VCE_PSERR );

        case ST_VCE_CHG_MSG:
            return( ST_VCE_CHANGE );

        case ST_VCE_CHANGE:
            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

            i = Wait(&port[channel].timer, 99L);

            if(i == FALSE)
				{
				SetFileAttributes(page[PagePointer].PageInfo, FILE_ATTRIBUTE_NORMAL);
				if(remove(page[PagePointer].PageInfo) == 0)
					{
                    PrintMessage(MSGTYPE_IVR,
									hwndD4xMsg,
									"Line %i Voice prompt: %s removed.",
									channel,
									page[PagePointer].PageInfo);
                    WriteStatLog(PagePointer, 6);
					}
                else
					{
                    PrintMessage(MSGTYPE_IVR,
									hwndD4xMsg,
									"Line %i Unable to remove prompt file: %s.",
									channel,
									page[PagePointer].PageInfo);
					}
				}
            else
				{
				WriteStatLog(PagePointer, 2);
				}

            if(param.ConfirmChange)
				{
				return(ST_VCE_MSG);
				}

            if(CheckMenu())
				{
                dx_clrdigbuf(chdev);
                for(i = 0; i < 12; i++)
					{
                    if(param.StatPress[i][0] == 'V')
						{
                        return(ST_MENU_HELLO);
						}
					}
                return(ST_HELLO_MSG);
				}
            page[PagePointer].PageStatus[0] = 'I';
			port[channel].timer.wHour = 70;
            return(ST_ONHK);

        // This section is used to check/change user password.

        case ST_PASS_MSG:
            if( param.ConfirmChange ) return( ST_PASS_NEXT );
            if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' )
                return( ST_PASS_MSG2 );
            else
                return( ST_PASS_PASS );

        case ST_PASS_NEXT:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == 0 || port[ channel ].digsave[ 0 ] == '#' )
				{
				// did not get the digits, re-prompt the user 

                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i No response while checking password",
								channel );

                if( CheckMenu( ) )
					{
                    if( param.StPromptStat )
                        return( ST_MENU_STAT );
                    else
                        return( ST_MENU_MSG );
					}
                page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}
            if( port[channel].digsave[0] == '*' )
				{
                if( page[ PagePointer ].Password[ 0 ] == 0 || page[ PagePointer ].Printed[ 0 ] == 'P' )
                    return( ST_PASS_MSG2 );
                else
                    return( ST_PASS_PASS );
				}
            if( port[ channel ].digsave[ 0 ] == '0' && param.OpTransfer[ 0 ] != 0 ) 
				return( ST_OP_TRANSFER );

            if( CheckMenu( ) )
				{
                if( param.StPromptStat )
                    return( ST_MENU_STAT );
                else
                   return( ST_MENU_MSG );
				}

            page[ PagePointer ].PageStatus[ 0 ] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

        case ST_PASS_PASS:
            return( ST_PASS_PSCHK );

        case ST_PASS_PSCHK:
			GetDigitBuffer( channel );

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Password %s, Entered %s.",
							channel,
							page[ PagePointer ].Password,
							port[ channel ].digsave);

            if(strncmp( port[ channel ].digsave,
				page[ PagePointer ].Password,
				strlen( page[ PagePointer ].Password ) ) == 0 )
				{
                page[ PagePointer ].Printed[ 0 ] = 'P';
                return( ST_PASS_MSG2 );
				}
            return( ST_PASS_PSERR );

        case ST_PASS_MSG2:
            return( ST_PASS_GET );

        case ST_PASS_GET:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == 0 )
				{
                if( CheckMenu( ) )
					{
                    if( param.StPromptStat )
                        return( ST_MENU_STAT );
                    else
                        return( ST_MENU_MSG );
					}
                page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
                return( ST_ONHK );
				}

            if(port[ channel ].digsave[ 0 ] == '0' &&
				(port[ channel ].digsave[ 1 ] == 0 || port[ channel ].digsave[ 1 ] == '#' ) &&
				param.OpTransfer[ 0 ] != 0 )
				{
				return( ST_OP_TRANSFER );
				}
            memset(string2, 0, sizeof(string2));
            strcpy( string2, port[ channel ].digsave );
            memset( page[ PagePointer ].Password, 0, sizeof( page[ PagePointer ].Password ) );

            for(i = 0; i < 10; i++)
				{
                if( string2[ i ] == '#' )
					break;
                page[ PagePointer ].Password[ i ] = string2[ i ];
				}
            return( ST_PASS_WRITE );

        case ST_PASS_WRITE:
            if( param.ConfirmChange ) return( ST_PASS_NEXT );
            if( CheckMenu( ) ){
                if( param.StPromptStat )
                    return( ST_MENU_STAT );
                else
                    return( ST_MENU_MSG );
            }
            page[ PagePointer ].PageStatus[ 0 ] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

		case ST_ADD_ALL_DVCS_Q:            
                return ST_IDLE;				

        case ST_ADD_PAGE:
            if(page[PagePointer].PageStatus[0] == 'C')
				{
                page[PagePointer].PageStatus[0] = 'I';
                return ST_IDLE;
				}
            else
				{
				port[channel].timer.wHour = 70;
                return ST_ADD_PAGE;
				}

        case ST_ANOTHER_GET:
        case ST_ANOTHER:
			GetDigitBuffer( channel );
            page[ PagePointer ].PageStatus[0] = 'B';
            if( port[ channel ].digsave[ 0 ] == param.AnotherPage[ 0 ] )
				{
				ResetBuffer(channel);
				return( ST_HELLO_MSG );
				}

            if( port[ channel ].digsave[ 0 ] == '0' )
				return( ST_OP_TRANSFER  );

            page[ PagePointer ].PageStatus[0] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

        case ST_ONHK:
            port[ channel ].timer.wHour = 70;
            return( ST_IDLE );

        case ST_ONHK_DELAY:
            if( port[ channel ].timer.wHour == 70 ) 
				{
				GetLocalTime( &port[ channel ].timer );
				}

            i = Wait( &port[ channel ].timer, 200L );

			if( i == FALSE ){
                return( ST_ONHK_DELAY );
			}
			else{
                port[ channel ].timer.wHour = 70;
                port[ channel ].PortStatus = 0;
				return( ST_IDLE );
			}

        case ST_CCPG_CALL:
            switch( param.PageType[ page[ PagePointer ].ScriptNumber ].Script[ page[ PagePointer ].ScriptFlag ]  )
				{
                case 'C':
                case '$':
					rvl = ATDX_CPTERM( chdev );
                    switch( rvl )
						{
                        case CR_BUSY:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i Busy", channel);
                            return( ST_CCPG_ERR );
                        case CR_NOANS:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i No Answer", channel);
                            return( ST_CCPG_ERR );
                        case CR_NORB:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i No Ringback", channel);
                            return( ST_CCPG_ERR );
                        case CR_CEPT:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i Intercept", channel);
                            return( ST_CCPG_CALL );
                        case CR_CNCT:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i Connected", channel);
                            if( param.PageType[ page[ PagePointer ].ScriptNumber ].nbrbeg > 1 )
                                return( ST_CCPG_ERR );
                            else
                                return( ST_CCPG_CALL );
                        default:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i ERROR Invalid callstat type %i.", channel, rvl );
                            return( ST_CCPG_ERR );
						}
                case '@':
					rvl = ATDX_CPTERM( chdev );
                    switch( rvl )
						{
                        case CR_BUSY:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i Busy", channel );
                            return( ST_CCPG_ERR );
                        default:
                            PrintMessage(MSGTYPE_IVR,  hwndD4xMsg, "Line %i No Busy", channel );
                            return( ST_CCPG_CALL );
						}
                default:
                    break;
				}
			return( ST_CCPG_CALL );

        case ST_CCPG_ERR:
            port[ channel ].Errors++;
            SetQueueError( -1, PagePointer, 1 );
            return( ST_ONHK_DELAY );

        case ST_VPAGE_TALK:
            pstate[ port[ channel ].ComNumber ] = PHANGUP;
            return( ST_VPAGE_DONE );

        case ST_VPAGE_DONE:
            return( ST_IDLE );

        case ST_NAME_MSG:
            return( ST_NAME_GET );

        case ST_NAME_GET:
			GetDigitBuffer( channel );
            if( port[channel].digsave[0] == '#' || port[channel].digsave[0] == 0 )
				{
                if( param.TimeOutTransfer && param.OpTransfer[ 0 ] != 0 && port[ channel ].digsave[ 0 ] == 0 )
					return( ST_OP_TRANSFER );

                if( param.timeoutRetry )
					return( ST_HELLO_MSG );

                page[ PagePointer ].PageStatus[ 0 ] = 'I';
                port[ channel ].Errors++;
				port[ channel ].timer.wHour = 70;
                return ( ST_ONHK );
				}
            if(port[ channel ].digsave[ 0 ] == '0' &&
				(port[ channel ].digsave[ 1 ] == 0 || port[ channel ].digsave[ 1 ] == '#') &&
				param.OpTransfer[ 0 ] != 0 )
				{
				return( ST_OP_TRANSFER );
				}
            strcpy( string2, port[ channel ].digsave );
			dx_clrdigbuf( chdev );

			/* validate the digits for range 0-9 */
            memset( page[ PagePointer ].Extension, 0, sizeof( page[ PagePointer ].Extension ) );
            memset( page[ PagePointer ].PagerId, 0, sizeof( page[ PagePointer ].PagerId ) );

            for(i = 0; i < 10; i++)
				{
                if( string2[ i ] == '#' ) break;
                page[ PagePointer ].Extension[ i ] = string2[ i ];
				}

            i = NameId( page[ PagePointer ].Extension, PagePointer );

            switch( i )
				{
                case 1:
					// 1 name found
                    j = 0;
                    for(i = 0; i < 10; i++)
						{
                        if( page[ PagePointer ].tempStr2[ i ] != ' ' )
							{
                            page[ PagePointer ].PagerId[ j ] = page[ PagePointer ].tempStr2[ i ];
                            j++;
							}
						}
                    return( ST_NAME_MSG2 );

				default:
					// multiple names found.
					//  or
					// 0 names found.
					if( i > 0 && i <= param.nameMax )
						{
						page[ PagePointer ].ACKMin = i;
						page[ PagePointer ].NextAckPtr = 0;
						return( ST_NAME_SELECT );
						}
					else
						{
						PrintMessage(MSGTYPE_IVR,
										hwndD4xMsg,
										"Line %i Name id %s not found",
										channel,
										page[ PagePointer ].PagerId);

						port[ channel ].Errors++;
						if( param.OpTransfer[ 0 ] != 0 ) return( ST_OP_TRANSFER );
						return( ST_NAME_MSG3 );
						}
					break;
				}

        case ST_NAME_SELECT:
            if( evtcode == TDX_CST && cstp->cst_event == DE_DIGITS )
				return( ST_NAME_CHOOSE );

            if( page[ PagePointer ].NextAckPtr >= (unsigned)page[ PagePointer ].ACKMin )
				return( ST_NAME_CHOOSE );

            return( ST_NAME_SELECT2 );

        case ST_NAME_SELECT2:
            if( evtcode == TDX_CST && cstp->cst_event == DE_DIGITS )
				return( ST_NAME_CHOOSE );

            page[ PagePointer ].NextAckPtr++;
            return( ST_NAME_SELECT );

        case ST_NAME_CHOOSE:
			GetDigitBuffer( channel );

            if( port[ channel ].digsave[ 0 ] == '*' )
				{
                page[ PagePointer ].NextAckPtr = 0;
                return( ST_NAME_SELECT );
				}

            if( port[ channel ].digsave[ 0 ] > '0' &&
				port[ channel ].digsave[ 0 ] <= (char)(page[ PagePointer ].ACKMin + 48) )
				{
                j = 0;
                k = ( port[ channel ].digsave[ 0 ] - 49 ) * 10;
                for(i = k; i < (k + 10); i++)
					{
                    if( page[ PagePointer ].tempStr2[ i ] != ' ' )
						{
                        page[ PagePointer ].PagerId[ j ] = page[ PagePointer ].tempStr2[ i ];
                        j++;
						}
                    if( k>0 )
                        page[ PagePointer ].tempStr2[ i-k ] = page[ PagePointer ].tempStr2[ i ];
					}
                return( ST_NAME_MSG2 );
				}

            if( port[ channel ].digsave[ 0 ] == '0' )
				return( ST_OP_TRANSFER  );

            if( port[ channel ].digsave[ 0 ] == 0 )
				{
                page[ PagePointer ].PageStatus[ 0 ] = 'I';
				port[ channel ].timer.wHour = 70;
                return ( ST_ONHK );
				}
            page[ PagePointer ].NextAckPtr = 0;
            return( ST_NAME_SELECT );

        case ST_NAME_MSG3:
            return( ST_NAME_MSG );

		case ST_STAT_M_FORWARD:
			return ST_GET_EMAIL_EXT;

		case ST_STAT_M_EMAIL:
			return ST_GET_FORWARD_EXT;

		case ST_GET_EMAIL_EXT:
			GetDigitBuffer( channel );
			ForwardEmail(port[channel].digsave, 
							page[PagePointer].iCurrentMessageNumber);
			return ST_STAT_M_MSG3;
			
		case ST_GET_FORWARD_EXT:
			GetDigitBuffer( channel );
			ForwardPage(port[channel].digsave, 
						page[PagePointer].iCurrentMessageNumber, 
						page[PagePointer].Extension);
			return ST_STAT_M_MSG3;

		//01/14/2016 - DE20568 - SOguz - Define new state for SMS(Spok Mobile) pager types
		//Sets the page status and calls the onhook state
		case ST_SMS_FS_MSG:
			page[ PagePointer ].PageStatus[ 0 ] = 'I';
			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );

		}
	return(port[channel].state);
	}


/****************************************************************
*        NAME : Exit_State( channel )
* DESCRIPTION : Reintialize array pointers before hanging up.
*             : chanel = channel number
*      OUTPUT : none
*     RETURNS : none
****************************************************************/
	int
Exit_State(int channel, int evtcode, int chdev, DX_CST *cstp)
	{
	int i, fc_status;
    int PagePointer;

	if(d4xDiags == TRUE)
		{
		PrintMessage(MSGTYPE_IVR,
						hwndD4xMsg,
						"Exit_State(): Line[%i] evtcode[0x%0x] State[%s]",
						channel,
						evtcode,
						DialogicStateName[port[channel].state]);
		}

    PagePointer = channel;

    switch(port[channel].state)
		{
        case ST_ADD_PAGE:
			if(page[PagePointer].pageAdded == TRUE)
				{
				page[PagePointer].PageStatus[0] = 'I';

				if(port[channel].file_handle > 0)
					{
					dx_fileclose(port[channel].file_handle);
					port[channel].file_handle = 0;
					}

				port[channel].timer.wHour = 70;
				return(ST_ONHK);
				}
            return(port[channel].state);

        case ST_IDLE:
        case ST_CCPG_CALL:
        case ST_CATS_PLAY:
        case ST_CATS_CALL:
        case ST_CATS_ONHK:
        case ST_VP_PAGE:
        case ST_ONHK_DELAY:
            return( port[ channel ].state );

        case ST_GOODBYE_MSG:
        case ST_GOODBYE3_MSG:
        case ST_GOODBYE_ID:
        case ST_GOODBYE_PROMPT:
        case ST_GET_VOICE:
            page[ PagePointer ].PageStatus[ 0 ] = 'I';

            if( port[ channel ].file_handle > 0 )
				{
                fc_status = dx_fileclose( port[ channel ].file_handle );
				port[ channel ].file_handle = 0;

 				PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Exit_State() state [%d] close voice record channel[%d] close status[%d]",
								port[ channel ].state,
								channel,
								fc_status);
				}
			port[ channel ].timer.wHour = 70;

			// 6/24/2016 - DE20747 - SOguz - we received the call back number
			// Move to next state and add each devices to queue
			if (isPageAllDevicesOn) 
			{ 
					return ST_ADD_ALL_DVCS_Q; 
			}

			PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"exit_state() - returning state == %d",
							ST_ADD_PAGE);

            return ST_ADD_PAGE;

        case ST_GET_DIS:
        case ST_LST_REC_OK:
        case ST_LST_RECORD:
        case ST_LST_GET_DIS:
            return( curr_state( channel, evtcode, chdev, cstp ) );

        case ST_VPAGE_TALK:
            pstate[ port[ channel ].ComNumber ] = PHANGUP;
            return( ST_VPAGE_DONE );

        case ST_GET_EXT:
			PrintMessage(MSGTYPE_IVR,
							hwndGeneralMsg,
							"Exit_State(): ST_GET_EXT");

            page[ PagePointer ].PageStatus[ 0 ] = 'I';
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

             if((param.IVRPort[channel].iType == 21 ||
				  param.IVRPort[channel].iType == 27 ) &&
				  port[ channel ].t1 == FALSE )
				{
				return( ST_IDLE );
				}

			port[ channel ].timer.wHour = 70;
            return( ST_ONHK );		
		// Exit_State();
        case ST_CATS_REC_ON:
        case ST_CATS_GET_DT:
			if(FALSE == param.CatsConfirm)
				{
				// a loop current loss, or other error on
				//  a cats call with CatsConfirm == FALSE
				//  ... no confirmation required, a hang-up means
				//  go forward with the cats call.

				// don't automatically exit on loop current loss.
				CatsFinalize(channel);

				PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "returning ST_CATS_REC_OK");

				return ST_CATS_REC_OK;
				}
			// intentional fall thru to next group of cases,
			//  when CatsConfirm is TRUE, the call is to be abandoned.

        case ST_CATS_MSG:
        case ST_CATS_REC_OF:
        case ST_CATS_REC_OK:
        case ST_CATS_REC_NG:
        case ST_CATS_ABORT:
			//PrintMessage(hwndD4xMsg, "Exit_State(): ST_CATS_XXXX - [%d]", port[channel].state);
            for(i = 0; i < 4; i++)
				{
                Cats[i].GroupCallWaiting = 0;
                if(param.Cats[i].Line > 0)
					{
                    port[param.Cats[i].Line].timer.wHour = 70;
                    port[param.Cats[i].Line].state = ST_IDLE;
                    UpdatePhoneLineStatusWindow(param.Cats[i].Line, "Idle");
					}
				}
            CatsCall = FALSE;
			// fall into the default case...

 			// catch all - hangup the phone...
		default:
            page[PagePointer].PageStatus[0] = 'I';

            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

			port[channel].timer.wHour = 70;
            return  ST_ONHK;
		}
	}


/****************************************************************
*        NAME : next_state(channel)
* DESCRIPTION : begin the next state, initiate multi-tasking function
*       INPUT : channel = channel number
*      OUTPUT : none
*     RETURNS : pass/fail status
****************************************************************/

	int
next_state(int channel, int chdev)
	{
	static DX_CAP ChannelParameter;
	SYSTEMTIME CurrTime;
	int errcode = 0;
	int i;
    int j;
    int k;
    int PagePointer;
    unsigned rv = 0;
	long termType;
    char Messages[255];
    char string[80];
    char fname2[80];
    char *substr;
	char string2[255];

    PagePointer = port[channel].PageNumber;

	if(d4xDiags == TRUE)
		{
		PrintMessage(MSGTYPE_IVR,
						hwndD4xMsg,
						"next_state(): Line[%i] Next State[%s]",
						channel,
						DialogicStateName[port[channel].state]);
		}

    switch( port[ channel ].state )
		{
        case ST_IDLE:
			/* channel idle, waiting for incoming call */
			dx_clrdigbuf( chdev );
            port[ channel ].PortStatus = 0;
            memset(string2, 0 , sizeof(string2));

            //if(port[channel].PortType == 3)
            if(param.IVRPort[channel].iType == 3)
				{
                sprintf(string2, "Busied");
				}
            else
				{
                sprintf(string2, "Idle");
				}

			UpdatePhoneLineStatusWindow(channel, string2);
			break;

        case ST_OFFHK:
			// call received, go off hook

			/*
			// removed kgill - 2001-07-24.
			if( port[ channel ].t1 == FALSE )
				{
				// why should the digit buffer not be cleared,
				//  if it is a t1 line.
				dx_clrdigbuf( chdev );
				}
			*/

			errcode = XnSetHook( channel, DX_OFFHOOK );
			UpdatePhoneLineStatusWindow(channel, "Incoming call");
            port[ channel ].PortStatus = 1;
			break;

        case ST_VP_PAGE:
            break;

        case ST_CATS_MSG:
            UpdatePhoneLineStatusWindow( channel, "CATS Call" );
            GetLocalTime( &CurrTime );
            memset(string, 0, sizeof(string));
            memset(fname2, 0, sizeof(fname2));
            i = 0;
            do{
                i++;
                memset(fname2, 0, sizeof(fname2));
                sprintf(fname2,
						"%s%02d%02d%04d.CAT",
						pageFileDir,
						CurrTime.wMonth,
						CurrTime.wDay,
						i);
                rv = GetFileAttributes( fname2 );
            }while( rv != 0xFFFFFFFF && i < 999 );

            page[ PagePointer ].VoiceFile = i;
			port[ channel ].file_handle = dx_fileopen(fname2,
														O_CREAT | O_BINARY | O_TRUNC | O_RDWR,
														S_IREAD | S_IWRITE);

            memset( Cats[ port[ channel ].DataPointer ].FileName,
					0,
					sizeof( Cats[ port[ channel ].DataPointer ].FileName ) );
            strcpy( Cats[ port[ channel ].DataPointer ].FileName, fname2 );

            memset( page[ PagePointer ].DisplayInfo, 0, sizeof( page[PagePointer].DisplayInfo ) );
            memset( page[ PagePointer ].PageInfo, 0, sizeof( page[PagePointer].PageInfo ) );
            memset( page[ PagePointer ].Extension, 0, sizeof( page[ PagePointer ].Extension ) );

            strncpy( page[ PagePointer ].PageInfo,
					param.Cats[ port[ channel ].DataPointer ].PageNumber,
					sizeof( page[PagePointer].PageInfo ) );

            strncpy( page[ PagePointer ].Extension, "CATS PG", 7 );

            sprintf( page[ PagePointer ].InitiatedDate,
					"%02i/%02i/%04i",
					CurrTime.wMonth,
					CurrTime.wDay,
					CurrTime.wYear );

            sprintf( page[ PagePointer ].InitiatedTime,
					"%02i:%02i:%02i",
					CurrTime.wHour,
					CurrTime.wMinute,
					CurrTime.wSecond );

            sprintf( page[ PagePointer ].InitiatorId, "%10s", fname2 );

            page[ PagePointer ].PageStatus[ 0 ] = 'R';

            memset(page[PagePointer].CompletedDate, ' ', sizeof(page[PagePointer].CompletedDate));
            memset(page[PagePointer].CompletedTime, ' ', sizeof(page[PagePointer].CompletedTime));

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i CATS Recording voice file: %s, File Handle: %i.",
							channel,
							Cats[ port[ channel ].DataPointer ].FileName,
							port[ channel ].file_handle );

			dx_clrdigbuf( chdev );
            Messages[ 0 ] = (char) param.Cats[ port[ channel ].DataPointer ].VoicePrompt;
            port[ channel ].PortStatus = 1;
            sprintf( string2, "Cats call line: %i.", channel );
            WriteLog( "XTPGCATS.MSG", string2 );

			// note vb41_play() ...................
            errcode = vb41_play( promptfh, channel, prompts, 1, Messages );

            for(i = 0; i < 4; i++)
				{
                if(param.Cats[i].Line > 0 && param.Cats[i].Line != channel)
					{
                    UpdatePhoneLineStatusWindow(param.Cats[i].Line, "CATS Active");
					}
				}
            break;

        case ST_CATS_REC_ON:
            /* begin the record operation */
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "next_state() at: ST_CATS_REC_ON");
			dx_clrdigbuf( chdev );
            UpdatePhoneLineStatusWindow( channel, "CATS Record" );
            GetLocalTime( &Cats[ port[ channel ].DataPointer ].timer );
            errcode = record(channel,
								port[channel].file_handle,
								param.voicePageTime - 1);
            break;

        case ST_CATS_REC_OF:
			/* go on hook and terminate the call */
            UpdatePhoneLineStatusWindow(channel, "Idle");

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i CATS going on hook",
							channel);

            port[ channel ].timer.wHour = 70;
			errcode = XnSetHook( channel, DX_ONHOOK );
            port[ channel ].PortStatus = 0;

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}
            break;

        case ST_CATS_REC_NG:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "next_state() at: ST_CATS_REC_NG");
            WriteLog("XTPGCATS.MSG", "Cat call challenged.\r\n");
			dx_clrdigbuf( chdev );
            Messages[ 0 ] = 49;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        case ST_CATS_REC_OK:
        case ST_LST_REC_OK:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "next_state() at: ST_CATS_REC_OK");
            WriteLog( "XTPGCATS.MSG", "Code Recorded." );
			dx_clrdigbuf( chdev );
            Messages[ 0 ] = 50;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        case ST_CATS_ABORT:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "next_state() at: ST_CATS_ABORT");
            for( i=0; i<4; i++ ){
                Cats[ i ].GroupCallWaiting = 0;
                if( param.Cats[ i ].Line > 0 ){
                    port[ param.Cats[ i ].Line ].timer.wHour = 70;
                    port[ param.Cats[ i ].Line ].state = ST_IDLE;
                    UpdatePhoneLineStatusWindow(param.Cats[i].Line, "Idle");
                }
            }

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            port[ channel ].timer.wHour = 70;
            port[ channel ].state = ST_ONHK_DELAY;
            CatsCall = FALSE;
			errcode = XnSetHook( channel, DX_ONHOOK );
            break;

        case ST_CATS_V_ON:
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "next_state() at: ST_CATS_V_ON");
			// Initialize CATS group call
            i = port[ channel ].DataPointer;
            Cats[ i ].timer.wHour = 70;
            Cats[ i ].StartTime.wHour = 70;
            Cats[ i ].GroupCallWaiting = 0;
            Cats[ i ].Group1On = 0;
            Cats[ i ].Group2On = 0;
            if( param.Cats[ i ].Group1Port > 0 ){
                memset( pcapcode[ param.Cats[ i ].Group1Port-1 ], 0, sizeof(pcapcode[0]));
                memset( pmessage[ param.Cats[ i ].Group1Port-1 ], 0, sizeof(pmessage[0]));
                for( j=0; j<16; j++ ){
                    if( param.Phone.Release[ j ] != -1 )
                        pmessage[ param.Cats[ i ].Group1Port-1 ][ j ] = (char) param.Phone.Release[ j ];
                    else break;
                }
                k = param.Cats[ i ].Group1Button;
                for( j=0; j<16; j++ ){
                    if( param.Phone.Button[ k ][ j ] != -1 )
                        pcapcode[ param.Cats[ i ].Group1Port-1 ][ j ] = (char) param.Phone.Button[ k ][ j ];
                    else break;
                }
                Cats[ i ].GroupCallWaiting++;
                PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg,
								"Line %i CAT Call 1: Type %i, COM%i:, Button %i",
								channel,
								i,
								param.Cats[i].Group1Port + 1,
								k );
            }
            if( param.Cats[ i ].Group2Port > 0 ){
                memset( pcapcode[ param.Cats[ i ].Group2Port-1 ], 0, sizeof(pcapcode[0]));
                memset( pmessage[ param.Cats[ i ].Group2Port-1 ], 0, sizeof(pmessage[0]));
                for( j=0; j<16; j++ ){
                    if( param.Phone.Release[ j ] != -1 )
                        pmessage[ param.Cats[ i ].Group2Port-1 ][ j ] = (char) param.Phone.Release[ j ];
                    else break;
                }
                k = param.Cats[ i ].Group2Button;
                for( j=0; j<16; j++ ){
                    if( param.Phone.Button[ k ][ j ] != -1 )
                        pcapcode[ param.Cats[ i ].Group2Port-1 ][ j ] = (char) param.Phone.Button[ k ][ j ];
                    else break;
                }
                Cats[ i ].GroupCallWaiting++;

                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i CAT Call 2: Type %i, COM%i:, Button %i",
								channel,
								i,
								param.Cats[i].Group2Port + 1,
								k );
            }
            UpdatePhoneLineStatusWindow(channel, "CATS Active *");

			// Add page to voice que
            j = TRUE;

            memset( string, 0, sizeof( string ) );

            if(strncmp(param.Cats[i].PageQue, "XN", 2) == 0)
				{
                memset( page[ PagePointer ].Extension, 0 ,sizeof( page[ PagePointer ].Extension ) );
                memset( page[ PagePointer ].PagerId, 0 ,sizeof( page[ PagePointer ].PagerId ) );
                memset( page[ PagePointer ].DisplayInfo, 0 ,sizeof( page[ PagePointer ].DisplayInfo ) );

                strncpy(page[ PagePointer ].Extension,
						param.Cats[ i ].PageNumber,
						sizeof( page[ PagePointer ].Extension ) );

                Set_Null(page[ PagePointer ].Extension,
							sizeof( page[ PagePointer ].Extension ) );

				GetPagerId( page[ PagePointer ].Extension, PagePointer, BY_PROFILEID);

                if( LogPager( page[ PagePointer ].Extension, PagePointer ) == FALSE )
					{
                    PrintMessage(MSGTYPE_IVR,
									hwndD4xMsg,
									"Line %i Pager not assigned in log file",
									channel );
                    j = FALSE;
					}
                else
					{
                    memset( string, 0, sizeof( string ) );
                    sprintf( string, "%%%s", page[ PagePointer ].PageType );

                    if( Get_Frq( PagePointer, string, string ) == TRUE )
						{
                        AddSetUp( PagePointer, "CATS PG" );
						}
                    else
						{
                        PrintMessage(MSGTYPE_IVR,
										hwndD4xMsg,
										"Line %i No pager assigned. Check frq",
										channel );
                        j = FALSE;
						}
					}
				}
            else
				{
                strcpy( string, "%" );
                strncat( string, param.Cats[ i ].PageQue, 2 );
				}

            GetPageType( PagePointer, string,  __FILE__, __LINE__ );
            page[ PagePointer ].PortNumber = GetQuePointer( PagePointer );

            if(((page[PagePointer].PortNumber >= 0) && (j == TRUE)) || IsSmsType(PagePointer))
				{
                if(AddQueueEx(PagePointer, page[PagePointer].PortNumber))
					{
					page[PagePointer].pageAdded = TRUE;
                    UpdatePhoneLineStatusWindow(channel, "Hanging Up");
                    port[channel].timer.wHour = 70;
                    page[PagePointer].PortNumber = -1;
					errcode = XnSetHook(channel, DX_ONHOOK);
					}
				}
            else
				{
                UpdatePhoneLineStatusWindow( channel, "Que add aborted" );
                port[ channel ].timer.wHour = 70;
                page[ PagePointer ].PortNumber = -1;
				errcode = XnSetHook( channel, DX_ONHOOK );
				}
            CheckCatsState( );

            break;

        case ST_CATS_CALL:
            UpdatePhoneLineStatusWindow( channel, "CATS Group Call" );
            break;

        case ST_CATS_PLAY:
			dx_clrdigbuf( chdev );
            UpdatePhoneLineStatusWindow( channel, "CATS Group Play" );

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            errcode = PlayFil(channel,
								Cats[ port[ channel ].DataPointer ].FileName,
								&port[ channel ].file_handle,
								0 );

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_CATS_PLAY - Line %i CATS message %s, type %i, fh %i, ret %i.",
							channel,
							Cats[ port[ channel ].DataPointer ].FileName,
							port[ channel ].DataPointer,
							port[ channel ].file_handle,
							errcode );
            break;

        case ST_CATS_ONHK:
            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

			j = port[ channel ].DataPointer;
            if( param.Cats[ j ].Group1Port > 0 && param.ComPort[ param.Cats[ j ].Group1Port-1 ].VoiceLine > 0 )
				{
				if( param.ComPort[ param.Cats[ j ].Group1Port-1 ].VoiceLine - 1 == channel )
					{
					pstate[ param.Cats[ j ].Group1Port-1 ] = PHANGUP;
					ComPort[ param.Cats[ j ].Group1Port-1 ].timer.wHour = 70;
					}
				}
            if( param.Cats[ j ].Group2Port > 0 && param.ComPort[ param.Cats[ j ].Group2Port-1 ].VoiceLine > 0 )
				{
				if( param.ComPort[ param.Cats[ j ].Group2Port-1 ].VoiceLine - 1 == channel )
					{
					pstate[ param.Cats[ j ].Group2Port-1 ] = PHANGUP;
					ComPort[ param.Cats[ j ].Group2Port-1 ].timer.wHour = 70;
					}
				}
            UpdatePhoneLineStatusWindow( channel, "CATS On Hook" );
            port[ channel ].timer.wHour = 70;
			errcode = XnSetHook( channel, DX_ONHOOK );
            break;

		//////	xxxxx

        case ST_HELLO_MSG:
			// play the greeting message, ask for 4 digits
            if(param.GreetingExt[0] == 0)
				{
				dx_clrdigbuf(chdev);
				}

			j = 0;

			for(i = 0; i < param.numIntroSilence; i++)
				{
				Messages[i] = VOX_PROMPT_silence_for_half_a_second;	//	1/2 Sec Silence
				j++;
				}

            Messages[j] = VOX_PROMPT_please_enter;					//	Please Enter
			j++;

			// variable param.PromptType may have values:
			//  Id Number == 19
			//  Extension == 48
			//  Pager Id == 12
			//
            Messages[j] = (char) param.PromptType;
			j++;

			i = FALSE;

           if(param.IVRPort[channel].iType != 17 &&
			   page[PagePointer].tempInt == 0)
				{
				if(param.IVRPort[channel].iType == 20 )
					{
					Messages[ j ] = VOX_PROMPT_to_connect_to;		//	to connect to
	                j++;
					}
				else
					{
					Messages[ j ] = VOX_PROMPT_to_page;				//	To Page
	                j++;

					i = TRUE;
					}
				}
            else
				{
                Messages[ j - 2 ] = VOX_PROMPT_please_dial_your;	//	Please Dial Your
				}

            if( param.IdLength > 8 )
				{
				Messages[ j ] = VOX_PROMPT_followed_by_pound_sign;	//	Followed by pound sign
				j++;
				}	

			if( param.promptNamePass && i )
				{
				Messages[ j ] = VOX_PROMPT_for_page_by_name_;		// for page by name enter
				j++;
				for( i=0; i<(int)strlen( param.PNamePass ); i++ )
					{
					// check 0 thru 9 range
					if( param.PNamePass[ i ] > 47 && param.PNamePass[ i ] < 58 )
						{
						Messages[ j ] = (char )( param.PNamePass[ i ] - 48 );
						j++;
						}

					if( param.PNamePass[ i ] == '*' )
						{
						Messages[ j ] = 10;
						j++;
						}

					if( param.PNamePass[ i ] == '#' )
						{
						Messages[ j ] = 11							;
						j++;
						}
					}
				}
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, j, Messages );
			break;

        // Use for states which require one touch-tone input.
        case ST_CATS_GET_DT:
        case ST_VCE_NEXT:
        case ST_STAT_CHANGE:
        case ST_STAT_CHANGE2:
        case ST_STAT_NEXT:
        case ST_STAT_M_GET:
        case ST_STAT_M_GET3:
		case ST_STAT_EX_GET3:
        case ST_MENU_CHK:
        case ST_PASS_NEXT:

        case ST_PARK_GET:
        case ST_ANOTHER_GET:
        case ST_NAME_CHOOSE:
            UpdatePhoneLineStatusWindow(channel, "Select Item");
            port[channel].PortStatus = 1;
            errcode = get_digits(channel, 1);
            break;

        // Use for states which require ten touch-tone input.
        // Usually password checks.
        case ST_MENU_PSCHK:
        case ST_STAT_PSCHK:
        case ST_VCE_PSCHK:
        case ST_STAT_M_PSCHK:
        case ST_PASS_PSCHK:
        case ST_PASS_GET:
            UpdatePhoneLineStatusWindow( channel, "10 DTMFs" );
            port[ channel ].PortStatus = 1;
            errcode = get_digits( channel, 10 );
            break;

        // Use for states which require five touch-tone input.
        case ST_STAT_EX_GET1:
        case ST_STAT_EX_GET2:
            UpdatePhoneLineStatusWindow( channel, "5 DTMFs" );
            port[ channel ].PortStatus = 1;
            errcode = get_digits( channel, 5 );
            break;

        // Play enter password prompt.
        case ST_MENU_PASS:
        case ST_STAT_PASS:
        case ST_VCE_PASS:
        case ST_STAT_M_PASS:
        case ST_PASS_PASS:
            UpdatePhoneLineStatusWindow( channel, "Enter Password" );
            Messages[0] = 29;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        // Play incorrect password prompt.
        case ST_MENU_PSERR:
        case ST_STAT_PSERR:
        case ST_PASS_PSERR:
        case ST_VCE_PSERR:
        case ST_STAT_M_PSERR:
            page[ PagePointer ].attempts++;
            UpdatePhoneLineStatusWindow( channel, "Invalid Password" );
			dx_clrdigbuf( chdev );
            Messages[0] = 30;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        // This section is the end user main menu.
        case ST_STAT_WRITE2:
            UpdatePhoneLineStatusWindow( channel, "Changed Status" );
            Change_Log2( PagePointer );
            if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == '#' )
				{
                memset( page[ PagePointer ].PageInfo, 0, sizeof( page[ PagePointer ].PageInfo ));
                strcpy( page[ PagePointer ].PageInfo, page[ PagePointer ].DisplayInfo);
				}

        case ST_MENU_STAT:
			Messages[0] = VOX_PROMPT_current;
            Messages[1] = VOX_PROMPT_page_status;
            Messages[2] = VOX_PROMPT_is;		// fixed for MASCO, kgill-translation-error was 
												//  incorrectly saying "greeting" instead of "is"
            memset(string2, 0, sizeof(string2));
            strcpy(string2, page[PagePointer].InitiatorId);
            i = 0;

            if(param.CoverPage && string2[0] != 0)
				{
                Messages[3] = VOX_PROMPT_covered_by;

                for(i = 0; i < (int) strlen(string2) && string2[i] != 0; i++)
					{
                    if(string2[i] >= '0' && string2[i] <= '9')
						{
                        Messages[i + 4] = (char) (string2[i] - 48);
						}
                    else
						{
						break;
						}
					}

                errcode = vb40_play(promptfh, channel, prompts, i + 4, Messages);
				}
            else
				{
                Messages[0] = VOX_STATUS_current_status_is;

				// second message is page status.
				if(param.altMenuPrompt[page[PagePointer].ACKMin] > 0 )
					{
					Messages[1] = (char) param.altMenuPrompt[page[PagePointer].ACKMin];
					}
				else
					{
					Messages[1] = (char) (Status[page[PagePointer].ACKMin].Prompt - 1);
					}

                if(Messages[1] < 0)
					{
					Messages[1] = VOX_STATUS_unavailable;
					}

                i = 0;

                switch(Status[page[PagePointer].ACKMin].Page[0])
					{
                    case 'F':
                    case 'O':
                    case '#':
                        memset(string2, 0, sizeof(string2));
                        k = 0;

						for(j = 0; j < MAX_DIGITS_IN_REFERRED_TO_NUMBER; j++)
							{
                            if(page[PagePointer].PageInfo[j] >= '0' && page[PagePointer].PageInfo[j] <= '9')
								{
                                string2[k] = (char) (page[PagePointer].PageInfo[j]);
                                k++;
								}
                            else if(param.StarPrompt > 1 && page[PagePointer].PageInfo[j] == '*')
								{
                                string2[k] = (char) VOX_STATUS_star; 
								k++;
								}
                            else if(page[PagePointer].PageInfo[j] != '-')
								{
                                break;
								}
							}

                        for(i = 0; i < (int) strlen(string2) && string2[i] != 0; i++)
							{
                            if(string2[i] >= '0' && string2[i] <= '9')
								{
                                Messages[i + 2] = (char) (string2[i] - 48);
								}
                            else
								{
                                Messages[i + 2] = (char) (string2[i]);
								}
							}

                        PrintMessage(MSGTYPE_IVR,
										hwndD4xMsg,
										"Line [%i] Status forwarded to [%s].",
										channel,
										page[PagePointer].PageInfo);
                        break;

                    default:
                        break;
					}

                port[channel].PortStatus = 1;
                errcode = vb40_play(StatusFh, channel, StatusPrompts, 2 + i, Messages);
				}
			break;

		case ST_MENU_STAT_2:
		case ST_EXCP_TIME:
            memset(Messages, 0, sizeof(Messages));

            i = ConvertVoiceTime2(page[PagePointer].PageInfo, Messages);

			if(port[channel].state == ST_EXCP_TIME)
				{
				Messages[i] = VOX_DIGITS_thank_you_goodbye;
				i++;
				}
            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Exception Status %s.",
							channel,
							page[PagePointer].PageInfo);

			errcode = vb40_play(Digitsfh, channel, DigitPrompts, i, Messages);
			break;


        case ST_MENU_MSG:
            UpdatePhoneLineStatusWindow(channel, "Main Menu");
            j = 0;
            for(i = 0; i < 12; i++)
				{
                switch(param.StatPress[i][0])
					{
                   case 'S':
                        Messages[j++] = VOX_PROMPT_press;
                        Messages[j++] = static_cast<char>(i);
                        Messages[j++] = VOX_PROMPT_to_change;
                        Messages[j++] = VOX_PROMPT_page_status;
                        break;
                    case 'V':
                        Messages[j++] = VOX_PROMPT_press;
                        Messages[j++] = static_cast<char>(i);
                        Messages[j++] = VOX_PROMPT_to_change;
                        Messages[j++] = VOX_PROMPT_greeting;
                        break;
                    case 'M':
                        Messages[j++] = VOX_PROMPT_press;
                        Messages[j++] = static_cast<char>(i);
                        Messages[j++] = VOX_PROMPT_to_retrieve_messages;
                        break;
                    case 'A':
                        Messages[j++] = VOX_PROMPT_press;
                        Messages[j++] = static_cast<char>(i);
                        Messages[j++] = VOX_PROMPT_to_change;
                        Messages[j++] = VOX_PROMPT_security_code;
                        break;
                    case 'R':
                        Messages[j++] = VOX_PROMPT_press;
                        Messages[j++] = static_cast<char>(i);
                        Messages[j++] = VOX_PROMPT_to_repeat_menu;
                        break;
					case 'G':
                        Messages[j++] = VOX_PROMPT_press;
                        Messages[j++] = static_cast<char>(i);
                        Messages[j++] = VOX_PROMPT_to_leave_a_voice_mail_;
                        break;

                    default:
                        break;
					}
				}
            errcode = vb40_play( promptfh, channel, prompts, j, Messages );
			break;

        case ST_OP_TRANSFER:
            UpdatePhoneLineStatusWindow( channel, "Op Trnsfr Msg" );
			dx_clrdigbuf( chdev );
            Messages[ 0 ] = 98;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        case ST_OP_TRANSFER2:
            UpdatePhoneLineStatusWindow( channel, "Op Transfer" );
			errcode = XnTransfer( channel, param.OpTransfer, NULL );
            break;

		case ST_VM_TR_MSG:
            UpdatePhoneLineStatusWindow( channel, "VM Trnsfr Msg" );
			dx_clrdigbuf( chdev );
            Messages[ 0 ] = 47;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

		case ST_VM_TRANSFER:
            UpdatePhoneLineStatusWindow(channel, "VM Transfer");
			memset(string2, 0, sizeof(string2));
			strcpy(string2, param.vmTransfer1);
			strcat(string2, page[PagePointer].PagerNumber);
			strcat(string2, param.vmTransfer2);

	        PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"VM Transfer %s.",
							string2 );

			errcode = XnTransfer( channel, string2, NULL );
			break;

        case ST_PARK_TMSG:
            UpdatePhoneLineStatusWindow( channel, "Park Trnsfr Msg" );
			dx_clrdigbuf( chdev );
            Messages[ 0 ] = 86;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        case ST_PARK_TRSFR:
            UpdatePhoneLineStatusWindow( channel, "Park Transfer" );
			memset( string2, 0, sizeof(string2) );
			strcpy( string2, param.ParkTransfer1 );
			strcat( string2, page[ PagePointer ].tempStr );

			if( page[ channel ].tempInt == 0 )
				{
				errcode = XnTransfer( channel, string2, NULL );
				}
			else
				{
				dx_clrcap( &ChannelParameter );
				ChannelParameter.ca_intflg = DX_OPTDIS; // Disable Operator Intercept
				ChannelParameter.ca_hedge = 1;			// 1 == rising edge
				ChannelParameter.ca_nbrdna = 1;			// number of rings before no answer
				ChannelParameter.ca_cnosig = 100;		// timeout delay for no-signal
				errcode = XnTransfer( channel, string2, &ChannelParameter );
				}
            break;

        case ST_PARK_RECALL:
            UpdatePhoneLineStatusWindow( channel, "Park Recall" );
			errcode = XnTransfer( channel, param.ParkTransfer2, NULL );
            break;

        case ST_PARK_NG_MSG:
            UpdatePhoneLineStatusWindow( channel, "Park Unable" );
			dx_clrdigbuf( chdev );
            Messages[ 0 ] = 87;
            if( param.OpTransfer[ 0 ] != 0 ) Messages[ 0 ] = 88;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        // This section is used to check/change paging status
        case ST_STAT_HELLO:
        case ST_MENU_HELLO:
            Messages[ 0 ] = 71;					/* Hello					*/
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_STAT_WRITE:
            UpdatePhoneLineStatusWindow( channel, "Changed Status" );
            Change_Log2( PagePointer );
            if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == '#' )
				{
                memset( page[ PagePointer ].PageInfo, 0, sizeof( page[ PagePointer ].PageInfo ) );
                strcpy( page[ PagePointer ].PageInfo, page[ PagePointer ].DisplayInfo );
				}
            if( Status[ page[ PagePointer ].ACKMin ].Page[ 2 ] == '0' )
				{
                UpdatePhoneLineStatusWindow( channel, "Op Trnsfr Msg" );
                dx_clrdigbuf( chdev );
                Messages[ 0 ] = 98;
                errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
                port[ channel ].state = ST_OP_TRANSFER;
                break;
				}

        case ST_STAT_MSG:
            UpdatePhoneLineStatusWindow( channel, "Check Status" );

            Messages[0] = VOX_PROMPT_current;
            Messages[1] = VOX_PROMPT_page_status;
            Messages[2] = VOX_PROMPT_is;
			memset(string2, 0, sizeof(string2));

            strcpy(string2, page[PagePointer].InitiatorId);
			PrintMessage(MSGTYPE_IVR, hwndD4xMsg, "InitiatorId: [%s]", page[PagePointer].InitiatorId);

            i = 0;

            if(param.CoverPage && string2[0] != 0)
				{				
                Messages[3] = VOX_PROMPT_covered_by;			

                for(i = 0; i < (int) strlen(string2) && string2[i] != 0; i++)
					{
                    if( string2[i] > 47 && string2[i] < 58)
						{
                        Messages[i + 4] = (char) (string2[i] - 48);
						}
                    else
						{
						break;
						}
					}

				// see if a .vce file is present, so that we can
				//  play the custom greeting announcement of the
				//  covering profile.
				//
                memset(string2, 0, sizeof( string2));
                strcpy(string2, page[PagePointer].FileName);
                getfile(string2, "VCE", sizeof(string2));

                rv = GetFileAttributes(string2);

				PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"custom greeting filename: [%s]",
							string2);

               if(rv != 0xFFFFFFFF ) 
					port[ channel ].state = ST_STAT_MSG6;

                if(param.ConfirmChange && port[ channel ].state != ST_STAT_MSG6 )
					{
                    Messages[ i + 4 ] = 23;			/* press * to change  	*/
                    Messages[ i + 5 ] = 56;			/* page status			*/

                    if( CheckMenu( ) )
                        Messages[ i + 6 ] = 81;		/* press # to return to main menu */
                    else
                        Messages[ i + 6 ] = 82;		/* press # to quit		*/

                    i = i + 3;
					}
                errcode = vb40_play(promptfh, channel, prompts, i+4, Messages );            
				}
            else
				{
                UpdatePhoneLineStatusWindow( channel, "Chng Stat Prmpt" );
                Messages[ 0 ] = 12;					/* Current Status is	*/
				if( param.altMenuPrompt[ page[ PagePointer ].ACKMin ] > 0 )
					Messages[ 1 ] = (char)param.altMenuPrompt[ page[ PagePointer ].ACKMin ];
				else
	                Messages[ 1 ] = (char)( Status[ page[ PagePointer ].ACKMin ].Prompt-1 );
                if( Messages[ 1 ] < 0 ) Messages[ 1 ] = 32;	/* Unavailable  */
                port[ channel ].PortStatus = 1;
                errcode = vb40_play( StatusFh, channel, StatusPrompts, 2, Messages );
                port[ channel ].state = ST_STAT_MSG2;
            }
            port[ channel ].PortStatus = 1;

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Status prompt# %i, covered by %s.",
							channel,
							Status[page[PagePointer].ACKMin].Prompt,
							page[ PagePointer ].FileName);
			break;

        case ST_STAT_MSG3:
            i = 0;

            if(param.ConfirmChange)
				{
                Messages[ 0 ] = 23;		/* Press * to change	*/
                Messages[ 1 ] = 56;		/* page status			*/
                if( CheckMenu( ) )
                    Messages[ 2 ] = 81; /* press # for main menu */
                else
                    Messages[ 2 ] = 82; /* press # to quit		*/
                i = i + 3;
				}

            switch(Status[page[PagePointer].ACKMin].Page[0])
				{
                case 'O':
                case 'F':
                case '#':
                    Messages[ 0 ] = 61;

                    if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'O' )
						Messages[ 0 ] = 43;

                    memset(string2, 0, sizeof(string2));
                    k = 0;

                    for(j = 0; j < 15; j++)
						{
                        if( page[ PagePointer ].PageInfo[ j ] > 47 && page[ PagePointer ].PageInfo[ j ] < 58 )
							{
                            string2[k] = (char) (page[PagePointer].PageInfo[j]);
                            k++;
							}
                        else if(param.StarPrompt > 1 && page[PagePointer].PageInfo[j] == '*')
							{
                            string2[k] = (char) (param.StarPrompt - 1); k++;
							}
                        else if( page[ PagePointer ].PageInfo[ j ] != '-' )
                            break;
						}

                    for( i=0; i<(int)strlen( string2 ) && string2[ i ] != 0; i++ )
						{
                        if( string2[ i ] > 47 && string2[ i ] < 58 )
                            Messages[ i + 1 ] = (char)( string2[ i ] - 48 );
                        else
							{
                            Messages[ i + 1 ] = (char)( string2[ i ] );
							}
						}

                    if( param.ConfirmChange )
						{
                        Messages[ i + 1 ] = 23;
                        Messages[ i + 2 ] = 56;

                        if( CheckMenu( ) )
                            Messages[ i + 3 ] = 81;
                        else
                            Messages[ i + 3 ] = 82;
                        i = i + 3;
						}

                    i++;

                    PrintMessage(MSGTYPE_IVR,
									hwndD4xMsg,
									"Line [%i] Status forwarded to [%s]..",
									channel,
									page[PagePointer].PageInfo);
                    break;
				}
            errcode = vb40_play(promptfh, channel, prompts, i, Messages);
            break;

        case ST_STAT_TYPES:
            UpdatePhoneLineStatusWindow(channel, "Status Types");
            j = 0;

			Messages[j] = 10;		// "available statuses are"
			j++;

            for(i = 0; i < 30; i++)
				{
                if(Status[i].Prompt != -1)
					{
                    k = FALSE;

                    if(param.MultiQueNumber > 0)
                        if(strncmp(page[PagePointer].PageType,
							Queue[param.MultiQueNumber].Id + 1, 2) == 0)
							{
                            k = TRUE;
							}

                    if(param.MultiQueNumber2 > 0)
                        if(strncmp(page[PagePointer].PageType,
							Queue[param.MultiQueNumber2].Id + 1, 2) == 0)
							{
                            k = TRUE;
							}

                    if(strncmp(page[PagePointer].PageType, Queue[0].Id + 1, 2) == 0)
						{
                        k = TRUE;
						}

                    if(page[PagePointer].ComNumber == -1 &&
						page[PagePointer].ScriptNumber == -1 &&
						Status[i].Page[0] == 'Y' &&
						k == FALSE)
						{
						continue;
						}

                    if(page[PagePointer].Flag3[0] == 'Y' && Status[i].Page[0] != 'Y')
						{
						continue;
						}

                    if(Status[i].Page[0] == ' ')
						continue;

                    k = i;
                    errcode = -1;

                    if(i > 9 && i < 21)
						{
                        k = 89 + i;
						}
                    else if( i > 20 )
						{
                        k = 109;
                        errcode = i - 20;
						}

                    if( param.PressAfter )
						{
						if( param.altMenuPrompt[ i ] > 0 )
							Messages[ j ] = (char)param.altMenuPrompt[ i ];
						else
							Messages[ j ] = (char)( Status[ i ].Prompt - 1 );

						j++;

                        Messages[ j ] = 47;		// "press..."
						j++;

                        Messages[ j ] = (char ) k; j++;

                        if( errcode > -1 )
							{
                            Messages[ j ] = (char ) errcode;
							j++;
							}

                        Messages[ j ] = 40;		// 1/2 sec silence
						j++;
						}
                    else
						{
                        Messages[ j ] = (char ) k; j++;

                        if( errcode > -1 )
							{
                            Messages[ j ] = (char ) errcode;
							j++;
							}

						if( param.altMenuPrompt[ i ] > 0 )
							Messages[ j ] = (char)param.altMenuPrompt[ i ];
						else
							Messages[ j ] = (char)( Status[ i ].Prompt - 1 );

						j++;

                        Messages[ j ] = 40;		// 1/2 sec silence
						j++;
						}
					}
				}

            Messages[ j ] = 11;		// "enter new status number or..."
			j++;

            if( param.StatPoundOpt[ 0 ] == 0 )
				{
                Messages[ j ] = 54;		// "press pound sign to leave unchanged"
				j++;
				}
            else
				{
                port[ channel ].state = ST_STAT_MSG4;
				}

            port[ channel ].PortStatus = 1;

			// play prompts from xtpgstat.vap
            errcode = vb40_play( StatusFh, channel, StatusPrompts, j, Messages );
			break;

        case ST_STAT_MSG5:
            UpdatePhoneLineStatusWindow( channel, "# Option" );
            j = 0;
            switch( param.StatPoundOpt[ 0 ] )
            {
                case 'V':
                    Messages[ j++ ] = 68;
                    Messages[ j++ ] = 11;
                    Messages[ j++ ] = 54;
                    Messages[ j++ ] = 57;
                    break;
                case 'M':
                    Messages[ j++ ] = 68;
                    Messages[ j++ ] = 11;
                    Messages[ j++ ] = 77;
                    break;
                case 'A':
                    Messages[ j++ ] = 68;
                    Messages[ j++ ] = 11;
                    Messages[ j++ ] = 54;
                    Messages[ j++ ] = 78;
                    break;
                case 'R':
                    Messages[ j++ ] = 68;
                    Messages[ j++ ] = 11;
                    Messages[ j++ ] = 66;
                    break;
                default:
                    break;
            }
            errcode = vb40_play( promptfh, channel, prompts, j, Messages );
			break;

        case ST_STAT_EX_MSG1:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = VOX_PROMPT_enter_month_and_day_;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_STAT_EX_MSG2:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = VOX_PROMPT_enter_time_followed_by_;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_STAT_EX_AMPM:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = VOX_PROMPT_enter_one_for_A_M_;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_STAT_EX_ERR:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = VOX_PROMPT_unable_to_set_schedule;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_STAT_F_MSG:
        case ST_STAT_RFD_MSG:
            dx_clrdigbuf(chdev);
			Messages[0] = VOX_PROMPT_please_enter;
            Messages[1] = VOX_PROMPT_forwarding_number;

            if(Status[page[PagePointer].ACKMin].Page[0] == 'O')
				{
				Messages[1] = VOX_PROMPT_room_number;
				}

            Messages[2] = VOX_PROMPT_followed_by_pound_sign;
            port[channel].PortStatus = 1;

            errcode = vb40_play(promptfh, channel, prompts, 3, Messages);
			break;

        case ST_STAT_RFD_NG:
            dx_clrdigbuf( chdev );
            Messages[0] = 61;

            if(Status[page[PagePointer].ACKMin].Page[0] == 'O')
				Messages[0] = 43;

            Messages[1] = 56;
            Messages[2] = 31;
            port[channel].PortStatus = 1;

            errcode = vb40_play(promptfh, channel, prompts, 3, Messages);
			break;

        case ST_STAT_C_MSG:
            dx_clrdigbuf(chdev);
            Messages[0] = 62;
            Messages[1] = (char) param.PromptType;
            i = 2;
            if( param.IdLength > 8 )
				{
                Messages[2] = 16;
                i++;
				}
            port[channel].PortStatus = 1;
            errcode = vb40_play(promptfh, channel, prompts, i, Messages);
			break;

        case ST_STAT_C_NG1:
        case ST_CVR_RECUR:
            UpdatePhoneLineStatusWindow( channel, "Recursive Cover" );
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 18;
            Messages[ 1 ] = 65;
            errcode = vb40_play( promptfh, channel, prompts, 2, Messages );
            break;

        case ST_STAT_C_NG2:
            UpdatePhoneLineStatusWindow(channel, "Invalid Cvr Ext");
            dx_clrdigbuf(chdev);
            Messages[0] = 18;						// "invalid"
            Messages[1] = (char) param.PromptType;	// "pager id"
            memset(string2, 0, sizeof(string2));
            strcpy(string2, page[PagePointer].InitiatorId);

            for(i = 0; i < 10 && string2[i] != 0; i++)
				{
                if(string2[i] > 47 && string2[i] < 58)
                    Messages[i + 2] = (char)(string2[i] - 48);
                else break;
				}
            errcode = vb40_play(promptfh, channel, prompts, i+2, Messages);
            break;

        case ST_STAT_C_NG6:
            UpdatePhoneLineStatusWindow(channel, "Invalid Cvr Target");
            dx_clrdigbuf(chdev);

			// insert a custom message to indicate that the coverage target 
			//  selected is someone who is not available.
			//   
			Messages[0] = VOX_PROMPT_please_call_page_operator_for_;	
            Messages[1] = VOX_PROMPT_silence_for_half_a_second;
	 
			//Messages[0] = VOX_PROMPT_coverage_change;	
            //Messages[1] = VOX_PROMPT_is;
            //Messages[2] = VOX_PROMPT_invalid;
            //Messages[3] = VOX_PROMPT_silence_for_half_a_second;

            errcode = vb40_play(promptfh, channel, prompts, 2, Messages);
            break;

        case ST_STAT_C_NG3:
            UpdatePhoneLineStatusWindow( channel, "Invalid Cvr Ext" );
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 18;							// "invalid"
            Messages[ 1 ] = (char) param.PromptType;	// "pager id"

            for(i = 0; i < sizeof(page[PagePointer].InitiatorId) && page[PagePointer].InitiatorId[i] != 0; i++)
				{
                if(page[PagePointer].InitiatorId[i] > 47 && page[PagePointer].InitiatorId[i] < 58)
                    Messages[i + 2] = (char) (page[PagePointer].InitiatorId[i] - 48);
                else 
					break;
				}
            Messages[i + 2] = 56;
            Messages[i + 3] = 59;
            errcode = vb40_play(promptfh, channel, prompts, i + 4, Messages);
            break;

        case ST_STAT_C_NG5:
            UpdatePhoneLineStatusWindow( channel, "Impaired Vendor");
            dx_clrdigbuf(chdev);

            Messages[0] = VOX_PROMPT_paging;
            Messages[1] = VOX_PROMPT_coverage_change;	
            Messages[2] = VOX_PROMPT_is;
            Messages[3] = VOX_PROMPT_unavailable;
            Messages[4] = VOX_PROMPT_silence_for_half_a_second;
            Messages[5] = VOX_PROMPT_forwarding_number;
            Messages[6] = VOX_PROMPT_is;
            Messages[7] = VOX_PROMPT_unavailable;
            Messages[8] = VOX_PROMPT_silence_for_half_a_second;
            errcode = vb40_play(promptfh, channel, prompts, 9, Messages);
            break;

        case ST_STAT_C_NG4:
			if(param.altMenuPrompt[page[PagePointer].ScriptNumber] > 0)
				{
				Messages[0] = (char) param.altMenuPrompt[page[PagePointer].ScriptNumber];
				}
			else
				{
				Messages[0] = (char) (Status[page[PagePointer].ScriptNumber].Prompt - 1);
				}

            if(Messages[0] < 0) 
				{
				Messages[0] = 31;
				}

            errcode = vb40_play(StatusFh, channel, StatusPrompts, 1, Messages);
            break;

        case ST_STAT_M_MSG:
            Messages[0] = VOX_PROMPT_press_one_to_retrieve_;
            Messages[1] = VOX_PROMPT_press_two_to_retrieve_;
            errcode = vb40_play(promptfh, channel, prompts, 2, Messages);
			break;

        case ST_STAT_M_MSG2:
            UpdatePhoneLineStatusWindow(channel, "Message Search");
            dx_clrdigbuf(chdev);
            Messages[0] = VOX_PROMPT_please_wait_retrieving_messages;
            errcode = vb40_play(promptfh, channel, prompts, 1, Messages);
			break;

        case ST_STAT_M_TALK:
            UpdatePhoneLineStatusWindow(channel, "Numeric Msg Play");
            dx_clrdigbuf(chdev);
            Messages[0] = VOX_PROMPT_call;
			j = 1;

            Set_Null(page[PagePointer].DisplayInfo, sizeof(page[PagePointer].DisplayInfo));

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"next_state(): ST_STAT_M_TALK [%s]",
							page[PagePointer].DisplayInfo);
            for(i = 0;
				i < sizeof(page[PagePointer].DisplayInfo) - 23 && page[PagePointer].DisplayInfo[i] != 0;
				i++)
				{
                if(page[PagePointer].DisplayInfo[i + 23] >= '0' &&
					page[PagePointer].DisplayInfo[i + 23] <= '9')
					{
					// '0' thru '9' character found,
					//  map the digit representation to the spoken voice prompt number
					//  in the .VAP file.
					//
					//  offset 0 in the file corresponds to the word "zero".
                    Messages[j++] = static_cast<char>(page[PagePointer].DisplayInfo[i + 23] - '0');
					}
                else if( page[ PagePointer ].DisplayInfo[i + 23] == '*')
					{
					// note that a <asterisk> embedded in a number
					//  will be spoken as "ID Number"
                    Messages[j++] = VOX_PROMPT_i_d_number;
					}
                else if(page[PagePointer].DisplayInfo[i + 23] != ' ' &&
						page[PagePointer].DisplayInfo[i + 23] != '-')
					{
					// no pronunciation for <space> or <minus>, but these
					//  may be included in a "numeric" message.
					break;
					}
				}
            errcode = vb40_play(promptfh, channel, prompts, j, Messages);
            break;

        case ST_STAT_M_TALK2:
            j = 11;
            UpdatePhoneLineStatusWindow( channel, "Oper Msg Play" );
            dx_clrdigbuf( chdev );
            Messages[0] = VOX_PROMPT_operator_page;
			k = 1;

            if(param.VoiceConv == FALSE)
				{
				Messages[1] = VOX_PROMPT_call;
				k++;
				}

            Set_Null( page[ PagePointer ].DisplayInfo, sizeof( page[ PagePointer ].DisplayInfo ) );

            for( i=11; i<( int )strlen( page[ PagePointer ].DisplayInfo ); i++ )
				{
                if( page[ PagePointer ].DisplayInfo[ i ] == '+' )
					{
					j=i + 1;
					}
				}

            for(i = 0; i < (int) sizeof(page[PagePointer].DisplayInfo) - j && 
							page[PagePointer].DisplayInfo[i] != 0; i++)
				{
				
                //if(page[PagePointer].DisplayInfo[i + j] > 47 && page[PagePointer].DisplayInfo[i + j] < 58)
					if(isdigit(page[PagePointer].DisplayInfo[i+j]))
					{
                    Messages[k++] = static_cast<char>(page[PagePointer].DisplayInfo[i+j] - 48);
					}
                else if( param.VoiceConv )
					{
                    switch( page[ PagePointer ].DisplayInfo[ i+j ] )
						{
                        case '*':
                        case 'X':
                            Messages[ k++ ] = 19;
                            break;
                        case 'H':
                            Messages[ k++ ] = 90;
                            break;
                        case 'B':
                            Messages[ k++ ] = 91;
                            break;
                        case 'E':
                            Messages[ k++ ] = 92;
                            break;
                        case 'N':
                            Messages[ k++ ] = 93;
                            break;
                        case 'R':
                            Messages[ k++ ] = 94;
                            break;
                        case 'S':
                            Messages[ k++ ] = 95;
                            break;
                        case 'P':
                            Messages[ k++ ] = 96;
                            break;
                        case 'O':
                            Messages[ k++ ] = 69;
                            break;
                        case 'C':
                            Messages[ k++ ] = 51;
                            break;
                        case ' ':
                            break;
                        default:
                            page[ PagePointer ].DisplayInfo[i + j + 1] = 0;
                            break;
						}
					}
                else
					break;
				}
            errcode = vb40_play(promptfh, channel, prompts, k, Messages);
            break;

        case ST_STAT_M_MSG3:
			// prepare "message retrieve" selection options.
			//  attempt to be intelligent about it.
			//
			//  *  if the most current message is the same as the newest message,
			//     there is no need to offer the "play previous message" option.
			//     because you are already on the most previous.
			//
			//  *  always offer "repeat current message".
			//
			//  *  if you are at the minimum numbered message,
			//     there are no "next messages" (older)
			//     threfore no need to offer "play next message"
			//
            UpdatePhoneLineStatusWindow(channel, "Message Options");
            i = 0;

			// if current message is less than newest,
			//  offer "play previous" (actually play older message).
            if( (page[PagePointer].iCurrentMessageNumber != 0) &&
				(page[PagePointer].iCurrentMessageNumber > page[PagePointer].iOldestMessageNumber))
				{
				Messages[i++] = VOX_PROMPT_press_one_to_play_previous_;
				}

			// always offer to play current message.
            Messages[i++] = VOX_PROMPT_press_two_to_repeat_current_;

			// if at least one message has been received,
			//  and you are not at the minimum limit,
			//  offer "play next" (actually play newer message).
            if( (page[PagePointer].iCurrentMessageNumber != 0) &&
				(page[PagePointer].iCurrentMessageNumber < page[PagePointer].iNewestMessageNumber))
				{
				Messages[i++]= VOX_PROMPT_press_three_to_play_next_;
				};

			//jph add extra voice prompts to handle forward and e-mail
			if(page[PagePointer].voiceMsg)
				{
				Messages[i++] = VOX_PROMPT_press_;
				Messages[i++] = VOX_PROMPT_four;
				Messages[i++] = VOX_PROMPT_press_;
				Messages[i++] = VOX_PROMPT_five;
				}

			// present 1 or 2 "control" menu options.
            if(CheckMenu())
				{
                Messages[i] = VOX_PROMPT_press_pound_sign_to_return_;
				}
            else
				{
                Messages[i] = VOX_PROMPT_press_pound_sign_quit;
				}
            i++;

            errcode = vb40_play(promptfh, channel, prompts, i, Messages);
            break;

        case ST_STAT_M_VOICE:
            dx_clrdigbuf( chdev );
            UpdatePhoneLineStatusWindow( channel, "Voice Message" );

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            errcode = PlayFil(channel, page[PagePointer].FileName, &port[channel].file_handle, 0);

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_STAT_M_VOICE - Line [%i] Voice file [%s], fh [%i], played.",
							channel,
							page[PagePointer].FileName,
							port[channel].file_handle);
			break;

        case ST_STAT_M_TEXT:
            UpdatePhoneLineStatusWindow(channel, "Text Message");
            dx_clrdigbuf(chdev);
            Messages[0] = VOX_PROMPT_text_message;
            errcode = vb40_play(promptfh, channel, prompts, 1, Messages);
			break;

        case ST_STAT_M_CVR:
            UpdatePhoneLineStatusWindow( channel, "Message Cover" );
            dx_clrdigbuf( chdev );
            Messages[0] = 32;
            Messages[1] = 58;

            memset(string2, 0, sizeof(string2));
            strcpy(string2, page[ PagePointer ].DisplayInfo+11);
            for( i=0; i<(int)strlen( string2 ) && string2[ i ] != 0; i++ ){
                if( string2[ i ] > 47 && string2[ i ] < 58 )
                    Messages[ i + 2 ] = (char)( string2[ i ] - 48 );
                else break;
            }
            errcode = vb40_play( promptfh, channel, prompts, i+2, Messages );
            break;

        case ST_STAT_M_STAT:
            UpdatePhoneLineStatusWindow( channel, "Message Status" );
            Messages[ 0 ] = 53; j=1;

            for(i = 0; i < 30; i++)
				{
                if(Status[i].Prompt > 0)
					{
                    if(strncmp(Status[i].Text, 
								page[PagePointer].DisplayInfo + 11, 
								strlen(Status[i].Text)) == 0)
						{
						if(param.altMenuPrompt[ i ] > 0)
							Messages[1] = (char) param.altMenuPrompt[i];
						else
							Messages[1] = (char) (Status[i].Prompt - 1);
                        j = 2;
                        substr = strstr(page[PagePointer].DisplayInfo, " #:");

                        if(substr != NULL)
							{
                            for(i = 0; i < 15; i++)
								{
                                if(isdigit(substr[i + 3]))
									{
                                    Messages[j++] = (char)( substr[i + 3] - 48);
									}
                                else break;
								}
							}
                        break;
						}
					}
				}
            errcode = vb40_play(StatusFh, channel, StatusPrompts, j, Messages);
			break;

        // This section is used to check/change custom voice prompts.
        case ST_VCE_MSG:
            UpdatePhoneLineStatusWindow( channel, "Check V Prompt" );
            dx_clrdigbuf( chdev );
            Messages[0] = 45;
            Messages[1] = 57;
            Messages[2] = 59;
            port[ channel ].PortStatus = 1;

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i Current voice prompt %s.VCE.",
							channel,
							page[ PagePointer ].Extension);

			// changed this.... 2006-10-02, why would we use anything other than
			//  the "initial" pagerid/profileid when prompting to change custom greeting?
			//  I can't think of any. This has been here a long time without complaint,
			//  and would be correct, in cases not involving coverage.
			//
			memset(page[PagePointer].PageInfo, 0, sizeof(page[PagePointer].PageInfo));
			strcpy(page[PagePointer].PageInfo, page[PagePointer].InitialExtension);

			// 2006-12-15 attempt to fix the problem where the password is 
			//  required only at application startup.
			memset(page[PagePointer].Password, 0, sizeof(page[PagePointer].Password));
			
			getfile(page[PagePointer].PageInfo, "VCE", 148);

            errcode = vb40_play(promptfh, channel, prompts, 3, Messages);
            break;

        case ST_VCE_MSG2:
            dx_clrdigbuf( chdev );
            UpdatePhoneLineStatusWindow( channel, "Voice Prmpt Play" );

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            errcode = PlayFil(channel, page[PagePointer].PageInfo, &port[channel].file_handle, 1);

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_VCE_MSG2 - Line %i Voice prompt message %s, fh %i, ret %i.",
							channel,
							page[ PagePointer ].PageInfo,
							port[ channel ].file_handle,
							errcode );
            break;

        case ST_VCE_MSG3:
            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_VCE_MSG3 - No Voice Prompt");

            UpdatePhoneLineStatusWindow(channel, "No Voice Prompt");

            Messages[0] = VOX_PROMPT_unavailable;

            port[channel].PortStatus = 1;
            errcode = vb40_play(promptfh, channel, prompts, 1, Messages);
            break;

        case ST_VCE_MSG4:
            Messages[ 0 ] = 23;
            Messages[ 1 ] = 57;
            if( CheckMenu( ) )
                Messages[ 2 ] = 81;
            else
                Messages[ 2 ] = 82;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 3, Messages );
			break;

        case ST_VCE_CHG_MSG:
            UpdatePhoneLineStatusWindow( channel, "Speak Now" );
            dx_clrdigbuf( chdev );
            Messages[0] = 36;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_VCE_CHANGE:
            memset(string2, 0, sizeof(string2));

			//PrintMessage(MSGTYPE_IVR,
			//				hwndD4xMsg,
			//				"Available Info - Extension [%s].",
			//				page[PagePointer].Extension);
			//
			//PrintMessage(MSGTYPE_IVR,
			//				hwndD4xMsg,
			//				"Available Info - InitialExtension [%s].",
			//				page[PagePointer].InitialExtension);

			// strcpy(string2, page[PagePointer].Extension);

			// changed/added 2006-09-21 kgill.
			//
			// Always use the initial pagerid / profileid when doing an update.
			//  it makes no sense to change the covering extension's "custom greeting".

			// must "push back" on the .Extension also, so that this will not be 
			//  spoken /repeated, on the next time thru the state machine / menu.
			//  this "cancels" any knowledge of coverage, which should be ok, as
			//  the custom greeting on the original pagerid / profileid was requested
			//  to be changed.
			//
			strcpy(page[PagePointer].Extension, page[PagePointer].InitialExtension);

			// why do we need this info in 2 places???, just grin and bear it.
			strcpy(page[PagePointer].PageInfo, page[PagePointer].InitialExtension);
			//getfile(page[PagePointer].PageInfo, "VCE");
			getfile(page[PagePointer].PageInfo, "VCE", sizeof(page[PagePointer].PageInfo));

			strcpy(string2, page[PagePointer].InitialExtension);
			//getfile(string2, "VCE");
			getfile(string2, "VCE", sizeof(string2));

			for(i = strlen(string2) - 1; i > 0; i--)
				{
				if(string2[i] == '\\')
					{
					string2[i] = 0;
					break;
					}
				}

            rv = GetFileAttributes(string2);

            if(rv == 0xFFFFFFFF ||
				(rv & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
				{
				// GetFileAttributes() failed,
				//  or the string does not identify a directory.

				rv = CreateDirectory(string2, NULL);

				if(rv == 0)
					{
					// failed to create directory
					PrintMessage(MSGTYPE_IVR,
									hwndD4xMsg,
									"Line [%i] Unable to CreateDirectory() [%s]",
									channel,
									string2);
					}
				else
					{
					// directory created.
					PrintMessage(MSGTYPE_IVR,
									hwndD4xMsg,
									"Line [%i] CreateDirectory() ok. [%s], return [%i]",
									channel,
									string2,
									rv);
					}

				}

            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

			rv = GetFileAttributes(page[PagePointer].PageInfo);

			if(rv != 0xFFFFFFFF)
				{
				if(SetFileAttributes(page[PagePointer].PageInfo, FILE_ATTRIBUTE_NORMAL) == TRUE)
					{
					remove(page[PagePointer].PageInfo);
					}
				}

			port[channel].file_handle = dx_fileopen(page[PagePointer].PageInfo,
														O_CREAT | O_BINARY | O_TRUNC | O_RDWR,
														S_IREAD | S_IWRITE);

            UpdatePhoneLineStatusWindow(channel, "Voice Prompt Rec");
            GetLocalTime(&port[channel].timer);

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line [%i] Recording voice prompt: [%s], File Handle: [%i].",
							channel,
							page[PagePointer].PageInfo,
							port[channel].file_handle);

            errcode = record(channel, port[channel].file_handle, param.VoicePromptTime);
            break;

        // This section is used to change user password.
        case ST_PASS_WRITE:
            Change_Log3( PagePointer );
            UpdatePhoneLineStatusWindow( channel, "Password Changed" );
            page[ PagePointer ].Printed[ 0 ] = 'P';

        case ST_PASS_MSG:
            UpdatePhoneLineStatusWindow( channel, "Check Password" );
            dx_clrdigbuf( chdev );
            Messages[0] = 45;
            Messages[1] = 78;
            Messages[2] = 59;

            memset(string2, 0, sizeof(string2));
            strcpy(string2, page[ PagePointer ].Password );
            if( string2[ 0 ] != 0 ){
                for( i=0; i<(int)strlen( string2 ) && string2[ i ] != 0; i++ ){
                    if( string2[ i ] > 47 && string2[ i ] < 58 )
                        Messages[ i + 3 ] = (char)( string2[ i ] - 48 );
                    else break;
                }
                if( param.ConfirmChange ){
                    Messages[ i + 3 ] = 23;
                    Messages[ i + 4 ] = 78;
                    if( CheckMenu( ) )
                        Messages[ i + 5 ] = 81;
                    else
                        Messages[ i + 5 ] = 82;
                    i = i + 3;
                }
                errcode = vb40_play( promptfh, channel, prompts, i+3, Messages );
            }
            else{
                i = 1;
                Messages[ 0 ] = 25;
                if( param.ConfirmChange ){
                    Messages[ 1 ] = 23;
                    Messages[ 2 ] = 78;
                    if( CheckMenu( ) )
                        Messages[ 3 ] = 81;
                    else
                        Messages[ 3 ] = 82;
                    i = 4;
                }
                errcode = vb40_play( promptfh, channel, prompts, i, Messages );
            }
            port[ channel ].PortStatus = 1;
            break;

        case ST_PASS_MSG2:
            UpdatePhoneLineStatusWindow( channel, "Get Password" );
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 67;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        case ST_DID_HKFLSH:
            UpdatePhoneLineStatusWindow( channel, "DID Hookflash" );
			errcode = XnTransfer( channel, "&", NULL );
            break;

        case ST_DID_GET_EXT:
        case ST_GET_EXT:
        case ST_STAT_C_GET:
            /* wait for digits from the caller           */
			UpdatePhoneLineStatusWindow( channel, "Receiving Ext" );
            port[ channel ].PortStatus = 1;
            errcode = get_digits( channel, param.IdLength );
			break;

        case ST_PLAY_PROMPT:
        case ST_GOODBYE_PROMPT:
        case ST_STAT_V_PRMPT:
        case ST_MENU_V_PRMPT:
        case ST_LSTOK_VCE:
        case ST_NAME_SELECT2:
        case ST_STAT_MSG7:
            UpdatePhoneLineStatusWindow( channel, "Voice Prmpt Play" );
            port[ channel ].PortStatus = 1;

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            memset(string2, 0, sizeof(string2));
            if( port[ channel ].state == ST_STAT_MSG7 )
                strcpy( string2, page[ PagePointer ].FileName );
            else
                strcpy( string2, page[ PagePointer ].Extension );


            //getfile( string2, "VCE" );
			getfile( string2, "VCE", sizeof(string2));
            errcode = PlayFil(channel, string2, &port[channel].file_handle, 1);

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_PLAY_PROMPT + Line %i Voice prompt message %s, fh %i, ret %i",
							channel,
							string2,
							port[ channel ].file_handle,
							errcode);
            break;

        case ST_GREETING1_MSG:
        case ST_GREETING2_MSG:
            UpdatePhoneLineStatusWindow( channel, "V Greeting Play");
            port[ channel ].PortStatus = 1;
            dx_clrdigbuf( chdev );

            if(port[channel].file_handle > 0)
				{
                dx_fileclose(port[channel].file_handle);
                port[channel].file_handle = 0;
				}

            memset(string2, 0, sizeof(string2));
            if( port[channel].state == ST_GREETING2_MSG)
				{
				if(param.IVRPort[channel].iType == 20)
					strcat(string2, param.greetingExtPark);
				else
					strcat(string2, param.GreetingExt2);
				}
            else
				{
                strcat( string2, param.GreetingExt);
				}
            getfile(string2, "VCE", sizeof(string2));
            errcode = PlayFil(channel, string2, &port[channel].file_handle, 1);

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"ST_GREETING1_MSG + Line %i Greeting prompt %s, fh %i, ret %i.",
							channel,
							param.GreetingExt,
							port[ channel ].file_handle,
							errcode);
            break;

        case ST_CHECK_IMPAIRED_VENDOR_ANN:
			break;

        case ST_PLAY_ID:
        case ST_GOODBYE_ID:
        case ST_STAT_I_PRMPT:
        case ST_MENU_I_PRMPT:
        case ST_NAME_MSG2:
			// prepare to play/speak the Pager Id that was entered.

            UpdatePhoneLineStatusWindow(channel, "Id Confirmed");

            Messages[0] = (char) (param.PromptType);
            i = 0;
            j = 1;
            memset(string2, 0, sizeof(string2));
            strcpy(string2, page[PagePointer].PagerId);

            if((int) strlen(string2) > param.IdPromptLength)
				{
                memset(string2, 0, sizeof(string2));
                strcpy(string2,
						page[PagePointer].PagerId + strlen(page[PagePointer].PagerId) - param.IdPromptLength);
				}

			// scan the string for '0' thru '9'
			//  adding to the message list.
			//  these are the voice prompts to be played/spoken.
            for(i = i; i < 10 && string2[i] != 0; i++)
				{
                if(string2[i] >= '0' && string2[i] <= '9')
					{
                    Messages[j] = (char) (string2[i] - 48);
					}
                else
					{
					break;
					}
                j++;
				}

            port[channel].PortStatus = 1;

            errcode = vb40_play(promptfh, channel, prompts, j, Messages);

            if( port[ channel ].state == ST_NAME_MSG2 )
				{
                memset( port[ channel ].digsave, 0, sizeof( port[ channel ].digsave ) );

				// remove spaces from ???
                j = 0;
                for(i = 0; i < 10; i++)
					{
                    if( page[ PagePointer ].tempStr2[ i ] != ' ' )
						{
                        port[ channel ].digsave[ j ] = page[ PagePointer ].tempStr2[ i ];
                        j++;
						}
					}
				}
            break;

        case ST_STAT_F_GET:
        case ST_STAT_RFD_GET:
        case ST_LST_GET_DIS:
        case ST_VOICE_DTMF:
			// wait for 30 digits from the caller
			UpdatePhoneLineStatusWindow( channel, "Receiving disp" );
            port[channel].PortStatus = 1;

			// xxx
            //errcode = get_digits(channel, 15);
            errcode = get_digits(channel,  MAX_DIGITS_IN_REFERRED_TO_NUMBER);
			break;

        case ST_GET_DIS:
			UpdatePhoneLineStatusWindow( channel, "Receiving disp" );
            port[ channel ].PortStatus = 1;
            errcode = get_digits( channel, 25 );
			break;
		
		// 6/24/2016 - DE20747 - SOguz - 
		// Add pageable all devices to queue file 
		case ST_ADD_ALL_DVCS_Q:
            // files exists, add record to queue file
			if (isPageAllDevicesOn)
			{
				int counter = GetAllPagingDevicesFromProfile (PagePointer);				
				char msg1[250];
				char msg2[250];
				int result = FALSE;

				if (counter > 0)
				{
					PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"PageAllDevices is in ST_ADD_ALL_DVCS_Q state");

					PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"PageAllDevices is adding pages to the queue. There are %i pagers for profileid = %s", 
					counter,
					page[PagePointer].Extension);

				

					for (int k=0; k<counter; k++)
					{
					//pointer to priority queue (list page) is bigger than 0 
					if(page[PagePointer].PriorityQPointer > 0)
						{
						i = FALSE;
						memset(string, 0, sizeof(string));
						sprintf(string, "%-3.3s", AllPageableDevices[k].FrqType);
						if(param.CodeQues[0] != 0)
							{
							page[PagePointer].ComNumber = -1;
							page[PagePointer].ScriptNumber = -1;

							// Calling GetPageType() to assure that page[PagePointer].ScriptNumber will be set to its propert value, before Get_Frq() call
							// as otherwise when paging all devices the ScriptNumber value is always the value related with the Profile's default pager type, which causes issues
							// when different type of pagers are used in a Profile, like when mixing Number Pagers (RCC) and AMC Devices (COM).
							GetPageType( PagePointer, string,  __FILE__, __LINE__ );

							i = Get_Frq(PagePointer, param.CodeQues, string, k);
							}
						if(i == FALSE)
							{
								// Calling GetPageType() to assure that page[PagePointer].ScriptNumber will be set to its propert value, before Get_Frq() call
								// as otherwise when paging all devices the ScriptNumber value is always the value related with the Profile's default pager type, which causes issues
								// when different type of pagers are used in a Profile, like when mixing Number Pagers (RCC) and AMC Devices (COM).
								GetPageType( PagePointer, string,  __FILE__, __LINE__ );

								result = Get_Frq(PagePointer, string, string, k);
							}
						}
					else
					{
						memset(string, 0, sizeof(string));
						sprintf(string, "%-3.3s", AllPageableDevices[k].FrqType);

						// Calling GetPageType() to assure that page[PagePointer].ScriptNumber will be set to its propert value, before Get_Frq() call
						// as otherwise when paging all devices the ScriptNumber value is always the value related with the Profile's default pager type, which causes issues
						// when different type of pagers are used in a Profile, like when mixing Number Pagers (RCC) and AMC Devices (COM).
						GetPageType( PagePointer, string,  __FILE__, __LINE__ );

						result = Get_Frq(PagePointer, string, string, k);
					}
				
					XnAutoFormat(PagePointer);
					AddSetUp(PagePointer, "External");

					UpdatePhoneLineStatusWindow(channel,
												"Add page %s",
												page[PagePointer].Extension);					

					// in order to accept the request, it must be "deliverable", that is
					//  defined somewhere in the pageserver configuration. 
					// don't accept requests that cannot be delivered, or handed off for delivery.
					//
					page[PagePointer].PortNumber = GetQuePointer(PagePointer);
					i = FALSE;

					if((page[PagePointer].PortNumber >= 0) || (IsSmsType(string)))
						{
							//06/30/2016 - DE20747 - SOguz - if page server in failsafe mode and pager type is SMS type
							// then log message and skip it. Don't add it to queue
							if (DATA_SOURCE_LOCAL == DataSource && (IsSmsType(string)))
							{
							
								// 7/1/2016 - DE20747 - SOguz - There is SMS type pager for this 
								// profile along with other type of pager types
								// Skip the SMS type pager and log that we are skipping it. 
								// After that move to other pager types
								sprintf(msg1,"Page Server is in failsafe mode. Therefore, it is skipping SMS pager %s for profile %s.",
								string,
								page[PagePointer].Extension);

								sprintf(msg2, "");

								PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,								
								"PageAllDevices: %s",
								msg1);

								WriteMessage(PagePointer, page[PagePointer].Extension, "PG", msg1, msg2);							
							
							}
							else
							{
								if (result == TRUE)
								{
								if(AddQueueEx(PagePointer, page[PagePointer].PortNumber))
									{
									page[PagePointer].pageAdded = TRUE;
									i = TRUE;
									}
								result = FALSE;
								}
							}
						}
					else
						{
						UpdatePhoneLineStatusWindow( channel, "Q add not ready");
						sprintf(string2,
									"Line %i Q add not ready %s, %s, fh %i.",
									channel,
									page[PagePointer].Extension,
									page[PagePointer].PageType,
									page[PagePointer].PortNumber);

						WriteLog("NOPGABRT", string2);

						PrintMessage(MSGTYPE_IVR,
										hwndD4xMsg,
										string2);

						page[PagePointer].PageStatus[0] = 'C';
						i = TRUE;
						}					
					}
				}
				if(param.UserMsg[0] == 'E')
					{
					// When the PageServerConfig.exe, "Reporting Options", "Leave User Message" option 
					//  is set to "EXTERN", an "Xtend MessageBase" message will be written.
					WriteUserLogEx(PagePointer, page[PagePointer].PortNumber);
					}

				if(i == TRUE)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"PageAllDevices added all of the pages to the queue.");

					if(param.AnotherPage[0] == 0 ||
					param.AnotherPage[0] == ' ' ||
					port[channel].LoopCurrent == FALSE)
					{
						UpdatePhoneLineStatusWindow(channel, "Hanging Up");

						if(param.IVRPort[channel].iType != 3 &&
							param.IVRPort[channel].iType != 5)
							{
							port[channel].timer.wHour = 70;
							page[PagePointer].PortNumber = -1;
							errcode = XnSetHook(channel, DX_ONHOOK);
							}
					}
					else
					{
						port[channel].state = ST_ANOTHER;
						UpdatePhoneLineStatusWindow(channel, "Another Page?");
						port[channel].PortStatus = 1;
						errcode = get_digits(channel, 1);
					}
				}
				
			}

			StatusOn = TRUE; //turn on play back status option 
			isPageAllDevicesOn = FALSE; //turn off page all devices

			break;

        case ST_ADD_PAGE:
            // files exists, add record to queue file
            if(page[PagePointer].PriorityQPointer > 0)
				{
                i = FALSE;
                memset(string, 0, sizeof(string));
                sprintf(string, "%%%s", page[PagePointer].PageType);
                if(param.CodeQues[0] != 0)
					{
                    page[PagePointer].ComNumber = -1;
                    page[PagePointer].ScriptNumber = -1;
                    i = Get_Frq(PagePointer, param.CodeQues, string);
					}
                if(i == FALSE)
					{
                    Get_Frq(PagePointer, string, string);
					}
				}

			XnAutoFormat(PagePointer);
            AddSetUp(PagePointer, "External");

            UpdatePhoneLineStatusWindow(channel,
										"Add page %s",
										page[PagePointer].Extension);

			// in order to accept the request, it must be "deliverable", that is
			//  defined somewhere in the pageserver configuration. 
			// don't accept requests that cannot be delivered, or handed off for delivery.
			//
            page[PagePointer].PortNumber = GetQuePointer(PagePointer);
            i = FALSE;

            if((page[PagePointer].PortNumber >= 0) || IsSmsType(PagePointer))
				{
                if(AddQueueEx(PagePointer, page[PagePointer].PortNumber))
					{
					page[PagePointer].pageAdded = TRUE;
                    i = TRUE;
					}
				}
            else
				{
                UpdatePhoneLineStatusWindow( channel, "Q add not ready");
                sprintf(string2,
							"Line %i Q add not ready %s, %s, fh %i.",
							channel,
							page[PagePointer].Extension,
							page[PagePointer].PageType,
							page[PagePointer].PortNumber);

                WriteLog("NOPGABRT", string2);

                PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								string2);

                page[PagePointer].PageStatus[0] = 'C';
                i = TRUE;
				}
			//}

            if(param.UserMsg[0] == 'E')
				{
				// When the PageServerConfig.exe, "Reporting Options", "Leave User Message" option 
				//  is set to "EXTERN", an "Xtend MessageBase" message will be written.
				WriteUserLogEx(PagePointer, page[PagePointer].PortNumber);
				}

            if(i == TRUE)
				{
                if(param.AnotherPage[0] == 0 ||
					param.AnotherPage[0] == ' ' ||
					port[channel].LoopCurrent == FALSE)
					{
                    UpdatePhoneLineStatusWindow(channel, "Hanging Up");

                    if(param.IVRPort[channel].iType != 3 &&
						param.IVRPort[channel].iType != 5)
						{
                        port[channel].timer.wHour = 70;
                        page[PagePointer].PortNumber = -1;
						errcode = XnSetHook(channel, DX_ONHOOK);
						}
					}
                else
					{
                    port[channel].state = ST_ANOTHER;
                    UpdatePhoneLineStatusWindow(channel, "Another Page?");
                    port[channel].PortStatus = 1;
                    errcode = get_digits(channel, 1);
					}
				}
			break;

        case ST_NO_LOG_MSG:
			// file does not exist, inform the caller
            UpdatePhoneLineStatusWindow(channel, "No Beeper");

            Messages[ 0 ] = (char)( param.PromptType );		// "pager id"

            memset( string2, 0, sizeof( string2 ) );
            strcpy( string2, page[ PagePointer ].PagerId );

            if( (int)strlen( string2 ) > param.IdPromptLength )
				{
                memset( string2, 0, sizeof( string2 ) );
                strcpy( string2,
						page[ PagePointer ].PagerId +
						strlen( page[ PagePointer ].PagerId ) -
						param.IdPromptLength );
				}

            for(i = 0; i < 10 && string2[i] != 0; i++)
				{
                if( string2[ i ] > 47 && string2[ i ] < 58 )
                    Messages[ i + 1 ] = (char)( string2[ i ] - 48 );
                else
					break;
				}

            if( param.RetryId && page[ PagePointer ].attempts < 4 )
                Messages[ i + 1 ] = 17;		// "invalid.. please re-enter."
            else
                Messages[ i + 1 ] = 38;		// "has no pager at this time."

            port[ channel ].PortStatus = 1;
            page[ PagePointer ].PageStatus[0] = 'N';
            errcode = vb40_play( promptfh, channel, prompts, i + 2, Messages );
			break;

        case ST_LSTNG_MSG:
            UpdatePhoneLineStatusWindow( channel, "List Unavailable" );
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 60;
            for( i=0; i<sizeof( page[ PagePointer ].Extension ) && page[ PagePointer ].Extension[ i ] != 0; i++ )
				{
                if( page[ PagePointer ].Extension[ i ] > 47 &&
					page[ PagePointer ].Extension[ i ] < 58 )
                    Messages[ i + 1 ] = (char)( page[ PagePointer ].Extension[ i ] - 48 );
                else
					break;
            }
            Messages[ i+1 ] = 31;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, i+2, Messages );
			break;

        case ST_LSTOK_MSG:
            UpdatePhoneLineStatusWindow( channel, "List Verified" );
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 37;
            Messages[ 1 ] = 60;
            for( i=0; i<sizeof( page[ PagePointer ].PagerId ) && page[ PagePointer ].PagerId[ i ] != 0; i++ )
				{
                if( page[ PagePointer ].PagerId[ i ] > 47 && page[ PagePointer ].PagerId[ i ] < 58 )
                    Messages[ i + 2 ] = (char)( page[ PagePointer ].PagerId[ i ] - 48 );
                else
					break;
				}
            Messages[ i + 2 ] = 36;
            if( page[ PagePointer ].VoiceFile == 2 ) Messages[ i + 2 ] = 13;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, i+3, Messages );
			break;

        case ST_LST_RECORD:
            UpdatePhoneLineStatusWindow(channel, "List Msg Record");
            errcode = Voice_Record(channel);
            break;

        case ST_NO_VOICE:
            dx_clrdigbuf( chdev );

            Messages[ 0 ] = (char)( param.PromptType );		// "pager id"

            memset( string2, 0, sizeof( string2 ) );
            strcpy( string2, page[ PagePointer ].PagerId );

            if( (int)strlen( string2 ) > param.IdPromptLength )
				{
                memset(string2, 0, sizeof( string2 ) );
                strcpy(string2,
						page[PagePointer].PagerId +
						strlen(page[PagePointer].PagerId) -
						param.IdPromptLength);
				}

            for(i = 0; i < 10 && string2[i] != 0; i++)
				{
                if( string2[ i ] > 47 && string2[ i ] < 58 )
                    Messages[ i + 1 ] = (char)( string2[ i ] - 48 );
                else
					break;
				}

            Messages[ i+1 ] = 15;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, i+2, Messages );
			break;

        case ST_CB_VOICE:
        case ST_LST_SPK_MSG:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 36;
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
			break;

        case ST_GET_VOICE:
            errcode = Voice_Record(channel);

			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
							"next_state() - state[%d] beginning VoiceRecord() on channel [%d] errcode [%d]",
							port[ channel ].state,
							channel,
							errcode);
			break;

        case ST_GOODBYE_MSG:
        case ST_GOODBYE2_MSG:
			UpdatePhoneLineStatusWindow( channel, "Goodbye" );
            if( param.PromptId && param.CustPrompt == FALSE &&
				page[ PagePointer ].PageStatus[ 0 ] != 'L' &&
				port[ channel ].state == ST_GOODBYE_MSG )
				{
                Messages[ 0 ] = 37;								// "paging..."

                Messages[ 1 ] = (char) ( param.PromptType );	// "pager id"

                memset( string2, 0, sizeof( string2 ) );
                strcpy( string2, page[ PagePointer ].PagerId );

                if( (int)strlen( string2 ) > param.IdPromptLength )
					{
                    memset( string2, 0, sizeof( string2 ) );
                    strcpy( string2, page[ PagePointer ].PagerId + strlen( page[ PagePointer ].PagerId ) - param.IdPromptLength );
					}

                for(i = 0; i < 10 && string2[i] != 0; i++)
					{
                    if( string2[ i ] > 47 && string2[ i ] < 58 )
                        Messages[ i + 2 ] = (char)( string2[ i ] - 48 );
                    else break;
					}

                Messages[ i + 2 ] = VOX_PROMPT_thank_you_good_bye;		
				}
            else
				{
                Messages[ 0 ] = VOX_PROMPT_thank_you_good_bye;
                i = -2;
                if( param.CustPrompt && port[ channel ].state == ST_GOODBYE_MSG && param.PromptId )
					{
                    Messages[ 0 ] = 37;		// "paging"
					}
				}
            port[ channel ].PortStatus = 1;
            errcode = vb40_play( promptfh, channel, prompts, i + 3, Messages );
			break;

        case ST_GOODBYE3_MSG:
            Messages[0] = VOX_PROMPT_thank_you_good_bye; 
			i = 1;

            if(param.AnotherPage[0] != 0 && param.AnotherPage[0] != ' ')
				{
                Messages[0] = VOX_PROMPT_to_try_another_pager_i_d_press;
                Messages[1] = (char) (param.AnotherPage[0] - 48);
                if( param.AnotherPage[ 0 ] == '*' )
					Messages[ 1 ] = 10;
                if( param.AnotherPage[ 0 ] == '#' )
					Messages[ 1 ] = 11;
                i = 2;
                if( param.PageOpTransfer )
					{
                    Messages[ 2 ] = 24;
					i++;
					}
				}
            errcode = vb40_play( promptfh, channel, prompts, i, Messages );
            break;

        case ST_CB_MSG:
        case ST_PARK_MSG:
            k = 13;			// Enter your callback number followed by #
			memset(Messages, 0, sizeof(Messages));

            if(port[channel].state == ST_CB_MSG &&
				page[PagePointer].PageType[0] != 0)
				{
                memset(string2, 0, sizeof(string2));
                strncpy(string2, page[PagePointer].PageType, 2);
                string2[2] = 0;

                if(strstr(param.VoiceQues, string2) != NULL)
					{
                    k = 36;	//  Speak Now
					}
				}

			UpdatePhoneLineStatusWindow(channel, "Waiting Disp");
            if(param.CustPrompt == FALSE)
				{
				dx_clrdigbuf(chdev);
				}

            memset(string2, 0, sizeof(string2));

            strcpy(string2, page[PagePointer].FileName);

            if((page[PagePointer].ACKMin == -1 && string2[0] == 0) ||
				(StatusOn == FALSE && string2[0] == 0))
				{
                i = 1;
                if(param.IVRPort[channel].iType == 20)
					{
                    Messages[0] = 85;	// Press * to page your party and remain on the line
					}
                else
					{
                    Messages[0] = (char) k;

                    if(param.FlagParkOffset > 0  &&
						page[ PagePointer ].Flag1[ 0 ] == 'Y' &&
						Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'Y' )
						{
                        Messages[ i ] = 26;
						i++;	// or press

						// park password
                        for( j=0; j<( int )strlen( param.ParkPass ); j++ )
							{
                            if( param.ParkPass[ j ] > 47 && param.ParkPass[ j ]  < 58 )
								{
                                Messages[ i ] = (char)( param.ParkPass[ j ] - 48 );
                                i++;
								}
                            else if( param.ParkPass[ j ] == '*' )
								{
                                Messages[ i ] = 10; i++;	// star
								}
							}
                        Messages[ i ] = 11; i++;	// Pound sign
                        Messages[ i ] = 44; i++;	// to remain on the line while
                                                    //	your party is being paged
						}
                    if(page[ PagePointer ].Flag5[ 0 ] == 'Y' )
						{
                        Messages[ i ] = 26; i++;	// or press
						// voice mail password
                        for( j=0; j<( int )strlen( param.vmPassword ); j++ )
							{
                            if( param.vmPassword[ j ] > 47 && param.vmPassword[ j ]  < 58 )
								{
                                Messages[ i ] = (char)( param.vmPassword[ j ] - 48 );
                                i++;
								}
                            else if( param.vmPassword[ j ] == '*' )
								{
                                Messages[ i ] = 10; i++;
								}
                            else if( param.vmPassword[ j ] == '#' )
								{
                                Messages[ i ] = 11; i++;
								}
							}
						if( Messages[ i-1 ] != 11 )
							{
	                        Messages[ i ] = 11; i++;	// Pound sign
							}
                        Messages[ i ] = 46; i++;	// to leave a voice mail message
						}
					}
				}
            else
				{				
				bool tmpBool1 = (param.CoverPage && string2[0] != 0); 
				// 5/5/2017 - SOguz - Added isPageAllDevicesOn  and FollowPageAllDevicesCoverage 
				// as part of the condition to make sure we handle IVR coverage based on proper 
				// parameter settings
				bool tmpBool2 = (isPageAllDevicesOn == TRUE && FollowPageAllDevicesCoverage == TRUE);
				bool tmpBool3 = (isPageAllDevicesOn == FALSE);
				bool tmpBool2Or3 = (tmpBool2 || tmpBool3);

				// Commented out as Coverage and other Profile statuses are following IGNORE_STATUS_WHEN_PAGE_ALL_DEVICES ini setting now.
				/*
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"The FOLLOW_PAGE_ALL_DEVICES_COVERAGE parameter in XaPgSrv.ini is set to [%s].",
								FollowPageAllDevicesCoverage ? "TRUE" : "FALSE");
				*/

				
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"The IGNORE_COVERAGE_WHEN_PAGE_ALL_DEVICES parameter in XaPgSrv.ini is set to [%s].",
								ignoreCoverageWhenPageAllDevices ? "TRUE" : "FALSE");

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"The IGNORE_STATUS_WHEN_PAGE_ALL_DEVICES parameter in XaPgSrv.ini is set to [%s].",
								ignoreStatusWhenPageAllDevices ? "TRUE" : "FALSE");

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"The YNFLAG_NAME_TO_PAGE_ALL_DEVICES parameter in XaPgSrv.ini is set to [%s].",
								g_csYNFlagNameToPageAllDevices);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"The PageAllDevices is [%s] for extension [%s].",
								isPageAllDevicesOn ? "TRUE" : "FALSE",
								page[PagePointer].Extension);
				

				if (tmpBool1 && tmpBool2Or3)
				{				
					// page coverage applies.
					//
					PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"The extension [%s] is covered by [%s]. Paging covering extension [%s].",
								page[PagePointer].Extension,
								page[PagePointer].FileName,
								page[PagePointer].FileName);

					if(param.CustPrompt == FALSE)
						{
						Messages[0] = (char) param.PromptType;		// "Pager Id"

						for(i = 0; i < sizeof(page[PagePointer].PagerId) && page[PagePointer].PagerId[i] != 0; i++)
							{
							if( page[ PagePointer ].PagerId[ i ] > 47 && page[ PagePointer ].PagerId[ i ] < 58 )
								Messages[ i+1 ] = (char)( page[ PagePointer ].PagerId[ i ] - 48 );
							else
								break;
							}
						Messages[i + 1] = 58;	// Covered By
						for( j=0; j<(int)strlen( string2 ) && string2[ j ] != 0; j++ )
							{
							if( string2[ j ] > 47 && string2[ j ] < 58 )
								Messages[ j+i+2 ] = (char)( string2[ j ] - 48 );
							else
								break;
							}
						i = j + i + 2;
						}
					else
						{
						Messages[0] = 58;		// Covered By
						i = 1;
						}

					if(strncmp(page[PagePointer].FileName, "ON CALL", 7) == 0 ||
						strncmp(page[PagePointer].FileName, "ONCALL", 6) == 0)
						{
						PrintMessage(MSGTYPE_IVR,
										hwndD4xMsg,
										"Line %i On call check.",
										channel);

						// this call to GetFirstOnCall() is to determine whether
						//  the on-call schedule exists, and if it exists,
						//  the position of the schedule in the oncall table
						//  is stored in the "BTRIEVEUNION OnCallBtrvBuf" structure.
						if(GetFirstOnCall(&page[PagePointer].btrPosition,
											page[PagePointer].Extension,
											page[PagePointer].PageInfo))
							{
							// TRUE - on-call schedule is available
							page[PagePointer].PageStatus[0] = 'L';
							Messages[0] = (char) k;
							}
						else
							{
							// FALSE - on-call schedule NOT available
							Messages[0] = VOX_PROMPT_unavailable;
							}
						}
					else
						{
						port[channel].state = ST_GET_CVR_RES;
						}
					
				  }
                else
					{
                    i = 0;
                    UpdatePhoneLineStatusWindow(channel, "Status Prompt");

                    j = page[PagePointer].ACKMin;

					PrintMessage(MSGTYPE_IVR_PROGRESS,
									hwndD4xMsg,
									"ST_CB_MSG: page[PagePointer].ACKMin [%d]",
									page[PagePointer].ACKMin);
					// 7/5/2016 - SOguz - Added isPageAllDevicesOn as part of the condition to make sure
					// we don't get inside the if statement to play page status since we don't respect status
					// when page all devices turned ON
                    if(((Status[j].Page[0] == 'Y' && Status[j].Page[1] == 'E') ||
						Status[j].Page[0] != 'Y') && (isPageAllDevicesOn == FALSE))
						{
						// first 2 chars match 'Y' and 'E' or
						//  first char not equal 'Y'
						//  this is all except "Y S"
						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"ST_CB_MSG: param.PromptCurrent [%d]",
										param.PromptCurrent);
						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"ST_CB_MSG: Status[j].Page[0] [%d]",
										Status[j].Page[0]);
						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"ST_CB_MSG: Status[j].Page[1] [%d]",
										Status[j].Page[1]);

						// insert the phrase "current status is..."
						//  before the phrase for the page status.
						//
                        if(param.PromptCurrent)
							{
							Messages[i] = VOX_STATUS_current_status_is;
							i++;
							}

						// consult information that was read from NOPGSTAT file.
						// remember that the prompt number entered into .VAP files
						//  is '1' based and you must subtract 1 to get the proper voice prompt.
						//
                        Messages[i] = (char) (Status[page[PagePointer].ACKMin].Prompt - 1);

						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"1-ST_CB_MSG: Messages[i] [%d]",
										Messages[i]);

						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"5-ST_CB_MSG: PagePointer [%d]",
										PagePointer);

						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"6-ST_CB_MSG: page[PagePointer].ACKMin [%d]",
										page[PagePointer].ACKMin);

						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"7-ST_CB_MSG: Status[page[PagePointer].ACKMin].Prompt [%d]",
										Status[page[PagePointer].ACKMin].Prompt);

						PrintMessage(MSGTYPE_IVR_PROGRESS,
										hwndD4xMsg,
										"8-ST_CB_MSG: Status[page[PagePointer].ACKMin].Prompt - 1 [%d]",
										Status[page[PagePointer].ACKMin].Prompt - 1);

                        if(Messages[i] < 0)
							{
							Messages[i] = VOX_STATUS_invalid;
							PrintMessage(MSGTYPE_IVR_PROGRESS,
											hwndD4xMsg,
											"9-ST_CB_MSG: Messages[i] [%d]",
											Messages[i]);
							}
						i++;
						}
					else
						{
						// the one case excluded is "Y S" ('Y' <space> 'S')
						}

                    port[channel].PortStatus = 1;

 					PrintMessage(MSGTYPE_IVR_PROGRESS, hwndD4xMsg, "BEFORE  %d", StatusFh);

					errcode = vb40_play(StatusFh, channel, StatusPrompts, i, Messages);

 					PrintMessage(MSGTYPE_IVR_PROGRESS, hwndD4xMsg, "AFTER  %d", StatusFh);

                    port[channel].state = ST_CB_MSG2;

                    if(Messages[0] == VOX_STATUS_unavailable_on_pager)	// 36  (37 - 1)
						{
						port[channel].state = ST_CB_VOICE;
						}
                    break;
					}
				}

			// if the prompt in NOPGSTAT is set to "unavalable on page" number 37 (1 based)
			//  then this is a assumed to be a voice pager...
			//  this logic makes no sense..... 
			//  please do not use prompt #37 unless you understand this logic.
			//
            if(Messages[0] == VOX_STATUS_unavailable_on_pager)
				{
				port[channel].state = ST_CB_VOICE;
				}

            port[channel].PortStatus = 1;
            errcode = vb40_play(promptfh, channel, prompts, i, Messages);
			break;


        case ST_CB_MSG3:
			rv = VOX_PROMPT_please_dial_your_;	

            if(page[PagePointer].PageType[0] != 0)
				{
                memset(string2, 0, sizeof(string2));
                strncpy(string2, page[PagePointer].PageType, 2);
                string2[2] = 0;

                if(strstr(param.VoiceQues, string2) != NULL)
					{
                    rv = VOX_PROMPT_speak_now;  
					}
				}
            i = 1;

            if(20 == param.IVRPort[channel].iType)
				{
                Messages[0] = VOX_PROMPT_press_star_to_page_;
				}
            else
				{
                Messages[0] = (char) rv;

                if(page[PagePointer ].PageStatus[0] == 'M')
					{
                    if(param.AnotherPage[0] == 0 || param.AnotherPage[0] == ' ')
						{
                        if(Status[ page[ PagePointer].ACKMin].Page[0] == 'M')
                            Messages[0] = 13;   /* Dial your calback #  	*/
                        else
                            Messages[0] = 38;	  /* Has no pager at this time
														dial oper for further
														assistance 			*/
						}
                    else
						{
                        Messages[ 0 ] = VOX_PROMPT_to_try_another_pager_i_d_press;
                        Messages[ 1 ] = ( char )( param.AnotherPage[ 0 ] - 48 );
                        if( param.AnotherPage[ 0 ] == '*' ) Messages[ 1 ] = 10;
                        if( param.AnotherPage[ 0 ] == '#' ) Messages[ 1 ] = 11;
                        i = 2;
                        if( param.PageOpTransfer )
							{
                            Messages[2] = VOX_PROMPT_or_for_the_operator;
							i++;
							}
						}
					}
                if( param.FlagParkOffset > 0  &&
					page[ PagePointer ].Flag1[ 0 ] == 'Y' &&
					Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'Y' )
					{
                    i = 1;
                    Messages[i] = VOX_PROMPT_or_press_;
					i++;

                    for( j=0; j<( int )strlen( param.ParkPass ); j++ )
						{
                        if( param.ParkPass[ j ] > 47 && param.ParkPass[ j ]  < 58 )
							{
                            Messages[ i ] = (char)( param.ParkPass[ j ] - 48 );
                            i++;
							}
                        else if( param.ParkPass[ j ] == '*' )
							{
                            Messages[i] = VOX_PROMPT_star;
							i++;
							}
						}
                    Messages[i] = VOX_PROMPT_pound_sign;
					i++;

                    Messages[i] = VOX_PROMPT_to_remain_on_the_line_;
					i++;
					}

                if( page[ PagePointer ].Flag5[ 0 ] == 'Y' )
					{
                    Messages[ i ] = VOX_PROMPT_or_press_;
					i++;

					// voice mail password
                    for( j=0; j<( int )strlen( param.vmPassword ); j++ )
						{
                        if( param.vmPassword[ j ] > 47 && param.vmPassword[ j ]  < 58 )
							{
                            Messages[ i ] = (char)( param.vmPassword[ j ] - 48 );
                            i++;
							}
                        else if( param.vmPassword[ j ] == '*' )
							{
                            Messages[ i ] = 10; i++;
							}
                        else if( param.vmPassword[ j ] == '#' )
							{
                            Messages[ i ] = 11; i++;
							}
						}

					if(Messages[i - 1] != VOX_PROMPT_pound_sign)
						{
	                    Messages[i] = VOX_PROMPT_pound_sign;
						i++;
						}

                    Messages[i] = VOX_PROMPT_to_leave_a_voice_mail_;
					i++;
					}
				}
            if( param.IgnoreStatus == FALSE )
				{
                switch( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] )
					{
                    case 'N':
                    case ' ':
                    case '#':
                    case '0':
                    case 'V':
                        if( param.AnotherPage[ 0 ] == 0 || param.AnotherPage[ 0 ] == ' ' )
							{
                            if( param.StatusNgPrompt > 0 )
								{
                                Messages[ 0 ] = ( char )( param.StatusNgPrompt - 1 );
                                Messages[ 1 ] = 14;		/* thankyou goodbye */
                                i = 2;
								}
                            else
								{
                                Messages[ 0 ] = 14;		/* thankyou goodbye */
                                i = 1;
								}
							}
                        else
							{
                            Messages[ 0 ] = VOX_PROMPT_to_try_another_pager_i_d_press;
														
                            Messages[ 1 ] = ( char )( param.AnotherPage[ 0 ] - 48 );
                            if( param.AnotherPage[ 0 ] == '*' )
								Messages[ 1 ] = 10;		/* star */

                            if( param.AnotherPage[ 0 ] == '#' )
								Messages[ 1 ] = 11; 	/* pound sign */
                            i = 2;
                            if( param.PageOpTransfer ){
                                Messages[ 2 ] = 24; i++;	/* or for operator
																press 0		*/
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            switch( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] )
				{
                case 'F':
                case 'O':
                    i = 0;
                    memset(string2, 0, sizeof(string2));
                    k = 0;

					// xxx
                    //for(j = 0; j < 15; j++)
                    for(j = 0; j < MAX_DIGITS_IN_REFERRED_TO_NUMBER; j++)
						{
                        if( page[ PagePointer ].PageInfo[ j ] > 47 && page[ PagePointer ].PageInfo[ j ] < 58 )
							{
                            string2[ k ] = (char)( page[ PagePointer ].PageInfo[ j ] );
                            k++;
							}
                        else if( param.StarPrompt > 1 && page[ PagePointer ].PageInfo[ j ] == '*' )
							{
                            string2[k] = (char) (param.StarPrompt - 1); 
                            //string2[k] = (char) VOX_PROMPT_star; 
							k++;
							}
                        else if( page[ PagePointer ].PageInfo[ j ] != '-' )
                            break;
						}

                    for(k = 0; k < 2; k++)
						{
                        Messages[ i ] = 61;
                        if( Status[ page[ PagePointer ].ACKMin ].Page[ 0 ] == 'O' ) Messages[ i ] = 43;
                        i++;
                        for( j=0; j<(int)strlen( string2 ) && string2[ j ] != 0; j++ )
							{
                            if( string2[ j ] > 47 && string2[ j ] < 58 )
                                Messages[ i ] = (char)( string2[ j ] - 48 );
                            else
								{
                                Messages[ i ] = (char)( string2[ j ] );
								}
                            i++;
							}
						}

                    if(param.StatusFdPrompt > 0)
						{
                        Messages[i] = (char) (param.StatusFdPrompt - 1);
                        i++;
						}
                    Messages[i] = 14;
					i++;

                default:
                    break;
				}

            port[channel].PortStatus = 1;

 
			// RECORD A VOICE PROMPT ... WHY, BECAUSE USING PROMPT 36 ??? COMPLETE NONSENSE LOGIC!
			//  UWHC an un-necessary voice recording is made when page holder status is Unavailable.
			//  a fix for this is to use prompt #26 instead of #36 (zero based numbers)
           if(Messages[0] == VOX_PROMPT_speak_now)
				{
				port[channel].state = ST_CB_VOICE;
				}

            errcode = vb40_play(promptfh, channel, prompts, i, Messages);
            break;

		// announce impairment....
		//
		case ST_IMPAIRMENT_MSG:
			// present a ProfileId, and FileExtension, 
			//  and receive a full path and filename to use.
			//  the file should exist, it was checked before entering this state.
			{
			CString csFileName;
			CString PageDeviceType;
			PageDeviceType.Format("%%%s", page[PagePointer].PageType);

			csFileName = g_IMP.GetCustomGreetingProfileId(PageDeviceType);

			char FileName[MAX_PATH];
			sprintf(FileName, "%s", (LPTSTR) (LPCTSTR) csFileName);
			getfile(FileName, "VCE", MAX_PATH);

			// parameter #4 of PlayFil is StopPlayingOnDtmfToneReceived.
			PlayFil(channel, FileName, &port[channel].file_handle, 0);
			}
			break;

         case ST_CCPG_CALL:
            port[ channel ].timer.wHour = 70;
            page[ PagePointer ].ScriptFlag++;
            page[ PagePointer ].PageStatus[ 0 ] = 'B';
            errcode = CheckScript( channel );
			break;

        case ST_CCPG_ERR:
			termType = ATDX_CPTERM( chdev );

			PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i termtype: %i, %ld Hz",
							channel,
							termType,
							ATDX_FRQHZ( chdev ) );

            UpdatePhoneLineStatusWindow( channel, "CPA ERROR: On Hk" );
            page[ PagePointer ].PageStatus[0] = 'A';
            port[ channel ].PortStatus = 1;
            port[ channel ].timer.wHour = 70;
            port[ channel ].Errors++;
			errcode = XnSetHook( channel, DX_ONHOOK );
			break;

        case ST_VPAGE_DONE:
        case ST_ONHK:
			// go on hook and terminate the call
            dx_clrdigbuf( chdev );
			port[ channel ].timer.wHour = 70;
			UpdatePhoneLineStatusWindow( channel, "On Hook" );

            PrintMessage(MSGTYPE_IVR,
							hwndD4xMsg,
							"Line %i going on hook",
							channel );

			errcode = XnSetHook( channel, DX_ONHOOK );

            if( port[ channel ].PortStatus == 2 )
				{
				termType = ATDX_CPTERM( port[channel].d4xHandle );

				PrintMessage(MSGTYPE_IVR,
								hwndD4xMsg,
								"Line %i termtype: %i, %ld Hz",
								channel,
								termType,
								ATDX_FRQHZ(port[channel].d4xHandle) );
				}

            port[ channel ].PortStatus = 0;
			break;

        case ST_NAME_MSG:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = VOX_PROMPT_press_the_first;
			Messages[ 1 ] = (char) param.NameIdLength;
			Messages[ 2 ] = VOX_PROMPT_digits_of_the_persons_last_name;
            errcode = vb40_play( promptfh, channel, prompts, 3, Messages );
            break;

        case ST_NAME_GET:
            UpdatePhoneLineStatusWindow( channel, "Receiving Name" );
            errcode = get_digits( channel, param.NameIdLength );
			break;

        case ST_NAME_SELECT:
            j = 0;
            if( page[ PagePointer ].NextAckPtr == 0 )
                errcode = 0;
            else
                errcode = 1;
            while( page[ PagePointer ].NextAckPtr < (unsigned)page[ PagePointer ].ACKMin )
				{
                memset( string2, 0, sizeof( string2 ) );
                memset( page[ PagePointer ].Extension, 0, sizeof( page[ PagePointer ].Extension ) );
                k = 0;

                for(i = 0; i < 10; i++)
					{
                    if( page[ PagePointer ].tempStr2[ page[ PagePointer ].NextAckPtr * 10 + i ] != ' ' )
						{
                        page[ PagePointer ].Extension[ k ] =
							page[ PagePointer ].tempStr2[ page[ PagePointer ].NextAckPtr * 10 + i ];
                        k++;
						}
					}

				k = page[ PagePointer ].ACKMin;
                i = GetPagerId( page[ PagePointer ].Extension, PagePointer, BY_PAGERID);
				page[ PagePointer ].ACKMin = k;
                if( i == TRUE )
					{
                    strcpy( string2, page[ PagePointer ].Extension );
                    getfile( string2, "VCE", sizeof(string2));
                    rv = GetFileAttributes( string2 );
                    if( rv != 0xFFFFFFFF ){
                        j = 1;
                        Messages[ 0 ] = 53;
                        Messages[ 1 ] = (char)( page[ PagePointer ].NextAckPtr + 1 );
                        Messages[ 2 ] = 22;
                        errcode = vb40_play( promptfh, channel, prompts, 3, Messages );
                        break;
						}
					}
                page[ PagePointer ].NextAckPtr++;
				}
            if( j==0 && errcode == 0 )
				{
                UpdatePhoneLineStatusWindow( channel, "Op Trnsfr Msg" );
                dx_clrdigbuf( chdev );
                Messages[ 0 ] = 98;
                errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
                port[ channel ].state = ST_OP_TRANSFER;
				}
            else if ( j == 0 )
				{
				Messages[ 0 ] = 53;
                Messages[ 1 ] = 10;
                Messages[ 2 ] = 66;
                errcode = vb40_play( promptfh, channel, prompts, 3, Messages );
				}
            break;

        case ST_NAME_MSG3:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = 99;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

		case ST_STAT_M_FORWARD:
			Messages[0] = VOX_PROMPT_please_enter;
			Messages[1] = VOX_PROMPT_pager_I_D;
            errcode = vb40_play( promptfh, channel, prompts, 2, Messages );
			break;

		case ST_STAT_M_EMAIL:
			//play message
			Messages[0] = VOX_PROMPT_please_enter;
			Messages[1] = VOX_PROMPT_pager_I_D;
            errcode = vb40_play( promptfh, channel, prompts, 2, Messages );
			break;

		case ST_GET_FORWARD_EXT:
		case ST_GET_EMAIL_EXT:
           errcode = dx_clrdigbuf( channel );

			UpdatePhoneLineStatusWindow( channel, "Forward #" );
            port[ channel ].PortStatus = 1;
            errcode = get_digits( channel, param.IdLength );

			break;

		 //01/14/2016 - DE20568 - SOguz - Defining new state to play  
		 //"sorry, unable to page" when the caller tries to send a message 
		 //to SMS (Spok Mobile) type pagers in Failsafe mode.
		 case ST_SMS_FS_MSG:
            dx_clrdigbuf( chdev );
            Messages[ 0 ] = VOX_PROMPT_sorry_unable_to_connect_;			
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );				
            break;

		case ST_ONHK_DELAY:
        case ST_STAT_M_SRCH:
        case ST_STAT_M_NO:
        case ST_STAT_M_NO2:
        case ST_VPAGE_TALK:
        case ST_VP_IDLE:
        default:
            break;
		}
	return(errcode);
	}

