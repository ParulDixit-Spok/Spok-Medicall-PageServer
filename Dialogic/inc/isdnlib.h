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
 * Filename:    isdnlib.h 
 * DESCRIPTION: Define Information Elements, Driver Command,and Structures
 *              for the ISDN library.   
 *****************************************************************************/

#ifndef _ISDNLIB_H_
#define _ISDNLIB_H_


#include "srllib.h"
#include "ccerr.h"

#ifdef VME_SPAN
#else
#pragma pack(1)
#endif 


#ifndef BYTE
#define BYTE                   unsigned char
#endif

#ifndef EV_SYNC
#define EV_SYNC                0x0000
#endif

#ifndef EV_ASYNC
#define EV_ASYNC               0x8000
#endif

#define ISDN_EV                0x100
#define ISDN_NOTUSED           0xFF

/*
 * Types of billing data
 */
#define ISDN_NEW_RATE          0x90
#define ISDN_FLAT_RATE         0x91
#define ISDN_PREM_CHARGE       0x92
#define ISDN_PREM_CREDIT       0x93
#define ISDN_FREE_CALL         0x98

/*
 * Types of Information Elements that may be gotten (is_GetCallInfo)
 */
#define AOC                    0x01    /* Advice of Charge */
#define NSF                    0x02    /* Network Specific Facility */
#define Calling_Num            0x03    /* Calling party number */
#define Called_Num             0x04    /* Called party number */
#define Called_subs            0x05    /* Called party subaddress */
#define CALLED_SUBS            0x05 /* Called party subaddress */
#define Cause                  0x06    /* Cause value */
#define UUI                    0x07    /* User2User information */
#define Calling_subs           0x08    /* Calling party subaddress */
#define U_IES                  0x10    /* User2User Unformated information */


/*****************************************************************************
 *                    library to driver command
 ****************************************************************************/
#define ISDN_CMD_DRV           0x80    /* ISDN driver level command, from 0x80 
                                          to 0xBF
                                        */
#define ISDN_GETDNIS           (ISDN_CMD_DRV | 0x01)
#define ISDN_RESERVEUSR2USRINFOBUF  (ISDN_CMD_DRV | 0x02)
#define ISDN_OPEN              (ISDN_CMD_DRV | 0x03)
#define ISDN_RESERVEINFOELEMBUF     (ISDN_CMD_DRV | 0x04)
#define ISDN_GETBILLING        (ISDN_CMD_DRV | 0x05)
#define ISDN_CRN2LINEDEV       (ISDN_CMD_DRV | 0x06)
#define ISDN_WRITE2DSK         (ISDN_CMD_DRV | 0x07)
#define ISDN_GETANI            (ISDN_CMD_DRV | 0x08)
#define ISDN_GETCALLINFO       (ISDN_CMD_DRV | 0x09)
#define ISDN_ECHO_ABORT        (ISDN_CMD_DRV | 0x0A)
#define ISDN_RESTART_ABORT     (ISDN_CMD_DRV | 0x0B)
#define ISDN_PUTEVT            (ISDN_CMD_DRV | 0x0C)

/* Defines for FW errors about Link status - KP, PL. */
#define FW_ISDN_LINKDISABLED   (DATA_LINK_DISABLED)
#define FW_ISDN_LINKDOWN       (DATA_LINK_DOWN)
#define FW_ISDN_LINKUP         (DATA_LINK_UP)
#define FW_ISDN_LINKRESET      (DATA_LINK_RESET)
/*
 * Maximum number of Network Specific Facility Elements that will be stored
 * at a time.
 */
#define MAX_NUM_NFACS          4

/**********************
 * Type  Code
 **********************/

#ifndef _EV_COMMON_DATA
/*
 *   When an event occurs in the ISDN environment, the API returns a
 *   block of event specific data.  This data conforms to the structure
 *   defined below.  The user can access event specific data using the
 *   following structure template.  This structure is returned in the
 *   event data portion of the standard event block and can be 
 *   retrieved using the function sr_getevtdatap().   The application
 *   should look at the ec_data field for data, in addition to the
 *   standard information.  This can be determined from the length
 *   of data passed with the event.
 */
typedef struct {
      char    ec_call_line_mode;
      CRN     ec_crn;
      LINEDEV ec_handle;
      int     ec_error_cause;
      char    ec_data[8];
   } EV_COMMON_DATA_T;
#define _EV_COMMON_DATA
#define EV_LENGTH	sizeof(EV_COMMON_DATA_T)
#endif

/**********************
 * Event Code
 *********************/

/* cc_TermRegisterAck() completed */
#define CCEV_TERM_REGISTER_ACK (ISDN_EV | 0x4C)


// 03/27/07 Use 0x2A00 for base event value going forward.
#define  ISDNEV_BASE		0x2A00					// beginning base value
#define  ISDNV_LAST_EVENT	(ISDNEV_BASE | 0xFF)	// Last event value


/*******************************
 * Event Mask
*******************************/
#define EVTMSK_NOFACILITYBUF   0x100
#define EVTMSK_NOUSRINFOBUF    0x200


/*
 *   Defines associated with the Parameters.
 */

#define BC_XFER_CAP            0x403
#define BC_XFER_MODE           0x404
#define BC_INFO_MODE           0x404
#define BC_XFER_RATE           0x405
#define USRINFO_LAYER1_PROTOCOL 0x406
#define USR_RATE                0x407
#define CALLED_NUM_TYPE        0x7003
#define CALLED_NUM_PLAN        0x7004
#define CALLING_NUM_TYPE       0x6c03
#define CALLING_NUM_PLAN       0x6c04
#define CALLING_PRESENTATION   0x6c05
#define CALLING_SCREENING      0x6c06

// Dynamic NRB
#define CC_UNSOLSIZE           0x8001

// HSS Code merge
#define RELEASE_CAUSE			0x804
// End HSS merge

/* 03/19/97 Using malloc to get rcv & snd ie buffer */
/* cc_SetParm for multiple ie buffers */
#define RECEIVE_INFO_BUF       0x789A
#define MAX_RECEIVE_INFO_BUF_NO 	 160
#define MAX_SEQ_LOOP					 29
 
#define PBX_TYPE	      30
 
#define IS_BILLSTR_SZ          32
#define IS_ANI_BUF_SZ          32
#define IS_DNIS_BUF_SZ         32
#define IS_SUBADDR_BUF_SZ      20

#define MAXNFACNETWORKID       251
#define MAXUSRINFOSIZE         250
#define MAXINFOELEMSIZE        0x110
#define TRACEBUFSIZE           0x800
#define MAXPDUSIZE		       250	// size limit of a PDU in FACILITY IE

// Register PRI device in SRL 
#define PRI_TYPE               0x3030
#define PRI_BOARD_ENTRY_TO_SRL 0x82
#define PRI_DEV_NAME           "dtiB"
#define PRI_DEV_NAME_LENGTH    4

/*
 *   Typedefs section.
 */

#ifndef _MODE
typedef unsigned long MODE;
#define _MODE
#endif

/* 
 * This structure is used with GetCallInfo to get the Network Specific
 * Facility elements for a call.
 */
typedef struct {
   int num_elem;                        /* Number of NSF elements gotten */
   NFACILITY_ELEM nfacs[MAX_NUM_NFACS]; /* The NSF elements themselves */
} USR_NFAC_T;

typedef struct {
   int   length;
   char  info_elem_str[MAXINFOELEMSIZE];
   int   uuinfo_elem_length;
   char  *uuinfo_elem_ptr;
} INFO_ELEM_BLK, *IEB_PTR;

typedef struct {
   unsigned char subaddr_type;
   unsigned char subaddr_plan;
   char          subaddr_info[IS_SUBADDR_BUF_SZ];
} CALLED_PARTY_SUBADDR_ELEM, *CPSAE_PTR;

typedef struct {
   unsigned char subaddr_type;
   unsigned char subaddr_plan;
   char          subaddr_info[IS_SUBADDR_BUF_SZ];
} CALLING_PARTY_SUBADDR_ELEM, *CLSAE_PTR;

typedef struct {
   int    dev_hdl;   /* board device handle */
   int    fh;        /* User should take this file handler into account if 
                      * the total number of opening file is near to the max. 
                      */
   int    head;
   int    tail;
   char   *buf;
} TRACEINFO, *TRACEINFOPTR;


/* DCHAN_CFG structure for BRI cc_SetDChanCfg() function */
typedef struct {
   BYTE           layer2_access;          /* Layer 2 or full stack */
   BYTE           switch_type;            /* Layer 3 switch type */
   BYTE           switch_side;            /* Network or User side */
   BYTE           number_of_endpoints;    /* # of logical data links */
   BYTE           feature_controlA;       /* Firmware feature mask A */
   BYTE           feature_controlB;       /* Firmware feature mask B */
   BYTE           rfu_1;                  /* Reserved for future use */

   BYTE           rfu_2;                  /* Reserved for future use */
   struct {
      BYTE        tei_assignment;         /* Auto assignment or Fixed Tei term */
      BYTE        fixed_tei_value;        /* Tei value if Fixed Tei terminal */
      union {
         struct {
            BYTE  auto_init_flag;         /* Auto Initializing term or not */
            BYTE  SPID[MAX_SPID_SIZE];    /* SPID for terminal, NULL
                                             terminated string. */
            BYTE  rfu_1;
            BYTE  rfu_2;
         } no_am; /* North America */      
      } protocol_specific;      
   } user;
 #define  RFU_COUNT  8  /* # of reserve for future use bytes */
   BYTE  rfu[RFU_COUNT];

   union {
      struct {
         long     T302;
         long     T303;
         long     T304;
         long     T305;
         long     T306;
         long     T308;
         long     T309;
         long     T310;
         long     T312;
         long     T322;
      } nt;
      struct {
         long     T303;
         long     T304;
         long     T305;
         long     T308;
         long     T310;
         long     T312;
         long     T313;
         long     T318;
         long     T319;
      } te;
   } tmr;
} DCHAN_CFG, *DCHAN_CFG_PTR;


typedef struct 
{
   char  tei;
   char  state;
   char  protocol;
} DLINK_CONFIG, *DLINK_CONFIG_PTR;

typedef struct 
{
   char  tei;
   int  state;
   int  protocol;
} DLINK_CFG, *DLINK_CFG_PTR;

/**********************************************************************
 * Terminal Initialization EVENT. CCEV_TERM_REGISTER.
 **********************************************************************/

/* Structure for SAPI and CES */
typedef struct 
{
   BYTE sapi;
   BYTE  ces;
} DLINK, *DLINK_PTR;


/*
 * Structure for CCEV_TERM_REGISTER Event.
 */
typedef struct
{
   DLINK data_link;
   BYTE  initializing_term;
   BYTE  SPID[MAX_SPID_SIZE];
} SPID_BLK;
   
/* 
 * Structure for CCEV_RCVTERMREG_ACK Event.
 */
typedef struct
{
   DLINK data_link;
   struct
   {
      BYTE usid;
      BYTE tid;
      BYTE interpreter;
   } uspid;
} USPID_BLK;

/* 
 * Structure for CCEV_RCVTERMREG_NACK Event.
 */   
typedef struct
{
   DLINK data_link;
   BYTE  cause_value;
} TERM_NACK_BLK;

/*
 * Structre for API cc_TermRegisterResponse.
 */
typedef struct
{
   DLINK data_link;
   BYTE ack_type;
   union
   {
      BYTE cause_value; 
                       /* Cause Value if ack type is ISDN_ERROR */
      struct 
      {
         BYTE usid;
         BYTE tid;
         BYTE interpreter;
      } uspid;
   } ack_info;
} TERM_BLK, *TERM_BLK_PTR;

     
/* 
 * Structure for cc_SetParmEx and cc_GetParmEx
 */

typedef struct
{
   BYTE parmdatalen;   
   unsigned char parmdata[256];	// maximum length of 256 bytes for ISDN information
} PARM_INFO, *PARM_INFO_PTR;

#ifdef    WIN32
#define ISTLS_DYNAMIC 
#ifdef ISTLS_DYNAMIC
extern	DWORD is_TLS_index;

typedef struct {
	int	is_error_in_handler;
	int	is_cclibid_error_in_handler;
	int is_error_not_in_handler;
	int	is_cclibid_error_not_in_handler;
} is_TLS_store;
#endif
#endif


#ifdef VME_SPAN
#else
#pragma pack()
#endif 


#endif   /*   for _ISDNLIB_H_    */
