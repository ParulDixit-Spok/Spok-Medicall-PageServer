/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1993-2007 Dialogic Corporation. All Rights Reserved.
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
*   C Header:       pdk_public.h
*   Instance:       1
*   Description:    
*   %date_created:  Tue Apr 09 13:23:23 2002 %
*
**********************************************************************/

#ifndef _1_pdk_public_h_H
#define _1_pdk_public_h_H

/* Define the set ID for PDKRT protocol parameters */
#define PDKSET_PSI_VAR              0xA000
#define PDKSET_SERVICE_VAR          0xA001

/* Define the parm ID for PDKRT PSI parameters */
#define PDKPARM_EXT_SIG_INT1        0x0   /* Protocol Extension Integer 1 */
#define PDKPARM_EXT_SIG_INT2        0x1   /* Protocol Extension Integer 2 */
#define PDKPARM_EXT_SIG_INT3        0x2   /* Protocol Extension Integer 3 */
#define PDKPARM_EXT_SIG_STR1        0x3   /* Protocol Extension String 1  */
#define PDKPARM_EXT_SIG_STR2        0x4   /* Protocol Extension String 2  */

/*
   Define the extension function ID for PDKRT extension functions
   supported by gc_Extension( )
*/
#define PDK_EXT_ID_PROT_EXTENSION   0x0


                    /********************************/
                    /* PDK MakeCall block structure */
                    /********************************/

/*
-- Note: this is set up to be exactly the same as IC_MAKECALL_BLK for ICAPI
-- thus allowing applications currently using ICAPI to run using the PDK
-- without additional changes for gc_MakeCall()
*/

#ifndef NO_CALL_PROGRESS                        /* In case app also included icapi.h    */
#define     NO_CALL_PROGRESS    0x1             /* Normally we would not use a          */
                                                /* double negative, but do so herehere  */
                                                /* to maintain backward compatability   */
                                                /* with ICAPI                           */
#endif

#define MEDIA_TYPE_DETECT  0x2


typedef struct pdk_makecall_blk {
    unsigned long       flags;                  
    void                *v_rfu_ptr;             /* RFU */
    unsigned long       ul_rfu[4];              /* RFU */
} PDK_MAKECALL_BLK;


#endif
