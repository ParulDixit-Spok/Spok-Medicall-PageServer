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

/*****************************************************************************
 * Filename:    isdncmd.h 
 * DESCRIPTION: Header file for all ISDN network firmware shared ram interface..   
 *****************************************************************************/

#ifndef _ISDNCMD
#define _ISDNCMD

#include "isdnos.h"

#define CALLID_FLAG              0x80     /* CALL ID PRESENT INDICATOR */

/*
 * Call Control states
 */
#define CC_NULL                  0        /* call released */
#define CC_OFFERED               1        /* inbound call received */
#define CC_ACCEPTED              2        /* call accepted */
#define CC_CONNECTED             3        /* call connected */
#define CC_INITIATED             4        /* outbound call request */
#define CC_FAILED                5        /* not applicable */
#define CC_ALERTING              6        /* call alerted send or received */
#define CC_DISCONNECTED          7        /* disconnect initiated Network */
#define CC_IDLE                  8        /* disconnect initiated by Host */  
#define CC_HOLD                  13       /* call is on hold */


/*
 * Equates for messages TO ISDN firmware via shared ram interface. ISDN_
 */
                                          /* H - Host, B - Board */
#define ISDN_GETDEF              0xFF     /* H ->B get default value of 
                                             mandatory IE */
#define ISDN_SETDEF              0xFE     /* H<->B set default value of 
                                             mandatory IE */
#define ISDN_RETDEF              0xFD     /* H<- B return default value of 
                                             mandatory IE */
#define ISDN_GETMM               0xFC     /* H ->B get message mask */
#define ISDN_SETMM               0xFB     /* H<->B set message mask */
#define ISDN_RETMM               0xFA     /* H<- B return message mask */
#define ISDN_GETTSST             0xF9     /* H ->B get time slot status */
#define ISDN_RETTSST             0xF8     /* H<- B return time slot status */
#define ISDN_MAINTREQ            0xF7     /* H<->B change time slot maintenance
                                             state */
#define ISDN_MAINTREP            0xF6     /* H<- B report time slot maintenance
                                             state */
#define ISDN_UNBLOCK_TS          0xF5     /* H<->B putting DSL in normal
                                             operation state */
#define ISDN_BLOCK_TS            0xF4     /* H<->B putting DSL in Out of 
                                             Service state */    
#define ISDN_ERROR               0xF3     /* H<- B general error message */
#define ISDN_REQANI              0xF2     /* H ->B get ANI value */
#define ISDN_RETANI              0xF1     /* H<- B return ANI value */
#define ISDN_STATUS              0xF0     /* H<- B report D-channel status */
#define ISDN_TRACE               0xEF     /* H ->B enable trace collection */
#define ISDN_TRCINFO             0xEE     /* H<- B report trace info. */
#define ISDN_GETCALLST           0xED     /* H ->B get call state */
#define ISDN_RETCALLST           0xEC     /* H<- B return call state */
#define ISDN_SETBILLING          0xEB     /* H<->B vari-a-bill */
#define CALL_ALERT_ACK           0xEA     /* H<- B response for call alert
                                             message */
#define CALL_DEALLOC             0xE9     /* H ->B deallocate call info block 
                                             */
#define CALL_DEALLOC_ACK         0xE8     /* H<- B response for call dealloc
                                             message */
#define ISDN_CHINFST             0xE7     /* H<->B change interface status */
#define ISDN_GETPROT             0xE6     /* H ->B get protocol type */
#define ISDN_RETPROT             0xE5     /* H<- B return protocol type */
#define ISDN_SETMINDIG           0xE4     /* H<->B set minimal digit expect for
                                             called party number */
#define ISDN_MOREDIGIT           0xE3     /* H <->B get more digit for called 
                                             party number */
#define ISDN_ECHO                0xE2     /* H ->B echo driver's event as 
                                             requested */
#define CALL_TERM                0xE1     /* Fast Disconnect termination */
#define ISDN_RESTART             0xE0     /* H <-> B restart the channel */
#define ISDN_GETL2ST             0xDF     /* H -> B get layer 2 state */
#define ISDN_RETL2ST             0xDE     /* H <- B return layer 2 state */
#define ISDN_GETFRAME            0xDD     /* H <- B get layer 2 access frame */
#define ISDN_SNDFRAME            0xDC     /* H -> B send layer 2 access frame */
#define ISDN_QUEUE_CLEAR		 0x3C	  /* H <- B send HDLC Queue clear message  */
#define ISDN_QUEUE_WARNING		 0x3D	  /* H <- B send HDLC Queue warning message */
#define ISDN_QUEUE_OVERFLOW		 0x3E	  /* H <- B send HDLC Queue overflow message */
#define ISDN_CALL_REJECT         0x3F     /* H <- B Call has been rejected by the firmware */
#define ISDN_CALL_DIALING        0x40     /* H <- B This message will be sent to the host every time ISDN sends
                                             a CALL_OUTGOING to CC. */

#define CALL_HOLD                0xDB
#define CALL_HOLD_ACK            0xDA
#define CALL_HOLD_REJ            0xD9
#define CALL_RETRIEVE            0xD8
#define CALL_RET_ACK             0xD7
#define CALL_RET_REJ             0xD6
#define CALL_INTRUD              0xD5
#define CALL_DIVERT              0xD4
#define CALL_NSI                 0xD3
#define CALL_TRANSFER            0xD2
#define CALL_DIVERTED            0xD0
#define CALL_TRANSFERACK         0xCF
#define CALL_TRANSFERREJ         0xCE
#define CALL_TRANSIT             0xCD

/* HSS:FR385...7/30/2003.....Starts */
#define CALL_SU                  0x55
#define CALL_EEM                 0x56
#define CALL_SNU                 0x57
/* HSS:FR385...7/30/2003.....Ends */

/* 01/30/97 BRI Device Driver Interface    isdncmd.h */
#define ISDN_SHIFTUP8            0xCC     /* H ->B for debugging driver only */
#define ISDN_STRTDN              0xCB     /* H<->B start code execution */
#define ISDN_STOPBD              0xCA     /* H<->B stop board execution */

#define ISDN_GETFTR              0xC9     /* H<->B get feature list */
#define ISDN_GETGLOB             0xC8     /* H<->B read global parameter */
#define ISDN_SETGLOB             0xC7     /* H<->B set global parameter */
#define ISDN_BRDERR              0xC6     /* H<- B board level error message */
#define ISDN_TSERR               0xC5     /* H<- B timeslot level error message
*/
#define ISDN_SETID7F             0xC4     /* H<->B set idle to 7f */
#define ISDN_SETIDLE             0xC3     /* H<->B set idle pattern */
#define ISDN_SETIDFF             0xC2     /* H<->B set idle to ff */
#define ISDN_ASSIGNTTS           0xC1     /* H<->B assign local timeslog */
#define ISDN_RDASSIGNTTS         0xC0     /* H<->B read local timeslot assignment */
#define ISDN_UNASSIGNTTS         0xBF     /* H<->B unassign local timeslot */
#define ISDN_SETCLK              0xBE     /* H<->B set clock source */
#define ISDN_LISTEN              0xBD     /* H<->B listen to SCbus timeslot */
#define ISDN_UNLISTEN            0xBC     /* H<->B stop listening to SCbus */
#define ISDN_COMTST              0xBB     /* H<->B communication test to board */
#define ISDN_DATTST              0xBA     /* H<->B perform data test */
#define ISDN_GETBDST             0xB9     /* H<->B get board status */
#define ISDN_RDRAM               0xB8     /* H<->B read from ram */
#define ISDN_WRTRAM              0xB7     /* H<->B write to ram */
#define ISDN_GETDNIN             0xB6     /* H<->B get download information */
#define ISDN_GETDEVINFO          0xB5     /* H<->B get device info */

/* BRI Device Driver Interface  */
#define ISDN_SETDCHANCFG         0xB4     /* H<->B config & activate D channel,
                                                   response to configuration */
#define ISDN_TERMREG             0xB3     /* B->H  Terminal registration events */
#define ISDN_SNDTERMREG_ACK      0xB2     /* B<->H Provide network ack for term registration */

#define ISDN_TONEREDEFINE        0xB1     /* 05/08/97 Tone Generation isdncmd.h */
#define ISDN_TONEPLAY            0xB0
#define ISDN_TONESTOP            0xAF
#define ISDN_TONEDEFINECMPLT     0xAE
#define ISDN_TONEPLAYCMPLT       0xAD
#define ISDN_TONESTOPCMPLT       0xAC
#define ISDN_TIMEOUT             0xAB	 /* Handling tone gen. timeout
                                           conditions.*/

#define ISDN_GETNETCRV           0xAA   /* PRI: H ->B retrieve network crv to f/w */

#define ISDN_RCVTERMREG_ACK      0xA9   /* B->H convey reception of Term Reg. Ack */
#define ISDN_RCVTERMREG_NACK     0xA8   /* B->H convey reception of Term Reg Nack */
#define ISDN_CONFDROP            0xA7
#define ISDN_CONFDROPACK         0xA6
#define ISDN_CONFDROPREJ         0xA5
#define ISDN_REDIRECT            0xA4   /* B->H convey reception of ISDN REDIRECT message */

#define ISDN_SETL2ST             0xA3   /* H <->B set layer 2 state */
#define ISDN_SETL2CFG            0xA2   /* H <->B set layer 2 Config */
#define ISDN_GETL2CFG            0xA1   /* H <->B set layer 2 Config */
#define ISDN_REGISTER_ACK		 0xA2   /* H <- B ack for isdn_Register msg */

#define CHAN_RESTARTED           0x01   /* The channel was restarted */
#define CHAN_MAINTENANCE         0x02   /* The channel received a maint msg */
#define CHAN_ENQUIRY             0x03   /* Host requests channel status */
#define CHAN_RESULT              0x04   /* CC responds to CHANNEL_ENQUIRY */
#define CHAN_RESTART_REQ         0x05   /* Local restart request */
#define CHAN_RESTART_ERR         0x06   /* Locally originated restart failed */
#define CHAN_MAINT_REQ           0x07   /* Host requests chan maintenance */
#define ISDN_INCOMMING_SERVICE	 0x08	
                           
/*                              
 * argument id for ISDN_SETDEF, ISDN_GETDEF, and ISDN_RETDEF commands
 */
#define BEAR_CAP_FIELD3          0x0403
#define BEAR_CAP_FIELD4          0x0404
#define BEAR_CAP_FIELD5          0x0405
#define BEAR_CAP_FIELD6          0x0406
#define BEAR_CAP_FIELD7          0x0407
#define CAUSE_FIELD3             0x0803
#define CALLED_NUM_FIELD3        0x7003
#define CALLED_NUM_FIELD4        0x7004
#define CALLED_NUM_FIELD5        0x7005

/* HSS:S018-47 - Starts */
#define NFAS_CFG_INFO			 0x7006
/* HSS:S018-47 - Ends */

#define DIRECTORY_NUMBER         0x7015
#define SUBADDRESS_NUMBER        0x7115
#define CALLING_NUM_FIELD3       0x6C03
#define CALLING_NUM_FIELD4       0x6C04
#define CALLING_NUM_FIELD5       0x6C05
#define CALLING_NUM_FIELD6       0x6C06
#define CALLING_NUM_FIELD7       0x6C07
#define HIGH_LAYER_COMP_FIELD3   0x7D03
#define HIGH_LAYER_COMP_FIELD4   0x7D04
#define HIGH_LAYER_COMP_FIELD6   0x7D06
#define SPID_NUMBER              0x8000
 
/*
 * bit mask for ISDN_SETMM, ISDN_GETMM, and ISDN_RETMM commands
 */
#define ISDN_CALL_ALERT          0x01
#define ISDN_CALL_DISC           0x02
#define ISDN_CALL_PROC           0x04
#define ISDN_CALL_PROG           0x08
#define ISDN_LINK_STATUS         0x10
#define ISDN_CALL_SETUP_ACK      0x20
#define ISDN_RELEASE             0x40
#define ISDN_CALL_PROC_SEND      0x80

#define ISDN_INCOMMING_STATUS			0x100	
#define ISDN_INCOMMING_STATUS_ENQUIRY	0x200	
#define ISDN_INCOMMING_SERVICE_MSG		0x400	
#define ISDN_INCOMMING_SERVICE_ACK		0x800	
#define ISDN_TMREXPEVENT                0x1000  

#define ISDN_INCALLREJECT_ENABLE        0x2000 /*Mask to turn on CCEV_CALLREJECT event*/
#define ISDN_DIALING_ENABLE             0x4000 /*Mask to turn on CCEV_DIALING message from board to host */
#define ISDN_SERVICE_NOTIFICATION       0x8000 /*Mask to turn on CCEV_DIALING message from board to host */
 
   
/*                           
 * Layer 2 Link State
 */
#define ISDN_L2_DOWN             0     /* Layer 2 is down */
#define ISDN_L2_UP               1     /* Layer 2 is up */

/*
 * B-Channel service states.
 */

#define ISDN_IN_SERVICE          0     /* B-Channel in service */
#define ISDN_MAINTENANCE         1     /* B-Channel in maintenance busy */
#define ISDN_OUT_OF_SERVICE      2     /* B-Channel out of service */

#ifndef ISDN_OS2
#ifndef IN_SERVICE
#define IN_SERVICE      ISDN_IN_SERVICE      /* B-Channel in service */
#endif
#ifndef MAINTENANCE
#define MAINTENANCE     ISDN_MAINTENANCE     /* B-Channel in maintenance busy */
#endif
#ifndef OUT_OF_SERVICE
#define OUT_OF_SERVICE  ISDN_OUT_OF_SERVICE  /* B-Channel out of service */
#endif
#endif

/*
 * Message IDs for multi-byte messages between FW and Host.
 */
#define CALL_ACCEPT              0x01  /* Not used for new designs */
#define CALL_DISC                0x02  /* Incoming or outgoing call 
                                          disconnect */
#define CALL_INCOMING            0x03  /* Incoming call to the host */
#define CALL_OUTGOING            0x04  /* Outgoing call from the host */
#define CALL_PROGRESS            0x05  /* Call status change (either 
                                          direction) */
#define CALL_REJECTION           0x06  /* Incoming or outgoing call 
                                          rejected */
#define CALL_UUI                 0x07  /* UUI string in either direction */
#define CALL_FACILITY            0x08  /* Network facility indication */
#define CALL_FACILITY_ACK        0x09  /* Facility ACK */
#define CALL_FACILITY_REJ        0x0A  /* Facility REJ */
#define CALL_CONGESTION          0x0B  /* Call congestion report (UUI) */
#define CALL_NOTIFY              0x0C  /* Call notify to/from HOST */
#define CHAN_STATUS              0x0D  /* Channel status to/from HOST */
#define CALL_CLEARED             0x0E  /* Call_ID is cleared to HOST */
#define CALL_INFO                0x0F  /* Information message */
#define CALL_FACILITY_REGISTER   0x10  /* Facility Register */
#define CALL_REGISTER_ACK        0x11  /* Ack of Registration */
#define CALL_REGISTER_REJ        0x12  /* Rej of Registration */
#define CALL_FACILITY_CANCEL     0x13  /* Facility Cancel */
#define CALL_CANCEL_ACK          0x14  /* Cancel Ack */
#define CALL_CANCEL_REJ          0x15  /* Cancel Reject */
#define CALL_RELEASE             0x16  /* Release with UUI or Redirection */
#define STATUS_UPDATE            0x17  /* Update to/from HOST */
#define CALL_DETACH              0x18  /* Detach to/from HOST */
#define CALL_ALERT               0x20  /* Report ALERTING in either direction
                                           */
#define CALL_CONNECT             0x21  /* Report CONNECT in either direction
                                           */
#define CALL_SETUP_ACK           0x22  /* Report SETUP_ACK */
#define CALL_CONNECT_ACK         0x23  /* Report CONNECT_ACK */
#define CALL_PROCEEDING          0x24  /* Report CALL_PROCEEDING */
#define CALL_DEACTIVATE_REQ      0x25  /* TPH Semi-permanent DEACTIVATE_REQ */
#define CALL_REACTIVATE_REQ      0x26  /* TPH Semi-permanent REACTIVATE_REQ */
#define CALL_DEACTIVATE_CONF     0x27  /* TPH DEACTIVATE_CONF */
#define CALL_REACTIVATE_CONF     0x28  /* TPH REACTIVATE_CONF */
#define CALL_DEACTIVATE_ERROR    0x29  /* TPH DEACTIVATE_ERROR */
#define CALL_REACTIVATE_ERROR    0x2A  /* TPH REACTIVATE_ERROR */
#define CALL_STATUS              0x2B  /* Report a STATUS message */
#define CALL_ERROR               0x2C  /* Report internal error */
#define ISDN_RETNETCRV           0x2D  /* Retrieve network call ref value to f/w */
#define CALL_STATUS_RESET        0x2E  /* STATUS message with NULL CALL  */         
#define CALL_STATUS_ENQ			 0x2E  /*send out a status enquiry message*/
#define ISDN_L3TIMEOUT           0x2F   
#define ISDN_OUTGOING_SERVICE_ACK	0x30	/*for service ack*/
#define ISDN_REGISTER			 0x32	

/* HSS:FR-418 - start */
#define CALL_WAITING_INCOMING    0x33  
#define CALL_WAITING_ACCEPT      0x34
/* HSS:FR-418 - stop */
                                
/*
 * Coding Standard
 */
#define ISDN_CODINGSTD_CCITT     0x00  /* CCITT Coding Standard */
#define ISDN_CODINGSTD_INTL      0x01  /* International Coding Standard */

/*
 * Information Transfer Capability
 */
#define BEAR_CAP_SPEECH          0x00  /* Speech Bearer Capability */
#define BEAR_CAP_UNREST_DIG      0x08  /* Unrestricted Digital Capability */
#define BEAR_CAP_REST_DIG        0x09  /* Restricted Digital Capability */
#define BEAR_CAP_3DOT1K_AUDIO    0x10  /* 3.1KHz Audio Capability */
#define BEAR_CAP_7K_AUDIO        0x11  /* 7KHz Audio Capability */
#define BEAR_CAP_VIDEO           0x18  /* Video Capability */

/*
 * Information Transfer Mode
 */
#define ISDN_ITM_CIRCUIT         0x00  /* Circuit mode */
#define ISDN_ITM_PACKET          0x02  /* Packet mode */

/*                                 
 * Information Transfer Rate
 */
#define BEAR_RATE_64KBPS         0x10  /* B_CHANNEL_UNITS 1X64 */
#define BEAR_RATE_128KBPS        0x11  /* Non-standard 2X64       */
#define BEAR_RATE_384KBPS        0x13  /* H0_CHANNEL_UNITS 6X64   */
#define BEAR_RATE_1536KBPS       0x15  /* H11_CHANNEL_UNITS 24X64 */
#define BEAR_RATE_1920KBPS       0x17  /* H12_CHANNEL_UNITS 30X64 */
#define PACKET_TRANSFER_MODE	 0x20 

/*
 *    Bearer Capability Element
 */
#define ISDN_UIL1_CCITTV110      0x01  /* user info layer 1 - CCITT V.110/X.3
                                          */
#define ISDN_UIL1_G711ULAW       0x02  /* user info layer 1 - G.711 u-law */
#define ISDN_UIL1_G711ALAW       0x03  /* user info layer 1 - G.711 A-law */
#define ISDN_UIL1_G721ADCPM      0x04  /* user info layer 1 - G.721 ADCPM */
#define ISDN_UIL1_G722G725       0x05  /* user info layer 1 - G.722 and G.725 */
#define ISDN_UIL1_G722F725       0x05  /* user info layer 1 - G.722 and G.725
                                          */
#define ISDN_UIL1_H261           0x06  /* user info layer 1 - H.261 */
#define ISDN_UIL1_NONCCITT       0x07  /* user info layer 1 - non-CCITT */
#define ISDN_UIL1_CCITTV120      0x08  /* user info layer 1 - CCITT V.120 */
#define ISDN_UIL1_CCITTX31       0x09  /* user info layer 1 - CCITT X.31 */
#define ISDN_UIL1_DEFAULT        0xFF  /* user info layer 1 - Default Value 
                                          */

#define ISDN_UR_EINI460          0x00  /* user rate - E bits in I.460 */
#define ISDN_UR_600              0x01  /* user rate - 0.6 kbits */
#define ISDN_UR_1200             0x02  /* user rate - 1.2 kbits */
#define ISDN_UR_2400             0x03  /* user rate - 2.4 kbits */
#define ISDN_UR_3600             0x04  /* user rate - 3.6 kbits */
#define ISDN_UR_4800             0x05  /* user rate - 4.8 kbits */
#define ISDN_UR_7200             0x06  /* user rate - 7.2 kbits */
#define ISDN_UR_8000             0x07  /* user rate - 8.0 kbits */
#define ISDN_UR_9600             0x08  /* user rate - 9.6 kbits */
#define ISDN_UR_14400            0x09  /* user rate - 14.4 kbits */
#define ISDN_UR_16000            0x0A  /* user rate - 16.0 kbits */
#define ISDN_UR_19200            0x0B  /* user rate - 19.2 kbits */
#define ISDN_UR_32000            0x0C  /* user rate - 32 kbits */
#define ISDN_UR_48000            0x0E  /* user rate - 48 kbits */
#define ISDN_UR_56000            0x0F  /* user rate - 56 kbits */
#define ISDN_UR_64000            0x10  /* user rate - 64 kbits */
#define ISDN_UR_134              0x15  /* user rate - .1345 kbits */
#define ISDN_UR_100              0x16  /* user rate - .100 kbits */
#define ISDN_UR_75_1200          0x17  /* user rate - .075/1200 kbits */
#define ISDN_UR_1200_75          0x18  /* user rate - 1200/.075 kbits */
#define ISDN_UR_50               0x19  /* user rate - .050 kbits */
#define ISDN_UR_75               0x1A  /* user rate - .075 kbits */
#define ISDN_UR_110              0x1B  /* user rate - .110 kbits */
#define ISDN_UR_150              0x1C  /* user rate - .150 kbits */
#define ISDN_UR_200              0x1D  /* user rate - .200 kbits */
#define ISDN_UR_300              0x1E  /* user rate - .300 kbits */
#define ISDN_UR_12000            0x1F  /* user rate - 12 kbits */
#define ISDN_UR_DEFAULT          0xFF  /* user rate - Default */
                                 
/*
 * CALLER/CALLED_NUMBER_IE 
 */

/* defines for CLC (these take the place of 'type of number' in Called/Calling IEs: - 627a206 */

#define	DPNSS_CLC_UNKNOWN	0x00
#define	DPNSS_CLC_ORD	0x01
#define	DPNSS_CLC_DEC	0x02
#define	DPNSS_CLC_ISDN	0x03
#define	DPNSS_CLC_PSTN	0x04
#define	DPNSS_CLC_MF5	0x05
#define	DPNSS_CLC_OP	0x06
#define	DPNSS_CLC_NET	0x07

/*
 * CALLER/CALLED_NUMBER_IE 
 */

/*
 * Number types
 */
#define EN_BLOC_NUMBER           0x00  /* Number is sent en-bloc */
#define INTL_NUMBER              0x01  /* International number */
#define NAT_NUMBER               0x02  /* National number */
#define LOC_NUMBER               0x04  /* Loacl (directory) number */
#define OVERLAP_NUMBER           0x05  /* Number is sent overlap */

/*
 * Numbering plans
 */
#define UNKNOWN_NUMB_PLAN        0x00  /* Unknown plan */
#define ISDN_NUMB_PLAN           0x01  /* ISDN numb. plan E.164 */
#define TELEPHONY_NUMB_PLAN      0x02  /* Telephony numb. plan E.163 */
#define PRIVATE_NUMB_PLAN        0x09  /* Private numbering plan */


/*
 * CALLER_SUB_ADDR_IE and CALLED_SUB_ADDR_IE
 */
#define OSI_SUB_ADDR             0x00  /* OSI Sub-address */
#define USER_SPECIFIC_SUB_ADDR   0x02  /* User-specific Sub-address */
#define IA_5_FORMAT              0x50  /* IA 5 sub-address digit format */
#ifndef MAX_SUB_ADDR_LEN
#define MAX_SUB_ADDR_LEN         23    /* Japanese Len > CCITT default of 23 */
#endif

/*
 * channel ID defines 
 */
#define NO_BCHAN      0    /* No B-channel */
#define ANY_BCHAN     0x7E /* Any B-channel */
#define DCHAN_IND     0x7F /* Non circuit switched */
#define B1            0x01 /* Bearer channel 1 */
#define B2            0x02 /* Bearer channel 2 */

#define PREFERRED     0x00 /* B-channel preferred */
#define EXCLUSIVE     0x01 /* B-channel exclusive */


#endif

