/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1990-2007 Dialogic Corporation. All Rights Reserved.
* The source code contained or described herein and all documents related
* to the source code ("Material") are owned by Dialogic Corporation or its
* suppliers or licensors. Title to the Material remains with Dialogic Corporation
* or its suppliers and licensors. The Material contains trade secrets and
* proprietary and confidential information of Dialogic or its suppliers and
* licensors. The Material is protected by worldwide copyright and trade secret
* laws and treaty provisions. No part of the Material may be used, copied,
* reproduced, modified, published, uploaded, posted, transmitted, distributed,
* or disclosed in any way without Dialogic's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery
* of the Materials, either expressly, by implication, inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be
* express and approved by Dialogic in writing.
*
***********************************@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********/

/******************************************************************************** 
 * FILENAME:    msilib.h
 * DESCRIPTION:  
 ********************************************************************************/

#ifndef __MSILIB_H__
#define __MSILIB_H__

/*
 * Device specific identifiers
 */
#define DT_MSI    0x00004000               /* MSI device class. NOTE: do not use 0x40C8. This is already used in scxlib.h */
#define DT_LAST_EVENT	(DT_MSI | 0xFF)		/* last event value */

#define DI_MSI    (DT_IO|DT_MSI)           /* Device is a MSI */
#define DI_MSICH  (DT_IO|DT_MSI|DT_CH)     /* Device is a MSI channel */

#define DI_MSIC   (DT_IO|DT_MSI|'C')       /* Device is a MSI/C */
#define DI_MSICCH (DT_IO|DT_MSI|DT_CH|'C') /* Device is a MSI/C channel */

#define MSEV_RING  (DT_MSI | 0x4D)         /* MSI Ring successful event */
#define MSEV_NORING (DT_MSI | 0x4e)        /* MSI ring unsuccessful event */
#define MSEV_ERREVT DTEV_ERREVT            /* Define error event */
#define MSEV_SIGEVT DTEV_SIG               /* Signalling event generated */
#define MSEV_SIGMSK DTG_SIGEVT             /* Equate to be used in setevtmsk */
#define MSEV_SETCHANSTATE (DT_MSI | 0x4F)  /* Set Channel State successful event */ 
#define MSEV_DATASENT (DT_MSI | 0x50)      /* Data successfully sent event */
#define MSEV_SENDDATAFAILED (DT_MSI | 0x51)/* Send data failed event*/
#define MSEV_LISTEN					(DT_MSI | 0x52)/*  listen event */
#define MSEV_LISTEN_FAIL			(DT_MSI | 0x53)/*  listen error */
#define MSEV_UNLISTEN				(DT_MSI | 0x56)/*  unlisten event*/
#define MSEV_UNLISTEN_FAIL			(DT_MSI | 0x55)/*  unlisten error*/

#define MSEV_CHANSTATE (DT_MSI | 0x54)     /* Channel state for alarms */

/*
 * Clocking Sources
 */
#define MS_NOCLK   0x00      /* No Clock */
#define MS_EXPBUS  0x02      /* Clock from Expansion Bus */
#define MS_INDEP   0x03      /* 1.544 MHz - Independent Timing */

/*
 * MSI/SC Clock Modes
 */
#define MSI_CLK_MASTER   0x10  /* MSI is clock Master */
#define MSI_CLK_RESOURCE 0x20  /* MSI is a Resource board */
#define MSI_CLK_SLAVE    0x30  /* MSI is slave Master */

/*
 * Device Parameters for ms_setbrdparm
 */
#define MSG_RING					0x06	/* (non)Ringing Board indicator */
#define MSG_DBONTM					0x22	/* Debounce on time */
#define MSG_DBOFFTM					0x23	/* Debounce off time */
#define MSG_MINFLASH				0x24	/* Minimum flash hook time */
#define MSG_MAXFLASH				0x25	/* Maximum flash hook time */
#define MSG_CLKMODE					0x26	/* Clk Mode */
#define MSG_ZIPENA					0x27	/* Zip Tone */
#define MSG_RNGCAD					0x31	/* Ring Cadence */
#define MSG_ACTID					0x32	/* Enable/Disable Active Talker  */
#define MSG_VOLDIG					0x33	/* Volume Control Digits         */
#define MSG_UDRNGCAD				0x31	/* User-Defined Ring Cadence */
#define MSG_PDRNGCAD				0x34	/* Pre-Defined Ring Cadence */
#define MSG_DISTINCTRNG				0x35	/* Multiple Ring Cadence */
#define MSG_RESTBL					0x50	/* Conf.Table Update Event Parm */ 
#define MSG_FIRSTRINGNOTIFY			0x36	/* Enable/Disable the notification of the first ring */
#define	MSG_ALGORITHM				0x37	/* Select DCB talkers Algorithm : Longest / Loudest*/
#define	MSG_LOUDENGTHR				0x38	/* Select Loudest talker threshold */
#define	MSG_TONECLAMP				0x39	/* Tone Clamping Parameter */
#define	MSG_ACTTALKNOTIFYINTERVAL	0x40	/* Active Talker Notify Interval */

/*
 * ToneClamping ON/OFF
 */
#define TONECLAMP_OFF      0x0000     /* Tone Clamping Disabled*/
#define TONECLAMP_ON       0x0001     /* Tone Clamping Enabled*/

#define ZIP_ON	      0x01	/* ziptone enable */
#define ZIP_OFF	      0x00	/* ziptone disable */

#define ACTID_ON        0x01 /* Active Talker detect enable   */
#define ACTID_OFF       0x00   /* Active Talker detect disable  */

#define	ALGO_LONG		0x00	/* Active Talker Algorithm - Longest	*/
#define	ALGO_LOUD		0x01	/* Active Talker Algorithm - Loudest	*/
 
 /*
 * Channel Parameters
 */
#define MSCP_GNXMT    0x40      /* Transmit Gain on CODEC */
#define MSCP_GNRCV    0x41      /* Receive Gain on CODEC */
#define MSCP_CHPWR    0x43      /* Channel Power State */
#define MSCP_FLTR1    0x44      /* Set coefficient of codec filter 1 */
#define MSCP_FLTR2    0x45      /* Set coefficient of codec filter 2 */
#define MSCP_FLTR3    0x46      /* Set coefficient of codec filter 3 */

/*
 * Station Parameters
 */
#define MSSP_STPWR	MSCP_CHPWR
/*
 * Channel Power ON/OFF
 */
#define MS_PWROFF     0x0000    /* Channel in Power Off State */
#define MS_PWRON      0x0001    /* Channel in Power On State */

/*
 * Message Waiting Indicator ON/OFF
 */
#define MS_MSGINDOFF     0x0000    /* Message Waiting Indicator in Off State */
#define MS_MSGINDON      0x0001    /* Message Waiting Indicator in On State */

/*
 * Channel Status
 */
#define MS_ONHOOK     0x00      /* On Hook */
#define MS_OFFHOOK    0x10      /* Off Hook */

/*
 * Message mask and event equates
 */
#define MSMM_OFFHOOK   0x0011      /* Mask for channel went off hook */
#define MSMM_RNGOFFHK  0x0004      /* Solicited off hook */
#define MSMM_RNGSTOP   0x0020      /* User stopped ringing by stopfn() cmd */
#define MSMM_ONHOOK    0x0110      /* Mask for channel went on hook */
#define MSMM_HOOKFLASH 0x1000      /* Mask for hook flash detection */
#define MSMM_TERM      0x8000      /* Mask for ring termination */
#define MSMM_FIRSTRING 0x0040	   /* Mask for first ring off */

/*
 * Message mask for MSEV_CHANSTATE event
 */
#define MSMM_CS_ALARM			0x0001	   /* Message mask for alarm channel state */
#define MSMM_CS_IDLE			0x0002	   /* Message mask for idle channel state */
#define MSMM_CS_OUT_OF_SERVICE	0x0004	   /* Message mask for out-of-service channel state */


/* 
 * MSI Conferencing Equates 
 */
#define MAXPTY_ESTCONF  4               /* Max # of parties for estab. conf. */
#define MAXRINGS        0xFF            /* Max # rings allowed (255) */
#define MAXCONFID       0xFFFF          /* Max Conference ID (65535) */

#define TS_ROUTE        0x01            /* Allocate ts for PCM */
#define TS_CONF         0x02            /* Allocate TS for CONF */

#define VOLADJ          0x00            /* Adjust Volume        */
#define VOLRES          0x01            /* Reset Volume         */

#define MS_ZIPDISABLE   0x00            /* Disable Zip Tone */
#define MS_ZIPENABLE    0x01            /* Enable Zip Tone */

#define MTF_RING        0x8a            /* Stop Ringing */

#define MS_BRDPRESENT   0x01            /* Board present */
#define MS_NOBRD        0xFF            /* No Board */
#define MS_RNGBRD	0x01		/* MSI/SC-R Board */
#define MS_NORNGBRD	0x00		/* MSI/SC Board */

#define MSI_SC_PRODID				0x3001      /* MSI/SC        Product ID */
#define MSI_240_SC_R1_FW_CTRL_RING	0x3011		/* MSI/240SC Rev 1 with firmware controlled ringing Product ID*/
#define MSI_80_SC_GLOBAL			0x3021		/* MSI/80SC-Global Product ID*/
#define MSI_80_PCI_GLOBAL			0x3031		/* MSI/80PCI-Global Product ID*/
#define HDSI_PRODID					0x7003      /* HDSI          Product ID */
#define DI_0408_LSA_PRODID			0x8001      /* DI_0408       Product ID */
#define DI_SI_32_PRODID				0x8004      /* DI_SI32       Product ID */
#define DI_0408_IP_R2_PRODID		0x8019      /* DI_0408_IP_R2 Product ID */

#define DEV_CLASS_MSI   "MSI"

/*
 * MSI Specific Error Codes
 */
#define E_MSBADVAL			0xAC	/* Bad Value */
#define E_MSINVTS			0xC0	/* Invalid Timeslot Number */
#define E_MSNOTS			0xC1	/* No Timeslot Assigned to Channel */
#define E_MSBADCHPARM		0xC2	/* Bad Channel Parameter Number */
#define E_MSINVFEMID		0xC3	/* Invalid ID Read from FEM ID PAL */
#define E_MSINVCLK			0xC4	/* Invalid Clock Rate for present PEB rate */
#define E_MSINVPEB			0xC5	/* Invalid PEB Rate for present Clock rate */
#define E_MSTSNOTEQ			0xC6	/* Tslots not equal (for zip tones) */
#define E_MSNOFEMCH			0xC7	/* NO FEM to Support this Channel */
#define E_MSGLOBREAD		0xC8	/* Can Not Read this Parameter Globally */
#define E_MSZIPEN			0xC9	/* Zip tones disabled - msg not allowed */
#define E_MSINVPTYCNT		0xCA	/* Invalid number of parties in msg. */
#define E_MSINVCNF			0xCB	/* Invalid conference number */
#define E_MSINVPTYNUM		0xCC	/* Invalid party number specified */
#define E_MSPTYASN			0xCD	/* Party already assigned */
#define E_MSSNDZIP			0xCE	/* Sending a zip tone to this station */
#define E_MSCNFFUL			0xCF	/* Conference is full */
#define E_MSNOCNF			0xD0	/* Conferencing is not available */
#define E_MSINVCB			0xD1	/* Invalid control block ID */
#define E_MSTSASNCNF		0xD2	/* Time slot already assigned to cnf. */
#define E_MSNONCNFCH		0xD3	/* Channel is not in spcfd cnf. */
#define E_MSCHASNCNF		0xD4	/* Channel is assigned to cnf. */
#define E_MS1PTY			0xD5	/* Cannot remove party from 1 party cnf. */
#define E_MSINVPATTR		0xD6	/* Invalid party attribute */
#define E_MSTSASN			0xD7	/* Timeslot already assigned to a station */
#define E_MSSTASN			0xD8	/* Timeslot already assigned to a station */
#define E_MSINVST			0xD9	/* Invalid station */
#define E_MSINVVAL			0xDA	/* Bad global parameter value */
#define E_MSBADRNGSTA		0xDB	/* Can't ring station: station busy */
#define E_MSINVCATTR		0xDC	/* Invalid Conference Attribute/Selector */
#define E_MSINVPTYTYPE		0xDD	/* Invalid Conference member type */
#define E_MSINVDSP			0xDE	/* Invalid DSP specified */
#define E_MSNODSPTS			0xDF	/* All timeslots going to the DSP are busy */
#define E_MSINVXTD			0xE0	/* Invalid extended connection number */
#define E_MSNOCNT			0xE1	/* Station not connected */
#define E_MSCNTXTD			0xE2	/* Station is in extended connection */
#define E_MSMONEXT			0xE3	/* Monitor already exists for this conference */
#define E_MSNOMON			0xE4	/* No monitor exists for this conference */
#define E_MSINVMT			0xE5	/* Invalid multi-tasking function */
#define E_MSINVXTDM			0xE6	/* Invalid extended connection member */
#define E_MSCNFLMT			0xE7	/* Exceeds conference limit */
#define E_MSZIPON			0xE8	/* Station is currently zipping */
#define E_MSNOTSALLOC		0xE9	/* No timeslots allocated to the board */
#define E_MSINVRNGCNT		0xEA	/* Invalid ring count */
#define E_MSNONRNGBRD		0xEC	/* Error ringing a non-ringing board */
#define E_MSBADRNGCAD		0xED	/* Bad Ring Cadence ID */
#define E_MSRNGCADCNFLCT	0xEE	/* Conflict between board and distinctive cadence lengths */
#define E_MSERRCHANSTATE	0xEF	/* Error setting a Channel state, also obtained for repeated PWRON or PWROF set chan state */
#define E_MSINVDATATYPE		0xF0	/* Invalid Data Type specified while sending data to the station */
#define E_MSINVVERSION		0xF1	/* Invalid Version number specified */
#define E_MSIOFFHOOK		0xF2	/* Invalid State of OFF HOOK */
#define E_MSIECNOTSUPPORTED	0xF3	/* Echo Cancellation Not Supported */

/* Distinctive Ring Cadence IDs */
#define MS_RNG_DEFAULT    0x00 

/* Distinctive Ring Cadences */
#define MS_RNG_REMOVE     0x00 
#define MS_RNGA_TWOSEC    0xFF 
#define MS_RNGA_ONESEC    0xF0 
#define MS_RNGA_SPLASH1   0xC0 
#define MS_RNGA_SPLASH2   0xD8 
#define MS_RNGA_SPLASH3   0xDB 
#define MS_RNGA_SPLASH4   0xAA 
#define MS_RNGA_LONGSHORT 0xFB 
#define MS_RNGA_SHORTLONG 0xDF 

/*
 * Ring Cadence Defines
 */
#define MS_MAX_CADID      0x8  /* Maximum ring cadence ID allowed */
#define MS_RNGA_CADLEN    0x8  /* Default ring cadence length for initial release of genringex() */
#define MS_CADENCE_OFFSET 0x1  /* Ring cadence offset, to separate boardwide cadences from genringex() cadences */
#define MS_BOARDCADENCE   0x1  /* Cadence ID for UDRNGCAD and PDRNGCAD only */

/*
 * Masks for 'party number' field of establish conference msg or
 * remove/add to conference msg.
 */
#define MSPN_STATION 0x00   /* Party is an MSI station */
#define MSPN_TS      0x10   /* Party is time slot */
#define MSPN_LOCPEB  0x40   /* Party is time slot on local PEB */
#define MSPN_RMTPEB  0x80   /* Party is time slot on remote PEB */

/*
 * Bit masks for party attributes
 */
#define MSPA_NULL   0x00   /* No attributes */ 
#define MSPA_RO     0x01   /* Receive-only (monitor) mode */
#define MSPA_TARIFF 0x02   /* Tariff Notification */
#define MSPA_DIG    0x04   /* Digital Front End */
#define MSPA_COACH  0x08   /* Coach */
#define MSPA_PUPIL  0x10   /* Pupil */
#define MSPA_NOAGC  0x20   /* Disable AGC */
/*The following defines are added for Conferencing over DM3 project */
#define MSPA_ECHOXCLEN		0x40   /* Echo cancel enable */
#define MSPA_BROADCASTEN	0x80   /* Broadcast enable */
/* Following defines are added for mode in Conf. over DM3  */
#define MSPA_MODENULL		0x100   /* Null Party */
#define MSPA_MODERECVONLY  	MSPA_RO    /* Receive only Party */
#define MSPA_MODEXMITONLY  	0x200   /* Xmit only Party */
#define MSPA_MODEFULLDUPLX	MSPA_NULL   /* Full Duplex - this is same as a Party with no specific attributes in Springware */
#define MSPA_PARTYTONECLAMP	0x400   /* Tone Clamping per Conferee */
#define MSPA_OUTPUTVOLABS	0x800   /* Absolute volume of a conferee */
#define MSPA_OUTPUTVOLRELCUR	0x1000   /* Relative volume of a conferee */

/*
 * Bit masks for conference attributes
 */
#define MSCA_NULL    0x00   /* Normal Attribute */
#define MSCA_ND      0x01   /* Notify-on-Add mode */
#define MSCA_NN      0x02   /* Do not notify if party joins in RO or monitor */
#define MSCA_NDOFF   0x04   /* Notify-tone off-Add mode */
/*
 * Control block equates
 */
#define MSCB_VOL    0x00   /* Volume control block ID */
#define MSCB_AGC    0x01   /* AGC control block ID */
#define MSCB_ND     0x02   /* Notify-on-Add control block ID */
#define MSCB_NM     0x03   /* Notify-on-Monitor control block ID */
#define MSCB_ZIP    0x04   /* Zip Tone control block ID */
#define NUM_CTLBLKS 0x05   /* Total number of control blocks */

/*
 * Ensure all structures are packed.
 */
#pragma pack(1)



typedef struct ms_cdt {
   int chan_num;         /* channel/time slot number */
   int chan_sel;         /* meaning of channel/time slot number */
   int chan_attr;        /* channel attribute description */
} MS_CDT;

#define chan_lts        chan_attr       /* Same area used for listen tslot info */
typedef struct ms_vcb {
   int confID;           /* conference ID */
   int volume;           /* party volume parameter */
} MS_VCB;

typedef struct ms_acb {
   unsigned char AGC1;
   unsigned char AGC2;
   unsigned char AGC3;
} MS_ACB;

typedef struct ms_ncb {
   unsigned char volume; /* volume */
   unsigned char tone;   /* tone frequency parameter */
   short duration;       /* tone duration parameter */
   short pulse;          /* pulse repetition interval parameter */
} MS_NCB;


typedef struct ms_xde {
   int chan_num;         /* channel/time slot number */
   int chan_sel;         /* meaning of channel/time slot number */
} MS_XDE;

/* Data Structures to transmit data to a station */
typedef enum 
{
	eMSInvalidDataType	= 0,
	eMSFSK				= 1,
	eMSMaxDataType		= 2
} eMSSendDataType;

typedef struct ms_DataInfo
{
	unsigned int		version;
	eMSSendDataType		dataType;    /* Data Type - FSK  */
	union
	{
		char* dataString;
	}uInfo;
} MS_DataInfo, *MS_DataInfoPtr;

#endif

/*
 * Type definition for Ring Cadence structure.
 */
#ifndef __MS_CADENCE__
#define __MS_CADENCE__
typedef struct ms_cadence {
   BYTE cadid;       // Cadence ID, 1-MS_MAX_CADID
   BYTE cadlength;   // Cadence Length
   BYTE *cadpattern; // Pointer to Cadence Pattern
} MS_CADENCE;
#endif

/*
 * Type definition for SCBus Timeslots information structure.
 */
#ifndef __SC_TSINFO__
#define __SC_TSINFO__
typedef struct sc_tsinfo {
   unsigned long  sc_numts;
   long          *sc_tsarrayp;
} SC_TSINFO;
#endif


/*
 * Restore default structures packing.
 */
#pragma pack()

/*
 *	Useful declaration specification.
 */
#ifdef __USING_DEF_FILE__

#ifdef DllExport
#undef DllExport
#endif

#ifdef DllImport
#undef DllImport
#endif

#ifdef DllLinkage
#undef DllLinkage
#endif

#define DllExport
#define DllImport
#define DllLinkage extern

#else	/* __USING_DEF_FILE__	*/

#ifndef DllExport
#define DllExport	__declspec( dllexport )
#endif	/*	Dllexport	*/

#ifndef DllImport
#define DllImport __declspec( dllimport )
#endif	/*	Dllimport	*/

#ifdef _MSI_DLL

#ifndef  DllLinkage
#define	DllLinkage	DllExport
#endif

#else

#ifndef DllLinkage
#define DllLinkage	DllImport
#endif

#endif   /* End _MSI_DLL */

#endif	/* __USING_DEF_FILE__	*/

#ifdef __CROSS_COMPAT_LIB__
#undef  DllLinkage
#define DllLinkage    
#endif

/*
/*
 * Prototypes for MSI/SC library functions.
 */
#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus) || defined(__BORLANDC__))
DllLinkage    int __cdecl ms_setbrdparm(int, unsigned char, void *);/* Set MSI Board parameters */
DllLinkage    int __cdecl ms_getbrdparm(int, unsigned char, void *);/* Get MSI Board parameters */
DllLinkage    int __cdecl ATMS_STATINFO(int, char *);               /* Get station info */
DllLinkage    char* __cdecl ATMS_FEMINFO(int);                      /* Get channel info */
DllLinkage    int __cdecl ms_genziptone( int);                      /* Generate ziptone */
DllLinkage    int __cdecl ms_estconf(int, MS_CDT *,int, int, int *);/* Estab a conference */
DllLinkage    int __cdecl ms_delconf(int, int);                     /* Delete conference */
DllLinkage    int __cdecl ms_addtoconf(int,int, MS_CDT *);          /* Add a party to conf*/
DllLinkage    int __cdecl ms_remfromconf(int,int, MS_CDT *);        /* Remove a party from conf*/
DllLinkage    int __cdecl ms_setcb(int,int, MS_CDT *, void *);      /* Set a control block */
DllLinkage    int __cdecl ms_getcb(int,int, MS_CDT *, void *);      /* Get a control block */
DllLinkage    int __cdecl ms_getcde(int, int, MS_CDT *);            /* Get conf. descr. elem */
DllLinkage    int __cdecl ms_setcde(int,int,MS_CDT *);              /* Set conf. descr. elem */
DllLinkage    int __cdecl ms_monconf(int,int, long *);              /* Monitor conference */
DllLinkage    int __cdecl ms_unmonconf(int, int);                   /* Unmonitor conference */
DllLinkage    int __cdecl ms_genring(int,unsigned short, unsigned short); /* Generate ringing */
DllLinkage    int __cdecl ms_setvol(int,int,int);                   /* Set volume */
DllLinkage    int __cdecl ms_getevtmsk(int, int , unsigned short *);/* Get event mask */
DllLinkage    int __cdecl ms_stopfn(int, int);                      /* Stop multitasking fn */
DllLinkage    int __cdecl ATMS_NUMCHAN(int);                        /* Get number of channels */
DllLinkage    int __cdecl ms_open(char *, int);                     /* Open MSI/SC device */
DllLinkage    int __cdecl ms_close(int);                            /* Close MSI/SC device */      
DllLinkage    int __cdecl ms_setevtmsk(int, int, unsigned short, int); /* Set event mask */
DllLinkage    int __cdecl ms_listen(int, SC_TSINFO *);              /* Listen to station */
DllLinkage    int __cdecl ms_listenEx(int, SC_TSINFO *, int);       /* Listen to station */
DllLinkage    int __cdecl ms_unlisten(int);                         /* Unlisten to station */
DllLinkage    int __cdecl ms_unlistenEx(int,int);                   /* Unlisten to station */
DllLinkage    int __cdecl ms_getxmitslot(int, SC_TSINFO *);         /* Get xmit ts */
DllLinkage    int __cdecl ms_tstcom(int, unsigned int);             /* Test communication */
DllLinkage    int __cdecl ms_tstdat(int, unsigned int);             /* Test data */
DllLinkage    int __cdecl ms_getctinfo(int, CT_DEVINFO *);          /* Get device info */
DllLinkage    long __cdecl ATMS_DNLDVER(int);                       /* Get download version */
DllLinkage    long __cdecl ATMS_TSSGBIT(int);                       /* Get hook state */
DllLinkage    int __cdecl ms_chgxtder(int,int,MS_CDT *);            /* Change connection extender */
DllLinkage    int __cdecl ms_delxtdcon(int,int);                    /* Delete Extended Connection */
DllLinkage    int __cdecl ms_estxtdcon(int,MS_CDT *,int *);         /* Establish extended connection*/
DllLinkage    int __cdecl ms_dsprescount(int, int *);               /* Get DSP resource count */
DllLinkage    int __cdecl ms_getcnflist(int, int, int *, MS_CDT *); /* Get config list */
DllLinkage    int __cdecl ms_getevt(int, EV_EBLK *, int);           /* Get MSI event */
DllLinkage    int __cdecl ms_setstparm(int, unsigned char, void *); /* Set MSI station parameters */
DllLinkage    int __cdecl ms_genringex(int,unsigned short, unsigned short, unsigned short); /* Generate distinctive ringing */                                                                                
DllLinkage    int __cdecl ms_libinit(HINSTANCE LibHandle);  /* Init funcs for clib lib */
DllLinkage    int __cdecl ms_genringCallerID(int,unsigned short, unsigned short, unsigned short,char*,void*); /* Generate distinctive ringing with CallerID*/       
DllLinkage    int __cdecl ms_SetMsgWaitInd(int,unsigned short,void*,void*); /* Set Message Waiting Indicator*/                                                                                
DllLinkage    int __cdecl ms_SendData(int,MS_DataInfo,int); /* Transmit Data to a station */ 
DllLinkage    int __cdecl ms_ResultValue(int,long,void*,long*); /* Retrieves the cause of an event */ 
DllLinkage    int __cdecl ms_ResultMsg(int,long,char**);        /* Retrieves an ASCII string describing a result code */ 
#else
DllLinkage    int __cdecl ms_setbrdparm();             /* Set MSI Board parameters */
DllLinkage    int __cdecl ms_getbrdparm();             /* Get MSI Board parameters */
DllLinkage    int __cdecl ATMS_STATINFO();             /* Get station info */
DllLinkage    char* __cdecl ATMS_FEMINFO();            /* Get channel info */
DllLinkage    int __cdecl ms_genziptone();             /* Generate ziptone */
DllLinkage    int __cdecl ms_estconf();                /* Establish a conference */
DllLinkage    int __cdecl ms_delconf();                /* Delete conference */
DllLinkage    int __cdecl ms_addtoconf();              /* Add a party to conf*/
DllLinkage    int __cdecl ms_remfromconf();            /* Remove a party from conf*/
DllLinkage    int __cdecl ms_setcb();                  /* Set a control block */
DllLinkage    int __cdecl ms_getcb();                  /* Set a control block */
DllLinkage    int __cdecl ms_getcde();                 /* Get conf. descr. element */
DllLinkage    int __cdecl ms_setcde();                 /* Set conf. descr. element */
DllLinkage    int __cdecl ms_monconf();                /* Monitor conference */
DllLinkage    int __cdecl ms_unmonconf();              /* Unmonitor conference */
DllLinkage    int __cdecl ms_genring();                /* Generate ringing */
DllLinkage    int __cdecl ms_setvol();                 /* Set volume */
DllLinkage    int __cdecl ms_getevtmsk();              /* Set event mask */
DllLinkage    int __cdecl ms_stopfn();                 /* Stop multitasking fn */
DllLinkage    int __cdecl ATMS_NUMCHAN();              /* Get number of channels */
DllLinkage    int __cdecl ms_open();                   /* Open MSI/SC device */
DllLinkage    int __cdecl ms_close();                  /* Close MSI/SC device */   
DllLinkage    int __cdecl ms_setevtmsk();              /* Set event mask */
DllLinkage    int __cdecl ms_listen();                 /* Listen to station */
DllLinkage    int __cdecl ms_listenEx();               /* Listen to station */
DllLinkage    int __cdecl ms_unlisten();               /* Unlisten to station */
DllLinkage    int __cdecl ms_unlistenEx();             /* Unlisten to station */
DllLinkage    int __cdecl ms_getxmitslot();            /* Get xmit ts */
DllLinkage    int __cdecl ms_tstcom();                 /* Test communication */
DllLinkage    int __cdecl ms_tstdat();                 /* Test data */
DllLinkage    int __cdecl ms_getctinfo();              /* Get device info */
DllLinkage    long __cdecl ATMS_DNLDVER();             /* Get download version */
DllLinkage    long __cdecl ATMS_TSSGBIT();             /* Get hookstate */
DllLinkage    int __cdecl ms_chgxtder();               /* Change connection extender */
DllLinkage    int __cdecl ms_delxtdcon();              /* Delete Extended connection */
DllLinkage    int __cdecl ms_estxtdcon();              /* Establish Extended connection*/
DllLinkage    int __cdecl ms_dsprescount();            /* Get DSP resource count */
DllLinkage    int __cdecl ms_getcnflist();             /* Get config list */
DllLinkage    int __cdecl ms_getevt();                 /* Get MSI event */
DllLinkage    int __cdecl ms_setstparm();	           /* Set MSI station parameters */
DllLinkage    int __cdecl ms_genringex();              /* Generate distinctive ringing */
DllLinkage    int __cdecl ms_libinit();                /* Init function for clib lib */
DllLinkage    int __cdecl ms_genringCallerID();    	   /* Generate distinctive ringing with CallerID*/
DllLinkage    int __cdecl ms_SetMsgWaitInd();		   /* Set Message Waiting Indicator*/ 
DllLinkage    int __cdecl ms_SendData();			   /* Transmit Data to a station */ 
DllLinkage    int __cdecl ms_ResultValue();	     	   /* Retrieves the cause of an event */ 
DllLinkage    int __cdecl ms_ResultMsg();              /* Retrieves an ASCII string describing a result code */ 
#endif

#ifdef __cplusplus
}
#undef extern

#endif
