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
 *        FILE: ccerr.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 *
 **********************************************************************/

#ifndef CC_ERROR
#define CC_ERROR  -1


/*
 * defines for ICAPI error code, not applicable to ISDN. Refer to isdnerr.h
 */
#define ECC_NOERR             0        /* no error                         */
#define ECC_NOCALL            1        /* no call was made or transfered   */
#define ECC_ALARM             2        /* function interrupt by alarm      */
#define ECC_ATTACHED          3        /* voice resource already attached  */
#define ECC_DEVICE            4        /* Bad device handle                */
#define ECC_INVPROTOCOL       5        /* Invalid protocol name            */
#define ECC_PARMERR           6        /* Invalid parameter specified      */
#define ECC_PROTOCOL          7        /* Protocol error                   */
#define ECC_SYNC              8        /* The mode flag must be EV_ASYNC   */
#define ECC_TIMEOUT           9        /* Multi-tasking function timed out */
#define ECC_UNSUPPORTED       10       /* Function is not defined          */
#define ECC_USER              11       /* Function interrupted by user     */
#define ECC_VOICE             12       /* No voice resource attached       */
#define ECC_NDEVICE           13       /* Too many devices opened          */
#define ECC_NPROTOCOL         14       /* Too many protocols opened        */

#define ECC_EVTERR            20
#define ECC_PUTEVT            20       /* Error queuing event              */

#define ECC_INTERR            25
#define ECC_NOMEM             25       /* Out of memory                    */
#define ECC_PFILE             26       /* Error opening parameter file     */
#define ECC_TIMER             27       /* Error starting timer             */
#define ECC_SYSTEM            28       /* System error                     */

#define ECC_VOXERR            30
#define ECC_BLDTONE           30       /* Build tone template failed       */
#define ECC_CHSTATE           31       /* Error getting voice channel state*/
#define ECC_CLRTONE           32       /* Clear tone template failed       */
#define ECC_DISTONE           33       /* Disable tone failed              */
#define ECC_ENBTONE           34       /* Enable tone failed               */
#define ECC_GETEVT            35       /* get event failed                 */
#define ECC_PLAYTONE          36       /* play tone failed                 */
#define ECC_SETHOOK           37       /* set hook switch failed           */
#define ECC_SETMASK           38       /* set transition mask failed       */
#define ECC_STOPCH            39       /* stop voice channel failed        */
#define ECC_SETDMASK          40       /* set DTMF mask and method failed  */
#define ECC_GETDIGIT          41       /* get DTMF or pulse digit failed   */
#define ECC_CLRDIGIT          42       /* clear DTMF buffer failed         */
#define ECC_DIAL              43       /* dialling failed                  */

#define ECC_DTIERR            50
#define ECC_DTIPARM           50       /* change DTI parameter failed      */
#define ECC_SETDTIEVT         51       /* set DTI signalling mask failed   */
#define ECC_SETIDLE           52       /* change DTI idle state failed     */
#define ECC_SETSIGBITS        53       /* set DTI signalling bits failed   */
#define ECC_SIGTYPE           54       /* change transmit type failed      */
#define ECC_DTITASK           55       /* start DTI task failed            */
#define ECC_SENDWINK          56       /* send wink failed                 */

#define ECC_LINERELATED       57
#define ECC_INVSTATE          58
#define ECC_INVCRN            59


#endif
