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

/*****************************************************************************
 * Filename:    srl_eblk.h 
 * DESCRIPTION: Header File for Dialogic Windows NT library.   
 *****************************************************************************/
 
#ifndef __SRL_EBLK_H__
#define __SRL_EBLK_H__


/*
 * EV_EBLK
 *
 * Event Block Structure - if 8 bytes or less of event-specific data, it
 * is stored in ev_data and ev_datap is NULL.  Otherwise ev_datap points to
 * malloc'd memory and ev_data is unused.  In either case, ev_len specifies
 * the number of bytes of event-specific data present.
 */
typedef struct ev_eblk {
   long           ev_dev;     /* Device on which event occurred    */
   unsigned long  ev_event;   /* Event that occured                */
   unsigned long  ev_len : 24,   /* # of bytes of event-specific data */   
   				  ev_flag : 7,	 /* RFU*/
   				  ev_forcefree :1; /* free without checkign length*/ 
   unsigned char  ev_data[8]; /* Event-specific data               */
   void          *ev_datap;   /* Event-specific data pointer       */
   void          *ev_UserContextp;    /* Ptr to user information Used for New Conferencing Library */
} EV_EBLK;


#endif

