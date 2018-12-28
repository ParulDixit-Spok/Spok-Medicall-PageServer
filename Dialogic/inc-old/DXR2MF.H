/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1991-2007 Dialogic Corporation. All Rights Reserved.
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
 *        FILE: dxr2mf.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 **********************************************************************/

#ifndef __DXR2MF_H__
#define __DXR2MF_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif


/*
 * Group I Forward Signals
 */
#define SIGI_1          101   /* Digit 1 */
                              /* Language digit - French */
#define SIGI_2          102   /* Digit 2 */
                              /* Language digit - English */
#define SIGI_3          103   /* Digit 3 */
                              /* Language digit - German */
#define SIGI_4          104   /* Digit 4 */
                              /* Language digit - Russian */
#define SIGI_5          105   /* Digit 5 */
                              /* Language digit - Spanish */
#define SIGI_6          106   /* Digit 6 */
                              /* Spare (Language digit) */
#define SIGI_7          107   /* Digit 7 */
                              /* Spare (Language digit) */
#define SIGI_8          108   /* Digit 8 */
                              /* Spare (Language digit) */
#define SIGI_9          109   /* Digit 9 */
                              /* Spare (Discriminating digit) */
#define SIGI_10         110   /* Digit 0 */
                              /* Discriminating Digit */
#define SIGI_11         111   /* Access to Incoming Operator (code 11) */
                              /* Outgoing Half-Echo Suppressor Required */
#define SIGI_12         112   /* Access to Delay Operator (code 12) */
                              /* No Echo Suppressor Required */
#define SIGI_13         113   /* Access to Test Equipment (code 13) */
                              /*   Satellite Link not Included */
                              /* Test Call Indicator */
#define SIGI_14         114   /* Incoming Half-Echo Suppressor Required; */
                              /*   Satellite Link Included */
                              /* Outgoing Half-Echo Suppressor Inserted */
#define SIGI_15         115   /* End of Pulsing (code 15); */
                              /*   End of Identification */
                              /* Signal Not Used */


/*
 * Group II Forward Signals
 */
#define SIGII_1         101      /* National: Subsrciber without Priority */
#define SIGII_2         102      /* National: Subsrciber with Priority */
#define SIGII_3         103      /* National: Maintenance Equipment */
#define SIGII_4         104      /* National: Spare */
#define SIGII_5         105      /* National: Operator */
#define SIGII_6         106      /* National: Data Transmission */
#define SIGII_7         107      /* International: Subscriber, operator, or */
                                 /*  Maintenance equipment */
                                 /*  (without forward Transfer) */
#define SIGII_8         108      /* International: Data Transmission */
#define SIGII_9         109      /* International: Subscriber with Priority */
#define SIGII_10        110      /* International: Operator with Forward */
                                 /*  Transfer Facility */
#define SIGII_11        111      /* Spare for National Use */
#define SIGII_12        112      /* Spare for National Use */
#define SIGII_13        113      /* Spare for National Use */
#define SIGII_14        114      /* Spare for National Use */
#define SIGII_15        115      /* Spare for National Use */


/*
 * Group A Backward Signals
 */
#define SIGA_1          101   /* Send next Digit (n+1) */
#define SIGA_2          102   /* Send Last but One Digit (n-1) */
#define SIGA_3          103   /* Address Complete, change to Group B Signals*/
#define SIGA_4          104   /* Congestion in the National Network */
#define SIGA_5          105   /* Send Calling Party's Category */
#define SIGA_6          106   /* Address Complete; Charge; Set-Up Speech */
#define SIGA_7          107   /* Send Last but Two Digit (n-2) */
#define SIGA_8          108   /* Send Last but Three Digit (n-3) */
#define SIGA_9          109   /* Spare for National Use */
#define SIGA_10         110   /* Spare for National Use */
#define SIGA_11         111   /* Send Country Code Indicator */
#define SIGA_12         112   /* Send Language or Discriminating Digit */
#define SIGA_13         113   /* Send Nature of Circuit(Satellite Link Only)*/
#define SIGA_14         114   /* Is an Incoming Half-Suppressor Required? */
#define SIGA_15         115   /* Congestion in an International Exchange */


/*
 * Group B Backward Signals
 */
#define SIGB_1          101   /* Spare for National Use */
#define SIGB_2          102   /* Send Special Information Tone */
#define SIGB_3          103   /* Subscriber Line Busy */
#define SIGB_4          104   /* Congestion Encountered after change */
                              /*   to Group B */
#define SIGB_5          105   /* Unallocated Number */
#define SIGB_6          106   /* Subscriber line Free, Charge */
#define SIGB_7          107   /* Subscriber line Free, No Charge */
#define SIGB_8          108   /* Subscriber line out of Order */
#define SIGB_9          109   /* Spare for National Use */
#define SIGB_10         110   /* Spare for National Use */
#define SIGB_11         111   /* Spare for National Use */
#define SIGB_12         112   /* Spare for National Use */
#define SIGB_13         113   /* Spare for National Use */
#define SIGB_14         114   /* Spare for National Use */
#define SIGB_15         115   /* Spare for National Use */


/*
 * Socotel Confirmation Signals
 */
#define SIGACK_1700     116
#define SIGACK_1900     117


/*
 * Miscellaneous Defines
 */
#define R2_SIGNAL       1
#define R2_BASEID       101
#define R2_LASTID       115
#define R2_ALLFSIG      255
#define R2_DEFAMPL      -10
#define R2_DEFDUR       1600

#define SOC_SIGNAL      2
#define SOC_LASTID      117
#define SOC_DEFAMPL     -9

typedef enum {
	 R2_START = 1,	/*   Start Protocol Tone        */
	 R2_NEXT  = 2,	/*   Send Next digit tone       */
	 R2_STOP  = 3	/*   Stop sending digits tone   */
} R2_REPLYTONE_e;

#endif


