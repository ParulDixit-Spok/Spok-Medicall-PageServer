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

#ifndef H_DLGEVENTPROXY_DEF_H_
#define H_DLGEVENTPROXY_DEF_H_

#include <wchar.h>

/* 0 - 499 */
#define Dlg_FAIL                    0
#define Dlg_OK                      1
#define Dlg_INVALID_POINTER         2
#define Dlg_MEM_ALLOC_ERR           3

/* 5000 - 5499 */
#define DlgEvent_READY              5000             
#define DlgEvent_ERROR_INIT         5001 
#define DlgEvent_NOT_READY          5002 
#define DlgEvent_CHANNEL_ERROR      5003 

typedef void*           DlgEvent_THANDLE;
typedef void*           ClientDataType;
typedef unsigned long	DlgFilterType;
typedef unsigned char	PayloadDataType;
typedef char*           IpAddressStringType;
typedef wchar_t*        SupplierNameType;
typedef long            DlgEvent_TRESULT;

typedef struct AdminCallbackMsg
{
    unsigned long       msgId;          // Message identifier	
    wchar_t*            supplierName;   // The name of the supplier of this msg
    char*               node;           // IP address of the supplier 
    int                 payLoadLen;     // Size of actual msg
    unsigned char*      pPayLoad;       // Serialized msg
    int                 conversion;     // Use to indicate conversion is needed
    char*               description;    // Event Description
    char*               date;           // Date Event Sent
    char*               time;           // Time Event Sent
	} AdminCallbackMsgType;		

typedef AdminCallbackMsgType	DlgEventMsgType;
typedef AdminCallbackMsgType*	DlgEventMsgTypePtr;
typedef DlgEvent_TRESULT (*ClientCallbackFunc)(AdminCallbackMsgType* pMsg, ClientDataType clientData) ;

typedef enum 
{
    DlgEvent_DISABLE=0,
    DlgEvent_ENABLE
} DlgEvent_FilterControl;

typedef struct ProxyFilterCallbackAssoc 
{
    ClientCallbackFunc     callback;
    void*                  clientData;
    unsigned long          filter;      //only one filter allow at this time
    DlgEvent_FilterControl enable;      // true if filter enable 
} ProxyFilterCallbackAssocType;


#define DEFAULT_HOST      NULL
#define DEFAULT_PORT      0

#define DLG_CONVERT         1
#define DLG_DONT_CONVERT    0

#endif

