/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2001-2007 Dialogic Corporation. All Rights Reserved.
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
 *	C Header:		cc_s7.h
 *	Instance:		hsw_2
 *	Description:	GlobalCall SS7 header file for application use
 *	%date_created:	Tue Apr 11 12:05:58 2006 %
 *
 **********************************************************************/

#ifndef _hsw_2_cc_s7_h_H
#define _hsw_2_cc_s7_h_H


/*
 * MAKECALL_BLK
 */
#define MAXPHONENUM     32

typedef union {
  struct ss7 {

    unsigned char trans_medium_req;
    /*
       TMR_SPEECH
       TMR_64K_UNREST
       TMR_3DOT1K_AUDIO
       TMR_64K_PREFERRED
       TMR_2_64K_UNREST
       TMR_386K_UNREST
       TMR_1536K_UNREST
       TMR_1920K_UNREST
       TMR_3_64K_UNREST
       TMR_4_64K_UNREST
       TMR_5_64K_UNREST
       TMR_7_64K_UNREST
       TMR_8_64K_UNREST
       TMR_9_64K_UNREST
       ...
       TMR_23_64K_UNREST
       TMR_25_64K_UNREST
       ...
       TMR_29_64K_UNREST
     */

    unsigned char destination_number_type;
    /* 
       SS7_UNKNOWN_NUMB_TYPE    - spare
       SS7_SUBSCRIBER_NUMBER    - Subscriber number (national use)
       SS7_UNKNOWN_NATIONAL     - Unknown (national use)
       SS7_NATIONAL_NUMBER      - National (significant) number
       SS7_INTERNATIONAL_NUMBER - International number
       SS7_NETWORK_SPECIFIC     - Network-specific number (national use)
     */

    unsigned char destination_number_plan;
    /* 
       SS7_UNKNOWN_NUMB_PLAN    - Unknown plan
       SS7_ISDN_NUMB_PLAN       - ISDN numb. plan E.164
       SS7_DATA_NUMB_PLAN       - Data numb. plan X.121
       SS7_TELEX_NUMB_PLAN      - Telex numb. plan F.69
     */

    unsigned char internal_network_number;
    /* 
       INN_ALLOWED              - routing to internal network allowed
       INN_NOT_ALLOWED          - routing to internal network not allowed
     */

    unsigned char origination_number_type;
    /* 
       SS7_UNKNOWN_NUMB_TYPE    - spare
       SS7_SUBSCRIBER_NUMBER    - Subscriber number (national use)
       SS7_UNKNOWN_NATIONAL     - Unknown (national use)
       SS7_NATIONAL_NUMBER      - National (significant) number
       SS7_INTERNATIONAL_NUMBER - International number
       SS7_NETWORK_SPECIFIC     - Network-specific number (national use)
     */

    unsigned char origination_number_plan;
    /* 
       SS7_UNKNOWN_NUMB_PLAN    - Unknown plan
       SS7_ISDN_NUMB_PLAN       - ISDN numb. plan E.164
       SS7_DATA_NUMB_PLAN       - Data numb. plan X.121
       SS7_TELEX_NUMB_PLAN      - Telex numb. plan F.69
     */

    char origination_phone_number[MAXPHONENUM];

    unsigned char origination_present_restrict;
    /*
       PRESENTATION_ALLOWED
       PRESENTATION_RESTRICTED
       PRESENTATION_NOT_AVAILABLE
     */

    unsigned char origination_screening;
    /*
       SCREEN_USER_PROVIDED
       SCREEN_USER_PROVIDED_VERIFIED
       SCREEN_USER_PROVIDED_FAILED
       SCREEN_NETWORK_PROVIDED
     */

    unsigned short calling_party_category;
    /*
       SS7_UNKNOWN_CATEGORY
       SS7_FR_OPERATOR_CATEGORY
       SS7_EN_OPERATOR_CATEGORY
       SS7_GE_OPERATOR_CATEGORY
       SS7_RU_OPERATOR_CATEGORY
       SS7_SP_OPERATOR_CATEGORY
       SS7_RESERVED_CATEGORY
       SS7_ORDINARY_SUBS_CATEGORY
       SS7_PRIORITY_SUBS_CATEGORY
       SS7_DATA_CATEGORY
       SS7_TEST_CATEGORY
       SS7_PAYPHONE_CATEGORY
     */

    unsigned short forward_call_indicators;
    /* bitmask - see defines below */

    void *usrinfo_bufp;            /* RFU */

	 unsigned char satellite_indicator;
    /*
       SI_NOSATELLITES
       SI_1SATELLITE
       SI_2SATELLITES
    */

	 unsigned char echo_device_indicator;
    /*
       EDI_ECHOCANCEL_NOTINCLUDED
       EDI_ECHOCANCEL_INCLUDED
    */

	 unsigned char continuity_check_indicator;
    /*
       CCI_CC_NOTREQUIRED
       CCI_CC_REQUIRED
       CCI_CC_ONPREVIOUS
    */

	 unsigned char user_to_user_indicators;
	 /*
       UUI_UUS1_REQ_NE - Service 1; request, not essential
       UUI_UUS1_REQ_E - Service 1; request, essential
       UUI_UUS2_REQ_NE - Service 2; request, not essential
       UUI_UUS2_REQ_E - Service 2; request, essential
       UUI_UUS3_REQ_NE - Service 3; request, not essential
       UUI_UUS3_REQ_E - Service 3; request, essential
       UUI_UUS1_RSP_P - Service 1: response, provided
       UUI_UUS2_RSP_P - Service 2: response, provided
       UUI_UUS3_RSP_P - Service 3: response, provided
       UUI_UUSx_RSP_P - Services 1, 2 and 3; response, provided
	 */

    long rfu[5];                   /* RFU */

  } ss7;
} S7_MAKECALL_BLK,  *S7_MAKECALL_BLK_PTR;


/*
 * WAITCALL_BLK
 */
typedef union {
  struct {
    void *usrinfo_buf;      /* RFU */
    long rfu[2];            /* RFU */
  } ss7;
} S7_WAITCALL_BLK,  *S7_WAITCALL_BLK_PTR;


/*
 * S7_IE and S7_IE_BLK
 */
typedef struct {
   unsigned char parm;     /* Parameter type */
   unsigned char length;   /* Number of bytes in the value part */
   unsigned char value;    /* First byte of the value part (there may be more) */
} S7_IE;

#define S7_MAXLEN_IEDATA            320
typedef struct {
   short  length;                   /* Number of bytes in the data part; must be less than S7_MAXLEN_IEDATA  */
   char   data[S7_MAXLEN_IEDATA];   /* The IEs (message parameters)        */
} S7_IE_BLK, *S7_IE_BLK_PTR;

/*
 * S7_SIGINFO_BLK
 */
typedef struct {
   short  length;           /* length of SigInfo block minus sizeof(length) */
   unsigned char prim;      /* ISUP primitive */
   S7_IE  data;					/* First IE of the message (there may be more) */
} S7_SIGINFO_BLK, *S7_SIGINFO_BLK_PTR;

/*
 * Type definition for SCBus Timeslots information structure.
 */
#ifndef __SC_TSINFO__
#define __SC_TSINFO__
typedef struct sc_tsinfo {
   unsigned long  sc_numts;
   long          *sc_tsarrayp;
} SC_TSINFO;
#endif


/* used with gc_SetParm, see gclib.h for the rest of GCPR_ defines */
#define GCPR_UNKNOWN_ISUP_MSGS  0x701
#define GCPR_IGNORE_BCI         0x702
#define GCPR_REPORT_ALARMS      0x703

/*
 * Possible values for GCPR_REPORT_ALARMS, applies only to Diva media boards
 */
#define SS7_REPORT_ALARMS_OFF		0
#define SS7_REPORT_ALARMS_TO_REMOTE	1
#define SS7_REPORT_ALARMS_TO_ALL	2


/*
 * Libgcs7 Specific CallInfo
 */
#define REDIRECTING_NUMBER       0x200


/*
 * SS7 CCLib parameters for GlobalCall Run-Time-Configuration-Manager (RTCM)
 */
#define SS7_BASE_SETID           GC_SS7LIB_SET_MIN       /* 0x4500h */

/*** Circuit Group Parameter Set ***/
#define S7SET_CCTGRP             (SS7_BASE_SETID+1)
#define S7PARM_CCTGRP_STATE      1
/*** Continuity check Parameter Set ***/
#define S7SET_CONTCHECK          (SS7_BASE_SETID+2)
#define S7PARM_CONTCHECK_RESULT  1
#define S7PARM_CONTCHECK_TYPE    2
/*** Unrecognized ISUP events Parameter Set ***/
#define S7SET_ISUP_EVENT         (SS7_BASE_SETID+3)
#define S7PARM_ISUP_EVENT_PARM   1
/*** Channel L4 Configuration ***/
#define S7SET_CHAN_L4_CFG        (SS7_BASE_SETID+4)
#define S7PARM_CHAN_CIC          1
#define S7PARM_CHAN_GROUP_ID     2
/*** Alarm indication ***/
#define S7SET_ALARMS					(SS7_BASE_SETID+5)
#define S7PARM_ALARM_ON				1
#define S7PARM_ALARM_OFF			2


/*
 * Circuit Group Set State data block and values
 */
typedef struct _s7parm_cctgrp_state_data
{
	int				group_id;
	unsigned int	cic_mask;
	int				state;
} S7PARM_CCTGRP_STATE_DATA;
/* Possible values for the state field */
#define S7_CCTGRP_STATE_MAINT_BLOCK    0x00
#define S7_CCTGRP_STATE_HW_BLOCK       0x01
#define S7_CCTGRP_STATE_MAINT_UNBLOCK  0x20
#define S7_CCTGRP_STATE_HW_UNBLOCK     0x21
#define S7_CCTGRP_STATE_RESET          0xf0


/*
 * Satellite indicator
 */
#define SI_NOSATELLITES             0x00
#define SI_1SATELLITE               0x01
#define SI_2SATELLITES              0x02

/*
 * Echo device indicator
 */
#define EDI_ECHOCANCEL_NOTINCLUDED  0x00
#define EDI_ECHOCANCEL_INCLUDED     0x10

/*
 * Continuity check indicator
 */
#define CCI_CC_NOTREQUIRED          0x00
#define CCI_CC_REQUIRED             0x04
#define CCI_CC_ONPREVIOUS           0x08

/*
 * Transmission Medium Requirement (TMR - Q.763)
 */
#define TMR_SPEECH             0x00
#define TMR_64K_UNREST         0x02
#define TMR_3DOT1K_AUDIO       0x03
#define TMR_64K_PREFERRED      0x06
#define TMR_2_64K_UNREST       0x07
#define TMR_384K_UNREST        0x08
#define TMR_1536K_UNREST       0x09
#define TMR_1920K_UNREST       0x0A
#define TMR_3_64K_UNREST       0x10
#define TMR_4_64K_UNREST       0x11
#define TMR_5_64K_UNREST       0x12
#define TMR_7_64K_UNREST       0x14
#define TMR_8_64K_UNREST       0x15
#define TMR_9_64K_UNREST       0x16
#define TMR_10_64K_UNREST      0x17
#define TMR_11_64K_UNREST      0x18
#define TMR_12_64K_UNREST      0x19
#define TMR_13_64K_UNREST      0x1A
#define TMR_14_64K_UNREST      0x1B
#define TMR_15_64K_UNREST      0x1C
#define TMR_16_64K_UNREST      0x1D
#define TMR_17_64K_UNREST      0x1E
#define TMR_18_64K_UNREST      0x1F
#define TMR_19_64K_UNREST      0x20
#define TMR_20_64K_UNREST      0x21
#define TMR_21_64K_UNREST      0x22
#define TMR_22_64K_UNREST      0x23
#define TMR_23_64K_UNREST      0x24
#define TMR_25_64K_UNREST      0x26
#define TMR_26_64K_UNREST      0x27
#define TMR_27_64K_UNREST      0x28
#define TMR_28_64K_UNREST      0x29
#define TMR_29_64K_UNREST      0x2A
                                 
/*
 * Number types
 */
#define SS7_UNKNOWN_NUMB_TYPE    0x00  /* spare */
#define SS7_SUBSCRIBER_NUMBER    0x01  /* Subscriber number (national use) */
#define SS7_UNKNOWN_NATIONAL     0x02  /* Unknown (national use) */
#define SS7_NATIONAL_NUMBER      0x03  /* National (significant) number */
#define SS7_INTERNATIONAL_NUMBER 0x04  /* International number */
#define SS7_NETWORK_SPECIFIC     0x05  /* Network-specific number (national use) */

/*
 * Numbering plans
 */
#define SS7_UNKNOWN_NUMB_PLAN    0x00  /* Unknown plan */
#define SS7_ISDN_NUMB_PLAN       0x01  /* ISDN numb. plan E.164 */
#define SS7_DATA_NUMB_PLAN       0x03  /* Data numb. plan X.121 */
#define SS7_TELEX_NUMB_PLAN      0x04  /* Telex numb. plan F.69 */

/*
 * Calling Party's Category
 */
#define SS7_UNKNOWN_CATEGORY            0x00  /* Unknown at this time */
#define SS7_FR_OPERATOR_CATEGORY        0x01  /* Operator, language French */
#define SS7_EN_OPERATOR_CATEGORY        0x02  /* Operator, language English */
#define SS7_GE_OPERATOR_CATEGORY        0x03  /* Operator, language German */
#define SS7_RU_OPERATOR_CATEGORY        0x04  /* Operator, language Russian */
#define SS7_SP_OPERATOR_CATEGORY        0x05  /* Operator, language Spanish */
#define SS7_RESERVED_CATEGORY           0x09  /* Reserved (see Q.104) */
#define SS7_ORDINARY_SUBS_CATEGORY      0x0A  /* Ordinary calling subscriber (Default) */
#define SS7_PRIORITY_SUBS_CATEGORY      0x0B  /* Calling subscriber with priority */
#define SS7_DATA_CATEGORY               0x0C  /* Data call (voice band data) */
#define SS7_TEST_CATEGORY               0x0D  /* Test call */
#define SS7_PAYPHONE_CATEGORY           0x0F  /* Payphone */

/*
 * Presentation indicators
 */
#define PRESENTATION_ALLOWED           0x00  /* Default */
#define PRESENTATION_RESTRICTED        0x01
#define PRESENTATION_NOT_AVAILABLE     0x02

/*
 * Screening indicators
 */
#define SCREEN_USER_PROVIDED           0x00  /* Default */
#define SCREEN_USER_PROVIDED_VERIFIED  0x01
#define SCREEN_USER_PROVIDED_FAILED    0x02
#define SCREEN_NETWORK_PROVIDED        0x03

/*
 * Internal Network Number indicator
 */
#define INN_ALLOWED              0x00  /* routing to internal network allowed */
#define INN_NOT_ALLOWED          0x01  /* routing to internal network not allowed */

/*
 * Forward Call Indicators
 */
/* National/international call indicator */
#define FCI_NATIONAL_CALL              0x0000
#define FCI_INTERNATIONAL_CALL         0x0001
/* End-to-end method indicator */
#define FCI_PASS_ALONG_AVAILABLE       0x0002
#define FCI_SCCP_AVAILABLE             0x0004
#define FCI_PASS_ALONG_AND_SCCP        0x0006  /* combination of both above */
/* Interworking indicator */
#define FCI_INTERWORKING               0x0008
/* End-to-end information indicator */
#define FCI_END_TO_END_AVAILABLE       0x0010
/* ISDN user part indicator */
#define FCI_ISUP_ALL_WAY               0x0020
/* ISDN user part preference indicator */
#define FCI_ISUP_PREFERRED_ALL_WAY     0
#define FCI_ISUP_NOT_REQUIRED_ALL_WAY  0x0040
#define FCI_ISUP_REQUIRED_ALL_WAY      0x0080
/* ISDN access indicator */
#define FCI_ISDN_ACCESS                0x0100
/* SCCP method indicator */
#define FCI_CONNECTIONLESS             0x0200
#define FCI_CONNECTION_ORIENTED        0x0400
#define FCI_CONNECTION_BOTH            0x0600  /* combination of both above */

/*
 * Backward Call Indicators
 */
/* Charge indicator */
#define BCI_NO_CHARGE                  0x0001
#define BCI_CHARGE                     0x0002
/* Called party's status indicator */
#define BCI_SUBSCRIBER_FREE            0x0004
#define BCI_CONNECT_WHEN_FREE          0x0008
/* Called party's category indicator */
#define BCI_ORDINARY_SUBSCRIBER        0x0010
#define BCI_PAYPHONE                   0x0020
/* End-to-end method indicator */
#define BCI_PASS_ALONG_AVAILABLE       0x0040
#define BCI_SCCP_AVAILABLE             0x0080
#define BCI_PASS_ALONG_AND_SCCP        0x00C0  /* combination of both above */
/* Interworking indicator */
#define BCI_INTERWORKING               0x0100
/* End-to-end information indicator */
#define BCI_END_TO_END_AVAILABLE       0x0200
/* ISDN user part indicator */
#define BCI_ISUP_ALL_WAY               0x0400
/* Holding indicator */
#define BCI_HOLDING_REQUESTED          0x0800
/* ISDN access indicator */
#define BCI_ISDN_ACCESS                0x1000
/* Echo control device indicator */
#define BCI_ECHO_CONTROL_INCLUDED      0x2000
/* SCCP method indicator */
#define BCI_CONNECTIONLESS             0x4000
#define BCI_CONNECTION_ORIENTED        0x8000
#define BCI_CONNECTION_BOTH            0xC000  /* combination of both above */

/*
 * User-to-User Indicators
 */
#define UUI_UUS1_REQ_NE                0x04  /* Service 1; request, not essential */
#define UUI_UUS1_REQ_E                 0x06  /* Service 1; request, essential */
#define UUI_UUS2_REQ_NE                0x10  /* Service 2; request, not essential */
#define UUI_UUS2_REQ_E                 0x18  /* Service 2; request, essential */
#define UUI_UUS3_REQ_NE                0x40  /* Service 3; request, not essential */
#define UUI_UUS3_REQ_E                 0x60  /* Service 3; request, essential */
#define UUI_UUS1_RSP_P                 0x05  /* Service 1; response, provided */
#define UUI_UUS2_RSP_P                 0x11  /* Service 2; response, provided */
#define UUI_UUS3_RSP_P                 0x41  /* Service 3; response, provided */
#define UUI_UUSx_RSP_P                 0x55  /* Services 1, 2 and 3; response, provided */


/*
 * Libgcs7 Extension functions and events
 */
#define S7_EXT_APPLYLOOPBACK        0x01  /* Request applying loopback */
#define S7_EXT_REQUESTCONTCHECK     0x02  /* Request a continuity check */
#define S7_EXT_SENDCONTCHECKRESULT  0x03  /* Send a continuity check result */
#define S7_EXT_CONTCHECK            0x04  /* Indicates a cont check event (GCEV_EXTENSION) */
#define S7_EXT_CONTCHECK_END        0x05  /* Indicates a cont check end event (GCEV_EXTENSION) */
#define S7_EXT_ALARM_EVENT          0x06  /* Indicates an alarm condition on/off */
#define S7_EXT_ISUP_EVENT           0x07  /* Indicates receiving event from ISUP that
                                             has not been processed by GCSS7 */
#define S7_EXT_APP_BLOCK            0x08  /* Indicates Management Application Block */
#define S7_EXT_APP_UNBLOCK          0x09  /* Indicates Management Application Unblock */
#define S7_EXT_CONTCHECK_MGMTREQ    0x0A  /* Management Requests Application to perform a continuity test */

/*
 * SS7 Specific cause definitions for dropping a call
 * Use as cause value in gc_DropCall()
 */
#define CONTCHECK_FAILED      0x90  /* Continuity check failed during call setup */

/*
 * Flags to use for extension S7_EXT_SENDCONTCHECKRESULT
 */
#define CONTI_SUCCESS         0x01  /* Continuity Check successful */
#define CONTI_FAILURE         0x00  /* Continuity Check failed */

/*
 * Alarm values.
 */
#define SS7_FRAME_SYNC_LOSS			0xa
#define SS7_FRAME_SYNC_OK			0xb
#define SS7_AIS_DETECTED			0xc
#define SS7_AIS_CLEARED				0xd
#define SS7_REMOTE_ALARM_DETECTED	0xe
#define SS7_REMOTE_ALARM_CLEARED	0xf
#define SS7_PCM_LOSS				0x14
#define SS7_PCM_OK					0x15
#define SS7_FRAME_SLIP				0x16
#define SS7_BER5					0x19
#define SS7_BER5_CLEARED			0x1a
#define SS7_BER3					0x1b
#define SS7_BER3_CLEARED			0x1c


/*
 * CCITT Cause values
 *
 * NOTE: Not all cause values are universally supported across switch types.
 *       Before a particular cause value is used, its validity should be
 *       compared with the appropriate switch vendor specs.
 */
#define UNASSIGNED_NUMBER        0x01  /* Cause 01 */
#define NO_ROUTE                 0x02  /* Cause 02 */
#define SEND_SIT                 0x04  /* Cause 04 */
#define MISDIALLED_TRUNK_PREFIX  0x05  /* Cause 05 */
#define CHANNEL_UNACCEPTABLE     0x06  /* Cause 06 */
#define NORMAL_CLEARING          0x10  /* Cause 16 */
#define USER_BUSY                0x11  /* Cause 17 */
#define NO_USER_RESPONDING       0x12  /* Cause 18 */
#define CALL_REJECTED            0x15  /* Cause 21 */
#define NUMBER_CHANGED           0x16  /* Cause 22 */
#define DEST_OUT_OF_ORDER        0x1B  /* Cause 27 */
#define INVALID_NUMBER_FORMAT    0x1C  /* Cause 28 */
#define FACILITY_REJECTED        0x1D  /* Cause 29 */
#define RESP_TO_STAT_ENQ         0x1E  /* Cause 30 */
#define UNSPECIFIED_CAUSE        0x1F  /* Cause 31 */
#define NO_CIRCUIT_AVAILABLE     0x22  /* Cause 34 */
#define NETWORK_OUT_OF_ORDER     0x26  /* Cause 38 */
#define TEMPORARY_FAILURE        0x29  /* Cause 41 */
#define NETWORK_CONGESTION       0x2A  /* Cause 42 */
#define ACCESS_INFO_DISCARDED    0x2B  /* Cause 43 */
#define REQ_CHANNEL_NOT_AVAIL    0x2C  /* Cause 44 */
#define PRE_EMPTED               0x2D  /* Cause 45 */
#define FACILITY_NOT_SUBSCRIBED  0x32  /* Cause 50 */
#define OUTGOING_CALL_BARRED     0x34  /* Cause 52 */
#define INCOMING_CALL_BARRED     0x36  /* Cause 54 */
#define BEAR_CAP_NOT_AVAIL       0x3A  /* Cause 58 */
#define SERVICE_NOT_AVAIL        0x3F  /* Cause 63 */
#define CAP_NOT_IMPLEMENTED      0x41  /* Cause 65 */
#define CHAN_NOT_IMPLEMENTED     0x42  /* Cause 66 */
#define FACILITY_NOT_IMPLEMENT   0x45  /* Cause 69 */
#define INVALID_CALL_REF         0x51  /* Cause 81 */
#define CHAN_DOES_NOT_EXIST      0x52  /* Cause 82 */
#define INCOMPATIBLE_DEST        0x58  /* Cause 88 */
#define INVALID_MSG_UNSPEC       0x5F  /* Cause 95 */
#define MANDATORY_IE_MISSING     0x60  /* Cause 96 */
#define NONEXISTENT_MSG          0x61  /* Cause 97 */
#define WRONG_MESSAGE            0x62  /* Cause 98 */
#define BAD_INFO_ELEM            0x63  /* Cause 99 */
#define INVALID_ELEM_CONTENTS    0x64  /* Cause 100 */
#define WRONG_MSG_FOR_STATE      0x65  /* Cause 101 */
#define TIMER_EXPIRY             0x66  /* Cause 102 */
#define MANDATORY_IE_LEN_ERR     0x67  /* Cause 103 */
#define PROTOCOL_ERROR           0x6F  /* Cause 111 */
#define SUBSCRIBER_LOCAL_BUSY    0x7D  /* Cause 125 */ /* CHINA specific */
#define SUBSCRIBER_TRUNK_BUSY    0x7E  /* Cause 126 */ /* CHINA specific */
#define INTERWORKING_UNSPEC      0x7F  /* Cause 127 */


/*
 * Libgcs7 Internal Cause values
 */
#define S7RV_RESULT            0x4000
#define S7RV_SUCCESS           S7RV_RESULT
#define S7RV_TIMEOUT           (S7RV_RESULT | 0x01)  /* Timeout (e.g. in MakeCall) */
#define S7RV_SIU_TRANSFER      (S7RV_RESULT | 0x02)  /* Circuit blocked for transferring its group between units */
#define S7RV_GLARE             (S7RV_RESULT | 0x03)  /* Glare condition */
#define S7RV_BLK_LOCAL_MAINT   (S7RV_RESULT | 0x04)  /* Circuit locally maintenance blocked */
#define S7RV_BLK_LOCAL_HARD    (S7RV_RESULT | 0x05)  /* Circuit locally hardware blocked */
#define S7RV_BLK_REMOTE_MAINT  (S7RV_RESULT | 0x06)  /* Circuit remotely maintenance blocked */
#define S7RV_BLK_REMOTE_HARD   (S7RV_RESULT | 0x07)  /* Circuit remotely hardware blocked */
#define S7RV_BLK_MTP           (S7RV_RESULT | 0x08)  /* Blocked at MTP level (DPC not available) */
#define S7RV_BLK_SIU_DOWN      (S7RV_RESULT | 0x09)  /* Blocked because of SIU failure */
#define S7RV_MGMT              (S7RV_RESULT | 0x0A)  /* Event caused by management operation */
#define S7RV_LOCAL_RESET       (S7RV_RESULT | 0x0B)  /* Circuit locally reset, clears maint. & hard. blocking */
#define S7RV_REMOTE_RESET      (S7RV_RESULT | 0x0C)  /* Circuit remotely reset, clears maint. & hard. blocking */
/* Result values for S7_EXT_CONTCHECK in GCEV_EXTENSION */
#define S7RV_CC_INBOUND        (S7RV_RESULT | 0x80)  /* Inbound continuity check, must apply loopback */
#define S7RV_CC_OUTBOUND       (S7RV_RESULT | 0x81)  /* May proceed with the outbound continuity check */
/* Result values for S7_EXT_CONTCHECK_END in GCEV_EXTENSION */
#define S7RV_CCEND_INBOUND_FAILURE  (S7RV_RESULT | 0x90)  /* Inbound continuity check failed */
#define S7RV_CCEND_INBOUND_SUCCESS  (S7RV_RESULT | 0x91)  /* Inbound continuity check succeded */
#define S7RV_CCEND_OUTBOUND_ERROR   (S7RV_RESULT | 0x92)  /* Outbound continuity check encountered an error */
#define S7RV_CCEND_OUTBOUND_GLARE   (S7RV_RESULT | 0x93)  /* Outbound continuity check ended due to glare with incoming IAM or CCR */
#define S7RV_CCEND_OUTBOUND         (S7RV_RESULT | 0x94)  /* Outbound continuity check ended by the S7_EXT_SENDCONTCHECKRESULT */
/* Result values for GCEV_MOREDIGITS/GCEV_MOREINFO */
#define S7RV_INFO_PRESENT_ALL       (S7RV_RESULT | 0xa0) /* The req info is available */
#define S7RV_INFO_PRESENT_MORE      (S7RV_RESULT | 0xa1) /* The req info is available, and even more can be requested */
#define S7RV_INFO_SOME_TIMEOUT      (S7RV_RESULT | 0xa2) /* The req info is not yet available. Only some info is present, not all  */
#define S7RV_INFO_SOME_NOMORE       (S7RV_RESULT | 0xa3) /* The req info is not yet available. No more info is coming in */
#define S7RV_INFO_NONE_TIMEOUT      (S7RV_RESULT | 0xa4) /* The req info is not available. No info came in  */
#define S7RV_INFO_NONE_NOMORE       (S7RV_RESULT | 0xa5) /* The req info is not available. No info came in and none is expected */
#define S7RV_INFO_SENT              (S7RV_RESULT | 0xa6) /* The information has been sent successfully */
#define S7RV_DESTINATION_ADDRESS_REQ   (S7RV_RESULT | 0xa7) /* The destination information has been requested by the remote side */
#define S7RV_ORIGINATION_ADDRESS_REQ   (S7RV_RESULT | 0xa8) /* The origination information has been requested by the remote side */
/* Result values for GCEV_ALARM */
#define S7RV_FRAME_SYNC_LOSS		(S7RV_RESULT | 0xa9) /* Frame Sync Loss alarm is active. */
#define S7RV_FRAME_SYNC_OK			(S7RV_RESULT | 0xaa) /* Frame Sync Loss alarm has cleared. */
#define S7RV_AIS_DETECTED			(S7RV_RESULT | 0xab) /* AIS alarm is active. */
#define S7RV_AIS_CLEARED			(S7RV_RESULT | 0xac) /* AIS alarm has cleared. */
#define S7RV_REMOTE_ALARM_DETECTED	(S7RV_RESULT | 0xad) /* Remote Alarm is active. */
#define S7RV_REMOTE_ALARM_CLEARED	(S7RV_RESULT | 0xae) /* Remote Alarm has cleared. */
#define S7RV_PCM_LOSS				(S7RV_RESULT | 0xaf) /* PCM Loss alarm is active. */
#define S7RV_PCM_OK					(S7RV_RESULT | 0xb0) /* PCM Loss alarm has cleared. */
#define S7RV_FRAME_SLIP				(S7RV_RESULT | 0xb1) /* Frame Slip alarm is active. */
#define S7RV_BER5					(S7RV_RESULT | 0xb2) /* BER > 1 in 100,000 alarm is active. */
#define S7RV_BER5_CLEARED			(S7RV_RESULT | 0xb3) /* BER > 1 in 100,000 alarm has cleared. */
#define S7RV_BER3					(S7RV_RESULT | 0xb4) /* BER > 1 in 1,000 alarm is active. */
#define S7RV_BER3_CLEARED			(S7RV_RESULT | 0xb5) /* BER > 1 in 1,000 alarm has cleared. */


/*
 * Libgcs7 CCLib Specific Error Codes
 */
#define S7ERR_NOERR                  0x00  /* No Error */
#define S7ERR_BASE               0x8000
#define S7ERR_NO_SESSION         (S7ERR_BASE | 0x01)  /* No session was established with SS7 Service */
#define S7ERR_UNSUPPORTED        (S7ERR_BASE | 0x02)  /* Function not supported */
#define S7ERR_INV_PARM           (S7ERR_BASE | 0x03)  /* Invalid parameter */
#define S7ERR_INV_INFO_ID        (S7ERR_BASE | 0x04)  /* Invalid Call Info ID */
#define S7ERR_INV_PARM_ID        (S7ERR_BASE | 0x05)  /* Invalid Parameter ID (in Set/GetParm) */
#define S7ERR_INV_SIGINFO_SIZE   (S7ERR_BASE | 0x06)  /* Invalid SigInfo Buffer size */
#define S7ERR_LDEV_RELATED       (S7ERR_BASE | 0x07)  /* Event is related to a LineDevice (=> no CRN, no SigInfo) */
#define S7ERR_NO_SIGINFO         (S7ERR_BASE | 0x08)  /* No SigInfo was associated with the event */
#define S7ERR_NO_SCBUSCONNECTOR  (S7ERR_BASE | 0x09)  /* Device does not support SCBus routing functions */
#define S7ERR_INV_DEVNAME        (S7ERR_BASE | 0x0A)  /* Invalid Device Name */
#define S7ERR_INV_STATE          (S7ERR_BASE | 0x0B)  /* Invalid State (Call/LineDev) */
#define S7ERR_INV_CRN            (S7ERR_BASE | 0x0C)  /* Invalid CRN */
#define S7ERR_INV_CID            (S7ERR_BASE | 0x0D)  /* Internal Error */
#define S7ERR_INV_LINEDEV        (S7ERR_BASE | 0x0E)  /* Invalid LineDevice */
#define S7ERR_INV_TRUNKDEV       (S7ERR_BASE | 0x0F)  /* Invalid TrunkDevice */
#define S7ERR_INV_CHANNEL        (S7ERR_BASE | 0x10)  /* TrunkDevice has no such channel (ts) */
#define S7ERR_NO_BASE_TS         (S7ERR_BASE | 0x11)  /* BaseTimeSlot not defined for the Trunk */
#define S7ERR_TLS_NULL           (S7ERR_BASE | 0x12)  /* ThreadLocalStorage is NULL */
#define S7ERR_PING_EVENT         (S7ERR_BASE | 0x13)  /* Sytem Error */
#define S7ERR_MSGQ_FULL          (S7ERR_BASE | 0x14)  /* Internal Error */
#define S7ERR_INV_PARM_SIZE      (S7ERR_BASE | 0x15)  /* Internal Error */
#define S7ERR_SRL                (S7ERR_BASE | 0x16)  /* SRL Error */
#define S7ERR_SRL_PUTEVT         (S7ERR_BASE | 0x17)  /* SRL PutEvt Error */
#define S7ERR_DTI_GENERIC        (S7ERR_BASE | 0x18)  /* Unspecified DTI error */
#define S7ERR_DTI_OPEN           (S7ERR_BASE | 0x19)  /* Error opening DTI device */
#define S7ERR_DTI_GETXMIT        (S7ERR_BASE | 0x1A)  /* Error getting DTI TX time slot */
#define S7ERR_DTI_LISTEN         (S7ERR_BASE | 0x1B)  /* Error listening on DTI device */
#define S7ERR_DTI_UNLISTEN       (S7ERR_BASE | 0x1C)  /* Error unlistening on DTI device */
#define S7ERR_LOG_ATTACH         (S7ERR_BASE | 0x1D)  /* Error attaching file to logger */
#define S7ERR_NOMEM              (S7ERR_BASE | 0x1E)  /* Out of memory */
#define S7ERR_GCT_SYSTEM         (S7ERR_BASE | 0x1F)  /* Error in DataKinetics GCT System */
#define S7ERR_COM_SYSTEM         (S7ERR_BASE | 0x20)  /* Error in COM system */
#define S7ERR_TIMER_INIT         (S7ERR_BASE | 0x21)  /* Error initializing Timer sub-system */
#define S7ERR_TIMER_ACTIVE       (S7ERR_BASE | 0x22)  /* Attempt to start an already active timer */
#define S7ERR_NO_MORE_CRN        (S7ERR_BASE | 0x23)  /* Too many CRNs allocated on the LineDevice */
#define S7ERR_ISUP_CODING        (S7ERR_BASE | 0x24)  /* Error while coding ISUP message */
#define S7ERR_ISUP_DECODING      (S7ERR_BASE | 0x25)  /* Error while decoding ISUP message */
#define S7ERR_INV_MODE           (S7ERR_BASE | 0x26)  /* SYNC/ASYNC Mode not supported */
#define S7ERR_OPEN_VOICE         (S7ERR_BASE | 0x27)  /* Error opening voice device (in gc_Open) */
#define S7ERR_NO_VOICE           (S7ERR_BASE | 0x28)  /* No voice resource attached */
#define S7ERR_VOX_LISTEN         (S7ERR_BASE | 0x29)  /* Error in dx_listen (in gc_Open) */
#define S7ERR_VOX_GETXMIT        (S7ERR_BASE | 0x2A)  /* Error in dx_getxmitslot (in gc_Open) */
#define S7ERR_INIT_EVTMSK        (S7ERR_BASE | 0x2B)  /* Internal initialization error */
#define S7ERR_CIRCUIT_IN_USE     (S7ERR_BASE | 0x2C)  /* Circuit is already in use in another process */
#define S7ERR_SERVICE_NOT_READY  (S7ERR_BASE | 0x2D)  /* SS7 Service is not running or not correctly initialized */
#define S7ERR_NOT_ATTACHED       (S7ERR_BASE | 0x2E)  /* Internal Error */
#define S7ERR_WATCHDOG_FAIL      (S7ERR_BASE | 0x2F)  /* Internal Error */
#define S7ERR_NO_MORE_DIGITS     (S7ERR_BASE | 0x30)  /* No additional digit can be obtained */
#define S7ERR_GC_CME             (S7ERR_BASE | 0x31)  /* Internal Error (GC CME Module) */
#define S7ERR_GC_DB              (S7ERR_BASE | 0x32)  /* Internal Error (GC DB Module) */
#define S7ERR_SRL_DEPOSIT        (S7ERR_BASE | 0x33)  /* Internal Error (sr_deposit) */
#define S7ERR_ISP_BAD_ID         (S7ERR_BASE | 0x34)  /* Internal Error (ISP response: Bad ID) */
#define S7ERR_ISP_BAD_PARAM      (S7ERR_BASE | 0x35)  /* Internal Error (ISP response: Bad parameter) */
#define S7ERR_ISP_BAD_MSG        (S7ERR_BASE | 0x36)  /* Internal Error (ISP response: Bad Message) */
#define S7ERR_PK_TRUNK_NOT_EXIST (S7ERR_BASE | 0x37)  /* Internal Error (Dynamic config: PK_TRUNK already existed) */
#define S7ERR_LIU_NOT_DEF        (S7ERR_BASE | 0x38)  /* Internal Error (Dynamic config: LIU not defined) */
#define S7ERR_PK_CGRP_EXIST      (S7ERR_BASE | 0x39)  /* Internal Error (Dynamic config: PK_XUP_CFG_CGRP already existed) */

#define S7ERR_UNKNOWN            (S7ERR_BASE | 0xFF)  /* Unknown Error */

#endif	/* __cc_s7_h__ */
