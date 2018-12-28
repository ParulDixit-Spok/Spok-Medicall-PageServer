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

#ifndef __DLGCEVENTS_H__
#define __DLGCEVENTS_H__

#include "dlgctypes.h"
#include "dlgadminconsumer.h"
#include "adminconsumer.h"
#include "adminconsumerfw.h"
#include "dlgceventbasedef.h"
#include "dlgeventproxydef.h"

/* Device Administration events */
const unsigned long MAX_ADMIN_EVENT_DESCRIPTION = MAX_EVENT_DESCRIPTION;
const unsigned long DLGC_EVT_SYSTEM_STARTED                 = (DLGC_ADMIN_BASE_EVT_ID | 0x0001);
const unsigned long DLGC_EVT_SYSTEM_STOPPED                 = (DLGC_ADMIN_BASE_EVT_ID | 0x0002);
const unsigned long DLGC_EVT_SYSTEM_ABOUTTOSTART            = (DLGC_ADMIN_BASE_EVT_ID | 0x0003);
const unsigned long DLGC_EVT_SYSTEM_ABOUTTOSTOP             = (DLGC_ADMIN_BASE_EVT_ID | 0x0004);
const unsigned long DLGC_EVT_BLADE_DETECTED                 = (DLGC_ADMIN_BASE_EVT_ID | 0x0005);
const unsigned long DLGC_EVT_BLADE_REMOVED                  = (DLGC_ADMIN_BASE_EVT_ID | 0x0006);
const unsigned long DLGC_EVT_BLADE_STARTED                  = (DLGC_ADMIN_BASE_EVT_ID | 0x0007);
const unsigned long DLGC_EVT_BLADE_STOPPED                  = (DLGC_ADMIN_BASE_EVT_ID | 0x0008);
const unsigned long DLGC_EVT_BLADE_ABOUT_TOSTART            = (DLGC_ADMIN_BASE_EVT_ID | 0x0009);
const unsigned long DLGC_EVT_BLADE_ABOUT_TOSTOP             = (DLGC_ADMIN_BASE_EVT_ID | 0x000A);
const unsigned long DLGC_EVT_BLADE_READY_FOR_RECONFIG       = (DLGC_ADMIN_BASE_EVT_ID | 0x000B);
const unsigned long DLGC_EVT_BLADE_PHYS_INSERTED            = (DLGC_ADMIN_BASE_EVT_ID | 0x000C);
const unsigned long DLGC_EVT_BLADE_PHYS_REMOVED             = (DLGC_ADMIN_BASE_EVT_ID | 0x000D);
const unsigned long DLGC_EVT_BLADE_PHYS_INSERT_PROCESSED    = (DLGC_ADMIN_BASE_EVT_ID | 0x000E);
const unsigned long DLGC_EVT_BLADE_PHYS_REMOVE_PROCESSED    = (DLGC_ADMIN_BASE_EVT_ID | 0x000F);
const unsigned long DLGC_EVT_WAIT_FOR_CLEANUP_COMPLETION    = (DLGC_ADMIN_BASE_EVT_ID | 0x0010);
const unsigned long DLGC_EVT_CLEANUP_COMPLETED              = (DLGC_ADMIN_BASE_EVT_ID | 0x0011);
const unsigned long DLGC_EVT_BLADE_ABOUT_TO_REMOVE          = (DLGC_ADMIN_BASE_EVT_ID | 0x0012);
const unsigned long DLGC_EVT_BLADE_STOP_FAILED              = (DLGC_ADMIN_BASE_EVT_ID | 0x0013);
const unsigned long DLGC_EVT_BLADE_START_FAILED             = (DLGC_ADMIN_BASE_EVT_ID | 0x0014);
const unsigned long DLGC_EVT_BLADE_DETECTED_FAILED          = (DLGC_ADMIN_BASE_EVT_ID | 0x0015);

/* associated payload definition */
typedef struct DevAdminEventMsgT 
{
    AUID     auid;
    char     szDescription[MAX_EVENT_DESCRIPTION];
} DevAdminEventMsg, *DevAdminEventMsgPtr;

typedef struct DevAdminPhysicalBoardMsgT 
{
    long    geoSlotId;      // Geographical Slot ID
    long    geoShelfId;     // Geographical Shelf ID
    long    pciSlotId;      // PCI Bus Slot ID
    long    pciBusId;       // PCI Bus ID
    long    driverId;       // Driver ID
    char    szTechnology[MAX_EVENT_DESCRIPTION];
    char    szDescription[MAX_EVENT_DESCRIPTION];
} DevAdminPhysicalBoardMsg, *DevAdminPhysicalBoardMsgPtr;

/* associated payload definition */
typedef struct DevAdminStopHandshakeEventMsgT 
{
    AUID     auid;
    char     szDescription[MAX_EVENT_DESCRIPTION];
} DevAdminStopHandshakeEventMsg, *DevAdminStopHandshakeEventMsgPtr;


/* TDMBus Clocking Events */
const     unsigned long   MAX_CLOCKINGFAULT_DESCRIPTION = MAX_EVENT_DESCRIPTION;
const     unsigned long   DLGC_EVT_CT_A_LINESBAD                = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0001);
const     unsigned long   DLGC_EVT_CT_B_LINESBAD                = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0002);
const     unsigned long   DLGC_EVT_SCBUS_COMPAT_LINESBAD        = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0003);
const     unsigned long   DLGC_EVT_MVIP_COMPAT_LINESBAD         = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0004);
const     unsigned long   DLGC_EVT_NETREF1_LINEBAD              = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0005);
const     unsigned long   DLGC_EVT_NETREF2_LINEBAD              = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0006);
const     unsigned long   DLGC_EVT_LOSS_MASTER_SOURCE_INVALID   = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0007);
const     unsigned long   DLGC_EVT_TDM_FAULT_CLEAR              = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0008);
const     unsigned long   DLGC_EVT_CT_A_LINE_CLEAR              = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x0009);
const     unsigned long   DLGC_EVT_CT_B_LINE_CLEAR              = (DLGC_CLOCKAPI_BASE_EVT_ID | 0x00010);

/* associated Payload  */
typedef struct ClockingFaultMsgT
{
    AUID     auid;
    short    nPhysicalBusNumber;
    char     szDescription[MAX_EVENT_DESCRIPTION];
} ClockingFaultMsg, *ClockingFaultMsgPtr;


/* T1/E1 Network Alarm Events */
const     unsigned long   MAX_NETWORKALARM_DESCRIPTION = MAX_EVENT_DESCRIPTION;
const     unsigned long   DLGC_EVT_SYNC_MASTER_CLOCK             = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0001);
const     unsigned long   DLGC_EVT_EXTERNAL_ALARM_RED            = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0002); 
const     unsigned long   DLGC_EVT_EXTERNAL_ALARM_RED_CLEAR      = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0003);
const     unsigned long   DLGC_EVT_EXTERNAL_ALARM_YELLOW         = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0004);
const     unsigned long   DLGC_EVT_EXTERNAL_ALARM_YELLOW_CLEAR   = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0005);
const     unsigned long   DLGC_EVT_EXTERNAL_CARRIER_DETECT       = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0006);
const     unsigned long   DLGC_EVT_EXTERNAL_LOSS_OF_SIGNAL       = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0007);
const     unsigned long   DLGC_EVT_EXTERNAL_LOSS_OF_SIGNAL_CLEAR = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0008);
const     unsigned long   DLGC_EVT_UNKNOWN_ALARM                 = (DLGC_NETWORKALARMS_BASE_EVT_ID | 0x0009);

//associated payload
typedef struct NetworkEventMsgT 
{
    AUID     auid;
    int      externalRef;
    short    nPhysicalBusNumber;
    char     szDescription[MAX_EVENT_DESCRIPTION];
} NetworkEventMsg, *NetworkEventMsgPtr;


/* Ethernet Alarm Events */
const     unsigned long   MAX_ENETALARM_DESCRIPTION = MAX_EVENT_DESCRIPTION;
const     unsigned long   DLGC_EVT_LINK_UP       = (DLGC_ENETALARMS_BASE_EVT_ID | 0x0001);
const     unsigned long   DLGC_EVT_LINK_DOWN     = (DLGC_ENETALARMS_BASE_EVT_ID | 0x0002);
const     unsigned long   DLGC_EVT_PORT_FAILURE  = (DLGC_ENETALARMS_BASE_EVT_ID | 0x0003); 
const     unsigned long   DLGC_EVT_INACTIVE_PEER = (DLGC_ENETALARMS_BASE_EVT_ID | 0x0004);
const     unsigned long   DLGC_EVT_ENET_UNKNOWN  = (DLGC_ENETALARMS_BASE_EVT_ID | 0x0005);

//associated payload
typedef struct EnetMsgT 
{
    AUID   auid;
    char   szDescription[MAX_EVENT_DESCRIPTION];
    int    EthernetInterfaceNumber;
} EnetMsg, *pEnetMsg;

typedef struct InactivePeerMsgT 
{ 
    AUID auid;
    char szDescription[MAX_EVENT_DESCRIPTION];
    int  RefNum1[MAX_ENETALARM_DESCRIPTION];
    int  RefNum2[MAX_ENETALARM_DESCRIPTION];
} InactivePeerMsg, *pInactivePeerMsg;


/* Processor Faults */
const     unsigned long    MAX_PROCESSOR_DESCRIPTION = MAX_EVENT_DESCRIPTION; 
const     unsigned long    DLGC_EVT_CP_FAILURE  = (DLGC_FAULT_BASE_EVT_ID | 0x0001);
const     unsigned long    DLGC_EVT_SP_FAILURE  = (DLGC_FAULT_BASE_EVT_ID | 0x0002);

/* associated payload*/
typedef struct ProcessorFaultMsgT
{
    AUID     auid;
    short    nProcessorNumber;
    char     szDescription[MAX_EVENT_DESCRIPTION];
} ProcessorFaultMsg, *ProcessorFaultMsgPtr;


/* Diagnostic Events */
const     unsigned long   MAX_DIAGGENERIC_DESCRIPTION = MAX_EVENT_DESCRIPTION;
const     unsigned long   DLGC_EVT_GENERIC_ERROR        = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0001);
const     unsigned long   DLGC_EVT_PING_SUCCESS         = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0002);
const     unsigned long   DLGC_EVT_PING_FAILURE         = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0003);
const     unsigned long   DLGC_EVT_DIAGNOSTIC_SUCCESS   = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0004);
const     unsigned long   DLGC_EVT_DIAGNOSTIC_FAILURE   = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0005);
const     unsigned long   DLGC_EVT_NETWORK_QUALITY_LOSS = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0006);
const     unsigned long   DLGC_EVT_MEMORY_UTILIZATION   = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0007);
const     unsigned long   DLGC_EVT_UNKNOWN_ALARM_DIAG   = (DLGC_DIAGGENERIC_BASE_EVT_ID | 0x0008);

//associated payload
typedef struct DiagGenericMsgT 
{
    AUID auid;
    int  entityType;
    int  entityInstance;
    int  eventName;
    int  timeStamp;
    int  streamId;
    char szDescription[MAX_EVENT_DESCRIPTION];
} DiagGenericMsg, *DiagGenericMsgPtr;


/* Internal Events */
const     unsigned long	DM_GOOD_REF = (DLGC_INTERNAL_EVT_ID | 0x0001) ;
const     unsigned long DM_BAD_REF  = (DLGC_INTERNAL_EVT_ID | 0x0002) ;

//associated payload
typedef struct InternalNetworkEventMsgT 
{
    AUID auid;
    char szDescription[MAX_EVENT_DESCRIPTION];
} InternalNetworkEventMsg, *InternalNetworkEventMsgPtr;

#endif //__DLGCEVENTS_H__
