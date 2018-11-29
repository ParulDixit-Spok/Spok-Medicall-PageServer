/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2000-2007 Dialogic Corporation. All Rights Reserved.
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
*	C Header:		dm3cc_parm.h
*	Instance:		dte_1
*	Description:	DM3CC Call Control Library Parameters
*	%date_created:	Wed July 26 2006 %
*
**********************************************************************/

#ifndef _dte_1_dm3cc_parm_h_H
#define _dte_1_dm3cc_parm_h_H

/********************************************************************************************/
/****************** Extension Ids (ext_id in gc_Extension and/or for GCEV_EXTENSION) ********/
/********************************************************************************************/
#define CC_EXT_EVT_BCHAN_NEGOTIATED         1
#define CC_EXT_EVT_XFER                     3
#define CC_EXT_EVT_DIAL                     4
#define CC_EXT_EVT_CALL_DIALING             5
#define CC_EXT_EVT_CALL_ORIGINATED          6
#define DM3CC_EXID_BIT_PATTERN              7
#define DM3CC_EXT_EVT_SUSPEND_RESUME        8
#define DM3CC_EXT_EVT_CALLSTATUS_TRANSITION	9		/* Reception of CST information */
#define DM3CC_EXID_SETDLINKSTATE            0x0a	/*the same as GCIS_EXID_SETDLINKSTATE in SW */
#define DM3CC_EXID_TXRX_SIGBITS_GET         0x10
#define DM3CC_EXID_INTRUSION                0x11	/* Feature Intrusion */

/* Extension ID defines for Non-Call message  */
#define DM3CC_EXT_EVT_FACILITYGLOBAL        0x69        /* GLOBAL Facility message */ 
#define DM3CC_EXT_EVT_FACILITYNULL          0x6A        /* NULL CRN Facility message */ 
#define DM3CC_EXT_EVT_INFOGLOBAL            0x6B        /* GLOBAL Info message */ 
#define DM3CC_EXT_EVT_INFONULL              0x6C        /* NULL CRN Info message */
#define DM3CC_EXT_EVT_NOTIFYGLOBAL          0x6D        /* GLOBAL Notify message */  
#define DM3CC_EXT_EVT_NOTIFYNULL            0x6E        /* NULL CRN Notify message */ 
/* The following pair of defines both provide call reject notification. The CC_EXT variant */
/* is here for backward compatibility but is deprecated. Use the DM3CC_EXT variant instead. */
#define CC_EXT_EVT_CALL_REJECTED            0x6F
#define DM3CC_EXT_EVT_CALL_REJECTED         CC_EXT_EVT_CALL_REJECTED

#define DM3CC_EXT_EVT_RAWEEM				0x70		/* Reception of DPNSS raw EEM */

/* Extension ID defines (ext_id for gc_Extension()) */
#define DM3CC_EXID_GET_NONCALLMSG			0x10


/* And the parameters to enable them (using gc_SetParm) */
#define DM3CC_PARM_BCHAN_NEGOTIATED_EVENT	0xC001
#define DM3CC_PARM_CALL_REJECTED_EVENT		0xC002
#define DM3CC_PARM_NONCALL_MSG_EVENT		0xC003			/* To enable non-call message */ 

/* Define of the maximum length of IE data */
#define DM3CC_MAXLEN_IEDATA						260

/* Define of the maximum length of Protocol name */
#define DM3CC_MAXLEN_PROTOCOL_NAME				32

/********************************************************************************************/
/*                          DM3 CC Set ID and Parm ID Start                                 */
/********************************************************************************************/

/********************************************************************************************/
/******************************** Channel Set ***********************************************/
/********************************************************************************************/
#define CCSET_CHANNEL                           GC_DM3CCLIB_SET_MIN
#define CCPARM_CHANNEL_ID                       0x1
#define CCPARM_INTERFACE_ID                     0x2

/********************************************************************************************/
/** "New Channel" set - the parameters of this set are the same as those of Channel ID set **/
/********************************************************************************************/
#define CCSET_NEW_CHANNEL GC_DM3CCLIB_SET_MIN + 1

/********************************************************************************************/
/*************************** Call Rejection set *********************************************/
/********************************************************************************************/
#define CCSET_CALLREJECTION_INFO				GC_DM3CCLIB_SET_MIN + 2
#define CCPARM_CALLREJECTION_CAUSE				0x1
#define CCPARM_CALLREJECTION_CAUSE_ALIAS		0x2

/********************************************************************************************/
/**************************** Tsc_MsgDial Set ***********************************************/
/********************************************************************************************/
#define CCSET_DIAL								GC_DM3CCLIB_SET_MIN + 4
#define CCPARM_DIAL_STRING						0x1
#define CCPARM_DIAL_CALLPROGRESS				0x2

/********************************************************************************************/
/************************ Transparent Setparm to NetTsc board Set ***************************/
/********************************************************************************************/
#define CCSET_NETTSC_PARM						GC_DM3CCLIB_SET_MIN + 5

/********************************************************************************************/
/**************************** Enable Unsollicited GCEV_EXTENSION ****************************/
/********************************************************************************************/

#define CCSET_EXTENSIONEVT_MSK					GC_DM3CCLIB_SET_MIN + 6
/* The ParmID in this Set are GCACT_SETMSK, GCACT_ADDMSK and GCACT_SUBMSK */
/* Possible value buf */
#define EXTENSIONEVT_BCHAN_NEGOTIATED			(1 << CC_EXT_EVT_BCHAN_NEGOTIATED)	/* 0x2  */
#define EXTENSIONEVT_CALL_REJECTED				0x4
#define EXTENSIONEVT_XFER						(1 << CC_EXT_EVT_XFER)				/* 0x10  */
#define EXTENSIONEVT_DIALING					(1 << CC_EXT_EVT_CALL_DIALING)	    /* 0x20 */
#define EXTENSIONEVT_ORIGINATED					(1 << CC_EXT_EVT_CALL_ORIGINATED)	/* 0x40 */
#define EXTENSIONEVT_BIT_PATTERN				(1 << DM3CC_EXID_BIT_PATTERN)	/* 0x80 */
#define EXTENSIONEVT_SUSPEND_RESUME				(1 << DM3CC_EXT_EVT_SUSPEND_RESUME)	/* 0x100 */
#define EXTENSIONEVT_CALLSTATUS_TRANSITION		(1 << DM3CC_EXT_EVT_CALLSTATUS_TRANSITION) /* 0x200 */
#define EXTENSIONEVT_RAWEEM						0x400

#define EXTENSIONEVT_NONCALLMSG					0x10000								/* 0x10000 */
#define EXTENSIONEVT_ALL						0xFFFF

/********************************************************************************************/
/************************** Message Waiting Indication (MWI) Set ****************************/
/********************************************************************************************/
#define CCSET_MWI_MSGSET						GC_DM3CCLIB_SET_MIN + 7
#define CCPARM_MWI_ON							0x0
#define CCPARM_MWI_OFF							0x1

/********************************************************************************************/
/**************************** D Channel State Set *******************************************/
/********************************************************************************************/
#define DM3CC_SET_DLINK					GC_DM3CCLIB_SET_MIN + 8
#define DM3CC_PARM_DLINK_STATE		0x03	/*the same as GCIS_PARM_DLINK_STATE in SW */

/********************************************************************************************/
/**************************** CallerID on CallWaiting Set for Tsc_msgDial *******************/
/********************************************************************************************/
#define CCSET_CALLERID_CALLWAITING_DIAL			GC_DM3CCLIB_SET_MIN + 9
#define CCPARM_CALLERID_DT_STRING				0x1
#define CCPARM_CALLERID_NAME_STRING				0x2
#define CCPARM_CALLERID_NUM_STRING				0x3
#define CCPARM_CALLERID_NUMAR_STRING			0x4
#define CCPARM_CALLERID_NAMEAR_STRING			0x5
#define CCPARM_CALLERID_RAW_STRING				0x6

/********************************************************************************************/
/***************************** Volume Control for Stations **********************************/
/********************************************************************************************/
#define CCSET_VOLUME_CONTROL					GC_DM3CCLIB_SET_MIN + 10
#define CC_PARM_VOL_RES							0x1
#define CC_PARM_VOL_ADJ							0x2

/********************************************************************************************/
/*********************************** Distinctive ringing ************************************/
/********************************************************************************************/
#define CCSET_DISTINCTIVE_RING					GC_DM3CCLIB_SET_MIN + 11
#define CC_PARM_RINGID							0x1	

/********************************************************************************************/
/************************************ Call Analysis related parameters **********************/
/********************************************************************************************/
#define CCSET_CALLANALYSIS						GC_DM3CCLIB_SET_MIN + 12
#define CCPARM_CA_MODE							0x1
#define CCPARM_CA_PAMDSPDVAL					0x2
#define CCPARM_CA_HEDGE							0x3
#define CCPARM_CA_LOGLTCH						0x4
#define CCPARM_CA_HIGHLTCH						0x5
#define CCPARM_CA_ANSRDGL						0x6
#define CCPARM_CA_NOANSR						0x7
#define CCPARM_CA_NOSIG							0x8
#define CCPARM_CA_PAMDFAILURE					0x9
#define CCPARM_CA_PAMD_QTEMP					0xA
#define CCPARM_CA_PVD_QTEMP						0xB

// CCPARM_CA_MODE can take any value combinations of one of the following flags
#define GC_CA_BUSY 								0x1 	// Busy tone Detection (Pre-Connect)
#define GC_CA_RINGING							0x2 	// Ring back detection (Pre-Connect)
#define GC_CA_SIT								0x4 	// SIT tone detection (Pre-Connect)
#define GC_CA_FAX								0x100 	// Fax detection (Post-Connect)
#define GC_CA_PVD								0x200 	// PVD detection (Post-Connect)
#define GC_CA_PAMD								0x400 	// PAMD detection (Post-Connect)

// Global Call recommand using of the following combinations
#define GC_CA_DISABLE					0x0					// All disabled
#define GC_CA_PREONLY					(GC_CA_BUSY|GC_CA_RINGING)		// Busy and Ringing enabled (pre-connect only)
#define GC_CA_PREONLY_SIT				(GC_CA_BUSY|GC_CA_RINGING|GC_CA_SIT)	// All Pre-connect enabled (Busy, Ringing, Sit)
#define GC_CA_POSTONLY_PVD				(GC_CA_FAX|GC_CA_PVD)			// FAX + PVD (Post-connect only)
#define GC_CA_POSTONLY_PVD_PAMD			(GC_CA_FAX|GC_CA_PVD|GC_CA_PAMD)	// All Post-connect enabled
#define GC_CA_ENABLE_PVD				(GC_CA_BUSY|GC_CA_RINGING|GC_CA_SIT|GC_CA_FAX|GC_CA_PVD)		// All Pre-connect + FAX and PVD (Post-connect) enabled
#define GC_CA_ENABLE_ALL				(GC_CA_BUSY|GC_CA_RINGING|GC_CA_SIT|GC_CA_FAX|GC_CA_PVD|GC_CA_PAMD)	// All Pre-connect and Post-connect enabled

/********************************************************************************************/
/************************************ Direct Bit access *************************************/
/********************************************************************************************/
#define CCSET_BIT_PATTERN						GC_DM3CCLIB_SET_MIN + 13
#define CCPARM_INTPARM1							1
#define CCPARM_INTPARM2							2
#define CCPARM_CHARPARM1						4
#define CCPARM_CHARPARM2						5

#define CCSET_SIG_BITS                          GC_DM3CCLIB_SET_MIN + 16
#define CCPARM_CURRENT_STATE                    1

/********************************************************************************************/
/********************* Transparent Setparm to DM3 Firmware instances ************************/
/********************************************************************************************/
#define CCSET_DM3FW_PARM						GC_DM3CCLIB_SET_MIN + 14

/* Set config parms at runtime on DM3 Analog and Digital TSC */
#define CCDM3FW_TSC_ANALOG_RUNTIMECONFIG									0x121e
#define CCDM3FW_TSC_ANALOG_DISABLE_TONE_DISCONNECT_SUPERVISION			0x00000001
#define CCDM3FW_TSC_ANALOG_ENABLE_TONE_DISCONNECT_SUPERVISION			0x80000001
#define CCDM3FW_TSC_ANALOG_DISABLE_DIAL_TONE_DETECTION					0x00000002
#define CCDM3FW_TSC_ANALOG_ENABLE_DIAL_TONE_DETECTION						0x80000002

#define CCPARM_ECHOCANCEL                     0x1220
#define CCDM3FW_PARMECHOCANCEL_DISABLE        0x0
#define CCDM3FW_PARMECHOCANCEL_ENABLE         0x1

#define	CCDM3FW_PARMECHOCANCEL_ENABLE_NLP     0x2    /* NLP - Non Linear Processing */ 
/*
* This parameter is used to enable or disable echo cancellation (EC) on a per channel basis. 
* The parameter value is a bitmask intended to disable or enable EC with or without NLP (Non Linear Processing).
*  The possible values are:
*  0x0 : ( CCDM3FW_PARMECHOCANCEL_DISABLE ) - No Echo cancellation
*  0x1 : ( CCDM3FW_PARMECHOCANCEL_ENABLE  ) - Echo cancellation enabled
*  0x3 : ( CCDM3FW_PARMECHOCANCEL_ENABLE | CCDM3FW_PARMECHOCANCEL_ENABLE_NLP ) - Echo cancellation enabled, NLP enabled
*  The combination of parameters = 0x2
*  ( CCDM3FW_ PARMECHOCANCEL_DISABLE | CCDM3FW_PARMECHOCANCEL_ENABLE_NLP ) is is illegal.
*/

#define CCPARM_TRANSPARENTMODE                0x1221
#define CCDM3FW_PARMTRANSPARENTMODE_DISABLE   0x0
#define CCDM3FW_PARMTRANSPARENTMODE_ENABLE    0x1

/********************************************************************************************/
/************************ Configure Line/Protocol *************************************/
/********************************************************************************************/
#define CCSET_LINE_CONFIG						GC_DM3CCLIB_SET_MIN + 15
#define CCPARM_CRC								CCPARM_LINE_TYPE
#define CCPARM_USER_NETWORK						0x17
#define CCPARM_LINE_TYPE						0x1601
#define CCPARM_SIGNALING_TYPE					0x1602
#define CCPARM_CODING_TYPE						0x1603

/* Values for  CCPARM_LINE_TYPE */
#define MAXLINETYPEVALUES 5
enum EnumLineTypeValues  
{
  Enum_LineType_ValueInvalid = 0,   /*Invalid */
  Enum_LineType_dsx1_D4		 = 1,	/*D4 framing type - Superframe(SF).  Available for T1 */
  Enum_LineType_dsx1_E1		 = 2,   /*E1 standard framing */
  Enum_LineType_dsx1_E1_CRC	 = 3,	/*E1 standard framing & CRC-4 multiframe. */
  Enum_LineType_dsx1_ESF	 = 4    /*Extended Superframe(ESF). Available for T1 */
}; 

/* Values for  CCPARM_CODING_TYPE */
#define MAXCODINGTYPEVALUES 4
enum EnumCodingTypeValues  
{
  Enum_CodingType_ValueInvalid  = 0,  /*Invalid */
  Enum_CodingType_B8ZS			= 1,  /*B8ZS is modified AMI. Used on T1 lines */
  Enum_CodingType_AMI			= 2,  /*Alternate Mark Inversion */
  Enum_CodingType_HDB3			= 3   /*High Density bipolar of order 3. Used on E1 lines */
};

/* Values for CCPARM_SIGNALING_TYPE */
#define SIGNALINGTYPE_RANGE 		7      /* Numerical range of the Signaling Type #defines (0x0 to 0x6) */
#define CC_SIGNALING_TYPE_INVALID	0x0    /* Invalid */
#define CC_SIGNALING_TYPE_ISDN		0x5    /* ISDN signaling */
#define CC_SIGNALING_TYPE_CLEAR		0x6    /* Clear Channel signaling */


/********************************************************************************************/
/************************ Set Signal Bits ***************************************************/
/********************************************************************************************/
#define CCSET_SIG_BITS                          GC_DM3CCLIB_SET_MIN + 16
#define CCPARM_CURRENT_STATE                    1

/********************************************************************************************/
/************************************ Suspend and Resume ************************************/
/********************************************************************************************/
#define CCSET_SUSPEND_RESUME					GC_DM3CCLIB_SET_MIN + 17
/* Use the same CCPARM_INTPARM1, CCPARM_INTPARM2, CCPARM_CHARPARM1, and CCPARM_CHARPARM2 defined above*/
/* To use suspend/resume, in sig_Extension(), the first argument must be 0 and second argument has to be */
/* one of the following values */
/* Notice: the number of bit pattern signals used in direct bit access is limited to 50000 */
#define PDKVAL_SUSPEND							50000
#define PDKVAL_RESUME							50001
/********************************************************************************************/
/************************ Parm ID for Feature Intrusion *************************************/
/********************************************************************************************/
#define CCPARM_INTRUSION						0xC
/* Use above Parm ID with Set ID GCSET_CALLINFO for Ext ID DM3CC_EXID_INTRUSION */
/********************************************************************************************/
/********************* Set ID and Parm ID for IE data   *************************************/
/********************************************************************************************/
#define CCSET_IE								GC_DM3CCLIB_SET_MIN + 18

#define CCPARM_IEDATA							0x1
#define CCPARM_UIEDATA							0x2

/* The defines of CRN type for non-call message event */
#define DM3CC_VAL_NULLCRN						0x1				/* NULL CRN */
#define DM3CC_VAL_GLOBALCRN						0x2				/* Global CRN */

/* The defines of message types required to send to FW */
#define DM3CC_VAL_MSGTYPE_FACILITY				0x49			/* FACILITY message */
#define DM3CC_VAL_MSGTYPE_INFO					0x0F			/* INFO message */ 
#define DM3CC_VAL_MSGTYPE_NOTIFY				0x11			/* NOTIFY message */

/********************************************************************************************/
/*********************   Set ID and Parm ID for TRACE DATA  *********************************/
/********************************************************************************************/
#define CCSET_TRACEDATA				    		GC_DM3CCLIB_SET_MIN + 19

#define CCPARM_TRACEQ_SIZE						0x1	  /* Queue size of tracing data buffers: value - integer */

/********************************************************************************************/
/*****************  Set ID, Parm ID, and values for Call Status Transition  *****************/
/********************************************************************************************/
#define CCSET_CALLSTATUS_TRANSITION				GC_DM3CCLIB_SET_MIN + 22

/* Parmater ID for LOOPCURRENT TYPE */
#define CCPARM_CST_TYPE							0x1

/* The defines for different CST types */
#define CC_CST_LCON								0x1
#define CC_CST_LCOFF							0x2
#define CC_CST_LCREV							0x3
#define CC_CST_RINGON							0x4
#define CC_CST_RINGOFF							0x5

/********************************************************************************************/
/*****************  Set ID, Parm ID, and values for DPNSS RAW EEM			*****************/
/********************************************************************************************/
#define CCSET_RAWEEM							GC_DM3CCLIB_SET_MIN + 23

/* Parmater ID for DPNSS RAW EEM DATA */
#define CCPARM_RAWEEM_DATA						0x1

/********************************************************************************************/
/*********************** DM3 CC Set ID and Parm ID  End *************************************/
/********************************************************************************************/



/********************************************************************************************/
/*                       DM3 CC Protocol Set ID and Parm ID Start                           */
/********************************************************************************************/
/* DM3CC Protocol defined set IDs must be from 0xB000h to 0xB200h (45056--45568) */
#define PROTOCOL_SET_MIN									0xB000

/********************************************************************************************/
/*********************   Set ID and Parm ID for CAS Component  *********************************/
/********************************************************************************************/
#define PRSET_CAS_SIGNAL									PROTOCOL_SET_MIN

/* The parm ID is CAS comp signal ID, which is assigned by FW during download.  The {set ID, parm ID}
 * should be obtained by gc_QueryConfigData() function */

/********************************************************************************************/
/*********************   Set ID and Parm ID for TSC Component  *********************************/
/********************************************************************************************/
#define PRSET_TSC_VARIABLE									PROTOCOL_SET_MIN + 1

/* The parm ID is CHP comp variable ID, which is assigned by FW during download.  The {set ID, parm ID}
 * should be obtained by gc_QueryConfigData() function */


/********************************************************************************************/
/*********************** DM3 PROTOCOL Set ID and Parm ID End ********************************/
/********************************************************************************************/


#endif	/* dm3cc_parm.h */
