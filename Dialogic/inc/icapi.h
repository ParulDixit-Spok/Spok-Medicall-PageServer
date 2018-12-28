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
*	C Header:		icapi.h
*	Instance:		dnj20
*	Description:	
*	%date_created:	Tue Apr 09 13:24:10 2002 %
*
**********************************************************************/
#ifndef _ICAPI_
#define _ICAPI_

#include <gcerr.h>
#include <gclib.h>
/*
 * define for ICAPI device number
 */
#define DV_ICAPI              100

/*
 * defines for ICAPI event codes
 */
#define DT_ICAPI              0x4000
#define ICEV_ACKCALL          (DT_ICAPI | 1)       /* ic_ackcall termination event    */
#define ICEV_SETLINESTATE     (DT_ICAPI | 2 )      /* ic_setlinestate termination event*/
#define ICEV_TEARDOWNCALL     (DT_ICAPI | 3 )      /* ic_teardowncall termination event*/
#define ICEV_LINESTATE        (DT_ICAPI | 4 )      /* line state changed due to network*/
#define ICEV_ALARM            (DT_ICAPI | 5 )      /* alarm started                    */
#define ICEV_RECOVERED        (DT_ICAPI | 6 )      /* alarm recovered                  */
#define ICEV_INCALL           (DT_ICAPI | 7 )      /* incoming call received           */
#define ICEV_DISCONNECT       (DT_ICAPI | 8 )      /* disconnection received           */
#define ICEV_MAKECALL         (DT_ICAPI | 9 )      /* ic_makecall termination event    */
#define ICEV_TRANSFER         (DT_ICAPI | 10)      /* ic_transfer termination event    */
#define ICEV_ANSWER           (DT_ICAPI | 11)      /* ic_answer termination event      */
#define ICEV_ONHOLD           (DT_ICAPI | 12)      /* line is on hold                  */
#define ICEV_UNHOLD           (DT_ICAPI | 13)      /* line is no longer on hold        */
#define ICEV_CALLINFO         (DT_ICAPI | 14)      /* request more digits       */
#define ICEV_DIAL             (DT_ICAPI | 15)      /* partial or complete dialling over*/
#define ICEV_CALLSTATUS       (DT_ICAPI | 16)      /* Call status */

/*
 * defines for ICAPI event explanation
 */
/*ic1*/
#define ICEC_NORMAL           0x0   /* DDI string complete              */
#define ICEC_INTERNAL         0x1   /* event caused by internal failure */
#define ICEC_SIGNALLING       0x2   /* event caused by signalling change*/
#define ICEC_TIMEOUT          0x3   /* event caused by time-out         */
#define ICEC_TONE             0x4   /* event caused by unexpected tone  */
#define ICEC_TEST             0x5   /* call was an automatic test call  */
#define ICEC_I12              0x6   /* call aborted because I12 received*/
#define ICEC_PARTIAL          0x7   /* DDI string uncomplete            */
#define ICEC_REDIAL           0x8   /* redial policy not fulfilled      */
#define ICEC_DIALTONE         0x9   /* dial tone not detected           */
#define ICEC_ALARM            0xA   /* Event caused by alarm            */
#define ICEC_NOVOICE          0xB   /* call needs voice, use ic_attach()*/
#define ICEC_NOANSWER         0xC   /* Ring, no answer                  */
#define ICEC_BUSY             0xD   /* Busy                             */
#define ICEC_NOT_INSERVICE    0xE   /* Number not in service            */
#define ICEC_CEPT					0xF	/* SIT operator tone						*/
#define ICEC_UNALLOCATED		0x10	/* number is unobtainable				*/
#define ICEC_CONGESTION			0x11	/* congestion on the line				*/
#define ICEC_REJECT				0x12	/* call is rejected						*/

/*
 * defines for ICAPI error codes
 */
/*ic2*/
/*-- [BEGIN GROUP] -- */
#define EIC_ERRORMASK		0x7FFF			/* Must be >= largest error number  */
														/* will be different on DOS			*/
														/* Used internally to mask data ->  */
														/* ICAPI error code						*/
#define EIC_NOERR          EGC_NOERR      /* no error                         */
#define IC_SUCCESS         EIC_NOERR      /* synonym of EIC_NOERR             */
#define EIC_NOCALL         EGC_NOCALL     /* no call was made or transfered   */
#define EIC_ALARM          EGC_ALARM      /* function interrupt by alarm      */
#define EIC_ATTACHED       EGC_ATTACHED   /* voice resource already attached  */
#define EIC_DEVICE         EGC_DEVICE     /* Bad device handle                */
#define EIC_INVPROTOCOL    EGC_INVPROTOCOL /* Ivalid protocol name             */
#define EIC_PROTOCOL       EGC_PROTOCOL   /* Protocol error                   */
#define EIC_SYNC           EGC_SYNC       /* The mode flag must be EV_ASYNC   */
#define EIC_TIMEOUT        EGC_TIMEOUT    /* Multi-tasking function timed out */
#define EIC_UNSUPPORTED    EGC_UNSUPPORTED/* Function is not defined          */
#define EIC_USER           EGC_USER       /* Function interrupted by user     */
#define EIC_VOICE          EGC_VOICE      /* No voice resource attached       */
#define EIC_NDEVICE        EGC_NDEVICE    /* Too many devices opened          */
#define EIC_NPROTOCOL      EGC_NPROTOCOL  /* Too many protocols opened        */
#define EIC_BADFCN         EGC_BADFCN     /* Bad function code (TSR)          */
#define EIC_TSRNOTACTIVE   EGC_TSRNOTACTIVE /* Icapi not active (TSR)           */
#define EIC_COMPATIBILITY  EGC_COMPATIBILITY /* incompatible protocol            */
#define EIC_ICAPIERR       0x12           /* ICAPI error */
/*ic3*/
#define EIC_EVTERR         EGC_EVTERR        /* Used by rs_errortype in stderr.c as a key */
                                             
#define EIC_PUTEVT         EGC_PUTEVT        /* Error queuing event              */

/*ic4*/
#define EIC_MAXBOARDS      EGC_MAXDEVICES    /* Exceeded Maximum devices limit   */
#define EIC_OPENH          EGC_OPENH         /* Error opening voice channel      */
#define EIC_INVCID         0x17              /* Invalid CID */

/*ic5*/
#define EIC_INTERR         EGC_INTERR        /* Internal ICAPI error */
                                             /* Used in rs_errortype in stderr.c as a key */
#define EIC_NOMEM          EGC_NOMEM         /* Out of memory                    */
#define EIC_PFILE          EGC_PFILE         /* Error opening parameter file     */
#define EIC_TIMER          EGC_TIMER         /* Error starting timer             */
#define EIC_WRITE          EGC_FILEWRITE     /* Error writing file               */
#define EIC_SYSTEM         EGC_SYSTEM        /* System error                     */

/*ic6*/
#define EIC_VOXERR         EGC_VOXERR        /* Voice error */
                                             /* Used as a key in rs_errortype in STDERR.C */
#define EIC_VOXDTI         0x400             /* Ask BB - changed from EGC_VOXDTI */
                                             /* Must leave at 0x400 for backward compatability */
                                          
#define EIC_BLDTONE        0x1E              /* Build tone template failed       */
#define EIC_CHSTATE        0x1F              /* Error getting voice channel state*/
#define EIC_CLRTONE        0x20              /* Clear tone template failed       */
#define EIC_DISTONE        0x21              /* Disable tone failed              */
#define EIC_ENBTONE        0x22              /* Enable tone failed               */
#define EIC_GETEVT         0x23              /* get event failed                 */
#define EIC_PLAYTONE       0x24              /* play tone failed                 */
#define EIC_SETHOOK        0x25              /* set hook switch failed           */
#define EIC_SETMASK        0x26              /* set transition mask failed       */
#define EIC_STOPCH         0x27              /* stop voice channel failed        */
#define EIC_SETDMASK       0x28              /* set DTMF mask and method failed  */
#define EIC_GETDIGIT       0x29              /* get DTMF or pulse digit failed   */
#define EIC_CLRDIGIT       0x2A              /* clear DTMF buffer failed         */
#define EIC_DIAL           0x2B              /* dialling failed                  */
#define EIC_R2MF           0x2C              /* Bad opcode in R2MF functions */
/*ic7*/
#define EIC_DTIERR         EGC_DTIERR        /* Used as a key in rs_errortype in STDERR.C */
#define EIC_DTIPARM        0x32              /* change DTI parameter failed      */
#define EIC_SETDTIEVT      0x33              /* set DTI signalling mask failed   */
#define EIC_SETIDLE        0x34              /* change DTI idle state failed     */
#define EIC_SETSIGBITS     0x35              /* set DTI signalling bits failed   */
#define EIC_SIGTYPE        0x36              /* change transmit type failed      */
#define EIC_DTITASK        0x37              /* start DTI task failed            */
#define EIC_SENDWINK       0x38              /* send wink failed                 */
/*ic8*/
#define EIC_INVSTATE       EGC_INVSTATE      /* Invalid state                    */
#define EIC_INVCRN         EGC_INVCRN        /* Invalid call reference number    */
#define EIC_INVLINEDEV     EGC_INVLINEDEV    /* Invalid line device passed       */
#define EIC_PARMERR        EGC_INVPARM    /* Invalid parameter specified      */
#define EIC_SRL            EGC_SRL           /* SRL failure                      */
#define EIC_USRATTRNOTSET  EGC_USRATTRNOTSET /* UsrAttr was not set for this ldev*/
/*ic9*/
#define EIC_OTHERERRORS    EGC_OTHERERRORS   /* Mark start of other errors */
#define EIC_XMITALRM       EGC_XMITALRM      /* Send alarm failed                */
#define EIC_SETALRM        EGC_SETALRM       /* Set alarm mode failed            */
#define EIC_INVDEVNAME     EGC_INVDEVNAME    /* Invalid device name */
#define EIC_DTOPEN         EGC_DTOPEN        /* dt open failed */
#define EIC_ILLSTATE       EGC_ILLSTATE      /* Function is not supported in the current state */
#define EIC_FUNC_NOT_DEFINED EGC_FUNC_NOT_DEFINED /* Low level function is not defined */
/*ic10*/
#define EIC_BUSY           EGC_BUSY          /* Line is busy */
#define EIC_NOANSWER       EGC_NOANSWER      /* Ring, no answer */

#define EIC_NOT_INSERVICE   EGC_NOT_INSERVICE  /* Number not in service */
#define EIC_NOVOICE			EGC_NOVOICE			/* Need voice resource */
#define EIC_ROUTEFAIL		EGC_ROUTEFAIL		/* Routing failed */
#define EIC_DTUNLISTEN		EGC_DTUNLISTEN		/* dt_unlisten() failed */
#define EIC_DXUNLISTEN		EGC_DXUNLISTEN		/* dx_unlisten() failed */
#define EIC_DTGETXMITSLOT	EGC_DTGETXMITSLOT	/* dt_getxmitslot() failed */
#define EIC_DXGETXMITSLOT	EGC_DXGETXMITSLOT	/* dx_getxmitslot() failed */
#define EIC_DTLISTEN			EGC_DTLISTEN		/* dt_listen failed */
#define EIC_DXLISTEN			EGC_DXLISTEN		/* dx_listen failed */
#define EIC_CEPT				EGC_CEPT				/* SIT operator tone			*/
#define EIC_UNALLOCATED		EGC_UNALLOCATED	/* number not obtainable	*/
#define EIC_CONGESTION		EGC_CONGESTION		/* congestion on line 		*/
#define EIC_REJECT			EGC_REJECT			/* call is rejected			*/
#define EIC_GTCALLID			0x2F					/* No ANI digits available */
#define EIC_STARTED			EGC_GC_STARTED		/* Library already started */


/*
 * R2 protocol cause values generated if the call processing failed
 */
#define ICRV_CAUSE         0x500
#define ICRV_NORMAL       0                              /* DDI string complete              */
#define ICRV_INTERNAL     (ICRV_CAUSE | ICEC_INTERNAL)   /* Event caused by internal failure */
#define ICRV_SIGNALLING   (ICRV_CAUSE | ICEC_SIGNALLING) /* Event caused by signalling change*/
#define ICRV_TIMEOUT      (ICRV_CAUSE | ICEC_TIMEOUT)    /* Event caused by time-out         */
#define ICRV_TONE         (ICRV_CAUSE | ICEC_TONE)       /* Event caused by unexpected tone  */
#define ICRV_TEST         (ICRV_CAUSE | ICEC_TEST)       /* Call was an automatic test call  */
#define ICRV_I12          (ICRV_CAUSE | ICEC_I12)        /* Call aborted because I12 received*/
#define ICRV_PARTIAL      (ICRV_CAUSE | ICEC_PARTIAL)    /* DDI string uncomplete            */
#define ICRV_REDIAL       (ICRV_CAUSE | ICEC_REDIAL)     /* Redial policy not fulfilled      */
#define ICRV_DIALTONE     (ICRV_CAUSE | ICEC_DIALTONE)   /* Dial tone not detected           */
#define ICRV_ALARM        (ICRV_CAUSE | ICEC_ALARM)      /* event caused by alarm            */
#define ICRV_NOVOICE      (ICRV_CAUSE | ICEC_NOVOICE)    /* call needs voice, use ic_attach()*/
#define ICRV_NOANSWER     (ICRV_CAUSE | ICEC_NOANSWER)   /* Ring, no answer                  */
#define ICRV_BUSY         (ICRV_CAUSE | ICEC_BUSY)       /* Busy                             */
#define ICRV_NOT_INSERVICE (ICRV_CAUSE | ICEC_NOT_INSERVICE) /* Number not in service            */
#define ICRV_CEPT				(ICRV_CAUSE | ICEC_CEPT)		/* SIT operator tone	*/
#define ICRV_UNALLOCATED	(ICRV_CAUSE | ICEC_UNALLOCATED)	/* number is unobtainable */
#define ICRV_CONGESTION		(ICRV_CAUSE | ICEC_CONGESTION)	/* congestion on line */
#define ICRV_REJECT			(ICRV_CAUSE | ICEC_REJECT)			/* call is rejected */

/*
 * LSI board
 */
#define IC_LSIBOARD           0

/*
 * defines for response parameter for ic_ackcall(), defines of different
 * groups can be ored together. Only ICRS_ACCEPT or ICRS_REJECT is
 * mandatory, other defines are optional
 */
#define ICRS_ACCEPT           0        /* accept call                      */
#define ICRS_REJECT           1        /* reject call                      */
#define ICRS_NOANSWER         2        /* accept call but do not answer it */

#define ICAC_CHARGE           0        /* charge call (default)   */
#define ICAC_NOCHARGE         0x100    /* do not charge call  */
#define ICAC_NORING           0x0200   /* do not send any ring        */

/*
 * number of ring before answer.
 * By default, take number of ring specified in CDP
 */
#define ICAC_1RING            0x1000   /* answer after 1 ring              */
#define ICAC_2RING            0x2000   /* answer after 2 rings             */
#define ICAC_3RING            0x3000   /* answer after 3 rings             */
#define ICAC_4RING            0x4000   /* answer after 4 rings             */
#define ICAC_5RING            0x5000   /* answer after 5 rings             */
#define ICAC_6RING            0x6000   /* answer after 6 rings             */
#define ICAC_7RING            0x7000   /* answer after 7 rings             */
#define ICAC_8RING            0x8000   /* answer after 8 rings             */
#define ICAC_9RING            0x9000   /* answer after 9 rings             */
#define ICAC_10RING           0xA000   /* answer after 10 rings            */
#define ICAC_11RING           0xB000   /* answer after 11 rings            */
#define ICAC_12RING           0xC000   /* answer after 12 rings            */
#define ICAC_13RING           0xD000   /* answer after 13 rings            */
#define ICAC_14RING           0xE000   /* answer after 14 rings            */
#define ICAC_NRING            0xF000   /* let the phone ring forever       */

/*
 * Reject reasons
 */
#define ICRJ_BUSY             0x0000   /* number is busy (default)         */
#define ICRJ_UNALLOC          0x0100   /* number is not allocated          */
#define ICRJ_CONGESTION       0x0200   /* congestion in network            */
#define ICRJ_SIT              0x0300   /* send special info tone           */
#define ICRJ_REJECTED			0x0400	/* call rejected for another reason */

/*
 * option for ic_makecall() and ic_transfer()
 */
#define ICOP_CALLPROGRESS     0x0000   /* do call progress (default)       */
#define ICOP_NOCALLPROGRESS   0x0100   /* terminates after dialling        */
#define ICOP_POSTCALLPROGRESS 0x0200   /* try to identy called party       */

/*
 * type of call for ic_makecall() and ic_transfer()
 */
#define ICOP_NORMALCALL       0x0000   /* normal call (default)            */
#define ICOP_PRIORITYCALL     0x0001   /* priority call                    */
#define ICOP_OPERATOR         0x0002   /* operator call                    */
#define ICOP_MAINTENANCE      0x0003   /* automatic maintenance equipment  */
#define ICOP_DATATRANS        0x0004   /* data transmission call           */
#define ICOP_REDIRECTED       0x0005   /* redirected calls              */

/*
 * number of ring before no answer in ic_makecall() and ic_transfer()
 * By default, number of ring or delay defined in CDP file
 */
#define ICOP_1RING            0x1000   /* no answer after 1 ring           */
#define ICOP_2RING            0x2000   /* no answer after 2 rings          */
#define ICOP_3RING            0x3000   /* no answer after 3 rings          */
#define ICOP_4RING            0x4000   /* no answer after 4 rings          */
#define ICOP_5RING            0x5000   /* no answer after 5 rings          */
#define ICOP_6RING            0x6000   /* no answer after 6 rings          */
#define ICOP_7RING            0x7000   /* no answer after 7 rings          */
#define ICOP_8RING            0x8000   /* no answer after 8 rings          */
#define ICOP_9RING            0x9000   /* no answer after 9 rings          */
#define ICOP_10RING           0xA000   /* no answer after 10 rings         */
#define ICOP_11RING           0xB000   /* no answer after 11 rings         */
#define ICOP_12RING           0xC000   /* no answer after 12 rings         */
#define ICOP_13RING           0xD000   /* no answer after 13 rings         */
#define ICOP_14RING           0xE000   /* no answer after 14 rings         */
#define ICOP_NRING            0xF000   /* wait forever for answer          */

/*
 * type of transfer in ic_transfer()
 */
#define ICTR_TRANSFER         0x00     /* normal transfer                  */
#define ICTR_UNHOLD           0x80     /* original party reconnect         */

/*
 * return value for attribute functions
 */
#define AT_FAILURE            -1       /* failure for integer function     */
#define AT_FAILUREP           NULL     /* failure for pointer function     */

/*
 * specify ICAPI board device in ic_open()
 */
#define IC_UNUSED             0        /* board device                     */

/*
 * specify all devices in ic_close()
 */
#define IC_ALLDEVICES         0        /* close all devices                */

/*
 * defines for return value of ATIC_CALLRESULT()
 */
#define ICST_NOCALL           0        /* no call was previously made      */
#define ICST_ANSWERED         1        /* distant party answered           */
#define ICST_BUSY             2        /* distant party busy               */
#define ICST_CONGESTION       3        /* congestion in network            */
#define ICST_DIALDONE         4        /* dialling done, no call progress  */
#define ICST_FAX              5        /* Fax machine answered             */
#define ICST_MODEM            6        /* modem answered                   */
#define ICST_NOANSWER         7        /* no answer                        */
#define ICST_OPERATOR         8        /* operator intercept               */
#define ICST_UNOBTAINABLE     9        /* number unobtainable *            */
#define ICST_NODIALLING       10       /* no dialling because of no voice  */
#define ICST_REJECT				11			/* call is rejected						*/

/*
 * line state values returned by ATIC_LINESTATE()
 */
#define ICST_BLOCKING         1        /* terminal blocking                */
#define ICST_IDLE             2        /* terminal and network idle        */
#define ICST_ALARM            3        /* terminal in alarm                */
#define ICST_ACCEPTING        4        /* incoming call being rejected     */
#define ICST_NTBLOCKING       5        /* network blocking                 */
#define ICST_CONNECTED        6        /* call in conversation state       */
#define ICST_CONNECTING       7        /* incomming call being received    */
#define ICST_DISCONNECTING    8        /* call disconnecting               */
#define ICST_MAKINGCALL       9        /* outgoing call being made         */
#define ICST_REJECTING        10       /* incomming call being rejected    */
#define ICST_TRANSFERING      11       /* transfer in progress             */
#define ICST_PROGRESSING      12       /* outgoing call in progress        */

/*
 * defines for return value of ATIC_EVTSTATUS()
 */
#define ICST_SUCCESS          0        /* event successfull                */
#define ICST_FAIL             -1       /* failure event                    */

/*
 * defines for ic_callinfo()
 */
#define ICIF_DDI              1        /* get additional DDI digits        */

/*
 * defines for option field when getting more digits with ic_callinfo()
 * Caution: do not change these constants without looking at the code
 * (specifically r2_CallAck in r2lib.c)
 */
#define ICDG_COMPLETE         0x0000   /* no more digits after that        */
#define ICDG_PARTIAL          0x0100   /* maybe more digits after that     */

#define ICDG_1DIGIT           0x1000   /* get 1 more digit                 */
#define ICDG_2DIGIT           0x2000   /* get 2 more digits                */
#define ICDG_3DIGIT           0x3000   /* get 3 more digits                */
#define ICDG_4DIGIT           0x4000   /* get 4 more digits                */
#define ICDG_5DIGIT           0x5000   /* get 5 more digits                */
#define ICDG_6DIGIT           0x6000   /* get 6 more digits                */
#define ICDG_7DIGIT           0x7000   /* get 7 more digits                */
#define ICDG_8DIGIT           0x8000   /* get 8 more digits                */
#define ICDG_9DIGIT           0x9000   /* get 9 more digits                */
#define ICDG_10DIGIT          0xA000   /* get 10 more digits               */
#define ICDG_11DIGIT          0xB000   /* get 11 more digits               */
#define ICDG_12DIGIT          0xC000   /* get 12 more digits               */
#define ICDG_13DIGIT          0xD000   /* get 13 more digits               */
#define ICDG_14DIGIT          0xE000   /* get 14 more digits               */
#define ICDG_NDIGIT           0xF000   /* get infinite string of digits    */
#define ICDG_MAXDIGITS        14       /* Max # of digits to get           */
/*
 * defines for parm parameter of ic_setparm() and ic_getparm()
 */
#define ICPR_ALARM         GCPR_ALARM        /* enable or disable alarm handling */
#define ICPR_WAITIDLE      GCPR_WAITIDLE     /* change wait for idle time-out    */
#define ICPR_LOADTONES     GCPR_LOADTONES    /* enable or disable loading tone   */
#define ICPR_RINGBACKID    GCPR_RINGBACKID   /* GTD id for ring back tone        */
#define ICPR_OUTGUARD      GCPR_OUTGUARD     /* maximum time for call progress   */
#define ICPR_CALLINGPARTY  GCPR_CALLINGPARTY /* set or get terminal phone number */
#define ICPR_GETCATEGORY   GCPR_CATEGORY     /* request caller category          */
#define ICPR_CALLINFOTYPE	GCPR_CALLINFOTYPE	/* type of call (charge, no charge, etc.) */
#define ICPR_CONNECT_TYPE	GCPR_CONNECT_TYPE	/* perfectcall connection type         */
#define ICPR_PROTVER			GCPR_PROTVER		/* reteieve protocol version        */

#define IC_ENABLE             1        /* enable feature                   */
#define IC_DISABLE            0        /* disable feature                  */

/*
 * equates for ic_getcallinfo
 */
#define ICPR_CALLTYPE         1
#define ICPR_METERING         2

/*
 * equates for ic_getcallinfo() with ICPR_CALLTYPE
 */
#define ICCT_NORMALCALL       0        /* normal DDI call                  */
#define ICCT_CENTRALCALL      1        /* centralized call (no DDI)        */

/*
 * alarm equates
 */
#define ICAL_LOS              0x0100   /* receive loss of signal           */
#define ICAL_DPM              0x0200   /* driver performance monitor       */
#define ICAL_MFSERR           0x0800   /* multi-frame sync error           */
#define ICAL_RSA1             0x1000   /* receive signalling all 1's       */
#define ICAL_RDMA             0x2000   /* receive distant multi-frame alarm*/
#define ICAL_RLOS             0x0001   /* receive loss of sync             */
#define ICAL_RUA1             0x0002   /* receive unframed all ones        */
#define ICAL_FSERR            0x0008   /* frame sync error                 */
#define ICAL_RRA              0x0020   /* receive remote alarm             */
#define ICAL_ECS              0x0040   /* error count saturation           */
#define ICAL_BPVS             0x0080   /* bipolar violation saturation     */
#define ICAL_CECS             0x4000   /* CRC4 error count saturation      */
#define ICAL_OOF              0x8000   /* out of frame error               */
#define ICAL_RCLX             0x0010   /* receive carrier loss             */
#define ICAL_B8ZSD            0x0004   /* bipolar eight zero subs detection*/
#define ICAL_RED              0x0400   /* red alarm condition              */


/*
 * structure for protocol parameter passing
 */
#define IC_PARM GC_PARM

/*
 * ICAPI device handle type
 */
/* zs: change to long otherwise DOS will not be able to convert linedevp
       to ic_handle in large model */
typedef /*int*/long IC_DEVHANDLE;

/*
 * declaration of attribute functions
 */
#if ( defined (_MSC_VER) || defined (__STDC__) || defined (__cplusplus) )
char *ATIC_ERRMSGP( void );
long ATIC_LASTERR( void );
long ATIC_LASTDLGERR( void );
long ATIC_EVTSTATUS( void );
long ATIC_EVTEXPLAIN( void );
long ATIC_CALLRESULT( IC_DEVHANDLE icdevh );
char *ATIC_GETANIS( IC_DEVHANDLE icdevh );
long ATIC_GETCATEGORY( IC_DEVHANDLE icdevh );
char *ATIC_GETDNIS( IC_DEVHANDLE icdevh );
long ATIC_GETLANGUAGE( IC_DEVHANDLE icdevh );
long ATIC_VOICE( IC_DEVHANDLE icdevh );
long ATIC_NETWORK( IC_DEVHANDLE icdevh );
long ATIC_ALARMSTATE( IC_DEVHANDLE icdevh );
long ATIC_LINESTATE( IC_DEVHANDLE icdevh );
long ATIC_GETBOARDH( IC_DEVHANDLE icdevh );


/*
 * declaration of ICAPI functions
 */
IC_DEVHANDLE ic_open( char *pname, long boardh, long timesloth );
long ic_attach( IC_DEVHANDLE icdevh, long voich );
long ic_detach( IC_DEVHANDLE icdevh, long voich );
long ic_setparm( IC_DEVHANDLE icdevh, long parm, IC_PARM parmvalue );
long ic_getparm( IC_DEVHANDLE icdevh, long parm, IC_PARM *parmp );
long ic_getcallinfo( IC_DEVHANDLE icdevh, long infonb, IC_PARM *infop );
long ic_close( IC_DEVHANDLE icdevh );
long ic_ackcall( IC_DEVHANDLE icdevh, long response, void *userinfo, long mode );
long ic_callinfo( IC_DEVHANDLE icdevh, long info, long mode );
long ic_teardowncall( IC_DEVHANDLE icdevh, long mode);
long ic_setlinestate( IC_DEVHANDLE icdevh, long state, long mode);
long ic_makecall( IC_DEVHANDLE icdevh, char *address, long option, void *userdata, long length, long mode);
long ic_transfer( IC_DEVHANDLE icdevh, char *address, long option, void *userdata, long length, long mode);
long ic_answer( IC_DEVHANDLE icdevh, long mode );
long ic_waitcall(IC_DEVHANDLE icdevh, int timeout);
long ic_stopch(IC_DEVHANDLE icdevh);
long ic_start( );
long ic_stop( );

#else    /* K & R function prototypes  */

char *ATIC_ERRMSGP( );
long ATIC_LASTERR( );
long ATIC_LASTDLGERR( );
long ATIC_EVTSTATUS( );
long ATIC_EVTEXPLAIN( );
long ATIC_CALLRESULT( );
char *ATIC_GETANIS( );
long ATIC_GETCATEGORY( );
char *ATIC_GETDNIS( );
long ATIC_GETLANGUAGE( );
long ATIC_VOICE( );
long ATIC_NETWORK( );
long ATIC_ALARMSTATE( );
long ATIC_LINESTATE( );
long ATIC_GETBOARDH( );
long ic_waitcall();
long ic_stopch();


/*
 * declaration of ICAPI functions
 */
IC_DEVHANDLE ic_open( );
long ic_attach( );
long ic_detach( );
long ic_setparm( );
long ic_getparm( );
long ic_getcallinfo( );
long ic_close( );
long ic_ackcall( );
long ic_callinfo( );
long ic_teardowncall( );
long ic_setlinestate( );
long ic_makecall( );
long ic_transfer( );
long ic_answer( );
long ic_start( );
long ic_stop( );

#endif

#ifdef _STATIC_BUILD_
/*
 * protocol list type
 */
extern struct protocol_def {
  char *pname;                         /* protocol name                    */
  char *pheader;
} protocol_list[];
#endif	/* _STATIC_BUILD_ */

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
#define ICAPI_COMPATIBILITY_MAJOR   0x00000200
#define ICAPI_COMPATIBILITY_MINOR   0x00000001
#define COMPATIBILITY_MAJOR_MASK    0x0000ff00
#define COMPATIBILITY_MINOR_MASK    0x000000ff

/*
 * structure containing debug information
 */
typedef struct dbg_logset {
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
} DBG_LOGSET;

/*
 * structure passed to debug function
 */
typedef struct dbg_buffer {
   short n_logset;            /* number of logset in buffer */
   short index;               /* current logset index       */
   DBG_LOGSET *plogset;       /* pointer to array of logset */
} DBG_BUFFER;


/*
 * definitions of flags in ic_makecall_blk
*/
#define		NO_CALL_PROGRESS	0x1	/* normally we would not use a */
												/* double negative, but here to */
												/* maintain backward compatability where */
												/* call progress is the default, we will */
												/* use the double negative */

/* Note: this is not required to be part of gc_MakeCall() call */
/* if this is not used, the defaults are:							   */
/*		flags: 0 (i.e. call progress is used)						   */
/*		everything else is rfu												*/
												
typedef struct ic_makecall_blk {
	unsigned long		flags;
	void					*v_rfu_ptr;	/* rfu */
	unsigned long		ul_rfu[4];	/* rfu */
} IC_MAKECALL_BLK;
extern int ic_mode;

/*
 * debugging function and variables
 */
#if ( defined (__STDC__) || defined (__cplusplus) )
char *dbg_printset( DBG_LOGSET *plogset );
DBG_BUFFER *dbg_getdebug( IC_DEVHANDLE icdeh );
void dbg_setdebug( void ( *pfunc )( IC_DEVHANDLE, DBG_BUFFER *, int ) );
#else
char *dbg_printset( );
DBG_BUFFER *dbg_getdebug( );
void dbg_setdebug( );
#endif

#endif
