/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2005-2007 Dialogic Corporation. All Rights Reserved.
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
 *        FILE: srltpt.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 *
 ***********************************************************************/


#ifndef __SRLTPT_H__
#define __SRLTPT_H__


/*
 * Termination Parameter Types
 */
#define IO_CONT   0x01        /* Next TPT is contiguous in memory  */
#define IO_LINK   0x02        /* Next TPT found thru tp_nextp ptr */
#define IO_EOT    0x04        /* End of the Termination Parameters */


/*
 * DV_TPT - Termination Parameter Table Structure.
 */
typedef struct dv_tpt DV_TPT;

struct dv_tpt {
   unsigned short tp_type;    /* Flags Describing this Entry */
   unsigned short tp_termno;  /* Termination Parameter Number */
   unsigned short tp_length;  /* Length of Terminator */
   unsigned short tp_flags;   /* Termination Parameter Attributes Flag */
   unsigned short tp_data;    /* Optional Additional Data */
   unsigned short rfu;        /* Reserved */
   DV_TPT        *tp_nextp;   /* Ptr to next DV_TPT if IO_LINK set */
};

#endif

