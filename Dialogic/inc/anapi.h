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
*  C Header:      anapi.h
*  Description:   
*  %date_created: Tue Apr 09 13:21:49 2002 %
*
**********************************************************************/
#ifndef _ANAPI_
#define _ANAPI_

#include <gcerr.h>
#include <gclib.h>
/*
 * define for ANAPI device number
 */
#define DV_ANAPI              100

/*
 * defines for ANAPI event codes
 */
#define DT_ANAPI              0x200
#define ANEV_ACKCALL          (DT_ANAPI | 1)       /* anapi_ackcall termination event    */
#define ANEV_SETLINESTATE     (DT_ANAPI | 2 )      /* anapi_setlinestate termination event*/
#define ANEV_TEARDOWNCALL     (DT_ANAPI | 3 )      /* anapi_teardowncall termination event*/
#define ANEV_LINESTATE        (DT_ANAPI | 4 )      /* line state changed due to network*/
#define ANEV_ALARM            (DT_ANAPI | 5 )      /* alarm started                    */
#define ANEV_RECOVERED        (DT_ANAPI | 6 )      /* alarm recovered                  */
#define ANEV_INCALL           (DT_ANAPI | 7 )      /* incoming call received           */
#define ANEV_DISCONNECT       (DT_ANAPI | 8 )      /* disconnection received           */
#define ANEV_MAKECALL         (DT_ANAPI | 9 )      /* anapi_makecall termination event    */
#define ANEV_TRANSFER         (DT_ANAPI | 10)      /* anapi_transfer termination event    */
#define ANEV_ANSWER           (DT_ANAPI | 11)      /* anapi_answer termination event      */
#define ANEV_ONHOLD           (DT_ANAPI | 12)      /* line is on hold                  */
#define ANEV_UNHOLD           (DT_ANAPI | 13)      /* line is no longer on hold        */
#define ANEV_CALLINFO         (DT_ANAPI | 14)      /* request more digits       */
#define ANEV_DIAL             (DT_ANAPI | 15)      /* partial or complete dialling over*/
#define ANEV_CALLSTATUS       (DT_ANAPI | 16)      /* Call status */
#define ANEV_RESETLINEDEV     (DT_ANAPI | 17)      /* gc_ResetlineDev() */

/*
 * defines for ANAPI event explanation
 */
/*ic1*/
#define ANEC_NORMAL           0x0   /* DDI string complete              */
#define ANEC_INTERNAL         0x1   /* event caused by internal failure */
#define ANEC_SIGNALLING       0x2   /* event caused by signalling change*/
#define ANEC_TIMEOUT          0x3   /* event caused by time-out         */
#define ANEC_TONE             0x4   /* event caused by unexpected tone  */
#define ANEC_TEST             0x5   /* call was an automatic test call  */
#define ANEC_I12              0x6   /* call aborted because I12 received*/
#define ANEC_PARTIAL          0x7   /* DDI string uncomplete            */
#define ANEC_REDIAL           0x8   /* redial policy not fulfilled      */
#define ANEC_DIALTONE         0x9   /* dial tone not detected           */
#define ANEC_ALARM            0xA   /* Event caused by alarm            */
#define ANEC_NOVOICE          0xB   /* call needs voice, use anapi_attach()*/
#define ANEC_NOANSWER         0xC   /* Ring, no answer                  */
#define ANEC_BUSY             0xD   /* Busy                             */
#define ANEC_NOT_INSERVICE    0xE   /* Number not in service            */
#define ANEC_CEPT             0xF   /* enhanced call progress term      */
#define ANEC_NORB             0x10  /* no ringback detected             */
#define ANEC_STOPD            0x11  /* call progress stopped            */
#define ANEC_CPERROR          0x12  /* call progress SIT tone error     */
#define ANEC_DISCSIG          0x13  /* disconnect by loop current       */
#define ANEC_DISCTONE         0x14  /* disconnect by tone detected      */

/*
 * defines for ANAPI error codes
 */
/*ic2*/
/*-- [BEGIN GROUP] -- */
#define EAN_ERRORMASK      0x7FFF         /* Must be >= largest error number  */
                                          /* will be different on DOS         */
                                          /* Used internally to mask data ->  */
                                          /* ANAPI error code                 */
#define EAN_NOERR          EGC_NOERR      /* no error                         */
#define AN_SUCCESS         EAN_NOERR      /* synonym of EAN_NOERR             */
#define EAN_NOCALL         EGC_NOCALL     /* no call was made or transfered   */
#define EAN_ALARM          EGC_ALARM      /* function interrupt by alarm      */
#define EAN_ATTACHED       EGC_ATTACHED   /* voice resource already attached  */
#define EAN_DEVICE         EGC_DEVICE     /* Bad device handle                */
#define EAN_INVPROTOCOL    EGC_INVPROTOCOL /* Ivalid protocol name             */
#define EAN_PROTOCOL       EGC_PROTOCOL   /* Protocol error                   */
#define EAN_SYNC           EGC_SYNC       /* The mode flag must be EV_ASYNC   */
#define EAN_TIMEOUT        EGC_TIMEOUT    /* Multi-tasking function timed out */
#define EAN_UNSUPPORTED    EGC_UNSUPPORTED/* Function is not defined          */
#define EAN_USER           EGC_USER       /* Function interrupted by user     */
#define EAN_VOICE          EGC_VOICE      /* No voice resource attached       */
#define EAN_NDEVICE        EGC_NDEVICE    /* Too many devices opened          */
#define EAN_NPROTOCOL      EGC_NPROTOCOL  /* Too many protocols opened        */
#define EAN_BADFCN         EGC_BADFCN     /* Bad function code (TSR)          */
#define EAN_TSRNOTACTIVE   EGC_TSRNOTACTIVE /* anapi not active (TSR)           */
#define EAN_COMPATIBILITY  EGC_COMPATIBILITY /* incompatible protocol            */
#define EAN_ANAPIERR       0x12           /* ANAPI error */
/*ic3*/
#define EAN_EVTERR         EGC_EVTERR        /* Used by anrs_errortype in stderr.c as a key */
                                             
#define EAN_PUTEVT         EGC_PUTEVT        /* Error queuing event              */

/*ic4*/
#define EAN_MAXBOARDS      EGC_MAXDEVICES    /* Exceeded Maximum devices limit   */
#define EAN_OPENH          EGC_OPENH         /* Error opening voice channel      */
#define EAN_INVCID         0x17              /* Invalid CID */

/*ic5*/
#define EAN_INTERR         EGC_INTERR        /* Internal ANAPI error */
                                             /* Used in anrs_errortype in stderr.c as a key */
#define EAN_NOMEM          EGC_NOMEM         /* Out of memory                    */
#define EAN_PFILE          EGC_PFILE         /* Error opening parameter file     */
#define EAN_TIMER          EGC_TIMER         /* Error starting timer             */
#define EAN_WRITE          EGC_FILEWRITE     /* Error writing file               */
#define EAN_SYSTEM         EGC_SYSTEM        /* System error                     */

/*ic6*/
#define EAN_VOXERR         EGC_VOXERR        /* Voice error */
                                             /* Used as a key in anrs_errortype in STDERR.C */
#define EAN_VOXDTI         0x400             /* Ask BB - changed from EGC_VOXDTI */
                                             /* Must leave at 0x400 for backward compatability */
                                          
#define EAN_BLDTONE        0x1E              /* Build tone template failed       */
#define EAN_CHSTATE        0x1F              /* Error getting voice channel state*/
#define EAN_CLRTONE        0x20              /* Clear tone template failed       */
#define EAN_DISTONE        0x21              /* Disable tone failed              */
#define EAN_ENBTONE        0x22              /* Enable tone failed               */
#define EAN_GETEVT         0x23              /* get event failed                 */
#define EAN_PLAYTONE       0x24              /* play tone failed                 */
#define EAN_SETHOOK        0x25              /* set hook switch failed           */
#define EAN_SETMASK        0x26              /* set transition mask failed       */
#define EAN_STOPCH         0x27              /* stop voice channel failed        */
#define EAN_SETDMASK       0x28              /* set DTMF mask and method failed  */
#define EAN_GETDIGIT       0x29              /* get DTMF or pulse digit failed   */
#define EAN_CLRDIGIT       0x2A              /* clear DTMF buffer failed         */
#define EAN_DIAL           0x2B              /* dialling failed                  */
#define EAN_UNUSED         0x2C              /* unused error */
#define EAN_ENABLECALLID   0x2D              /* Enable Caller ID failed */
#define EAN_DISABLECALLID  0x2E              /* Disable Caller ID failed */
#define EAN_GTCALLID       0x2F              /* dx_gtcallid() failed */
#define EAN_GTEXTCALLID    0x30              /* dx_gtextcallid() failed */

/*ic7*/
#define EAN_DTIERR         EGC_DTIERR        /* Used as a key in anrs_errortype in STDERR.C */
#define EAN_DTIPARM        0x32              /* change DTI parameter failed      */
#define EAN_SETDTIEVT      0x33              /* set DTI signalling mask failed   */
#define EAN_SETIDLE        0x34              /* change DTI idle state failed     */
#define EAN_SETSIGBITS     0x35              /* set DTI signalling bits failed   */
#define EAN_SIGTYPE        0x36              /* change transmit type failed      */
#define EAN_DTITASK        0x37              /* start DTI task failed            */
#define EAN_SENDWINK       0x38              /* send wink failed                 */

/*ic8*/
#define EAN_INVSTATE       EGC_INVSTATE      /* Invalid state                    */
#define EAN_INVCRN         EGC_INVCRN        /* Invalid call reference number    */
#define EAN_INVLINEDEV     EGC_INVLINEDEV    /* Invalid line device passed       */
#define EAN_PARMERR        EGC_INVPARM    /* Invalid parameter specified      */
#define EAN_SRL            EGC_SRL           /* SRL failure                      */
#define EAN_USRATTRNOTSET  EGC_USRATTRNOTSET /* UsrAttr was not set for this ldev*/
/*ic9*/
#define EAN_OTHERERRORS    EGC_OTHERERRORS   /* Mark start of other errors */
#define EAN_XMITALRM       EGC_XMITALRM      /* Send alarm failed                */
#define EAN_SETALRM        EGC_SETALRM       /* Set alarm mode failed            */
#define EAN_INVDEVNAME     EGC_INVDEVNAME    /* Invalid device name */
#define EAN_DTOPEN         EGC_DTOPEN        /* dt open failed */
#define EAN_ILLSTATE       EGC_ILLSTATE      /* Function is not supported in the current state */
#define EAN_FUNC_NOT_DEFINED EGC_FUNC_NOT_DEFINED /* Low level function is not defined */
/*ic10*/
#define EAN_BUSY           EGC_BUSY          /* Line is busy */
#define EAN_NOANSWER       EGC_NOANSWER      /* Ring, no answer */

#define EAN_NOT_INSERVICE   EGC_NOT_INSERVICE  /* Number not in service */
#define EAN_NOVOICE        EGC_NOVOICE      /* Need voice resource */
#define EAN_CEPT           EGC_CEPT
#define EAN_NORB           EGC_NORB
#define EAN_STOPD          EGC_STOPD
#define EAN_CPERROR        EGC_CPERROR 
#define EAN_DIALTONE       EGC_DIALTONE
#define EAN_ROUTEFAIL      EGC_ROUTEFAIL     /* Routing failed */
#define EAN_DTUNLISTEN     EGC_DTUNLISTEN    /* dt_unlisten() failed */
#define EAN_DXUNLISTEN     EGC_DXUNLISTEN    /* dx_unlisten() failed */
#define EAN_AGUNLISTEN     EGC_AGUNLISTEN    /* ag_unlisten() failed */
#define EAN_DTGETXMITSLOT  EGC_DTGETXMITSLOT /* dt_getxmitslot() failed */
#define EAN_DXGETXMITSLOT  EGC_DXGETXMITSLOT /* dx_getxmitslot() failed */
#define EAN_AGGETXMITSLOT  EGC_AGGETXMITSLOT /* ag_getxmitslot() failed */
#define EAN_DTLISTEN       EGC_DTLISTEN      /* dt_listen failed */
#define EAN_DXLISTEN       EGC_DXLISTEN      /* dx_listen failed */
#define EAN_AGLISTEN       EGC_AGLISTEN      /* ag_listen failed */
#define EAN_STARTED        EGC_GC_STARTED    /* ANAPI has been started */

/*
 * Protocol cause values generated if the call processing failed
 */
#define ANRV_CAUSE         0x500
#define ANRV_NORMAL       0                              /* DDI string complete              */
#define ANRV_INTERNAL     (ANRV_CAUSE | ANEC_INTERNAL)   /* Event caused by internal failure */
#define ANRV_SIGNALLING   (ANRV_CAUSE | ANEC_SIGNALLING) /* Event caused by signalling change*/
#define ANRV_TIMEOUT      (ANRV_CAUSE | ANEC_TIMEOUT)    /* Event caused by time-out         */
#define ANRV_TONE         (ANRV_CAUSE | ANEC_TONE)       /* Event caused by unexpected tone  */
#define ANRV_TEST         (ANRV_CAUSE | ANEC_TEST)       /* Call was an automatic test call  */
#define ANRV_I12          (ANRV_CAUSE | ANEC_I12)        /* Call aborted because I12 received*/
#define ANRV_PARTIAL      (ANRV_CAUSE | ANEC_PARTIAL)    /* DDI string uncomplete            */
#define ANRV_REDIAL       (ANRV_CAUSE | ANEC_REDIAL)     /* Redial policy not fulfilled      */
#define ANRV_DIALTONE     (ANRV_CAUSE | ANEC_DIALTONE)   /* Dial tone not detected           */
#define ANRV_ALARM        (ANRV_CAUSE | ANEC_ALARM)      /* event caused by alarm            */
#define ANRV_NOVOICE      (ANRV_CAUSE | ANEC_NOVOICE)    /* call needs voice, use anapi_attach()*/
#define ANRV_NOANSWER     (ANRV_CAUSE | ANEC_NOANSWER)   /* Ring, no answer                  */
#define ANRV_BUSY         (ANRV_CAUSE | ANEC_BUSY)       /* Busy                             */
#define ANRV_NOT_INSERVICE (ANRV_CAUSE | ANEC_NOT_INSERVICE) /* Number not in service            */
#define ANRV_CEPT         (ANRV_CAUSE | ANEC_CEPT)       /* operator intercept */
#define ANRV_NORB         (ANRV_CAUSE | ANEC_NORB)       /* operator intercept */
#define ANRV_STOPD        (ANRV_CAUSE | ANEC_STOPD)      /* operator intercept */
#define ANRV_CPERROR      (ANRV_CAUSE | ANEC_CPERROR)    /* operator intercept */
#define ANRV_DISCSIG      (ANRV_CAUSE | ANEC_DISCSIG)  /* loop disconnect */   
#define ANRV_DISCTONE     (ANRV_CAUSE | ANEC_DISCTONE) /* tone disconnect */

/*
 * LSI board
 */
#define AN_LSIBOARD           0

/*
 * defines for response parameter for anapi_ackcall(), defines of different
 * groups can be ored together. Only ANRS_ACCEPT or ANRS_REJECT is
 * mandatory, other defines are optional
 */
#define ANRS_ACCEPT           0        /* accept call                      */
#define ANRS_REJECT           1        /* reject call                      */
#define ANRS_NOANSWER         2        /* accept call but do not answer it */

#define ANAC_CHARGE           0        /* charge call (default)   */
#define ANAC_NOCHARGE         0x100    /* do not charge call  */
#define ANAC_NORING           0x0200   /* do not send any ring        */

/*
 * number of ring before answer.
 * By default, take number of ring specified in CDP
 */
#define ANAC_1RING            0x1000   /* answer after 1 ring              */
#define ANAC_2RING            0x2000   /* answer after 2 rings             */
#define ANAC_3RING            0x3000   /* answer after 3 rings             */
#define ANAC_4RING            0x4000   /* answer after 4 rings             */
#define ANAC_5RING            0x5000   /* answer after 5 rings             */
#define ANAC_6RING            0x6000   /* answer after 6 rings             */
#define ANAC_7RING            0x7000   /* answer after 7 rings             */
#define ANAC_8RING            0x8000   /* answer after 8 rings             */
#define ANAC_9RING            0x9000   /* answer after 9 rings             */
#define ANAC_10RING           0xA000   /* answer after 10 rings            */
#define ANAC_11RING           0xB000   /* answer after 11 rings            */
#define ANAC_12RING           0xC000   /* answer after 12 rings            */
#define ANAC_13RING           0xD000   /* answer after 13 rings            */
#define ANAC_14RING           0xE000   /* answer after 14 rings            */
#define ANAC_NRING            0xF000   /* let the phone ring forever       */

/*
 * Reject reasons
 */
#define ANRJ_BUSY             0x0000   /* number is busy (default)         */
#define ANRJ_UNALLOC          0x0100   /* number is not allocated          */
#define ANRJ_CONGESTION       0x0200   /* congestion in network            */
#define ANRJ_SIT              0x0300   /* send special info tone           */

/*
 * option for anapi_makecall() and anapi_transfer()
 */
#define ANOP_CALLPROGRESS     0x0000   /* do call progress (default)       */
#define ANOP_NOCALLPROGRESS   0x0100   /* terminates after dialling        */
#define ANOP_POSTCALLPROGRESS 0x0200   /* try to identy called party       */

/*
 * type of call for anapi_makecall() and anapi_transfer()
 */
#define ANOP_NORMALCALL       0x0000   /* normal call (default)            */
#define ANOP_PRIORITYCALL     0x0001   /* priority call                    */
#define ANOP_OPERATOR         0x0002   /* operator call                    */
#define ANOP_MAINTENANCE      0x0003   /* automatic maintenance equipment  */
#define ANOP_DATATRANS        0x0004   /* data transmission call           */
#define ANOP_REDIRECTED       0x0005   /* redirected calls              */

/*
 * number of ring before no answer in anapi_makecall() and anapi_transfer()
 * By default, number of ring or delay defined in CDP file
 */
#define ANOP_1RING            0x1000   /* no answer after 1 ring           */
#define ANOP_2RING            0x2000   /* no answer after 2 rings          */
#define ANOP_3RING            0x3000   /* no answer after 3 rings          */
#define ANOP_4RING            0x4000   /* no answer after 4 rings          */
#define ANOP_5RING            0x5000   /* no answer after 5 rings          */
#define ANOP_6RING            0x6000   /* no answer after 6 rings          */
#define ANOP_7RING            0x7000   /* no answer after 7 rings          */
#define ANOP_8RING            0x8000   /* no answer after 8 rings          */
#define ANOP_9RING            0x9000   /* no answer after 9 rings          */
#define ANOP_10RING           0xA000   /* no answer after 10 rings         */
#define ANOP_11RING           0xB000   /* no answer after 11 rings         */
#define ANOP_12RING           0xC000   /* no answer after 12 rings         */
#define ANOP_13RING           0xD000   /* no answer after 13 rings         */
#define ANOP_14RING           0xE000   /* no answer after 14 rings         */
#define ANOP_NRING            0xF000   /* wait forever for answer          */

/*
 * type of transfer in anapi_transfer()
 */
#define ANTR_TRANSFER         0x00     /* normal transfer                  */
#define ANTR_UNHOLD           0x80     /* original party reconnect         */

/*
 * return value for attribute functions
 */
#define AT_FAILURE            -1       /* failure for integer function     */
#define AT_FAILUREP           NULL     /* failure for pointer function     */

/*
 * specify ANAPI board device in anapi_open()
 */
#define AN_UNUSED             0        /* board device                     */

/*
 * specify all devices in anapi_close()
 */
#define AN_ALLDEVICES         0        /* close all devices                */

/*
 * defines for return value of ATAN_CALLRESULT()
 */
#define ANST_NOCALL           0        /* no call was previously made      */
#define ANST_ANSWERED         1        /* distant party answered           */
#define ANST_BUSY             2        /* distant party busy               */
#define ANST_CONGESTION       3        /* congestion in network            */
#define ANST_DIALDONE         4        /* dialling done, no call progress  */
#define ANST_FAX              5        /* Fax machine answered             */
#define ANST_MODEM            6        /* modem answered                   */
#define ANST_NOANSWER         7        /* no answer                        */
#define ANST_OPERATOR         8        /* operator intercept               */
#define ANST_UNOBTAINABLE     9        /* number unobtainable *            */
#define ANST_NODIALLING       10       /* no dialling because of no voice  */
#define ANST_CPTERM           11       /* call not connected, check        */
                                       /* ATDX_CPTERM()                    */
#define ANST_DIALTONE         12       /* no dialtone                      */

/*
 * line state values returned by ATAN_LINESTATE()
 */
#define ANST_BLOCKING         1        /* terminal blocking                */
#define ANST_IDLE             2        /* terminal and network idle        */
#define ANST_ALARM            3        /* terminal in alarm                */
#define ANST_ACCEPTING        4        /* incoming call being rejected     */
#define ANST_NTBLOCKING       5        /* network blocking                 */
#define ANST_CONNECTED        6        /* call in conversation state       */
#define ANST_CONNECTING       7        /* incomming call being received    */
#define ANST_DISCONNECTING    8        /* call disconnecting               */
#define ANST_MAKINGCALL       9        /* outgoing call being made         */
#define ANST_REJECTING        10       /* incomming call being rejected    */
#define ANST_TRANSFERING      11       /* transfer in progress             */
#define ANST_PROGRESSING      12       /* outgoing call in progress        */
#define ANST_RESETLINEDEV     13       /* reset line device                */

/*
 * defines for return value of ATAN_EVTSTATUS()
 */
#define ANST_SUCCESS          0        /* event successfull                */
#define ANST_FAIL             -1       /* failure event                    */

/*
 * defines for anapi_callinfo()
 */
#define ANIF_DDI              1        /* get additional DDI digits        */

/*
 * defines for option field when getting more digits with anapi_callinfo()
 * Caution: do not change these constants without looking at the code
 * (specifically at_CallAck in r2lib.c)
 */
#define ANDG_COMPLETE         0x0000   /* no more digits after that        */
#define ANDG_PARTIAL          0x0100   /* maybe more digits after that     */

#define ANDG_1DIGIT           0x1000   /* get 1 more digit                 */
#define ANDG_2DIGIT           0x2000   /* get 2 more digits                */
#define ANDG_3DIGIT           0x3000   /* get 3 more digits                */
#define ANDG_4DIGIT           0x4000   /* get 4 more digits                */
#define ANDG_5DIGIT           0x5000   /* get 5 more digits                */
#define ANDG_6DIGIT           0x6000   /* get 6 more digits                */
#define ANDG_7DIGIT           0x7000   /* get 7 more digits                */
#define ANDG_8DIGIT           0x8000   /* get 8 more digits                */
#define ANDG_9DIGIT           0x9000   /* get 9 more digits                */
#define ANDG_10DIGIT          0xA000   /* get 10 more digits               */
#define ANDG_11DIGIT          0xB000   /* get 11 more digits               */
#define ANDG_12DIGIT          0xC000   /* get 12 more digits               */
#define ANDG_13DIGIT          0xD000   /* get 13 more digits               */
#define ANDG_14DIGIT          0xE000   /* get 14 more digits               */
#define ANDG_NDIGIT           0xF000   /* get infinite string of digits    */
#define ANDG_MAXDIGITS        14       /* Max # of digits to get           */
/*
 * defines for parm parameter of anapi_setparm() and anapi_getparm()
 */
#define ANPR_ALARM         GCPR_ALARM        /* enable or disable alarm handling */
#define ANPR_WAITIDLE      GCPR_WAITIDLE     /* change wait for idle time-out    */
#define ANPR_LOADTONES     GCPR_LOADTONES    /* enable or disable loading tone   */
#define ANPR_RINGBACKID    GCPR_RINGBACKID   /* GTD id for ring back tone        */
#define ANPR_OUTGUARD      GCPR_OUTGUARD     /* maximum time for call progress   */
#define ANPR_CALLINGPARTY  GCPR_CALLINGPARTY /* set or get terminal phone number */
#define ANPR_GETCATEGORY   GCPR_CATEGORY  /* request caller category          */

#define ANPR_MEDIADETECT   GCPR_MEDIADETECT  /* enable or disable GCEV_MEDIADETECTED event. */
#define ANPR_PROTVER       GCPR_PROTVER      /* reteieve protocol version        */

#define AN_ENABLE             1        /* enable feature                   */
#define AN_DISABLE            0        /* disable feature                  */

/*
 * equates for anapi_getcallinfo
 */
#define ANPR_CALLTYPE         1
#define ANPR_METERING         2

/*
 * equates for anapi_getcallinfo() with ANPR_CALLTYPE
 */
#define ANCT_NORMALCALL       0        /* normal DDI call                  */
#define ANCT_CENTRALCALL      1        /* centralized call (no DDI)        */

/*
 * alarm equates
 */
#define ANAL_LOS              0x0100   /* receive loss of signal           */
#define ANAL_DPM              0x0200   /* driver performance monitor       */
#define ANAL_MFSERR           0x0800   /* multi-frame sync error           */
#define ANAL_RSA1             0x1000   /* receive signalling all 1's       */
#define ANAL_RDMA             0x2000   /* receive distant multi-frame alarm*/
#define ANAL_RLOS             0x0001   /* receive loss of sync             */
#define ANAL_RUA1             0x0002   /* receive unframed all ones        */
#define ANAL_FSERR            0x0008   /* frame sync error                 */
#define ANAL_RRA              0x0020   /* receive remote alarm             */
#define ANAL_ECS              0x0040   /* error count saturation           */
#define ANAL_BPVS             0x0080   /* bipolar violation saturation     */
#define ANAL_CECS             0x4000   /* CRC4 error count saturation      */
#define ANAL_OOF              0x8000   /* out of frame error               */
#define ANAL_RCLX             0x0010   /* receive carrier loss             */
#define ANAL_B8ZSD            0x0004   /* bipolar eight zero subs detection*/
#define ANAL_RED              0x0400   /* red alarm condition              */


/*
 * structure for protocol parameter passing
 */
#define AN_PARM GC_PARM

/*
 * size of caller ID buffers
 */
#define  AN_MAXCALLIDNAME 81
#define  AN_MAXCALLIDTIME 20
 
/*
 * ANAPI device handle type
 */
/* zs: change to long otherwise DOS will not be able to convert linedevp
       to anapi_handle in large model */
typedef /*int*/long AN_DEVHANDLE;

/*
 * declaration of attribute functions
 */
#if ( defined (_MSC_VER) || defined (__STDC__) || defined (__cplusplus) )
char *ATAN_ERRMSGP( void );
long ATAN_LASTERR( void );
long ATAN_LASTDLGERR( void );
long ATAN_EVTSTATUS( void );
long ATAN_EVTEXPLAIN( void );
long ATAN_CALLRESULT( AN_DEVHANDLE andevh );
char *ATAN_GETANIS( AN_DEVHANDLE andevh );
long ATAN_GETCATEGORY( AN_DEVHANDLE andevh );
char *ATAN_GETDNIS( AN_DEVHANDLE andevh );
long ATAN_GETLANGUAGE( AN_DEVHANDLE andevh );
long ATAN_VOICE( AN_DEVHANDLE andevh );
long ATAN_NETWORK( AN_DEVHANDLE andevh );
long ATAN_ALARMSTATE( AN_DEVHANDLE andevh );
long ATAN_LINESTATE( AN_DEVHANDLE andevh );
long ATAN_GETBOARDH( AN_DEVHANDLE andevh );


/*
 * declaration of ANAPI functions
 */
AN_DEVHANDLE anapi_open( char *pname, long boardh, long timesloth );
long anapi_attach( AN_DEVHANDLE andevh, long voich );
long anapi_detach( AN_DEVHANDLE andevh, long voich );
long anapi_setparm( AN_DEVHANDLE andevh, long parm, AN_PARM parmvalue );
long anapi_getparm( AN_DEVHANDLE andevh, long parm, AN_PARM *parmp );
long anapi_getcallinfo( AN_DEVHANDLE andevh, long infonb, AN_PARM *infop );
long anapi_getconnecttype( AN_DEVHANDLE andevh, char *valuep );
long anapi_getcalltime( AN_DEVHANDLE andevh, unsigned char *valuep );
long anapi_getcallname( AN_DEVHANDLE andevh, unsigned char *valuep );
long anapi_setconnecttype( AN_DEVHANDLE andevh, char value );
long anapi_close( AN_DEVHANDLE andevh );
long anapi_ackcall( AN_DEVHANDLE andevh, long response, void *userinfo, long mode );
long anapi_callinfo( AN_DEVHANDLE andevh, long info, long mode );
long anapi_teardowncall( AN_DEVHANDLE andevh, long mode);
long anapi_setlinestate( AN_DEVHANDLE andevh, long state, long mode);
long anapi_makecall( AN_DEVHANDLE andevh, char *address, long option, void *userdata, long length, long mode);
long anapi_transfer( AN_DEVHANDLE andevh, char *address, long option, void *userdata, long length, long mode);
long anapi_answer( AN_DEVHANDLE andevh, long mode );
long anapi_waitcall(AN_DEVHANDLE andevh, int timeout);
long anapi_stopch(AN_DEVHANDLE andevh);
long anapi_start( void );
void anapi_stop( void );

#else    /* K & R function prototypes  */

char *ATAN_ERRMSGP( );
long ATAN_LASTERR( );
long ATAN_LASTDLGERR( );
long ATAN_EVTSTATUS( );
long ATAN_EVTEXPLAIN( );
long ATAN_CALLRESULT( );
char *ATAN_GETANIS( );
long ATAN_GETCATEGORY( );
char *ATAN_GETDNIS( );
long ATAN_GETLANGUAGE( );
long ATAN_VOICE( );
long ATAN_NETWORK( );
long ATAN_ALARMSTATE( );
long ATAN_LINESTATE( );
long ATAN_GETBOARDH( );
long anapi_waitcall();
long anapi_stopch();

/*
 * declaration of ANAPI functions
 */
AN_DEVHANDLE anapi_open( );
long anapi_attach( );
long anapi_detach( );
long anapi_setparm( );
long anapi_getparm( );
long anapi_getcallinfo( );
long anapi_close( );
long anapi_ackcall( );
long anapi_callinfo( );
long anapi_teardowncall( );
long anapi_setlinestate( );
long anapi_makecall( );
long anapi_transfer( );
long anapi_answer( );
long anapi_start( );
void anapi_stop( );

void at_init_error_list();

#endif

#ifdef _STATIC_BUILD_
/*
 * protocol list type
 */
extern struct anapi_protocol_def {
  char *pname;                         /* protocol name                    */
  char *pheader;
} anprotocol_list[];
#endif   /* _STATIC_BUILD_ */

/*
 * debug facility, should not be used unless protocol errors
 */
#define DBG_ACTION            1        /* logset contains action info      */
#define DBG_EVENT             2        /* logset contains event info       */
#define DBG_LOG               3        /* protocol-specific message        */

#define DBG_MATCH             1        /* event was used by protocol       */
#define DBG_NOMATCH           2        /* event was not used               */
#define DBG_ERROR             3        /* event produced an error          */
#define DBG_SENT              4        /* event sent to protocol           */
#define DBG_TERM              5        /* event sent to application        */
#define DBG_DELETED           6        /* event was deleted                */

/*@
 *@ compatibility defines
 */
#define ANAPI_COMPATIBILITY_MAJOR   0x00000100
#define ANAPI_COMPATIBILITY_MINOR   0x00000001
#define COMPATIBILITY_MAJOR_MASK    0x0000ff00
#define COMPATIBILITY_MINOR_MASK    0x000000ff

/*
 * structure containing debug information
 */
typedef struct anapi_dbg_logset {
   short type;
   long  time;                /* type of debug info         */
   short linestate;           /* device line state          */
   short state;               /* device state               */
   union {
      struct {                /* if event information       */
         short device;        /* device type                */
         short code;          /* event code                 */
         short data;          /* event data                 */
         short use;           /* event use (matched, ...)   */
      } event;
      struct {                /* if action information      */
         char  *name;         /* action name                */
         short parm1;         /* action parameter           */
         short parm2;         /* action parameter           */
      } action;
      char *message;          /* pointer to protocol message*/
   } set;
} ANAPI_DBG_LOGSET;

/*
 * structure passed to debug function
 */
typedef struct anapi_dbg_buffer {
   short n_logset;            /* number of logset in buffer */
   short index;               /* current logset index       */
   ANAPI_DBG_LOGSET *plogset;       /* pointer to array of logset */
} ANAPI_DBG_BUFFER;

/*
 * definitions of flags in anapi_makecall_blk
*/
#define     NO_CALL_PROGRESS  0x1   /* normally we would not use a */
                                    /* double negative, but here to */
                                    /* maintain backward compatability where */
                                    /* call progress is the default, we will */
                                    /* use the double negative */

#define     MEDIA_TYPE_DETECT 0x2   /* Enable/disable GCEV_MEDIADETECTED */
                                    /* event. This overrides gc_SetParm() */

/* Note: this is not required to be part of gc_MakeCall() call */
/* if this is not used, the defaults are:                      */
/*    flags: 0 (i.e. call progress is used)                    */
/*    everything else is rfu                                   */
 
typedef struct anapi_makecall_blk {
   unsigned long     flags;
   void              *v_rfu_ptr; /* rfu */
   unsigned long     ul_rfu[4];  /* rfu */
} AN_MAKECALL_BLK;

extern int anapi_mode;

/*
 * debugging function and variables
 */
#if ( defined (__STDC__) || defined (__cplusplus) )
char *anapi_dbg_printset( ANAPI_DBG_LOGSET *plogset );
ANAPI_DBG_BUFFER *anapidbg_getdebug( AN_DEVHANDLE icdeh );
void anapidbg_setdebug( void ( *pfunc )( AN_DEVHANDLE, ANAPI_DBG_BUFFER *, int ) );
#else
char *anapi_dbg_printset( );
ANAPI_DBG_BUFFER *anapidbg_getdebug( );
void anapidbg_setdebug( );
#endif

#endif
