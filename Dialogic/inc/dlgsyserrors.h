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

#ifndef __DLG_SYS_ERROR_H__
#define __DLG_SYS_ERROR_H__

// Remove since it's not in dlgeventproxydef.h
#if 0

#define DLG_SYS_ERROR_BASE          0
#define DLG_SYS_ERROR_OFFSET        500

/* 0 - 499 */
#define DLG_GEN_MSG_ERR_BASE        DLG_SYS_ERROR_BASE
#define Dlg_FAIL                    DLG_GEN_MSG_ERR_BASE
#define Dlg_OK                      DLG_GEN_MSG_ERR_BASE + 1
#define Dlg_INVALID_POINTER         DLG_GEN_MSG_ERR_BASE + 2
#define Dlg_MEM_ALLOC_ERR           DLG_GEN_MSG_ERR_BASE + 3



/* 500 - 999 */
#define DLG_SYS_MONITOR_ERR_BASE                DLG_GEN_MSG_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 1000 - 1499 */
#define DLG_FAULT_DETECTOR_ERR_BASE             DLG_SYS_MONITOR_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 1500 - 1999 */
#define DLG_PnP_ERR_BASE                        DLG_FAULT_DETECTOR_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 2000 - 2499 */
#define DLG_DETECTOR_ERR_BASE                   DLG_PnP_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 2500 - 2999 */
#define DLG_DETECTOR_FACTORY_ERR_BASE           DLG_DETECTOR_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 3000 - 3499 */
#define DLG_DETECTOR_CONTROL_ERR_BASE           DLG_DETECTOR_FACTORY_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 3500 - 3999 */
#define DLG_SYSTEM_CONTROL_ERR_BASE             DLG_DETECTOR_CONTROL_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 4000 - 4499 */
#define DLG_DATA_MGR_ERR_BASE                   DLG_SYSTEM_CONTROL_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 4500 4999 */
#define DLG_CLOCK_DEAMON_ERR_BASE               DLG_DATA_MGR_ERR_BASE + DLG_SYS_ERROR_OFFSET


/* 5000 - 5499 */
#define DLG_EVENT_SERVER_ERR_BASE               DLG_CLOCK_DEAMON_ERR_BASE + DLG_SYS_ERROR_OFFSET
#define DlgEvent_READY                          DLG_EVENT_SERVER_ERR_BASE      //5000
#define DlgEvent_ERROR_INIT                     DLG_EVENT_SERVER_ERR_BASE + 1  //5001
#define DlgEvent_NOT_READY                      DLG_EVENT_SERVER_ERR_BASE + 2  //5002
#define DlgEvent_CHANNEL_ERROR                  DLG_EVENT_SERVER_ERR_BASE + 3  //5003



/* 5500 - 5999 */
#define DLG_TDM_BUSAGENT_ERR_BASE               DLG_EVENT_SERVER_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 6000 - 6499 */
#define DLG_INITIALIZER_SERVICE_ERR_BASE        DLG_TDM_BUSAGENT_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 6500 - 6999 */
#define DLG_INITIALIZER_FACTORY_ERR_BASE        DLG_INITIALIZER_SERVICE_ERR_BASE + DLG_SYS_ERROR_OFFSET


/* 7000 - 7499 */
#define DLG_INITIALIZER_CONTROLLER_ERR_BASE     DLG_TDM_BUSAGENT_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 7500 - 7999 */
#define DLG_TIMESLOT_MGR_ERR_BASE               DLG_INITIALIZER_CONTROLLER_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 8000 - 8499 */
#define DLG_TIMESLOT_ASSIGN_ERR_BASE            DLG_TIMESLOT_MGR_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 8500 - 8999 */
#define DLG_CLUSTER_ERR_BASE                    DLG_TIMESLOT_ASSIGN_ERR_BASE + DLG_SYS_ERROR_OFFSET

/* 9000 - 9499 */
#define DLG_DASI_ERR_BASE                       DLG_CLUSTER_ERR_BASE + DLG_SYS_ERROR_OFFSET

#endif  // if 0

#endif

