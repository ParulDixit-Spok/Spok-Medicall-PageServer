/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1998-2007 Dialogic Corporation. All Rights Reserved.
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
/****************************************************************************
 *                           All Rights Reserved
 ****************************************************************************
 *
 ****************************************************************************
 * 
 *                                 TITLE
 *
 * FILE: PROTOMSLIB.H
 *
 * REVISION: 1.0
 *
 * DATE: January 01, 2000
 *
 * PURPOSE: Define Information Elements, Driver Command,and Structures
 *          for the PROTIMS library.
 *
 * INTERFACE: None
 *
 * Note: None
 *
 * REVISION HISTORY:
 *
 *     Date         Description
 *
 ****************************************************************************/


#ifndef __PROTIMSLIB_H__
#define __PROTIMSLIB_H__


#include "srllib.h"
#include "ccerr.h"
#include "isdnlib.h"

typedef struct 
{
	int total;
	int success;
	int error;
}PRTMS_LBACK,*PRTMS_LBACK_PTR;

#endif   /*   __PROTIMSLIB_H__    */
