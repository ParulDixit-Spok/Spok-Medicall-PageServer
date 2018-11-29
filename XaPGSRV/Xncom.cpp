
// xncom.cpp


#include "common.h"
// disable deprecation
#pragma warning(disable:4996)


COMMTIMEOUTS TimeOut;


// comNumber must be in the range of 0 thru 33 corresponding
//  to comm ports with device names: COM1 thru COM34

	int
OpenComport(int comNumber)
	{
	ComPort[comNumber].hComm = CreateFile(CommportName[comNumber].OSName,
											GENERIC_READ | GENERIC_WRITE,
											(DWORD) 0,
											0,
											(DWORD) OPEN_EXISTING,
											FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
											NULL);

	if(!SetupComm(ComPort[comNumber].hComm, (DWORD)1024, (DWORD)1024))
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg,
						"OpenComport(): comNumber [%d] [%s] Could not set buffers",
						comNumber + 1,
						CommportName[comNumber].FamiliarName);						
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg,
						"OpenComport(): comNumber [%d] [%s] opened ok!",
						comNumber + 1,
						CommportName[comNumber].FamiliarName);
		}

	SetComportState(comNumber);

	GetCommTimeouts(ComPort[comNumber].hComm, &TimeOut);

	TimeOut.ReadIntervalTimeout = MAXDWORD;
	TimeOut.ReadTotalTimeoutConstant = 0;		
	TimeOut.ReadTotalTimeoutMultiplier = 0;		
	TimeOut.WriteTotalTimeoutConstant = 0;		
	TimeOut.WriteTotalTimeoutMultiplier = 0;	

	if(!SetCommTimeouts(ComPort[comNumber].hComm, &TimeOut))
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg,
						"OpenComport(): comNumber [%d] [%s] Could not set Timeouts",
						comNumber + 1,
						CommportName[comNumber].FamiliarName);
		return FALSE;
		}



	ComPort[comNumber].hReadThread = (HANDLE) _beginthreadex(NULL,
	    													 (DWORD) 0,
		    												 ReadComport,
			    											 (LPVOID) comNumber,
				    										 CREATE_SUSPENDED,
					    									 NULL);

	if(ComPort[comNumber].hReadThread == 0)
		{		
		PrintMessage(MSGTYPE_SYSTEM,
						hwndComMsg,
						"OpenComport(): comNumber [%d] [%s] Could not set Thread",
						comNumber + 1,
						CommportName[comNumber].FamiliarName);
		return FALSE;
		}

	if(ComPort[comNumber].hComm == INVALID_HANDLE_VALUE)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg,
						"OpenComport(): comNumber [%d] [%s] INVALID_HANDLE_VALUE",
						comNumber + 1,
						CommportName[comNumber].FamiliarName);
		return FALSE;
		}
	
	return TRUE;
	}


	void
SetComportState(int comNumber)
	{
	DCB dcb;	
	BOOL fSuccess;

	dcb.DCBlength = sizeof(DCB);

	fSuccess = GetCommState(ComPort[comNumber].hComm, &dcb);

	if(!fSuccess)
		{
		}

	dcb.BaudRate = param.ComPort[comNumber].ComBaud;

	if( param.ComPort[comNumber].ComSetup == 0x1a )
		{
		dcb.ByteSize = 7;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = ONESTOPBIT;
		}
	else if( param.ComPort[ comNumber ].ComSetup == 0x4 )
		{
		dcb.ByteSize = 7;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = ONESTOPBIT;
		}
	else
		{
		dcb.ByteSize = 8;
		dcb.Parity	= NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		}

	dcb.fParity				= TRUE;
	dcb.fTXContinueOnXoff	= TRUE;
	dcb.fDtrControl			= DTR_CONTROL_ENABLE;
    dcb.fRtsControl			= RTS_CONTROL_ENABLE;
    dcb.fOutxCtsFlow		= FALSE;
    dcb.fOutxDsrFlow		= FALSE;
    dcb.fDsrSensitivity		= FALSE;

	fSuccess = SetCommState(ComPort[comNumber].hComm, &dcb);

	if(!fSuccess)
		{
		}
	}


	unsigned __stdcall
ReadComport(void * temp)
	{
	int i, j;
	DWORD Count;
    DWORD dwEvent = 0, dwError = 0, dwRead = 0, dwModemStatus = 0;
    COMSTAT	ComStat;
    OVERLAPPED Overlapped;
    HANDLE hEvent;
    BYTE by[1025];
    TCHAR buf[1025];
	TCHAR buf2[100];
	int comNumber;

	comNumber = (int) temp;

	memset(&Overlapped, 0, sizeof(OVERLAPPED));
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	Overlapped.hEvent = hEvent;

	dwEvent = EV_RXCHAR | EV_BREAK | EV_CTS | EV_DSR |
				EV_ERR | EV_RING | EV_RLSD | EV_RXFLAG;

	SetCommMask(ComPort[comNumber].hComm, dwEvent);

	while(!bKillThreads)
		{
		WaitCommEvent(ComPort[comNumber].hComm, &dwEvent, &Overlapped);

		GetOverlappedResult(ComPort[comNumber].hComm, &Overlapped, &Count, TRUE);
		ClearCommError(ComPort[comNumber].hComm, &dwError, &ComStat);

		if(dwEvent == EV_RXCHAR)
			{
			while(ComStat.cbInQue > 0)
				{
				if(ComStat.cbInQue > 1024)
					dwRead = 1024;
				else
					dwRead = ComStat.cbInQue;

				memset(by, 0, sizeof(by));

				if(!ReadFile(ComPort[comNumber].hComm, &by, dwRead, &Count, &Overlapped))
					{
					dwError = GetLastError();
					if(dwError == ERROR_IO_PENDING)
						{
	 					GetOverlappedResult(ComPort[comNumber].hComm, &Overlapped, &Count, TRUE);
 						}
					else
						{							
						}
					}

				if(Count > 0)
					{
					//	Set Up Data Scope
					if((iSelectedItem == comNumber) && (hwndDataScopeLB != NULL))
						{
						memset(buf, 0, sizeof(buf));

						// this SendMessage stuff should be mutex protected.

						dwRead = SendMessage(hwndDataScopeLB, LB_GETCOUNT, (WPARAM)0, (LPARAM)0L);
						SendMessage(hwndDataScopeLB, LB_GETTEXT, (WPARAM) dwRead - 1, (LPARAM)buf);

						for(i = strlen(buf); i < DataScopeChar; i++)
							{
							if(i >= 0)
								{
								buf[i] = ' ';
								}
							}
						}

					for(i = 0; i < (int) Count; i++)
						{
						// process each incomming character from the comm port receive
						//  buffer into the application character buffer.

						// ignore chars less than 0, or greater than 255
						if((by[i] >= 0) && (by[i] <= 255))
								{
								// char is in normal ascii/ansi range.
								}
							else
								{
								// ignore this char.
								continue;
								}

						// as chars come in, place them in the appropriate
						//  port buffer structure.

						// this is a shared resource among the ReadCommPort() thread
						//  and the other threads.
						//  we will use a mutex to control access.

						__try
							{
							WaitForSingleObject(hMutexComm[comNumber], INFINITE);

							ComPort[comNumber].buffer[ComPort[comNumber].nextWrite] = by[i];
							ComPort[comNumber].nextWrite++;

							if(ComPort[comNumber].nextWrite > sizeof(ComPort[comNumber].buffer) - 1)
								{
								ComPort[comNumber].nextWrite = 0;
								}
							}
						__finally
							{
							ReleaseMutex(hMutexComm[comNumber]);
							}

						//	Update Datascope buffer
						if((iSelectedItem == comNumber) && (hwndDataScopeLB != NULL))
							{
							if((by[i] >= 32) &&
								(by[i] <= 126) &&
								(dataScopeHex == FALSE))
								{
								// normal printable char.
								buf[DataScopeChar] = by[i];
								DataScopeChar++;
								}
							else
								{
								// show as hex representation.
								char HexString[5]
									;
								sprintf(HexString, "[%2.2X]", (by[i] & 0xff));
								for(j = 0; j < 4; j++)
									{
									buf[DataScopeChar + j] = HexString[j];
									}
								DataScopeChar += 4;
								}
							}

						if(DataScopeChar > 80)
							{
							memset(buf2, 0, sizeof(buf2));
							SendMessage(hwndDataScopeLB, LB_GETTEXT, (WPARAM)dwRead - 2, (LPARAM)buf2 );

							SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) dwRead - 1, (LPARAM) 0);
							SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) dwRead - 1, (LPARAM) buf);

							SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);
							SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM) 0);

							if( dataScopeFh != -1 )
								{
								Log_Message(dataScopeFh, &dataScopeMesgByte, buf2 );
								Log_Message(dataScopeFh, &dataScopeMesgByte, buf );
								}

							SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) - 1, (LPARAM) (LPSTR) "TX ");
							SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) - 1, (LPARAM) (LPSTR) "RX ");

							DataScopeChar = 3;
							memset( buf, 0, sizeof( buf ) );
							strcpy( buf, "RX " );
							}
						}

					if((iSelectedItem == comNumber) && (hwndDataScopeLB != NULL))
						{
						SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) dwRead - 1, (LPARAM) 0);
						SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) dwRead - 1, (LPARAM) buf);

						if(dataScopeScroll)
							{
							SendMessage(hwndDataScopeLB, LB_SETCURSEL, (WPARAM) dwRead - 1, (LPARAM) 0L);
							}
						}
					}

				if(bKillThreads)
					{
					CloseHandle(hEvent);
					return TRUE;
					}
				ClearCommError(ComPort[comNumber].hComm, &dwError, &ComStat);
				}
			}

		if(dwEvent & EV_ERR)
			{
			if(dwError & CE_OVERRUN)	
				{
				//strcpy ( lpstError, "CE_OVERRUN" );
				}
			else if(dwError & CE_BREAK)	
				{	
				//strcpy ( lpstError, "CE_BREAK");
				}
			else if(dwError & CE_FRAME)	
				{	
				//strcpy ( lpstError, "CE_FRAME");
				}
			else if(dwError & CE_RXOVER)
				{	
				//strcpy ( lpstError, "CE_RXOVER");
				}
			else if(dwError & CE_RXPARITY)
				{	
				//strcpy ( lpstError, "CE_RXPARITY");
				}
			else if(dwError & CE_TXFULL)
				{	
				//strcpy ( lpstError, "CE_TXFULL");
				}
			else
				{
				ClearCommError( ComPort[ comNumber ].hComm, &dwError, &ComStat);

				if(dwError & CE_FRAME)
					{
					//strcpy ( lpstError,"EV_ERR:CE_FRAME");
					}
				if(dwError & CE_OVERRUN)
					{	
					//strcpy ( lpstError,"EV_ERR:CE_OVERRUN");
					}
				if(dwError & CE_RXOVER)
					{	
					//strcpy ( lpstError,"EV_ERR:CE_RXOVER");
					}
				if(dwError & CE_RXPARITY)
					{	
					//strcpy ( lpstError,"EV_ERR:CE_RXPARITY");
					}
				if(dwError & CE_TXFULL)	
					{	
					//strcpy ( lpstError,"EV_ERR:CE_TXFULL");
					}
				else
					{
					//wsprintf(buf, "EV_ERR: %x", dwError);					
					}
				}
			// if we are in the process of shutting down,
			//  no error message should be printed,
			//  the error is likely due to the fact that
			//  the system is in process of shutting down.
			if(!bKillThreads)
				{
				// print the error message.
				}
			}

		if(dwEvent & (EV_CTS | EV_DSR | EV_RLSD))
			{
			if(!GetCommModemStatus(ComPort[comNumber].hComm, &dwModemStatus))

			if(dwEvent & EV_RLSD)
				{
				if(dwModemStatus & MS_RLSD_ON)
					{						
					//strcpy ( lpstError, "DCD on" );						
					}
				else
					{
					//strcpy ( lpstError, "DCD drop" );						
					}
				}

			if(dwEvent & EV_CTS)
				{
				if(dwModemStatus & MS_CTS_ON)
					{
					//strcpy ( lpstError,"CTS on");
					}
				else
					{
					//strcpy ( lpstError,"CTS drop");
					}
				}

			if(dwEvent & EV_DSR)
				{
				if(dwModemStatus & MS_DSR_ON)
					{
					//strcpy ( lpstError,"DSR on");
					}
				else
					{
					//strcpy ( lpstError,"DSR drop");
					}
				}
			}

		if(dwEvent & EV_BREAK)
			{
			//strcpy ( lpstError,"EV_BREAK" );
			}

		if(dwEvent & EV_TXEMPTY)
			{
			//strcpy ( lpstError,"EV_TXEMPTY" );
			}
		}

	return 0;
	}


	//
	//  Function: WriteComport ( int, char *, DWORD );
	//
	//	Writes characters to any comport.
	//
	//	Parameters : HWND  -- handle of the owner parent
	//				 char  -- text or message to send
	//				 DWORD -- size in bytes of message being sent
	//	NOTE: Set dwToWrite to sizeof ( lpBuf )
	//
	//	Returns: BOOL True if successful and False if failed
	//

	BOOL
WriteComport(int comNumber, const char *lpBuf)
	{
	OVERLAPPED	osWrite = {0};
	DWORD		dwWritten;
	DWORD		dwRes;
	DWORD		dwNumLines;
	BOOL		fRes = TRUE;
	int			i;
	int			j;
    TCHAR		buf[1025];
	TCHAR		buf2[ 100 ];
	TCHAR		tempString[ 10 ];

	size_t dwToWrite = strlen(lpBuf);

	//	Set Up Data Scope
	if((iSelectedItem == comNumber) && (hwndDataScopeLB != NULL))
		{
		memset(buf, 0, sizeof(buf));
		dwNumLines = SendMessage(hwndDataScopeLB, LB_GETCOUNT, (WPARAM)0, (LPARAM)0L);
		SendMessage(hwndDataScopeLB, LB_GETTEXT, (WPARAM)dwNumLines - 2, (LPARAM)buf);

		for( i=strlen( buf ); i<DataScopeChar; i++ )
			if( i>= 0 )
				buf[ i ] = ' ';

		for( i=0; i<(int)dwToWrite; i++ )
			{
			//	Update Datascope buffer
			if( (lpBuf[ i ] >= 32) &&
				(lpBuf[ i ] <= 126) &&
				(dataScopeHex == FALSE) )
				{
				buf[ DataScopeChar ] = lpBuf[ i ];
				DataScopeChar++;
				}
			else
				{
				sprintf( tempString, "[%2.2X]", lpBuf[ i ] );
				for( j=0; j<4; j++ )
					{
					buf[ DataScopeChar+j ] = tempString[ j ];
					}
				DataScopeChar = DataScopeChar + 4;
				}

			if( DataScopeChar > 80 )
				{
				// Scroll is required, scroll up 2 lines
				memset( buf2, 0, sizeof( buf2 ) );
				SendMessage(hwndDataScopeLB, LB_GETTEXT, (WPARAM) dwNumLines - 1, (LPARAM) buf2);

				SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) dwNumLines - 2, (LPARAM) 0);
				SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) dwNumLines - 2, (LPARAM) buf);

				SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM)0);
				SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) 0, (LPARAM)0);

				// log to a diskfile if the file is open (file handle not equal -1)
				if( dataScopeFh != -1 )
					{
					Log_Message(dataScopeFh, &dataScopeMesgByte, buf);
					Log_Message(dataScopeFh, &dataScopeMesgByte, buf2);
					}

				SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) -1, (LPARAM) (LPSTR) "TX ");
				SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) -1, (LPARAM) (LPSTR) "RX ");

				DataScopeChar = 3;
				memset( buf, 0, sizeof( buf ) );
				strcpy( buf, "TX " );
				}
			}
		SendMessage(hwndDataScopeLB, LB_DELETESTRING, (WPARAM) dwNumLines - 2, (LPARAM) 0);
		SendMessage(hwndDataScopeLB, LB_INSERTSTRING, (WPARAM) dwNumLines - 2, (LPARAM) buf);

		if(dataScopeScroll)
			{
			SendMessage(hwndDataScopeLB, LB_SETCURSEL, (WPARAM) (dwNumLines - 1), (LPARAM) 0L);
			}
		}

	memset(&osWrite, 0, sizeof(OVERLAPPED));

	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if(osWrite.hEvent == NULL)
		{
		return FALSE;
		}

	if(!WriteFile(ComPort[comNumber].hComm, lpBuf, dwToWrite, &dwWritten, &osWrite))
		{		
		if(GetLastError() != ERROR_IO_PENDING)
			{
			fRes = FALSE;
			}
		else
			{
			dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
			switch(dwRes)
				{
				case WAIT_ABANDONED:
					break;

				case WAIT_FAILED:
					break;

				case WAIT_OBJECT_0 :				
					if ( !GetOverlappedResult ( ComPort[ comNumber ].hComm, &osWrite, &dwWritten, FALSE ) )
						{
						if ( dwToWrite != dwWritten)
							{
							}
						if(GetLastError() == ERROR_IO_INCOMPLETE )
							{
							}
						fRes = FALSE;
						}
					else
						{
						fRes = TRUE;			
						}
					break;

				default:
					fRes = FALSE;
					break;
				}
 			}
		}
	else
		{
		fRes = FALSE;
		}

	CloseHandle(osWrite.hEvent);
	return fRes;
	}


	int
CloseComPort(int comNumber)
	{
	if(CloseHandle(ComPort[comNumber].hComm))
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg,
						"CloseComPort(): [%s]: Closed",
						CommportName[comNumber].FamiliarName);
		return TRUE;
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndComMsg,
						"CloseComPort(): Error [%s]: Not Closed",
						CommportName[comNumber].FamiliarName);
		return FALSE;
		}
	}

