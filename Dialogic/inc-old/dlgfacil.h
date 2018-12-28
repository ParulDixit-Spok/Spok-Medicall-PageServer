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

#ifndef __DLG_FACIL_SYS_ERROR_H__
#define __DLG_FACIL_SYS_ERROR_H__

/*
    Bits 00 - 15   - msg code
    Bits 16 - 27   - facility
    Bits 28 - 29   - 29 28    (Status Code bits)
                     0  1     error
                     1  0     warning
                     1  1     eventing (used only by the eventing mechanism)
                     1  0     info
    Bits 30          30    
                     0     external msg error    exposed to client
                     1     internal msg error    not exposed to client

    Bit 31 - reserved by Dialogic Corporation.
 
                                  
*/

typedef long DlgSysResultType;

const DlgSysResultType DLG_ERROR_CODE  = 0x00000000;
const DlgSysResultType DLG_WARN_CODE   = 0x10000000;
const DlgSysResultType DLG_EVENT_CODE  = 0x20000000;
const DlgSysResultType DLG_INFO_CODE   = 0x30000000;

#define DLG_CODE_MASK                 0xF0000000
#define DLG_FACIL_MASK                0x0FFF0000
#define DLG_MSGID_MASK                0x0000FFFF
#define DLG_INTERNAL_MASK             0x40000000

#define DLG_FACIL_SHIFT_VAL           16           



/* Locale Idenfiers */
const int   DLG_ENGLISH  = 0 ;
const int   DLG_SPANISH  = 1 ;
const int   DLG_FRENCH   = 2 ;
const int   DLG_GERMAN   = 3 ;

const int   MAX_SUPPORTED_LANG = 4;     //increment this value when
                                        //defining a new language above.


/* Dialogic Error Facility Definitions... */

#define DLG_SYS_GEN_FACILITY                        0x00000000      //0
#define DLG_SYS_MONITOR_FACILITY                    0x00010000      //1
#define DLG_FAULT_DETECTORS_FACILITY                0x00020000      //2
#define DLG_PNP_FACILITY                            0x00030000      //3
#define DLG_DETECTORS_FACILITY                      0x00040000      //4
#define DLG_DETECTOR_FACTORY_FACILITY               0x00050000      //5
#define DLG_DETECTOR_CONTROLLER_FACILITY            0x00060000      //6
#define DLG_SYSTEM_CONTROLLER_FACILITY              0x00070000      //7
#define DLG_DATA_MGR_FACILITY                       0x00080000      //8
#define DLG_CLOCK_DEAMON_FACILITY                   0x00090000      //9
#define DLG_EVENT_SERVER_FACILITY                   0x000A0000      //10
#define DLG_CLOCK_API_FACILITY                      0x000B0000      //11
#define DLG_INITIALIZER_SERVICE_FACILITY            0x000C0000      //12
#define DLG_INITIALIZER_FACTORY_FACILITY            0x000D0000      //13
#define DLG_INITIALIZER_CONTROLLER_FACILITY         0x000E0000      //14
#define DLG_TIMESLOT_MGR_FACILITY                   0x000F0000      //15
#define DLG_TIMESLOT_ASSIGN_FACILITY                0x00100000      //16
#define DLG_CLUSTER_FACILITY                        0x00110000      //17
#define DLG_DASI_FACILITY                           0x00120000      //18
#define DLG_LOGGER_FACILITY                         0x00130000      //19
#define DLG_CLOCKFW_FACILITY                        0x00140000      //20
#define DLG_PRODUCTAGENTMGR_FACILITY                0x00150000      //21
#define DLG_CLOCKSERVER_FACILITY                    0x00160000      //22
#define DLG_DATAUTILS_FACILITY                      0x00170000      //23
#define DLG_DATAENGINE_FACILITY                     0x00180000      //24
#define DLG_HAMANAGER_FACILITY                      0x00190000      //25
#define DLG_CDM_FACILITY                   	        0x001A0000      //26
#define DLG_DEVMAPADAPTOR_FACILITY             	    0x001B0000      //27
#define DLG_DM3DIAGNOSTICSAGENT_FACILITY            0x001C0000      //28
#define DLG_OSLEXT_FACILITY                         0x001D0000      //29
#define DLG_LAST_FACILITY                           29              //increment this value everytime a facility is added

#endif
