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
 * Filename:    isdnerr.h 
 * DESCRIPTION: This file contains the error codes for the ISDN library..   
 *****************************************************************************/

#ifndef isdnerr_h_included
#define isnderr_h_included

#include "isdnos.h"

/* 
 * Bases for the various error codes
 */
#define ERR_ISDN_FW            0x100
#define ERR_ISDN_CAUSE         0x200
#define ERR_ISDN               0x300
#define ERR_ISDN_LIB           ERR_ISDN
#define ERR_ISDN_DRV           ERR_ISDN
#define ERR_ISDN_FMT           0x500		// new error code for formatter KP

/*
 ******************* Message passing error codes ***************************
 */
#define E_ISSUCC           0x00   /* Message acknowledged. */

#define E_ISREADY          0x01  /* Board not ready */
#define E_ISCONFIG         0x02  /* Configuration error */
#define E_ISNOINFO         0x03  /* information not available */
#define E_ISNOUSRINFOBUF   0x04  /* User information element buffer is not
                                    ready */
#define E_ISNOFACILITYBUF  0x05  /* Facility buffer is not ready */
#define E_ISBADBUFADDR     0x06  /* Bad buffer address */
#define E_ISBADTS          0x07  /* bad time slot */
#define E_ISOVERWRITE      0x08  /* over writen error */


#define  E_ISGLOBAL        0x81  /* global event */
#define  E_ISCALLRELATED   0x82  /* call related event */
#define  E_ISLINERELATED   0x83  /* line related event */
#define  E_ISNOSPRT        0x84  /* Function not supported on device */
#define  E_ISMAXLEN        0x85  /* exceed maximum lenght allowed */
#define  E_ISNULLPTR       0x86  /* null pointer error */
#define  E_ISNOMEM         0x87  /* out of memory */
#define  E_ISFILEOPENFAIL  0x88  /* failed to open a file, application can 
                                    check DOS 'errno' to get more 
                                    informations */
#define  E_ISTNACT         0x89  /* trace information is not activated */
#define  E_ISBADPAR        0x8A  /* bad input parameter */
#define  E_SETTWICE        0x8B  /* SetParm(RECEIVE_INFO_BUF) twice */
#define  E_ISMISGCI        0x8C  /* miss using GetCallInfo */
#define  E_ISMISGSI        0x8D  /* miss using GetSigInfo */
#define  E_ISLSIEBUF       0x8E  /* Data may be overwrite */

#ifdef ISDN_DOS
#define  E_ISBADLINEDEV    0xC1  /* bad LINEDEV - dos only */ 
#else
#define  E_ISBADCALLID     0xC1  /* bad callid - unix and os2 only */
#endif
#define  E_ISBADCRN        0xC2  /* bad CRN */
#define  E_ISNOINFOBUF     0xC3  /* information buffer not ready */
#define  E_ISINVNETWORK    0xC4  /* invalid network type */
#define  E_LINKDOWN        0xC5  /* data link is down */
#define  E_LINKUP          0xC6  /* data link is up */
#define  E_LINKRESET       0xC7  /* data link reset */
#define  E_FB_UNAVAIL      0xC8  /* flex billing unavailable */
#define  E_ISBADIF         0xC9  /* bad interface number */
#define  E_TRACEFAIL       0xCA  /* failed to get trace information */
#define  E_UNKNOWNRESULT   0xCB  /* Unknown result code */
#define  E_ISCRNBLK        0xCC  /* failed to assigned CRNBLK */
#define  E_BADSTATE        0xCD  /* bad state */
#define  E_ABORTED         0xCE  /* task aborted */
#define  E_LINKDISABLED    0xCF  /* data link disabled */


/* additional error codes for the ISDN DNA library - JMB */

#define	E_INVNDIINTERFACE 0xCF  /* invalid NDI interface */
#define	E_INVNRB				0xD0	/* invalid NRB */
#define	E_NOERROR			0xD1  /* No Errors */
#define	E_TIMEOUT			0xD2  /* Function Timed Out */
#define	E_BADDEV				0xD3  /* Invalid Device Descriptor */
#define	E_ISNOCALLSLOT		0xD4  /* Out of CallInfo slots in LineDeviceInfo */
#define  E_INVNTFMSG			0xD5  /* Invalid message from the formatter */
#define  E_SRLSEND			0xD6  /* Error occurred sending SRL command */
#define  E_EVENT				0xD7	/*	Specified error event occurred */

#define	E_DRVERROR			0xD8	/* Error returned by driver */

/* Error code added for callid management. */

#define E_NOCALLIDAVAIL		0xD9	/* No callid is available for use */

/*
 * Error codes 
 */
#define  ISDN_OK                 0
#define  ISDN_BADDSL             1
#define  ISDN_BADTS              2
#define  ISDN_BADARG             3
#define  ISDN_BADLEN             4
#define  ISDN_BADSTR             5
#define  ISDN_BADIF              6
#define  ISDN_MISSIE             7
#define  ISDN_CFGERR             8
#define  ISDN_TELNT              9
#define  ISDN_CHRST_ERR          10
#define  ISDN_BADCOMP            11    /* bad component */
#define  ISDN_BADARGU            12    /* bad argument */
#define  ISDN_BADSERVICE         13    /* bad service */
#define  ISDN_BADCALLID          14    /* bad call id */
#define  ISDN_BADSTATE           15    /* can not accept event in current 
                                          state */
#define  ISDN_BADSS              16    /* bad serveice state */
#define  ISDN_TSBUSY             17    /* time slot already in use */
#define  ISDN_NOAVAIL            18    /* no more avail info record */
#define  ISDN_LINKFAIL           19    /* Data link don't come up */
#define  ISDN_FB_AVAIL           ISDN_LINKFAIL+FLEX_BILLING_AVAILABLE      
#define  ISDN_FB_UNAVAIL         ISDN_LINKFAIL+FLEX_BILLING_UNAVAILABLE    
#define  ISDN_FB_ACCEPTED        ISDN_LINKFAIL+FLEX_BILLING_ACCEPTED       
#define  ISDN_FB_BAD_OPER        ISDN_LINKFAIL+FLEX_BILLING_BAD_OPERATION  
#define  ISDN_FB_BAD_ARG         ISDN_LINKFAIL+FLEX_BILLING_BAD_ARGUMENT   
#define  ISDN_FB_RET_ERR         ISDN_LINKFAIL+FLEX_BILLING_RETURN_ERR     
#define  ISDN_FB_IE_ERR          ISDN_LINKFAIL+FLEX_BILLING_IE_ERR
#define  ISDN_NO_FB_INFO         ISDN_LINKFAIL+NO_FLEX_BILLING_INFO
#define  ISDN_NO_MSG             30   /* don't send any message to Network 
                                          or Host */
#define  ISDN_BADMSG             31   /* Bad message in service/feature 
                                          provided */
#define  ISDN_INVALID_EVENT      32   /* invalid event for the switch */
#define  ISDN_GLARE              33   /* Glare condition has occured */
#define  ISDN_BADIE_BEARCAP      34   /* bad transfer rate in Bearer Cap */
#define  ISDN_BADDLID            35   /* Bad data link id */
#define  ISDN_SYSTEM_ERR         36   /* System related error 
                                         (i.e., alloc failure) */
#define  ISDN_INVALID_SWITCH_TYPE 37  /* Invalid switch type specified
                                         in configuration */
#define  ISDN_MISSING_FIXED_TEI  38   /* Fixed TEI value not provided for initializing terminal */
#define  ISDN_MISSING_DN         39   /* SPID not provided for North American terminal */
#define  ISDN_MISSING_SPID       40   /* SPID not provided for North American termina */ 
/* tone generation errors */
#define  ISDN_BADTONEMSG         41   /* Invalid tone message */
#define  ISDN_TONEBUSY           42   /* Channel is busy playing a tone */                          
#define  ISDN_ERRDSP             44   /* DSP is not responding */
#define  ISDN_BADSTOP            54   /* Channel is already stopped */

/*
 * Cause values
 *
 * NOTE: Not all cause values are universally supported across switch types.
 *       Before a particular cause value is used, its validity should be
 *       compared with the appropriate switch vendor specs.
 */
#define UNASSIGNED_NUMBER        0x01  /* Cause 01 */
#define NO_ROUTE                 0x02  /* Cause 02 */
#define NO_ROUTE_TO_DEST			0x03	/* Cause 03 */
#define SEND_SPECIAL_ITONE			0x04	/* Cause 04 */
#define MISDIALLED_TRUNK_PFX		0x05	/* Cause 05 */
#define CHANNEL_UNACCEPTABLE     0x06  /* Cause 06 */
#define CALL_AWARDED_IN_EST_CHAN 0x07  /* Cause 07 */
#define PREEMPTION               0x08  /* Cause 08 */
#define PREEMPTION_CHAN_RESERVED 0x09  /* Cause 09 */
#define QOR_PORTED_NUMBER        0x0E  /* Cause 14 */
#define NORMAL_CLEARING          0x10  /* Cause 16 */
#define USER_BUSY                0x11  /* Cause 17 */
#define NO_USER_RESPONDING       0x12  /* Cause 18 */
#define NO_ANSWER_FROM_USER      0x13  /* Cause 19 */
#define SUBSCRIBER_ABSENT	      0x14  /* Cause 20 */
#define CALL_REJECTED            0x15  /* Cause 21 */
#define NUMBER_CHANGED           0x16  /* Cause 22 */
#define REDIR_NEW_DESTINATION    0x17  /* Cause 23 */
#define REJECTED_FEATURE_DEST    0x18  /* Cause 24 */
#define EXCHANGE_ROUTING_ERR     0x19  /* Cause 25 */
#define NON_SELECTED_USR_CLEAR   0x1A  /* Cause 26 */
#define DEST_OUT_OF_ORDER        0x1B  /* Cause 27 */
#define INVALID_NUMBER_FORMAT    0x1C  /* Cause 28 */
#define FACILITY_REJECTED        0x1D  /* Cause 29 */
#define RESP_TO_STAT_ENQ         0x1E  /* Cause 30 */
#define UNSPECIFIED_CAUSE        0x1F  /* Cause 31 */
#define NO_CIRCUIT_AVAILABLE     0x22  /* Cause 34 */
#define NETWORK_OUT_OF_ORDER     0x26  /* Cause 38 */
#define PFM_CONN_OUT_OF_SERVICE  0x27  /* Cause 39 */
#define PFM_CONN_OPERATIONAL     0x28  /* Cause 40 */
#define TEMPORARY_FAILURE        0x29  /* Cause 41 */
#define NETWORK_CONGESTION       0x2A  /* Cause 42 */
#define ACCESS_INFO_DISCARDED    0x2B  /* Cause 43 */
#define REQ_CHANNEL_NOT_AVAIL    0x2C  /* Cause 44 */
#define PRE_EMPTED               0x2D  /* Cause 45 */
#define CALL_BLOCKED             0x2E  /* Cause 46 */
#define RESOURCE_UNAVAILABLE     0x2F  /* Cause 47 */
#define QOS_UNAVAILABLE          0x31  /* Cause 49 */
#define FACILITY_NOT_SUBSCRIBED  0x32  /* Cause 50 */
#define BEARER_INCOMPAT_SERVICE  0x33  /* Cause 51 */
#define OUTGOING_CALL_BARRED     0x34  /* Cause 52 */
#define OUTGOING_BARRED_IN_CUG   0x37  /* Cause 53 */ 
#define INCOMING_CALL_BARRED     0x36  /* Cause 54 */
#define INCOMING_BARRED_IN_CUG  	0x37  /* Cause 55 */ 
#define BEAR_CAP_NOT_AUTHL       0x39  /* Cause 57 */
#define BEAR_CAP_NOT_AVAIL       0x3A  /* Cause 58 */
#define INCONSISTANT_OUTB_INFO   0x3E  /* Cause 62 */
#define SERVICE_NOT_AVAIL        0x3F  /* Cause 63 */
#define CAP_NOT_IMPLEMENTED      0x41  /* Cause 65 */
#define CHAN_NOT_IMPLEMENTED     0x42  /* Cause 66 */
#define FACILITY_NOT_IMPLEMENT   0x45  /* Cause 69 */
#define RESTRICTED_DIG_INFO_ONLY 0x46  /* Cause 70 */
#define SERVICE_NOT_IMPLEMENTED  0x4F  /* Cause 79 */
#define INVALID_CALL_REF         0x51  /* Cause 81 */
#define CHAN_DOES_NOT_EXIST      0x52  /* Cause 82 */
#define BAD_CALL_ID_FOR_SUS_CALL 0x53  /* Cause 83 */
#define CALL_ID_NOT_IN_USE       0x54  /* Cause 84 */
#define NO_SUSPENDED_CALL        0x55  /* Cause 85 */
#define CALL_ID_CLEARED          0x56  /* Cause 86 */
#define NOT_MEMBER_OF_CUG        0x57  /* Cause 87 */
#define INCOMPATIBLE_DEST        0x58  /* Cause 88 */
#define NONEXISTENT_CUG          0x5A  /* Cause 90 */
#define INVALID_TRANS_NETWORK    0x5B  /* Cause 91 */
#define INVALID_MSG_UNSPEC       0x5F  /* Cause 95 */
#define MANDATORY_IE_MISSING     0x60  /* Cause 96 */
#define NONEXISTENT_MSG          0x61  /* Cause 97 */
#define WRONG_MESSAGE            0x62  /* Cause 98 */
#define BAD_INFO_ELEM            0x63  /* Cause 99 */
#define INVALID_ELEM_CONTENTS    0x64  /* Cause 100 */
#define WRONG_MSG_FOR_STATE      0x65  /* Cause 101 */
#define TIMER_EXPIRY             0x66  /* Cause 102 */
#define MANDATORY_IE_LEN_ERR     0x67  /* Cause 103 */
#define MSG_W_UNRECOGNIZED_PARAM 0x6E  /* Cause 110 */
#define PROTOCOL_ERROR           0x6F  /* Cause 111 */
#define INTERWORKING_UNSPEC      0x7F  /* Cause 127 */

/* 
   The Cause value greater than 127 is allowed to define here. There is
   no conflict with those universal defines which ranges 0 - 127 
*/
#define RESTART_ON_NON_PRIMARY 0x80                         /* Cause 128 */

#endif
