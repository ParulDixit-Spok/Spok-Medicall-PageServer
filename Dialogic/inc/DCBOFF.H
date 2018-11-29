/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1990-2007 Dialogic Corporation. All Rights Reserved.
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
 
/******************************************************************************** 
 * FILENAME:    DCBOFF.H
 * DESCRIPTION:  
 ********************************************************************************/

#define DCB_DSP1                    1
#define DCB_DSP2                    2

#define DCB_DSP1_ATI_OFFSET     0x1480
#define DCB_DSP2_ATI_OFFSET     0x1484
#define DCB_DSP1_RESTBL_OFFSET  0x1500
#define DCB_DSP2_RESTBL_OFFSET  0x15c0

#define DCB_RESCOUNT            32 

#define DCB_ATI_SIZE            0x4
#define DCB_RESTBL_SIZE         DCB_RESCOUNT*6
#define DCB_APPRESTBL_SIZE      DCB_RESCOUNT*12

#define NAMELENGTH               64

typedef struct _DCBEXTRA {
   CHAR DBName[NAMELENGTH];
   CHAR ResTbl[DCB_RESTBL_SIZE];
} DCBEXTRA, *LPDCBEXTRA;
