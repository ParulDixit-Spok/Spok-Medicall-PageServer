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
 /****************************************************************************
 * 
 *                                 TITLE
 *
 * FILE: PRTMSD300LIB.H
 *
 * REVISION: 
 *
 * DATE: 
 *
 * PURPOSE: 
 *
 * INTERFACE: None
 *
 * Note: None
 *
 * REVISION HISTORY:
 *
 *
 ****************************************************************************/


#ifndef __PRTMSD300LIB_H__
#define __PRTMSD300LIB_H__

#include "prtmscmd.h"
#include "cclib.h"
#include "protimslib.h"
#include "prtmserr.h"


/* Code for PROTIMS */
#define CCEV_DLI_STARTLOOPBACK (DT_CC | 0x66) 
#define CCEV_DLI_STOPLOOPBACK (DT_CC | 0x67) 
#define CCEV_RING					(DT_CC | 0x68) /* a connection request has been made */
#define CCEV_CALL		(DT_CC | 0x69)
#define CCEV_DLI_CALLID	(DT_CC | 0x6A)
#define CCEV_DLI_CHNSTATUS	(DT_CC | 0x6B)
#define CCEV_DLI_PBXMSG	(DT_CC | 0x6C)
#define CCEV_CALLDISCONNECT (DT_CC | 0x6D)
#define PRTMSEV_RING       (DT_CC | 0x70) /* a connection request has been made */
#define PRTMSEV_CALL		(DT_CC | 0x71)
#define PRTMSEV_DLI_CALLID	(DT_CC | 0x72)
#define PRTMSEV_DLI_CHNSTATUS	(DT_CC | 0x73)
#define PRTMSEV_DLI_PBXMSG	(DT_CC | 0x74)
#define PRTMSEV_CALLDISCONNECT (DT_CC | 0x75)
#define CCEV_HOOKSTATE	(DT_CC | 0x76)
#define PRTMSEV_HOOKSTATE (DT_CC | 0x77)
#define CCEV_RINGING		(DT_CC | 0x78)
#define PRTMSEV_RINGING		(DT_CC | 0x79)
#define CCEV_DIALING		(DT_CC | 0x7A)
#define PRTMSEV_DIALING		(DT_CC | 0x7B)

// 03/27/07 Use 0x3E00 for base event value going forward.
#define  PRTEV_BASE		0x3E00			// beginning base value
#define  PRTEV_LAST_EVENT	(PRTEV_BASE | 0xFF)	// Last event value



/* Code for PROTIMS ends here */

#endif   /* for __PRTMSD300LIB_H__ */
