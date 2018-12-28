/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2001-2007 Dialogic Corporation. All Rights Reserved.
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
/**********************************************************************
*
*	C Header:		Libgcs7.h
*	Instance:		dte_3
*	Description:	GlobalCall SS7 header file for application use
*	%date_created:	Tue June 06 10:24:40 2006 %
*
**********************************************************************/

#ifndef _dte_3_Libgcs7_h_H
#define _dte_3_Libgcs7_h_H

#include "gclib.h"
#include "cc_s7.h"
#include <memory.h>

#define GCSS7_ISUP_CFG_CCTGRP_VERSION 0x1001

/* GC SS7 data structure for configuration of an ISUP circuit group, which matches the ISUP_CFG_CCTGRP 
 * configuration command in config.txt file. Application must initialize the structure before use.
 * Use INIT_GCSS7_ISUP_CFG_CCTGRP function to initialize the structure.*/
typedef struct {
    unsigned long   version;            /* Version of the data structure */
    unsigned long   dpc;                /* The Destination Point Code for all circuits in the circuit group */
    unsigned short  basic_cic;          /* The first Circuit Identification Code (CIC) in the Circuit Group - for non BICC variants only */
    unsigned short  basic_cid;          /* Logical Circuit Identifier (CID) corresponding the first CIC  */
    unsigned long   cic_mask;           /* A 32-bit mask with bits set to indicate which circuits are to be allocated */
    unsigned long   options;            /* A 32-bit value containing run-time options for the ISUP circuit group */
    unsigned char   user_inst;          /* The instance number of the user application */
    unsigned char   user_id;            /* The user application module ID */
    unsigned long   opc;                /* The Originating Point Code for all circuits in the circuit group */
    unsigned long   ssf;                /* The value to be used in the Sub_Service Field (SSF) of all ISUP messages */
    unsigned char   variant;            /* The protocol variant for this circuit group */
    unsigned long   options2;           /* A 32-bit value containing additional run-time options for the ISUP circuit group */
    unsigned long   basic_cic_32bit;    /* 32 bit Circuit Identification Code (CIC) in the Circuit Group - for BICC variant only */
} GCSS7_ISUP_CFG_CCTGRP, *GCSS7_ISUP_CFG_CCTGRPP;

/* Macro for initializing the GCSS7_ISUP_CFG_CCTGRP structure. */
static __inline void INIT_GCSS7_ISUP_CFG_CCTGRP (GCSS7_ISUP_CFG_CCTGRP *pIsupCfgCctgrp) 
{
   memset((GCSS7_ISUP_CFG_CCTGRP *) pIsupCfgCctgrp, 0, sizeof(GCSS7_ISUP_CFG_CCTGRP));
   pIsupCfgCctgrp ->version = GCSS7_ISUP_CFG_CCTGRP_VERSION;            
}


/* GC SS7 data structure of configuration of a GC trunk device on the circuit group, which matches the CGrp 
 * configuration command in gcss7.cfg file */
typedef struct {
    unsigned long   version;            /* Version of the data structure */
    char            trunk_name[20];     /* The physical device name where the circuits in the group are terminated (e.g., dtiB1 or dkB1) */
    unsigned char   base_ts;            /* The first timeslot of the trunk that corresponds the first circuit of the group */
    unsigned char   pref_siu;           /* The default SIU for the group  */
} GCSS7_TRUNK_CFG, *GCSS7_TRUNK_CFGP;



/********************************************************************************************/
/*                       SS7 CC  Set ID and Parameter ID Start                           */
/********************************************************************************************/
/* SS7CC defined set IDs must be from 0x4500 to 0x45FF (17664-17919) */
#define SS7CCSET_GENERIC                                GC_SS7LIB_SET_MIN


/********************************************************************************************/
/******************   Set ID and Parm ID for adding a circuit group *************************/
/********************************************************************************************/
#define SS7SET_ADD_CCTGRP                               GC_SS7LIB_SET_MIN + 1
/* The first parameter must be group ID (integer value) */ 
#define SS7PARM_GRP_ID                                  0x1
/* The second parameter is mandatory for adding a circuit group, which provides the  
 * basic data required by ISP_MSG_CNF_GRP message.  Its value type is GCSS7_ISUP_CFG_CCTGRP data 
 * structure */ 
#define SS7PARM_ISUP_CFG_GRP                            0x2
/* The third parameter is also mandatory for adding a circuit group, which provides the 
 * data required by GC SS7 to configure a trunk device.  Its value type is GCSS7_TRUNK_CFG data
 * structure */ 
#define SS7PARM_TRUNK_CFG                               0x3


/********************************************************************************************/
/***************   Set ID and Parm ID for deleting a circuit group     ************************/
/********************************************************************************************/
#define SS7SET_DEL_CCTGRP                               GC_SS7LIB_SET_MIN + 2
/* The first parameter must be group ID (integer value). */ 
/* Use SS7PARM_GRP_ID = 0x1                              */
/* No other data is required by ISP_MSG_END_GRP message */



/********************************************************************************************/
/*********************** SS7 CC Set ID and Parm ID End ********************************/
/********************************************************************************************/

#endif
