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
 * Filename:    cclib.h 
 * DESCRIPTION: Call Control function Library. Include Event Code, 
 *              MakeCall Block, and function prototype.   
 ****************************************************************************/
 
#ifndef __CCLIB_H__

#define __CCLIB_H__

#include "isdncmd.h"
/* gc21 */
#include "isdndef.h" /*DNJ: File name changed from isdn.h (JS)*/

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

/*
 * The following two types are needed by the files included below.
 */
#ifndef LINEDEV
#define LINEDEV            int
#endif

#ifndef CRN
#define CRN                long
#endif

/*
 * defines for CCAPI event codes
 */
#ifndef DOS
#define DT_CC              0x800
#else
#define DT_CC              0x2000
#endif
#define CCEV_TASKFAIL      (DT_CC | 0x01) /* abnormal condition state
                                             unchanged */
#define CCEV_ANSWERED      (DT_CC | 0x02) /* call answered and connected */
#define CCEV_ACCEPT        (DT_CC | 0x04) /* call is accepted */
#define CCEV_DROPCALL      (DT_CC | 0x05) /* cc_DropCall is completed */
#define CCEV_RESTART       (DT_CC | 0x06) /* cc_Restart is completed */
#define CCEV_CALLINFO      (DT_CC | 0x07) /* Info message received */
#define CCEV_REQANI        (DT_CC | 0x08) /* cc_ReqANI() is completed */
#define CCEV_SETCHANSTATE  (DT_CC | 0x09) /* cc_SetChanState() is completed */
#define CCEV_FACILITY_ACK  (DT_CC | 0x0A) /* cc_facilityreq() is
                                             completed and successfully */
#define CCEV_FACILITY_REJ  (DT_CC | 0x0B) /* cc_facilityreq() is completed
                                             but rejected, use cc_CauseValue 
                                             to get the reason */
#define CCEV_MOREDIGITS    (DT_CC | 0x0C) /* cc_moredigits() is completed*/
#define CCEV_SETBILLING    (DT_CC | 0x0E) /* cc_SetBilling() is completed*/
#define CCEV_ALERTING      (DT_CC | 0x21) /* The destination telephone
                                             terminal equipment has received
                                             connection request (in ISDN acc-
                                             epted the connection request. This
                                             event is an unsolicited event) */
#define CCEV_CONNECTED     (DT_CC | 0x22) /* The destination answered the
                                             request */
#define CCEV_ERROR         (DT_CC | 0x23) /* unexpected error event */
#define CCEV_OFFERED       (DT_CC | 0x24) /* a connection request has
                                             been made */
#define CCEV_DISCONNECTED  (DT_CC | 0x26) /* remote disconnected*/
#define CCEV_PROCEEDING    (DT_CC | 0x27) /* the call state has been
                                             changed to the proceeding state */
#define CCEV_PROGRESSING   (DT_CC | 0x28) /* a call progress message has
                                             been received */
#define CCEV_CALLPROGRESS   (DT_CC | 0x28) /* a call progress message has
                                             been received */
#define CCEV_USRINFO       (DT_CC | 0x29) /* a user to user information
                                             event is coming */
#define CCEV_FACILITY      (DT_CC | 0x2A) /* a facility request is made
                                             by CO */
#define CCEV_CONGESTION    (DT_CC | 0x2B) /* remote end is not ready to
                                             accept incoming user information */
#define CCEV_D_CHAN_STATUS (DT_CC | 0x2E) /* report D-channel status to
                                             the user */
#define CCEV_NOUSRINFOBUF  (DT_CC | 0x30) /* User information element
                                             buffer is not ready */
#define CCEV_NOFACILITYBUF (DT_CC | 0x31) /* Facility buffer is not ready*/
#define CCEV_BLOCKED       (DT_CC | 0x32)
#define CCEV_UNBLOCKED     (DT_CC | 0x33)

#define CCEV_ISDNMSG       (DT_CC | 0x34) /* An "information" message or unde- 
                                             fined massage has been received */
#define CCEV_NOTIFY        (DT_CC | 0x35) /* A Notify message has been 
                                             received */
#define CCEV_L2FRAME       (DT_CC | 0x36) /* A layer 2 access message has been
                                             received */
#define CCEV_L2BFFRFULL    (DT_CC | 0x37) /* reserved for future use */
#define CCEV_L2NOBFFR      (DT_CC | 0x38) /* Failed to get a free space for an
                                             incoming layer 2 access message */
#define CCEV_SETUP_ACK     (DT_CC | 0x39) /* CALL_SETUP_ACK message has been received */
#define CCEV_CALLWAITING_OFFERED   (DT_CC | 0x3A) /* HSS:FR-418 */ /* CALL_WAITING_INCOMING message has been received */  
#define CCEV_CALLWAITING_ACCEPT     (DT_CC | 0x3B) /* HSS:FR-418 */ /* CALL_WAITING_ACCEPT message  */  

#define CCEV_DIVERTED      (DT_CC | 0x40)
#define CCEV_HOLDACK       (DT_CC | 0x41)
#define CCEV_HOLDCALL      (DT_CC | 0x42)
#define CCEV_HOLDREJ       (DT_CC | 0x43)
#define CCEV_RETRIEVEACK   (DT_CC | 0x44)
#define CCEV_RETRIEVECALL  (DT_CC | 0x45)
#define CCEV_RETRIEVEREJ   (DT_CC | 0x46)
#define CCEV_NSI           (DT_CC | 0x47)
#define CCEV_TRANSFERACK   (DT_CC | 0x48)
#define CCEV_TRANSFERREJ   (DT_CC | 0x49)
#define CCEV_TRANSIT       (DT_CC | 0x4A)
#define CCEV_RESTARTFAIL   (DT_CC | 0x4B) /* abnormal condition state */
#define CCEV_TERM_REGISTER (DT_CC | 0x4C) /* BRI msg - An action is required for the
                                             North Am. terminal initialization procedure */
#define CCEV_RCVTERMREG_ACK  (DT_CC | 0x4E) /* Positive acknowledgement of cc_TermRegisterResponse */
#define CCEV_RCVTERMREG_NACK (DT_CC | 0x4F) /* Negative acknowledgement of cc_TermRegisterResponse */
#define CCEV_INFONULL        (DT_CC | 0x50) /* NULL (Dummy) CRN Information IE */

#define CCEV_TONEREDEFINEFAIL      (DT_CC | 0x51)
#define CCEV_TONEREDEFINE          (DT_CC | 0x52)
#define CCEV_PLAYTONEFAIL          (DT_CC | 0x53)
#define CCEV_PLAYTONE              (DT_CC | 0x54)
#define CCEV_STOPTONEFAIL          (DT_CC | 0x55)
#define CCEV_STOPTONE              (DT_CC | 0x56)

#define CCEV_NOTIFYNULL      (DT_CC | 0x57) /* NULL (Dummy) CRN Notify IE */
#define CCEV_REDIRECT        (DT_CC | 0x58) /* ISDN REDIRECT message */
#define CCEV_RELEASECALL     (DT_CC | 0x59) /* Asynchronous cc_ReleaseCall() Completion event */
#define CCEV_RELEASECALLFAIL (DT_CC | 0x5A) /* Asynchronous cc_ReleaseCall() fail event */

#define CCEV_FACILITYNULL    (DT_CC | 0x5B) /* NULL (Dummy) CRN Facility IE */
#define CCEV_INFOGLOBAL      (DT_CC | 0X5C) /* GLOBAL CRN Information IE */
#define CCEV_NOTIFYGLOBAL    (DT_CC | 0X5D) /* GLOBAL CRN Notify IE */
#define CCEV_FACILITYGLOBAL  (DT_CC | 0x5E) /* GLOBAL CRN Facility IE */
#define CCEV_DROPACK         (DT_CC | 0x5F) /* DROP Request Acknowledgement message */
#define CCEV_DROPREJ         (DT_CC | 0x60) /* DROP Request Reject message */
#define CCEV_CONFDROP		 (DT_CC | 0x61) /* Request to drop the call*/

#define CCEV_STATUS_ENQUIRY	(DT_CC | 0x62) 
#define CCEV_STATUS			(DT_CC | 0x63)
#define CCEV_SERVICE		(DT_CC | 0x64)
#define CCEV_TIMER           (DT_CC | 0x65) /* Timer Expiry Event*/
#define CCEV_REGISTER_ACK	(DT_CC | 0x66)		/* Register Msg ACK from FW */

#define CCEV_L2QWARNING	    (DT_CC | 0x67)		/* HDLC Queue warning message */
#define CCEV_L2QOVERFLOW	(DT_CC | 0x68)		/* HDLC Queue overflow message */
#define CCEV_L2QCLEAR   	(DT_CC | 0x69)		/* HDLC Queue clear message */
#define CCEV_CALLREJECT     (DT_CC | 0x70) /*Call has been rejected by the firmware */
#define CCEV_DIALING        (DT_CC | 0x71) /*Setup msg has been sent by firmware  */

/* HSS:FR385...7/30/2003.....Starts */
#define CCEV_SNU    (DT_CC | 0x72)   /*CALL_SNU msg has been sent by firmware  */
#define CCEV_SU     (DT_CC | 0x73)   /*CALL_SU msg has been sent by firmware  */
#define CCEV_EEM    (DT_CC | 0x74)   /*CALL_EEM msg has been sent by firmware  */
/* HSS:FR385...7/30/2003.....Ends */


// 03/27/07 Use 0x3F00 for base event value going forward.
#define  CCEV_BASE			0x3F00				// beginning base value
#define  CCEV_LAST_EVENT	(CCEV_BASE | 0xFF)	// Last event value

															
/*
 * BUFFER sizes
 */
#define CC_ADDRSIZE        0x30 
#define CC_BILLSIZE        0x30

/*
 *      Cause definitions for dropping a call
 */
#define CC_NORMAL_CLEARING              0x10
#define CC_CHANNEL_UNACCEPTABLE         0x06
#define CC_USER_BUSY                    0x11
#define CC_CALL_REJECTED                0x15
#define CC_DEST_OUT_OF_ORDER            0x19
#define CC_REQ_CHANNEL_NOT_AVAIL        0x2c

/*
 *   MASK for cc_MakeCall to keep the call valid even after timeout in SYNC mode
 */

#define CC_TMO_RELEASE  0x01

/*
 * RATE types
 */
#define CCR_CHARGE         0x0000      /* charge call (default)            */
#define CCR_NOCHARGE       0x0100      /* do not charge call               */


/*
 * STATES
 */
#define CCST_NULL          0
#define CCST_OFFERED       1
#define CCST_ACCEPTED      2
#define CCST_CONNECTED     3
#define CCST_DIALING       4
#define CCST_ALERTING      6
#define CCST_DISCONNECTED  7
#define CCST_IDLE          8

/*****************************************************************************
*                  Event Mask Action value
*****************************************************************************/
#define CCACT_SETMSK 0x01   /* Enable notification of events specified in
                               bitmask and disable notification of previously
                               set events. */
#define CCACT_ADDMSK 0x02   /* Enable notification of events specified in
                               bitmask in addition to previously set events.*/
#define CCACT_SUBMSK 0x03   /* Disable notification of events specified
                               in bitmask */

/* bit mask for ISDN_SETMM, ISDN_GETMM, and ISDN_RETMM commands */
#define CCMSK_ALERT            ISDN_CALL_ALERT          /* 0x01 */
#define CCMSK_PROCEEDING       ISDN_CALL_PROC           /* 0x04 */
#define CCMSK_PROGRESS         ISDN_CALL_PROG           /* 0x08 */
#define CCMSK_SETUP_ACK        ISDN_CALL_SETUP_ACK      /* 0x20 */
#define CCMSK_TERMINATE        ISDN_RELEASE             /* 0x40 */
#define CCMSK_CALLACK_SEND     ISDN_CALL_PROC_SEND      /* 0x80 */
#define CCMSK_PROC_SEND        ISDN_CALL_PROC_SEND      /* 0x80 */

#define CCMSK_STATUS		   ISDN_INCOMMING_STATUS	
#define CCMSK_STATUS_ENQUIRY   ISDN_INCOMMING_STATUS_ENQUIRY
#define CCMSK_SERVICE		   ISDN_INCOMMING_SERVICE_MSG	
#define CCMSK_SERVICE_ACK	   ISDN_INCOMMING_SERVICE_ACK	
#define CCMSK_TMREXPEVENT	   ISDN_TMREXPEVENT	            

#define CCMSK_CALLREJECT_ENABLE ISDN_INCALLREJECT_ENABLE /*0x2000*/
#define CCMSK_DIALING_ENABLE    ISDN_DIALING_ENABLE /*0x4000*/
#define CCMSK_SERVICE_NOTIFICATION ISDN_SERVICE_NOTIFICATION /*0x8000*/


#ifdef VME_SPAN
#else
#pragma pack(1)
#endif 

#ifndef RATE_DEFINED
typedef union {
   struct ATT {
      long cents;
   } att;
} CC_RATE_U;
#define RATE_DEFINED
#endif

typedef struct {
   unsigned char length;    /* protocol_discriminator + usrinformation length */
   unsigned char protocol_discriminator;
   char usrinformation[256];
} USRINFO_ELEM, *USRINFO_ELEM_PTR;

#define CC_MAXNFACNETWORKID 251
typedef struct {
   unsigned char feature_service;
   /*  
       ISDN_FEATURE - requested facility is a feature
       ISDN_SERVICE - requested facility is a service 
   */

   unsigned char facility_code;  
   /* 
       ISDN_CPN_PREF   - CPN preferred
       ISDN_SDN        - SDN
       ISDN_BN_PREF    - BN preferred
       ISDN_ACCUNET    - ACCUNET SDS
       ISDN_LONG_DIS   - long distance
       ISDN_INT_800    - international 800
       ISDN_CA_TSC     - CA TSC
       ISDN_ATT_MULTIQ - ATT multiquest 
   */

   unsigned char network_id_type; 
   /* 
       ISDN_TNID_USER  - user specified
       ISDN_TNID_NAT   - national
       ISDN_TNID_INTER - internation
   */

   unsigned char network_id_plan; 
   /* 
       ISDN_NIDPLN_UNK  - unknown
       ISDN_NIDPLN_CIC  - carrier identification code
       ISDN_NIDPLN_DNIC - datanetwork identification code 
   */

   unsigned char network_id_length; 
   /*  Length of the next field. If the
       value is 0, then the local serving network is assumed, and the setting
       of network_id_type and network_id_plan are omitted. 
   */

   unsigned char network_id[CC_MAXNFACNETWORKID]; 
   /* network ID */
} NFACILITY_ELEM, *NFACILITY_ELEM_PTR;


#define MAXPHONENUM     32

typedef struct channel_id{ 
	unsigned char channel;       /* inidicated channel */
	unsigned char channel_mode;  /* channel mode preferref/exclusive */
	short rfu;          /* reserved for future use */
}CHAN_ID, *CHAN_ID_PTR ;                                                      

typedef union u{
  struct is {
    unsigned char BC_xfer_cap;
    /* 
       BEAR_CAP_SPEECH
       BEAR_CAP_UNREST_DIG
       BEAR_CAP_REST_DIG
       BEAR_CAP_3DOT1K_AUDIO
       BEAR_CAP_7K_AUDIO
       BEAR_CAP_VIDEO
     */

    unsigned char BC_xfer_mode;
    /*  
       ISDN_ITM_CIRCUIT
       ISDN_ITM_PACKET
     */

    unsigned char BC_xfer_rate;
    /* 
       BEAR_RATE_64KBPS
       BEAR_RATE_128KBPS
       BEAR_RATE_384KBPS
       BEAR_RATE_1536KBPS
       BEAR_RATE_1920KBPS
       PACKET_TRANSFER_MODE	

     */

    unsigned char usrinfo_layer1_protocol;
    /* 
       ISDN_UIL1_CCITTV110 - CCITT standardized rate adaptation V.110/X.30
       ISDN_UIL1_G711ULAW  - Recommendation G.711 u-Law
       ISDN_UIL1_G711ALAW  - Recommendation G.711 a-Law
       ISDN_UIL1_G721ADCPM - Recommendation G.721 32 kbits/s ADCPM and 
                             Recommendation I.460  
       ISDN_UIL1_G722G725  - Recommendation G.722 and G.725 - 7kHz audio
       ISDN_UIL1_H261      - Recommendation H.261 - 384 kbits/s video
       ISDN_UIL1_NONCCITT  - Non-CCITT standardized rate adaptation
       ISDN_UIL1_CCITTV120 - CCITT standardized rate adaptation V.120
       ISDN_UIL1_CCITTX31  - CCITT standardized rate adaptation X.31 HDLC
    */

    unsigned char usr_rate;
    /*
       ISDN_UR_EINI460 - Determined by E bits in I.460
       ISDN_UR_600     - 0.6 kbits, V.6 and X.1
       ISDN_UR_1200    - 1.2kbits, V.6
       ISDN_UR_2400    - 2.4 kbits, V.6 and X.1
       ISDN_UR_3600    - 3.6 kbits, V.6
       ISDN_UR_4800    - 4.8 kbits, V.6 and X.1
       ISDN_UR_7200    - 7.2 kbits, V.6
       ISDN_UR_8000    - 8.0 kbits, I.460
       ISDN_UR_9600    - 9.6 kbits, V.6 and X.1
       ISDN_UR_14400   - 14.4 kbits, V.6
       ISDN_UR_16000   - 16 kbits, I.460
       ISDN_UR_19200   - 19.2 kbits, B.6
       ISDN_UR_32000   - 32 kbits, I.460
       ISDN_UR_48000   - 48 kbits, V.6 and X.1
       ISDN_UR_56000   - 56 kbits, V.6
       ISDN_UR_64000   - 64 kbits, X.1
       ISDN_UR_134     - 1345 kbits, X.1
       ISDN_UR_100     - 100 kbits, X.1
       ISDN_UR_75_1200 - 075/1200 kbits, V.6 and X.1
       ISDN_UR_1200_75 - 1200/.075 kbits, V.6 and X.1
       ISDN_UR_50      - 050 kbits, V.6 and X.1
       ISDN_UR_75      - 075 kbits, V.6 and X.1
       ISDN_UR_110     - 110 kbits, V.6 and X.1
       ISDN_UR_150     - 150 kbits, V.6 and X.1
       ISDN_UR_200     - 200 kbits, V.6 and X.1
       ISDN_UR_300     - 300 kbits, V.6 and X.1
       ISDN_UR_12000   - 12 kbits, V.6
    */

    unsigned char destination_number_type;
    /* 
       EN_BLOC_NUMBER - number is sent en-block
       INTL_NUMBER    - international number
       NAT_NUMBER     - national number
       LOC_NUMBER     - subscriber number
       OVERLAP_NUMBER - overlap sending
     */

    unsigned char destination_number_plan;
    /* 
       UNKNOWN_NUMB_PLAN   - unknown new
       ISDN_NUMB_PLAN      - ISDN/telephony (E.164/E.163)
       TELEPHONY_NUMB_PLAN - telephony numbering
       plan (E.164)
       PRIVATE_NUMB_PLAN   - private numbering plan
     */

    unsigned char destination_sub_number_type;
    /* 
       OSI_SUB_ADDR           - NSAP(X.213/ISO 8348 AD2)
       USER_SPECIFIC_SUB_ADDR - user specified
       IA_5_FORMAT            - IA5 sub_address digit format
     */

    unsigned char destination_sub_number_plan;

    char destination_sub_phone_number[MAXPHONENUM];

    unsigned char origination_number_type;
    /* 
       EN_BLOC_NUMBER - number is sent en-block
       INTL_NUMBER    - international number
       NAT_NUMBER     - national number
       LOC_NUMBER     - subscriber number
       OVERLAP_NUMBER - overlap sending
     */

    unsigned char origination_number_plan;
    /* 
       UNKNOWN_NUMB_PLAN   - unknown
       ISDN_NUMB_PLAN      - ISDN/telephony (E.164/E.163)
       TELEPHONY_NUMB_PLAN - telephony numbering plan (E.164)
       PRIVATE_NUMB_PLAN   - private numbering plan
     */

    char origination_phone_number[MAXPHONENUM];
    /* 
       Scope : only one time;
       use is_SetOrigination() to set it permanently
     */

    unsigned char origination_sub_number_type;
    /* 
       OSI_SUB_ADDR           - NSAP(X.213/ISO 8348 AD2)
       USER_SPECIFIC_SUB_ADDR - user specified
       IA_5_FORMAT            - IA5 sub_address digit format
     */

    unsigned char origination_sub_number_plan;

    char origination_sub_phone_number[MAXPHONENUM];

    unsigned char facility_feature_service;
    /* 
       ISDN_FEATURE - request facility is a feature
       ISDN_SERVICE - requested facility is a service
     */

    unsigned char facility_coding_value;
    /* 
       ISDN_CPN_PREF   - CPN preferred
       ISDN_SDN        - SDN
       ISDN_BN_PREF    - BN preferred
       ISDN_ACCUNET    - ACCUNET SDS
       ISDN_LONG_DIS   - long distance
       ISDN_INT_800    - international 800
       ISDN_CA_TSC     - CA TSC
       ISDN_ATT_MULTIQ - ATT multiquest
     */

    int completion_point;   

    union {
      struct {
        int rfu;    
      } att;
      struct {
        CHAN_ID channel_id;    
      } bri;
      struct {
        int rfu;    
      } vn3;
      struct {
        int rfu;   
      } dass;	  
    }u;             /* switch system dependent parameters */

    USRINFO_ELEM *usrinfo_bufp; 
    /* 
       reserve space for the connecting set up message's user to user 
       information. Driver should ignore it if it is a zero.
    */

    NFACILITY_ELEM *nsfc_bufp; /* Reserved for furture */
  }isdn;
} MAKECALL_BLK,  *MAKECALL_BLK_PTR;

/* difference between PKG 1 and PKG 2 */
#define BC_info_mode	BC_xfer_mode
#define usrinfo_buf	usrinfo_bufp
#define nsfbufp		nsfc_bufp
#define CCEV_FACILITYREQ        (DT_CC | 0xFF)  



typedef union {
  struct {
    USRINFO_ELEM *usrinfo_buf;  
    /* 
       reserve space for the connecting set up
       message's user to user information
     */
    NFACILITY_ELEM *nsfbufp;
  }isdn;

  struct {
    int num_of_digits;
  } belgian_r2mf;
} WAITCALL_BLK,  *WAITCALL_BLK_PTR;


typedef union {
  short s;
  long  l;
  char  c;
  float f;
} CC_PARM;

#define MAXLEN_IEDATA              260
typedef struct {
  short  length;                   /* must be less than MAXLEN_IEDATA  */
  char   data[MAXLEN_IEDATA];      /* CCITT standard                   */
} IE_BLK, *IE_BLK_PTR;

typedef struct {
  char   sapi;
  char   ces;
  short  length;                   /* must be less than MAXLEN_IEDATA  */
  char   data[MAXLEN_IEDATA];      /* CCITT standard                   */
} L2_BLK, *L2_BLK_PTR;


typedef struct {
  INT  nSapi;
  INT  nCes;
  short  length;                   /* length of Public IE data - must be less than MAXLEN_IEDATA  */
  char   data[MAXLEN_IEDATA];      /* Public IE data - must be CCITT standard */
} NONCRN_BLK, *NONCRN_BLK_PTR; /* added support for non call related messages */

typedef NONCRN_BLK NONCALL_BLK;			
typedef NONCRN_BLK_PTR NONCALL_BLK_PTR;	


typedef struct {
   unsigned short duration;
   unsigned short freq1;
   short          amp1;
   unsigned short freq2;
   short          amp2;
   unsigned short toneOn1;
   unsigned short toneOff1;
   unsigned short reserv1;
   unsigned short reserv2;
} toneParm;

/*
 * Addition made on requirement that CC should not
 * post any event directly to SRL when working under GC
 */
typedef long (*gcis_PutEvtType)( long,unsigned long,long,void  *,long);

#ifdef VME_SPAN
#else
#pragma pack()
#endif 

/*
 * Prototypes for all library functions.
 */

#if (defined (__cplusplus) || defined( __STDC__ ))
#include "isdncmd.h"
#include "isdnlib.h"
#include "isdnerr.h"

#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable funcs to be called from C++
#endif
// Call Control Functions
int __cdecl cc_AnswerCall(CRN crn, int rings, unsigned long mode);
int __cdecl cc_DropCall(CRN crn, int cause, unsigned long mode);
int __cdecl cc_MakeCall(LINEDEV linedev, CRN *crn_buf, char *phone_num,
                MAKECALL_BLK *makecall_blkp, int timeout, unsigned long mode);
int __cdecl cc_ReleaseCall(CRN crn);
int __cdecl cc_ReleaseCallEx(CRN crn, MODE nMode);
int __cdecl cc_WaitCall(LINEDEV linedev, CRN *crnptr, WAITCALL_BLK *waitcall_blkp,
                int timeout, unsigned long mode);

// Optional Cal Handling Functions
int __cdecl cc_AcceptCall(CRN crn, int rings, unsigned long mode);
int __cdecl cc_CallAck(CRN crn, LINEDEV new_linedev, int msg_id);
int __cdecl cc_CallProgress(CRN crn, int indicator);
int __cdecl cc_Congestion(CRN crn, int cause);
int __cdecl cc_FacilityAck(CRN crn, NFACILITY_ELEM *facility_ptr);
int __cdecl cc_FacilityRej(CRN crn, NFACILITY_ELEM *facility_ptr, int cause);
int __cdecl cc_FacilityReq(CRN crn, NFACILITY_ELEM *facility_buf, unsigned long mode);
int __cdecl cc_GetANI(CRN crn, char * anibuf);
int __cdecl cc_GetBilling(CRN crn, char *billing_buf);
int __cdecl cc_GetCallInfo(CRN crn, int info_id, char *valuep);
int __cdecl cc_GetDNIS(CRN crn, char *dnis);
int __cdecl cc_GetMoreDigits(CRN crn, int num_digits, long timeout, unsigned long mode);
int __cdecl cc_GetNonCallMsg(LINEDEV linedev, NONCRN_BLK *noncallmsgptr);
int __cdecl cc_HoldAck(CRN crn);
int __cdecl cc_HoldCall(CRN crn, unsigned long mode);
int __cdecl cc_HoldRej(CRN crn, int cause);
int __cdecl cc_RejectCall(CRN crn, int cause, unsigned long mode);
int __cdecl cc_ReqANI(CRN crn, char *anibuf, int req_type, unsigned long mode);
int __cdecl cc_RetrieveAck(CRN crn);
int __cdecl cc_RetrieveCall(CRN crn, unsigned long mode);
int __cdecl cc_RetrieveRej(CRN crn, int cause);
int __cdecl cc_CallWaitingAccept(CRN crn, LINEDEV linedev, unsigned long mode);  /* HSS:CW_Flag Issue...6/27/2003 */
int __cdecl cc_TransferCall(CRN crn1, CRN crn2, unsigned long mode);
int __cdecl cc_TransferAck(CRN crn);
int __cdecl cc_TransferRej(CRN crn, INT nCause);

int __cdecl cc_SetBilling(CRN crn, int rate_type, CC_RATE_U *ratep, unsigned long mode);
int __cdecl cc_SetCallingNum(LINEDEV linedev, char *callingnum);
int __cdecl cc_SetMinDigits(LINEDEV linedev, int mindigs);
int __cdecl cc_SndNonCallMsg(LINEDEV linedev, int crn_type, int msg_type, NONCRN_BLK *noncallmsgptr);
int __cdecl cc_SndMsg( CRN crn, int msg_type, IE_BLK *sndmsgptr );
int __cdecl cc_TerminateCall(CRN crn, int cause);
int __cdecl cc_TerminateRelease(CRN crn);

// System Control Functions
int __cdecl cc_Close(LINEDEV linedev);
int __cdecl cc_Open(LINEDEV *linedevbuf, char *dev_name, unsigned short rfu);
int __cdecl cc_Restart(LINEDEV linedev, unsigned long mode);

// System Tools Functions
int __cdecl cc_CallState(CRN crn, int *state_ptr);
int __cdecl cc_CauseValue(LINEDEV linedev);
int __cdecl cc_CRN2LineDev(CRN crn, LINEDEV *linedevp);
int __cdecl cc_GetBChanState(LINEDEV ddd, int *l2stateptr );
int __cdecl cc_GetChanId(CRN crn, CHAN_ID * channel_id_ptr);
int __cdecl cc_GetCRN(CRN *crn_ptr, void * evtdatap);
int __cdecl cc_GetDChanState(LINEDEV ddd, int *l2stateptr );
int __cdecl cc_GetCES(char *ces, void *voidptr);
int __cdecl cc_GetDLinkState(LINEDEV linedev, DLINK *dlinkptr, int *state_buf );
int __cdecl cc_GetNetCRV(CRN crn,  int *netcrvp);
int __cdecl cc_GetSigInfo(CHAR *lpValue,INT nInfoID,LPVOID lpVoid);
int __cdecl cc_GetEvtMsk(LINEDEV devhdl, unsigned long *maskp);
int __cdecl cc_GetFrame(LINEDEV linedev, L2_BLK *rcvframptr);
int __cdecl cc_GetInfoElem( LINEDEV ddd, IE_BLK *iep );
int __cdecl cc_GetLineDev(LINEDEV *linedev_ptr, void * evtdata_ptr);
int __cdecl cc_GetNetH(LINEDEV linedev, int * neth);
int __cdecl cc_GetParm(LINEDEV linedev, int parm_id, CC_PARM *value_ptr);
int __cdecl cc_GetSAPI(char *sapi, void *voidptr);
int __cdecl cc_GetSigInfo(char *valuep, int info_id, void *voidptr);
int __cdecl cc_GetUsrAttr(LINEDEV linedev, long *usr_attrp);
int __cdecl cc_GetVer(LINEDEV linedev, unsigned int * major_ptr, unsigned int *minor_ptr);
int __cdecl cc_GetVoiceH(LINEDEV linedev, int * voiceh);
CRN __cdecl cc_LineDev2CRN(LINEDEV linedev);
int __cdecl cc_PutEvt(LINEDEV ddd, long event_type);
int __cdecl cc_ResultMsg(LINEDEV linedev, int ResultCode, char **msg);
int __cdecl cc_ResultValue(void * evtdata_ptr);
int __cdecl cc_SetChanState(LINEDEV linedev, int chan_state, unsigned long mode);
int __cdecl cc_SetDChanCfg(LINEDEV linedev, DCHAN_CFG *dchan_cfg_ptr);
int __cdecl cc_SetEvtMsk(LINEDEV devhdl, unsigned long mask, int action);
int __cdecl cc_SetInfoElem(LINEDEV linedev, IE_BLK *ie_ptr);
int __cdecl cc_SetParm(LINEDEV linedev, int parm_id, long value);
int __cdecl cc_TermRegisterResponse(LINEDEV, TERM_BLK *term_blk_ptr);
int __cdecl cc_SetUsrAttr(LINEDEV linedev, long usr_attr);
int __cdecl cc_SndFrame(LINEDEV linedev, L2_BLK *sndframptr);
int __cdecl cc_StartTrace(LINEDEV linedev, char *tracefilename);
int __cdecl cc_StopTrace(LINEDEV linedev);
int cc_SetDLinkState(LINEDEV linedev, DLINK *dlinkptr, int *state_buf );
int cc_GetDLinkCfg( LINEDEV linedev, DLINK *dlinkptr, DLINK_CFG *config);
int cc_SetDLinkCfg( LINEDEV linedev, DLINK *dlinkptr, DLINK_CFG *config);
int cc_Register(LINEDEV LineDev,CRN *crnptr,NONCALL_BLK *lpMsgBlk,unsigned long Mode);
int __cdecl cc_CRN2LineDevUpdate(CRN crn, LINEDEV *linedevp); /* HSS:FR-418 */

// Global Call Interface Functions
int __cdecl cc_BlockTs(LINEDEV linedev);
int __cdecl cc_libinit(HINSTANCE LibHandle);	 /* Init ISDN funcs for xcompat lib */
int __cdecl cc_geterror(void);
int __cdecl cc_seterror(int error);
int __cdecl cc_UnBlockTs(LINEDEV linedev);

//  BRI Interface Function
int __cdecl cc_ToneRedefine(LINEDEV LineDev, unsigned char sigtype, toneParm *pToneParm, MODE Mode);
int __cdecl cc_PlayTone(LINEDEV LineDev, toneParm *pToneParm, DV_TPT *tptp, MODE Mode);
int __cdecl cc_StopTone(LINEDEV LineDev, MODE Mode);

int __cdecl cc_GetParmEx(LINEDEV linedev, int parm_id, PARM_INFO_PTR value_ptr);
int __cdecl cc_SetParmEx(LINEDEV linedev, int parm_id, PARM_INFO_PTR value);

/*
 * Addition made on requirement that CC should not
 * post any event directly to SRL when working under GC
 */
void __cdecl cc_WorkingUnderGC(gcis_PutEvtType func);

#ifdef __cplusplus
}
#endif

#else

#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable funcs to be called from C++
#endif
// Call Control Functions
DllExport
int __cdecl cc_AnswerCall();
DllExport
int __cdecl cc_DropCall();
DllExport
int __cdecl cc_MakeCall();
DllExport
int __cdecl cc_ReleaseCall();
DllExport
int __cdecl cc_ReleaseCallEx();
DllExport
int __cdecl cc_WaitCall();

// Optional Cal Handling Functions
DllExport
int __cdecl cc_AcceptCall();
DllExport
int __cdecl cc_CallAck();
DllExport
int __cdecl cc_CallProgress();
DllExport
int __cdecl cc_Congestion();
DllExport
int __cdecl cc_FacilityAck();
DllExport
int __cdecl cc_FacilityRej();
DllExport
int __cdecl cc_FacilityReq();
DllExport
int __cdecl cc_GetANI();
DllExport
int __cdecl cc_GetBilling();
DllExport
int __cdecl cc_GetCallInfo();
DllExport
int __cdecl cc_GetDNIS();
DllExport
int __cdecl cc_GetMoreDigits();
DllExport
int __cdecl cc_GetNonCallMsg();
DllExport
int __cdecl cc_HoldAck();
DllExport
int __cdecl cc_HoldCall();
DllExport
int __cdecl cc_HoldRej();
DllExport
int __cdecl cc_RejectCall();
DllExport
int __cdecl cc_ReqANI();
DllExport
int __cdecl cc_RetrieveAck();
DllExport
int __cdecl cc_RetrieveCall();
DllExport
int __cdecl cc_RetrieveRej();

DllExport
int __cdecl cc_CallWaitingAccept();  /* HSS:CW_Flag Issue...6/27/2003 */

DllExport
int __cdecl cc_TransferCall();
DllExport
int __cdecl cc_TransferAck();
DllExport
int __cdecl cc_TransferRej();

DllExport
int __cdecl cc_SetBilling();
DllExport
int __cdecl cc_SetCallingNum();
DllExport
int __cdecl cc_SetMinDigits();
DllExport
int __cdecl cc_SndNonCallMsg();
DllExport
int __cdecl cc_SndMsg();
DllExport
int __cdecl cc_TerminateCall();
DllExport
int __cdecl cc_TerminateRelease();

// System Control Functions
DllExport
int __cdecl cc_Close();
DllExport
int __cdecl cc_Open();
DllExport
int __cdecl cc_Restart();

// System Tools Functions
DllExport
int __cdecl cc_CallState();
DllExport
int __cdecl cc_CauseValue();
DllExport
int __cdecl cc_CRN2LineDev();

DllExport
int __cdecl cc_CRN2LineDevUpdate(); /* HSS:FR-418 */

DllExport
int __cdecl cc_GetBChanState();
DllExport
int __cdecl cc_GetCRN();
DllExport
int __cdecl cc_GetChanId();
DllExport
int __cdecl cc_GetDChanState();
DllExport
int __cdecl cc_GetCES();
DllExport
int __cdecl cc_GetDLinkState();
DllExport
int __cdecl cc_GetEvtMsk();
DllExport
int __cdecl cc_GetFrame();
DllExport
int __cdecl cc_GetInfoElem();
DllExport
int __cdecl cc_GetLineDev();
DllExport
int __cdecl cc_GetNetCRV();
DllExport
int __cdecl cc_GetSigInfo();
DllExport
int __cdecl cc_GetNetH();
DllExport
int __cdecl cc_GetParm();
DllExport
int __cdecl cc_GetSAPI();
DllExport
int __cdecl cc_GetSigInfo();
DllExport
int __cdecl cc_GetUsrAttr();
DllExport
int __cdecl cc_GetVer();
DllExport
int __cdecl cc_GetVoiceH();
DllExport
CRN __cdecl cc_LineDev2CRN();
DllExport
int __cdecl cc_PutEvt();
DllExport
int __cdecl cc_ResultMsg();
DllExport
int __cdecl cc_ResultValue();
DllExport
int __cdecl cc_SetChanState();
DllExport
int __cdecl cc_SetDChanCfg();
DllExport
int __cdecl cc_SetEvtMsk();
DllExport
int __cdecl cc_SetInfoElem();
DllExport
int __cdecl cc_SetParm();
DllExport
int __cdecl cc_TermRegisterResponse();
DllExport
int __cdecl cc_SetUsrAttr();
DllExport
int __cdecl cc_SndFrame();
DllExport
int __cdecl cc_StartTrace();
DllExport
int __cdecl cc_StopTrace();
DllExport
int cc_SetDLinkState();
DllExport
int cc_GetDLinkCfg(); 
DllExport
int cc_SetDLinkCfg();
DllExport
int cc_Register();	

// Global Call Interface Functions
DllExport
int __cdecl cc_BlockTs();
DllExport
int __cdecl cc_libinit();	 /* Init ISDN funcs for xcompat lib */
DllExport
int __cdecl cc_geterror();
DllExport
int __cdecl cc_seterror();
DllExport
int __cdecl cc_UnBlockTs();

//  BRI Interface Function
DllExport
int __cdecl cc_ToneRedefine();
DllExport
int __cdecl cc_PlayTone();
DllExport
int __cdecl cc_StopTone();


DllExport
int __cdecl cc_GetParmEx();
DllExport
int __cdecl cc_SetParmEx();

/*
 * Addition made on requirement that CC should not
 * post any event directly to SRL when working under GC
 */
DllExport
void __cdecl cc_WorkingUnderGC();

#ifdef __cplusplus
}
#endif


#include "isdncmd.h"
#include "isdnlib.h"
#include "isdnerr.h"

#endif


#endif   /* for __CCLIB_H__ */
