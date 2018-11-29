/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2000-2007 Dialogic Corporation. All Rights Reserved.
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

#ifndef __DLGC_BASE_EVENT_DEF_H__
#define __DLGC_BASE_EVENT_DEF_H__

#include    "dlgctypes.h"

#define MAX_EVENT_DESCRIPTION 256

/* 
  Thirdparty and Dialogic developers, please update this section when adding a new channel.
     
*/

#define FAULT_CHANNEL                       "FAULT"                     // Hardware Faults (CP/SP)
#define CLOCK_EVENT_CHANNEL                 "CLOCKING"                  // TDM Bus Faults
#define ADMIN_CHANNEL                       "ADMIN"                     // Generic Admin (Start/Stop)
#define DEFAULT_CHANNEL	                    "DEFAULT"                   // Not Really Used
#define STOP_DEVICE_REGISTRATION_CHANNEL    "STOP_DEVICE_REGISTRATION"
#define NETWORK_ALARM_CHANNEL               "NETWORKALARMS"             // All T1/E1 Network Alarms
#define ENET_ALARM_CHANNEL                  "ENETALARMS"                // Ethernet Alarms
#define DIAGGENERIC_CHANNEL                 "DIAG_GENERIC"              // Diagnostics

/*
    BASE EVENTS Class Section
    For all thirdparty customers and Dialogic developers, please update this section properly 
    when adding a new class of events. Typically but not always; a class of events 
    is represented by a one to one relation to a channel.
*/

#define  DLGC_INTERNAL_EVT_ID                       0x00010000 
#define  DLGC_CLOCKAPI_BASE_EVT_ID                  0x00020000
#define  DLGC_FAULT_BASE_EVT_ID                     0x00030000
#define  DLGC_ADMIN_BASE_EVT_ID                     0x00040000
#define  DLGC_STOP_BASE_EVT_ID                      0x00050000
#define  DLGC_NETWORKALARMS_BASE_EVT_ID             0x00060000
#define  DLGC_ENETALARMS_BASE_EVT_ID                0x00070000
#define	 DLGC_DIAGGENERIC_BASE_EVT_ID               0x00080000

#endif //__DLGC_BASE_EVENT_DEF_H__
