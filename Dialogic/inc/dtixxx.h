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
/****************************************************************
* NAME          : DTIXXX.H
* VERSION       : Windows Nt (Native) Production 1
* DESCRIPTION   : Header file for all network shared ram firmware.
****************************************************************/
#ifndef _DTIXXX
#define _DTIXXX

#define DTMM_TERM       0x8000;
#define DTMM_DTERM      0x08;   
/*
 * Equates for messages TO Network firmware via shared ram interface. NTH_
 */
#define NTH_LISTEN        0x01            /* Listen to SCbus timeslot */
#define NTH_UNLISTEN      0x02            /* Stop listening to SCbus timeslot */
#define NTH_ASSIGNTTS     0x03            /* Assign local timeslot */
#define NTH_UNASSIGNTTS   0x04            /* Unassign local timeslot */
#define NTH_RDASSIGNTTS   0x05            /* Read local timeslot assignment     */
#define NTH_GETDEVINFO    0x06            /* Get device info */

#define NTH_COMTST        0x10            /* Comtest to DTI/xxx */
#define NTH_GETBDST       0x11            /* Get board status */
#define NTH_RNDIAG        0x12            /* Run diagnostics */
#define NTH_DATTST        0x13            /* Perform datatest */
#define NTH_STOPBD        0x14            /* Stop DTI/xxx execution */
#define NTH_STRTDN        0x15            /* Start code execution */
#define NTH_GETDNIN       0x16            /* Get download information */
#define NTH_GETSGMD       0x17            /* Get signalling mode */
#define NTH_GETBDSG       0x18            /* Get board signalling */
#define NTH_GETTSSG       0x19            /* Get timeslot signalling */
#define NTH_GETBDMD       0x1a            /* Get board mode */
#define NTH_SETBDTR       0x1b            /* Set board to transparent mode */
#define NTH_SETTSTR       0x1c            /* Set timeslot to transparent mode */
#define NTH_SETBDINS      0x1d            /* Set board to insertion mode */
#define NTH_SETTSINS      0x1e            /* Set timeslot to insertion mode */
#define NTH_SETID7F       0x1f            /* Set idle to 7f */
#define NTH_SETIDFF       0x20            /* Set idle to ff */
#define NTH_SETIDLE       0x21            /* Set idle pattern */
#define NTH_DSBDID        0x22            /* Disable board idle transmission */
#define NTH_DSTSID        0x23            /* Disable timeslot idle transmission */
#define NTH_ENBDID        0x24            /* Enable board idle transmission */
#define NTH_ENTSID        0x25            /* Enable timeslot idle transmission */
#define NTH_SETBDSG       0x26            /* Set board level signalling */
#define NTH_SETTSSG       0x27            /* Set timeslot level signalling */
#define NTH_TONEOFF       0x28            /* Set test tone to OFF */
#define NTH_TONEON        0x29            /* Set test tone to ON */
#define NTH_DSPD          0x2a            /* Disable pulse digit detection */
#define NTH_ENPD          0x2b            /* Enable pulse digit detection */
#define NTH_GETMM         0x2c            /* Get message mask */
#define NTH_GETTSST       0x2d            /* Get timeslot status */
#define NTH_WRTRAM        0x2e            /* Write to ram */
#define NTH_RDRAM         0x2f            /* Read from ram */
#define NTH_SETGLOB       0x30            /* Set global parameter */
#define NTH_GETGLOB       0x31            /* Read global parameter */
#define NTH_SETBDMD       0x32            /* Set board mode */
#define NTH_DSBDMM        0x33            /* Disable board message mask */
#define NTH_DSTSMM        0x34            /* Disable timeslot message mask */
#define NTH_ENBDMM        0x35            /* Enable board message mask */
#define NTH_ENTSMM        0x36            /* Enable timeslot message mask */
#define NTH_DSERRC        0x37            /* Disable error conditions */
#define NTH_ENERRC        0x38            /* Enable error conditions */
#define NTH_SETCLK        0x39            /* Set clock source */
#define NTH_SETALRM       0x3a            /* Set board alarm handling mode */
#define NTH_GETALRM       0x3b            /* Get board alarm handling mode */
#define NTH_SENDYEL       0x3c            /* Transmit a yellow alarm */
#define NTH_STOPYEL       0x3d            /* Stop transmitting a yellow alarm */
#define NTH_SENDBLU       0x3e            /* Transmit a blue alarm */
#define NTH_STOPBLU       0x3f            /* Stop transmitting a blue alarm */
#define NTH_SNDWINK       0x40            /* Transmit a wink */
#define NTH_WAITCALL      0x41            /* Wait for a call */
#define NTH_MTFCN         0x42            /* Start multi-tasking function */
#define NTH_PDDIGS        0x43            /* Pulse dial digits */
#define NTH_STOPTS        0x44            /* Stop multi-tasking function */
#define NTH_SENDDMA           0x45        /* Send distant multiframe alarm */
#define NTH_STOPDMA           0x46        /* Send distant multiframe alarm */
#define NTH_SENDSA1           0x47        /* Send signalling all 1's alarm */
#define NTH_STOPSA1           0x48        /* Stop signalling all 1's alarm */
/* mlk 980923 - next set of defines were added for NDIS support */
#define NTH_DATA_LISTEN        0x49            /* Listen to SCbus timeslot */
#define NTH_DATA_UNLISTEN      0x4a            /* Stop listening to SCbus timeslot */
#define NTH_DATA_ASSIGNTTS     0x4b            /* Assign local timeslot */
#define NTH_DATA_UNASSIGNTTS   0x4c            /* Unassign local timeslot */
#define NTH_DATA_RDASSIGNTTS   0x4d            /* Read local timeslot assignment     */
#define NTH_DATA_GETDEVINFO    0x4e            /* Get device info */
/* end of NDIS addition */
#define NTH_CAS_TEMPLATE_MGMT 0x50        /* Template mgmt generic command */
#define NTH_SETID54       0x1f            /* Set idle to 7f */
#define NTH_SETIDD5       0x20            /* Set idle to ff */
#define NTH_SENDUA1       NTH_SENDBLU     /* Transmit unframed all 1's   */
#define NTH_STOPUA1       NTH_STOPBLU     /* Stop transmitting unframed all 1's*/
#define NTH_SENDREM       NTH_SENDYEL     /* Transmit remote alarm.                             */
#define NTH_STOPREM       NTH_STOPYEL     /* Stop xmitting remote alarm.                */
#define NTH_GETFTR        0x70            /* Get Feature List */
#define NTH_DEBUG         0xD0            /* Hardware debug command  */
#define NTH_SETSENDALIVE  0xE0			  /* Set SendAlive feature on */
#define NTH_GETSENDALIVE  0xE1            /* Ping FW for SendAlive */
/*
 * Equates for MSI specific message from host to firmware via shared RAM MTH_
 */
#define MTH_ADDCNF      0x80            /* Add party to a conference */
#define MTH_DELCNF      0x81            /* delete conference */
#define MTH_ESTCNF      0x82            /* Establish a conference */
#define MTH_GETCDE      0x83            /* Get conf descr. element */
#define MTH_MONCNF      0x84            /* Monitor a conference */
#define MTH_RMVCNF      0x85            /* Remove a party from conf */
#define MTH_SNDZIP      0x86            /* Send zip */
#define MTH_SETCDE      0x87            /* Set conf. descr. element */
#define MTH_STAVOL      0x88            /* Station volume adjust */
#define MTH_GETSTA      0x89            /* Get dau. brd/stat info */
#define MTH_RNGSTA      0x8a            /* Ring station */
#define MTH_RMMCNF      0x8b            /* Remove monitor from conf */
#define MTH_GETMM       0x8c            /* Get message mask */
#define MTH_ALLOCTTS    0x8d            /* Alloc SCbus xmit timeslot */
#define MTH_UALLOCTTS   0x8e            /* Unalloc. xmit timeslot */
#define MTH_DSASMSG     0x8f            /* Disable spec msg from all stat */
#define MTH_ENALLSTM    0x90            /* Enable spec msg from all stat */
#define MTH_GETTSREQ    0x92            /* Get timeslot requirements */
#define MTH_GETBRDPARM  0x93            /* Get board parameter */
#define MTH_SETBRDPARM  0x94            /* Set board parameter */
#define MTH_DSPRESCOUNT 0x95            /* Set DSP reset count */
#define MTH_STOPMT      0x96            /* Stop multitasking function */
#define MTH_DELXTDX     0x97            /* Delete Extended connection */
#define MTH_CHGXTDX     0x98            /* Change Extended connection */
#define MTH_ESTXTDX     0x99            /* Establish extended connection */
#define MTH_GETCNFLST   0x9a            /* Get conference list */
#define MTH_GETCNFMM   0x9b             /* Get dtmf detection mask */
#define MTH_MSEVRING    0x9c            /* Ring termination */
#define MTH_ENCNFMM     0x9e            /* Enable dtmf detection */
#define MTH_DSCNFMM     0x9f            /* Disable dtmf detection */
#define MTH_DCBGETTSREQ 0xb4            /* DCB get time slot requirements */
#define MTH_SETSTPARM   0xb5            /* Set Station Parameters */
/*
 * Equates for messages FROM Network firmware via shared ram interface. NTT_
 */
#define NTT_LISTEN        0x01  /* Reply to listen to SCbus timeslot */
#define NTT_UNLISTEN      0x02  /* Reply to stop listening to SCbus timeslot */
#define NTT_ASSIGNTTS     0x03  /* Assign transmit timeslot complete */
#define NTT_UNASSIGNTTS   0x04  /* Unassign transmit timeslot complete */
#define NTT_RDASSIGNTTS   0x05  /* Read local timeslot assignment complete */
#define NTT_GETDEVINFO    0x06  /* Get device info */

#define NTT_COMRSP        0x10            /* Reply to host comtest */
#define NTT_RETBDST       0x11            /* Reply to get board status */
#define NTT_RETDIAG       0x12            /* Reply to run diagnostics */
#define NTT_DATRSP        0x13            /* Reply to perform datatest */
#define NTT_STOPBD        0x14            /* Reply to stop DTI/xxx execution */
#define NTT_STRTDN        0x15            /* Reply to start code execution */
#define NTT_RETDNIN       0x16            /* Reply to get download information */
#define NTT_RETSGMD       0x17            /* Reply to get signalling mode */
#define NTT_RETSG         0x18            /* Reply to get board signalling */
#define NTT_RETTSSG       0x19            /* Reply to get timeslot signalling */
#define NTT_RETBDMD       0x1a            /* Reply to get board mode */
#define NTT_SETBDTR       0x1b            /* Reply to set board to transparent mode */
#define NTT_SETTSTR       0x1c            /* Reply to set timeslot to transparent mode */
#define NTT_SETBDINS      0x1d            /* Reply to set board to insertion mode */
#define NTT_SETTSINS      0x1e            /* Reply to set timeslot to insertion mode */
#define NTT_SETID7F       0x1f            /* Reply to set idle to 7f */
#define NTT_SETIDFF       0x20            /* Reply to set idle to ff */
#define NTT_SETIDLE       0x21            /* Reply to set idle pattern */
#define NTT_DSBDID        0x22            /* Reply to disable board idle transmission */
#define NTT_DSTSID        0x23            /* Reply to disable timeslot idle transmission */
#define NTT_ENBDID        0x24            /* Reply to enable board idle transmission */
#define NTT_ENTSID        0x25            /* Reply to enable timeslot idle transmission */
#define NTT_SETBDSG       0x26            /* Reply to set board level signalling */
#define NTT_SETTSSG       0x27            /* Reply to set timeslot level signalling */
#define NTT_TONEOFF       0x28            /* Reply to set test tone to OFF */
#define NTT_TONEON        0x29            /* Reply to set test tone to ON */
#define NTT_DSPD          0x2a            /* Reply to disable pulse digit detection */
#define NTT_ENPD          0x2b            /* Reply to enable pulse digit detection */
#define NTT_RETMM         0x2c            /* Reply to get message mask */
#define NTT_RETTSST       0x2d            /* Reply to get timeslot status */
#define NTT_WRTRAM        0x2e            /* Reply to write to ram */
#define NTT_RETRAM        0x2f            /* Reply to read from ram */
#define NTT_SETGLOB       0x30            /* Reply to set global parameter */
#define NTT_RETGLOB       0x31            /* Reply to read global parameter */
#define NTT_SETBDMD       0x32            /* Reply to set board mode */
#define NTT_DSBDMM        0x33            /* Reply to disable board message mask */
#define NTT_DSTSMM        0x34            /* Reply to disable timeslot message mask */
#define NTT_ENBDMM        0x35            /* Reply to enable board message mask */
#define NTT_ENTSMM        0x36            /* Reply to enable timeslot message mask */
#define NTT_DSERRC        0x37            /* Reply to disable error conditions */
#define NTT_ENERRC        0x38            /* Reply to enable error conditions */
#define NTT_SETCLK        0x39            /* Reply to set clock source */
#define NTT_SETALRM       0x3a            /* Reply to set board alarm handling mode */
#define NTT_RETALRM       0x3b            /* Reply to get board alarm handling mode */
#define NTT_SENDYEL       0x3c            /* Reply to transmit a yellow alarm */
#define NTT_STOPYEL       0x3d            /* Reply to stop transmitting a yellow alarm */
#define NTT_SENDBLU       0x3e            /* Reply to transmit a blue alarm */
#define NTT_STOPBLU       0x3f            /* Reply to stop transmitting a blue alarm */
#define NTT_WINKCMP       0x40            /* Reply to transmit a wink */
#define NTT_WAITCALL      0x41            /* Reply to wait for a call */
#define NTT_MTFCN         0x42            /* Multi-tasking function complete */
#define NTT_PDDIGS        0x43            /* Reply to pulse dial digits */
#define NTT_SENDDMA       0x45            /* Send distant multiframe alarm */
#define NTT_STOPDMA       0x46            /* Send distant multiframe alarm */
#define NTT_SENDSA1       0x47            /* Send signalling all 1's alarm */
#define NTT_STOPSA1       0x48            /* Stop signalling all 1's alarm */
#define NTT_RETSETSA      0xE0            /* Reply to set send alive */
#define NTT_RETGETSA      0xE1            /* Reply to get send alive timer and threshold */
#define NTT_CAS_TEMPLATE_MGMT 0x50        /* Template mgmt generic command ack msg*/
#define NTT_SETID54       0x1f            /* Set idle to 7f */
#define NTT_SETIDD5       0x20            /* Set idle to ff */
#define NTT_SENDUA1       NTH_SENDBLU     /* Transmit unframed all 1's   */
#define NTT_STOPUA1       NTH_STOPBLU     /* Stop transmitting unframed all 1's*/
#define NTT_SENDREM       NTH_SENDYEL     /* Transmit remote alarm.                             */
#define NTT_STOPREM       NTH_STOPYEL     /* Stop xmitting remote alarm.                */
#define NTT_BRDERR        0x60            /* Board level error message */
#define NTT_TSERR         0x61            /* Timeslot level error message */
#define NTT_SIGTRCV       0x62            /* Signalling transition received */
#define NTT_NETERRC       0x63            /* Network error condition detected */
#define NTT_WINKREC       0x64            /* Wink received */
#define NTT_RCVPDG        0x65            /* Pulse digit received */
#define NTT_REMLPBK       0x66            /* Went into remote loopback */
#define NTT_OVRFLW        0x67            /* Overflow of buffer */
#define NTT_RLPBKOK       0x68            /* Came out of remote loopback */
#define NTT_CAS_TEMPLATE_MATCH 0x6a       /* Event returned on template match */
#define NTT_CAS_TEMPLATE_SEND_END 0x6b	  /* Event returned on template transmit finished */
#define NTT_RETFTR        0x70            /* Return Feature List */
#define NTT_CLKFALLS      0xC0            /* Clk Fallback Status announcement*/
#define NTT_DEBUG         0xD0            /* Hardware debug command  */

/*
 * Equates for MSI specific message from firmware to host via shared RAM MTT_
 */
#define MTT_EVTRAN      0x69            /* Event transition on MSI/240 */
#define MTT_ADDCNF      0x80            /* Add party to a conference reply */
#define MTT_DELCNF      0x81            /* delete conference reply */
#define MTT_ESTCNF      0x82            /* Establish a conference reply */
#define MTT_GETCDE      0x83            /* Get conf descr. element reply */
#define MTT_MONCNF      0x84            /* Monitor a conference reply */
#define MTT_RMVCNF      0x85            /* Remove a party from conf reply */
#define MTT_SNDZIP      0x86            /* Send zip reply */
#define MTT_SETCDE      0x87            /* Set conf. descr. element reply */
#define MTT_STAVOL      0x88            /* Station volume adjust reply */
#define MTT_GETSTA      0x89            /* Get dau. brd/stat info reply */
#define MTT_RNGSTA      0x8a            /* Ring station reply */
#define MTT_RMMCNF      0x8b            /* Remove monitor from conf reply */
#define MTT_RETMM       0x8c            /* Return message mask */
#define MTT_ALLOCTTS    0x8d            /* Alloc SCbus xmit timeslot reply */
#define MTT_UALLOCTTS   0x8e            /* Unalloc. xmit timeslot reply */
#define MTT_DSASMSG     0x8f            /* Dis. spec msg from all stat reply */
#define MTT_ENALLSTM    0x90            /* En. spec msg from all stat reply */
#define MTT_GETTSREQ    0x92            /* Get timeslot requirements */
#define MTT_GETBRDPARM  0x93            /* Get board parameter reply */
#define MTT_SETBRDPARM  0x94            /* Set board parameter reply */
#define MTT_DSPRESCOUNT 0x95            /* Set DSP reset count */
#define MTT_STOPMT      0x96            /* Stop multitasking function */
#define MTT_DELXTDX     0x97            /* Delete Extended connection */
#define MTT_CHGXTDX     0x98            /* Change Extended connection */
#define MTT_ESTXTDX     0x99            /* Establish extended connection */
#define MTT_GETCNFLST   0x9a            /* Get conference list */
#define MTT_GETCNFMM    0x9b            /* Get More conference */
#define MTT_MSEVRING    0x9c            /* Solicited ring event */
#define MTT_GETCNFMORE  0x9d            /* Get More conference */ 
#define MTT_ENCNFMM     0x9e            /* Enable dtmf detection */
#define MTT_DSCNFMM     0x9f            /* Disable dtmf detection */

#define MTT_ATT         0xb1            /* Active Talker Table Update */
#define MTT_DETDIG      0xb2            /* DTMF detected */
#define MTT_DCBGETTSREQ 0xb4            /* DCB/SC get ts requirements */
#define MTT_SETSTPARM   0xb5            /* Set Station Parameters */


/*
 * SCX Unsolicited Event
 */
#define MTT_SCL_EVNTMSG 0xc8                    /* SCX Unsolicited Event */
/*
 * Network signalling message mask equates. NETMM_
 */
#define NETMM_AON         0x0001          /* Mask for A went to on msgs.   */
#define NETMM_AOFF        0x0002          /* Mask for A went to off msgs.  */
#define NETMM_BON         0x0004          /* Mask for B went to on msgs.   */
#define NETMM_BOFF        0x0008          /* Mask for B went to off msgs.  */
#define NETMM_WINK        0x0010          /* Mask for sig bit wink.        */
#define NETMM_CON         0x0100          /* Mask for C went to on msgs.   */
#define NETMM_COFF        0x0200          /* Mask for C went to off msgs.  */
#define NETMM_DON         0x0400          /* Mask for D went to on msgs.   */
#define NETMM_DOFF        0x0800          /* Mask for D went to off msgs.  */

/*
 * Network firmware timeslot signalling masks. NETSG_
 */
#define NETSG_RCVA        0x01            /* Value of receive A.           */
#define NETSG_RCVB        0x02            /* Value of receive B.           */
#define NETSG_XMTA        0x04            /* Value of transmit A.          */
#define NETSG_XMTB        0x08            /* Value of transmit B.          */
#define NETSG_RCVC        0x10            /* Value of receive C.           */
#define NETSG_RCVD        0x20            /* Value of receive D.           */
#define NETSG_XMTC        0x40            /* Value of transmit C.          */
#define NETSG_XMTD        0x80            /* Value of transmit D.          */

/*
 * Network mask for enabling and disabling DTI/xxx alarm conditions. NETAM_
 */
/*
 * MSB of mask.
 */
#define NETAM_LOS         0x0100          /* Loss of Signal.                 */
#define NETAM_DPM         0x0200          /* Driver perf. mon. alarm.        */
#define NETAM_RED         0x0400          /* Red alarm condition mask.       */
#define NETAM_OOF         0x8000          /* OOF condition mask.             */

/*
 * LSB of mask.
 */
#define NETAM_RLOS        0x0001          /* Receive loss of sync.           */
#define NETAM_RUA1        0x0002          /* Receive unframed all 1's (blue) */
#define NETAM_B8ZSD       0x0004          /* Bipolar 8 zero subs. detection  */
#define NETAM_FSERR       0x0008          /* Frame resync alarm.             */
#define NETAM_RCL         0x0010          /* Receive carrier loss            */
#define NETAM_RRA         0x0020          /* Receive remote alarm (yellow).  */
#define NETAM_ECS         0x0040          /* Error count saturation.         */
#define NETAM_BPVS        0x0080          /* Bipolar viol. count saturation. */

/*
 * Network device parameter numbers. NETDP_
 */
#define NETDP_MINPDON      0x00           /* Min pulse dial reception on.    */
#define NETDP_MINPDOFF     0x01           /* Min pulse dial reception off.   */
#define NETDP_MAXPDOFF     0x02           /* Max interdigit reception delay  */
#define NETDP_MINIPD       0x03           /* Max pulse digit reception off.  */
#define NETDP_P_MK         0x04           /* Send pulse make time.           */
#define NETDP_P_BK         0x05           /* Send pulse break time.          */
#define NETDP_P_IDD        0x06           /* Send pulse interdigit delay.    */
#define NETDP_PDDLFTIM     0x07           /* Flash time.                     */
#define NETDP_PDFLASH      0x08           /* Flash character.                */
#define NETDP_CABTYPE      0x09           /* LIU cable length & type values  */
#define NETDP_OOFMAX       0x0A           /* Out of frame errors max <=15.   */
#define NETDP_FECSMAX      0x0B           /* Frame error count saturation.   */
#define NETDP_BPVCMAX      0x0C           /* Bipolar violtn cnt max <=255.   */
#define NETDP_RDEBON       0x0D           /* Rcv debnce on val (in 10ms).    */
#define NETDP_RDEBOFF      0x0E           /* Rcv debnce off val (n 10ms).    */
#define NETDP_PCDEAD       0x0F           /* Signalling dead parameter.      */
#define NETDP_RCVTOL       0x10           /* Low word of rcv tmout (& ack).  */
#define NETDP_RCVTOH       0x11           /* Hi byte of rcv & ack tmout.     */
#define NETDP_CODESUPR     0x12           /* Suppression value.              */
#define NETDP_WINKMAX      0x13           /* Wink time, in 10ms units.       */
#define NETDP_PDPSTIM      0x16           /* Pulse dialing pause time.       */
#define NETDP_WINKMIN      0x17           /* Wink time, in 10ms units.       */
#define NETDP_WINKLEN      0x18           /* Wink length(xmt), in 10ms units */
#define NETDP_PREWINK      0x19           /* Pre wink delay, in 10ms units.  */
#define NETDP_CECRMAX      0x1B           /* Max CRC4 error count <= 255.    */
#define NETDP_ECRRSTTM     0x1C           /* Err cnt reg rst tim(100ms unts) */
#define NETDP_RXTXIDLE     0x1D           /* Rx & Tx Idle Patterns           */
#define NETDP_SEIZESIG     0x1E           /* Line Seizure and Response       */

/*
 * Network firmware message processing error codes. NTE_
 */
#define NET_OK             0x00           /* Successful */
#define NET_INVCFG         0xA0           /* Invld cfg area or EEPROM cfg data*/
#define NET_INVMSG         0xA1           /* Invalid message.                 */
#define NET_MBFMT          0xA2           /* Wrong # bytes for this mb req.   */
#define NET_MSGCNT         0xA3           /* Byte count wrong in dnld msg.    */
#define NET_CHKSUM         0xA4           /* Bad chksum in download message.  */
#define NET_NOTDNLD        0xA5           /* Not down loaded.                 */
#define NET_MBIMM          0xA6           /* Received an immediate term.      */
#define NET_INVTS          0xA7           /* Invalid timeslot.                */
#define NET_INVBD          0xA8           /* Wrong board.                     */
#define NET_SIGINS         0xA9           /* Insertion sig not enabled.       */
#define NET_BADGLOB        0xAA           /* Bad global parameter number.     */
#define NET_TSASN          0xAB           /* Time slot already assigned.      */
#define NET_BADVAL         0xAC           /* Bad value.                       */
#define NET_STARTED        0xAD           /* Can't start when already startd. */
#define NET_TSBSY          0xAE           /* Tslot is busy (multi-tsking fnc) */
#define NET_PDOFFHK        0xAF           /* Must be offhk idle to pd.        */
#define NET_MTSIG          0xB0           /* Can't disable insert if multi-tsk*/
#define NET_BADCNT         0xB1           /* Cnt of bytes requested is bad.   */
#define NET_ADDRS          0xB2           /* Bad address.                     */
#define NET_SIGTO          0xB3           /* xmt/rcv did not update in time.  */
#define NET_NOTSACS        0xB4           /* No tsacs on this device.         */
#define NET_NOCLK          0xB5           /* No PEB clk src - can't set clk.  */
#define NET_TSTMOD         0xB6           /* In test mode - can't set dev md. */
#define NET_BITBSY         0xB7           /* This bit is busy (already "set") */
#define NET_INVSIGST       0xB8           /* Invalid signalling state.        */
#define NET_NOWTCALL       0xB9           /* Not waiting for a call.          */
#define NET_NOTPEBMODE     0xBA           /* Not in PEB mode                  */

#define NET_SHLIB_BUSY          0x51        /* SH_SHLIB_BUSY+0x50 : SH library is busy processing another request. */
#define NET_BAD_SHINDX          0x52        /* SH_BAD_SHINDX+0x50 : Invalid SH index number. */
#define NET_SHLIB_NOT_INIT      0x53        /* SH_SHLIB_NOT_INIT+0x50 : SH library has not been initialized via sh_setloc(). */
#define NET_NO_CLOCK            0x54        /* SH_NO_CLOCK+0x50 : Clock fallback failed. */
#define NET_NO_SH_PRESENT       0x56        /* SH_NO_SH_PRESENT+0x50 : SH is not present. */
#define NET_BAD_MODE            0x5A        /* SH_BAD_MODE+0x50 : Invalid bus mode. */
#define NET_BAD_LOCTS           0x5F        /* SH_BAD_LOCTS+0x50 : Invalid local timeslot parameter. */
#define NET_BAD_TYPE            0x60        /* SH_BAD_TYPE+0x50 : Invalid type. */
#define NET_BAD_EXTTS           0x61        /* SH_BAD_EXTTS+0x50 : External timeslot not supported at current clock rate. */
#define NET_LOCTS_SET           0x62        /* SH_LOCTS_SET+0x50 :Local timeslot already connected. */
#define NET_LOCTS_DISCONNECT    0x63        /* SH_LOCTS_DISCONNECT+0x50 : Local timeslot disconnected. */

/*
 * Network firmware pulse dialing variable equates. NETV_
 */
#define NETV_MAXPDCHR      0x1F           /* Maximum pulse digit chars.       */
#define NETV_FLASHCHR      '&'            /* Def flash char (can be changed)  */
#define NETV_DELAYCHR      ','            /* Delay character (not changable)  */

/*
 * Network device status masks. NETS_
 */
#define NETS_DNLDERR       0x01           /* Bit 0 0=ok, 1=err.               */
#define NETS_DNLD          0x02           /* Bit 1 0=not dwnld, 1=dwnld.      */
#define NETS_DNLDSTRT      0x04           /* Bit 2 0=not started, 1=started.  */
#define NETS_CLKBIT1       0x08           /* Bit 3 - clock source bit 1.      */
#define NETS_CLKBIT2       0x10           /* Bit 4 - clock source bit 1.      */
#define NETS_RLOOPBIT      0x20           /* Remote loopback bit.             */

/*
 * Network hardware diagnostic error codes. NETD_
 */
#define NETD_NOERR         0x00           /* No errors.                       */
#define NETD_MEMTST        0x01           /* Memory test failed.              */
#define NETD_LIUFAIL       0x02           /* Read/Write to LIU failed.        */
#define NETD_XCVRFAIL      0x03           /* Read/write of XCVR failed.       */
#define NETD_ROMCHK        0x04           /* Incorrect ROM checksum.          */
#define NETD_BRDCFG        0x05           /* Invalid board configuration data */
#define NETD_INVEE         0x06           /* Invalid EEPROM data.             */

/*
 * Network multitasking functions.  NETMT_
 */
#define NETMT_WINK         0x00           /* Send Wink.                      */
#define NETMT_STOP         0x01           /* Stop a multitasking function.   */
#define NETMT_WTCALL       0x02           /* Wait for a call.                */

/*
 * Network multitasking status values. NETMTS_
 */
#define NETMTS_IDLE        0x00           /* Idle - no multitasking active.  */
#define NETMTS_PDIALING    0x01           /* Pulse dialing function active.  */
#define NETMTS_WINKING     0x02           /* Transmit wink function active.  */
#define NETMTS_WTCALL      0x03           /* Waiting for call func active.   */

/*
 * Network firmware signalling modes.  NETSG_
 */
#define NETSGM_INSERT      0x00           /* Signalling insertion.           */
#define NETSGM_TRANSP      0x01           /* Transparant signalling.         */

/*
 * Network firmware clock source equates. NETC_
 */
#define NETC_NOCLK         0x00           /* No clocking.                    */
#define NETC_LOOP          0x01           /* Loop timing.                    */
#define NETC_EXT           0x02           /* Clock from the expansion bus.   */
#define NETC_IND           0x03           /* On brd crystal- indep timing.   */

/*
 * Network device modes. NETMD_
 */
#define NETMD_NORMAL       0x00           /* Normal mode.                    */
#define NETMD_XCVRLB       0x01           /* Tranceiver local loop back mode */
#define NETMD_LIULLB       0x02           /* LIU local loop back mode        */
#define NETMD_LIURLB       0x03           /* LIU remote loop back mode       */

/*
 * Network alarm condition values. NETA_
 */
#define NETA_OOF           0x00           /* Out of Frame */

#define NETA_LOS           0x01           /* Initial loss of signal detection.*/
#define NETA_DPM           0x02           /* Driver performance monitor.      */
#define NETA_BPVS          0x03           /* Bipolar violation cnt saturation */
#define NETA_ECS           0x04           /* Error count saturation.          */
#define NETA_RRA           0x05           /* Receive remote alarm.            */
#define NETA_RCL           0x06           /* Receive carrier loss.            */
#define NETA_FSERR         0x07           /* Frame resync in progress.        */
#define NETA_B8ZSD         0x08           /* Bipolar 8 zero substitution detec*/
#define NETA_RUA1          0x09           /* Receive unframed all 1's.        */
#define NETA_RLOS          0x0A           /* Receive loss of sync.            */
#define NETA_RED           0x0B           /* Red alarm condition.             */
#define NETA_MFSERR        0x0C           /* Multi-frame sync error.          */
#define NETA_RSA1          0x0D           /* Receive signalling all 1's.      */
#define NETA_RDMA          0x0E           /* Rcved distant multi-frame alarm. */
#define NETA_CECS          0x0F           /* CRC4 error count saturation.     */
#define NETA_OK            0x10
#define NETA_OOFOK         (NETA_OOF+NETA_OK)
#define NETA_LOSOK         (NETA_LOS+NETA_OK)   /* Signal restored.           */
#define NETA_DPMOK         (NETA_DPM+NETA_OK)   /* Driver perf. mon. restored */
#define NETA_BPVSOK        (NETA_BPVS+NETA_OK)  /* Bipolar viol cnt saturation*/
#define NETA_ECSOK         (NETA_ECS+NETA_OK)   /* Err cnt saturation restored*/
#define NETA_RCLOK         (NETA_RCL+NETA_OK)   /* Carrier Loss recovered     */
#define NETA_RRAOK         (NETA_RRA+NETA_OK)   /* Remote alarm recovered.    */
#define NETA_FSERROK       (NETA_FSERR+NETA_OK) /* Frame resync complete.     */
#define NETA_B8ZSDOK       (NETA_B8ZSD+NETA_OK) /* B8ZS Substitution recovered*/
#define NETA_RUA1OK        (NETA_RUA1+NETA_OK)  /* Unframed all 1's recovered */
#define NETA_REDOK         (NETA_RED+NETA_OK)   /* Red alarm recovered        */
#define NETA_RLOSOK        (NETA_RLOS+NETA_OK)  /* Rcv loss of sync restored  */

/*
 * Network firmware alarm handling modes. NTAMD_
 */
#define NTAMD_NONE         0x00           /* No alarm handling.               */
#define NTAMD_TERM         0x01           /* Terminate alarm handling mode.   */
#define NTAMD_DROP         0x02           /* Drop and insert alarm handling.  */

/*
 * Network T1 suppression values. NT1SP_
 */
#define  NT1SP_TRAN        0x00           /* Suppression is transparent    */
#define  NT1SP_B8ZS        0x01           /* Suppression is B8ZS           */
#define  NT1SP_BIT7        0x02           /* Suppression is Bit 7          */
#define  NT1SP_MAX         0x03           /* Number of code suppression modes */

/*
 * Network cable line length values. NTLL_   (T1 only)
 */
#define NTLL_000           0x00           /* 000-110 ft */
#define NTLL_110           0x01           /* 110-220 ft */
#define NTLL_220           0x03           /* 220-330 ft */
#define NTLL_330           0x02           /* 330-440 ft */
#define NTLL_440           0x06           /* 440-550 ft */
#define NTLL_550           0x07           /* 550-655 ft */
#define NTLL_655           0x05           /* > 655 ft   */
#define NTLL_SQUARE        0x04           /* Square pulse */

/*
 * Network device types. NETDVT_
 */
#define NETDVT_DTI2X2      0x90           /* DTI/212 or DTI/222 */
#define NETDVT_DTI2X1      0x91           /* DTI/211 or DTI/221 */
#define NETDVT_MSI         0xA0           /* MSI */
#define NETDVT_MSIC        0xA1           /* MSI-C (with conferencing) */
#define NETDVT_E1MSIC      0xA2           /* MSI-C 2.048MHz clock & conference*/
#define NETDVT_DMX         0xB0           /* DMX */
#define NETDVT_EDMX        0xB1           /* EDMX */

#define NTH_DEVNUM 0x0002
#define NTH_TERM   0x00FF

#endif

