// disable deprecation
#pragma warning(disable:4996)

// xnpgport.cpp


#include "common.h"


enum {UNKNOWN = 0, HIGH, LOW};


	void
ShowCommportDsrTextInStatusLine(CString text)
	{
	SetStatusText(STATUS_LINE_BIT_FIELD_1, (LPTSTR) (LPCTSTR) text);
	}


//	void
//xxxShowBinaryValueInStatusLine(unsigned char cByte)
//	{
//	char Bits[10];
//	char temp[10];
//	char TemporaryString[40];
//	
//	memset(temp, 0, sizeof(temp));
//    memset(Bits, 0, sizeof(Bits));
//
//    itoa((int) cByte, temp, 2);
//
//    sprintf(Bits, "%8.8s", temp);
//
//    for(int i = 0; i < 8; i++)
//		{
//		if(Bits[i] == ' ')
//			{
//			Bits[i] = '0';
//			}
//		}
//
//	// show a representation of the "raw" input bits 
//	//  on display status line.
//	sprintf(TemporaryString, "  %s", Bits);
//	SetStatusText(STATUS_LINE_BIT_FIELD_1, TemporaryString);
//	}

 
	int
GetExternalControlEvent(void)
	{
	//unsigned char cByte;
	static int CurrentValue = UNKNOWN;

	// the Count variable ranges from 0 to 6, 
	//  when it is at the min or max value
	//  and the input is different from the 
	//  current "debounced" value, an "event" will be generated.
	//  the "event" will be either went-high, or went-low.
	//
	// at startup it is set smack in the middle,
	//  so that an "event" will be generated shortly after startup.
	//
	static int Count = 3;

	// bit number                     0     1     2     3     4     5     6     7
//	static unsigned char Mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	bool bInputValue;

	// sample the "input".
	//  this could be the parallel port, or the serial port.
	//  if "external control" is not enabled, 
	//  this routine will not be called.

	// only one of the following sampling methods will be used,
	//  and the value of "input" (bInputValue) will be known.

	// either parallel...
	//
//	if(param.Lpt.Address > 0)
//		{
//		// sample the state of the input port (parallel port).
//		//  and mask it with the bitmask corresponding to the 
//		//  bit number that we are monitoring.
//		// this will yield a zero or non-zero value.
//		//  non-zero for high, zero for low.
//		//
//		cByte = DlPortReadPortUchar(param.Lpt.Address);
//
//		ShowBinaryValueInStatusLine(cByte);
//
//		if(Mask[g_iExternalControlParallelPortInputBit] & cByte)
//			{
//			// high - detected.
//			bInputValue = TRUE;
//
//			Count++;
//			if(Count >= 6)
//				{
//				// don't go past the maximum value.
//				Count = 6;
//				}
//			}
//		else
//			{
//			// low - detected.
//			bInputValue = FALSE;
//
//			Count--;
//			if(Count <= 0)
//				{
//				// don't go past the minimum value.
//				Count = 0;
//				}
//			}
//		}

	// or serial...
	//
	if(g_iExternalControlCommPortToUse != -1)
		{
		// sample the state of the DSR signal (serial port).
		// this will yield a zero or non-zero value.
		//  non-zero for high, zero for low.
		//
		DWORD ModemBits;
		CString csText;

		GetCommModemStatus(ComPort[g_iExternalControlCommPortToUse].hComm, &ModemBits);

		if(ModemBits & MS_DSR_ON)
			{
			csText.Format("%s DSR is High", CommportName[g_iExternalControlCommPortToUse].FamiliarName);

			// PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "DSR HIGH... [%d]", Count);

			// high - detected.
			bInputValue = true;

			Count--;
			if(Count <= 0)
				{
				// don't go past the minimum value.
				Count = 0;
				}
			}
		else
			{
			csText.Format("%s DSR is Low", CommportName[g_iExternalControlCommPortToUse].FamiliarName);

			// PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "DSR LOW... [%d]", Count);

			// low - detected.
			bInputValue = false;

			Count++;
			if(Count >= 6)
				{
				// don't go past the maximum value.
				Count = 6;
				}
			}

		ShowCommportDsrTextInStatusLine(csText);
		}

	// evaluate for each possible current state.
	//  this is so complicated because we want 
	//  cleanly debounced "events", and we want 
	//  some "event" at startup, to get things going.
	//
	switch(CurrentValue)
		{
		case UNKNOWN:
			if(Count == 6)
				{
				// reached the max value.
				//  replace the value of CurrentValue, 
				//  and return a went-high event. 
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"EVENT_GO_STANDBY");

				CurrentValue = HIGH;
				return EVENT_GO_STANDBY;
				}
			else if(Count == 0)
				{
				// reached the min value.
				//  replace the value of CurrentValue, 
				//  and return a went-low event. 
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"EVENT_GO_ACTIVE");

				CurrentValue = LOW;
				return EVENT_GO_ACTIVE;
				}
			else
				{
				// no outcome yet... keep sampling.
				}
			break;

		case HIGH:
			if(Count == 0)
				{
				// the count went all the way to 0
				//  time to CHANGE CurrentValue,
				//  and return a went-low event.
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"EVENT_GO_ACTIVE");

				CurrentValue = LOW;
				return EVENT_GO_ACTIVE;
				}
			else
				{
				// any other value of Count is of no interest, 
				//  as no change would be indicated..
				}
			break;

		case LOW:
			if(Count == 6)
				{
				// the count went all the way to 6
				//  time to CHANGE CurrentValue,
				//  and return a went-high event.
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"EVENT_GO_STANDBY");

				CurrentValue = HIGH;
				return EVENT_GO_STANDBY;
				}
			else
				{
				// any other value of Count is of no interest, 
				//  as no change would be indicated..
				}
			break;
		}
	
	return EVENT_NONE;
	}


//	void
//SetResetBit(void)
//	{
//    int  i;
//	unsigned char cByte;
//  char Bits[10];
//    char temp[10];
//    char BitValue[2];
//    char BitValue2[2];
//
//    if(param.Lpt.ResetLevel <= 0 && param.Lpt.QueueLevel <= 0)
//		{
//		// feature is not configured, no work to do.
//		// note that the parallel port address is NOT considered here.
//		return;
//		}
//
//	if(DATA_SOURCE_LOCAL == DataSource)
//		{
//		// in standalone mode (failsafe) no work to be done.
//		return;
//		}
//
//	if(bExternalControl && (PsOpMode == MODE_STANDBY))
//		{
//		// when externally controlled, and in the standby mode, 
//		//  no database can be written to, so just return.
//		return;
//		}
//
//    BitValue[0] = '1';
//    BitValue2[0] = '1';
//
//    for(i = 0; i < MAX_COMMPORTS; i++)
//		{
//        if(ComPort[i].reset > param.Lpt.ResetLevel)
//			{
//			BitValue[0] = '0';
//			}
//		}
//
//	cByte = 0;
//
//    memset(temp, 0, sizeof(temp));
//    memset(Bits, 0, sizeof(Bits));
//
//    itoa(cByte, temp, 2);
//    sprintf(Bits, "%8.8s", temp);
//
//    for(i = 0; i < 8; i++)
//		{
//		if(Bits[i] == ' ')
//			{
//			Bits[i] = '0';
//			}
//		}
//
//    for(i = 0; i < 8; i++)
//		{
//        switch(param.Lpt.Bit[i][0])
//			{
//            case 'C':
//                Bits[7 - i] = BitValue[0];
//                break;
//
//            case 'Q':
//                Bits[7 - i] = BitValue2[0];
//                break;
//
//            case 'A':
//            case '1':
//            case '2':
//            case '3':
//            case '4':
//            case '5':
//            case '6':
//            case '7':
//            case '8':
//            case '9':
//                Bits[7 - i] = '1';
//                break;
//
//            default:
//                break;
//        }
//    }
//
//	unsigned char BitMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
//    cByte = 0;
//
//    for(i = 0; i <= 7; i++)
//		{
//        if(Bits[i] == '1')
//			{
//			// turn on the bit at the corresponding position.
//			cByte |= BitMask[i];
//			}
//		}
//
//    if((BitValue[0] == '0' || BitValue2[0] == '0') && LptStatus == TRUE)
//		{
//		// not in standby mode... 
//		//  the database must be open to use this, 
//		//  and in standby mode it's closed.
//		//
//        PageThePageSupervisor(PAGE_MISC_POS);	
//
//        LptStatus = FALSE;
//		}
//    else if(BitValue[0] == '1' && BitValue2[0] == '1')
//		{
//		LptStatus = TRUE;
//		}
//	}



