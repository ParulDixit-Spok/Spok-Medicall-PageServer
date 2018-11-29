/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2005-2007 Dialogic Corporation. All Rights Reserved.
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
 /***********************************************************************
 *        FILE: srllib.h
 * DESCRIPTION: Header File for DIALOGIC Standard Runtime Windows
 *              NT/95 library
 ********************************************************************/

// 05-02-2002 [mem] ptr 25698, 21616 Added device mapper api functions which call into libdevmap.dll

#ifndef __SRLLIB_H__
#define __SRLLIB_H__


#pragma warning (disable:4786)
#include <windows.h>


/*
 * Included header files
 */
#include "srl_eblk.h"
#include "srltpt.h"
// #include "dlgctypes.h"
typedef long    AUID;
typedef long    SRL_DEVICE_HANDLE;

/* Define NULL if it hasn't been defined already */
#ifndef NULL
#define NULL        0L
#endif

//High Availability defines
typedef struct _tagSrlDeviceInfo
{
     char szDevName[12];    //name of the device
     int    iDevType;	        //device type - see ATDV_DEVICETYPE.
} SRLDEVICEINFO, *LPSRLDEVICEINFO;

/**
 ** Defines used for standard attributes 
 **/
/*
 * Undefined attribute or Failed to get attribute
 */
#define AT_FAILURE   -1
#define AT_FAILUREP  NULL

#define EDV_BADDESC  -1
#define EDV_NOERROR  0

/* Open Flags for devices */
#define OF_DEFAULT   0x00  /* Open device without automatic notification */
#define O_NOTIFY     0x01  /* Open device with automatic notification */


/*
 * General Defines
 */
#define EV_ANYEVT          0xFFFFFFFF  /* Matches Any Event Type */
#define EV_ANYDEV          -1          /* Matches Any Device */
#define SR_DFLT_DATASIZE   48          /* Max size for SRl's statically allocated
                                          event data area */

#ifndef __DEVMAPR4_H__      // Check to see if devmapr4.h has been included
/******************************************************************************
** These are Virtual R4 Devices that may exist in the device map.
** 'Virtual R4 Device' sometimes refer to:
**    Virtual Boards (e.g., TYPE_R4_VOX_BOARD   )
**    Voice Channels (e.g., TYPE_R4_VOX_CHANNEL )
**    DTI Timeslots  (e.g., TYPE_R4_DTI_TIMESLOT)
**    CPs            (e.g., TYPE_SW_DCP         )
** These are Enumerated int values of the attribute
** DM_ATTR_TYPE (defined in devmap.h)
**
** WARNING: The defines below are also defined in devmapr4.h. 
******************************************************************************/
#define TYPE_R4_VOX_BOARD          501  /* E.g., dxxxB1   */
#define TYPE_R4_VOX_CHANNEL        502  /* E.g., dxxxB1C1 */
#define TYPE_R4_DTI_BOARD          503  /* E.g., dtiB1    */
#define TYPE_R4_DTI_TIMESLOT       504  /* E.g., dxxxB1T1 */
#define TYPE_R4_MSI_BOARD          505  /* E.g., msiB1    */
#define TYPE_R4_MSI_STATION        506  /* E.g., msiB1C1  */
#define TYPE_R4_FAX_BOARD          507  /*                */
#define TYPE_R4_FAX_CHANNEL        508  /*                */
#define TYPE_R4_VR_BOARD           509  /* E.g., vrxB1    */
#define TYPE_R4_VR_CHANNEL         510  /* E.g., vrxB1C1  */
#define TYPE_R4_BRI_BOARD          511  /* E.g., briS1    */
#define TYPE_R4_BRI_TIMESLOT       512  /* E.g., briS1T1  */
#define TYPE_R4_SCX_BOARD          513  /* E.g., scxB1    */
#define TYPE_R4_DCB_BOARD          514  /* E.g., dcbB1    */
#define TYPE_SW_DCP                515  /* E.g., fruB1sd1 */
#define TYPE_R4_DPD_BOARD          516  /* E.g., dpdB1    */
#define TYPE_R4_DPD_CHANNEL        517  /* E.g., dpdB1C1  */
#define TYPE_R4_TTS_BOARD          518  /* E.g., ttsB1    */
#define TYPE_R4_TTS_CHANNEL        519  /* E.g., ttsB1C1  */
#define TYPE_R4_DMX_BOARD          520  /* E.g., dmxB1    */
#define TYPE_R4_CSP_BOARD          521  /* E.g., cspB1    */
#define TYPE_R4_CSP_CHANNEL        522  /* E.g., cspB1C1  */
#define TYPE_R4_BRI_DATA_BOARD     523  /* E.g., briD1    */
#define TYPE_R4_BRI_DATA_CHANNEL   524  /* E.g., briD1C1  */
#define TYPE_R4_IPT_BOARD          525  /*                */
#define TYPE_R4_IPT_TIMESLOT       526  /*                */
#define TYPE_R4_IPM_BOARD          527  /* E.g., ipmB1    */
#define TYPE_R4_IPM_CHANNEL        528  /* E.g., ipmB1C1  */
#define TYPE_R4_DCB_DSP            529  /* E.g., dcbB1D1  */
#define TYPE_R4_MOH_BOARD          530  /*                */
#define TYPE_R4_TPI_BOARD          531  /*                */
#define TYPE_R4_PHYSICAL_BOARD	   532  /*E.g., brdB1      */
#define TYPE_R4_MM_BOARD           534  /* E.g., mmB1    */
#define TYPE_R4_MM_CHANNEL         535  /* E.g., mmB1C1  */
#define TYPE_R4_CNF_BOARD          536  /* e.g., cnfB1  */
#define TYPE_R4_M3G_BOARD          537  /* (0x0219) */
#define TYPE_R4_M3G_CHANNEL        538  /* (0x021A) */
#endif


/* 
 * EVENT MANAGEMENT
 */
#define SR_INTERPOLLID  0x01  /* Parameter id for inter-poll delay */
#define SR_MODEID       0x02  /* Set SRL to run in SIGNAL/POLL mode */
#define SR_DATASZID     0x03  /* Parameter id for getting/setting SRL's
                                 preallocated event data memory size */
#define SR_QSIZEID      0X04  /* Maximum size of SRL's internal event queue */
#define SR_MODELTYPE    0x05  /* Set SRL model type (for NT only) */
#define SR_USERCONTEXT  0x06  /* Allows user to set per device context */
#define SR_WIN32INFO	   0x07  /* Set Win32 Notification mechanism */     
#define SR_STATISTICS   0x08  /* Set statistics monitoring */
#define SR_PARMHIPRIMODE 0x09 /* High Priority handler mode */

#define SR_POLLMODE     0     /* Run SRL in polling mode */
#define SR_SIGMODE      1     /* Run SRL in signalling/interrupt mode */
#define SRL_DEVICE      0     /* The SRL device */
#define SR_TMOUTEVT     0     /* Timeout event - occurs on the SRL DEVICE */

#define SR_STASYNC      0     /* Single threaded async model */
#define SR_MTASYNC      1     /* Multithreaded asynchronous model */
#define SR_MTSYNC       2     /* Multithreaded synchronous model */

#define SR_NOTIFY_ON    0     /* Turn on message notification */
#define SR_NOTIFY_OFF   1     /* Turn off message notification */

#define SR_HIPRIDEFAULT 0        
#define SR_HIPRISYNC    1


/* SRL errors */
#define ESR_NOERR       0     /* No SRL errors */
#define ESR_SCAN        1     /* SRL scanning function returned an error */
#define ESR_PARMID      2     /* Unknown parameter id */
#define ESR_TMOUT       3     /* Returned by ATDV_LASTERR(SRL_DEVICE) when an
                                 SRL function times out */
#define ESR_SYS         4     /* System error - consult errno */
#define ESR_DATASZ      5     /* Invalid size for default event data memory */
#define ESR_QSIZE       6     /* Illegal event queue size */
#define ESR_NOHDLR      7     /* No such handler */
#define ESR_MODE        8     /* Illegal mode for this operation */
#define ESR_NOTIMP      9     /* Function not implemented */

#define ESR_NULL_DATAP  10      /* Pointer argument is null */
#define ESR_BADDEV      11      /* Invalid or Missing device */
#define ESR_NOMEM       12      /* No or insufficient memory available */
#define ESR_BADPARM     13      /* Invalid parameter or parameter value */
#define ESR_NOCOM       14      /* SRL can not communicate with another sub system */
#define ESR_INSUFBUF    15      /* No or insufficient buffers available */
#define ESR_THREAD_DEVICE_GROUP_EXISTS 16	/* Thread Device Group already exists you for this thread cannot be created again */
#define ESR_THREAD_DEVICE_GROUP_NO_GROUP_DEFINED 17 /* No Thread Device Group created for this thread therefore cannot call sr_WaitThreadDeviceGroup */


#define SR_TMOUT        -1    /* Returned by event scanning functions
                                 (e.g. sr_waitevt()) when they time out */

/*
 * Defines for DLL modes (to be passed to the xx_libinit functions)
 */
#define DLGC_ST         0     /* Single threaded operation */
#define DLGC_MT         1     /* Multithreaded operation */

/*
 * SRL Win/32 Synchronization Structure (use sr_setparm to set)
 */
typedef struct srlwin32info_tag {
   DWORD		dwTotalSize;	/* Structure size */
   HANDLE		ObjectHandle;	/* Object Handle */
   DWORD		dwHandleType;   /* Handle type */
   DWORD		UserKey;        /* User supplied key */
   LPOVERLAPPED	lpOverlapped;   /* pointer to an overlapped */
} SRLWIN32INFO, *LPSRLWIN32INFO;


typedef enum tagSR_EVENTDATASCOPE_TAG
{
   SR_EVENTDATASCOPE_TAG_SRL,  /* SRL will free up the event data memory upon 
								  the next call to sr_waitevt() or sr_getevtdatap().*/
   SR_EVENTDATASCOPE_TAG_USER  /* Application must free up the event data  memory 
								  through a call to sr_destroy().*/
}SR_EVENTDATASCOPE;

/*
 * Flags for dwHandleType
 */
#define  SR_IOCOMPLETIONPORT	1	/* I/O Completion Port */
#define  SR_RESETEVENT			2	/* Reset Event */
#define  SR_WINDOWHANDLE        3   /* Window Handle */


#if defined(__cplusplus)
extern "C" {
#endif

/* SRL standard attribute functions */
extern char * __cdecl ATDV_ERRMSGP(long ddd);
extern long __cdecl   ATDV_IOPORT(long ddd);
extern long __cdecl   ATDV_IRQNUM(long ddd);
extern long __cdecl   ATDV_LASTERR(long ddd);
extern char * __cdecl ATDV_NAMEP(long ddd);
extern long __cdecl   ATDV_SUBDEVS(long ddd);

extern long SRLGetAllPhysicalBoards(int *piNum, AUID *pPhysicalBoards);
extern long SRLGetPhysicalBoardName(AUID physicalBoard, int *piLen, char *szName);
extern long SRLGetVirtualBoardsOnPhysicalBoard(AUID physicalBoard,int *piNum,SRLDEVICEINFO *pInfo);
extern long SRLGetSubDevicesOnVirtualBoard(char *szVirtualBoard,int *piNum,SRLDEVICEINFO *pInfo);
extern long SRLGetJackForR4Device(char *szR4Device, int *piJackNum);
extern long SRLGetDataFromDeviceName(const char *szVirtualDevice,AUID *pDeviceAuid,int *pTotalSubDevices,int *busfabrictype);
extern long SRLGetSoftBusXmitSlotFromAnalogDeviceName(const char *szVirtualDevice,int *pSoftBusTimeslotNumber);

/* Wait for next event */
extern long __cdecl sr_waitevt(long tmout);
extern long __cdecl sr_waitevtEx(long *handlep, int count, long tmout, long *evthp);
extern int sr_CreateThreadDeviceGroup(long *handlep, int count);
extern int sr_WaitThreadDeviceGroup(long tmout);
extern int sr_RemoveFromThreadDeviceGroup(LONG *devhandlep, int num_handle);
extern int sr_DeleteThreadDeviceGroup(void);
extern int sr_GetThreadDeviceGroup(LONG *devhandlep, int *ptrnumhandle);
extern int sr_AddtoThreadDeviceGroup(LONG *devhandlep, int num_handle);


/* Enable an event handler */
extern long __cdecl sr_enbhdlr(long ddd, unsigned long event_type,
                       long (*hdlr)(unsigned long parm));

/* Disable an event handler */
extern long __cdecl sr_dishdlr(long ddd, unsigned long event_type,
                       long (*hdlr)(unsigned long parm));

extern long __cdecl sr_getboardcnt(char *brdname, int *brdcnt);


#if (defined(__BORLANDC__) || defined(DLGC_CLIB))
extern long __cdecl sr_getevtdev(unsigned long evt_handle);
extern long __cdecl sr_getevttype(unsigned long evt_handle);
extern long __cdecl sr_getevtlen(unsigned long evt_handle);
extern void * __cdecl sr_getevtdatap(unsigned long evt_handle);
extern void * __cdecl sr_getUserContext(void);
#elif defined(__cplusplus)
extern long __cdecl sr_getevtdev(...);         /* Get the ddd of the current event */
extern long __cdecl sr_getevttype(...);        /* Get the type of the current event */
extern long __cdecl sr_getevtlen(...);         /* Get the data length for crnt evt */
extern void * __cdecl sr_getevtdatap(...);      /* Get the data pointer for crnt evt */
extern void * __cdecl sr_getUserContext(...);  /* Get the UserContext pointer */
#else
extern long __cdecl sr_getevtdev();
extern long __cdecl sr_getevttype();
extern long __cdecl sr_getevtlen();
extern void * __cdecl sr_getevtdatap();
extern void * __cdecl sr_getUserContext();
#endif



/*	Free event data blocks generated by sr_createevtdatapcopy() and sr_getevtdatapex()*/
extern void * __cdecl sr_createevtdatapcopy(long evt, const void * evtdatap, int datalen); 
/* Request a copy of event data from the SRL */
extern void * __cdecl sr_getevtdatapex(unsigned long evt_handle, SR_EVENTDATASCOPE evtDataScopeValue); 
/* Request a copy of event data from the SRL */
extern long __cdecl sr_destroy(void * evtdatap);

extern long __cdecl sr_hold(void);             /* Hold off events in SIGNAL mode */
extern long __cdecl sr_release(void);          /* Release events in SIGNAL mode */

/* Get the value of an SRL paramater */
extern long __cdecl sr_getparm(long ddd, long parmid, void* valuep);

/* Set the value of an SRL paramater */
extern long __cdecl sr_setparm(long ddd, long parmid, void* valuep);

extern void __cdecl sr_NotifyEvent(HWND handle, unsigned int message, unsigned int flag);

extern long __cdecl sr_libinit(unsigned short flag);
extern long __cdecl sr_GetVersion(char *libnamep, LPDWORD fileverp, LPDWORD prodverp);
extern long __cdecl sr_GetDllVersion(LPDWORD fileverp, LPDWORD prodverp);
extern void __cdecl sr_cleanup(void);
extern long __cdecl sr_putevt(long dev, unsigned long evt, long len, void  *datap, long err);
extern long __cdecl sr_putevtUserContext(LONG dev, ULONG evt, LONG len, VOID *datap, LONG err, VOID *UserContextp);
extern long __cdecl SRLGetAllAuid (AUID* lpAuid, int *lpLen);
extern long __cdecl SRLGetDevices (AUID auBrd, LPSRLDEVICEINFO lpSrlDev, int* lpLen);

extern LONG __cdecl _sr_stdclean(LONG ddd);
extern VOID *_sr_attr_error(LONG tag);


/* terminate the extern "C" for c plus plus */
#if defined(__cplusplus)
}
#endif


#endif

