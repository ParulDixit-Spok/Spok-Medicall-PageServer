/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1993-2007 Dialogic Corporation. All Rights Reserved.
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
/**********************************************************************
*
*   C Header:       pdkerror_list.h
*   Instance:       1
*   Description:    
*   %date_created:  Tue Apr 09 13:23:25 2002 %
*
**********************************************************************/

#ifndef _1_pdkerror_list_h_H
#define _1_pdkerror_list_h_H

//
// Caution: if update this, also need to update gc_pdk_error.cpp for GC
// there is probably also a DM3 file that also needs to be updated
//
enum {
    EPDK_NOERR = 0,             // Success - must be 0 for compatability with GC
    EPDK_NOCALL,                // No call made/transfered
    EPDK_ALARM,                 // Function interrupted by alarm
    EPDK_ATTACHED,              // Voice resource already attached
    EPDK_DEVICE,                // Bad device handle
    EPDK_INVPROTOCOL,           // Invalid protocol name
    EPDK_PROTOCOL,              // Protocol error
    EPDK_SYNC,                  // The mode flag must be EV_ASYNC
    EPDK_ASYNC,                 // The mode flag must be EV_SYNC
    EPDK_TIMEOUT,               // Multi-tasking function timed out
    EPDK_UNSUPPORTED,           // Function is not supported
    EPDK_USER,                  // Function interrupted by user
    EPDK_VOICE,                 // No voice resource attached
    EPDK_NDEVICE,               // Too many devices opened
    EPDK_NPROTOCOL,             // Too many protocols opened
    EPDK_COMPATIBILITY,         // Incompatible component
    EPDK_PUTEVT,                // Error queuing event
    EPDK_DXOPEN,                // Error opening voice channel
    EPDK_NOMEM,                 // Out of memory
    EPDK_PFILE,                 // Error opening parameter file
    EPDK_SYSTEM,                // System error
    EPDK_CHSTATE,               // Error getting voice channel state
    EPDK_CLRTONE,               // Clear tone template failed
    EPDK_DISTONE,               // Disable tone failed
    EPDK_ENBTONE,               // Enable tone failed
    EPDK_GETEVT,                // Get event failed
    EPDK_PLAYTONE,              // Play tone failed
    EPDK_SETDMASK,              // set DTMF mask and method failed
    EPDK_GETDIGIT,              // Get DTMF or pulse digit failed
    EPDK_CLRDIGIT,              // Clear DTMF buffer failed
    EPDK_DIAL,                  // "Dialing failed"
    EPDK_SETSIGBITS,            // Set DTI signaling bits failed
    EPDK_SIGTYPE,               // Change transmit type failed
    EPDK_DTITASK,               // Start DTI task failed
    EPDK_SENDWINK,              // Send wink failed
    EPDK_INVSTATE,              // Invalid state
    EPDK_INVCRN,                // Invalid CRN
    EPDK_R2MF,                  // Bad opcode in R2MF functions
    EPDK_DTIPARM,               // Change DTI parameter failed
    EPDK_SETDTIEVT,             // set DTI signaling mask failed
    EPDK_SETIDLE,               // Change DTI idle state failed
    EPDK_INVLINEDEV,            // Invalid line device
    EPDK_INVPARM,               // Invalid parameter specified
    EPDK_SRL,                   // SRL error
    EPDK_USRATTRNOTSET,         // UsrAttr was not set for this line device
    EPDK_XMITALRM,              // Send alarm failed
    EPDK_SETALRM,               // Set alarm mode failed
    EPDK_INVDEVNAME,            // Invalid device name
    EPDK_DTOPEN,                // dt_open failed
    EPDK_ILLSTATE,              // Function is not supported in the current state
    EPDK_BUSY,                  // Line is busy
    EPDK_NOANSWER,              // Ring, no answer
    EPDK_NOT_INSERVICE,         // Number not in service
    EPDK_NOVOICE,               // Call needs voice, use     attach(
    EPDK_NO_TIMERS_AVL,         // No more timers available
    EPDK_TIMER_FAILED,          // Timer start failed
    EPDK_NO_MORE_AVL,           // No more information available
    EPDK_BUFFEROVFL,            // Buffer overflow
    EPDK_NOFREECALL,            // No free call object
    EPDK_NOTFOUND,              // Object not found
    EPDK_INV_CDP_LINE,          // Invalid line in cdp or sit file
    EPDK_DYN_CAST,              // Dynamic cast
    EPDK_INTERNAL,              // Internal
    EPDK_INVALID_ARG,           // Invalid argument
    EPDK_INVALID_OPERATION,     // Invalid operation
    EPDK_INVALID_OBJECT,        // Invalid object exception
    EPDK_UNSUPPORTED_FEATURE,   // Unsupported feature
    EPDK_NOT_ASSIGNED,          // Error number not assigned
    EPDK_SYSTEMEXCEPTION,       // System exception
    EPDK_NODATA,                // No data for request
    EPDK_BLOCKED,               // The channel is blocked
    EPDK_NO_MORE_DNIS,          // There is no more DNIS digits
    EPDK_LOADDXPARM,                // Unable to load DX parameter file
    EPDK_FATALERROR_ACTIVE,     // Fatal error occurred
    EPDK_RESETABLE_FATALERROR,  // Resetable fatal error occurred
    EPDK_RECOVERABLE_FATALERROR,    // Recoverable fatal error occurred
    EPDK_NON_RECOVERABLE_FATALERROR,    // Non-Recoverable fatal error occurred
    EPDK_FATALERROR_OCCURRED,   // Fatal error occurred
    EPDK_FAIL_OPEN_CDP,         // Failure to open a CDP file
    EPDK_INVALID_CDP_PLATFORM,  // Invalid platform of a cdp parameter in cdp file
    EPDK_INVALID_CDP_TYPE,      // Invalid type of a cdp parameter in cdp file
    EPDK_INVALID_CDP_NAME,      // Invalid name of a cdp parameter in cdp file
    EPDK_INVALID_CDP_VALUE,     // Invalid value of of a cdp parameter in cdp file
    EPDK_INVALID_CDP_FORMAT,    // Invalid format (mostly number of elements) of a cdp parameter in cdp file
    EPDK_UNLOADED_CDP_NAME,     // Unloaded cdp parameter's name: it is defined in cdp file but not used in psi file
    EPDK_UNLOADED_CDP_VALUE,    // Unloaded cdp value: it can be set in psidatebase
    EPDK_UNINIT_CDP_NAME,       // Uninitialized cdp parameter's name: it is used in psi file but not defined in cdp file
    EPDK_DISCONNECTED,          // Remote site is disconnected
    EPDK_INVTARGETTYPE,         // invalid target object type
    EPDK_INVTARGETID,           // invalid target object ID 
    EPDK_INVPARMBLK,            // invalid GC_PARM_BLKP
    EPDK_INVDATABUFFSIZE,       // invalid parm data buffer size
    EPDK_INVSETID,              // invalid set ID
    EPDK_INVPARMID,             // invalid parm ID
    EPDK_PARM_UPDATEPERM_ERR,   // the parameter is not allowed to be updated
    EPDK_PARM_VALUESIZE_ERR,    // value buffer size error
    EPDK_PARM_VALUE_ERR,        // parm value error
    EPDK_INVPARM_TARGET,        // invalid parmater for target object
    EPDK_INVPARM_GCLIB,         // invalid parameter for GCLib
    EPDK_INVPARM_CCLIB,         // invalid parameter for CCLib  
    EPDK_INVPARM_PROTOCOL,      // invalid parameter for protocol
    EPDK_INVPARM_FIRMWARE,      // invalid parameter for firmware
    EPDK_PARM_DATATYPE_ERR,     // the parameter data type error
    EPDK_NEXT_PARM_ERR,         // next parm error
    EPDK_INVUPDATEFLAG,         // Invalid update flag,
    EPDK_INVQUERYID,            // invalid query ID
    EPDK_QUERYEDATA_ERR,        // query data error
    EPDK_GLARE,                 // glare condition with incoming call present
    EPDK_CALLPROGRESS,          // Call progress
    EPDK_NODIALTONE,            // No dial tone
    EPDK_NORINGBACK,            // No ringback
    EPDK_REJECTED,              // Call is rejected
    EPDK_SITTONEDETECTED,       // Sit tone is detected
    EPDK_UNASSIGNEDNUMBER,      // Unassigned number
    EPDK_CONGESTION,       	    // Network congestion
    EPDK_NORMALCLEARING,        // Normal clearing, e.g. other inbound side may have done a disconnect before connect	

};


enum PDK_RESULT_VALUE
{
	/* Cause Value*/
	
	EPDKRV_INFO_PRESENT_ALL = 1366,		/* The req info is available */
	EPDKRV_INFO_PRESENT_MORE,			/* The req info is available, and even more are present */
	EPDKRV_INFO_SOME_TIMEOUT,			/* The req info is not yet available. Only some info is present, not all  */
	EPDKRV_INFO_SOME_NOMORE,			/* The req info is not yet available. No more info is coming in */
	EPDKRV_INFO_NONE_TIMEOUT,			/* The req info is not available. No info came in  */
	EPDKRV_INFO_NONE_NOMORE,			/* The req info is not available. No info came in and none is expected */
	EPDKRV_INFO_SENT,					/* The information has been sent successfully */
	EPDKRV_DESTINATION_ADDRESS_REQ,		/* The destination information has been requested by the remote side */
	EPDKRV_ORIGINATION_ADDRESS_REQ,		/* The origination information has been requested by the remote side */
		
	/*
	* The following are the possible list of reasons for a channel state change
	*/
	
	EPDKRV_CHAN_STATE_CHANGE_NORMAL = 1900, /* Event caused by normal channel state change */
	EPDKRV_CHAN_STATE_CHANGE_WAITCALL,		/* Channel State Change caused by gc_Waitcall()  */	
	EPDKRV_CHAN_STATE_CHANGE_SETCHANSTATE,  /* Channel State Change by gc_SetChanState() */
	
	/*
	* The following are the possible list of reasons for a call state change
	*/

	EPDKRV_ALARM = 2001,				/* event caused by alarm            */
	EPDKRV_BLOCKED,						/* event caused by blocked event    */
	EPDKRV_BUSY,						/* event caused as line is busy     */
	EPDKRV_CALLPROGRESS,				/* call progress error				*/
	EPDKRV_CONGESTION,					/* congestion on line				*/
	EPDKRV_DISCONNECTED,				/* Signaling bit change (e.g. network disconnect) */
	EPDKRV_NOANSWER,					/* event caused by no answer        */
	EPDKRV_NODIALTONE,					/* event caused by no dialtone		*/
	EPDKRV_NORINGBACK,					/* event caused by no ringback		*/
	EPDKRV_NORMAL,						/* normal completion                */
	EPDKRV_PROTOCOL,					/* event caused by protocol error   */
	EPDKRV_REJECTED,					/* call rejected					*/
	EPDKRV_SIT,							/* operator intercept (Standard Information Tone)  */
	EPDKRV_TIMEOUT,						/* event caused by timeout          */
	EPDKRV_UNASSIGNEDNUMBER,			/* event caused by Number not in service (i.e. unassigned number)*/
	EPDKRV_UNBLOCKED,					/* event caused by unblocked event  */
	EPDKRV_USERREQUEST,					/* event caused by users request    */
	EPDKRV_GLARE,						/* Glare condition with incoming call present */
	
	/*
	* The following are the possible list of reasons for a GCEV_FATALERROR/GCEV_NODYNMEM
	*/
	
	EPDKRV_FATALERROR_ACTIVE = 2050,		/* Fatal error active */
	EPDKRV_FATALERROR_RESETABLE,			/* Resetable Fatal error occurred */
	EPDKRV_FATALERROR_RECOVERABLE,			/* Recoverable Fatal error occurred */
	EPDKRV_FATALERROR_NON_RECOVERABLE,		/* Non-Recoverable Fatal error occurred */
	EPDKRV_FATALERROR_OCCURRED,				/* Fatal error occurred */
	EPDKRV_FATALERROR_NODYNAMICMEMORY,		/* Dynamic memory allocation failed */

	/*
	* The following are the possible list of reasons for a GCEV_TASKFAIL
	*/
	
	EPDKRV_TASKFAIL_NOT_SUPPORTED=2101,			/* Request not supported*/
	EPDKRV_TASKFAIL_INVALID_STATE,				/* Invalid State */
	EPDKRV_TASKFAIL_WAITCALL_INPROGRESS =2104,	/* Protocol failed to setup for a waitcall */
};

#endif
