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
/**********************************************************************
*
*   C Header:       gccfgparm.h
*   Instance:       gc_1
*   Description:    
*   %date_created:  Mon Sept 12 15:27:01 2005 %
*
**********************************************************************/

#ifndef _gc_1_gccfgparm_h_H
#define _gc_1_gccfgparm_h_H

#include <gclib.h>
#include <ipmedia_codecs.h>

/***************************************************************************************
* This header file contains information about configurable parameters only             *
* defined in GCLib module.  Other configurable parameters that are defined in other    *
* software modules may be found in similar header files of those software modules.     *
*                                                                                      *
* The set ID is used to represent a group of parameters that are closely related       *
* All set IDs in GlobalCall are divided into four groups according to the software     *
* module categories (that are GCLib, CCLib, Protocol, Firmware). Each software         *
* module category has a unique set ID range.  For example, GCLib category has          * 
* a different set ID range from CCLib category.                                        *
*                                                                                      *
* The range of set IDs defined in GCLib module is further divided into two             *
* sub-groups: the parameters maintained in GCLib and the parameters maintained         *
* in CCLibs.  The parameters maintained in CCLibs are grouped into non-technology      *
* and technology specific ones. For those technology specific parameters, each         *
* individual technology has its set ID range.                                          *
*                                                                                      *
* The set ID range of CCLib module category is further divided according to            * 
* CCLib IDs.  In other words, each individual CCLib module has its set ID              *
* range respect to its CCLib ID.  For example, PDKRT and ISDN call control library     *
* has a different set ID range.                                                        *
* However, all protocol modules in protocol category shares the same set ID range.     *
*                                                                                      *
* Generally, the parameters defined in GCLib module are common and thus can be         *
* applied to across all GlobalCall software modules (e.g., CCLib modules) while        *
* non-GCLib defined parameters can only be only applicable to a specific software      *
* module that defines them (e.g., the parameter for a protocol).                       *
*                                                                                      *
* This header file only lists the set ID and parm ID defines of GCLib defined          *
* parameters.  Other information, like target object types that support a parameter,   *
* parameter value type, and updateable permission flag, are given in the GlobalCall    *
* API document.                                                                        *
*                                                                                      *
* Caution: Since those parameters are common across different technologies, they are   *
* defined in this file but they may be used in different individual software modules.  *
* User should refer to the related manuals or documents before using them.             *
*                                                                                      *
****************************************************************************************/

/***************************************************************************************
* The ranges of set IDs for different software module types are as follows             *
***************************************************************************************/

/*
  GCLib defined set IDs must be from 0 to 0x3FFFh (0 to 16383)
*/

/*
  The parameters are defined and also maintained or used in GCLib from 0x000h to
  0x0FFFh (0 to 4095)
*/ 
#define GC_GCLIB_USE_SET_MIN        0
#define GC_GCLIB_USE_SET_MAX        0x0FFF

/* 
  The parameters are defined in this header file but maintained in other software modules
  from 0x1000h to 0x3FFFh (4096 to 16383).  The range is divided into non-technology and
  technology specific sub-ranges. 
*/
#define GC_GCLIB_DEF_SET_MIN        0x1000
#define GC_GCLIB_DEF_SET_MAX        0x3FFF

/* 
  The parameters are not technology specific but maintained in other software modules.
  Range is from 0x1000h to 0x2FFFh (4096 to 12287) 
*/
#define GC_GCLIB_GEN_SET_MIN        GC_GCLIB_DEF_SET_MIN
#define GC_GCLIB_GEN_SET_MAX        0x2FFF

/* 
  The parameters are technology specific and maintained in other software modules.
  The set ID ranges of technologies from 0x3000h to 0x3FFFh (12288 to 16383).
  Maximum number of technologies is limited to 16.  
  Each TECHNOLOGY has a set ID span 0xFFh (256)
*/
#define GC_GCLIB_TECH_SET_MIN       0x3000
#define GC_GCLIB_TECH_SET_MAX       0x3FFF

/* The IP technology must be from 0x3000h to 0x31FFh (16384-12799) */
#define GC_IP_TECH_SET_MIN          0x3000 
#define GC_IP_TECH_SET_MAX          0x30FF

/* 
 * The set ranges for any new technology shall be defined here
 * .... 
 */





/***************************************************************************************
*  The following defines are set ID ranges of CCLib modules from 0x4000h to 0x9FFFh
*  (16384--40959).  Total number of CCLibs is limited to 60.
*  Each CCLib module has set ID span 0xFFh (256)
***************************************************************************************/

#define GC_CCLIB_SET_START          0x4000
#define GC_CCLIB_SET_SPAN           0xFF 

/* The ICAP library must be from 0x4100h to 0x41FFh (16640-16895) */
#define GC_ICAPILIB_SET_MIN         (GC_CCLIB_SET_START + (GC_ICAPI_LIB << 8)) 
#define GC_ICAPILIB_SET_MAX         (GC_ICAPILIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The ISDN library must be from 0x4200h to 0x42FFh (16896-17151) */
#define GC_ISDNLIB_SET_MIN          (GC_CCLIB_SET_START + (GC_ISDN_LIB << 8))
#define GC_ISDNLIB_SET_MAX          (GC_ISDNLIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The ANALOG library must be from 0x4300h to 0x43FFh (17152-17407) */
#define GC_ANAPILIB_SET_MIN         (GC_CCLIB_SET_START + (GC_ANAPI_LIB << 8))
#define GC_ANAPILIB_SET_MAX         (GC_ANAPILIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The PDKRT library must be from 0x4400h to 0x44FFh (17408-17663) */
#define GC_PDKRTLIB_SET_MIN         (GC_CCLIB_SET_START + (GC_PDKRT_LIB << 8))
#define GC_PDKRTLIB_SET_MAX         (GC_PDKRTLIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The SS7 library must be from 0x4500h to 0x45FFh (17664-17919) */
#define GC_SS7LIB_SET_MIN           (GC_CCLIB_SET_START + (GC_SS7_LIB << 8))
#define GC_SS7LIB_SET_MAX           (GC_SS7LIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The DM3 library must be from 0x4600h to 0x46FFh (17920-18175) */
#define GC_DM3CCLIB_SET_MIN         (GC_CCLIB_SET_START + (GC_DM3CC_LIB << 8))
#define GC_DM3CCLIB_SET_MAX         (GC_DM3CCLIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The IP Media library must be from 0x4700h to 0x47FFh (18176-18431) */
#define GC_IPMLIB_SET_MIN           (GC_CCLIB_SET_START + (GC_IPM_LIB << 8))
#define GC_IPMLIB_SET_MAX           (GC_IPMLIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The IP H323 library must be from 0x4800h to 0x48FFh (18432-18687) */
#define GC_H3RLIB_SET_MIN           (GC_CCLIB_SET_START + (GC_IPM_LIB << 8))
#define GC_H3RLIB_SET_MAX           (GC_H3RLIB_SET_MIN | GC_CCLIB_SET_SPAN)

/* The Diva ISDN cc library uses same range as DM3CC to be compatible */
#define GC_DIVAISDN_SET_MIN         (GC_CCLIB_SET_START + (GC_DM3CC_LIB << 8))
#define GC_DIVAISDN_SET_MAX         (GC_DIVAISDN_SET_MIN | GC_CCLIB_SET_SPAN)

/* 
 * The set ranges for any new CCLib shall be defined here
 * .... 
 */


/* The Customer library 1 must be from 0x9E00h to 0x9EFFh (40448-40703) */
#define GC_CUSTOM1LIB_SET_MIN       0x9E00
#define GC_CUSTOM1LIB_SET_MAX       (GC_CUSTOM1LIB_SET_MIN | GC_CCLIB_SET_SPAN)


/* Protocol defined set IDs must be from 0xA000h to 0xBFFFh (40960--49165) */
#define GC_PROTOCOL_SET_MIN         0xA000
#define GC_PROTOCOL_SET_MAX         0xBFFF

/* Firmware defined set IDs must be from 0xC000h to 0xFFFFh (49166--65535) */
#define GC_FIRMWARE_SET_MIN         0xC000
#define GC_FIRMWARE_SET_MAX         0xFFFF




/*********************************************************************************** 
* The following defines the possible value types for configuration parameters (used 
* in parameter info define macros) and for the value_type field in GC_PARM_ID data
* structure
************************************************************************************/
typedef enum {
    GC_VALUE_INT          =      0x1,     /* Integer data ==> int */ 
    GC_VALUE_LONG         =      0x2,     /* Long integer data ==> long */
    GC_VALUE_SHORT        =      0x3,     /* Short integer data ==> short */
    GC_VALUE_CHAR         =      0x4,     /* Character data ==> char */
    GC_VALUE_STRING       =      0x5,     /* Character string data ==> char* */
    GC_VALUE_UINT         =      0x6,     /* Unsigned Integer data ==> int */ 
    GC_VALUE_ULONG        =      0x7,     /* Unsigned Long integer data ==> long */
    GC_VALUE_USHORT       =      0x8,     /* Unsigned Short integer data ==> short */
    GC_VALUE_UCHAR        =      0x9,     /* Unsigned Character data ==> char */
    GC_VALUE_PARM_ID      =      0xF,     /* GC_PARM_ID data structure that only used for gc_QueryConfigData() */
    GC_VALUE_CAS_TRANS    =      0x10,    /* CAS Transition data struture ==> GC_CASPROT_TRANS */
    GC_VALUE_CAS_PULSE    =      0x11,    /* CAS Pulse data struture ==> GC_CASPROT_PULSE */
    GC_VALUE_CAS_TRAIN    =      0x12,    /* CAS Train data struture ==> GC_CASPROT_TRAIN */
 /*
 * New parm value type shall be defined before this line
 */
    GC_VALUE_UNKNOWN      =     0xFF
} GC_PARM_VALUETYPE;


/*
 * The following defines the possible update flag values for configuration parameters
 * (used in parmater info define macros)and for the update_perm field 
 * in GC_PARM_ID data structure 
 */
#define     GC_W_I                  0       /* GC_W_I <==> update immediately*/
#define     GC_R_O                  1       /* GC_R_O <==> retrieve only */
#define     GC_W_N                  2       /* GC_W_N <==> update only at GC null state */
#define     GC_W_X                  3       /* GC_W_X <==> not available */




/***********************************************************************************/
/*  The following set ID and parameter ID defines are defined and maintained (used)*/
/*  in GCLib.                                                                      */
/*  Range: 0x0000 to 0x0FFF                                                        */
/***********************************************************************************/

/* 
 *
 * The following set ID is generic
 *
 */
#define     GCSET_GENERIC           0x0 /* Generic set */

/*The following are the parameters that supports set ID = GCSET_GENERIC */
#define     GCPARM_GENERIC          0x0 /* generic parm */




/* 
 *
 * The following set ID is for device information
 *
 */
#define     GCSET_DEVICEINFO        0x1 /* Protocol information set */

/* The following parm IDs support setID = GCSET_PROTOCOL */
#define     GCPARM_DEVICENAME       0x1 /* device name */
#define     GCPARM_NETWORKH         0x2 /* network handle */
#define     GCPARM_VOICENAME        0x3 /* voice name */
#define     GCPARM_VOICEH           0x4 /* voice handle */
#define     GCPARM_CALLSTATE        0x5 /* call state */
#define     GCPARM_BOARD_LDID       0x6 /* board line device ID */




/* 
 *
 * The following set ID is for protocol information
 *
 */
#define     GCSET_PROTOCOL          0x2 /* Protocol information set */

/* The following parm IDs support setID = GCSET_PROTOCOL */
#define     GCPARM_PROTOCOL_ID      0x1 /* protocol ID */
#define     GCPARM_PROTOCOL_NAME    0x2 /* protocol Name */




/* 
 *
 * The following set ID is for CCLib information
 *
 */
#define     GCSET_CCLIB_INFO        0x3 /* CCLib information set */

/* The following parm IDs support setID = GCSET_CCLIB_INFO */
#define     GCPARM_CCLIB_ID         0x1 /* CCLib ID */
#define     GCPARM_CCLIB_NAME       0x2 /* CCLib Name */




/* 
 *
 * The following set ID is for call event mask of a time slot
 *
 */
#define     GCSET_CALLEVENT_MSK     0x8 /* Call event mask set*/
#define     GCPARM_GET_MSK          0x0 /* generic parm */ 
/* Other parm IDs that support set ID = GCSET_CALLEVENT_MSK include */
/* GCACT_SETMSK, GCACT_ADDMSK, and GCACT_SUBMSK */



/* 
 *
 * The following set ID is for call state mask of a time slot
 *
 */  
#define     GCSET_CALLSTATE_MSK     0x9 /* call state mask set */

/* The parm IDs that support set ID = GCSET_CALLSTATE_MSK include GCPARM_GET_MSK, */
/* GCACT_SETMSK, GCACT_ADDMSK, and GCACT_SUBMSK */



/* 
 *
 * The following set ID is for CRN value by given index of a time slot
 *
 */
#define     GCSET_CRN_INDEX         0xA /* CRN index set */

/* The following parm IDs support setID = GCSET_CRN_INDEX */
#define     GCPARM_1ST_CRN          0x0 /* The first crn of a time slot line device */
#define     GCPARM_2ND_CRN          0x1 /* The second crn of a time slot line device */

/* 
 *
 * The following set ID is used to inform applications that the GC_PARM_DATA.value_buf
 * data cannot be returned because the value_buf size is > 255 bytes. This scenario can 
 * occur if an application uses gc_util_next_parm() or gc_util_find_parm() and receives 
 * a GC_PARM_BLK that contains GC_PARM_DATA > 255 bytes. 
 *
 * To receive parm data > 255 bytes, applications must use gc_util_next_parm_ex() or 
 * gc_util_find_parm_ex().
 *
 */
#define     GC_SET_LARGE_PARM_DATA  0x0FFF /* Protocol information set */


/*
 *
 *  The set ID and parm ID for the new GCLib-maintained parameters shall be defined here
 *
 */



/***********************************************************************************/
/*  The following set ID and parameter ID defines are defined in GCLib but         */
/*  maintained (used) in CCLib.                                                    */
/*  Range: 0x1000 - 0x3FFF                                                         */
/***********************************************************************************/

/*
 * 
 * The following set ID is for call information:
 * call info set used as an alternative to gc_GetCallInfo.
 * Also used in gc_SetConfigData/gc_GetConfigData
 */
#define     GCSET_CALLINFO          0x1001  /*  */
/* The parm IDs that support set ID = GCSET_CALLINFO include CALLINFOTYPE, */
/* CATEGORY_DIGIT, CONNECT_TYPE and GCPARM_CONSDROP_HKFLASH_OVERRIDE*/


/* Parameter ID for GCSET_CALLINFO used in gc_SetConfigData/gc_GetConfigData*/
#define GCPARM_CONSDROP_HKFLASH_OVERRIDE			1


/* The defines for Parm ID GCPARM_CONSDROP_HKFLASH_OVERRIDE*/
#define GCPV_DISABLED							0
#define GCPV_DBL_HKFLASH						1
#define GCPV_SINGLE_HKFLASH						2



/* 
 *
 * The following set ID is for call information:
 * parm set used as an alternative to gc_SetParm/gc_GetParm
 *
 */
#define     GCSET_PARM              0x1002
/* The parm IDs that support set ID = GCSET_PARM include GCPR_CALLINGPARTY */
/* and GCPR_LOADTONES */




/*
 *
 *  The following set ID is for configuring the sending of call acknowledgement
 *  and call proceeding indication.
 *
 */
#define     GCSET_CALL_CONFIG       0x1003  

/* Parameter IDs for GCSET_CALL_CONFIG */
#define     GCPARM_CALLACK          1   /* determines who sends the Call Acknowledgment */
#define     GCPARM_CALLPROC         2   /* determines who sends the Call Proceeding */
#define     GCPARM_MIN_INFO         3   /* specifies minimum amount of info/digits to receive  */
#define     GCPARM_MAX_INFO         4   /* specifies maximum amount of info/digits to receive  */

/* Possible values for GCPARM_CALLACK and GCPARM_CALLPROC parameters */
#define     GCCONTROL_APP           1   /* Application controlled sending */
#define     GCCONTROL_TCCL          2   /* Technology call control layer controlled sending */




/*
 *
 *  The following set ID is for setting the channel capabilities
 *
 */
#define     GCSET_CHAN_CAPABILITY   0x1004  /* MakeCall block - Channel capability parameters */

/* Parameter IDs for GCSET_CHAN_CAPABILITY */
#define     GCPARM_TYPE                     1    /* Type of capability */
#define     GCPARM_CAPABILITY               2    /* Capability of the channel */ 
#define     GCPARM_RATE                     3    /* Information transfer rate */ 

/* Possible values for GCPARM_TYPE parameter in the GCSET_CHAN_CAPABILITY set.*/
#define     GCCAPTYPE_VIDEO                 1    /* Video */
#define     GCCAPTYPE_AUDIO                 2    /* Audio */
#define     GCCAPTYPE_3KHZ_AUDIO            3    /* 3Khz Audio */
#define     GCCAPTYPE_7KHZ_AUDIO            4    /* 7Khz Audio */
#define     GCCAPTYPE_RDATA                 5    /* Raw data */
#define     GCCAPTYPE_UDATA                 6    /* User data */
#define     GCCAPTYPE_UNDEFINED             7    /* Undefined capability type */
#define     GCCAPTYPE_MUX                   8    /* Multiplexed capability */
#define     GCCAPTYPE_DTMF                  9    /* DTMF capability */

/* Possible values for GCPARM_CAPABILITY parameter in the GCSET_CHAN_CAPABILITY set.*/
/* Data Capabilites */
#define     GCCAP_DATA_t120           CODEC_DATA_T120         /* 1: T.120 data protocol chosen */
#define     GCCAP_DATA_dsm_cc         CODEC_DATA_DSC_CC       /* 2: DSM-cc data protocol chosen */   
#define     GCCAP_DATA_usrData        CODEC_DATA_USRDATA      /* 3: User data protocol chosen */ 
#define     GCCAP_DATA_t84            CODEC_DATA_T84          /* 4: T.84 data protocol chosen. */    
#define     GCCAP_DATA_t434           CODEC_DATA_T434         /* 5: T.434 data protocol chosen. */   
#define     GCCAP_DATA_h224           CODEC_DATA_H224         /* 6: H.224 data protocol chosen. */   
#define     GCCAP_DATA_nlpd           CODEC_DATA_NLPD         /* 7: NLPD data protocol chosen. */    
#define     GCCAP_DATA_dsvdControl    CODEC_DATA_DSVDCONTROL  /* 8: DSVD control data protocol chosen. */    
#define     GCCAP_DATA_h222           CODEC_DATA_H222         /* 9: H.222 data protocol chosen */    
#define     GCCAP_DATA_t30Fax         CODEC_DATA_T30FAX       /* 10: T.30 Fax protocol chosen */  
#define     GCCAP_DATA_t140           CODEC_DATA_T140         /* 11: T.140 data protocol chosen */    
#define     GCCAP_DATA_t38UDPFax      CODEC_DATA_T38FAX       /* 12: T.38 Fax protocol over UDP chosen */  
#define     GCCAP_DATA_CCITTV110      CODEC_DATA_CCITTV110    /* 13: CCITT V.110 standard */
#define     GCCAP_DATA_CCITTV120      CODEC_DATA_CCITTV120    /* 14: CCITT V.120 standard */  
#define     GCCAP_DATA_CCITTX31       CODEC_DATA_CCITTX31     /* 15: CCITT X.31 standard */   
#define     GCCAP_DATA_nonStandard    CODEC_DATA_NONSTANDARD  /* 16: non-Standard Data capability */

/* Audio Capabilities */
#define     GCCAP_AUDIO_nonstandard   CODEC_AUDIO_NONSTANDARD /* 100: Non standard audio codec chosen */
#define     GCCAP_AUDIO_g711Alaw64k   CODEC_AUDIO_G711ALAW64K /* 101: G.711 audio, A-law, 64k */
#define     GCCAP_AUDIO_g711Alaw56k   CODEC_AUDIO_G711ALAW56K /* 102: G.711 audio, A-law, 56k */
#define     GCCAP_AUDIO_g711Ulaw64k   CODEC_AUDIO_G711ULAW64K /* 103: G.711 audio, U-law, 64k */
#define     GCCAP_AUDIO_g711Ulaw56k   CODEC_AUDIO_G711ULAW56K /* 104: G.711 audio, U-law, 56k */
#define     GCCAP_AUDIO_G721ADPCM     CODEC_AUDIO_G721ADPCM   /* 105: ADPCM */ 
#define     GCCAP_AUDIO_g722_48k      CODEC_AUDIO_G722_48K    /* 106: G.722 audio 48k */ 
#define     GCCAP_AUDIO_g722_56k      CODEC_AUDIO_G722_56K    /* 107: G.722 audio 56k */ 
#define     GCCAP_AUDIO_g722_64k      CODEC_AUDIO_G722_64K    /* 108: G.722 audio 64k */ 
#define     GCCAP_AUDIO_g7231         GCCAP_AUDIO_g7231_6_3k  /* 111: G.723.1 at received bit rate */
#define     GCCAP_AUDIO_g7231_5_3k    CODEC_AUDIO_G7231_5_3K  /* 109: G.723.1 transmit at 5.3 kbit/s */   
#define     GCCAP_AUDIO_g7231_6_3k    CODEC_AUDIO_G7231_6_3K  /* 110: G.723.1 transmit at 6.3 kbit/s */   
#define     GCCAP_AUDIO_g726_16k      CODEC_AUDIO_G726_16K    /* 111: G.726 at 16 kbit/s. Mapped to 
                                                                      nonstandard coder in H.245 as G.726.
                                                                      is not yet specified in H.245 */
#define     GCCAP_AUDIO_g726_24k      CODEC_AUDIO_G726_24K    /* 112: G.726 at 24 kbit/s. Mapped to 
                                                                      nonstandard coder in H.245 as G.726.
                                                                      is not yet specified in H.245 */
#define     GCCAP_AUDIO_g726_32k      CODEC_AUDIO_G726_32K    /* 113: G.726 at 32 kbit/s. Mapped to 
                                                                      nonstandard coder in H.245 as G.726
                                                                      is not yet specified in H.245 */
#define     GCCAP_AUDIO_g726_40k      CODEC_AUDIO_G726_40K    /* 114: G.726 at 40 kbit/s. Mapped to
                                                                      nonstandard coder in H.245 as G.726
                                                                      is not yet specified in H.245 */
#define     GCCAP_AUDIO_g728          CODEC_AUDIO_G728        /* 115: G.728 audio at 16 kbit/s */  
#define     GCCAP_AUDIO_g729          CODEC_AUDIO_G729        /* 116: G.729 audio at 8 kbit/s */   
#define     GCCAP_AUDIO_g729AnnexA    CODEC_AUDIO_G729ANNEXA  /* 118: G.729AnnexA audio at 8 kbit/s */ 
#define     GCCAP_AUDIO_g729wAnnexB   CODEC_AUDIO_G729WANNEXB /* 119: G.729 audio at 8 kbit/s with silence
                                                                      suppression as in Annex B */    
#define     GCCAP_AUDIO_g729AnnexAwAnnexB          CODEC_AUDIO_G729ANNEXAWANNEXB /* 120: G.729AnnexA audio at 
                                                                                         8 kbit/s with silence 
                                                                                         suppression as in 
                                                                                         Annex B */
#define     GCCAP_AUDIO_g7231AnnexCCapability      CODEC_AUDIO_G7231ANNEXCCAP /* 121: G.723.1 with Annex C */    
#define     GCCAP_AUDIO_gsmFullRate                CODEC_AUDIO_GSMFULLRATE    /* 122: Full-rate speech transcoding 
                                                                                     (GSM 06.10) */  
#define     GCCAP_AUDIO_gsmHalfRate                CODEC_AUDIO_GSMHALFRATE    /* 123: Half-rate speech transcoding 
                                                                                     (GSM 06.20) */  
#define     GCCAP_AUDIO_gsmEnhancedFullRate        CODEC_AUDIO_GSMEFULLRATE   /* 124: Enhanced Full Rate (EFR) 
                                                                                      speech transoding (GSM 06.60) */  
#define     GCCAP_AUDIO_gsmAdaptiveMultiRate       CODEC_AUDIO_GSMADAPTIVEMULTIRATE  /* 125: GSM 06.90 AdaptiveMultRate.
                                                                                             Mapped to nonstandard coder 
                                                                                             in H.245 as GSM AMR not 
                                                                                             yet specified in H.245 */
#define     GCCAP_AUDIO_is11172                    CODEC_AUDIO_IS11172        /* 126: is11172AudioCapability_chosen */ 
#define     GCCAP_AUDIO_is13818                    CODEC_AUDIO_IS13818        /* 127: is13818AudioCapability_chosen */ 
#define     GCCAP_AUDIO_is127EnhancedVariableRate  CODEC_AUDIO_IS127EVARIABLERATE  /* 128: TIA/EIA standard IS-127 
                                                                                           transcoding. Mapped to nonstandard
                                                                                           coder in H.245 as IS-127 EVRC 
                                                                                           is not yet specified in H.245.*/
#define     GCCAP_AUDIO_disabled                   129                        /* fax or data call only - no initial audio */
#define     GCCAP_AUDIO_AMRNB_4_75k                130                        /* 130: CODEC_AUDIO_AMRNB_4_75k - GSM AMR rate is 4.75k*/
#define     GCCAP_AUDIO_AMRNB_5_15k                131                        /* 131: CODEC_AUDIO_AMRNB_5_15k - GSM AMR rate is 5.15kk*/
#define     GCCAP_AUDIO_AMRNB_5_9k                 132                        /* 132: CODEC_AUDIO_AMRNB_5_9k - GSM AMR rate is 5.9k*/
#define     GCCAP_AUDIO_AMRNB_6_7k                 133                        /* 133: CODEC_AUDIO_AMRNB_6_7k - GSM AMR rate is 6.7k*/
#define     GCCAP_AUDIO_AMRNB_7_4k                 134                        /* 134: CODEC_AUDIO_AMRNB_7_4k - GSM AMR rate is 7.4k*/
#define     GCCAP_AUDIO_AMRNB_7_95k                135                        /* 135: CODEC_AUDIO_AMRNB_7_95k - GSM AMR rate is 7.95k*/
#define     GCCAP_AUDIO_AMRNB_10_2k                136                        /* 136: CODEC_AUDIO_AMRNB_10_2k - GSM AMR rate is 10.2k*/
#define     GCCAP_AUDIO_AMRNB_12_2k                137                        /* 137: CODEC_AUDIO_AMRNB_12_2k - GSM AMR rate is 12.2k*/

/* Video Capabilites */
#define     GCCAP_VIDEO_nonstandard   CODEC_VIDEO_NONSTANDARD     /* 200: Non standard video codec chosen */   
#define     GCCAP_VIDEO_h261          CODEC_VIDEO_H261            /* 201: H.261 video codec */ 
#define     GCCAP_VIDEO_h262          CODEC_VIDEO_H262            /* 202: H.261 video codec */ 
#define     GCCAP_VIDEO_h263          CODEC_VIDEO_H263            /* 203: H.263 video codec */ 
#define     GCCAP_VIDEO_is11172       CODEC_VIDEO_IS11172         /* 204: IS11172 video codec */   

/* Other Capabilities */
#define     GCCAP_txEncryption        CODEC_OTHER_TXENCRYPTION    /* 300: Transmit enxryption capability */    
#define     GCCAP_rxEncryption        CODEC_OTHER_RXENCRYPTION    /* 301: Receive enxryption capability */ 
#define     GCCAP_conference          CODEC_OTHER_CONFERENCE      /* 302: Conference capability */
#define     GCCAP_h235Security        CODEC_OTHER_H235SECURITY    /* 303: H.235 security capability */ 
#define     GCCAP_clientUserInput     CODEC_OTHER_CLIENTUSERINPUT /* 304: Client user input, used for DTMF tones */    
#define     GCCAP_muxNonStandard      CODEC_OTHER_MUXNONSTANDARD  /* 305: Non standard Mux capability */   
#define     GCCAP_muxH222             CODEC_OTHER_MUXH222         /* 306: H.222 Mux capability */  
#define     GCCAP_muxH223             CODEC_OTHER_MUXH223         /* 307: H.223 Mux capability */  
#define     GCCAP_muxVgMux            CODEC_OTHER_MUXVGMUX        /* 308: VG Mux capability */ 
#define     GCCAP_muxH2250            CODEC_OTHER_MUXH2250        /* 309: H.225.0 Mux capability */    
#define     GCCAP_muxH223AnnexA       CODEC_OTHER_MUXH223ANNEXA   /* 310: H.223 Annex A Mux capability */
#define     GCCAP_unknown             CODEC_OTHER_UNKNOWN         /* 311: unknown capability */
#define     GCCAP_noChange            CODEC_OTHER_NOCHANGE        /* 312: use previous capability */
#define     GCCAP_dontCare            CODEC_OTHER_DONTCARE        /* 313: use any capability */
#define     GCCAP_nonStandard         CODEC_OTHER_NONSTANDARD     /* 314: non-Standard capability */


/* Possible values for GCPARM_RATE parameter in the GCSET_CHAN_CAPABILITY set.*/
#define     GCCAPRATE_EINI460               1       
#define     GCCAPRATE_50                    2    /* 50 bps */
#define     GCCAPRATE_75                    3    /* 75 bps */
#define     GCCAPRATE_100                   4    /* 100 bps */
#define     GCCAPRATE_134                   5    /* 134 bps */
#define     GCCAPRATE_110                   6    /* 110 kbps */
#define     GCCAPRATE_150                   7    /* 150 kbps */
#define     GCCAPRATE_200                   8    /* 200 kbps */
#define     GCCAPRATE_300                   9    /* 300 kbps */
#define     GCCAPRATE_600                   10   /* 600 kbps */
#define     GCCAPRATE_1200                  11   /* 1.2 kbps */
#define     GCCAPRATE_2400                  12   /* 2.4 kbps */
#define     GCCAPRATE_3600                  13   /* 3.6 kbps */
#define     GCCAPRATE_4800                  14   /* 4.8 kbps */
#define     GCCAPRATE_5300                  15   /* 5.3 kbps */  
#define     GCCAPRATE_6300                  16   /* 6.3 kbps */  
#define     GCCAPRATE_7200                  17   /* 7.2 kbps */
#define     GCCAPRATE_8000                  18   /* 8 kbps */
#define     GCCAPRATE_9600                  19   /* 9.6 kbps */
#define     GCCAPRATE_12000                 20   /* 12 kbps */
#define     GCCAPRATE_14400                 21   /* 14.4 kbps */
#define     GCCAPRATE_16000                 22   /* 16 kbps */
#define     GCCAPRATE_19200                 23   /* 19.2 kbps */
#define     GCCAPRATE_32000                 24   /* 32 kbps */
#define     GCCAPRATE_48000                 25   /* 48 kbps */
#define     GCCAPRATE_56000                 26   /* 56 kbps */
#define     GCCAPRATE_64000                 27   /* 64 kbps */
#define     GCCAPRATE_128000                28   /* 128 kbps */
#define     GCCAPRATE_384000                29   /* 384 kbps */
#define     GCCAPRATE_1536000               30   /* 1536 kbps */
#define     GCCAPRATE_1920000               31   /* 1920 kbps */
#define     GCCAPRATE_75_1200               32
#define     GCCAPRATE_1200_75               33
#define     GCCAPRATE_DEFAULT               34




/*
 *
 *  The following set ID is for setting the caller ID information
 *
 */
#define     GCSET_CALLER_ID         0x1005  /* MakeCall block - Caller ID parameters    */

/* Parameter IDs for GCSET_CALLER_ID */
#define     GCPARM_NAME             1       /* Name to be displayed (NULL terminated) */
#define     GCPARM_ADDRESS          2       /* Address to be displayed (NULL terminated) */ 
#define     GCPARM_DATE             3       /* Date in MM/DD/YYYY format */ 
#define     GCPARM_TIME             4       /* Time in HH:MM format */ 
#define     GCPARM_NAME_ABSENCE     5       /* Reason for absence of name */ 
#define     GCPARM_NUMBER_ABSENCE   6       /* Reason for absence of number */ 
#define     GCPARM_INFO             7       /* Other User data (NULL terminated) */ 

/* Possible values for GCPARM_NAME_ABSENCE, GCPARM_NUMBER_ABSENCE */
#define     GCABSENCE_PRIVATE       1       /* Private address */
#define     GCABSENCE_OUT_OF_AREA   2       /* Out of area address */




/*
 *
 *  The following set ID is for configuring the service request parameters
 *
 */
#define     GCSET_SERVREQ           0x1006  /* GCSR parameters */

/* Parameter IDs for GCSET_SERVREQ */
#define     PARM_SERVICEID          1       /* Request ID */
#define     PARM_REQTYPE            2       /* Request Type */
#define     PARM_ACK                3       /* Request Acknowledgement */

/* Possible values for PARM_ACK parameter in GCSET_SERVREQ */
#define     GC_NACK                 0       /* No ack required or rejection */
#define     GC_ACK                  1       /* Ack required or confirmation */




/*
 * 
 * The following two set IDs are corresponding to the structure: GCLIB_ADDRESS_BLK
 *
 */
#define GCSET_ORIG_ADDR             0x1007  /* structure: GCLIB_ADDRESS_BLK */
#define GCSET_DEST_ADDR             0x1008  /* structure: GCLIB_ADDRESS_BLK */

/* Parameter IDs for GCSET_ORIG_ADDR and GCSET_DEST_ADDR */
#define GCPARM_ADDR_DATA            0x0     /* field: address */
#define GCPARM_ADDR_TYPE            0x1     /* field: address_type */
#define GCPARM_ADDR_PLAN            0x2     /* field: address_plan */
#define GCPARM_SUBADDR_DATA         0x3     /* field: sub_address */
#define GCPARM_SUBADDR_TYPE         0x4     /* field: sub_address_type */
#define GCPARM_SUBADDR_PLAN         0x5     /* field: sub_address_plan */


/* Possible values for GCPARM_ADDR_DATA parameter               */
/*      GCADDRTYPE_TRANSPARENT  - Transparent Number            */
/*      GCADDRTYPE_NAT          - National Number               */
/*      GCADDRTYPE_INTL         - International Number          */
/*      GCADDRTYPE_LOC          - Local Number                  */
/*      GCADDRTYPE_IP           - Internet Protocol Address)    */
/*      GCADDRTYPE_URL          - URL Address                   */
/*      GCADDRTYPE_DOMAIN       - Domain address)               */
/*      GCADDRTYPE_EMAIL        - Email Address                 */

/* Possible values for GCPARM_ADDR_PLAN parameter               */ 
/*      GCADDRPLAN_UNKNOWN      - Address plan - Unknown        */
/*      GCADDRPLAN_ISDN         - Address plan - ISDN plan      */
/*      GCADDRPLAN_TELEPHONY    - Address plan - Telephony plan */
/*      GCADDRPLAN_PRIVATE      - Address plan - Private plan   */

/* Possible values for GCPARM_SUBADDR_TYPE parameter            */ 
/*      GCSUBADDR_UNKNOWN       - Address type - Unknown type   */
/*      GCSUBADDR_OSI           - Address type - OSI type       */
/*      GCSUBADDR_USER          - Address type - User type      */
/*      GCSUBADDR_IA5           - Address type - IA5 characters */



/* 
 *
 * The following set ID is corresponding to the structure: GCLIB_CHAN_BLK 
 *
 */
#define GCSET_CHAN_BLK              0x1009

/* Parameter IDs for GCSET_CHAN_BLK */
#define GCPARM_CHAN_MEDIA_ID        0x0     /* field: medium_id  */
#define GCPARM_CHAN_MEDIA_SEL       0x1     /* field: medium_sel */

/* Possible values for GCPARM_CHAN_MEDIA_SEL parameter           */
/*      GCMEDSEL_MEDIUM_PREF    - Timeslot selected is preferred */
/*      GCMEDSEL_MEDIUM_EXCL    - Timeslot selected is exclusive */



/*
 *
 * The following set ID is corresponding to the structure: GCLIB_CALL_BLK
 *
 */
#define GCSET_CALL_BLK              0x100A

/* Parameter IDs for GCSET_CALL_BLK */
#define GCPARM_CALL_CATEGORY        0x0     /* field: category */
#define GCPARM_CALL_ADDR_INFO       0x1     /* field: address_info */

/* Possible values for GCPARM_CALL_CATEGORY parameter               */ 
/*      GCCAT_SUB_NOPRIOR       - Subscriber without priority       */
/*      GCCAT_SUB_PRIOR         - Subscriber with priority          */
/*      GCCAT_MAINT_EQUIP       - Maintenance equipment             */
/*      GCCAT_COIN_BOX          - Coinbox or subscriber with chargemetering */
/*      GCCAT_OPERATOR          - Operator                          */  
/*      GCCAT_DATA              - Data transmission                 */
/*      GCCAT_CPTP              - C.P.T.P.                          */  
/*      GCCAT_SPECIAL           - Special Line                      */
/*      GCCAT_MOBILE            - Mobile users                      */
/*      GCCAT_VPN               - Virtual private network line      */

/* Possible values for GCPARM_CALL_ADDR_INFO parameter              */
/*      GCADDRINFO_ENBLOC       - Address is complete               */
/*      GCADDRINFO_OVERLAP      - Address is not complete           */


/*
 *
 * The following set ID is for specifying a network interface for setting
 * interface parameters e.g. setting the IP address of a network interface board.
 *
 */
#define GCSET_NETIF_NUM             0x100B

/* Parameter IDs for GCSET_NETIF_NUM */
#define GCPARM_IP_NETIF             0x01


/*
 *
 * The following set ID is for supplementary services
 *
 */
#define GCSET_SUPP_XFER             0x100C

/* Parameter IDs for GCSET_SUPP_XFER */
#define GCPARM_PRIMARYCALL_CRN    0x01    /* CRN of primary call   */
#define GCPARM_SECONDARYCALL_CRN  0x02    /* CRN of secondary call */
#define GCPARM_TRANSFERRING_ADDR  0x100   /* NULL terminated address of transferring   */
                                          /* party of maximum length of 128 characters */
                                          /* (including NULL character) */

/*
 *  The set ID and parm ID for the new GCLib-defined parameters shall be defined here
 */


#endif


