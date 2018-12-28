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
 /***********************************************************************
 *	  FILE: devmgmt.h
 * DESCRIPTION: Device Management library API
 *	  DATE: July 19, 2002
 *
 **********************************************************************/

#ifndef __DEVMGMTLIB_H__
#define __DEVMGMTLIB_H__

#include <srllib.h>
#define DEV_MAXERRMSGSIZE 80	// Maximum size of error message string

typedef enum {
   BRD_SUCCESS = 0,
   EBRD_FAILEDOPENINGDTILIB,
   EBRD_DEVICEMAPPERFAILED,
   EBRD_INVALIDPHYSICALNAME,
   EBRD_INVALIDVIRTUALNAME,
   EBRD_BUFFERTOOSMALL,
   EBRD_OUTOFMEMORY,
   EBRD_NULLPOINTERARGUMENT,
   EBRD_COMMANDNOTSUPPORTED,
   EBRD_INVALIDDEVICEHANDLE,
   EBRD_INVALIDINTERVAL,
   EBRD_INVALIDTHRESHOLD,
   EBRD_SENDALIVENOTENABLED,
   EBRD_NOTIMP,
   EBRD_IPMCONNECT_FAILED,
   EBRD_DEVBUSY,
   EBRD_INVALIDCONNTYPE,
   EBRD_IPMDISCONNECT_FAILED,
   EBRD_CREATEEVENT_FAILED,
   EBRD_CREATEFILE_FAILED,
   EBRD_DEVICEIOCONTROL_FAILED,
   EBRD_WAITFORMULTOBJ_FAILED,
   EBRD_FWASSERTED,
   EBRD_MAX
} T_BRDERRORVALUE;

typedef enum {
   DEV_SUCCESS = 0,
   EDEV_INVALIDDEVICEHANDLE,
   EDEV_INVALIDMODE,
   EDEV_EVENTTIMEOUT,
   EDEV_DEVICEBUSY,
   EDEV_INVALIDCONNTYPE,
   EDEV_IPM_SUBSYSTEMERR,
   EDEV_FAX_SUBSYSTEMERR,
   EDEV_IPM_INTERNALERR,
   EDEV_MAX
} DEV_ERRORVALUE;

typedef struct errinfo {
	int dev_ErrValue;
	int dev_SubSystemErrValue;
	char dev_Msg[DEV_MAXERRMSGSIZE];
} DEV_ERRINFO;

typedef enum {
    DM_FULLDUP = 1,
    DM_HALFDUP
} eCONN_TYPE;

typedef enum {
    RESOURCE_TYPE_NONE,
    RESOURCE_IPM_LBR,
    RESOURCE_TYPE_MAX
} eDEV_RESOURCE_TYPE;

typedef struct getresourceinfo {
    int                 version;            // struct version
    eDEV_RESOURCE_TYPE  resourceType;       // Resource Type
    int                 curReserveCount;      // reservation count for dev
    int                 curReservePoolCount;  // current reserve pool count
    int                 maxReservePoolCount;  // max available pool count
} DEV_RESOURCE_RESERVATIONINFO;


#define BRD_FAILURE -1
#define DM3_TYPE    0x81
#define BRD_FW_ASSERT_ENABLE 0x4000

// dev_Connect events
#define DMEV_MASK                              0X9E00          // DML events start at 0x9E00
#define DMEV_CONNECT                           (DMEV_MASK | 0x01)
#define DMEV_CONNECT_FAIL                      (DMEV_MASK | 0x02)
#define DMEV_DISCONNECT                        (DMEV_MASK | 0x03)
#define DMEV_DISCONNECT_FAIL                   (DMEV_MASK | 0x04)
#define DMEV_RESERVE_RESOURCE                  (DMEV_MASK | 0x05)
#define DMEV_RESERVE_RESOURCE_FAIL             (DMEV_MASK | 0x06)
#define DMEV_RELEASE_RESOURCE                  (DMEV_MASK | 0x07)
#define DMEV_RELEASE_RESOURCE_FAIL             (DMEV_MASK | 0x08)
#define DMEV_GET_RESOURCE_RESERVATIONINFO      (DMEV_MASK | 0x09)
#define DMEV_GET_RESOURCE_RESERVATIONINFO_FAIL (DMEV_MASK | 0x0A)

// FW Assert Events
#define DMEV_FW_ASSERT                         (DMEV_MASK | 0x0B)
#define DMEV_FW_ASSERT_FAILURE                 (DMEV_MASK | 0x0C)

// DML complete callback prototype
typedef int(*fpDMLComplete)(int nDeviceHandle, int nReason, void *data, int size);

#ifdef _WIN32


#ifdef LIBDM3DEVMGMT_EXPORTS
#define DEVMGMTLIB_API __declspec( dllexport )
#else
#define DEVMGMTLIB_API __declspec( dllimport )
#endif
#define DEVMGMT_CONV	__cdecl

#else /* !_WIN32 */

#define DEVMGMTLIB_API extern
#define DEVMGMT_CONV

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {	// C Plus Plus function bindings
#endif

DEVMGMTLIB_API int DEVMGMT_CONV brd_Open(char *physical, long mode);
DEVMGMTLIB_API int DEVMGMT_CONV brd_VirtualToPhysicalName(char *virt, char *physical, int *len);
DEVMGMTLIB_API int DEVMGMT_CONV brd_SendAlive(int ddd, long mode);
DEVMGMTLIB_API int DEVMGMT_CONV brd_SendAliveEnable(int ddd, unsigned short interval, unsigned short threshold, long mode);
DEVMGMTLIB_API int DEVMGMT_CONV brd_SendAliveDisable(int ddd, long mode);
DEVMGMTLIB_API int DEVMGMT_CONV brd_GetAllPhysicalBoards(SRLDEVICEINFO *physicalDevs, int *count);
DEVMGMTLIB_API int DEVMGMT_CONV brd_Close(int ddd);
DEVMGMTLIB_API int DEVMGMT_CONV brd_ErrorValue( void );
DEVMGMTLIB_API char* DEVMGMT_CONV brd_ErrorMsg( void );
DEVMGMTLIB_API int DEVMGMT_CONV dev_ErrorInfo(DEV_ERRINFO *pErrInfo);
DEVMGMTLIB_API int DEVMGMT_CONV dev_Connect(int devHandle1, int devHandle2, eCONN_TYPE connType, unsigned short mode);
DEVMGMTLIB_API int DEVMGMT_CONV dev_Disconnect(int devHandle, unsigned short mode);
DEVMGMTLIB_API int DEVMGMT_CONV dev_ConnectCallBack(int a_devHandle, int a_Reason, void *data, int size);
DEVMGMTLIB_API int DEVMGMT_CONV dev_DisconnectCallBack(int a_devHandle, int a_Reason, void *data, int size);
DEVMGMTLIB_API int DEVMGMT_CONV dev_ReserveResource(int devHandle, eDEV_RESOURCE_TYPE resType, unsigned short mode);
DEVMGMTLIB_API int DEVMGMT_CONV dev_ReserveCallBack(int a_devHandle, int a_Reason, void *data, int size);
DEVMGMTLIB_API int DEVMGMT_CONV dev_ReleaseResource(int devHandle, eDEV_RESOURCE_TYPE resType, unsigned short mode);
DEVMGMTLIB_API int DEVMGMT_CONV dev_ReleaseCallBack(int a_devHandle, int a_Reason, void *data, int size);
DEVMGMTLIB_API int DEVMGMT_CONV dev_GetResourceReservationInfo(int devHandle, DEV_RESOURCE_RESERVATIONINFO *resInfo, unsigned short mode);
DEVMGMTLIB_API int DEVMGMT_CONV dev_GetResourceCallBack(int a_devHandle, int a_Reason, void *data, int size);

#ifdef __cplusplus
}		// C Plus Plus function bindings
#endif

#endif /* __DEVMGMTLIB_H__ */

