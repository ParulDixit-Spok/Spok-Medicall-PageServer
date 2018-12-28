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
 * FILE:            gcisdn.h
 * PURPOSE:         Defines for GC-ISDN Translation
 ****************************************************************************/

#ifndef GCISDN_H_INCLUDED
#define GCISDN_H_INCLUDED


#ifdef WIN32
#ifdef VME_SPAN
#else
#pragma pack(1)
#endif
#endif /* WIN32 */
/*
 *     Network-Specific facility element
 */
#define GCNSF_ISDN_CPN_PREF       0x01  /* facility coding - CPN perferred */
#define GCNSF_ISDN_BN_PREF        0x02  /* facility coding - BN preferred */
#define GCNSF_ISDN_CPN            0x03  /* facility coding - CPN */
#define GCNSF_ISDN_BN             0x04  /* facility coding - BN */
#define GCNSF_ISDN_CA_TSC         0x09  /* service coding - CA TSC */



#include "isdndef.h"
#include "cclib.h"

/* extension ids : These defines can be used by gc app to access isdn specific features
 * through gc_Extension function. */
#define GCIS_EXID_CALLPROGRESS		0x01
#define GCIS_EXID_GETBCHANSTATE		0x02
#define GCIS_EXID_GETDCHANSTATE		0x03
#define GCIS_EXID_GETDLINKSTATE		0x04
#define GCIS_EXID_GETENDPOINT		0x05
#define GCIS_EXID_GETFRAME			0x06
#define GCIS_EXID_GETNETCRV			0x07
#define GCIS_EXID_GETNONCALLMSG		0x08
#define GCIS_EXID_PLAYTONE			0x09
#define GCIS_EXID_SETDLINKSTATE		0x0a
#define GCIS_EXID_SNDFRAME			0x0b
#define GCIS_EXID_SNDMSG			0x0c
#define GCIS_EXID_SNDNONCALLMSG		0x0d
#define GCIS_EXID_STOPTONE			0x0e
#define GCIS_EXID_TONEREDEFINE		0x0f

/* set ids */
#define GCIS_SET_ADDRESS			0x4201

#define GCIS_SET_BEARERCHNL			0x4211

#define GCIS_SET_CALLPROGRESS		0x4221
#define GCIS_SET_CHANSTATE			0x4222

#define GCIS_SET_DLINK				0x4231
#define GCIS_SET_DLINKCFG			0x4232
#define GCIS_SET_DCHANCFG			0x4233

#define GCIS_SET_EVENTMSK			0x4241

#define GCIS_SET_FACILITY			0x4242

#define GCIS_SET_GENERIC			0x4251

#define GCIS_SET_IE					0x4261

#define GCIS_SET_SERVREQ			0x4271		
#define GCIS_SET_SNDMSG				0x4272

#define GCIS_SET_TONE				0x4281

#define GCIS_SET_CALLTYPE			0x4282


/* PARM ids */

/* parm ids under set GCIS_SET_ADDRESS */
/* GCIS_PARM_CALLEDADDRESSPLAN :Called number plan. Value type is int.
 * GCIS_PARM_CALLEDADDRESSTYPE :Called number type. Value type is int.
 * GCIS_PARM_CALLINGADDRESSPLAN :Calling number plan. Value type is int.
 * GCIS_PARM_CALLINGADDRESSTYPE :Calling number type. Value type is int.
 *
 */
#define GCIS_PARM_CALLEDADDRESSPLAN		0x01
#define GCIS_PARM_CALLEDADDRESSTYPE		0x02
#define GCIS_PARM_CALLINGADDRESSPLAN	0x03
#define GCIS_PARM_CALLINGADDRESSTYPE	0x04

/* parm ids under set GCIS_SET_BEARERCHNL */
/* GCIS_PARM_TRANSFERMODE : Bearer channel Information Transfer Mode. Value type is int.
 * GCIS_PARM_TRANSFERRATE : Bearer channel, Information Transfer Rate. Value type is int.
 */
#define GCIS_PARM_TRANSFERMODE			0x01
#define GCIS_PARM_TRANSFERRATE			0x02
#define GCIS_PARM_CODINGSTANDARD		0x03
#define GCIS_PARM_TRANSFERCAP			0x04

/* parm ids under set GCIS_SET_CALLPROGRESS */
/* GCIS_PARM_CALLPROGRESS_INDICATOR : Specifies the progress indicator. Value type is int.
 * GCIS_PARM_CALLPROGRESS_TONETYPE : Indicates the type of call progress tone.Value 
 * type is unsigned char.
 */ 
#define GCIS_PARM_CALLPROGRESS_INDICATOR	0x01
#define GCIS_PARM_CALLPROGRESS_TONETYPE		0x02

/* parm ids under set GCIS_SET_CHANSTATE  */
/* GCIS_PARM_BCHANSTATE : This holds the status of B channel. Value type is int.
 * GCIS_PARM_DCHANSTATE : This holds the status of D channel. Value type is int.
 */
#define GCIS_PARM_BCHANSTATE			0x01
#define GCIS_PARM_DCHANSTATE			0x02

/* parm ids under set GCIS_SET_DLINK  */
/* GCIS_PARM_DLINK_CES :The connection endpoint suffix. This is zero for PRI. The 
 * connection endpoint suffix specifies the telephone equipment associated with the 
 * station.Value type is char.
 *
 * GCIS_PARM_DLINK_SAPI :The service access pointer identifier. This is zero for PRI.
 * Value type is char.
 *
 * GCIS_PARM_DLINK_STATE :This holds data link state. Value type is int.
 */
#define GCIS_PARM_DLINK_CES				0x01
#define GCIS_PARM_DLINK_SAPI			0x02
#define GCIS_PARM_DLINK_STATE			0x03

/* parm ids under set GCIS_SET_DLINKCFG  */
/* GCIS_PARM_DLINKCFG_PROTOCOL : The protocol to be used on this logical link. 
 * Value type is int.
 *
 * GCIS_PARM_DLINKCFG_STATE :The original state in which the logical link should be 
 * configured. Value type is int.
 *
 * GCIS_PARM_DLINKCFG_TEI :Terminal Endpoint Identifier. Valid range 0-63. Value 
 * type is char.
 */
#define GCIS_PARM_DLINKCFG_PROTOCOL		0x01
#define GCIS_PARM_DLINKCFG_STATE		0x02
#define GCIS_PARM_DLINKCFG_TEI			0x03

/* parm ids under set GCIS_SET_DCHANCFG  */
/* 
 * GCIS_PARM_DCHANCFG_AUTOINITFLAG :Boolean value defining whether or not the terminal
 * is an auto initializing terminal. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_FIRMWARE_FEATUREMASKA :Firmware feature control parameter A. This 
 * is a bit mask parameter for setting features in the firmware. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_FIRMWARE_FEATUREMASKB :Firmware feature control parameter B. This is 
 * a bit mask parameter for setting features in the firmware. Currently not used.
 * Value type is int.
 *
 * GCIS_PARM_DCHANCFG_FIXEDTEIVALUE :Holds the TEI to be used for a fixed TEI assigning 
 * terminal. Valid values lie in range 0 to 63. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_L2ACCESS :Boolean value used to configure the DSL for direct 
 * layer 2 access or for full stack access. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_NUMENDPOINTS :Number of logical data links to be supported. Valid 
 * values lie in 1 to MAX_DLINK range. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_SPID :Holds the assigned Service Provider Identifier (SPID) value 
 * for terminal initialization. Value type ASCII digit string limited to the digits 
 * 0-9 and limited in length to MAX_SPID_SIZE
 *
 * GCIS_PARM_DCHANCFG_SWITCHSIDE :Holds the value defining whether the DSL should be 
 * configured as the Network side (NT) or the User side (TE). Value type is int.
 *
 * GCIS_PARM_DCHANCFG_SWITCHTYPE :Basic rate protocol (switch type) for DSL. Multiple 
 * run-time selectable switch types are available. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_TEIASSIGNMENT :Applies to User Side only. It specifies if the 
 * terminal has a fixed TEI or an auto-assigning TEI. Value type is int.
 *
 * GCIS_PARM_DCHANCFG_TMRXXX :Defines the Layer 3 timer values. See Q.931 specification 
 * and corresponding switch specifications for exact definitions and default values for
 * these timers. Not all timers are applicable to all of the switches. Specified 
 * values are in 10 millisecond increments. Possible values are:
 * 0/-1 : Default value for switch, 0 < n : Timer value in tens of milliseconds
 */
#define GCIS_PARM_DCHANCFG_AUTOINITFLAG				0x01
#define GCIS_PARM_DCHANCFG_FIRMWARE_FEATUREMASKA	0x02
#define GCIS_PARM_DCHANCFG_FIRMWARE_FEATUREMASKB	0x03
#define GCIS_PARM_DCHANCFG_FIXEDTEIVALUE			0x04
#define GCIS_PARM_DCHANCFG_L2ACCESS					0x05
#define GCIS_PARM_DCHANCFG_NUMENDPOINTS				0x06
#define GCIS_PARM_DCHANCFG_SPID						0x07
#define GCIS_PARM_DCHANCFG_SWITCHSIDE				0x08
#define GCIS_PARM_DCHANCFG_SWITCHTYPE				0x09
#define GCIS_PARM_DCHANCFG_TEIASSIGNMENT			0x0A
#define GCIS_PARM_DCHANCFG_TMR302					0x0B
#define GCIS_PARM_DCHANCFG_TMR303					0x0C
#define GCIS_PARM_DCHANCFG_TMR304					0x0D
#define GCIS_PARM_DCHANCFG_TMR305					0x0E
#define GCIS_PARM_DCHANCFG_TMR306					0x0F
#define GCIS_PARM_DCHANCFG_TMR308					0x10
#define GCIS_PARM_DCHANCFG_TMR309					0x11
#define GCIS_PARM_DCHANCFG_TMR310					0x12
#define GCIS_PARM_DCHANCFG_TMR312					0x13
#define GCIS_PARM_DCHANCFG_TMR313					0x14
#define GCIS_PARM_DCHANCFG_TMR318					0x15
#define GCIS_PARM_DCHANCFG_TMR319					0x16
#define GCIS_PARM_DCHANCFG_TMR322					0x17

/* parm ids under set GCIS_SET_EVENTMSK  */
/* GCIS_PARM_ADDMSK :Enables notification of events specified in the bitmask in addition
 * to previously set events.
 *
 * GCIS_PARM_SETMSK :Enables notification of events specified in the bitmask and disables 
 * notification of previously set events. 
 *
 * GCIS_PARM_SUBMSK :Disables notification of events specified in the bitmask.
 */
#define GCIS_PARM_ADDMSK			0x01
#define GCIS_PARM_SETMSK			0x02
#define GCIS_PARM_SUBMSK			0x03
#define GCIS_PARM_GETMSK				0x04

/* values for GCIS_PARM_ADDMSK, GCIS_PARM_SETMSK, GCIS_PARM_SUBMSK : These values
 * can be ORed
 */
#define GCISMSK_STATUS			0x4000
#define GCISMSK_STATUS_ENQUIRY	0x8000
#define GCISMSK_TERMINATE		0x10000
#define GCISMSK_TMREXPEVENT		0x20000
#define GCISMSK_SERVICE_NOTIFICATION	0x80000


/* parm ids under set GCIS_SET_FACILITY */
/* GCIS_PARM_FACILITY_CODINGVALUE :Facility coding value. Identifies the specific feature 
 * or service provided. Value type unsigned char.
 *
 * GCIS_PARM_FACILITY_FEATURESERVICE :Identifies facility request as a feature or a service.
 * Value type is unsigned char.
 */
#define GCIS_PARM_FACILITY_CODINGVALUE		0x01
#define GCIS_PARM_FACILITY_FEATURESERVICE	0x02


/* parm ids under set GCIS_SET_GENERIC */
/* GCIS_PARM_CALLINGPRESENTATION :Calling presentation indicator. Value type is int.
 *
 * GCIS_PARM_CALLINGSCREENING :Calling screening indicator field. Value type is int.
 *
 * GCIS_PARM_CRNTYPE :This idnentifies CRN type. Value type is int.	
 *
 * GCIS_PARM_DIRECTORYNUMBER :Directory Number (applicable to BRI User Side switches only).
 * Value type is unsigned char array of max size GCIS_MAX_VARDATA.
 *
 * GCIS_PARM_EVENTDATAP :This is used to pass event data buffer pointer from app to CCLIB 
 * to retrieve event specific information. Value type is void pointer.
 *
 * GCIS_PARM_NETCRV :Holds the network call reference value. Value type is int.
 *
 * GCIS_PARM_RECEIVEINFOBUF :Multiple IE buffer. Sets the size of the buffer, that 
 * is,the number of messages that can be stored in the information queue. Valid value 
 * lie in the range of 1 to MAX_RECEIVE_INFO_BUF. Value type is int.
 *
 * GCIS_PARM_SUBADDRESSNUMBER :Subaddress Number (applicable to BRI User Side switches only).
 * Value type is unsigned char array of max size GCIS_MAX_VARDATA.
 */
#define GCIS_PARM_CALLINGPRESENTATION	0x01
#define GCIS_PARM_CALLINGSCREENING		0x02
#define GCIS_PARM_CRNTYPE				0x03
#define GCIS_PARM_DIRECTORYNUMBER		0x04
#define GCIS_PARM_EVENTDATAP			0x05
#define GCIS_PARM_NETCRV				0x06
#define GCIS_PARM_RECEIVEINFOBUF		0x07
#define GCIS_PARM_SUBADDRESSNUMBER		0x08
#define GCIS_PARM_EXTENSIONEVENT		0x09
#define GCIS_PARM_GENERICCALLMODEL		0x0a

/* maximum size of directory number, subaddress number and spid number */
#define MAX_PARM_INFO_SIZE 0xFF

/* parm ids under set GCIS_SET_IE  */
/* GCIS_PARM_IEDATA :This parameter is used to pass information elements in 
 * GCIS_EXID_GETFRAME, GCIS_EXID_GETNONCALLMSG, GCIS_EXID_SNDFRAME, GCIS_EXID_SNDMSG, 
 * GCIS_EXID_SNDNONCALLMSG, gcis_SetUserInfo. Value type is char array, length should 
 * not exceed GCIS_MAXLEN_IEDATA.
 *
 * GCIS_PARM_UIEDATA :This parameter is used to pass unprocessed IEs from CCLIB to APP.
 * Value type is char array, length should not exceed GCIS_MAXLEN_IEDATA.
 */
#define GCIS_PARM_IEDATA			0x01
#define GCIS_PARM_UIEDATA			0x02

#define GCIS_MAXLEN_IEDATA	254

/* parm ids under set GCIS_SET_SERVREQ  */
/* GCIS_PARM_SERVREQ_CAUSEVALUE :Value type is unsigned char.
 *
 * GCIS_PARM_SERVREQ_INTERPRETER :Specifies how the usid and tid values are to be 
 * interpreted. Value type is unsigned char.
 *
 * GCIS_PARM_SERVREQ_TID :Terminal Identifier. The range is 01 - 63. 00 signifies that 
 * the firmware is to provide a default. Value type is unsigned char.
 *
 * GCIS_PARM_SERVREQ_USID :User Service Identifier. The range is 01 - FF. 00 signifies 
 * default. Value type is unsigned char.
 */
#define GCIS_PARM_SERVREQ_CAUSEVALUE	0x01
#define GCIS_PARM_SERVREQ_INTERPRETER	0x02
#define GCIS_PARM_SERVREQ_TID			0x03
#define GCIS_PARM_SERVREQ_USID			0x04

/* parm ids under set GCIS_SET_SNDMSG  */
/* GCIS_PARM_SNDMSGTYPE :Value type is int.
 */
#define GCIS_PARM_SNDMSGTYPE		0x01

/* parm ids under set GCIS_SET_TONE  */
/* GCIS_PARM_TONE_AMP1 :Specifies the amplitude of the tone. The range is -40 - +3 dB.
 * Value type is short.
 *
 * GCIS_PARM_TONE_AMP2 :Specifies the amplitude of the tone. The range is -40 - +3 dB.
 * Value type is short.
 *
 * GCIS_PARM_TONE_DURATION :Specifies the duration of the tone in 10 ms units. The range 
 * is 1 - 65535. Set to -1 to play forever.Value type is unsigned short.
 *
 * GCIS_PARM_TONE_FREQ1 :Specifies the frequency of the tone. The range is 200 - 3100 Hz.
 * Value type is unsigned short.
 *
 * GCIS_PARM_TONE_FREQ2 :Specifies the frequency of the tone. The range is 200-3100 Hz.
 * Value type is unsigned short.
 *
 * GCIS_PARM_TONE_OFF1 : Specifies the tone interval, in 10 ms units. The range is 
 * 0 - 65534 ms. Set to 0 to play a continuous tone.Value type is unsigned short.
 *
 * GCIS_PARM_TONE_ON1 :Specifies the tone interval, in 10 ms units. The range is 
 * 1 - 65535 ms. Set to 1 or greater for continuous tone.Value type is unsigned short.
 *
 * GCIS_PARM_TONE_TERMPARMLENGTH :Duration for which tone has to be played.Value type 
 * is unsigned short.
 */
#define GCIS_PARM_TONE_AMP1				0x01
#define GCIS_PARM_TONE_AMP2				0x02
#define GCIS_PARM_TONE_DURATION			0x03
#define GCIS_PARM_TONE_FREQ1			0x04
#define GCIS_PARM_TONE_FREQ2			0x05
#define GCIS_PARM_TONE_OFF1				0x06
#define GCIS_PARM_TONE_ON1				0x07
#define GCIS_PARM_TONE_TERMPARMLENGTH	0x08

/* Parm for GCIS_SET_CALLTYPE - Call type value: NCAS or CIRCUIT*/ 
#define GCIS_PARM_CALL_TYPE				0x01


/* set_ID definitions */

#define ISDN_SET_CFG					0x4000
#define ISDN_SET_GETEVTMSK				0x4001
#define ISDN_SET_PARM					0x4002
#define ISDN_SET_GENERIC				0x4003
#define ISDN_SET_CALL_INFO				0x4004
#define ISDN_SET_EVTMSK					0x4005
#define ISDN_SET_BEARER_CHNL			0x4006
#define ISDN_SET_FACILITY				0x4008
#define ISDN_SET_SERVICEREQ				0x4009

#define ISDN_DLINK_CFG					0x01
#define ISDN_DCHAN_CFG					0x02
#define ISDN_GET_EVTMSK					0x03
#define ISDN_DIRECTORY_NUMBER			0x04
#define ISDN_SPID_NUMBER				0x05
#define ISDN_SUBADDR_NUMBER				0x06
#define CC_VERSION						0x07
#define ISDN_MINDIGITS					0x08
#define ISDN_STATUS_EVTMSK				0x09
#define ISDN_STATUS_ENQUIRY_EVTMSK		0x0a
#define ISDN_TERMINATE_EVTMSK			0x0b
#define ISDN_TMREXPEVENT_EVTMSK			0x0c
#define ISDN_TRANSFER_MODE				0x0d
#define ISDN_TRANSFER_RATE				0x0e
#define ISDN_INFO_ELEMENTS				0x0f
#define ISDN_FACILITY_FEATURE_SERVICE	0x10
#define ISDN_FACILITY_CODING_VALUE		0x11

/* parameters under ISDN_SET_SERVICEREQ */

#define ISDN_SAPI						0x01
#define ISDN_CES						0x02
#define ISDN_ACK_TYPE					0x03
#define ISDN_CAUSE_VALUE				0x04
#define ISDN_USID						0x05
#define ISDN_TID						0x06
#define ISDN_INTERPRETER				0x07
#define ISDN_INITIALIZING_TERM			0x08
#define ISDN_SPID						0x09


/* Extension ids for extension events */
#define GCIS_EXEV_CONFDROP			0x01
#define GCIS_EXEV_CONGESTION		0x02
#define GCIS_EXEV_DIVERTED			0x03
#define GCIS_EXEV_DROPACK			0x04
#define GCIS_EXEV_DROPREJ			0x05
#define GCIS_EXEV_FACILITY			0x06
#define GCIS_EXEV_FACILITY_ACK		0x07
#define GCIS_EXEV_FACILITY_REJ		0x08
#define GCIS_EXEV_FACILITYGLOBAL	0x09
#define GCIS_EXEV_FACILITYNULL		0x10
#define GCIS_EXEV_INFOGLOBAL		0x11		
#define GCIS_EXEV_INFONULL			0x12
#define GCIS_EXEV_L2BFFRFULL		0x13
#define GCIS_EXEV_L2FRAME			0x14
#define GCIS_EXEV_L2NOBFFR			0x15
#define GCIS_EXEV_NOFACILITYBUF		0x16
#define GCIS_EXEV_NOTIFY			0x17
#define GCIS_EXEV_NOTIFYGLOBAL		0x18
#define GCIS_EXEV_NOTIFYNULL		0x19
#define GCIS_EXEV_NOUSRINFOBUF		0x20
#define GCIS_EXEV_NSI				0x21
#define GCIS_EXEV_PLAYTONE			0x22
#define GCIS_EXEV_PLAYTONEFAIL		0x23
#define GCIS_EXEV_PROGRESSING		0x24
#define GCIS_EXEV_STATUS			0x25
#define GCIS_EXEV_STATUS_ENQUIRY	0x26
#define GCIS_EXEV_STOPTONE			0x27
#define GCIS_EXEV_STOPTONEFAIL		0x28
#define GCIS_EXEV_TIMER				0x29
#define GCIS_EXEV_TONEREDEFINE		0x30
#define GCIS_EXEV_TONEREDEFINEFAIL	0x31
#define GCIS_EXEV_TRANSFERACK		0x32
#define GCIS_EXEV_TRANSFERREJ		0x33
#define GCIS_EXEV_TRANSIT			0x34
#define GCIS_EXEV_USRINFO			0x35
#define GCIS_EXEV_L2QWARNING		0x36
#define GCIS_EXEV_L2QOVERFLOW		0x37
#define GCIS_EXEV_L2QCLEAR			0x38
#define GCIS_EXEV_DIALING			0x39
#define GCIS_EXEV_SNU				0x40
#define GCIS_EXEV_SU				0x41
#define GCIS_EXEV_EEM				0x42

/* Values under Parm GCIS_PARM_CALL_TYPE*/
#define CALLTYPE_NCAS					0x1
#define CALLTYPE_CIRCUIT				0x2


#ifdef WIN32
#ifdef VME_SPAN
#else
#pragma pack()
#endif
#endif /* WIN32 */

#endif /* GCISDN_H_INCLUDED */

