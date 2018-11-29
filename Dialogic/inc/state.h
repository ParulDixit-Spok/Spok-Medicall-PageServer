/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1991-2007 Dialogic Corporation. All Rights Reserved.
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
 *        FILE: state.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 *
 **********************************************************************/

#ifndef __STATE_H__
#define __STATE_H__

/*
 ******** Message passing error codes
 */
#define DT_IDLE               0x01    /* Station is IDLE */
#define DT_RING               0x02    /* Station is Ringing */
#define DT_WAITCALL           0x04    /* Station is waiting for a call */
#define DT_WAITEVT            0x08    /* Station is waiting on an event */
#define DT_WTEVTORCALL        0x10    /* Waiting for a call or an event */
#define DT_BLOCKED            0x20    /* Station is Blocked */
#endif
