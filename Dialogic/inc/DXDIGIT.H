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
 *        FILE: dxdigit.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 **********************************************************************/

#ifndef __DXDIGIT_H__
#define __DXDIGIT_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif


/* 
 * dx_getdig( ) related defines
 */
#define LEN_DIGBUF   31          /* Max # of entries for collecting DTMF */
#define DG_MAXDIGS   LEN_DIGBUF  /* Max Digits Returned by dx_getdig() */
#define DG_END       -1        /* Terminator for dg_type Array in DV_DIGIT */


/*
 * Defines for Digit Control Mask.
 */
#define D_DTMF       0x01        /* Enable DTMF detection */
#define D_LPD        0x02        /* Enable Loop pulse detection */
#define D_APD        0x04        /* Enable Audio pulse detection */
#define D_MF         0x08        /* Enable MF digit detection */
#define D_DPD        0x10        /* Enable DPD detection */
#define D_DPDZ       0x20        /* Enable Zero-Trained DPD Detection */

/*
 * Ascii digit type returned in DV_DIGIT structure.
 */
#define DG_DTMF_ASCII   0x0030   /* DTMF Digit */
#define DG_LPD_ASCII    0x0031   /* Loop Pulse Digit  */
#define DG_APD_ASCII    0x0032   /* Audio Pulse Digit */
#define DG_DPD_ASCII    0x0032   /* Dial Pulse Digit */
#define DG_MF_ASCII     0x0033   /* MF Digit */


/* 
 * DV_DIGIT structure
 */
typedef struct dv_digit {
   char dg_value[DG_MAXDIGS +1];
   char dg_type[DG_MAXDIGS +1];
} DV_DIGIT;


#endif

