
// xnpgd4xb.cpp


#include "common.h"

#include <time.h>
#include <fcntl.h>
#include <sys\stat.h>

// disable deprecation
#pragma warning(disable:4996)

// This function is used to decide wether the system is to use a
//  menu for end users. Not necessary if only one option is available.
int CheckMenu(void)
{
    int i;
    int j;
	
    for(i=0, j=0; i < 12; ++i)
	{
        if((param.StatPress[i][0] != ' ') && (param.StatPress[i][0] != 0))
		{
			if(j) return TRUE;
			++j;
		}
	}
    return FALSE;
}


	int
ConvertNumeric(char * Text, char * Msgs)
	{
	int i;
	int j;
	int l;
	int numStart = 0;

    i = atoi(Text);

    l = i / 1000;                     		/*  1000 's portion 		*/
    if( l > 0 )
		{
        Msgs[ numStart ] = (char)( l );
        numStart++;
        Msgs[ numStart ] = (char)( 29 );
        numStart++;
		}

    l = ( i - ( i / 1000 ) * 1000 ) / 100;	/*  100 's portion			*/

    if( l > 0 )
		{
        Msgs[ numStart ] = (char)( l );
        numStart++;
        Msgs[ numStart ] = (char)( 28 );
        numStart++;
		}

    l = ( i - ( i / 100 ) * 100 );		/*  10 's & 1's portion			*/

    if( l < 21 )
		{
		if( l > 0 || numStart == 0 )
			{
        	Msgs[ numStart ] = ( char )l;
        	numStart++;
			}
		}
	else
		{
        l = ( i - ( i / 100 ) * 100 ) / 10;		/*  10 's portion			*/

        if( l > 0 )
			{
            Msgs[ numStart ] = (char)( l + 18 );
            numStart++;
			}

        j = i - ( ( i / 10 ) * 10 );    /*  1 's portion                	*/

        if( j > 0 )
			{
            Msgs[ numStart ] =( char )j;
            numStart++;
			}
		}
	return( numStart );
	}


	void
ResetBuffer(int i)
	{
	// at the beginning of a page operation, clear out
	//  selected information in page[i] PAGE_STRUCT structure.

    page[i].PriorityQPointer = 0;
    page[i].attempts = 0;
	page[i].pageAdded = FALSE;
	page[i].pageFormatted = FALSE;
	page[i].VoiceFile = FALSE;
	page[i].tempInt = 0;

    memset(page[i].DisplayInfo,			0, sizeof(page[i].DisplayInfo));
    memset(page[i].AutCode,				0, sizeof(page[i].AutCode));
    memset(page[i].PagerNumber,			0, sizeof(page[i].PagerNumber));
    memset(page[i].PageType,			0, sizeof(page[i].PageType));
    memset(page[i].PagerId,				0, sizeof(page[i].PagerId));
    memset(page[i].InitialPagerId,		0, sizeof(page[i].InitialPagerId));
    memset(page[i].InitialExtension,	0, sizeof(page[i].InitialExtension));
    memset(page[i].Extension,			0, sizeof(page[i].Extension));
    memset(page[i].InitiatorId,			0, sizeof(page[i].InitiatorId));
    memset(page[i].FileName,			0, sizeof(page[i].FileName));
    memset(page[i].PageInfo,			0, sizeof(page[i].PageInfo));
	page[i].longMessage.erase();
	}


	int
XnSetHook(int channel, int hookstate)
	{
	int errcode = -1;

	switch(hookstate)
		{
		case DX_OFFHOOK:
			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "XnSetHook(): Line [%i] DX_OFFHOOK command", channel);

			if( port[ channel ].t1 == FALSE )
				{
				errcode = dx_sethook( port[ channel ].d4xHandle, DX_OFFHOOK, EV_ASYNC );
				}
			else
				{
				errcode = dt_settssigsim( port[ channel ].d4xT1Handle, param.t1CmdOffHook);
				if( errcode == 0 )
					{
					if( param.t1OffHookPause == 0 )
						{
						SetNextD4xState(channel, NULL);
						}
					else
						{
						port[ channel ].timer.wHour = 70;
						port[ channel ].state = ST_T1_PAUSE;
						}
					}
				}
			break;

		case DX_ONHOOK:
			PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "XnSetHook(): Line [%i] DX_ONHOOK command", channel);

			dx_stopch(port[channel].d4xHandle, EV_ASYNC);
			Sleep(100);

			port[ channel ].timer.wHour = 70;

			if( port[ channel ].t1 == FALSE )
				{
				// analog line type  on-hook
				switch(param.IVRPort[channel].iType)
					{
					case 3:
					case 5:
						errcode = dx_sethook( port[channel].d4xHandle, DX_OFFHOOK, EV_ASYNC );
						break;

					case 21:
					case 27:
						errcode = dx_dial( port[channel].d4xHandle, "&", (DX_CAP*)NULL, EV_ASYNC );
						break;

					default:
						errcode = dx_sethook( port[channel].d4xHandle, DX_ONHOOK, EV_ASYNC );
						break;
					}
				}
			else
				{
				// T1 line type on-hook
				errcode = dt_settssigsim( port[ channel ].d4xT1Handle, param.t1CmdOnHook);
				if( errcode == 0 )
					SetNextD4xState(channel, NULL);
				}

			dx_clrdigbuf( port[channel].d4xHandle );
			break;
		}

	if( errcode != 0 )
		{
		// non-zero error code, there was an error hanging up.
        PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg,
						"Line %i, State %i, Hangup Error: %s.",
						channel,
						port[ channel ].state,
						ATDV_ERRMSGP( port[channel].d4xHandle ) );
		}

	return errcode;
	}


	int
XnTransfer(int channel, char *transferString, DX_CAP *channelParam)
	{
	int		errcode;
	int		i;
	int		j;
	int		k;
	char	*tempPos;
	char	tempXferString[ 50 ];

	PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
				"XnTransfer(): channel[%d] transferString[%s]",
				channel,
				transferString);

	if(port[ channel ].t1 == FALSE)
		{
		// Analog line.
		if(channelParam == NULL)
			{
			errcode = dx_dial(port[channel].d4xHandle, transferString, (DX_CAP*)NULL, EV_ASYNC);
			PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i, Dial CP Transfer: %s.", channel, transferString );
			}
		else
			{
			errcode = dx_dial(port[channel].d4xHandle, transferString, channelParam, DX_CALLP|EV_ASYNC);
			PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i, Dial Transfer: %s.", channel, transferString );
			}
		page[ channel ].tempInt = -1;
		}
	else
		{
		// T1 line.
		memset( tempXferString, 0, sizeof( tempXferString ) );

		tempPos = strchr( transferString + page[ channel ].tempInt, '&' );
		if( tempPos != NULL )
			{
			// processing when: tempPos != NULL

			i = tempPos - transferString;
			if( i == page[ channel ].tempInt )
				{
				PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i, T1 Transfer Wink", channel );

				errcode = dt_xmitwink( port[ channel ].d4xT1Handle, 0 );
				page[ channel ].tempInt = i + 1;
				}
			else
				{
				k = 0;
				for( j=page[ channel ].tempInt; j<i; j++ )
					{
					tempXferString[ k ] = transferString[ j ];
					k++;
					}
				PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i, T1 Dial Transfer: %s.", channel, tempXferString );

				errcode = dx_dial( port[channel].d4xHandle, tempXferString, (DX_CAP*)NULL, EV_ASYNC );
				page[ channel ].tempInt = i;
				}
			}
		else
			{
			// processing when: tempPos == NULL

			strcpy( tempXferString, transferString + page[ channel ].tempInt );
			page[ channel ].tempInt = -1;
			if( channelParam == NULL )
				{
				PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i, T1 Dial Transfer: [%s]", channel, tempXferString );
				errcode = dx_dial( port[channel].d4xHandle, tempXferString, (DX_CAP*)NULL, EV_ASYNC );
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg, "Line %i, T1 Dial CP Transfer: [%s]", channel, tempXferString );
				errcode = dx_dial( port[channel].d4xHandle, tempXferString, channelParam, DX_CALLP|EV_ASYNC );
				}
			}
		}

	if( errcode != 0 )
		{
        PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
						"Line %i, State %i, Transfer Error: %s.",
						channel,
						port[ channel ].state,
						ATDV_ERRMSGP( port[channel].d4xHandle ) );
		}
	return errcode;
	}


	int
PlayFil(int channel, char *FileName, int *fh, int StopPlayingOnDtmfToneReceived)
	{
	int i;
	static DV_TPT tpt[MAX_IVRPORTS + 1][4];
	static DX_XPB xpb[MAX_IVRPORTS + 1];

	CString csFileName; csFileName = FileName;


	// truncate the string at the first character that does not belong in a file path name.
	//  this happens when covering on voice pagers.
	//
	int iCount;

	if((iCount = csFileName.FindOneOf("\r\n ")) > 0)
		{
		// take the left portion of the string upto the point of the "bad" character.
		csFileName = csFileName.Left(iCount);
		}

    *(fh) = dx_fileopen((LPTSTR) (LPCTSTR) csFileName, O_BINARY | O_RDONLY);

	if(*(fh) > 0)
		{
		indexplay[channel][0].io_type = IO_DEV | IO_EOT;
		indexplay[channel][0].io_bufp = 0;
		indexplay[channel][0].io_nextp = NULL;
		indexplay[channel][0].io_prevp = NULL;
		indexplay[channel][0].io_fhandle = *(fh);
		indexplay[channel][0].io_offset = 0;
		indexplay[channel][0].io_length = -1;

		if(param.waveFileFormat)
			{
			xpb[channel].wFileFormat = FILE_FORMAT_WAVE;
			xpb[channel].wDataFormat = DATA_FORMAT_PCM;
			xpb[channel].nSamplesPerSec = DRT_8KHZ;
			xpb[channel].wBitsPerSample = 8;
			}
		else
			{
			xpb[channel].wFileFormat = FILE_FORMAT_VOX;
			xpb[channel].wDataFormat = DATA_FORMAT_DIALOGIC_ADPCM;
			xpb[channel].nSamplesPerSec = DRT_6KHZ;
			xpb[channel].wBitsPerSample = 4;
			}

		// setup to initialize a set of 4 tpt blocks.
		tpt[channel][0].tp_type = IO_CONT;
		tpt[channel][1].tp_type = IO_CONT;
		tpt[channel][2].tp_type = IO_CONT;
		tpt[channel][3].tp_type = IO_EOT;

		// initialize 4 tpt blocks using the approved dialogic function.
		dx_clrtpt(&tpt[channel][0], 4);

		// first block is always a timeout on silence block.
		i = 0;
		tpt[channel][i].tp_type = IO_CONT;
		tpt[channel][i].tp_termno = DX_MAXSIL;
		tpt[channel][i].tp_length = 6000;
		i++;

		// if commanded, setup a stop on touchtone detect block.
		if(StopPlayingOnDtmfToneReceived)
			{
			// setup a tpt block to stop on any digit
			tpt[channel][i].tp_type = IO_CONT;
			tpt[channel][i].tp_termno = DX_MAXDTMF;
			tpt[channel][i].tp_length = 1;
			tpt[channel][i].tp_flags = TF_MAXDTMF;
			i++;
			}

		if((param.IVRPort[channel].iType == 21 ||		// 21 == DPG == DID40 Page In Port
				param.IVRPort[channel].iType == 27 ||	// 27 == DST == DID40 Page Status Port
				param.DetectCurrent) &&
				param.IVRPort[channel].iType != 6)		//  6 == VAO == Voice Audio Out Port
			{
			tpt[channel][i].tp_type = IO_CONT;
			tpt[channel][i].tp_termno = DX_LCOFF;
			i++;
			}

//		if( param.GTD.freq1 > 0 &&
//			param.IVRPort[channel].iType != 6)			// 6 == VAO port
//			{
//			tpt[channel][i].tp_type = IO_CONT;
//			tpt[channel][i].tp_termno = DX_TONE;
//			tpt[channel][i].tp_length = (unsigned short) param.GTD.tid;
//			tpt[channel][i].tp_data = DX_TONEON|DX_TONEOFF;
//			i++;
//			}

		// 2006-02-13 - transferred the following from 
		//  ver. 3-10-0080 to support CATS in 4-xx-xxxx.
		//
		// 2003-07-29 - kgill
		//
		// cats problem
		//  z15 problem,
		// do not detect a burst of dialtone, that may be at the end of a recording.
		//  if detection occurrs, a hangup event will occurr.
		//

		// 6  == VAO port
		// 16 == RST port
		// 22 == RS2 port

		if(param.GTD.freq1 > 0)
			{
			// global tone detect for dialtone is enabled.

			// when playing a recorded file,
			//  a burst of dialtone at the end of the recording,
			//  could be considered a disconnect signal.
			//
			// don't use this detection feature if the line is a VAO
			//  port, or if it is a CATS port usually RST, or RS2.
			//
			if((param.IVRPort[channel].iType == 6) ||
				(param.IVRPort[channel].iType == 22) ||
				(param.IVRPort[channel].iType == 16))
				{
				// do nothing
				}
			else
				{
				// feature allowed, setup as follows:
				tpt[channel][i].tp_type = IO_CONT;
				tpt[channel][i].tp_termno = DX_TONE;
				tpt[channel][i].tp_length = (unsigned short) param.GTD.tid;
				tpt[channel][i].tp_data = DX_TONEON|DX_TONEOFF;
				i++;
				}
			}

		tpt[channel][i - 1].tp_type = IO_EOT;

		return(dx_playiottdata(port[channel].d4xHandle,
								&indexplay[channel][0],
								&tpt[channel][0],
								&xpb[channel],
								EV_ASYNC));
		}
	return -1;
	}


	int
record(int channel, int fh, int duration)
	{
	int i;
    unsigned short mode = RM_TONE | EV_ASYNC;		// use AGC (default is MD_GAIN)
	static DV_TPT tpt[MAX_IVRPORTS + 1][5];
	static DX_XPB xpb[MAX_IVRPORTS + 1];

	if(fh <= 0) 
		{
		return -1;
		}

	indexplay[channel][0].io_type = IO_DEV|IO_EOT;
	indexplay[channel][0].io_bufp = 0;
	indexplay[channel][0].io_nextp = NULL;
	indexplay[channel][0].io_fhandle = fh;
	indexplay[channel][0].io_offset = 0;
	indexplay[channel][0].io_length = 100000;

	if( param.waveFileFormat )
		{
		xpb[channel].wFileFormat = FILE_FORMAT_WAVE;
		xpb[channel].wDataFormat = DATA_FORMAT_PCM;
		xpb[channel].nSamplesPerSec = DRT_8KHZ;
		xpb[channel].wBitsPerSample = 8;
		}
	else
		{
		xpb[channel].wFileFormat = FILE_FORMAT_VOX;
		xpb[channel].wDataFormat = DATA_FORMAT_DIALOGIC_ADPCM;
		xpb[channel].nSamplesPerSec = DRT_6KHZ;
		xpb[channel].wBitsPerSample = 4;
		}

	tpt[channel][ 0 ].tp_type = IO_CONT;
	tpt[channel][ 1 ].tp_type = IO_CONT;
	tpt[channel][ 2 ].tp_type = IO_CONT;
	tpt[channel][ 3 ].tp_type = IO_CONT;

	tpt[channel][ 4 ].tp_type = IO_EOT;
	dx_clrtpt( &tpt[channel][0], 5);

	i = 0;

	tpt[channel][ i ].tp_type = IO_CONT;
	tpt[channel][ i ].tp_termno = DX_MAXSIL;

	// dialogic units are 100 ms,
	//  xtend parameter is specified in unit seconds.
	tpt[channel][ i ].tp_length = (unsigned short) (param.MaxSilence * 10);
	tpt[channel][i].tp_flags = TF_MAXSIL;
	i++;

	tpt[channel][ i ].tp_type = IO_CONT;
	tpt[channel][ i ].tp_termno = DX_MAXTIME;
	tpt[channel][ i ].tp_length = (unsigned short) duration;
	tpt[channel][ i ].tp_flags = TF_MAXTIME;
	i++;

	tpt[channel][ i ].tp_type = IO_CONT;
	tpt[channel][ i ].tp_termno = DX_MAXDTMF;
	tpt[channel][ i ].tp_length = 1;
	tpt[channel][ i ].tp_flags = TF_MAXDTMF;
	i++;

	if(param.IVRPort[channel].iType == 21 ||
		param.IVRPort[channel].iType == 27 ||
		param.DetectCurrent)
		{
		tpt[channel][ i ].tp_type = IO_CONT;
		tpt[channel][ i ].tp_termno = DX_LCOFF;
		i++;
		}

	if( param.GTD.freq1 > 0 )
		{
		tpt[channel][ i ].tp_type = IO_CONT;
		tpt[channel][ i ].tp_termno = DX_TONE;
		tpt[channel][ i ].tp_length = (unsigned short) param.GTD.tid;
		tpt[channel][ i ].tp_data = DX_TONEON|DX_TONEOFF;
		i++;
		}

	tpt[channel][ i-1 ].tp_type = IO_EOT;

    return(dx_reciottdata( port[channel].d4xHandle,
							&indexplay[channel][0],
							&tpt[channel][0],
							&xpb[channel],
							mode));
	}


/****************************************************************
*        NAME : get_digits(channel, maxdig)
* DESCRIPTION : initiate the get dtmf string function
*       INPUT : channel = channel number
*             : buffer = pointer to the dtmf buffer
*             : maxdig = max number of dtmf digits requested
*      OUTPUT : d40drv will move the digits to *buffer
*     RETURNS : pass/fail status of the getdtmfs function
****************************************************************/
	int
get_digits(int channel, short maxdigits)
	{
	int i;
	static DV_TPT tpt[MAX_IVRPORTS + 1][6];

	tpt[channel][0].tp_type = IO_CONT;
	tpt[channel][1].tp_type = IO_CONT;
	tpt[channel][2].tp_type = IO_CONT;
	tpt[channel][3].tp_type = IO_CONT;
	tpt[channel][4].tp_type = IO_CONT;
	tpt[channel][5].tp_type = IO_EOT;
	dx_clrtpt(&tpt[channel][0], 6);
	i = 0;

	if( maxdigits > MAXDIG )
		{
		maxdigits = MAXDIG;
		}

	tpt[channel][ i ].tp_type = IO_CONT;
	tpt[channel][ i ].tp_termno = DX_MAXDTMF;
	tpt[channel][ i ].tp_length = maxdigits;
	tpt[channel][ i ].tp_flags = TF_MAXDTMF;

	i++;
	tpt[channel][ i ].tp_type = IO_CONT;
	tpt[channel][ i ].tp_termno = DX_MAXTIME;
	tpt[channel][ i ].tp_length = 200;
	i++;
	tpt[channel][ i ].tp_type = IO_CONT;
	tpt[channel][ i ].tp_termno = DX_MAXSIL;
	tpt[channel][ i ].tp_flags = TF_MAXSIL;
	tpt[channel][ i ].tp_length = (unsigned short) (param.TouchToneTimeOut * 10);
	i++;
    if( maxdigits > 1 )
		{
		tpt[channel][ i ].tp_type = IO_CONT;
		tpt[channel][ i ].tp_termno = DX_DIGMASK;
		tpt[channel][ i ].tp_length = DM_P;
		tpt[channel][ i ].tp_flags = TF_DIGMASK;
		i++;
		}

	if(param.IVRPort[channel].iType == 21 ||
		param.IVRPort[channel].iType == 27 ||
		param.DetectCurrent)
		{
		tpt[channel][ i ].tp_type = IO_CONT;
		tpt[channel][ i ].tp_termno = DX_LCOFF;
		i++;
		}

	if( param.GTD.freq1 > 0 )
		{
		tpt[channel][ i ].tp_type = IO_CONT;
		tpt[channel][ i ].tp_termno = DX_TONE;
		tpt[channel][ i ].tp_length = (unsigned short) param.GTD.tid;
		tpt[channel][ i ].tp_data = DX_TONEON|DX_TONEOFF;
		i++;
		}
	tpt[channel][ i-1 ].tp_type = IO_EOT;

	PrintMessage(MSGTYPE_SYSTEM,  hwndD4xMsg,
					"Line %i, Handle %i Get %i Digits",
					channel,
					port[channel].d4xHandle,
					maxdigits );

	return(dx_getdig(port[channel].d4xHandle,
						&tpt[channel][0],
						&port[channel].digBuf,
						EV_ASYNC));
	}


	void
GetDigitBuffer(int channel)
	{
	int i;
	int j;
	int numDigits;

	j = 0;
	memset(port[channel].digsave, 0, sizeof(port[channel].digsave));
	numDigits = strlen(port[channel].digBuf.dg_value);

	for(i = 0; i < numDigits; i++)
		{
		port[channel].digsave[j] = port[channel].digBuf.dg_value[i];
		j++;
		}

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndD4xMsg,
					"Line [%i], Digits received: [%s]",
					channel,
					port[channel].digsave);

	// see project worksheet Sequence 0043, and 0050.
	//  logic sense was backwards in version 3-00-0030, corrected in 3-00-0031.
	if(param.FlushDigitBuffer)
		{
		// don't allow dial-ahead on "Covered By" pagers,

		// after digits are read, clear out the firmware digit buffer,
		//  so that its contents cannot be re-read as if
		//  it is newly received data.

		// this option solved the problem of "phantom pages" at MASCO.
		dx_clrdigbuf(port[channel].d4xHandle);

		// terminate the source string at position 0,
		//  thereby wiping out the received chars.
		//  so that they cannot be retrieved again,
		port[channel].digBuf.dg_value[0] = 0;

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndD4xMsg,
						"Line [%i], firmware, and DV_DIGIT buffers ARE cleared ",
						channel);
		}
	else
		{
		// allow dial-ahead on "Covered By" pagers,
		//  by not clearing the digit buffer.
		//  this was requested by customers:
		//  Main-Line Health, and Medical College of Central Georgia
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndD4xMsg,
						"Line [%i], firmware, and DV_DIGIT buffers NOT cleared ",
						channel);
		}
	}


	int
Voice_Record(int channel)
	{
	SYSTEMTIME stTime;
    unsigned rv = 0;
    int errcode;
    int PagePointer;
    char string[80];
    char fname2[80];

    PagePointer = port[channel].PageNumber;

 	// get local time into a SYSTEMTIME structure
	GetLocalTime(&stTime);

    memset(string, 0, sizeof(string));
    memset(fname2, 0, sizeof(fname2));

	sprintf(fname2,
			"%s%02d%02d%04d.VPO",
			pageFileDir,
			stTime.wMonth,
			stTime.wDay,
			VoiceFileCounter);

    rv = GetFileAttributes(fname2);

    while(rv != 0xFFFFFFFF && VoiceFileCounter < 9999)
		{
        VoiceFileCounter++;
		memset(fname2, 0, sizeof(fname2));
		sprintf(fname2,
				"%s%02d%02d%04d.VPO",
				pageFileDir,
				stTime.wMonth,
				stTime.wDay,
				VoiceFileCounter );

		rv = GetFileAttributes(fname2);
		}

    page[PagePointer].VoiceFile = VoiceFileCounter;

    VoiceFileCounter++;

    if(VoiceFileCounter > 9999)
		{
		// overflow, back to low value.
		VoiceFileCounter = 1;
		}

    strcpy(page[PagePointer].FileName, fname2);

    if(port[channel].file_handle > 0)
		{
        dx_fileclose(port[channel].file_handle);
        port[channel].file_handle = 0;
		}

    port[channel].file_handle = dx_fileopen(fname2,
											O_CREAT | O_BINARY | O_TRUNC | O_RDWR,
											S_IREAD | S_IWRITE);

    dx_clrdigbuf(port[channel].d4xHandle);
    UpdatePhoneLineStatusWindow(channel, "Voice Page Rec");

    GetLocalTime(&port[channel].timer);
    errcode = record(channel, port[channel].file_handle, param.voicePageTime - 1);

    PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
					"Line [%i] Recording voice file: [%s], FH: [%i].",
					channel,
					fname2,
					port[channel].file_handle);

    return(errcode);
	}


	// add to a vector of characters
	//  each character added to the vector represents the number
	//  of a message in the DIGITS.VAP file.
	//  the vector will be passed off to the vb40_play() function.
	// 'Text' represents the date numbers to be spoken
	// 'Msgs' is a pointer into the vector.
	int
ConvertVoiceTime(char * Text, char * Msgs)
	{
    int i, j, l;
	int k= VOX_DIGITS_a_m;
    int TimeStart = 0;
 	struct tm when;
 	time_t result;
    char Temp[10];

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"ConvertVoiceTime() - begin");

    if(param.MessageRtrOpt)
		{
		// Day of week
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ConvertVoiceTime() - Text  0123456789012345678901234567890123456789");
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ConvertVoiceTime() - Text [%s]", Text);

		// recognize the format of the date,
		//  the expected date format can be:
		//   0123456789 - string position gauge line
		//  '06042131 2'
		//  the above represents june 4th, 21:31 hours, year 2002.
		//  recognizing that character Text[8] is a <space> character
		//	 identifies this date format.
		if(Text[8] != ' ')
			{
			// Text[8] is not a <space> ...
			//  defines 'format 2' date and time. (new 'console' style)
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ConvertVoiceTime() - time and date format 2 detected.");

			memset( Temp, 0, sizeof( Temp ) );
			Temp[ 0 ] = Text[ 8 ];
			i = atoi( Temp );
			Msgs[ TimeStart ] = ( char )( i + 34 );
			TimeStart++;
			}
		else
			{
			//  Text[8] is a <space>
			//  defines 'format 1' date and time. (old 'notis' style)
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ConvertVoiceTime() - time and date format 1 detected.");

			memset( Temp, 0, sizeof( Temp ) );
			sprintf( Temp, "%2.2s", Text );
			when.tm_mon = atoi( Temp ) - 1;

			memset( Temp, 0, sizeof( Temp ) );
			sprintf( Temp, "%2.2s", Text+2 );
			when.tm_mday = atoi( Temp );

			// year is stored as a single digit... 
			//  for example: 0 for 2000, 2010, 2020, etc. all will map to 
			//  1900, the year that Lord Mountbatten was born.
			memset( Temp, 0, sizeof( Temp ) );
			sprintf( Temp, "%1.1s", Text+9 );
			when.tm_year = atoi( Temp );

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ConvertVoiceTime() - when.tm_year [%d]", when.tm_year);

			// the function mktime() is defined to
			//  start at year 1900.
			//  given a 1 digit value for year, we must add 100 for the period
			//  2000 thru 2009.
			// e.g. given a 1 digit year '2' add 100 yielding 102
			//  1900 + 102 = 2002.
			//  passing 102 to mktime() it will "complete" the
			//  structure, and we will determine the day of the week.

			//when.tm_year = when.tm_year + 90;		// for 1990 thru 1999
			//when.tm_year = when.tm_year + 100;	// for 2000 thru 2009 - recompile on 2000-01-01
			when.tm_year = when.tm_year + 110;		// for 2010 thru 2019 - recompile on 2010-01-01
			//when.tm_year = when.tm_year + 120;	// for 2020 thru 2029 - recompile on 2020-01-01

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ConvertVoiceTime() - add 110 [%d]", when.tm_year);

			when.tm_isdst = 0;
			when.tm_sec = 0;
			when.tm_min = 0;
			when.tm_hour = 0;
			when.tm_wday = 0;
			when.tm_yday = 0;

			// remaining in the structure is
		    //  tm_mday, tm_mon, and tm_year.

			// mktime will "complete" the structure
			//  this will yeild the day of the week ???????????.
			if( (result = mktime( &when )) != (time_t) -1 )
				{
				// date completed/converted without error.

				// VOX_DIGITS_sunday is the offset into the recordings for "0 == sunday".
				Msgs[TimeStart] = ( char )( when.tm_wday + VOX_DIGITS_sunday);		
				TimeStart++;
				}

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ConvertVoiceTime() - when.tm_wday [%d]", when.tm_wday);
			}

        memset(Temp, 0, sizeof(Temp));
        strncpy( Temp, Text, 2 ); Temp[ 2 ] = 0;
        i = atoi( Temp );
        Msgs[ TimeStart ] = ( char )( i + 41 );
        TimeStart++;

        memset(Temp, 0, sizeof(Temp));
        strncpy( Temp, Text+2, 2 ); Temp[ 2 ] = 0;
        i = atoi( Temp );
        if( i < 21 )
			{
            Msgs[ TimeStart ] = ( char )i;
            TimeStart++;
            Msgs[ TimeStart ] = 57;
            TimeStart++;
			}
        else
			{
            l = i / 10;                     //  10 minute portion
            if( l > 0 )
				{
                Msgs[ TimeStart ] = (char)( l + 18 );
                TimeStart++;
				}
            j = i - ( ( i / 10 ) * 10 );    //  1 minute portion
            if( j > 0 )
				{
                Msgs[ TimeStart ] =( char )j;
                TimeStart++;
				}
            Msgs[ TimeStart ] = 57;
            TimeStart++;
			}
		}
    memset(Temp, 0, sizeof(Temp));
    strncpy( Temp, Text+4, 2 ); Temp[ 2 ] = 0;
    i = atoi( Temp );

    if(i > 12 )
		{
        if(i < 24)
			{
			k = VOX_DIGITS_p_m;
			}
        i = i - 12;
		}
    else if(i == 12)
		{
		k = VOX_DIGITS_p_m;
		}

    Msgs[ TimeStart + 0 ] = ( char )i;

    memset(Temp, 0, sizeof(Temp));
    strncpy( Temp, Text+6, 2 ); Temp[ 2 ] = 0;
    i = atoi( Temp );
    if( i < 21 && i > 9 )
		{
        Msgs[ TimeStart + 1 ] = ( char )i;
        Msgs[ TimeStart + 2 ] = ( char )k;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ConvertVoiceTime() - exit 1");

        return( TimeStart + 3 );
		}
    else
		{
        l = i / 10;                     //  10 minute portion
        Msgs[TimeStart + 1] = (char) (l + 18);
        j = i - ( ( i / 10 ) * 10 );    //  1 minute portion
        Msgs[TimeStart + 2] = (char) j;
        Msgs[TimeStart + 3] = (char) k;

        if( j == 0 )
			{
            if( l == 0 )
				{
                Msgs[TimeStart + 1] = (char) k;
                Msgs[TimeStart + 2] = (char) 0;

				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"ConvertVoiceTime() - exit 2");

                return( TimeStart + 2 );
				}

            Msgs[TimeStart + 2] = (char) k;
            Msgs[TimeStart + 3] = (char) 0;

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ConvertVoiceTime() - exit 3");

            return(TimeStart + 3);
			}
        else if( l == 0 )
			{
			Msgs[TimeStart + 1] = 58;  // O
			}

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ConvertVoiceTime() - exit 4");

        return(TimeStart + 4);
		}
	}


	int
ConvertVoiceTime2( char *Text, char *Msgs )
	{
    int i;
    int j;
    int k= 32;  // a.m.
    int l;
    int TimeStart = 0;
    char Temp[ 10 ];

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"ConvertVoiceTime2() - begin");
	// Month
    i = atoi( Text );
    Msgs[ TimeStart ] = ( char )( i + 41 );
    TimeStart++;

	// Day
	TimeStart = TimeStart + ConvertNumeric( Text + 3, Msgs + TimeStart );

	// Year
    memset(Temp, 0, sizeof(Temp));
	sprintf( Temp, "%4.4s", Text+6 );

    i = atoi( Temp );
	if( i >= 2000 )
		{
    	TimeStart = TimeStart + ConvertNumeric( Temp, Msgs + TimeStart );
		}
	else
		{
    	memset(Temp, 0, sizeof(Temp));
    	sprintf( Temp, "%2.2s", Text+6 );
    	TimeStart = TimeStart + ConvertNumeric( Temp, Msgs + TimeStart );
    	memset(Temp, 0, sizeof(Temp));
    	sprintf( Temp, "%2.2s", Text+8 );
    	TimeStart = TimeStart + ConvertNumeric( Temp, Msgs + TimeStart );
		}

    Msgs[ TimeStart ] = 57;				// At
    TimeStart++;

    memset(Temp, 0, sizeof(Temp));
    sprintf( Temp, "%2.2s", Text+11 );
    i = atoi( Temp );

    if( i>12 )
		{
        if( i<24 )
			k = 33;          // p.m.

        i = i - 12;
		}
    else if( i == 12 )
		k = 33;

    Msgs[TimeStart + 0] = (char) i;

    memset(Temp, 0, sizeof(Temp));
    sprintf( Temp, "%2.2s", Text+13 );
    i = atoi( Temp );

    if(i < 21 && i > 9)
		{
        Msgs[TimeStart + 1] = ( char )i;
        Msgs[TimeStart + 2] = ( char )k;
        return(TimeStart + 3);
		}
    else
		{
        l = i / 10;                     //  10 minute portion
        Msgs[TimeStart + 1] = (char) (l + 18);
        j = i - ((i / 10) * 10);    //  1 minute portion
        Msgs[TimeStart + 2] = (char) j;
        Msgs[TimeStart + 3] = (char) k;

        if(j == 0)
			{
            if(l == 0)
				{
                Msgs[TimeStart + 1] = (char) k;
                Msgs[TimeStart + 2] = (char) 0;
                return(TimeStart + 2);
				}
            Msgs[TimeStart + 2] = (char) k;
            Msgs[TimeStart + 3] = (char) 0;
            return(TimeStart + 3);
			}
        else if(l == 0)
			Msgs[TimeStart + 1] = 58;  // O

        return(TimeStart + 4);
		}
	}


// Check the database for messages to retrieve
//  this applies when a dialogic channel is in the message retrieval mode.
//
	void CheckD4xMessages(int j, char * Today)
	{
		int i, k;
		char Messages[255];
		char string2[300];
		
		if(port[j].state != ST_STAT_M_SRCH) return;
		
		if(port[j].timer.wHour == 70)
			{
			GetLocalTime(&port[j].timer);
			}
		
		// approx. 3 cycles per second allowed per channel.
		//  remember Wait() units are 1/100 second, so 30 == 300 milliseconds
		i = Wait(&port[j].timer, 30L);
		
		if(i == 0) return;
		
		port[j].timer.wHour = 70;
		
		// find a message to play if one is available.
		//	note that this is the only call to GetLogPageMsg().
		i = GetLogPageMsg(j, Today);
		
		switch(i)
		{
		case MSG_FIND_NO_MSGS:
			UpdatePhoneLineStatusWindow(j, "No New Messages");
			dx_clrdigbuf(port[j].d4xHandle);
			Messages[0] = VOX_PROMPT_no_messages;
			port[j].state = ST_STAT_M_NO;
			vb40_play(promptfh, j, prompts, 1, Messages);
			break;
			
		case MSG_FIND_NO_MORE_MSGS:
			UpdatePhoneLineStatusWindow(j, "No More Messages");
			dx_clrdigbuf( port[j].d4xHandle);
			Messages[0] = VOX_PROMPT_no_more_messages;
			port[j].state = ST_STAT_M_NO2;
			vb40_play(promptfh, j, prompts, 1, Messages);
			break;
			
		case MSG_FIND_NUMERIC_MSG_FOUND:
		case MSG_FIND_VOICE_MSG:
		case MSG_FIND_TEXT_MSG:
		case MSG_FIND_XN_NUMERIC_MSG:
		case MSG_FIND_STATUS_CHANGE_MSG:
		case MSG_FIND_COVERAGE_CHANGE_MSG:
			UpdatePhoneLineStatusWindow(j, "Msg Date Play");
			dx_clrdigbuf(port[j].d4xHandle);
			page[j].Flag2[0] = 'Y';
			Messages[0] = VOX_DIGITS_message_arrived;
			
			// play upto 10 digits from DisplayInfo.
			memset(string2, 0, sizeof(string2));
			strncpy(string2, page[j].DisplayInfo, 10);
			string2[10] = 0;
			
			k = ConvertVoiceTime(string2, Messages + 1);
			
			// based on message type found in the database
			//  advance to next state.
			if(i == 3)
				port[j].state = ST_STAT_M_DATE;
			
			if(i == 6)
				port[j].state = ST_STAT_M_DATE2;
			
			if(i == 7)
				port[j].state = ST_STAT_M_DATE3;
			
			if(i == 8)
				port[j].state = ST_STAT_M_DATE4;
			
			if(i == 9)
				port[j].state = ST_STAT_M_DATE5;
			
			if(i == 10)
				port[j].state = ST_STAT_M_DATE6;
			
			vb40_play(Digitsfh, j, DigitPrompts, k + 1, Messages);
			break;
			
		case MSG_FIND_DIAL_0_TEXT_MSG:
			UpdatePhoneLineStatusWindow(j, "Text Msg Found");
			dx_clrdigbuf(port[j].d4xHandle);
			Messages[0] = VOX_PROMPT_text_messages_;
			port[j].state = ST_STAT_M_NO2;
			vb40_play(promptfh, j, prompts, 1, Messages);
			break;
			
		case MSG_FIND_STILL_LOOKING:
			// still looking... don't change state.
			break;
			
		case MSG_FIND_MSGS_UNAVAILABLE:
		default:
			// no messages available, in Failsafe mode,
			//  advance state, as if no new messages are available.
			UpdatePhoneLineStatusWindow(j, "msgs not avaialble");
			dx_clrdigbuf(port[j].d4xHandle);
			Messages[0] = VOX_PROMPT_no_messages;
			port[j].state = ST_STAT_M_NO;
			vb40_play(promptfh, j, prompts, 1, Messages);
			break;
		}
	}
