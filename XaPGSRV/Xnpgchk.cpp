
// xnpgchk.cpp


#include "common.h"
#include "List2.h"
#include "IniFilesReadWrite.h"


#include <io.h>
#include <sys\stat.h>
#include <share.h>
#include <fcntl.h>


// disable deprecation
#pragma warning(disable:4996)

// Hourly and midnight routines.

	void
HourMidnight(void)
	{
	int j;
	int k;
    char HourNow[3];
    char Today[10];
	char string2[300];
	char string3[20];

	// do actual work only every 2 seconds.
	//  this keeps the display of the "time of day" in the status line 
	//  up to date at least every few seconds.
	//
    if(MidnightTimer.wHour == 70)
		{
		GetLocalTime(&MidnightTimer);
		}

	if(Wait(&MidnightTimer, 200L))
		{
		MidnightTimer.wHour = 70;

		memset(string2, 0, sizeof(string2));
		memset(string3, 0, sizeof(string3));

		MSGTimeStamp(string2, string3);

		// string2 now contains a date formatted as: "MMDDHHWY"

		strncpy(HourNow, string2 + 4, 2);
		HourNow[2] = 0;

		// detect when the hour changes.
		//
		if(strncmp(HourStart, HourNow, 2) != 0)
			{
			// the hour has just changed...
			//
			j = atoi(string3);
			k = atoi(string3 + 4);

			// calculate the week number of the year
			if(j - k + 1 < 0)
				{
				WeekNumber = 52;
				}
			else
				{
				WeekNumber = (int) ((j - k) / 7);
				}

			// ensure range 0 thru 52 inclusive.
			WeekNumber %= 53;

			// update the content of HourStart... capturing the current hour.
			//
			strncpy(HourStart, HourNow, 2);

			// if PageServer statistics are enabled 
			//  by configuration parameter setting...
			//
			// insanity... 0 for "true", 2 for "false", see definition in xtpgmain.
			if(param.Statistics == 0)
				{
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"write statistics is enabled.");

				param.Statistics = 1;
				}
			else if(param.Statistics == 2)
				{
				for(j = 0; j < MAX_QUEUE_FILES; j++)
					{
					Queue[j].Attempts = 0;
					Queue[j].Errors = 0;
					Queue[j].Cancels = 0;
					Queue[j].External = 0;
					}

				for(j = 0; j < MAX_COMMPORTS; j++)
					{
					if(param.ComPort[j].PortActive[0] == 'N')
						continue;

					// could be 'Y' or 'I' ("IN")
					ComPort[j].Attempts = 0;
					ComPort[j].Errors = 0;
					ComPort[j].Cancels = 0;
					ComPort[j].External = 0;
					}

				for(j = 0; j < totalD4xChannels; j++)
					{
					port[j].Attempts = 0;
					port[j].Errors = 0;
					port[j].Cancels = 0;
					port[j].External = 0;
					}
				}
			}

		if(param.Statistics == 1)
			{
			memset(string3, 0, sizeof(string3));
			sprintf(string3,
					"%2.2s%2.2s%2.2s",
					xnSystemTime + 8,
					xnSystemTime,
					xnSystemTime + 3);
			param.Statistics = (short int) WriteStatistics(string3, HourNow);
			}

		// detect when the date changes,
		//  the "Today" variable holds a string with the current date.
		//  hourly, the "Today" string is compared to
		//  string2, which contains an up-to-date version
		//  of the time and date string.
		//  when the MMDD part does not compare any longer, the
		//  date has changed, a new day has arrived,
		//  midnight processing is required.

		//  a new day!
		//  and possibly a new week,
		//  and possibly a new month.

		strncpy(Today, string2, 4);
		Today[4] = 0;

		if(strncmp(Yesterday, Today, 4) != 0)
			{
			// the day has just changed.

			//  update the month, and week indexes
			//  for the billing counts.
			string2[2] = 0;
			MonthNumber = atoi(string2) - 1;

			// ensure range 0 thru 11 inclusive.
			MonthNumber %= 12;

			// perform midnight routines.
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndGeneralMsg,
							"%s - Midnight activities started.",
							DateTimeISO8601);

			// capture the new month and day as Yesterday, for comparison use.
			strncpy(Yesterday, Today, 4);

			// the day has just changed... 
			//  kick-off the midnight processing routine from idle, into
			//  its first step, it will go back into idle itself,
			//  when all steps have completed.
			//
			MidnightState = MIDNIGHT_STATE_STEP1_INITIALIZE;

			// set VoiceFileCounter back to a low value, for the new day.
			VoiceFileCounter = 1;
			}

		// this gets a "chance to run" every 2 seconds.
		//
		switch(MidnightState)
			{
			case MIDNIGHT_STATE_IDLE:
				// in the idle state... nothing to do,
				//  not time to do midnight activities, 
				//  or activities have been completed for today.
				break;


			case MIDNIGHT_STATE_STEP1_INITIALIZE:
				// FileService() will step thru all queues,
				//  "renaming" each one, until all that are in-use
				//  have been renamed.
				//
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// call FileService() with a "start" == 1 signal.
				//  this will get the process started at queue number 0;
				//  the process stops at "max queue" number.
				//  the return value is intentionally ignored.
				//
				FileService(1);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP1_RUN;
				break;

			case MIDNIGHT_STATE_STEP1_RUN:
				// call FileService() repeatedly, until it returns "true", 
				//  signaling that all are "renamed".
				//
				if(FileService(0))
					{
					// the file processing "recycle" step of
					//  "midnight" processing is now complete.
					//  
					// advance to next state.
					MidnightState = MIDNIGHT_STATE_STEP1_CLEANUP;
					}
				break;

			case MIDNIGHT_STATE_STEP1_CLEANUP:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP2_INITIALIZE;
				break;


			case MIDNIGHT_STATE_STEP2_INITIALIZE:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP2_RUN;
				break;

			case MIDNIGHT_STATE_STEP2_RUN:

				if(param.MidExtension[0] != 0)
					{
					// the midnight extension is defined in the configuration,
					//  send a page to that pager.
					//
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndGeneralMsg,
									"Prcessing midnight extension [%s]",
									param.MidExtension);

					memset(page[PAGE_MISC_POS].Extension, 0 ,
						sizeof(page[PAGE_MISC_POS].Extension));

					memset(page[PAGE_MISC_POS].PagerId, 0 ,
						sizeof(page[PAGE_MISC_POS].PagerId));

					memset(page[PAGE_MISC_POS].DisplayInfo, 0 ,
						sizeof(page[PAGE_MISC_POS].DisplayInfo));

					memset(page[PAGE_MISC_POS].AutCode, 0 ,
						sizeof(page[PAGE_MISC_POS].AutCode));

					memset(page[PAGE_MISC_POS].PagerNumber, 0 ,
						sizeof(page[PAGE_MISC_POS].PagerNumber));

					strcpy(page[PAGE_MISC_POS].Extension, param.MidExtension);
					
					GetPagerId(page[PAGE_MISC_POS].Extension, PAGE_MISC_POS, BY_PROFILEID);

					if(LogPager(page[PAGE_MISC_POS].Extension, 0) == FALSE)
						{
						PrintMessage(MSGTYPE_SYSTEM,  
										hwndGeneralMsg, 
										"Pager not assigned in log file correctly");
						}
					else
						{
						page[PAGE_MISC_POS].VoiceFile = FALSE;
						memset(string3, 0, sizeof( string3));
						sprintf(string3, "%%%s", page[PAGE_MISC_POS].PageType);

						if(Get_Frq(PAGE_MISC_POS, string3, string3) == TRUE)
							{
							AddSetUp(PAGE_MISC_POS, "XP Night");

							if(page[PAGE_MISC_POS].ScriptNumber == -1)
								page[PAGE_MISC_POS].PortNumber = ComPort[page[PAGE_MISC_POS].ComNumber].QueuePointer[page[PAGE_MISC_POS].ComOffset];
							else
								page[PAGE_MISC_POS].PortNumber = PageType[page[PAGE_MISC_POS].ScriptNumber].QueuePointer;

							if( page[PAGE_MISC_POS].PortNumber >= 0)
								{
								page[PAGE_MISC_POS].PriorityQPointer = 0;

								AddQueueEx(PAGE_MISC_POS, page[PAGE_MISC_POS].PortNumber);
								}
							else
								{
								sprintf(string2,
										"Night Q add not ready %s, %s, fh %i.",
										page[PAGE_MISC_POS].Extension,
										page[PAGE_MISC_POS].PageType,
										page[PAGE_MISC_POS].PortNumber);

								WriteLog("NOPGABRT", string2);

								PrintMessage(MSGTYPE_SYSTEM,  
												hwndGeneralMsg, 
												string2);
								}
							}
						else
							{
							PrintMessage(MSGTYPE_SYSTEM,  
											hwndGeneralMsg, 
											"No pager currently assigned. Check frq file - 1.");
							}
						}
					}
				else
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndGeneralMsg,
									"ProfileId for midnight page NOT defined.");
					}

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP2_CLEANUP;
				break;

			case MIDNIGHT_STATE_STEP2_CLEANUP:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP3_INITIALIZE;
				break;


			case MIDNIGHT_STATE_STEP3_INITIALIZE:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP3_RUN;
				break;

			case MIDNIGHT_STATE_STEP3_RUN:
				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP3_CLEANUP;
				break;

			case MIDNIGHT_STATE_STEP3_CLEANUP:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP4_INITIALIZE;
				break;


			case MIDNIGHT_STATE_STEP4_INITIALIZE:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP4_RUN;
				break;

			case MIDNIGHT_STATE_STEP4_RUN:
				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP4_CLEANUP;
				break;

			case MIDNIGHT_STATE_STEP4_CLEANUP:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP5_INITIALIZE;
				break;


			case MIDNIGHT_STATE_STEP5_INITIALIZE:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP5_RUN;
				break;

			case MIDNIGHT_STATE_STEP5_RUN:
				// advance to next state.
				MidnightState = MIDNIGHT_STATE_STEP5_CLEANUP;
				break;

			case MIDNIGHT_STATE_STEP5_CLEANUP:
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								MidnightStateText[MidnightState]);

				// advance to next state.
				MidnightState = MIDNIGHT_STATE_FINAL;
				break;


			case MIDNIGHT_STATE_FINAL:
				// announce completion of midnight activities.
				//
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								"%s - Midnight activities completed.",
								DateTimeISO8601);

				// back to idle state.
				MidnightState = MIDNIGHT_STATE_IDLE;
				break;

			default:
				// if ever found in an "impossible" state,
				//  return to idle.
				//
				MidnightState = MIDNIGHT_STATE_IDLE;
				break;
			}
		}
	}


	// Check for pause on outgoing TouchTone pages
	//
	void
RCC_Check(int PagePointer)
	{
	int i;
	int k;

    if( port[ PagePointer ].PortStatus == 2 )
		{
        k = page[ PagePointer ].ScriptFlag;

        switch( param.PageType[ page[ PagePointer ].ScriptNumber ].Script[ k ])
			{
			case '1':
                CheckScript( PagePointer );
				break;

			case '2':
                CheckScript( PagePointer );
				break;

			case '3':
                CheckScript( PagePointer );
				break;

			case 'O':
				if( port[ PagePointer ].t1 == TRUE )
					{
					page[ PagePointer ].ScriptFlag++;
					CheckScript( PagePointer );
					}
				// fall into default case.

            default:
                // Time out if status does not change
                if( port[ PagePointer ].timer.wHour == 70 )
					{
					GetLocalTime( &port[ PagePointer ].timer );
					}

                i = Wait( &port[ PagePointer ].timer, 6000L );

                if( i )
					{
                    UpdatePhoneLineStatusWindow( PagePointer, "Timeout OnHook" );

					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"Line %i Timeout OnHook", 
									PagePointer);

                    port[ PagePointer ].timer.wHour = 70;
                    port[ PagePointer ].state = ST_ONHK_DELAY;
					XnSetHook( PagePointer, DX_ONHOOK );
                    page[ PagePointer ].PageStatus[0] = 'A';
                    port[ PagePointer ].PortStatus = 1;
                    port[ PagePointer ].Errors++;
                    SetQueueError( -1, PagePointer, 1 );
					}
                break;
			}

        if( k < page[ PagePointer ].ScriptFlag )
			CheckScript( PagePointer );
		}
	}


	//    Check for delay after going on hook.
	//
	void
Check_D4xHookState(int j)
	{
	int i;

    if( port[ j ].state == ST_ONHK_DELAY )
		{
        if( port[ j ].timer.wHour == 70 )
			{
			GetLocalTime( &port[ j ].timer );
			}

		// delay 2 seconds
        i = Wait(&port[j].timer, 200L);

		if(i)
			{
            port[j].state = ST_IDLE;
            port[j].PortStatus = 0;
            port[j].timer.wHour = 70;
            UpdatePhoneLineStatusWindow(j, "Idle");
			}
		}

	// Check for Pause in T1 Offhook
	//
	if( port[ j ].t1 == TRUE )
		{
		if( port[ j ].state == ST_T1_PAUSE )
			{
			if( port[ j ].timer.wHour == 70 )
				{
				GetLocalTime( &port[ j ].timer );
				}

			i = Wait( &port[ j ].timer, param.t1OffHookPause );

			if( i )
				{
				port[ j ].state = ST_OFFHK;
				SetNextD4xState(j, NULL);
				port[ j ].timer.wHour = 70;
				}
			}
		}

	// Check for completion of on-hook event 
	//
    if( port[ j ].state == ST_ONHK )
		{
        if( port[ j ].timer.wHour == 70 )
			{
			GetLocalTime( &port[ j ].timer );
			}

		// delay 5 seconds
        i = Wait( &port[ j ].timer, 500L );

		if( i )
			{
            port[ j ].timer.wHour = 70;
			XnSetHook( j, DX_ONHOOK );

			PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Line %i Hangup Timeout", 
							j);
			}
		}

    if( port[ j ].state == ST_IDLE ||
		port[ j ].LoopCurrent == FALSE)
		{
    	//switch( port[ j ].PortType )
    	switch(param.IVRPort[j].iType)
			{
        	case 3:		// line type B/O == busied out
        	case 5:		// line type MET == Dialogic DTMF Leased Line (always off hook)
				break;

        	case 21:	// line type DPG == Dialogic DID-40  page in line
        	case 27:	// line type DST == Dialogic DID-40  page status line
				if( port[ j ].t1 == FALSE )
            		break;

        	default:
                if( port[ j ].timer.wHour == 70 )
					{
					GetLocalTime( &port[ j ].timer );
					}

				// delay 30 seconds after hangup...???? why?
                i = Wait( &port[ j ].timer, 5000L );

				if( i )
					{
					if( port[ j ].t1 == FALSE)
						{
						// check analog line hook state
						if( ATDX_HOOKST( port[ j ].d4xHandle ) == DX_OFFHOOK )
							{
							PrintMessage(MSGTYPE_SYSTEM,  
											hwndD4xMsg,
											"Analog-Line [%i] Set Onhook, State [%d] LoopCurrent [%d]",
											j,
											port[j].state,
											port[j].LoopCurrent);

							port[ j ].timer.wHour = 70;
							port[ j ].state = ST_ONHK;
							XnSetHook( j, DX_ONHOOK );
							}
						}
					else
						{
						// check T1 line hook state
						long tsBits;
						long status;
						char *sigbits;
						unsigned short axmt;
						unsigned short bxmt;
						unsigned short CmdOnHook;
						int Diag_arcv, Diag_brcv, Diag_axmt, Diag_bxmt; // Bit mask values

						tsBits = ATDT_TSSGBIT(port[ j ].d4xT1Handle);
						if( tsBits == AT_FAILURE )
							{
							// print a bunch of diagnostic information.
							PrintMessage(MSGTYPE_SYSTEM, 
											hwndD4xMsg,
											"Check_D4xHookState(): Line [%d] T1 TSSGBIT Error [%s]",
											j,
											ATDV_ERRMSGP(port[j].d4xT1Handle));

							PrintMessage(MSGTYPE_SYSTEM, 
											hwndD4xMsg,
											"Check_D4xHookState(): port[%d].d4xT1HandleT1 == [%d] tsBits[%d]",
											j,
											port[j].d4xT1Handle,
											tsBits);

							status = ATDT_STATUS(port[j].d4xT1Handle);
							if(status == DTST_INACTIVE)
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg, 
												"ATDT_STATUS() returned [%d] == DTST_INACTIVE", 
												status);
								}
							else if(status == DTST_BUSY)
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg, 
												"ATDT_STATUS() returned [%d] == DTST_BUSY", 
												status);
								}
							else
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg, 
												"ATDT_STATUS() returned [%d] == ???", 
												status);
								}

							status = ATDT_IDLEST(port[j].d4xT1Handle);
							if(status == DTIS_ENABLE)
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg, 
												"ATDT_IDLEST() returned [%d] == DTIS_ENABLE", 
												status);
								}
							else if(status == DTIS_DISABLE)
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg, 
												"ATDT_IDLEST() returned [%d] == DTIS_DISABLE", 
												status);
								}
							else
								{
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg, 
												"ATDT_IDLEST() returned [%d] == ???", 
												status);
								}

							// board level status information.
							sigbits = ATDT_BDSGBIT(bddev);

							if(sigbits == AT_FAILUREP)
								{
								// failed to get bits
								PrintMessage(MSGTYPE_SYSTEM, 
												hwndD4xMsg,
												"error getting board info.Error message = %s.",
												ATDV_ERRMSGP(bddev));
								}
							else
								{
								// bits are:
								for(i = 0; i < 24; i++)
									{
									Diag_arcv = ( sigbits[ i ] & DTSG_RCVA ) ? 1 : 0;
									Diag_brcv = ( sigbits[ i ] & DTSG_RCVB ) ? 1 : 0;
									Diag_axmt = ( sigbits[ i ] & DTSG_XMTA ) ? 1 : 0;
									Diag_bxmt = ( sigbits[ i ] & DTSG_XMTB ) ? 1 : 0;

									PrintMessage(MSGTYPE_SYSTEM, 
													hwndD4xMsg,
													"tslot #%d Diag_arcv = %d, Diag_brcv = %d, "
													"Diag_axmt = %d, Diag_bxmt = %d",
													i + 1,
													Diag_arcv,
													Diag_brcv,
													Diag_axmt,
													Diag_bxmt);
									}
								}
							}
						else
							{
							axmt = (unsigned short) ((tsBits & DTSG_XMTA) ? DTB_AON : DTB_AOFF);
							bxmt = (unsigned short) ((tsBits & DTSG_XMTB) ? DTB_BON : DTB_BOFF);

							CmdOnHook = (unsigned short) (axmt | bxmt);

							if( CmdOnHook != param.t1CmdOnHook )
								{
								PrintMessage(MSGTYPE_SYSTEM,  
												hwndD4xMsg,
												"T1-Line [%i] Set Onhook, State [%d]",
												j,
												port[j].state);

								port[ j ].timer.wHour = 70;
								port[ j ].state = ST_ONHK;
								XnSetHook( j, DX_ONHOOK );
								}
							}
						}
					}
				break;
			}
 		}
	}


	// attempt to add a page request queue record.
	//
	void
Check_D4x_Add_Que(int j)
	{
	int i;

	if(port[j].state == ST_ADD_PAGE)
		{
		// we are in the ST_ADD_PAGE state.
		//  make an attempt to add to the queue file,
		//  until the status becomes 'C' due to successful queue write...
		//  we will continue to try the write-to-queue action.
		//

		if(port[j].timer.wHour == 70)
			{
			GetLocalTime( &port[j].timer);
			}

        i = Wait(&port[j].timer, 500L);

		if(i)
			{
			// 5 seconds has elapsed, try the AddQueue () again.
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg, 
							"Line %i Add Page Retry", 
							j);

			port[j].timer.wHour = 70;
			page[j].PortNumber = GetQuePointer(j);

			if(((page[j].PortNumber >= 0) && (page[j].PageStatus[0] != 'C')) || (IsSmsType(j)))
				{
				// attempt to take the IVR request data, and
				//  write what needs to be written to the page queue file.
				//  this will complete the request process.
				//  (AddQueue () sets the status to 'C' on successful write).
				//
				if(AddQueueEx(j, page[j].PortNumber))
					{
					// AddQueue () was successful...

					// once the page has been successfully added to the 
					//  queue file, the data structure can be re-used,
					//  the IVR channel can be set to on-hook, and
					//  IVR line can be reused.

					// indicate that the IVR channel is idle.
					page[j].PageStatus[0] = 'I';

					UpdatePhoneLineStatusWindow(j, "Hanging Up");
					port[j].state = ST_ONHK;
					port[j].PortStatus = 0;

					// XnSetHook() will cause a state change.
					//  away from ST_ADD_PAGE.
					//
					XnSetHook(j, DX_ONHOOK);
					}
				}
			}
		}
	}



	CString
GetEmailDateTimeString(void)
	{
	SYSTEMTIME st;
	char DateTimeString[45];

	// get "Greenwich Mean Time" UTC time.
	GetSystemTime(&st);

	// date and time for email purposes.
	sprintf(DateTimeString,
		"%04.4d-%02.2d-%02.2d %02.2d:%02.2d:%02.2d UTC",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond
		);

	return (CString) (LPCTSTR) DateTimeString;
	}



	CString
GetDateTimeString(void)
	{
	SYSTEMTIME st;
	char DateTimeString[45];

	// get current time.
	GetLocalTime(&st);

	// prepare a date and time string using ISO-8601 style format.
	sprintf(DateTimeString,
		"%04.4d-%02.2d-%02.2dT%02.2d-%02.2d-%02.2d-%03.3d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);

	return (CString) (LPCTSTR) DateTimeString;
	}


	void
SendCancelledPageEmail(int Index)
	{
	// send cancelled page information via email.
	CString csEmailFilenameDate = GetDateTimeString();
	CString csEmailContentDate = GetEmailDateTimeString();

	// first pickup some ini parameters.
	//char PathAndFilename[MAX_PATH];
	//sprintf(PathAndFilename, "%sXaPgSrv.ini", szApplicationPathName);

	CString PathAndFilename;
	PathAndFilename.Format("%sXaPgSrv.ini", szApplicationPathName);


	//char section[] = "CANCELLED_PAGE_EMAIL";
	CString section = "CANCELLED_PAGE_EMAIL";

	//                                 section   key                  default   ini path and name.
	bool SendEmailEnabled = GetIniBool(section, "SEND_EMAIL_ENABLED", "FALSE",  PathAndFilename);

	if(!SendEmailEnabled)
		{
        PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Cancelled page SEND_EMAIL_ENABLED option NOT enabled.");
		return;
		}

	bool SendPreambleEnabled  = GetIniBool(section, "EMAIL_PREAMBLE_TEXT_ENABLED", "FALSE",  PathAndFilename);
	bool SendPostambleEnabled = GetIniBool(section, "EMAIL_POSTAMBLE_TEXT_ENABLED", "FALSE",  PathAndFilename);

    PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Cancelled page SEND_EMAIL_ENABLED option IS enabled.");

	// use IIS SMTP (email) via "file-drop on pickup-directory" feature.
	CString csPathToPickupDir = GetIniString(section, "PICKUP_DIRECTORY", "c:\\inetpub\\mailroot\\pickup", PathAndFilename);

	CString csFrom    = GetIniString(section, "FROM",    "pageserver@spok.com", PathAndFilename);
	CString csTo      = GetIniString(section, "TO",      "kgill@spok.com",      PathAndFilename);
	CString csSubject = GetIniString(section, "SUBJECT", "Cancelled Page.",      PathAndFilename);

	CString csEmailFromLine     = "From: " + csFrom + "\r\n";
	CString csEmailToLine       = "To: " + csTo + "\r\n";
	CString csEmailDateLine     = "Date: " + csEmailContentDate + "\r\n";
	CString csEmailSubjectLine  = "Subject: " + csSubject + "\r\n";
	CString csEmailSeparator    = "\r\n";

	CString csEmailBody;

	// add preamble text, if enabled;
	if(SendPreambleEnabled)
		{
		CString csPreambleText = GetIniString(section, "EMAIL_PREAMBLE_TEXT", "", PathAndFilename);
		csEmailBody +=  csPreambleText + "\r\n";
 		csEmailBody += "\r\n";
		}


	// add body text;
	CString csTemp;

	csEmailBody += "Pageserver Cancelled Page Report: \r\n";

	csTemp.Format("Customer Name: [%s]\r\n", param.CustomerInfoFullName);
	csEmailBody += csTemp;

	csTemp.Format("StationId: [%s]\r\n", param.stationId);
	csEmailBody += csTemp;

	csTemp.Format("ProfileId: [%s]\r\n", page[Index].Extension);
	csEmailBody += csTemp;

	csTemp.Format("PagerId: [%s]\r\n", page[Index].PagerId);
	csEmailBody += csTemp;

	csTemp.Format("Initiated: [%10.10s] [%8.8s]\r\n", page[Index].InitiatedDate, page[Index].InitiatedTime);
	csEmailBody += csTemp;

	csTemp.Format("PageType: [%%%2.2s]  Initiated By: [%s]\r\n", page[Index].PageType, page[Index].InitiatorId);
	csEmailBody += csTemp;

	csTemp.Format("Number Sent: [%s]\r\n", page[Index].PagerNumber);
	csEmailBody += csTemp;

	csTemp.Format("Id Number: [%s]\r\n", page[Index].AutCode);
	csEmailBody += csTemp;

	if(Index >= PAGE_COM_START && Index < PAGE_LIST_POS)
		{
		if(ComPort[Index - PAGE_COM_START].errorResponse[0] != 0)
			{
			csTemp.Format("Response: [%s]\r\n", ComPort[Index - PAGE_COM_START].errorResponse);
			csEmailBody += csTemp;
			}
		}

	// break up message into 65 characters per line, as necessary.
	//
	int msgLength = strlen(page[Index].DisplayInfo);
	int start;
	int last = 0;
	int count;

	csEmailBody += "\r\n";

	csTemp.Format(" Message Sent: \r\n");
	csEmailBody += csTemp;

	while(msgLength)
		{
		start = last;
		if(msgLength > 65)
			{
			msgLength -= 65;
			last += 65;
			count = 65;
			}
		else
			{
			last += msgLength;
			count = msgLength;
			msgLength = 0;
			}

		CString msg(&page[Index].DisplayInfo[start], count);

		csTemp.Format("%s\r\n", static_cast<LPCTSTR>(msg));
		csEmailBody += csTemp;
		}

	csEmailBody += "\r\n";

	// add postamble text, if enabled.
	if(SendPostambleEnabled)
		{
 		CString csPostambleText = GetIniString(section, "EMAIL_POSTAMBLE_TEXT", "", PathAndFilename);
		csEmailBody +=  csPostambleText + "\r\n";
		csEmailBody += "\r\n";
		}

	// drop filename is named with date and time information.
	CString csDropFilePathAndFilename = csPathToPickupDir + "\\EmailPickup_" + csEmailFilenameDate + ".txt";

    PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg, 
					"Writing to email file. [%s].", 
					csDropFilePathAndFilename);

    int fh = _sopen(csDropFilePathAndFilename,
					_O_CREAT | _O_APPEND | _O_WRONLY | _O_BINARY,
					_SH_DENYWR,
					_S_IREAD | _S_IWRITE);
	if(fh != -1)
		{
		_write(fh, csEmailFromLine,    csEmailFromLine.GetLength());
        _write(fh, csEmailToLine,      csEmailToLine.GetLength());
        _write(fh, csEmailDateLine,    csEmailDateLine.GetLength());
        _write(fh, csEmailSubjectLine, csEmailSubjectLine.GetLength());
        _write(fh, csEmailSeparator,   csEmailSeparator.GetLength());
        _write(fh, csEmailBody,        csEmailBody.GetLength());
        _write(fh, csEmailSeparator,   csEmailSeparator.GetLength());

        _close(fh);
		}
	else
		{
        PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"ERROR !!! Unable write to email drop file. [%s].", 
						csDropFilePathAndFilename);
		}
	}


	void
CancelledPageProcessing(int IndexToPageStruct)
	{
	// request a cancelled page report, if configured to do so.
	//  
    if(param.CrtclPrint[0] == 'C' || param.CrtclPrint[0] == 'B')
		{
        CancelPrint(IndexToPageStruct);
		}

	// send an email if configuration parameter enable is set to true.
	SendCancelledPageEmail(IndexToPageStruct);
	}


	// parameter 1 - j identifies which of 96 IVR ports 
	//  numberes (0 thru 95) to be used.
	//
	void
Check_RCC_Queues(int j)
	{
	//	Check for aborted pages.
    if(page[j].PageStatus[0] == 'A' && port[j].state == ST_IDLE)
		{
        if(page[j].attempts+1 < param.PageType[page[j].ScriptNumber].retry)
			{
            port[j].PortStatus = 2;
            port[j].state = ST_CCPG_CALL;
            port[j].timer.wHour = 70;
            port[j].PageNumber = j;
            page[j].ScriptFlag = 0;
            page[j].PortNumber = j;
            page[j].attempts++;
            page[j].PageStatus[0] = 'B';

            cpcinit(page[j].ScriptNumber);

            UpdatePhoneLineStatusWindow(j, 
										"Script %3s", 
										param.PageType[page[j].ScriptNumber].pageid);
            CheckScript(j);
			}
        else
			{
            // Maximum number of retries exceeded for this page. 
            if(param.SupperExt[0] != 0)
				{
				WriteSuppLog_Cancelled(j, FALSE, param.SupperExt);
				}

            if(param.UserMsg[0] != 'N')
				{
				WriteSuppLog_Cancelled(j, TRUE, page[j].Extension);
				}

				char string2[300];

				sprintf(string2,
						"Line %i CANCELLED RCC Page - Number %s Display %s, "
						"extension %s, attempts %i, queue %%%s.",
						j,
						page[j].PagerNumber,
						page[j].DisplayInfo,
						page[j].Extension,
						page[j].attempts + 1,
						page[j].PageType);

            PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg,
							"%s",
							string2);

            page[j].attempts = 0;
            page[j].PageStatus[0] = 'I';

            WriteLog("NOPGERR", string2);

			CancelledPageProcessing(j);

            SetQueueError(-1, j, 2);
			}
		}
	else
		{
		// Check each defined RCC queue.
		//
        for(int QuePointer = 0; QuePointer < param.PgTypsNum; QuePointer++)
			{
			if((port[j].PortStatus == 0) && 
				(param.IVRPort[j].iType == param.PageType[QuePointer].PortType))
				{
				if(PageType[QuePointer].InQueuePointer > 0)
					{
					if(PageType[QuePointer].QTimer.wHour == 70)
						{
						GetLocalTime(&PageType[QuePointer].QTimer);
						}

					if(Wait(&PageType[QuePointer].QTimer, (long) QPause))
						{
						if(CheckQueue(PageType[QuePointer].InQueuePointer) > 0)
							{
							ProcessIvrQueue(QuePointer, j);
							break;
							}
						else
							{
							PageType[QuePointer].QTimer.wHour = 70;
							}
						}
					}
				}
			}
		}
	}


	// each comm port may indicate that a voice page should be
	//  added to the queue on a regular time interval.
	//
	//  this "canned" voice page should be a spoken voice recording
	//   of the FCC station identification call-sign.
	//
	//  note that the zetron model 15 paging encoder and dialogic
	//   IVR port hookup for voice paging is required for this to work.
	void
CheckCallSign(void)
	{
 	if(bExternalControl && (PsOpMode == MODE_STANDBY))
		{
		// when externally controlled, and in the standby mode, 
		//  no database can be written to, so just return.
		return;
		}

	// for each of XX comm ports.
    for(int i = 0; i < MAX_COMMPORTS; i++)
		{
        if(param.ComPort[i].CallSignExtension[0] != 0 &&
			param.ComPort[i].CallSignInterval > 0)
			{
			// non-zero, positive delay interval,
			//  and non-null ProfileID must be defined.
			//  profile leads to the filename of the voice recording.
            if(ComPort[i].CSTimer.wHour == 70)
				{
				GetLocalTime(&ComPort[i].CSTimer);
				}
 
            if(Wait(&ComPort[i].CSTimer, param.ComPort[i].CallSignInterval))
				{
				// time to place a "station identification" page request in the queue.
                ComPort[i].CSTimer.wHour = 70;

                PrintMessage(MSGTYPE_SYSTEM,  
								hwndComMsg, 
								"COM%i:, Initializing Call Sign", 
								(i + 1));

                memset( page[ PAGE_MISC_POS ].DisplayInfo, 0 ,sizeof( page[ PAGE_MISC_POS ].DisplayInfo ) );
                strcpy( page[ PAGE_MISC_POS ].Extension, param.ComPort[ i ].CallSignExtension );

				GetPagerId( page[ PAGE_MISC_POS ].Extension, PAGE_MISC_POS, BY_PROFILEID);

                if( LogPager( page[ PAGE_MISC_POS ].Extension, PAGE_MISC_POS ) == FALSE )
					{
                    PrintMessage(MSGTYPE_SYSTEM, 
									hwndComMsg, 
									"Pager not assigned in log file." );
					}
                else
					{
                    strcpy( page[ PAGE_MISC_POS ].DisplayInfo, page[ PAGE_MISC_POS ].Extension );
                    getfile( page[ PAGE_MISC_POS ].DisplayInfo, "VCE", 255 );

					char string3[20];

                    memset( string3, 0, sizeof( string3 ) );
                    sprintf( string3, "%%%s", page[ PAGE_MISC_POS ].PageType );

                    if( Get_Frq( PAGE_MISC_POS, string3, string3 ) == TRUE )
						{
                        AddSetUp(PAGE_MISC_POS, "XP CS");

                        if( page[ PAGE_MISC_POS ].ScriptNumber == -1 )
							{
                            page[PAGE_MISC_POS].PortNumber =
								ComPort[page[PAGE_MISC_POS].ComNumber].QueuePointer[page[PAGE_MISC_POS].ComOffset];
							}
                        else
							{
                            page[PAGE_MISC_POS].PortNumber =
								PageType[page[PAGE_MISC_POS].ScriptNumber].QueuePointer;
							}

                        if( page[ PAGE_MISC_POS ].PortNumber >= 0 )
							{
                            page[ PAGE_MISC_POS ].PriorityQPointer = 0;

                            AddQueueEx(PAGE_MISC_POS, page[PAGE_MISC_POS].PortNumber);
                            WriteUserLogEx(PAGE_MISC_POS, page[PAGE_MISC_POS].PortNumber);
							}
                        else
							{
							char string2[300];

                            sprintf(string2,
									"CS Q add not ready %s, %s, fh %i.",
									page[ PAGE_MISC_POS ].Extension,
									page[ PAGE_MISC_POS ].PageType,
									page[ PAGE_MISC_POS ].PortNumber);

                            WriteLog("NOPGABRT", string2);

                            PrintMessage(MSGTYPE_SYSTEM,  
											hwndComMsg, 
											string2);
							}
						}
                    else
						{
                        PrintMessage(MSGTYPE_SYSTEM,  
										hwndComMsg, 
										"Pager not assigned. Check frq file.");
						}
					}
				}
			}
		}
	}


	void
CheckCatsCall(void)
	{
	int	i;
	int j;
    //      Check to see if CATS call or page need to be processed.
    for( i=0; i<4; i++ )
		{
        if( Cats[ i ].GroupCallWaiting )
			{
            if( Cats[ i ].StartTime.wHour == 70 )
				{
				GetLocalTime( &Cats[ i ].StartTime );
				}

			// 12000 represents 2 minutes
            j = Wait(&Cats[i].StartTime, 12000L);

            if( j == 0 )
				{
                GroupCatCall( i );
				}
            else
				{
				if( param.Cats[ i ].Group1Port > 0 )
					page[ param.Cats[ i ].Group1Port + PAGE_COM_START - 1 ].PageStatus[ 0 ] = 'I';

				if( param.Cats[ i ].Group2Port > 0 )
	                page[ param.Cats[ i ].Group2Port + PAGE_COM_START - 1 ].PageStatus[ 0 ] = 'I';

                Cats[ i ].GroupCallWaiting = 0;
                CatsCall = FALSE;
                WriteLog( "XTPGCATS.MSG", "Cat call aborted.\r\n" );
				}
			}
		}
	}


	void
CheckKeepAlive(void)
	{
	int j;

	if(param.keepAliveAlarm)
		{
		if(keepAliveTimer.wHour == 70)
			{
			GetLocalTime(&keepAliveTimer);
			}

		j = Wait(&keepAliveTimer, 6000L);	// 6000L == one minute

        if(j)
			{
			keepAliveTimer.wHour = 70;

			CString csText;
			csText.Format("%-2.2s", param.ActionIdForKeepAlive);

			SetActionReminder(param.ActionReminderProfileId, 
								param.ActionIdForKeepAlive, 
								0, 
								REMINDER_UPDATE, 
								(5 * 60), 
								csText);
			}
		}
	}



	void
CheckQueueErrorMonitor(void)
	{
	enum STATE_VARIABLE	
		{
		STATE_IDLE = 0,
		STATE_INITIALIZE,
		STATE_DETECT,
		STATE_REPORT,
		STATE_DELAY,
		};

	// a tick interval is defined so that the ErrorMonitor will not
	//  consume excessive cpu resource.
	//
	// the ErrorMonitor is implemented using a "State Machine" model.
	//
	// when the time interval (tick) has elapsed, the state machine will run
	//  and determine what to do.
	//
	// states:
	//  Idle		check if feature is enabled, advance to initialize state if enabled.
	//  Initialize	init internal extended state variable(s).
	//  Detect		check each "queue manager" object for non-zero error counts.
	//				no action if no errors, advance to the report state if errors
	//				are detected.
	//  Report		report errors immediately via an Xtend "action reminder", 
	//				and enter the delay state.
	//  Delay		increase the delay count, and if it has exceeded its
	//				limit, advance to the clear state. if the error persists
	//				it will be detected again, and reported again shortly.
	//
	static STATE_VARIABLE State = STATE_IDLE;
	static SYSTEMTIME stTime;					// SystemTime to track time interval.
	static int ReportDelay;						// extended state variable.
	static bool bFirstTimeThru = true;			// startup variable.

	if(bFirstTimeThru)
		{
		bFirstTimeThru = false;
		GetLocalTime(&stTime);
		}

	// wait for a 10 second "tick" interval.
	if(!Wait(&stTime, 10 * 100L))		// units are 1/100 sec.
		{
		// the interval has not passed yet...
		return;
		}
	else
		{
		// the old interval has elapsed, start a new one by
		// capturing the value of the current time.
		GetLocalTime(&stTime);
		}

	// dispatch each tick, based on state machine state.
	//
	switch(State)
		{
		case STATE_IDLE:
			if(param.queueAccessError)
				{
				// feature is enabled, advance to next state.
				State = STATE_INITIALIZE;
				}
			break;

		case STATE_INITIALIZE:
			ReportDelay = 0;
			State = STATE_DETECT;
			break;

		case STATE_DETECT:
			// check queue information only in pageserver's "active" modes.
			switch(PsOpMode)
				{
				case MODE_ACTIVE:
				case MODE_ALWAYS_ACTIVE:
					{
					// examine the queue manager error count.
					//
					for(int i = 0; i < MAX_QUEUE_FILES; i++)
						{
						// check if defined in queue control.
						if(Queue[i].Id[0] == '%') 
							{
							if(Queue[i].pQFM->GetErrorCount() > 0)
								{
								// error(s) detected...
								State = STATE_REPORT;

								// one error is enough to report on.
								break;
								}
							}
						}
					}
					break;

				case MODE_IDLE:
				default:
					// do nothing...
					break;
				}
			break;

		case STATE_REPORT:
			{
			// log all information collected in TroubleText
			//  to the pageserver logfile, as a recap of 
			//  the reason that an action reminder is being sent.
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "Queue Access Trouble Recap:");

			for(int i = 0; i < MAX_QUEUE_FILES; i++)
				{
				// check if defined in queue control.
				if(Queue[i].Id[0] == '%')
					{
					//
					if(Queue[i].pQFM->GetErrorCount() > 0)
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										(LPTSTR) (LPCTSTR) Queue[i].pQFM->GetFirstErrorText());

						// clear error information.
						Queue[i].pQFM->ClearErrorInfo();

						// no "break" from this loop, 
						//  collect/show all messages for all queues.
						}
					}
				}

			// send 1 action reminder.
			CString csText;
			csText.Format("%-2.2s", param.ActionIdForQueueAccessError);

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"Sending ActionReminder to ProfileId: [%s] ActionId: [%s]",
							param.ActionReminderProfileId,
							param.ActionIdForQueueAccessError
							);
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "");

			SetActionReminder(param.ActionReminderProfileId, 
								(LPTSTR) (LPCTSTR) param.ActionIdForQueueAccessError, 
								0, 
								REMINDER_ADD, 
								0, 
								csText);

			State = STATE_DELAY;
			}
			break;

		case STATE_DELAY:
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"CheckQueueErrorMonitor() - State [STATE_DELAY]");

			// 30 ticks at 10 seconds/tick == 300 seconds == 5 minutes.
			if(ReportDelay++ > 30)
				{
				State = STATE_IDLE;
				}
			else
				{
				// delaying after a report has been given.
				}
			break;

		default:
			State = STATE_IDLE;
			break;
		}
	}

