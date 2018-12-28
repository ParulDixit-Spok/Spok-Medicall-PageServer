/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2007 Dialogic Corporation. All Rights Reserved.
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
 *	C Header:		ipmerror.h
 *	Description:	
 *	created_by:	Omitowoo %
 *	date_created:	
 *
 **********************************************************************/

#ifndef _IPMError_h_
#define _IPMError_h_

//ErrorCodes
enum IPM_ERROR
{
	EIPM_NOERROR = 0x00,	/* No error*/
	EIPM_FWERROR,           /* Firmware error*/
	EIPM_BADPARM,           /* Bad argument or parameter*/
	EIPM_BUSY,              /* Device busy*/
	EIPM_INV_DEVNAME,		/* Invalid device name*/
	EIPM_TIMEOUT,			/* timeout*/
	EIPM_INV_STATE,			/* Invalid state*/
	EIPM_NOMEMORY,			/* Memory allocation error*/
	EIPM_SYSTEM,			/* System error*/
	EIPM_SRL,				/* SRL error*/
	EIPM_SRL_SYNC_TIMEOUT,	/* SRL timeout*/
	EIPM_INTERNAL,			/* Internal error*/
	EIPM_INTERNAL_INIT,		/* Internal initialization error.*/
	EIPM_EVT_LIST_FULL,		/* Too many events*/
	EIPM_INV_MODE,			/* Invalid mode*/
	EIPM_INV_EVT,			/* Invalid event*/
	EIPM_EVT_EXIST,			/* Event already enabled*/
	EIPM_NO_H323_PROCESS,	/* No H323 process*/
	EIPM_CONFIG,			/* Configuration error*/
	EIPM_UNSUPPORTED,		/* Function unsupported*/
	EIPM_NOT_ACTIVE,		/* Channel already Idle*/
	EIPM_ACTIVE,			/* Listen already active*/
	EIPM_NOASSIGN,			/* Channel has no timeslot*/
	EIPM_ASSIGN,			/* Channel already has a timeslot*/
	EIPM_NOCONNECTION,		/* No CTBus connection between two device*/
	EIPM_NOTIMESLOT,		/* No TimeSlot*/
	EIPM_RESOURCESINUSE,

	//Added with NetStructure
	EIPM_INV_REMOTEDATA,
	EIPM_INTERNAL_GATEWAY,
	EIPM_SERVICE_UNAVAILABLE,
	EIPM_INSUFFICIENT_BANDWIDTH,
	EIPM_HARDWARE_FAILURE,
	EIPM_NETWORK_FAILURE,
	EIPM_DRIVER_ERROR,
	EIPM_UNKNOWN
};


#endif
