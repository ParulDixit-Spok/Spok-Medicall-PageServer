/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1996-2007 Dialogic Corporation. All Rights Reserved.
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
/****************************************************************************
 *                           All Rights Reserved
 ****************************************************************************
 *
 ****************************************************************************
 * 
 *                                 TITLE
 *
 * FILE:       prtmscmd.h
 * VERSION:    1.0
 *
 * FULLNAME:   %full_name: hsw#dnj23/incl/isdncmd.h/8 %
 *
 * PURPOSE: Header file for all ISDN network firmware shared ram interface.
 *
 * INTERFACE:                   
 *
 * Note:
 *
 * REVISION HISTORY:

 ****************************************************************************/


#ifndef __PRTMSCMD_H__
#define __PRTMSCMD_H__

#include "isdnos.h"
#include "isdncmd.h"


/* PROTIMS code added */
#define PRTMS_MESSAGE_BASE			0x9F
#define PRTMS_DRU_STARTLOOPBACK		PRTMS_MESSAGE_BASE 
#define PRTMS_DRU_STARTLOOPBACK_ACK	PRTMS_MESSAGE_BASE-0x01
#define PRTMS_DRU_STOPLOOPBACK		PRTMS_MESSAGE_BASE-0x02
#define PRTMS_DRU_STOPLOOPBACK_ACK	PRTMS_MESSAGE_BASE-0x03
#define PRTMS_DLI_STARTLOOPBACK		PRTMS_MESSAGE_BASE-0x04
#define PRTMS_DLI_STOPLOOPBACK		PRTMS_MESSAGE_BASE-0x05


/* Messages to Firmware */
#define PRTMS_SETL2ST			PRTMS_MESSAGE_BASE-0x06
#define PRTMS_GETL2ST			PRTMS_MESSAGE_BASE-0x07	
#define PRTMS_TRACE				PRTMS_MESSAGE_BASE-0x08	
#define PRTMS_SNDFRAME			PRTMS_MESSAGE_BASE-0x09	
#define PRTMS_REPCALLID			PRTMS_MESSAGE_BASE-0x0A	
#define PRTMS_REPDSP			PRTMS_MESSAGE_BASE-0x0B	
#define PRTMS_REPLINE			PRTMS_MESSAGE_BASE-0x0C	
#define PRTMS_UPDPARM_BOARD		PRTMS_MESSAGE_BASE-0x0D
#define PRTMS_UPDPARM_CHANNEL	PRTMS_MESSAGE_BASE-0x0E
#define PRTMS_READPARM_BOARD	PRTMS_MESSAGE_BASE-0x0F
#define PRTMS_READPARM_CHANNEL	PRTMS_MESSAGE_BASE-0x10
#define PRTMS_GETVER			PRTMS_MESSAGE_BASE-0x11
#define PRTMS_REPBDST			PRTMS_MESSAGE_BASE-0x12
#define PRTMS_REPFLAG			PRTMS_MESSAGE_BASE-0x13
#define PRTMS_GENERATELONGBEEP	PRTMS_MESSAGE_BASE-0x14
#define PRTMS_REPCHNSTATUS		PRTMS_MESSAGE_BASE-0x15
#define PRTMS_SEND_PBXMSG		PRTMS_MESSAGE_BASE-0x16

/* Message from Firmware */

#define PRTMS_RETL2ST				PRTMS_MESSAGE_BASE-0x17
#define PRTMS_TRCINFO				PRTMS_MESSAGE_BASE-0x18
#define PRTMS_CALLID				PRTMS_MESSAGE_BASE-0x19
#define PRTMS_DSP					PRTMS_MESSAGE_BASE-0x1A
#define PRTMS_LINE					PRTMS_MESSAGE_BASE-0x1B
#define PRTMS_UPARM_BOARD			PRTMS_MESSAGE_BASE-0x1C
#define PRTMS_UPARM_CHANNEL			PRTMS_MESSAGE_BASE-0x1D
#define PRTMS_PARM_CHANNEL			PRTMS_MESSAGE_BASE-0x1E
#define PRTMS_PARM_BOARD			PRTMS_MESSAGE_BASE-0x1F
//#define PRTMS_GETVER				PRTMS_MESSAGE_BASE-0x20
#define PRTMS_BDST					PRTMS_MESSAGE_BASE-0x21
#define PRTMS_FLAG					PRTMS_MESSAGE_BASE-0x22
#define PRTMS_RING					PRTMS_MESSAGE_BASE-0x23
#define PRTMS_CHNSTATUS				PRTMS_MESSAGE_BASE-0x24
#define PRTMS_PBXMSG_COMPLETE		PRTMS_MESSAGE_BASE-0x25
#define PRTMS_DLI_PBXMSG			PRTMS_MESSAGE_BASE-0x26


#define PRTMS_ERROR					PRTMS_MESSAGE_BASE-0x27
#define PRTMS_STATUS				PRTMS_MESSAGE_BASE-0x28
#define PRTMS_GETFRAME				PRTMS_MESSAGE_BASE-0x29
#define	PRTMS_TIMEOUT				PRTMS_MESSAGE_BASE-0x2A
#define	PRTMS_CALLINCOMMING			PRTMS_MESSAGE_BASE-0x2B
#define	PRTMS_WAITRING				PRTMS_MESSAGE_BASE-0x2C
#define	PRTMS_SETHOOK				PRTMS_MESSAGE_BASE-0x2D
#define PRTMS_MAKECALL				PRTMS_MESSAGE_BASE-0x2E
#define PRTMS_ASYNCCALLID			PRTMS_MESSAGE_BASE-0x2F
#define PRTMS_CALLOUTGOING			PRTMS_MESSAGE_BASE-0x30
#define PRTMS_ASYNCCHNSTATUS		PRTMS_MESSAGE_BASE-0x31

#define PRTMS_CALLDISCONNECT		PRTMS_MESSAGE_BASE-0x32
#define PRTMS_SETHOOKREP			PRTMS_MESSAGE_BASE-0x33
#define PRTMS_DIAL_COMPLETE			PRTMS_MESSAGE_BASE-0x34
/* Protims code ends here */


#endif	/* __PRTMSCMD_H__ */

