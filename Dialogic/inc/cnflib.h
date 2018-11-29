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
 /***********************************************************************
 *        FILE: cnflib.h
 * DESCRIPTION: Conferencing library API
 *        DATE: Oct. 14, 2002
 *
 **********************************************************************/

#ifndef _CNFLIB_H_
#define _CNFLIB_H_

#define CNFEVBASE 0x100
typedef enum
{
   CNFEV_CONFCREATED            = (CNFEVBASE + 0x1),
   CNFEV_CREATECONFFAILED       = (CNFEVBASE + 0x2),
   CNFEV_CONFATTACHED           = (CNFEVBASE + 0x3),
   CNFEV_ATTACHCONFFAILED       = (CNFEVBASE + 0x4),
   CNFEV_CONFDELETED            = (CNFEVBASE + 0x5),
   CNFEV_DELETECONFFAILED       = (CNFEVBASE + 0x6),
   CNFEV_ALLCONFDELETED         = (CNFEVBASE + 0x7),
   CNFEV_DELETEALLCONFFAILED    = (CNFEVBASE + 0x8),

   CNFEV_PARTYADDED             = (CNFEVBASE + 0x11),
   CNFEV_ADDPARTYFAILED         = (CNFEVBASE + 0x12),
   CNFEV_PARTYREMOVED           = (CNFEVBASE + 0x13),
   CNFEV_REMOVEPARTYFAILED      = (CNFEVBASE + 0x14),
   CNFEV_ALLPARTIESREMOVED      = (CNFEVBASE + 0x15),
   CNFEV_REMOVEALLPARTIESFAILED = (CNFEVBASE + 0x16),

   CNFEV_EVTENABLED             = (CNFEVBASE + 0x21),
   CNFEV_ENABLEEVTFAILED        = (CNFEVBASE + 0x22),
   CNFEV_EVTDISABLED            = (CNFEVBASE + 0x23),
   CNFEV_DISABLEEVTFAILED       = (CNFEVBASE + 0x24),

   CNFEV_ACTTALKERS             = (CNFEVBASE + 0x31),
   CNFEV_GETACTTALKERSFAILED    = (CNFEVBASE + 0x32),
   CNFEV_PARTYLIST              = (CNFEVBASE + 0x33),
   CNFEV_GETPARTYLISTFAILED     = (CNFEVBASE + 0x34),
   CNFEV_VOLCONTROLSET          = (CNFEVBASE + 0x35),
   CNFEV_SETVOLCONTROLFAILED    = (CNFEVBASE + 0x36),
   CNFEV_VOLCONTROL             = (CNFEVBASE + 0x37),
   CNFEV_GETVOLCONTROLFAILED    = (CNFEVBASE + 0x38),
   CNFEV_RESCOUNT               = (CNFEVBASE + 0x39),
   CNFEV_GETRESCOUNTFAILED      = (CNFEVBASE + 0x3A),

   CNFEV_BRDATTRSET             = (CNFEVBASE + 0x41),
   CNFEV_SETBRDATTRFAILED       = (CNFEVBASE + 0x42),
   CNFEV_BRDATTR                = (CNFEVBASE + 0x43),
   CNFEV_GETBRDATTRFAILED       = (CNFEVBASE + 0x44),
   CNFEV_CONFATTRSET            = (CNFEVBASE + 0x45),
   CNFEV_SETCONFATTRFAILED      = (CNFEVBASE + 0x46),
   CNFEV_CONFATTR               = (CNFEVBASE + 0x47),
   CNFEV_GETCONFATTRFAILED      = (CNFEVBASE + 0x48),
   CNFEV_PARTYATTRSET           = (CNFEVBASE + 0x49),
   CNFEV_SETPARTYATTRFAILED     = (CNFEVBASE + 0x4A),
   CNFEV_PARTYATTR              = (CNFEVBASE + 0x4B),
   CNFEV_GETPARTYATTRFAILED     = (CNFEVBASE + 0x4C),

   CNFEV_BRIDGECREATED          = (CNFEVBASE + 0x61),
   CNFEV_CREATEBRIDGEFAILED     = (CNFEVBASE + 0x62),
   CNFEV_BRIDGEDELETED          = (CNFEVBASE + 0x63),
   CNFEV_DELETEBRIDGEFAILED     = (CNFEVBASE + 0x64),

   CNFEV_DTMF                   = (CNFEVBASE + 0x71),
   CNFEV_STATUSADDPARTY         = (CNFEVBASE + 0x72),
   CNFEV_STATUSPARTYREMOVED     = (CNFEVBASE + 0x73)
} EEventValue;

#define CNF_FAILURE -1

typedef enum
{
 CNF_SUCCESS = 0,
   ECNF_BUFFERTOOSMALL,                 /* Buffer too small */
   ECNF_OUTOFMEMORY,                    /* Out of memory */
   ECNF_NULLPOINTERARGUMENT,            /* Null pointer argument */
   ECNF_COMMANDNOTSUPPORTED,            /* Command not supported */
   ECNF_INVALIDDEVICEHANDLE,            /* Invalid device handle */
   ECNF_INVALIDPHYSICALBOARD,           /* Invalid physical board */
   ECNF_COMMANDTIMEDOUT,                /* Command timed-out */
   ECNF_TIMESLOTDOLERFAILED,            /* TimeslotDoler failed */
   ECNF_INVALIDPARTY,                   /* Invalid party (timeslot?) */
   ECNF_INVALIDATTRIBUTE,               /* Invalid attribute */
   ECNF_INVALIDEVENT,                   /* Invalid event */
   ECNF_INVALIDARGUMENT,                /* Invalid argument */
   ECNF_INVALIDCONFNAME,                /* Invalid conference name */
   ECNF_INVALIDTIMESLOT,                /* Invalid timeslot */
   ECNF_INVALIDBRIDGE,                  /* Invalid bridge */
   ECNF_MAXEDOUTRESOURCES,              /* Maxed out system resources */ 
   ECNF_DM3SUBSYSTEMERROR,              /* DM3 subsystem error */
   ECNF_ECHOCANCELLATIONNOTSUPPORTED,   /* Echo Cancellation Not Supported */
 ECNF_MAX
} EErrorValue;


typedef enum
{
   CNFENB_Event_DTMFDetection = 0x01,   /* triggers CNFEV_DTMF */
   /*
    * Use cnf_EnableEvents() or cnf_DisableEvents() on a conference handle.
    * The CNFEV_DTMF event is returned and sr_getevtdatap() is a pointer to
    * CNF_DIGINFO (see CNF_DIGINFO).
    * To get the DTMF, use 
    * CNF_DIGINFO lDTMF = *((CNF_DIGINFO *)sr_getevtdatap());
    */

   CNFENB_Event_StatusChanged = 0x02
   /*
    * Use cnf_EnableEvents() or cnf_DisableEvents() on a board handle.
    * The events returned are:
    * CNFEV_STATUSADDPARTY or CNFEV_STATUSPARTYREMOVED
    * To get the Party, use
    * unsigned int lPartyID = *((unsigned int *)sr_getevtdatap());
    */
} EListOfEvents;

typedef enum
{
   PARM_VAL_Party_Mode_NULL = 0, /* Null party means no timeslots assigned */
   PARM_VAL_Party_Mode_FULL, /* Full-duplex party */
   PARM_VAL_Party_Mode_RCV,  /* Receive-only party (same as Monitor) */
   PARM_VAL_Party_Mode_XMIT  /* Transmit-only */
} EPARTY_Parm_Party_Mode;
#define PARM_VAL_Party_Mode_Monitor PARM_VAL_Party_Mode_RCV

typedef enum
{
   PARTY_Parm_Party_Mode = 0x100, /* use EPARTY_Parm_Party_Mode */
   PARTY_Parm_Echo_Cancel_En = 0x101, /* 1 = Enable, 0 = Disable */
   PARTY_Parm_Tariff_En = 0x102, /* 1 = Enable, 0 = Disable */
   PARTY_Parm_AGC_En = 0x103, /* 1 = Enable, 0 = Disable */
   PARTY_Parm_Broadcast_En = 0x104, /* 1 = Enable, 0 = Disable */
   PARTY_Parm_Coach_En = 0x105, /* 1 = Enable, 0 = Disable */
   PARTY_Parm_Pupil_En = 0x106, /* 1 = Enable, 0 = Disable */
   PARTY_Parm_ToneClampingPty = 0x107 /* 1 = Enable, 0 = Disable */
} EPartyAttributes;

typedef enum
{
   CONF_Parm_ToneClamping = 0x200, /* 1 = Enable, 0 = Disable */
   CONF_Parm_DTMF_Mask = 0x201,/* CNF_DIG values can be ORed to form the mask */
   CONF_Parm_Notify_Enable = 0x202 /* 1 = Enable, 0 = Disable */
} EConferenceAttributes;

typedef enum
{
   BOARD_Parm_Act_Talk_En = 0x300, /* 0 = Enable, 0xFFFF = Disable */
   BOARD_Parm_ToneClamping = 0x301, /* 1 = Enable, 0 = Disable */
   BOARD_Parm_ActTalkerNotifyInterval = 0x302 /* Interval is x 100 ms. */
} EBoardAttributes;


typedef enum cnf_dig
{
   CNF_DIGNULLMASK      = 0,
   CNF_DIG0             = 0x0001,
   CNF_DIG1             = 0x0002,
   CNF_DIG2             = 0x0004,
   CNF_DIG3             = 0x0008,
   CNF_DIG4             = 0x0010,
   CNF_DIG5             = 0x0020,
   CNF_DIG6             = 0x0040,
   CNF_DIG7             = 0x0080,
   CNF_DIG8             = 0x0100,
   CNF_DIG9             = 0x0200,
   CNF_DIGSTAR          = 0x0400,
   CNF_DIGPOUND         = 0x0800,
   CNF_DIGA             = 0x1000,
   CNF_DIGB             = 0x2000,
   CNF_DIGC             = 0x4000,
   CNF_DIGD             = 0x8000,
   CNF_DIGALL           = 0xFFFF,

   /*
    * These are used by CONF_Parm_DTMF_Mask to add, remove or set the DTMF
    * mask in cnf_SetConferenceAttributes().
    */
   CNF_DIG_ADD_MASK     = 0x10000,
   CNF_DIG_SUB_MASK     = 0x20000,
   CNF_DIG_SET_MASK     = 0x40000
} CNF_DIG;

#define MAX_CNFDIGS 32
typedef struct cnf_diginfo
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of CNF_DIGINFO is CNF_DIGINFO_VERSION_1.
    */
   unsigned int m_nPartyID; /* Party Identifier */
   unsigned int m_nPartyGroup; /* Party Group # */
   unsigned char m_nDigits[MAX_CNFDIGS+1]; /* ASCIIZ Digits */
} CNF_DIGINFO;
#define CNF_DIGINFO_VER(v) ((sizeof(CNF_DIGINFO) << 16) | (0xFFFF & v))
#define CNF_DIGINFO_VERSION_1 CNF_DIGINFO_VER(1)

typedef struct cnf_vol
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of CNF_VOL is CNF_VOL_VERSION_1.
    */
   int m_volEnable; /* 1 = Enable, 0 = Disable */
   int m_volUpDigit; /* use CNF_DIG */
   int m_volDownDigit; /* use CNF_DIG */
   int m_volResetDigit; /* use CNF_DIG */
} CNF_VOL;
#define CNF_VOL_VER(v) ((sizeof(CNF_VOL) << 16) | (0xFFFF & v))
#define CNF_VOL_VERSION_1 CNF_VOL_VER(1)

typedef struct SPartyGroupRes
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSPartyGroupRes is TSPartyGroupRes_VERSION_1.
    */
   unsigned short m_nGrpMaxParties;
   unsigned short m_nGrpFreeParties;
   unsigned short m_nGrpMaxConferences;
   unsigned short m_nGrpFreeConferences;
   unsigned int m_nPartyGroup; /* use in TSConferenceInfo */
} TSPartyGroupRes;
#define TSPartyGroupRes_VER(v) ((sizeof(TSPartyGroupRes) << 16) | (0xFFFF & v))
#define TSPartyGroupRes_VERSION_1 TSPartyGroupRes_VER(1)

typedef struct cnf_res
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of CNF_RES is CNF_RES_VERSION_1.
    */
   unsigned short m_nBrdMaxParties;
   unsigned short m_nBrdFreeParties;
   unsigned short m_nBrdMaxConferences;
   unsigned short m_nBrdFreeConferences;
   unsigned int m_nGroupCount;
   TSPartyGroupRes *m_pGroups;
} CNF_RES;
#define CNF_RES_VER(v) ((sizeof(CNF_RES) << 16) | (0xFFFF & v))
#define CNF_RES_VERSION_1 CNF_RES_VER(1)


typedef struct SAttribute
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of this structure is TSAttribute_VERSION_1.
    */
   unsigned int m_nAttrType;
   /* One of: EPartyAttributes, EConferenceAttributes, EBoardAttributes */
   union
   {
      int m_nVal; /* Actual value of the parameter (see parameter) */
   } uValue;
} TSPartyAttribute, TSConferenceAttribute, TSBoardAttribute, TSAttribute;
#define TSAttribute_VER(v) ((sizeof(TSAttribute) << 16) | (0xFFFF & v))
#define TSAttribute_VERSION_1 TSAttribute_VER(1)

typedef struct SAttributesList
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSAttributesList is TSAttributesList_VERSION_1.
    */
   unsigned int m_nAttrCount;
   TSAttribute *m_pAttrs; /* Array of 'm_nAttrCount' attributes */
} TSAttributesList;
#define TSAttributesList_VER(v) ((sizeof(TSAttributesList) << 16) | (0xFFFF & v))
#define TSAttributesList_VERSION_1 TSAttributesList_VER(1)

typedef struct SEventsList
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSEventsList is TSEventsList_VERSION_1.
    */
   unsigned int m_nEventCount;
   EListOfEvents *m_pEvents; /* Array of 'm_nEventCount' Events */
} TSEventsList;
#define TSEventsList_VER(v) ((sizeof(TSEventsList) << 16) | (0xFFFF & v))
#define TSEventsList_VERSION_1 TSEventsList_VER(1)

typedef struct SPartyInfo
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSPartyInfo is TSPartyInfo_VERSION_1.
    */
   unsigned int m_nPartyID; /* Party Identifier (returned) */
   int m_nHandle; /* Party handle as returned by xx_open (eg dx_open) */
   unsigned int m_nAttrCount;
   TSPartyAttribute *m_pAttrs; /* Array of 'm_nAttrCount' attributes */
} TSPartyInfo;
#define TSPartyInfo_VER(v) ((sizeof(TSPartyInfo) << 16) | (0xFFFF & v))
#define TSPartyInfo_VERSION_1 TSPartyInfo_VER(1)

typedef struct SPartyList
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSPartyList is TSPartyList_VERSION_1.
    */
   unsigned int m_nPartyCount;
   TSPartyInfo *m_pPartyInfo; /* Array of 'm_nPartyCount' Parties */ 
} TSPartyList;
#define TSPartyList_VER(v) ((sizeof(TSPartyList) << 16) | (0xFFFF & v))
#define TSPartyList_VERSION_1 TSPartyList_VER(1)

typedef struct SThirdPartyInfo
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSThirdPartyInfo is TSThirdPartyInfo_VERSION_1.
    */
   unsigned int m_nPartyID; /* Party Identifier (returned) */
   long m_nTxTimeslot;
   long m_nRxTimeslot;
   unsigned int m_nAttrCount;
   TSPartyAttribute *m_pAttrs; /* Array of 'm_nAttrCount' attributes */
} TSThirdPartyInfo;
#define TSThirdPartyInfo_VER(v) ((sizeof(TSThirdPartyInfo) << 16) | (0xFFFF & v))
#define TSThirdPartyInfo_VERSION_1 TSThirdPartyInfo_VER(1)

typedef struct SThirdPartyList
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSThirdPartyList is TSThirdPartyList_VERSION_1.
    */
   unsigned int m_nPartyCount;
   TSThirdPartyInfo *m_pPartyInfo; /* Array of 'm_nPartyCount' Parties */ 
} TSThirdPartyList;
#define TSThirdPartyList_VER(v) ((sizeof(TSThirdPartyList) << 16) | (0xFFFF & v))
#define TSThirdPartyList_VERSION_1 TSThirdPartyList_VER(1)

typedef struct SConferenceInfo
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSConferenceInfo is TSConferenceInfo_VERSION_1.
    */
   unsigned int m_nAttrCount;
   TSConferenceAttribute *m_pAttrs; /* Array of 'm_nAttrCount' attributes */
   unsigned int m_nPartyGroup;
} TSConferenceInfo;
#define TSConferenceInfo_VER(v) ((sizeof(TSConferenceInfo) << 16) | (0xFFFF & v))
#define TSConferenceInfo_VERSION_1 TSConferenceInfo_VER(1)

#define MAX_CNFNAME 20
typedef struct SCreateConferenceReply
{
   unsigned int m_nVersion; /* Version of this structure */
   /* The version is used so that an app can be binary compatible with
    * future changes to this structure.
    * The current version of TSCreateConferenceReply is TSCreateConferenceReply_VERSION_1.
    */
   int m_nConfHandle;
   char m_szConfName[MAX_CNFNAME + 1];
} TSCreateConferenceReply;
#define TSCreateConferenceReply_VER(v) ((sizeof(TSCreateConferenceReply) << 16) | (0xFFFF & v))
#define TSCreateConferenceReply_VERSION_1 TSCreateConferenceReply_VER(1)


#ifdef _WIN32

#ifdef DllExport
#undef DllExport
#endif
#define DllExport __declspec( dllexport )

#ifdef DllLinkage
#undef DllLinkage
#endif
#define DllLinkage DllExport int __cdecl

#else /*_WIN32*/
#define DllLinkage extern int
#endif/*_WIN32*/

#ifdef __cplusplus
extern "C" {    // C Plus Plus function bindings
#endif

DllLinkage cnf_Open (int *a_pnBrdHandle, const char *a_szName, int a_oFlags);
DllLinkage cnf_Close (int a_nBrdHandle);
DllLinkage cnf_CreateConference (int a_nBrdHandle, TSConferenceInfo *a_pConfInfo, TSCreateConferenceReply *a_pConfReply, void *a_pUserContext, int a_mode);
DllLinkage cnf_AttachConference (int a_nBrdHandle, char *a_szConfName, TSCreateConferenceReply *a_pConfReply, void *a_pUserContext, int a_mode);
DllLinkage cnf_DeleteConference (int a_nBrdHandle, int a_nConfHandle, void *a_pUserContext, int a_mode);
DllLinkage cnf_DeleteAllConferences (int a_nBrdHandle, void *a_pUserContext, int a_mode);
DllLinkage cnf_AddParty (int a_nConfHandle, TSPartyList *a_pPartyList, void *a_pUserContext, int a_mode);
DllLinkage cnf_AddPartyByTimeslot (int a_nConfHandle, TSThirdPartyList *a_pThirdPartyList, void *a_pUserContext, int a_mode);
DllLinkage cnf_RemoveParty (int a_nConfHandle, TSPartyList *a_pPartyList, void *a_pUserContext, int a_mode);
DllLinkage cnf_RemoveAllParties (int a_nConfHandle, void *a_pUserContext, int a_mode);
DllLinkage cnf_EnableEvents (int a_nHandle, TSEventsList *a_pEvents, void *a_pUserContext, int a_mode);
DllLinkage cnf_DisableEvents (int a_nHandle, TSEventsList *a_pEvents, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetActiveTalkers (int a_nConfHandle, TSPartyList *a_pPartyList, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetPartyList (int a_nConfHandle, TSPartyList *a_pPartyList, void *a_pUserContext, int a_mode);
DllLinkage cnf_SetVolumeControl (int a_nBrdHandle, CNF_VOL *a_pVol, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetVolumeControl (int a_nBrdHandle, CNF_VOL *a_pVol, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetResourceCount (int a_nBrdHandle, CNF_RES *a_pRes, void *a_pUserContext, int a_mode);
DllLinkage cnf_SetBoardAttributes (int a_nBrdHandle, TSAttributesList *a_pAttributes, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetBoardAttributes (int a_nBrdHandle, TSAttributesList *a_pAttributes, void *a_pUserContext, int a_mode);
DllLinkage cnf_SetConferenceAttributes (int a_nConfHandle, TSAttributesList *a_pAttributes, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetConferenceAttributes (int a_nConfHandle, TSAttributesList *a_pAttributes, void *a_pUserContext, int a_mode);
DllLinkage cnf_SetPartyAttributes (int a_nConfHandle, unsigned int a_nPartyId, TSAttributesList *a_pAttributes, void *a_pUserContext, int a_mode);
DllLinkage cnf_GetPartyAttributes (int a_nConfHandle, unsigned int a_nPartyId, TSAttributesList *a_pAttributes, void *a_pUserContext, int a_mode);
DllLinkage cnf_CreateBridge (int a_nConfHandle1, int a_nConfHandle2, void *a_pUserContext, int a_mode);
DllLinkage cnf_DeleteBridge (int a_nConfHandle1, int a_nConfHandle2, void *a_pUserContext, int a_mode);

#ifdef __cplusplus
}               // C Plus Plus function bindings
#endif

#ifdef _WIN32
#undef DllLinkage
#endif/*_WIN32*/


#endif /* _CNFLIB_H_ */

