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

#ifdef DLG_WIN32_OS

#pragma warning( disable: 4251 )
#pragma warning( disable: 4275 )

#endif

#ifndef _DLG_ADMIN_MSG_H
#define _DLG_ADMIN_MSG_H


#include "dlgeventproxydef.h"

namespace DlgEventService
{
    /*
        The C++ "Consumer" clients must extend this class and override the HandleEvent method. 
        This is the callback object that is invoked by the consumer when an event is dispatched. 
        Clients creating consumers only use this class. Client creating suppliers do not care 
        for this class.

        Note: When a CEventHandlerAdaptor object is shared among different consumer objects. 
        The developer must make sure that the HandleEvent callback method is thread safe. 
        Remember that a consumer component runs on its own thread; therefore, having an 
        EventHandlerAdaptor shared by multiple threads can have unpredictable behavior if 
        not thread safe.

        Since:
            10/04/2000

        Version:
            1.0
        Author:  Dialogic Admin Software Group
    */
	class CEventHandlerAdaptor {

	public:


        /*
            virtual int HandleEvent( const DlgEventMsgTypePtr evMsg,
			                         ClientDataType clientData )
            C++ Consumer client must override this virtual method in order to receive events 
            from the Event Framework.

            Since:
                10/04/2000

            Parameter:
                [in]const DlgEventMsgTypePtr	evMsg : This is the actual event message 
                                                        that is sent by the supplier component. The actual client message is 
                                                        found under the payload field. The supplier and consumers must agree on the payload message format. The consumer uses the msgId to correctly typecast the message payload. 
                                                        The AdminCallbackMsg also contains the supplier name and node IP address.

            [in]const ClientDataType clientData : 	A void pointer value that is passed in during the filter registration by the consumer client. This value is
                                                    returned back to the client in the callback object. 

            Return :
                Return zero for no error
                non-zero - error		 Note: At this time the return value has no meaning.

            Version:
                1.0

            throws:

            See Also:

            Author:
                Dialogic Admin Software Group
        */
		virtual int HandleEvent( const DlgEventMsgTypePtr evMsg,
			         ClientDataType clientData ) = 0; 
		
	private:

	};

}

#endif

