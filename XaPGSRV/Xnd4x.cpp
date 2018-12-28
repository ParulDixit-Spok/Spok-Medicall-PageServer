
// xnd4x.cpp


#include "common.h"
#include "List2.h"

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <winsvc.h>

// disable deprecation
#pragma warning(disable:4996)


	void
XnD4xInitialize(void)
	{
	static  int chncnt = 0;
	int		dticnt;
	int		index;
	int     cnt;
	int		errcode;
	int		t1BoardNumber;
	int		t1ChannelNumber;
	int i;
	char	buf[15];
	char	**chnamesp;
	long	subdevs;
	static	HWND  x;
	static	DWORD dwThreadId;
	CT_DEVINFO ctinfo;
	t1BoardNumber = 1;
	t1ChannelNumber = 1;

	//
	//	Dialogic initialization
	//
	// query to see if dialogic service started
	//
	QueryService();
	//
	// query for number of voice boards in system
	sr_getboardcnt("Voice", &d4xBoardCount);
	PrintMessage(MSGTYPE_SYSTEM,  
					hwndD4xMsg, 
					"%d dialogic board found.", 
					d4xBoardCount);

	// query number of dti boards
	dticnt = 0;
	sr_getboardcnt("DTI", &dticnt);
	if( dticnt > 0 )
		{
		PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg, 
						"%d dialogic DTI board found.", 
						dticnt);
		}

	//		set up channel device name table
	dlgctrls = (DLG_CTRLS *) malloc(((d4xBoardCount*4) + 1) * sizeof(DLG_CTRLS));
	if (dlgctrls == NULL)
		{
		PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg, 
						"Error allocating d4x controls");
		return;
		}

	//
	//		Setup T1 Board Parameters
	//
	for(index=1; index <= dticnt; index++)
		{
		sprintf(buf, "dtiB%d", index);

		if ((bddev = dt_open(buf, 0)) != -1)
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg, 
							"DTI Board %s opened, handle [%i]", 
							buf, 
							bddev);	
			i = param.t1WinkLen;

			if( dt_setparm( bddev, DTG_WINKLEN, (void *)&i ) == -1 )
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg, 
								"Error setting dti winklen [%s]", 
								ATDV_ERRMSGP(bddev));	
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg, 
								"Dti winklen set [%d]", 
								i);
				}
			dt_close( bddev );
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg, 
							"Error opening dti board [%s]", 
							buf);
			}
		}

	// put channel device names in table
	for(index = 1; index <= d4xBoardCount; index++)

		{
		// open board to query number of channels
		sprintf(buf,"dxxxB%d", index);

		if((bddev = dx_open(buf, NULL)) == -1)
			{
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Error opening d4x board [%s].", 
							buf);
			}

		PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg, 
						"Dialogic Board [%s] opened, handle [%i].", 
						buf, 
						bddev);

		// get channel device names
		subdevs = ATDV_SUBDEVS(bddev);
		chnamesp = ATDX_CHNAMES(bddev);

		for(cnt = 0; cnt < (int) subdevs; cnt++)
			{
			memset( dlgctrls[chncnt].str, 0, sizeof( dlgctrls[chncnt].str ) );
			strcpy(dlgctrls[chncnt].str, *(chnamesp + cnt));
			dlgctrls[chncnt].def = FALSE;
			port[ chncnt ].t1 = FALSE;

			i = 50;
			if(dx_setparm(bddev, DXBD_R_IRD, (void *) &i) == -1)
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg, 
								"D4x error setting R_IRD: %s.", 
								ATDV_ERRMSGP(bddev));
				}

			i = ET_RON;
			if(dx_setparm(bddev, DXBD_R_EDGE, (void *) &i) == -1)
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg, 
								"D4x error setting R_EDGE: %s.", 
								ATDV_ERRMSGP(bddev));
				}

			//if( port[ chncnt ].PortType == 21 || port[ chncnt ].PortType == 27 )
			if(param.IVRPort[chncnt].iType == 21 || param.IVRPort[chncnt].iType == 27)
				{
				i = 55;
				if(dx_setparm(bddev, DXBD_FLASHTM, (void *) &i) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"D4x error setting FLASHTM: %s.", 
									ATDV_ERRMSGP(bddev));
					}
				i = 15;
				if(dx_setparm(bddev, DXBD_MINLCOFF, (void *) &i) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"D4x error setting MINLCOFF: %s.", 
									ATDV_ERRMSGP(bddev));
					}
				}
			chncnt++;
			totalD4xChannels++;
			}
		dx_close(bddev);
		}

	for(cnt = 0; cnt < chncnt; cnt++)
		{
		port[cnt].d4xT1Handle = -1;

		if((port[cnt].d4xHandle = dx_open(dlgctrls[cnt].str, NULL)) == -1)
			{
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Line [%i], dx_open [%s] failed", 
							cnt, 
							dlgctrls[cnt].str);	

			UpdatePhoneLineStatusWindow( cnt, "open %s failed", dlgctrls[cnt].str );	
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Line [%i] Device [%s] opened, Handle: [%i].", 
							cnt, 
							dlgctrls[cnt].str, 
							port[cnt].d4xHandle);

			UpdatePhoneLineStatusWindow(cnt, "%s opened", dlgctrls[cnt].str);

			errcode = dx_getctinfo(port[cnt].d4xHandle, &ctinfo);

			if(errcode == 0)
				{
				// board identification ctinfo == Computer Telephony information.
				//
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndD4xMsg, 
								"ctinfo.ct_nettype [%d]...", ctinfo.ct_nettype);

				PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "t1ChannelNumber [%d]", t1ChannelNumber);

				switch(ctinfo.ct_nettype)
					{
					case CT_NTT1:
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "CT_NTT1 detected");

						sprintf( dlgctrls[ cnt ].t1Str, "dtiB%dT%d", t1BoardNumber, t1ChannelNumber );
						port[cnt].t1 = TRUE;
						t1ChannelNumber++;

						if(t1ChannelNumber > 24)
							{
							t1BoardNumber++;
							t1ChannelNumber = 1;
							}
						break;

					case CT_NTNONE:
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "CT_NTNONE detected");

						sprintf( dlgctrls[ cnt ].t1Str, "dtiB%dT%d", t1BoardNumber, t1ChannelNumber );
						port[cnt].t1 = TRUE;
						t1ChannelNumber++;

						if(t1ChannelNumber > 24)
							{
							t1BoardNumber++;
							t1ChannelNumber = 1;
							}
						break;

					case CT_NTANALOG:
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "CT_NTANALOG detected");
						break;

					case CT_NTE1:
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "CT_NTE1 detected");
						break;

					/*
					case CT_NTNONE:
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "CT_NTNONE detected");
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "t1ChannelNumber [%d]", t1ChannelNumber);

						if(t1ChannelNumber > 1)
							{
							sprintf(dlgctrls[cnt].t1Str, "dtiB%dT%d", t1BoardNumber, t1ChannelNumber);
							port[cnt].t1 = TRUE;
							t1ChannelNumber++;

							if(t1ChannelNumber > 24)
								{
								t1BoardNumber++;
								t1ChannelNumber = 1;
								}
							}
						break;
					*/

					default:
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "frontend unknown");
						break;
					}
				}
			else
				{
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndD4xMsg, 
								"error getting ct-info");
				}

			// Open T1 ports
			if(port[cnt].t1 == TRUE)
				{
				i = cnt;
				port[ cnt ].d4xT1Handle = dt_open( dlgctrls[ cnt ].t1Str, 0);
				if( port[ cnt ].d4xT1Handle == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg, 
									"Line %i, dt_open %s failed", 
									cnt, 
									dlgctrls[cnt].t1Str);
					
					UpdatePhoneLineStatusWindow(cnt, "open %s failed", dlgctrls[cnt].t1Str);	
					}
				else
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg,
									"Line %i T1 Device %s opened, handle: %i.",
									cnt,
									dlgctrls[cnt].t1Str,
									port[cnt].d4xT1Handle);

					//		Set to signal insertion mode
					if(dt_setsigmod(port[cnt].d4xT1Handle, DTM_SIGINS)== -1)
						{
						PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg,
										"Error in dt_setsigmod : %s.",
										ATDV_ERRMSGP(port[cnt].d4xT1Handle));
						}
					}

				// Make a full duplex connection between the voice device and dti TS device
				if(nr_scroute(port[cnt].d4xHandle, SC_VOX, port[cnt].d4xT1Handle, SC_DTI, SC_FULLDUP) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg, 
									"Error in nr_scroute");
					}
				}

            // cst mask:
            //   drop in loop current, rings recived,
            //   offhook completed, onhook completed
			//
			if(port[cnt].t1 == FALSE)
				{
				if(param.IVRPort[cnt].iType == 21 || param.IVRPort[cnt].iType == 27)
					{
					dx_setevtmsk(port[cnt].d4xHandle, DM_LCOFF | DM_LCON);
					}
				else if(param.DetectCurrent)
					{
					dx_setevtmsk(port[cnt].d4xHandle, DM_RINGS | DM_LCOFF);	
					}
				else
					{
					dx_setevtmsk(port[cnt].d4xHandle, DM_RINGS);
					}

				i = 1;
				if(dx_setparm(port[cnt].d4xHandle, DXCH_RINGCNT, (void *) &i) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"D4x error setting DXCH_RINGCNT: %s.", 
									ATDV_ERRMSGP(bddev));
					}

				// 2006-04-19
				//
				// set some dtmf parameters, dtmf duration, dtmf interdigit delay.
				//  units are 10ms. defaults were 5 for DXCH_T_IDD, and 10 for DXCH_TTDATA.
				//
				i = 10;		// 100 ms inter-digit delay.
				if(dx_setparm(port[cnt].d4xHandle, DXCH_T_IDD, (void *) &i) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"D4x error setting DXCH_T_IDD: %s.", 
									ATDV_ERRMSGP(bddev));
					}

				i = 15;		// 150 ms dtmf tones.
				if(dx_setparm(port[cnt].d4xHandle, DXCH_TTDATA, (void *) &i) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"D4x error setting DXCH_TTDATA: %s.", 
									ATDV_ERRMSGP(bddev));
					}
				}
			else
				{
				if(dt_setevtmsk((int) port[cnt].d4xT1Handle,
									(int)DTG_SIGEVT,
									(unsigned short)DTMM_AON | DTMM_AOFF | DTMM_BON | DTMM_BOFF,
									(int)DTA_SETMSK) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"Error dt_setevtmsk : %s.", 
									ATDV_ERRMSGP(port[cnt].d4xHandle));
					}

				i = 1;
				if(dx_setparm(port[cnt].d4xHandle, DXCH_RINGCNT, (void *) &i) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"D4x error setting DXCH_RINGCNT: %s.", 
									ATDV_ERRMSGP(bddev));
					}
				}				

			//		Setup Global Tone detection
	        dx_deltones( port[ cnt ].d4xHandle );
			errcode = -1;
			switch( param.GTD.repcnt )
				{
				case 0:
					if( param.GTD.freq2 > 0 )
						{
						errcode = dx_blddt(param.GTD.tid, 
											param.GTD.freq1, 
											param.GTD.fq1dev, 
											param.GTD.freq2, 
											param.GTD.fq2dev, 
											TN_LEADING);
						if( errcode != 0 )
							{
							PrintMessage(MSGTYPE_SYSTEM,  
											hwndD4xMsg, 
											"Line %i ERROR Unable to build dual tone: return %i.",
											cnt, 
											errcode);
							}
						}
					else if( param.GTD.freq1 > 0 )
						{
						errcode = dx_bldst( param.GTD.tid, param.GTD.freq1, param.GTD.fq1dev, TN_LEADING );
						if( errcode != 0 )
							{
							PrintMessage(MSGTYPE_SYSTEM,  
											hwndD4xMsg, 
											"Line %i ERROR Unable to build single tone: return %i.", 
											cnt, 
											errcode);
							}
						}
					break;

				default:
					if( param.GTD.repcnt == -1 ) param.GTD.repcnt = 0;
					if( param.GTD.freq2 > 0 )
						{
						errcode = dx_blddtcad(param.GTD.tid, 
												param.GTD.freq1, 
												param.GTD.fq1dev, 
												param.GTD.freq2, 
												param.GTD.fq2dev, 
												param.GTD.ontime, 
												param.GTD.ontdev, 
												param.GTD.offtime, 
												param.GTD.offtdev, 
												param.GTD.repcnt);
						if( errcode != 0 )
							{
							PrintMessage(MSGTYPE_SYSTEM,  
											hwndD4xMsg, 
											"Line %i ERROR Unable to build dual tone with cadence: return %i", 
											cnt, 
											errcode);
							}
						}
					else if( param.GTD.freq1 > 0 )
						{
						errcode = dx_bldstcad(param.GTD.tid, 
												param.GTD.freq1, 
												param.GTD.fq1dev, 
												param.GTD.ontime, 
												param.GTD.ontdev, 
												param.GTD.offtime, 
												param.GTD.offtdev, 
												param.GTD.repcnt);
						if( errcode != 0 )
							{
							PrintMessage(MSGTYPE_SYSTEM,  
											hwndD4xMsg, 
											"Line %i ERROR Unable to build single tone with cadence: return %i", 
											cnt,
											errcode);
							}
						}
					}

			if(errcode == 0)
				{
				errcode = dx_addtone( port[ cnt ].d4xHandle, NULL, 0);
				if( errcode != 0 )
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg, 
									"Line %i ERROR Tone not added return %i.", 
									cnt, 
									errcode);
					}
				}

			errcode = dx_adjsv(port[cnt].d4xHandle, 
								SV_SPEEDTBL, 
								SV_ABSPOS, 
								param.IVRPort[cnt].speedLevel);


			if(errcode != 0)
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg, 
								"Line %i ERROR setting speed %i return %i.",
								cnt, 
								param.IVRPort[cnt].speedLevel, 
								errcode);
				}

			errcode = dx_adjsv(port[cnt].d4xHandle, 
								SV_VOLUMETBL, 
								SV_ABSPOS, 
								param.IVRPort[cnt].volumeLevel);

			if(errcode != 0)
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg, 
								"Line %i ERROR setting volume %i return %i.",
								cnt, 
								param.IVRPort[cnt].volumeLevel, 
								errcode);
				}

			port[cnt].state = ST_ONHK;

			// this is a possible fix (set startup state to: ST_IDLE) for the 
			//  "ignored ring event at startup" problem, 
			//  startup already in the idle state... 
			//  but what other problems does it introduce???
			//  KGILL 2004-09-15
			//
			//port[cnt].state = ST_IDLE;

			if(port[cnt].t1 == FALSE)
				{
				// Analog line
				if(param.IVRPort[cnt].iType == 3 ||		// B/O == busied out
					param.IVRPort[cnt].iType == 5 ||	// Dialogic DTMF Leased Line (always off hook)
					param.IVRPort[cnt].iType == 21 ||	// DID 40 page in
					param.IVRPort[cnt].iType == 27 )	// DID 40 page status	
					{
					// these port types are always off-hook
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg,
									"XnD4xInitialize(): setting DX_OFFHOOK for analog line [%i]",
									cnt);

					dx_sethook(port[cnt].d4xHandle, DX_OFFHOOK, EV_ASYNC);
					}
				else
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg,
									"XnD4xInitialize(): setting DX_ONHOOK for analog line [%i]",
									cnt);

					dx_sethook(port[cnt].d4xHandle, DX_ONHOOK, EV_ASYNC);
					}
				}
			else
				{
				// T1 line
				if( dt_settssigsim( port[ cnt ].d4xT1Handle, param.t1CmdOnHook) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg,
									"dt_settssigsim error: %s",
									ATDV_ERRMSGP(port[cnt].d4xT1Handle));
					}
				else
					{
					SetNextD4xState(cnt, NULL);
					}
				}

            // set up the first state for each channel
            port[cnt].timer.wHour = 70;
            port[cnt].PageNumber = 1;
            port[cnt].PortStatus = 0;
            port[cnt].Attempts = 0;
            port[cnt].Errors = 0;
            port[cnt].Cancels = 0;
            port[cnt].External = 0;

            page[cnt].ScriptFlag = -1;
            page[cnt].ComNumber = -1;

            strncpy( page[ cnt ].PageStatus, "I", 2 );
			}
		}
    page[PAGE_LIST_POS].ScriptFlag = -1;
    page[PAGE_LIST_POS].ComNumber = -1;
    page[PAGE_LIST_POS].ComOffset = -1;
	}


	void
QueryService(void)
	{
	BOOL Done;
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	SERVICE_STATUS ssServiceStatus;
	char szService[] = "Dialogic";
	int Tries = 0;

	// connect to the service manager
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(schSCManager == NULL)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndD4xMsg, 
						"Cannot connect to service manager");
		return;
		}

	// Open a handle to the Dialogic service
	schService = OpenService(schSCManager,
								szService,
								(DWORD)(SERVICE_ALL_ACCESS | SERVICE_QUERY_STATUS));
	if(schService == NULL)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndD4xMsg,
						"Cannot open Dialogic service: [%d]",
						GetLastError());

		CloseServiceHandle(schSCManager);
		return;
		}

	// query service status
	PrintMessage(MSGTYPE_SYSTEM, 
					hwndD4xMsg,
					"Attempting to query and wait for the Dialogic service to be started");

	Done = FALSE;
	Tries = 3 * 60;		// allow 

	while(!Done && (Tries > 0))
		{
		if(QueryServiceStatus(schService, &ssServiceStatus) == TRUE)
			{
			switch(ssServiceStatus.dwCurrentState)
				{
				case SERVICE_START_PENDING:
					// The service is in the process of being started.
					Tries--;
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Dialogic service is pending... [%d]", Tries);
					Sleep(1000);
					break;

				case SERVICE_STOPPED:
					// The service is stopped, 
					//  wait for it to become pending, or time out after xx tries if it remains stopped.
					//
					// This change is intended to cope with a race condition after PC reboot.
					//  what we noticed is that at PC reboot, as the PC gets started, the service is "stopped" at first, 
					//  then goes "pending", and finally becomes "running".
					//  (previously, we would fail immediately if the service was found to be "stopped").
					//
					Tries--;
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Dialogic service is stopped... [%d]", Tries);
					Sleep(1000);			
					break;

				case SERVICE_RUNNING:
					// proceed to use the Dialogic service.
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "The Dialogic service is running.");
					Done = TRUE;
					break;

				case SERVICE_STOP_PENDING:           
					Tries--;
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Dialogic service is stop_pending... [%d]", Tries);
					Sleep(1000);			
					break;

				case SERVICE_CONTINUE_PENDING:      
					Tries--;
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Dialogic service is continue_pending... [%d]", Tries);
					Sleep(1000);			
					break;

				case SERVICE_PAUSE_PENDING:          
					Tries--;
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Dialogic service is pause_pending... [%d]", Tries);
					Sleep(1000);			
					break;

				case SERVICE_PAUSED:                
					Tries--;
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Dialogic service is paused... [%d]", Tries);
					Sleep(1000);			
					break;

				default:
					PrintMessage(MSGTYPE_SYSTEM, hwndD4xMsg, "Unknown Dialogic service state... [%d]", ssServiceStatus.dwCurrentState);

					// show a specific service error code if available.
					if(ssServiceStatus.dwWin32ExitCode == ERROR_SERVICE_SPECIFIC_ERROR)
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndD4xMsg,
										"The Dialogic service error is: [%ld]",
										ssServiceStatus.dwServiceSpecificExitCode);
						}
					Done = TRUE;
					break;

				}
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg,
							"Cannot query Dialogic service [%d]",
							GetLastError());

			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			return;
			}
		}
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	}


	void
XnD4xClose(void)
	{
	int i;

	for(i = 0; i < totalD4xChannels; i++)
		{
        if( param.ExitOnhk )
			{
			if( port[ i ].t1 == FALSE )
				{
				dx_sethook(port[i].d4xHandle, DX_ONHOOK,  EV_SYNC ); // it was EV_ASYNC changed by TK
				}
			else
				{
				if( dt_settssigsim( port[ i ].d4xT1Handle, param.t1CmdOnHook) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg,
									"dt_settssigsim error: %s",
									ATDV_ERRMSGP(port[i].d4xT1Handle));
					}
				}
			}
        else
			{
			if( port[ i ].t1 == FALSE )
				{
				dx_sethook(port[i].d4xHandle, DX_OFFHOOK, EV_SYNC); // it was EV_ASYNC , changed by TK
				}
			else
				{
				if( dt_settssigsim( port[ i ].d4xT1Handle, param.t1CmdOffHook) == -1)
					{
					PrintMessage(MSGTYPE_SYSTEM,  
									hwndD4xMsg,
									"dt_settssigsim error: %s",
									ATDV_ERRMSGP(port[i].d4xT1Handle));
					}
				}
			}

		if( port[ i ].t1 == TRUE )
			{
			//	Unroute the voice and the DTI before closing them
			nr_scunroute( port[i].d4xHandle, SC_VOX, port[ i ].d4xT1Handle, SC_DTI, SC_FULLDUP);
			dt_close( port[ i ].d4xT1Handle );
			}
		PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg,
						"Line %i Closing %s handle %i.",
						i,
						dlgctrls[i].str,
						port[i].d4xHandle);
		
		// allow the dialogic to digest all these commands before shutting
		//  the system down. Time is required to allow the commands to complete.
		dx_close(port[i].d4xHandle);
		}
	free(dlgctrls);
	}


	void
cpcinit(int i)
	{
	dx_clrcap(&ChannelParameter[i]);
    ChannelParameter[i].ca_hedge = 2;
    ChannelParameter[i].ca_intflg   = param.PageType[i].intflg;	// DX_OPTNOCON == 3, DX_OPTDIS == 2
																//  Enable Operator Intercept w/o Connect
    ChannelParameter[i].ca_stdely   = (unsigned short)param.PageType[i].stdely;
    ChannelParameter[i].ca_lowerfrq = (unsigned short)param.PageType[i].lowerfrq;
    ChannelParameter[i].ca_upperfrq = (unsigned short)param.PageType[i].upperfrq;
    ChannelParameter[i].ca_timefrq  = param.PageType[i].timefrq;
    ChannelParameter[i].ca_rejctfrq = param.PageType[i].rejctfrq;
    ChannelParameter[i].ca_intfltr  = param.PageType[i].intfltr;
    ChannelParameter[i].ca_nbrbeg   = param.PageType[i].nbrbeg;

    ChannelParameter[i].ca_cnosig   = 6000;	// 6000 * 10ms = 60 seconds
											// ca_cnosig, default is 4000 == 40 seconds.
	}


	int
GetNumberOfTitleLinesInPhoneLineStatusWindow(void)
	{
	const int HeaderLines = 3;

	return (int) HeaderLines;
	}


	void
WriteToPhoneLineStatusWindow(int Line, LPCSTR Text)
	{
	// disable re-draw temporarily.
	SendMessage(hwndD4xStatus, WM_SETREDRAW, (WPARAM) FALSE, (LPARAM) 0);

	// update line item.
	SendMessage(hwndD4xStatus, LB_DELETESTRING, (WPARAM) Line, (LPARAM) 0L);
	SendMessage(hwndD4xStatus, LB_INSERTSTRING, (WPARAM) Line, (LPARAM) (LPSTR) Text);

	// allow re-draw now.
	SendMessage(hwndD4xStatus, WM_SETREDRAW, (WPARAM) TRUE, (LPARAM) 0);
	}


	void
InitializePhoneLineStatusWindow(int IsStandby)
	{
	int L;

	// display the title/header lines:
	//
	WriteToPhoneLineStatusWindow(0, "Line    Line    Line          Line           ");
	WriteToPhoneLineStatusWindow(1, "Number  Type    Description   Status         ");
	WriteToPhoneLineStatusWindow(2, "------  ------  ------------  ---------------");

	// write empty info to each line.
	//
	for(L = 0; L < MAX_IVRPORTS; L++)
		{
		WriteToPhoneLineStatusWindow(GetNumberOfTitleLinesInPhoneLineStatusWindow() + L, 
										" ");
		}

	if(bCmdLineUseDialogicHardware)
		{
		// Dialogic hardware is enabled...
		//  no further action is required,
		//  actual status data will be updated as it changes.

		// post a standby message if requested.
		//
		if(IsStandby)
			{
			WriteToPhoneLineStatusWindow(4, "  Standing By ...");
			}
		}
	else
		{
		// not using Dialogic hardware.
		//  give an indication.
		//
		WriteToPhoneLineStatusWindow(4, "  Use of Dialogic IVR is Disabled");
		WriteToPhoneLineStatusWindow(5, "  By an Application Command-Line Option.");
		}

	}


	// display information in the "Phone Line Status" (IVR status screen)
	//  
	void
UpdatePhoneLineStatusWindow(int channel, LPSTR message, ...)
	{	
	va_list marker;
	char temp[80];
	char tempString[80];

	// if the window is not open, or has been closed,
	//  the handle should be NULL,
	//  skip any window access/update.
	if(hwndD4xStatus == NULL)
		{
		return;
		}

	// decide where to put the information inside the listbox,
	//  leaving header lines undisturbed.
	//
	int ListboxLineItemToUpdate = GetNumberOfTitleLinesInPhoneLineStatusWindow() + 
									channel;

	memset(tempString, 0, sizeof(tempString));

	// prepare the leftmost fields of the status line.
	sprintf(tempString, 
			"  %-02.2i     %-3.3s    %-12.12s  ", 
			channel + 1, 
			IVRPortDescriptionText[param.IVRPort[channel].iType],
			param.IVRPort[channel].UserDefinedShortDesignation);

	// add on more fields as requested.
	va_start(marker, message);
	vsprintf(tempString + strlen(tempString), message, marker);
	va_end(marker);

	memset(temp, 0, sizeof(temp));

	SendMessage(hwndD4xStatus, LB_GETTEXT, (WPARAM) ListboxLineItemToUpdate, (LPARAM) temp);

	if(strcmp(temp, tempString) == 0)
		{
		// if text is the same, no need to update.
		return;
		}
	else
		{
		WriteToPhoneLineStatusWindow(ListboxLineItemToUpdate, tempString);
		}
	}


	// process the misc rcc dialout scripts.
	int
CheckScript(int channel)
	{
    int i;
	int k;
    int PageNumber;
    int ScriptNumber;
    int errcode = 0;
    int WaitTime;
	DWORD rv;
	long termType;
    char Messages[255];
	char string2[MAX_PATH];

    PageNumber = channel;
    ScriptNumber = page[ PageNumber ].ScriptNumber;
    port[ channel ].PortStatus = 2;

    if( page[ PageNumber ].ScriptFlag < 0 )
		{
		page[ PageNumber ].ScriptFlag = 0;
		}

    switch( param.PageType[ page[ PageNumber ].ScriptNumber ].Script[ page[ PageNumber ].ScriptFlag ] )
		{
		case '1':
			UpdatePhoneLineStatusWindow(channel, 
										"Waiting %i", 
										param.PageType[ScriptNumber].wait1);

            if( port[ channel ].timer.wHour == 70 )
				{
				GetLocalTime( &port[ channel ].timer );
				}
            WaitTime = param.PageType[ ScriptNumber ].wait1 * 100;

            i = Wait( &port[ channel ].timer, (long)WaitTime );

            if( i )
				{
                page[ PageNumber ].ScriptFlag++;
                port[ channel ].timer.wHour = 70;
				}
			break;

		case '2':
			UpdatePhoneLineStatusWindow(channel, 
										"Waiting %i", 
										param.PageType[ScriptNumber].wait2);

            if( port[ channel ].timer.wHour == 70 )
				{
				GetLocalTime( &port[ channel ].timer );
				}

            WaitTime = param.PageType[ ScriptNumber ].wait2 * 100;

            i = Wait( &port[ channel ].timer, (long)WaitTime );

            if( i )
				{
                page[ PageNumber ].ScriptFlag++;
                port[ channel ].timer.wHour = 70;
				}
			break;

		case '3':
			UpdatePhoneLineStatusWindow(channel, 
										"Waiting %i", 
										param.PageType[ScriptNumber].wait3);

            if( port[ channel ].timer.wHour == 70 )
				{
				GetLocalTime( &port[ channel ].timer );
				}

            WaitTime = param.PageType[ ScriptNumber ].wait3 * 100;

            i = Wait( &port[ channel ].timer, (long)WaitTime );

            if( i )
				{
                page[ PageNumber ].ScriptFlag++;
                port[ channel ].timer.wHour = 70;
				}
			break;

		case 'C':
            GetLocalTime( &port[ channel ].timer );
            if( page[ PageNumber ].attempts == 0 )
				{
                UpdatePhoneLineStatusWindow(channel, "Calling Pager");
				}
            else
				{
                UpdatePhoneLineStatusWindow(channel, 
											"Re-Calling %i", 
											page[PageNumber].attempts + 1);
				}
            PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg,
							"Line %i Dialing: %s, Status %i. Attempts %i",
							channel,
							page[PageNumber].PagerNumber,
							port[channel].PortStatus,
							page[PageNumber].attempts + 1);

            cpcinit( ScriptNumber );
            errcode = dx_dial(port[channel].d4xHandle,
								page[PageNumber].PagerNumber,
								&ChannelParameter[ScriptNumber],
								DX_CALLP | EV_ASYNC);
			break;

        case 'c':
            GetLocalTime(&port[channel].timer);
            UpdatePhoneLineStatusWindow(channel, "Dialing");

            PrintMessage(MSGTYPE_SYSTEM,
							hwndD4xMsg, 
							"Line %i %s dialed", 
							channel, 
							page[PageNumber].PagerNumber);

            cpcinit( ScriptNumber );
            errcode = dx_dial(port[channel].d4xHandle, page[PageNumber].PagerNumber, NULL, EV_ASYNC);
			if( errcode == -1 )
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg,
								"Line %i Dial Error %i:%s Device %s",
								channel,
								ATDV_LASTERR(port[channel].d4xHandle),
								ATDV_ERRMSGP(port[channel].d4xHandle),
								ATDV_NAMEP(port[channel].d4xHandle));
				}
			break;

        case 'a':
            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow( channel, "Dialing PIN" );

            PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Line %i PIN %s dialed. Waiting for response.", 
							channel, 
							page[PageNumber].AutCode);

            cpcinit( ScriptNumber );
            errcode = dx_dial(port[channel].d4xHandle,
								page[ PageNumber ].AutCode,
								&ChannelParameter[ScriptNumber],
								DX_CALLP|EV_ASYNC);
			break;

		case 'A':
            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow( channel, "Dialing PIN" );

            PrintMessage(MSGTYPE_SYSTEM,
							hwndD4xMsg, 
							"Line %i PIN %s dialed", 
							channel, 
							page[PageNumber].AutCode);

            cpcinit(ScriptNumber);
            errcode = dx_dial(port[channel].d4xHandle, 
								page[PageNumber].AutCode, 
								NULL, 
								EV_ASYNC);
			break;

        case 'd':
            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow( channel, "Dialing Display");

            PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Line %i Display info %s dialed. Waiting for response.", 
							channel, 
							page[PageNumber].DisplayInfo);

            cpcinit( ScriptNumber );
            errcode = dx_dial(port[channel].d4xHandle,
								page[PageNumber].DisplayInfo,
								&ChannelParameter[ScriptNumber],
								DX_CALLP | EV_ASYNC);
			break;

		case 'D':
            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow( channel, "Dialing Display");

            PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg,
							"Line %i Display info %s dialed",
							channel,
							page[PageNumber].DisplayInfo);

			cpcinit(ScriptNumber);

            errcode = dx_dial(port[channel].d4xHandle,
								page[PageNumber].DisplayInfo,
								NULL,
								EV_ASYNC);
			if( errcode == -1 )
				{
				PrintMessage(MSGTYPE_SYSTEM,  
								hwndD4xMsg,
								"Line %i Dial Error %i:%s Device %s",
								channel,
								ATDV_LASTERR(port[channel].d4xHandle),
								ATDV_ERRMSGP(port[channel].d4xHandle),
								ATDV_NAMEP(port[channel].d4xHandle));
				}
			break;

        case 'V':
            dx_clrdigbuf( port[channel].d4xHandle );

            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow(channel, "Voice Message");

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}
			
			errcode = PlayFil(channel,
								page[PageNumber].FileName,
								&port[channel].file_handle,
								0);

			PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg,
							"V - PlayFil() parameters: Line [%i] Voice file [%s], fh [%i], played [%d]",
							channel,
							page[PageNumber].FileName,
							port[channel].file_handle,
							0);
			break;

		case 'I':
			dx_clrdigbuf( port[channel].d4xHandle );

        	memset(string2, 0, sizeof(string2));
        	strcat(string2, page[ PageNumber ].Extension);
        	getfile( string2, "VCE", sizeof(string2) );
        	rv = GetFileAttributes( string2 );
        	if( rv != 0xffffffff )
				{
				UpdatePhoneLineStatusWindow(channel, "Voice Greeting");

            	if(port[channel].file_handle > 0 )
					{
                	dx_fileclose(port[channel].file_handle);
                	port[channel].file_handle = 0;
            		}
				
				errcode = PlayFil(channel, string2, &port[ channel ].file_handle, 0);

            	PrintMessage(MSGTYPE_SYSTEM, 
								hwndD4xMsg,
								"I - Line %i Voice prompt %s.VCE, fh %i, ret %i.",
								channel,
								page[PageNumber].Extension,
								port[channel].file_handle,
								errcode);
				}
			else
				{
				UpdatePhoneLineStatusWindow(channel, "Voice Id");

            	Messages[0] = (char) (param.PromptType);		// "pager id"

            	i = 0;
            	k = 1;
            	memset(string2, 0, sizeof(string2));
            	strcpy(string2, page[PageNumber].PagerId);

            	if((int) strlen(string2) > param.IdPromptLength)
					{
                	memset(string2, 0, sizeof(string2));
                	strcpy(string2, 
							page[PageNumber].PagerId + 
							strlen(page[PageNumber].PagerId) - 
							param.IdPromptLength);
            		}

            	for(i = i; i < 10 && string2[i] != 0; i++)
					{
                	if(string2[i] > 47 && string2[i] < 58)
                    	Messages[k] = (char) (string2[i] - 48);
                	else break;
                	k++;
            		}
            	errcode = vb40_play(promptfh, channel, prompts, k, Messages);
			}
			break;

        case 'N': // overhead and voice paging
			dx_clrdigbuf( port[channel].d4xHandle );
            if( page[ PageNumber ].VoiceFile == 0 ){
                k = 0;
                if( strncmp( page[ PageNumber ].InitiatorId, "External", 8 ) == 0 || param.VoiceConv )
					{
                    Messages[ k ] = 51;		// "call"
                    k++;
					}
                for(i = 0; i < sizeof(page[PageNumber].DisplayInfo) && page[PageNumber].DisplayInfo[i] != 0; i++)
					{
					// 47 == 'G'     58 == 'X'   ???
                    if(page[PageNumber].DisplayInfo[i] > 47 &&
						page[PageNumber].DisplayInfo[i] < 58)
						{
                        Messages[k] = (char) (page[PageNumber].DisplayInfo[i] - 48);
                        k++;
						}
                    else if(param.VoiceConv)
						{
                        switch(page[PageNumber].DisplayInfo[i])
							{
                            case '*':
                            case 'X':
                                if(param.StarPrompt > 1)
									{
                                    Messages[k] = (char) (param.StarPrompt - 1); k++;
									}
                                break;
                            case 'C':
                                Messages[ k ] = 51; k++;	// "code completed"
                                break;
                            case 'H':
                                Messages[ k ] = 90; k++;	// "home"
                                break;
                            case 'B':
                                Messages[ k ] = 91; k++;	// "back"
                                break;
                            case 'E':
                                Messages[ k ] = 92; k++;	// "emergency"
                                break;
                            case 'N':
                                Messages[ k ] = 93; k++;	// "number"
                                break;
                            case 'R':
                                Messages[ k ] = 94; k++;	// "operating room"
                                break;
                            case 'S':
                                Messages[ k ] = 95; k++;	// "stat"
                                break;
                            case 'P':
                                Messages[ k ] = 96; k++;	// "page"
                                break;
                            case 'O':
                                Messages[ k ] = 69; k++;	// "operator"
                                break;
                            default:
                                break;
							}
						}
					}

                errcode = vb40_play(promptfh, channel, prompts, k, Messages);

                PrintMessage(MSGTYPE_SYSTEM, 
								hwndD4xMsg,
								"Line %i Talking message: %s",
								channel,
								page[PageNumber].DisplayInfo);
				}
            else
				{            	
				UpdatePhoneLineStatusWindow(channel, "Voice File");

				errcode = PlayFil(channel,
									page[PageNumber].FileName,
									&port[channel].file_handle,
									0);

                PrintMessage(MSGTYPE_SYSTEM, 
								hwndD4xMsg,
								"N - Line %i Playing voice file: %s, File Handle: %i.",
								channel,
								page[PageNumber].FileName,
								port[channel].file_handle);
				}
            break;

        case '#':
            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow( channel, "Dialing #" );
            cpcinit( ScriptNumber );
            errcode = dx_dial( port[channel].d4xHandle, "#", NULL, EV_ASYNC );
			break;

        case '@':
        case '$':
            GetLocalTime( &port[ channel ].timer );
            UpdatePhoneLineStatusWindow( channel, "Calling #" );
            cpcinit( ScriptNumber );
            errcode = dx_dial(port[channel].d4xHandle,
								"#",
								&ChannelParameter[ScriptNumber],
								DX_CALLP|EV_ASYNC );
            break;

        case 'T':
            UpdatePhoneLineStatusWindow( channel, "Playing Tone" );
            Messages[ 0 ] = 33;
            errcode = vb40_play( promptfh, channel, prompts, 1, Messages );
            break;

        case 'O':
			// swallow any events on this channel before starting to use it.
			//  implement this with dx_stopch()
			//
			// dx_stopch( port[channel].d4xHandle, EV_ASYNC );
			// PrintMessage(MSGTYPE_SYSTEM,  
			//				hwndD4xMsg, 
			//				"Channel %i stopped.",
			//				channel);

            GetLocalTime( &port[ channel ].timer );
			errcode = XnSetHook( channel, DX_OFFHOOK );
            UpdatePhoneLineStatusWindow( channel, "Off Hook" );

			PrintMessage(MSGTYPE_SYSTEM,  
							hwndD4xMsg, 
							"Line %i Offhook", 
							channel);
            break;

		case 'H':
            UpdatePhoneLineStatusWindow( channel, "On Hook" );

            if( port[ channel ].file_handle > 0 )
				{
                dx_fileclose( port[ channel ].file_handle );
                port[ channel ].file_handle = 0;
				}

            port[ channel ].timer.wHour = 70;
            port[ channel ].state = ST_ONHK_DELAY;
			errcode = XnSetHook( channel, DX_ONHOOK );
			termType = ATDX_CPTERM( port[channel].d4xHandle );

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg,
							"Line %i OnHook termtype: %i, %ld Hz",
							channel,
							termType, 							
							ATDX_FRQHZ(port[channel].d4xHandle));

            if(termType == CR_CEPT ||
				(termType == CR_CNCT && param.PageType[ ScriptNumber ].nbrbeg <= 1) ||
				param.PageType[ScriptNumber].nbrbeg == 0)
				{
                page[ PageNumber ].attempts = 0;
                page[ PageNumber ].PageStatus[0] = 'I';
                port[ channel ].PortStatus = 1;
				}
			else
				{
                UpdatePhoneLineStatusWindow( channel, "Error: On Hook" );
                page[ PageNumber ].PageStatus[0] = 'A';
                port[ channel ].PortStatus = 1;
                port[ channel ].timer.wHour = 70;
				}
			break;

        case 'E':
            UpdatePhoneLineStatusWindow( channel, "Page Completed" );
            port[ channel ].timer.wHour = 70;
            port[ channel ].state = ST_ONHK_DELAY;
			termType = ATDX_CPTERM( port[channel].d4xHandle );

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg,
							"Line %i termtype: %i, %ld Hz",
							channel,
							termType,
							ATDX_FRQHZ(port[channel].d4xHandle));

			if( termType == CR_CEPT ||
				(termType == CR_CNCT && param.PageType[ScriptNumber].nbrbeg == 0))
				{
                page[ PageNumber ].attempts = 0;
                page[ PageNumber ].PageStatus[0] = 'I';
                port[ channel ].PortStatus = 1;
				}
			else
				{
                UpdatePhoneLineStatusWindow( channel, "ERROR Terminated" );
                page[ PageNumber ].PageStatus[0] = 'A';
                port[ channel ].PortStatus = 1;
                port[ channel ].timer.wHour = 70;
				}
			break;
		}
    return( errcode );
	}


/****************************************************************
*        NAME : vbinit(namep,vbtable,max)
* DESCRIPTION : init vbase40 header tables
*       INPUT : namep = the name of the vbase40 disk file
*             : vbtable = pointer to a vbase40 data structure
*             : max = number of entries in data structure
*      OUTPUT : offset/size of each vbase40 entry copied to vbtable
*     RETURNS : file handle of the vbase40 disk file,
*             : exits if dos error reading the vbase40 file
****************************************************************/
	int
vbinit( char * namep, struct vbase40 vbtable[ ], int max )
	{
    unsigned long vbdata[6];
    int n, entries, fh, xfrcnt;

    // open the voice file (created using vbase/40)
    fh = _open( (char* ) namep, O_RDONLY | O_BINARY, S_IREAD | S_IWRITE);

    if( fh > 0 )
		{
		//
		// read in the header information
		//
		// The start of a vbase40 file contains 6 - long words of
		// data.  The third word, contains the number of entries
		// in the vbase40 file.
		//
		xfrcnt = _read(fh, (char *) vbdata, 6 * sizeof(unsigned long));

		if(xfrcnt == 6 * sizeof(unsigned long))
			{
			entries = (long)vbdata[0];

			PrintMessage(MSGTYPE_SYSTEM,
							hwndD4xMsg, 
							"Voice file contains %i prompts.",
							entries);

			// check for too many vbase40 entries
			if( entries > max ) entries = max;

			//
			// read in list of offsets and lengths for each entry
			//
			// Each entry is 3 - long words of data.  The first word
			// is the offset into the file where the prompt starts,
			// the second word is the length (in bytes) of the prompt.		
			//
			for (n=0; n<entries; n++)
				{
				xfrcnt = _read(fh, (char *) vbdata, 3 * sizeof(long));
				if(xfrcnt < 3 * sizeof(long))
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndD4xMsg, 
									"ERROR!!! Read error on vbase40 file %s", 
									namep);
					break;
					}
				vbtable[n].ofst = vbdata[0];
				vbtable[n].size = vbdata[1];
				}
			_close( fh );
			fh = dx_fileopen(namep, O_RDONLY | O_BINARY);
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndD4xMsg, 
							"ERROR Read error on vbase40 file %s", 
							namep);
			}
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndD4xMsg, 
						"ERROR Can not open vbase40 file %s", 
						namep);
		}

    return(fh);
	}


/****************************************************************
*        NAME : vb40_play(channel,vbheader,entries,digitp)
* DESCRIPTION : initiate play from vbase40 digit file
*       INPUT : channel = channel number
*             : vbheader = vbase40 header structure
*             : entries = number of entries in digitp
*             : digitp = pointer to ascii array of digits
*      OUTPUT : none
*     RETURNS : pass/fail status of the indexed play function
****************************************************************/
	int
vb40_play(int fh, int channel, struct vbase40 * vbheader, int entries, char * digitp)
	{
	static DV_TPT tpt[MAX_IVRPORTS + 1];
	int n, dig;
	bool OkToPrint = false;

	// note: no printing is allowed when entries == 0
	//  the data items pointed to when entries == 0
	//  are not valid.
	//  
	if(entries > 0)
		{
		OkToPrint = true;
		}

	tpt[channel].tp_type = IO_EOT;
	tpt[channel].tp_termno = DX_MAXDTMF;
	tpt[channel].tp_length = 1;
	tpt[channel].tp_flags = TF_MAXDTMF;

	// no prompts to play... play a null, or silent prompt.
	//  note that each of the .VAP files has a prompt #150 (1 based)
	//  that is intended as silent.
	// see definitions of the .VAP file - located in defines.h.
	if(entries == 0)
		{
		entries = 1;
		digitp[0] = (char) (unsigned char) VOX_STATUS_Silent_Prompt;
		}

	// build/initialize the indexed play data structure
	for(n = 0; n < entries; n++)
		{
		dig = *digitp;
		digitp++;

		indexplay[channel][n].io_type = IO_DEV | IO_CONT;
		indexplay[channel][n].io_bufp = 0;
		indexplay[channel][n].io_nextp = NULL;
		indexplay[channel][n].io_fhandle = fh;
		indexplay[channel][n].io_offset = vbheader[dig].ofst;
		indexplay[channel][n].io_length = vbheader[dig].size;

		//  show text of recording to be played.
		if(OkToPrint && fh == Digitsfh)
			{
			PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,
							hwndD4xMsg,
							"vb40_play(): Digits [%02d] [%03d] [%s]",
							n,
							dig,
							DigitsText[dig]);
			}
		else if(OkToPrint && fh == StatusFh)
			{
			PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,
							hwndD4xMsg,
							"vb40_play(): Status [%02d] [%03d] [%s]",
							n,
							dig,
							StatusText[dig]);
			}
		else if(OkToPrint && fh == promptfh)
			{
			PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,
							hwndD4xMsg,
							"vb40_play(): Prompt [%02d] [%03d] [%s]",
							n,
							dig,
							PromptText[dig]);
			}
		else
			{
			if(OkToPrint)
				{
				PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,
								hwndD4xMsg,
								"vb40_play(): Digitsfh %d   StatusFh %d  promptfh %d", 
								Digitsfh, 
								StatusFh, 
								promptfh);

				PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,
								hwndD4xMsg,
								"vb40_play(): unrecognized prompt file handle %d", 
								fh);
				}
			}
		}

	indexplay[channel][n-1].io_type = IO_DEV | IO_EOT;



	int err = dx_play(port[channel].d4xHandle,
		&indexplay[channel][0],
		&tpt[channel],
		(unsigned short) (EV_ASYNC | param.VoiceFileFormat));

	PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,	hwndD4xMsg,	"using vb40_play()");

	return err;
	}


	// a variation of vb40_play() from above,
	//  that is designed to stop on 0 thru 9 or star.
	int
vb41_play(int fh, int channel, struct vbase40 *vbheader, int entries, char *digitp)
	{
	static DV_TPT tpt[MAX_IVRPORTS + 1][2];
	int n, dig;

	// see dialogic documentation - Table 19. tp_length Settings.
	tpt[channel][0].tp_termno = DX_DIGMASK;
	tpt[channel][0].tp_length = DM_0 | DM_1 | DM_2 | DM_3 |
								DM_4 | DM_5 | DM_6 | DM_7 |
								DM_8 | DM_9 | DM_S;
	tpt[channel][0].tp_flags = TF_DIGMASK;
	tpt[channel][0].tp_type = IO_EOT;			// last structure.

	// no prompts to play... play a null, or silent prompt.
	//  note that each of the .VAP files has a prompt #150 (1 based)
	//  that is intended as silent.
	// see definitions of the .VAP file - located in PageWin4 project (defines.h).
	//
	if(entries == 0)
		{
		entries = 1;
		digitp[0] = (char) (unsigned char) (150 - 1);		// 150 is VOX_STATUS_Silent_Prompt.
		}



	// build/initialize the indexed play data structure
	for(n = 0; n < entries; n++)
		{
		dig = (*digitp++);
		indexplay[channel][n].io_type = IO_DEV | IO_CONT;
		indexplay[channel][n].io_bufp = 0;
		indexplay[channel][n].io_nextp = NULL;
		indexplay[channel][n].io_fhandle = fh;
		indexplay[channel][n].io_offset = vbheader[dig].ofst;
		indexplay[channel][n].io_length = vbheader[dig].size;
		}

	indexplay[channel][n-1].io_type = IO_DEV|IO_EOT;

	PrintMessage(MSGTYPE_IVR_VOICE_PROMPT_TEXT,	hwndD4xMsg,	"using vb41_play()");

	return(dx_play(port[channel].d4xHandle,
					&indexplay[channel][0],
					&tpt[channel][0],
					(unsigned short) (EV_ASYNC | param.VoiceFileFormat)));
	}


	void
D4xWorkerThread(void)
	{
	int chdev;
	int channel;
	int i;
	DX_CST *cstp;
	int evtcode;
	int errcode;

	// NOTE: ring at startup detect, and re-generate later feature, is
	//       ANALOG ONLY... NOT IMPLEMENTED FOR T1.
	//

	// a simple technique to capture a possible
	//  ring event at system startup.  the event will be
	//  caught and then re-generated a short time later, after
	//  on-hook initialization has been completed.
	//  for now... this is "timing" based.  (a sloppy method).
	//
//	static bool bFirstTimeThru = true;
//	static bool bCaptureMode = true;
//	static bool bRingingAtStartup[MAX_IVRPORTS];
//	static SYSTEMTIME stStartupTimer;		

/*
	if(bFirstTimeThru)
		{
		bFirstTimeThru = false;

		// initialize all "ringing" flags.
		for(int i = 0; i < MAX_IVRPORTS; i++)
			{
			bRingingAtStartup[i] = false;
			}

		// start a timer, that is used to catch ring events at startup
		//  and then release them a short time later.
		GetLocalTime(&stStartupTimer);
		}
*/

/* disable for all lines, analog and t1, for now...

	// a one-time "re-generation" of ring events 
	//  XX seconds after startup.
	//
	if(bCaptureMode && (Wait(&stStartupTimer, (long) 5 * 100)))
		{
		// it is now time to release any ring events 
		//  that were captured at startup.
		bCaptureMode = false;

		// for each "ringing-at-startup" flag....

		DX_CST cst;
		cst.cst_event = DE_RINGS; 
		cst.cst_data = 0; 

		for(int i = 0; i < totalD4xChannels; i++)
			{
			if(bRingingAtStartup[i] && 
				((port[i].d4xHandle > 0) || (port[i].d4xT1Handle > 0)) )
				{
				// was ringing at startup, and the port handle is known.
				//  now "re-generate" a ring event.

				// array index is 0 thru (N - 1), 
				//  channel handles, are arbitrary numbers, 
				//  saved from when the port was opened. 
				//
				sr_putevt(port[i].d4xHandle, TDX_CST, sizeof(DX_CST), &cst, 0);

				PrintMessage(MSGTYPE_SYSTEM,
								hwndD4xMsg,
								"Re-Generating ring event for channel: [%d] handle: [%d]",
								i,
								port[i].d4xHandle);
				}
			}
		}
*/

	if(sr_waitevt(0) != -1)
		{
		chdev = sr_getevtdev(0);
		evtcode = sr_getevttype(0);

		// examples:
		//  TDX_CST == call status transition event == 0x86
		//  TDX_PLAY == play completed == 0x81
		//  TDX_ERROR == error event == 0x89

		cstp = (DX_CST *) sr_getevtdatap(0);

		// an event occurred for chdev.
		//  chdev, a handle... must be translated / recognized 
		//  for use by the state machine, and to be used as an array index.
		//
		channel = -1;	// means not-found

		for(i = 0; i < totalD4xChannels; i++)
			{
			if(port[i].d4xHandle == chdev || port[i].d4xT1Handle == chdev)
				{
				channel = i;
				break;
				}
			}
/*
		// if the event is a "ring event" that happened at startup,
		//  a flag is set to indicate that it was detected, and 
		//  can be released / regenerated shortly after startup initialization.
		//
		if(bCaptureMode && 
			(channel != -1) && 
			(evtcode == TDX_CST) && (cstp->cst_event == DE_RINGS))
			{
			// set the corresponding boolean flag.
			//
			bRingingAtStartup[channel] = true;

			PrintMessage(MSGTYPE_SYSTEM,
							hwndD4xMsg,
							"Handle from sr_getevtdev(): [%d] channel: [%d] RINGING AT STARTUP!, Handle from array: [%d]",
							chdev,
							channel,
							port[channel].d4xHandle);
			}
*/
		// print a message for each event if this option is turned on.
		if(d4xDiags == TRUE)
			{
			PrintMessage(MSGTYPE_SYSTEM,
							hwndD4xMsg,
							"D4xWorkerThread(): Line[%i] Event[0x%0x] Term Mask[0x%0x] State[%s]",
							channel,
							evtcode,
							ATDX_TERMMSK(chdev),
							DialogicStateName[port[channel].state]);
			}

		if(channel != -1)
			{
			// capture the current state in variable i.
			//  for detecting subsequent state change.
			i = port[channel].state;

			// process the event, and save the next channel state
			port[channel].state = process_event(channel, evtcode, chdev, cstp);

			// begin the new state, check error code on return
			switch(evtcode)
				{
				case DTEV_SIG:   // 0x204d == Signaling Event
					if(i == port[channel].state)
						{
						// old state == current state,
						//  no state change due to call to process_event().
						//  do not call next_state();
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndD4xMsg, 
										"DTEV_SIG Ignored");
						break;
						}

				case TDX_CST:	// 0x86 == CST Event
					if(cstp->cst_event == DE_TONEOFF)
						{
						// ignore TDX_CST sub-type DE_TONEOFF events.
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndD4xMsg, 
										"TDX_CST sub-type DE_TONEOFF Ignored");
						break;
						}

				default:
					// advance to the next state.
					errcode = next_state(channel, port[channel].d4xHandle);

					if(errcode != 0)
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndD4xMsg,
										"Line %d, ERROR:%d %s, state %d",
										channel,
										errcode,
										ATDV_ERRMSGP(port[channel].d4xHandle),
										port[channel].state);

						UpdatePhoneLineStatusWindow(channel, "ERROR");
						page[channel].PageStatus[0] = 'I';
						port[channel].PortStatus = 1;
						port[channel].Errors++;
						port[channel].timer.wHour = 70;
						port[channel].state = ST_ONHK_DELAY;

						XnSetHook(channel, DX_ONHOOK);
						}
					break;
				}
			}
		}
	}


	void
SetNextD4xState(int channel, DX_CST *cstp)
	{
	int errcode;

	port[ channel ].state = process_event( channel, TDX_SETHOOK, port[ channel ].d4xHandle, cstp);

	errcode = next_state( channel, port[ channel ].d4xHandle);

	if(errcode != 0)
		{
		PrintMessage(MSGTYPE_SYSTEM,  
						hwndD4xMsg,
						"Line %d, ERROR: %s, state %d",
						channel,
						ATDV_ERRMSGP(port[channel].d4xHandle),
						port[channel].state);

		UpdatePhoneLineStatusWindow(channel, "ERROR");
		page[ channel ].PageStatus[0] = 'I';
		port[ channel ].PortStatus = 1;
		port[ channel ].Errors++;
		port[ channel ].timer.wHour = 70;
		port[ channel ].state = ST_ONHK_DELAY;

		XnSetHook( channel, DX_ONHOOK );
		}
	}



