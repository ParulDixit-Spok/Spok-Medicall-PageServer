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
 * Filename:     isdndef.h
 * DESCRIPTION : This file contains the error codes for the ISDN library.
 *   
 ****************************************************************************/

#ifndef __ISDNDEF_H__
#define __ISDNDEF_H__

/*
 * Non-Call state related message
 */
#define SndMsg_Progress        0x03
#define SndMsg_Information     0x0F
#define SndMsg_Congestion      0x0B
#define SndMsg_UsrInformation  0x07 
#define SndMsg_Facility        0x08
#define SndMsg_FacilityACK     0x09
#define SndMsg_FacilityREJ     0x0A
#define SndMsg_Notify          0x0C

#define SndMsg_Divert          0x10
#define SndMsg_Intrude         0x11
#define SndMsg_NSI             0x12
#define SndMsg_Transfer        0x13
#define SndMsg_Transit         0x14

#define SndMsg_Drop            0x15
#define SndMsg_DropAck         0x16
#define SndMsg_DropRej         0x17
#define SndMsg_Redirect        0x18

#define SndMsg_ServiceAck      0x19	
#define SndMsg_Status          0x20	
#define SndMsg_StatusEnquiry   0x21	
#define SndMsg_GlobalStatus    0x22

#define SndMsg_SNU             0x23
#define SndMsg_SU              0x25
#define SndMsg_RawEEM          0x26

/*
 * Single-byte Codeset 0 information elements
 */
#define CODESET_SHIFT_IE         0x90  /* Codeset shift - Type 1 */
#define MORE_DATA_IE             0xA0  /* More data - Type 2 */
#define SENDING_COMPLETE_IE      0xA1  /* Send complete - Type 2 */
#define CONGEST_LEVEL_IE         0xB0  /* Congestion level - Type 1 */
#define REPEAT_IND_IE            0xD0  /* Repeat indicator - Type 1 */   

/*                                    
 * Multi-byte Codeset 0 information elements
 */
#define SEGMENTED_MSG_IE         0x00  /* Segmented message */
#define CHANGE_STATUS_IE         0x01  /* Change status maint. ie */
#define BEARER_CAPABILITY_IE     0x04  /* Bearer capability */
#define CAUSE_IE                 0x08  /* Cause elem */
#define CONNECTED_NUM_IE         0x0C  /* Connected number */

#define CALL_IDENT_IE            0x10  /* Call identity */
#define CALL_STATE_IE            0x14  /* Call state */
#define CHANNEL_ID_IE            0x18  /* Channel identification */
#define FACILITY_IE              0x1C  /* Facility element */
#define PROGRESS_IND_IE          0x1E  /* Progress indicator */

#define NETWORK_FACILITY_IE      0x20  /* Network-specific facility */
#define NATIONAL_FACILITY_IE     NETWORK_FACILITY_IE      /* VN3 element */
#define ENDPOINT_ID_IE           0x26  /* Endpoint Identification */
#define NOTIFY_IND_IE            0x27  /* Notification indicator */
#define DISPLAY_IE               0x28  /* Display element */
#define TIME_DATE_IE             0x29  /* Time/Date element */
#define KEYPAD_FACILITY_IE       0x2C  /* Keypad facility */

/*
 * Values used in CHAN_STATUS message.
 */
#define CHAN_STATUS_IE           0x30  /* Channel status */
#define CALL_STATUS_IE           0x2d  /* Call status by HSS:Pkg5_R1_xx*/

/*
 * Values used in STATUS_UPDATE message
 */
#define UPDATE_IE                0x31  /* update info */

#define CONGESTION_IE            0x31  /* Channel status */

#define INFO_REQ_IE              0x32  /* Information request */
#define SIGNAL_IE                0x34  /* Signal element */
#define SWITCHHOOK_IE            0x36  /* Switchhook element */
#define FEATURE_ACTIV_IE         0x38  /* Feature activation */
#define FEATURE_IND_IE           0x39  /* Feature indication */

#define INFO_RATE_IE             0x40  /* Information rate */
#define TRANSIT_DELAY_EE_IE      0x42  /* End to end transit delay */
#define TRANSIT_DELAY_SEL_IE     0x43  /* Transit delay select & indic */
#define PACKET_BIN_PARMS_IE      0x44  /* Packet layer binary parameters */
#define PACKET_WINDOW_IE         0x45  /* Packet layer window size */
#define PACKET_SIZE_IE           0x46  /* Packet size */

#define CALLER_NUMBER_IE         0x6C  /* Calling party number */
#define CALLER_SUB_ADDR_IE       0x6D  /* Calling party sub-address */

#define CALLED_NUMBER_IE         0x70  /* Called party number */
#define CALLED_SUB_ADDR_IE       0x71  /* Called party sub-address */
#define ORIGINAL_CALLED_NUM_IE   0x73  /* Original Called Number */
#define REDIRECTING_NUM_IE       0x74  /* Redirecting number */
#define REDIRECTION_NUM_IE       0x76  /* Redirection number */
#define TRANSIT_NETWORK_SEL_IE   0x78  /* Transit network selection */
#define RESTART_IND_IE           0x79  /* Re-start indication */
#define USER_USER_FAC_IE         0x7A  /* User-to-user facility */
#define LOW_LAYER_COMPAT_IE      0x7C  /* Low layer compatibility */
#define HIGH_LAYER_COMPAT_IE     0x7D  /* High layer compatibility */
#define USER_USER_IE             0x7E  /* User-to-User */
#define ESCAPE_FOR_EXT_IE        0x7F  /* Escape for extension */

#define LOCKING_SHIFT_TO_6       0x96  /* for VerifyIEs function */
#define CODESET_6_UNLOCKED       0x86  /* for VerifyIEs function */
#define CODESET_6_LOCKED         0x06  /* for VerifyIEs function */

#define ADVICE_OF_CHARGE_IE      0x01  /* NTT-KDD advice of charge */

/*
 *     Calling party number element
 */
#define ISDN_PI_RESTRICT         0x01  /* presentation indicator - restricted */
#define ISDN_PI_NOTAVAIL         0x02  /* presentation indicator - not avail */
#define ISDN_PI_RESRV            0x03  /* presentation indicator - reserved */
#define ISDN_SI_USRVERSCRN       0x01  /* screening indicator - verified and passed */
#define ISDN_SI_USRVERFAIL       0x02  /* screening indicator - verified and failed */
#define ISDN_SI_NET              0x03  /* screening indicator - network provided */

/*
 *     Congestion Level element
 */
#define ISDN_RCV_READY           0x00  /* ready to receive more user to user info */
#define ISDN_RCV_NOT_READY       0x01  /* not ready to receive more user to user info */

/*
 *     Network-Specific facility element
 */
#define ISDN_FEATURE             0x00  /* feature/service - feature */
#define ISDN_SERVICE             0x01  /* feature/service - service */
#define ISDN_CPN_PREF            0x01  /* facility coding - CPN perferred */
#define ISDN_BN_PREF             0x02  /* facility coding - BN preferred */
#define ISDN_CPN                 0x03  /* facility coding - CPN */
#define ISDN_BN                  0x04  /* facility coding - BN */
                                 
#define ISDN_SDN                 0x01  /* service coding - SDN */
#define ISDN_MEGACOM800          0x02  /* service coding - MEGACOM 800 */
#define ISDN_MEGACOM             0x03  /* service coding - MEGACOM */
#define ISDN_WATS                0x04  /* service coding - WATS */
#define ISDN_TIE                 0x05  /* service coding - TIE */
#define ISDN_ACCUNET             0x06  /* service coding - ACCUNET SDS */
#define ISDN_LONG_DS             0x07  /* service coding - Long distance */
#define ISDN_INT_800             0x08  /* service coding - internation 800 */
#define ISDN_CA_TSC              0x09  /* service coding - CA TSC */
#define ISDN_ATT_MULTIQ          0x10  /* service coding - ATT MultiQuest */
#define ISDN_TNID_USER           0x00  /* network id type - user specified */
#define ISDN_TNID_NAT            0x02  /* network id type - national */
#define ISDN_TNID_INTER          0x03  /* network id type - international */
#define ISDN_NIDPLN_UNK          0x00  /* network id plan - unknown */
#define ISDN_NIDPLN_CIC          0x01  /* network id plan - carrier id code */
#define ISDN_NIDPLN_DNIC         0x03  /* network id plan - datanetwork id co */

#define ISDN_ITM_CIRCUIT         0x00  /* info transfer mode - circuit mode */
#define ISDN_ITM_PACKET          0x02  /* info transfer mode - packet mode */

/*                                 
 * Information Transfer Rate
 */
#define BEAR_RATE_64KBPS         0x10  /* B_CHANNEL_UNITS 1X64 */
#define BEAR_RATE_128KBPS        0x11  /* Non-standard 2X64       */
#define BEAR_RATE_384KBPS        0x13  /* H0_CHANNEL_UNITS 6X64   */
#define BEAR_RATE_1536KBPS       0x15  /* H11_CHANNEL_UNITS 24X64 */
#define BEAR_RATE_1920KBPS       0x17  /* H12_CHANNEL_UNITS 30X64 */

/*
 * Presentation indicators
 */
#define PRESENTATION_ALLOWED     0x00  /* Default */

/*
 * Screening indicators
 */
#define USER_PROVIDED            0x00  /* Default */

/*
 * CAUSE_IE
 */
#define CAUSE_LENGTH             0x02  /* Internal length = 2 */
#define DIAG_LEN                 0x03  /* Maximum diagnostic length */
#define NO_DIAG                  0x00  /* No CAUSE diagnostic specified */


/*
 * CHANNEL_ID_IE
 */
#define  ISDN_CHAN_EXCLUSIVE     0x01

/*
 * B-Channel choices for the Channel Identification IE.
 */
#define B_CHANNEL_UNITS          3     /* Chan type -   single B channel */
#define B_CHANNEL                B_CHANNEL_UNITS     /* Borrow the ID */
#define CHANNEL_1X64             B_CHANNEL           /* BEAR_RATE_64KBPS */

/*
 * CONGEST_LEVEL_IE
 */
#define NOT_CONGESTED            0     /* Level = NOT_CONGESTED */
#define CONGESTED                0x0F  /* Level = CONGESTED */

/*
 * Coding Standards 
 */

#define CCITT_STANDARD            0    /* Coding standard */
#define CEPT_NATIONAL_STANDARD    2    /* CEPT/National standard */

#define NATIONAL_INTERPRETATION   0    /* Interpretation value */
#define CCITT_CEPT_INTERPRETATION 4    /* Interpretation value */
#define PROFILE_PRESENTATION      1    /* Presentation value */

/*
 * CCITT Teleservices
 */
#define TELEPHONY_SERVICE        0x01  /* Teleservice */
#define GR3_FAX_SERVICE          0x04  /* Teleservice */
#define TELETEX_MIXED_SERVICE    0x24  /* Teleservice */
#define GR4_FAX_SERVICE          0x21  /* Teleservice */
#define TELETEX_CHAR_SERVICE     0x31  /* Teleservice */
#define ALPHA_MOS_VID_SERVICE    0x32  /* Teleservice */
#define TELEX_SERVICE            0x35  /* Teleservice */
#define MHS_SERVICE              0x38  /* Teleservice */
#define OSI_APPL_SERVICE         0x41  /* Teleservice */
                                 
/*
 * CEPT Teleservices
 */
#define UNKNOWN_SERVICE          0x00  /* Teleservice (interworking) */

/*
 * National Standard Teleservices
 */
#define AUDIOSCRIPT_SERVICE      0x40  /* Teleservice */
#define AUDIOVIDEOTEX_SERVICE    0x48  /* Teleservice */
#define NON_STD_APPL_SERVICE     0x7F  /* Teleservice */


/*
 * PROGRESS_IND_IE 
 */

/*
 * Progress values
 */
#define CALL_NOT_END_TO_END_ISDN 1     /* Progress indicator value */
#define IN_BAND_INFO             8     /* Progress indicator value */


/*
 * USER_USER_IE 
 */
#define USER_SPEC_PROTO          0x00  /* User-specific protocol */
#define OSI_HIGH_LAYER_PROTO     0x01  /* OSI high layer protocol */
#define X244_PROTO               0x02  /* X.244 protocol */
#define MAINT_MGMT_PROTO         0x03  /* Maintenance/management protocol */
#define IA5_CHAR_PROTO           0x04  /* IA5 character protocol */
#define V120_RATE_ADPT_PROTO     0x07  /* V.120 rate adaption protocol */
#define Q931_USER_NET_PROTO      0x08  /* Q.931 user-network protocol */
#define NT2_NT2_FAC_PROTO        0x40  /* VN2 NT2-NT2 facilities protocol */
#define MINI_MSG_SERV_PROTO      0x41  /* VN2 mini-msg handling protocol */
#define MAX_UUI_LEN              131   /* A network-dependent value */

/*
 * "update" values
 */
#define DATA_LINK_DOWN           0x01  /* CC->Host data link failure report */
#define DATA_LINK_UP             0x02  /* CC->Host data link restore report */
#define EST_DATA_LINK            0x03  /* Host->CC data link restore request */
#define REL_DATA_LINK            0x04  /* Host->CC data link restore request */
#define DATA_LINK_RESET          0x05  /* CC->Host data link reset for DASS2 */
#define DATA_LINK_DISABLED		0x00

#define AUTO_TEI				0x7F
#define DATA_LINK_PROTOCOL_Q931	0x01
#define DATA_LINK_PROTOCOL_X25	0x02

/*
 * vari-a-bill
 */
#define FLEX_BILLING_AVAILABLE      0x01
#define FLEX_BILLING_UNAVAILABLE    0x02
#define FLEX_BILLING_ACCEPTED       0x03
#define FLEX_BILLING_BAD_OPERATION  0x04
#define FLEX_BILLING_BAD_ARGUMENT   0x05
#define FLEX_BILLING_RETURN_ERR     0x06
#define FLEX_BILLING_IE_ERR         0x07
#define NO_FLEX_BILLING_INFO        0x08

/*
 * definitions for flex billing billing type field
 */
#define BILLING_NEW_RATE            0x10
#define BILLING_FLAT_RATE           0x11
#define BILLING_PREMIUM_CHARGE      0x12
#define BILLING_PREMIUM_CREDIT      0x13
#define BILLING_FREE_CALL           0x18
/*
 * DPNSS_IE SINGLE BYTE IE
 */
#define BUSY_IE                     0xE0     /* Busy IE */
#define DIVERSION_BYPASS_IE         0xE1     /* Diversion bypass IE */
#define ENQUIRY_IE                  0xE2     /* Enquiry IE */
#define VIRTUALCALL_IE              0xE3     /* Virtual call IE */
#define DIVERSION_VALIDATION_IE     0xE4     /* Diversion validation IE */
#define	DPNSS_SIMULATED_ANSWER_IE		0xE6     
#define	DPNSS_EXPLICIT_CALL_TRANSFER_IE	0xE7	 

/*
 * DPNSS_IE MULITIPLE BYTES IE
 */
#define DIVERSION_IE                0x58     /* Diversion IE */
#define INTRUDE_IE                  0x59     /* Intrude IE */
#define INTRUSION_IE                0x5A     /* Intrusion IE */
#define NSI_IE                      0x5B     /* NSI IE */
#define TRANSFER_IE                 0x5C     /* Transfer IE */
#define TRANSIT_IE                  0x5D     /* Transit IE */
#define EXTENSION_STATUS_IE         0xE5     /* Extension Status IE */
#define TEXT_DISPLAY_IE             0x5E     /* Text Display IE */
#define SNU_IE                      0x51     /* SNU IE */
#define SU_IE                       0x53     /* SU IE */

/*
 *     Diversion Information Element
 */
#define DIVERT_IMMEDIATE            0x01
#define DIVERT_ON_BUSY              0x02
#define DIVERT_NO_REPLY             0x03

#define DIVERT_LOCAL                0x01
#define DIVERT_REMOTE               0x02

/*
 *     Intrusion Information Element
 */
#define INTRUDE_PRIOR_VALIDATION    0x01
#define INTRUDE_NORMAL              0x02

#define INTRUSION_LEVEL_1           0x01
#define INTRUSION_LEVEL_2           0x02
#define INTRUSION_LEVEL_3           0x03

/*
 *     Intrude Information Element
 */
#define INTRUDE                     0x01
#define INTRUDE_WITHDRAW            0x02

/*
 *     Transfer Information Element
 */
#define TRANSFER_ORIG               0x01
#define TRANSFER_TERM               0x02

/*
 *     NSI Information Element
 */
#define NSI_EEM                     0x01
#define NSI_LLM                     0x02

/*     SNU and SU Information Element  */
#define SNU_EEM                     0x01
#define SU_EEM                      0x01

/*
 *     Text Display Information Element
 */
#define TEXT_TYPE_NOT_PRESENT		0x00
#define TEXT_TYPE_NAME			   0x01
#define TEXT_TYPE_MESSAGE		   0x02
#define TEXT_TYPE_REASON		   0x03


typedef enum {
   ISDN_INVALID_SWITCH=0x80,
   ISDN_BRI_5ESS,
   ISDN_BRI_DMS100,
   ISDN_BRI_NTT,
   ISDN_BRI_NET3,
   ISDN_BRI_NI1,
   ISDN_BRI_NI2
   } IsdnSwitchType;

#ifndef MAX_DN_SIZE
#define MAX_DN_SIZE              (16+1)
#endif

#ifndef MAX_SPID_SIZE
#define MAX_SPID_SIZE            (20+1)
#endif

#define USER_SIDE                0        /* User side of ISDN protocol */
#define NETWORK_SIDE             1        /* Network side of ISDN protocol */

#define LAYER_2_ONLY             0        /* ISDN access @ layer 2 */
#define FULL_ISDN_STACK          1        /* ISDN access @ L3 call control */

#define AUTO_INIT_TERMINAL       0        /* Auto Initializing terminal */
#define NON_INIT_TERMINAL        1        /* Non Initilizing Terminal   */

#define AUTO_TEI_TERMINAL        0        /* auto TEI assigning Terminal */
#define FIXED_TEI_TERMINAL       1        /* Non-auto TEI assigning term.*/

#define TMR_DEFAULT              0        /* Use default values of timers */
#define TMR_DISABLE              -1       /* Do not use the timer */

#define USE_DEFAULT_USID_TID     0        /* Use values by Telnetworks */ 
#define MATCH_USID_AND_TID       0        /* Match Usid and Tid */
#define MATCH_USID_BUT_NOT_TID   1        /* match usid but tid shouldn't
                                              match */

/*
 * BIT MASKS FOR D CHANNEL CONFIGURATION FEATURE CONTROL BYTE 
*/
#define SENDING_COMPLETE_ATTACH  0x04     /* Add SENDING_COMPLETE IE to SETUP message */
#define HOST_CONTROLLED_RELEASE  0x10     /* Delay RELEASE reply until host issues cc_ReleaseCall */
#define USER_PERST_L2_ACT	 0x08	  /* Persistent L2 activation on user side */

/*
 * BIT MASKS FOR D CHANNEL CONFIGURATION FEATURE CONTROL BYTE 
*/
#define NO_PCM_TONE         0x00     /* Disable automatic tones, but set encoding by switch type. */
#define ULAW_PCM_TONE	    0x01     /* Provide tones and use ULAW encoding for B chan tones */
#define ALAW_PCM_TONE	    0x02     /* Provide tones and use ALAW encoding for B chan tones */
#define DEFAULT_PCM_TONE    0x03     /* Provide tones and use default encoding for B chan tones, by switchtype. */
#define PCM_LAW_MASK        0x03     /* Bit mask for PCM settings */

/* 
 * Type of CRNs in sending Non-Call Related Messages
 */
#define GLOBAL_CRN            0x00
#define NULL_CRN              0x01

#endif /* __ISDNDEF_H__ */


