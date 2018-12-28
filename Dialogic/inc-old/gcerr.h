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
/**********************************************************************
*
*  C Header:      gcerr.h
*  Description:   GlobalCall error header file for application use
*  %date_created: Tue Aug 2 2005 %
*
**********************************************************************/

#ifndef _GCERR_H_
#define _GCERR_H_

/****************/
/* Error values */
/****************/

#define GC_ERROR  -1
#define EGC_NOERR             0        /* No error                         */
#define GC_SUCCESS            EGC_NOERR /* synonym of EGC_NOERR            */
#define EGC_NOCALL            1        /* No call was made or transfered   */
#define EGC_ALARM             2        /* Function interrupt by alarm      */
#define EGC_ATTACHED          3        /* specified resource already attached  */
#define EGC_DEVICE            4        /* Bad device handle                */
#define EGC_INVPROTOCOL       5        /* Ivalid protocol name             */
#define EGC_PROTOCOL          7        /* Protocol error                   */
#define EGC_SYNC              8        /* The mode flag must be EV_ASYNC   */
#define EGC_TIMEOUT           9        /* function time out                */
#define EGC_UNSUPPORTED       0xA      /* Function is not supported        */
#define EGC_USER              0xB      /* Function interrupted by user     */
#define EGC_VOICE             0xc      /* No voice resource attached       */
#define EGC_NDEVICE           0xd      /* Too many devices opened          */
#define EGC_NPROTOCOL         0xe      /* Too many protocols opened        */
#define EGC_BADFCN            0xf      /* Bad function code (TSR)          */
#define EGC_TSRNOTACTIVE      0x10     /* CCLIB not active (TSR)           */
#define EGC_COMPATIBILITY     0x11     /* incompatible components          */
#define EGC_EVTERR            0x13     /* Internal Dialogic use only       */
#define EGC_PUTEVT            0x14     /* Error queuing event              */
#define EGC_MAXDEVICES        0x15     /* Exceeded Maximum devices limit   */
#define EGC_OPENH             0x16     /* Error opening voice channel      */
#define EGC_DXOPEN            EGC_OPENH
#define EGC_INTERR            0x18     /* Internal Global Call Error */
#define EGC_NOMEM             0x19     /* Out of memory                    */
#define EGC_PFILE             0x1A     /* Error opening parameter file     */
#define EGC_TIMER             0x1B     /* Error starting timer             */
#define EGC_FILEWRITE         0x1C     /* Error writing file               */
#define EGC_SYSTEM            0x1D     /* System error                     */
#define EGC_VOXERR            0x1E     /* Internal Dialogic use only       */
#define EGC_DTIERR            0x32     /* Internal Dialogic use only       */
#define EGC_ERR               0x39     /* Internal Dialogic use only       */
#define EGC_LINERELATED       0x40     /* Error is related to line device  */
#define EGC_INVSTATE          0x41     /* Invalid state                    */
#define EGC_INVCRN            0x42     /* Invalid call reference number    */
#define EGC_INVLINEDEV        0x43     /* Invalid line device passed       */
#define EGC_INVPARM           0x44     /* Invalid parameter(argument)      */
#define EGC_SRL               0x45     /* SRL failure                      */
#define EGC_OTHERERRORS       0x80     /* Internal Dialogic use only       */
#define EGC_USRATTRNOTSET     0x81     /* UsrAttr was not set for this ldev*/
#define EGC_INVMETAEVENT      0x82     /* Invalid metaevent                */
#define EGC_GCDBERR           0x83     /* GlobalCall database error        */
#define EGC_NAMENOTFOUND      0x84     /* trunk device name name not found */
#define EGC_DRIVER            0x85     /* driver error                     */
#define EGC_FILEREAD          0x86     /* File read                        */
#define EGC_FILEOPEN          0x87     /* file open                        */
#define EGC_TASKABORTED       0x88     /* task aborted                     */
#define EGC_CCLIBSPECIFIC     0x89     /* cclib specific - a catchall      */
#define EGC_XMITALRM          0x8A     /* Send alarm failed                */
#define EGC_SETALRM           0x8B     /* Set alarm mode failed            */
#define EGC_CCLIBSTART        0x8C     /* At least one cclib failed to start*/
#define EGC_ALARMDBINIT       0x8D     /* Alarm database failed to init    */
#define EGC_INVDEVNAME        0x8E     /* Invalid device name              */
#define EGC_DTOPEN            0x8F     /* dt open failed                   */
#define EGC_GCNOTSTARTED      0x90     /* GlobalCall not started           */
#define EGC_DUPENTRY          0x91     /* inserting a duplicate entry into the database */
#define EGC_ILLSTATE          0x92     /* Function is not supported in the current state */
#define EGC_FUNC_NOT_DEFINED  0x93     /* Low level function is not defined */
#define EGC_GC_STARTED        0x94     /* GlobalCall is already started    */
#define EGC_BUSY              0x95     /* Line is busy                     */
#define EGC_NOANSWER          0x96     /* Ring, no answer                  */
#define EGC_NOT_INSERVICE     0x97     /* Number not in service            */
#define EGC_NOVOICE           0x98     /* No voice                         */
#define EGC_NORB              0x99     /* no ringback                      */
#define EGC_CEPT              0x100    /* operator intercept               */
#define EGC_STOPD             0x101    /* call progress stopped            */
#define EGC_CPERROR           0x102    /* SIT detection error              */
#define EGC_DIALTONE          0x103    /* no dial tone detected            */
#define EGC_ROUTEFAIL         0x104    /* routing failed                   */
#define EGC_DTUNLISTEN        0x105    /* dt_unlisten failed               */
#define EGC_DXUNLISTEN        0x106    /* dx_unlisten failed               */
#define EGC_AGUNLISTEN        0x107    /* ag_unlisten failed               */
#define EGC_DTGETXMITSLOT     0x108    /* dt_getxmitslot failed            */
#define EGC_DXGETXMITSLOT     0x109    /* dx_getxmitslot failed            */
#define EGC_AGGETXMITSLOT     0x10A    /* ag_getxmitslot failed            */
#define EGC_DTLISTEN          0x10B    /* dt_listen failed                 */
#define EGC_DXLISTEN          0x10C    /* dx_listen failed                 */
#define EGC_AGLISTEN          0x10D    /* ag_listen failed                 */
#define EGC_NO_MORE_AVL       0x10E    /* No more info avl                 */
#define EGC_UNALLOCATED       0x10F    /* number is unallocated            */
#define EGC_CONGESTION        0x110    /* congestion on the line           */
#define EGC_REJECT            0x111    /* call is rejected                 */
#define EGC_GETXMITSLOT       0x112    /* xx_getxmitslot failed            */
#define EGC_LISTEN            0x113    /* xx_listen failed                 */
#define EGC_UNLISTEN          0x114    /* xx_unlisten failed               */
#define EGC_RESETABLE_FATALERROR        0x115 /* resetable error                     */
#define EGC_RECOVERABLE_FATALERROR      0x116 /* recoverable error                   */
#define EGC_NON_RECOVERABLE_FATALERROR  0x117 /* non-recoverable fatal error         */
#define EGC_FATALERROR_ACTIVE           0x118 /* fatal error recovery is in progress */
#define EGC_FATALERROR_OCCURRED         0x119 /* fatal error has occurred            */
#define EGC_ASYNC             0x120    /* function must be sync            */    
#define EGC_DT_GETPARM        0x121    /* dt_getparm call failed           */
#define EGC_DT_SETPARM        0x122    /* dt_setparm call failed           */
#define EGC_ANM               0x123    /* ANM event handler creation or registration failed */
#define EGC_CCLIB_NOTFOUND    0x124    /* Call control library not found */
#define EGC_INVCCLIBFUNC      0x125    /* Invalid Call control library function */
#define EGC_INVCCLIB          0x126    /* Invalid Call control library - may be corrupted */
#define EGC_RTCMINIT          0x127    /* RTCM failed to initialize */

#define EGC_INVTARGETTYPE     0x130    /* invalid target object type */
#define EGC_INVTARGETID       0x131    /* invalid target object ID */
#define EGC_INVPARMBLK        0x132    /* invalid GC_PARM_BLKP */
#define EGC_INVDATABUFFSIZE   0x133    /* invalid parm data buffer size since it is not equal to the total parm data size */
#define EGC_INVSETID          0x134    /* invalid set ID */
#define EGC_INVPARMID         0x135    /* invalid parm ID */
#define EGC_PARM_UPDATEPERM_ERR 0x136    /* the parameter is not allowed to be updated */
#define EGC_PARM_VALUESIZE_ERR  0x137    /* value buffer size error */
#define EGC_PARM_VALUE_ERR    0x138    /* parm value error */
#define EGC_INVPARM_TARGET    0x139    /* invalid parmater for target object */
#define EGC_INVPARM_GCLIB     0x13A    /* invalid parameter for GCLib */
#define EGC_INVPARM_CCLIB     0x13B    /* invalid parameter for CCLib */
#define EGC_INVPARM_PROTOCOL  0x13C    /* invalid parameter for protocol */
#define EGC_INVPARM_FIRMWARE  0x13D    /* invalid parameter for firmware */
#define EGC_PARM_DATATYPE_ERR 0x13E    /* the parameter data type error */
#define EGC_NEXT_PARM_ERR     0x13F    /* next parm error */
#define EGC_INVUPDATEFLAG     0x140    /* Invalid update condition flag */
#define EGC_INVQUERYID        0x141    /* invalid query ID */
#define EGC_QUERYEDATA_ERR    0x142    /* source data error */
#define EGC_ATTACH_ERROR      0x143    /* error attaching media device */
#define EGC_PARM_FAILURE      0x144    /* parm failure and see additional message */
#define EGC_NOCCLIBSTARTED    0x145    /* No CCLIBS started */
#define EGC_GLARE             0x146    /* glare condition present */
#define EGC_DEVICE_REG_ERR    0x147    /* Device Registration for async open failed */
#define EGC_DEVICE_DETACHED   0x148    /* No device is attached */
#define EGC_GC_FUNC_AFTER_STOP 0x149	/* Not valid to call any GC function after gc_Stop() */
#define EGC_LOCALPARTY_PROT_TIMEOUT    0x14A /* Local party protocol time-out */ 
#define EGC_REMOTEPARTY_PROT_TIMEOUT   0x14B /* Remote party protocol time-out */
#define EGC_REMOTEPARTY_REJ            0x14C /* Remote party rejected the request */
#define EGC_INVCRN2	                   0x14D /* Invalid the second call reference number */ 
#define EGC_DT_CONFIGUREBLOCKINGALARMS 0x150 /* dt_ConfigureBlockingAlarms() failed */
#define EGC_PARMBLK_MGR_INIT           0x15A /* The GC_PARM_BLK Manager failed to initialize */
#define EGC_NO_MORE_PARMS              0x15B /* There are no more parms in the GC_PARM_BLK */
#define EGC_INVPARMDATAEXT             0x15C /* The GC_PARM_DATA_EXT is not valid */

#define EGC_NO_EMBEDDED_MEDIA_IN_3PCC  0x15D /* Attempt to media operation on 3PCC call control handle */
#define EGC_INVALID_IN_1PCC            0x15E /* Attempt to perform 3PCC only operation in 1PCC mode */
#define EGC_RESOURCE_NOT_LICENSED      0x15F /* Attempt to open a device or configure a library that */
                                             /* has not been licensed.                               */
#define EGC_SDP_ANSWER_MISSING         0x160 /* Attempt to complete a SIP invite transaction without */
                                             /* completing it's associated SDP offer/answer protocol.*/
#define EGC_VACANT_CIRCUIT             0x161 /* Vacant Circuit SIT detected */
#define EGC_SIT_UNKNOWN                0x162 /* Unknown SIT detected */
#define EGC_NO_CIRCUIT_INTERLATA       0x163 /* No Circuit Interlata SIT detected */
#define EGC_REORDER_INTERLATA          0x164 /* Reorder Interlata SIT detected */
#define EGC_INEFFECTIVE_OTHER          0x165 /* Ineffetive Other SIT detected */
#define EGC_NOT_ENABLED                0x166 /* Option not enabled */

/*****************/
/* result values */
/*****************/
#define GCRV_RESULT         0x500
#define GCRV_NORMAL         (GCRV_RESULT | 0)    /* normal completion                */
#define GCRV_ALARM          (GCRV_RESULT | 1)    /* event caused by alarm            */
#define GCRV_TIMEOUT        (GCRV_RESULT | 2)    /* event caused by timeout          */
#define GCRV_PROTOCOL       (GCRV_RESULT | 3)    /* event caused by protocol error   */
#define GCRV_NOANSWER       (GCRV_RESULT | 4)    /* event caused by no answer        */
#define GCRV_INTERNAL       (GCRV_RESULT | 5)    /* event caused internal failure   */
#define GCRV_CCLIBSPECIFIC  (GCRV_RESULT | 6)    /* event caused by cclib specific failure */
#define GCRV_NOVOICE        (GCRV_RESULT | 7)    /* Call needs voice, use ic_attach() */
#define GCRV_SIGNALLING     (GCRV_RESULT | 8)    /* Signaling change */
#define GCRV_BUSY           (GCRV_RESULT | 9)    /* Line is busy */
#define GCRV_NOT_INSERVICE  (GCRV_RESULT | 0x40) /* Number not in service        */
#define GCRV_NORB           (GCRV_RESULT | 0x41) /* no ringback           */
#define GCRV_CEPT           (GCRV_RESULT | 0x42) /* operator intercept    */
#define GCRV_STOPD          (GCRV_RESULT | 0x43) /* call progress stopped */
#define GCRV_CPERROR        (GCRV_RESULT | 0x44) /* call progress error   */
#define GCRV_DIALTONE       (GCRV_RESULT | 0x45) /* no dial tone          */
#define GCRV_UNALLOCATED    (GCRV_RESULT | 0x46) /* number not allocated */
#define GCRV_CONGESTION     (GCRV_RESULT | 0x47) /* congestion            */
#define GCRV_REJECT         (GCRV_RESULT | 0x48) /* call rejected         */
#define GCRV_FATALERROR_ACTIVE      (GCRV_RESULT | 0x49) /* Fatal error recovery is in progress */
#define GCRV_DATA_OVERWRITTEN (GCRV_RESULT | 0x4A) /* Data overwritten */
#define GCRV_RESETABLE_FATALERROR   (GCRV_RESULT | 0x50) /* resetable fatal error */
#define GCRV_RECOVERABLE_FATALERROR (GCRV_RESULT | 0x51) /* recoverable fatal error */
#define GCRV_NONRECOVERABLE_FATALERROR (GCRV_RESULT | 0x52) /* non recoverable fatal error */
#define GCRV_FATALERROR_OCCURRED    (GCRV_RESULT | 0x53) /* fatal error has occurred */
#define GCRV_NODYNMEM               (GCRV_RESULT | 0x54) /* No dynamic memory available */
#define GCRV_ATTACH_ERROR           (GCRV_RESULT | 0x55) /* event caused by failure to attach media device */
#define GCRV_INFO_PRESENT_ALL       (GCRV_RESULT | 0x56) /* The req info is available */
#define GCRV_INFO_PRESENT_MORE      (GCRV_RESULT | 0x57) /* The req info is available, and even more are present */
#define GCRV_INFO_SOME_TIMEOUT      (GCRV_RESULT | 0x58) /* The req info is not yet available. Only some info is present, not all  */
#define GCRV_INFO_SOME_NOMORE       (GCRV_RESULT | 0x59) /* The req info is not yet available. No more info is coming in */
#define GCRV_INFO_NONE_TIMEOUT      (GCRV_RESULT | 0x5A) /* The req info is not available. No info came in  */
#define GCRV_INFO_NONE_NOMORE       (GCRV_RESULT | 0x5B) /* The req info is not available. No info came in and none is expected */
#define GCRV_INFO_SENT              (GCRV_RESULT | 0x5C) /* The information has been sent successfully */
#define GCRV_DESTINATION_ADDRESS_REQ   (GCRV_RESULT | 0x5D) /* The destination information has been requested by the remote side */
#define GCRV_ORIGINATION_ADDRESS_REQ   (GCRV_RESULT | 0x5E) /* The origination information has been requested by the remote side */
#define GCRV_MEDIA_REQ_CANCELLED   (GCRV_RESULT | 0x5F) /* The media request was cancelled by the remote side */
#define GCRV_NETWORK_OPEN_FAILED   (GCRV_RESULT | 0x60) /* The network channel failed to open */
#define GCRV_MEDIA_OPEN_FAILED     (GCRV_RESULT | 0x61) /* The media channel failed to open */
#define GCRV_GLARE                 (GCRV_RESULT | 0x62) /* A glare condition occurred */
#define GCRV_CALLABANDONED         (GCRV_RESULT | 0x63) /* The call is abandoned before entering offered state */ 
#define GCRV_ROUTING_FAILED        (GCRV_RESULT | 0x64) /* Timeslot routing failed */ 
#define GCRV_XFERCALL              (GCRV_RESULT | 0x65) /* Transferred call */ 
#define GCRV_XFERCALL_CMPLT        (GCRV_RESULT | 0x66) /* Call transfer has completed */
#define GCRV_LOCALPARTY_PROT_TIMEOUT   (GCRV_RESULT | 0x67) /* Local party protocol time-out */  
#define GCRV_REMOTEPARTY_PROT_TIMEOUT  (GCRV_RESULT | 0x68) /* Remote party protocol time-out */

#define GCRV_REMOTEREJ_UNSPECIFIED     (GCRV_RESULT | 0x69) /* Remote rejected - reason not given */
#define GCRV_REMOTEREJ_UNAVAIL         (GCRV_RESULT | 0x6A) /* Remote rejected - service unavailable */
#define GCRV_REMOTEREJ_INVADDR         (GCRV_RESULT | 0x6B) /* Remote rejected - invalid number or address */
#define GCRV_REMOTEREJ_NOTALLOWED      (GCRV_RESULT | 0x6C) /* Remote rejected - the service not allowed */
#define GCRV_REMOTEREJ_INSUFFINFO      (GCRV_RESULT | 0x6D) /* Remote rejected - Insufficient information */
#define GCRV_REMOTEREJ_NOTSUBSCRIBED   (GCRV_RESULT | 0x6E) /* Remote rejected - User is not subscribed */
#define GCRV_VACANT_CIRCUIT            (GCRV_RESULT | 0x6F) /* Vacant Circuit SIT detected */
#define GCRV_SIT_UNKNOWN               (GCRV_RESULT | 0x70) /* Unknown SIT detected */
#define GCRV_NO_CIRCUIT_INTERLATA      (GCRV_RESULT | 0x71) /* No Circuit Interlata SIT detected */
#define GCRV_REORDER_INTERLATA         (GCRV_RESULT | 0x72) /* Reorder Interlata SIT detected */
#define GCRV_INEFFECTIVE_OTHER         (GCRV_RESULT | 0x73) /* Ineffetive Other SIT detected */
 
/*
-- alarm values
-- initialized such that matches values (well actually + GCRV_ RESULT + 0x10)
-- in DTT1_xxx and DTE1_xxx (as of 4/8/96).
-- Also, doesn't differentiate between T1 & E1 when the same value
-- is used for both with a different meaning. The same value
-- will be returned here (albeit from different mneumonics)
-- User is expected to know if the board is T1 or E1
-- At the present time (4/8/96) Dialogic software does not tell
-- the difference at the library level.
-- gc_ResultMsg() will use the E1 vocabulary.
*/
#define GCRV_OOF     (GCRV_RESULT | 0x10) /* out of frame error, count saturation */
#define GCRV_LOS     (GCRV_RESULT | 0x11) /* Initial loss of signal detection */
#define GCRV_DPM     (GCRV_RESULT | 0x12) /* Driver performance monitor failure */
#define GCRV_BPVS    (GCRV_RESULT | 0x13) /* Bipolar violation count saturation */
#define GCRV_ECS     (GCRV_RESULT | 0x14) /* Error count saturation */
#define GCRV_RYEL    (GCRV_RESULT | 0x15) /* Received yellow alarm */
#define GCRV_RRA     GCRV_RYEL            /* Received remote alarm */
#define GCRV_RCL     (GCRV_RESULT | 0x16) /* Received carrier loss */
#define GCRV_FERR    (GCRV_RESULT | 0x17) /* Frame bit error */
#define GCRV_FSERR   GCRV_FERR            /* Received frame sync error */
#define GCRV_B8ZSD   (GCRV_RESULT | 0x18) /* Bipolar eight zero substitution detect */   
#define GCRV_RBL     (GCRV_RESULT | 0x19) /* Received blue alarm */
#define GCRV_RUA1    GCRV_RBL             /* Received unframed all 1s */ 
#define GCRV_RLOS    (GCRV_RESULT | 0x1A) /* Received loss of sync */
#define GCRV_RED     (GCRV_RESULT | 0x1B) /* Got a read alarm condition */
#define GCRV_MFSERR  (GCRV_RESULT | 0x1C) /* Received multi frame sync error */
#define GCRV_RSA1    (GCRV_RESULT | 0x1D) /* Received signalling all 1s */
#define GCRV_RDMA    (GCRV_RESULT | 0x1E) /* Received distant multi-frame alarm */
#define GCRV_CECS    (GCRV_RESULT | 0x1F) /* CRC4 error count saturation */

/*
-- alarm values
-- initialized such that matches values (well actually + GCRV_ RESULT + 0x100)
-- in DTT1_xxx and DTE1_xxx (as of 6/21/02).
*/
#define GCRV_LOOPBACK_CFA (GCRV_RESULT + 0x100) /*Diagnostic mode on the line trunk */
#define GCRV_DCHAN_CFA    (GCRV_RESULT + 0x101) /* D channel out of service */
#define GCRV_CRC_CFA  (GCRV_RESULT + 0x102)     /* Timeslot 16 CRC failure */


/* -- recovered series -- */
#define GCRV_OOFOK   (GCRV_RESULT | 0x20) /* out of frame error, count saturation recovered */
#define GCRV_LOSOK   (GCRV_RESULT | 0x21) /* Initial loss of signal detection recovered */
#define GCRV_DPMOK   (GCRV_RESULT | 0x22) /* Driver performance monitor failure recovered */
#define GCRV_BPVSOK  (GCRV_RESULT | 0x23) /* Bipolar violation count saturation recovered */
#define GCRV_ECSOK   (GCRV_RESULT | 0x24) /* Error count saturation recovered */
#define GCRV_RYELOK  (GCRV_RESULT | 0x25) /* Received yellow alarm recovered */
#define GCRV_RRAOK   GCRV_RYELOK          /* Received remote alarm recovered */
#define GCRV_RCLOK   (GCRV_RESULT | 0x26) /* Received carrier loss recovered */
#define GCRV_FERROK  (GCRV_RESULT | 0x27) /* Frame bit error recovered */
#define GCRV_FSERROK GCRV_FERROK          /* Received frame sync error recovered */
#define GCRV_B8ZSDOK (GCRV_RESULT | 0x28) /* Bipolar eight zero substitution dtct recovered */  
#define GCRV_RBLOK   (GCRV_RESULT | 0x29) /* Received blue alarm recovered */
#define GCRV_RUA1OK  GCRV_RBLOK           /* Received unframed all 1s recovered */ 
#define GCRV_RLOSOK  (GCRV_RESULT | 0x2A) /* Received loss of sync recovered */
#define GCRV_REDOK   (GCRV_RESULT | 0x2B) /* Got a read alarm condition recovered */
#define GCRV_MFSERROK (GCRV_RESULT | 0x2C)   /* Received multi frame sync error recovered */
#define GCRV_RSA1OK  (GCRV_RESULT | 0x2D) /* Received signalling all 1s recovered */
#define GCRV_RDMAOK  (GCRV_RESULT | 0x2E) /* Received distant multi-frame alarm recovered */
#define GCRV_CECSOK  (GCRV_RESULT | 0x2F) /* CRC4 error count saturation recovered */

#define GCRV_LOOPBACK_CFAOK (GCRV_RESULT + 0x110) /* Diagnostic mode on the */
                                                  /* line trunk recovered */
#define GCRV_DCHAN_CFAOK (GCRV_RESULT + 0x111) /* D channel out of service recovered */
#define GCRV_CRC_CFAOK  (GCRV_RESULT + 0x112) /* Timeslot 16 CRC failure recovered */


#endif   /* _GCERR_H_ */
