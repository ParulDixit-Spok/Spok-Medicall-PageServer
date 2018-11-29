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

#ifndef __ADMIN_CONSUMER_H__
#define __ADMIN_CONSUMER_H__

#if defined DLG_WIN32_OS
#pragma warning( disable: 4251 )
#pragma warning( disable: 4275 )
#endif

#include "adminconsumerfw.h"
#include "dlgadminmsg.h"
#include "dlgeventproxydef.h"

using DlgEventService::CEventHandlerAdaptor;

namespace DlgEventService
{
   class AdminConsumer
      {
        public:
         struct FilterCallbackAssoc {

            CEventHandlerAdaptor* callback;
            ClientDataType		clientData;
            DlgFilterType		filter;		//only one filter allow at this time
                                          //in this case msgid.
            DlgEvent_FilterControl		enable;		// true if filter enable 
         };

      };
   

} //end of namespace 


#endif
