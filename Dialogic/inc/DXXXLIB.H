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

 * Filename:    dxxxlib.h 

 * DESCRIPTION: Voice Library. Include defines, events, 

 *              structures, and function prototypes.   

 ****************************************************************************/





#ifndef __DXXXLIB_H__

#define __DXXXLIB_H__



#ifdef __STANDALONE__            /* List of required header files */

#endif





/*

 * Required header files

 */



/* NOTE: srllib.h must be included before here for DV_TPT */



#include "dxdevdef.h"

#include "dxcallp.h"

#include "dxdigit.h"

#include "dxr2mf.h"

#include "dxsync.h"

#include "dxtables.h"

#include "d40low.h"

#include "dxxxlow.h"

#include "dxuio.h"

#include "ctinfo.h"



/*

 * The following equates and structures are D/4x dependent

 */



#ifndef VOXLIB



/*

 * Macros to get at the class and model parts of a device ID

 */

#define dl_devclass(i)  ((i) & 0xFF00)

#define dl_devmodel(i)  ((i) & 0x00FF)



#define DEV_CLASS_VOICE "Voice"

#define DEV_CLASS_AUDIO_IN "ai"



/*

 * Maximum terminator values.

 */

#define MAX_TC_TIMER 6000  /* Max time units for term. conditions */





/*

 * Values for setting hook state

 */

#define DL_ONHOOK             0

#define DL_OFFHOOK            1

#define DX_ONHOOK             0

#define DX_OFFHOOK            1





/**

 ** Define Event Types

 **/

#define TDX_PLAY        0x81     /* Play Completed */

#define TDX_RECORD      0x82     /* Record Completed */

#define TDX_GETDIG      0x83     /* Get Digits Completed */

#define TDX_DIAL        0x84     /* Dial Completed */

#define TDX_CALLP       0x85     /* Call Progress Completed */

#define TDX_CST         0x86     /* CST Event Received */

#define TDX_SETHOOK     0x87     /* SetHook Completed */

#define TDX_WINK        0x88     /* Wink Completed */

#define TDX_ERROR       0x89     /* Error Event */

#define TDX_PLAYTONE    0x8A     /* Play Tone Completed */

#define TDX_GETR2MF     0x8B     /* Get R2MF completed */

#define TDX_BARGEIN     0x8C     /* Barge in completed */

#define TDX_NOSTOP      0x8D     /* No Stop needed to be Issued */

#define TDX_TXDATA      0x90	 /* Transmit Data Completed */

#define TDX_RXDATA      0x91	 /* Receive Data Completed */

#define TDX_CACHEPROMPT 0x92	 /* Data cached */

#define TDX_UNDERRUN	0x93 	 /* Generated when an underrun is detected by FW */

#define TDX_LOWWATER	0x94	 /* Generated when a low water mark is hit in buffer. */

#define TDX_HIGHWATER	0x95	 /* Generated when a high water mark is hit in buffer. */

#define TDX_QUERYTONE		0x96	/* Query Tone Completed */

#define TDX_QUERYTONE_FAIL	0x97	/* Query Tone Failed */

#define TDX_DELETETONE		0x98	/* Delete Tone Completed */

#define TDX_DELETETONE_FAIL	0x99	/* Delete Tone Failed */

#define TDX_CREATETONE		0x9A	/* Create Tone Completed */

#define TDX_CREATETONE_FAIL	0x9B	/* Create Tone Failed */

#define TDX_VAD				0x9C

#define TDX_LISTEN          0x9D /* completion event for dx_listenEx */

#define TDX_UNLISTEN        0x9E /* completion event for dx_unlistenEx */

#define TDX_LISTEN_FAIL     0x9F /* failure completion event for dx_listenEx */

#define TDX_UNLISTEN_FAIL   0xA0 /* failure completion event for dx_unlistenEx */

#define TDX_DRVNOMEM			 0xA1 /* Generated when Driver has no memory */

#define DX_ATOMIC_ERR		0xEE     /* Error event for Atomic Funcs */

/*

 *	Added to handle new sendevt() capability.

 */

#define	TDX_UNKNOWN		1000		/* Yes, Decimal 1000.  Hex value: 0x3E8 */


// 03/27/07 Use this base for all dxxx event codes going forward: 0x3000

#define DXXEV_BASE			0x3000					// Beginning base value

#define DXXEV_LAST_EVENT	(DXXEV_BASE | 0xFF)		// Last event


#define TDX_RESET           (DXXEV_BASE | 0x01)		// reset event for Rest API's

#define TDX_RESETERR	    (DXXEV_BASE | 0x02)	    // reset error event for Rest API's




/**

 ** Define sendevt flag values

 **/

#define EVFL_SENDSELF   0x01     /* Send event to self process */

#define EVFL_SENDOTHERS 0x02     /* Send event to other processes */

#define EVFL_SENDALL    0x03     /* Send event to all processes */





/*

 * Error codes returned by ATDV_LASTERR()

 */

#define EDX_NOERROR     0     /* No Errors */

#define EDX_SYSTEM      1     /* System Error */

#define EDX_FWERROR     2     /* Firmware Error */

#define EDX_TIMEOUT     3     /* Function Timed Out */

#define EDX_BADIOTT     4     /* Invalid Entry in the DX_IOTT */

#define EDX_BADTPT      5     /* Invalid Entry in the DX_TPT */

#define EDX_BADPARM     6     /* Invalid Parameter in Function Call */

#define EDX_BADDEV      7     /* Invalid Device Descriptor */

#define EDX_BADPROD     8     /* Func. Not Supported on this Board */

#define EDX_BUSY        9     /* Device is Already Busy */

#define EDX_IDLE        10    /* Device is Idle */

#define EDX_STOPRINGS   11    /* Stop waitrings (MT only) */

#define EDX_WTRINGSTOP  11    /* Wait for Rings stopped by user */

#define EDX_BADWAVEFILE 12    /* Bad/Unsupported WAV file */

#define EDX_XPBPARM     13    /* Bad XPB structure */

#define EDX_NOSUPPORT   14    /* Data format not supported */

#define EDX_NOTIMP      15    /* Function not implemented */

#define EDX_BADSUBCOMMAND              16

#define EDX_BADCHANNELNUMBER           17

#define EDX_BADRESOURCEID              18

#define EDX_NORESOURCE                 19    /* No Resources */

#define EDX_DSPERROR                   20    /* Resource DSP error */

#define EDX_INUSE                      21

#define EDX_HOOKSTATETRANSITIONERROR   25    /* dx_sethook() unable to transition hookstate */





/*

 * GTD Error Codes

 */

#define EDX_TONEID      0x41  /* Bad Tone Template ID */

#define EDX_MAXTMPLT    0x42  /* Max number of Templates Exists */

#define EDX_INVSUBCMD   0x43  /* Invalid Sub Command Number */

#define EDX_CHANNUM     0x44  /* Invalid Channel Number Specified */

#define EDX_FREQDET     0x45  /* Invalid Freq Component Values */

                              /* in Tone Template Description */

#define EDX_CADENCE     0x46  /* Invalid Cadence Component Values */

                              /* in Tone Template Description */

#define EDX_ASCII       0x47  /* Invalid Ascii Value */

                              /* in Tone Template Description */

#define EDX_DIGTYPE     0x48  /* Invalid Dig_Type Value */

                              /* in Tone Template Description */

#define EDX_MSGSTATUS   0x49  /* Invalid Message Status Setting */



/*

 * Speed and Volume Control Error Codes

 */

#define EDX_SVADJBLKS   0x4A  /* Invalid Number of Speed/Volume */

                              /* Adjustment Blocks */

#define EDX_SPDVOL      0x4B  /* Must Specify either SV_SPEEDTBL or */

                              /* SV_VOLUMETBL */

#define EDX_SVMTSIZE    0x4C  /* Invalid Table Size Specified */

#define EDX_SVMTRANGE   0x4D  /* An out of Range entry in DX_SVMT */

#define EDX_NONZEROSIZE 0x4E  /* Reset to Default was Requested but */

                              /* size was non-zero */



/*

 * GTG Error Codes

 */

#define EDX_FLAGGEN     0x4F  /* Invalid tg_dflag field in */

                              /* Tone Generation Template */

#define EDX_FREQGEN     0x50  /* Invalid Frequency Component in */

                              /* Tone Generation Template */

#define EDX_AMPLGEN     0x51  /* Invalid Amplitude Value in */

                              /* Tone Generation Template */

#define EDX_TNQUERYDELETE	0x52 /*Tone not queried/deleted prior to create*/



/*

 * Switch Handler / SCSA  Error Codes

 */

#define EDX_SH_CMDBLOCK   0x59  /* Blocking Command is in progress */

#define EDX_SH_BADCMD     0x60  /* Command not supported */

#define EDX_SH_LIBBSY     0x61  /* Switching Handler Lib is busy */

#define EDX_SH_BADINDX    0x62  /* Invalid Switching Handler indx # */

#define EDX_SH_LIBNOTINIT 0x63  /* Switching Handler Library has not been initialized */

#define EDX_SH_NOCLK      0x64  /* Switching Handler Clock fallback failed */

#define EDX_SH_MISSING    0x66  /* Switching Handler is not Present */

#define EDX_SH_BADMODE    0x6A  /* Invalid Switching Handler Bus Mode */

#define EDX_SH_BADLCLTS   0x6F  /* Invalid local timeslot (Channel) */

#define EDX_SH_BADTYPE    0x70  /* Invalid local timeslot type */

#define EDX_SH_BADEXTTS   0x71  /* External timeslot not supported at current clock rate */

#define EDX_SH_LCLTSCNCT  0x72  /* Channel is already connected to SCbus */

#define EDX_SH_LCLDSCNCT  0x73  /* Channel is already disconnected from SCbus */





/*

 * Error Codes Specific to caller ID

 */

#define EDX_CLIDINFO 0x80  /* caller ID info/sub-msg not available */

#define EDX_CLIDBLK  0x81  /* caller ID private ('P') or blocked   */

#define EDX_CLIDOOA  0x82  /* caller ID out of area ('O')          */

#define EDX_CLIDPUB  0x83  /* caller ID public phone ('C')	   */

#define EDX_CLIDPLAN 0x84  /* caller ID unknown plan ('S')	   */



/*

 * Error Codes Specific to PBXpert TSF loading

 */

#define	EDX_BADREGVALUE         0x90

#define	EDX_BADTSFFILE          0x91

#define	EDX_BADTSFDATA          0x92

#define	EDX_FEATUREDISABLED     0x93



/*

 * Error Codes Specific to EC TAP

 */

#define	EDX_BADMODE             0x90

#define	EDX_UNSUPPORTED         0x91



/*

 * GTD Learn Mode Error Codes

 */

#define EDX_TNINFO     0x101   /* Error retrieving tone information */

#define EDX_TNINVALID  0x102   /* Inavlid tone detected */

#define EDX_TNFRAMES   0x103   /* Inavlid tone frame count */

#define EDX_TNCONTIM   0x104   /* Inavlid continuous tone time */

#define EDX_TNCADFLAG  0x105   /* Inavlid cadence/continuous tone flag */

#define EDX_LMPARM     0x106   /* Inavlid learn mode parameter */

#define EDX_TNAMP      0x107   /* Inavlid tone amplitude specified */

#define EDX_LMMETHOD   0x108   /* Inavlid frequency/duration tolerance method */

#define EDX_LMFREQTOL  0x109   /* Inavlid Learn Mode frequency tolerance */

#define EDX_LMDURTOL   0x10A   /* Inavlid Learn Mode duration tolerance */

#define EDX_TNDFLAG    0x10B   /* Inavlid Single/Dual Tone Flag */

#define EDX_TNPARM     0x10C   /* Inavlid Tone Template Parameter */

#define EDX_LMQUALID   0x10D   /* Inavlid Learn Mode Qualification Template ID */



/* Board Specific error */

#define EDX_NOTENOUGHBRDMEM 0x100





/*

 * Equates for generic caller ID message type IDs 

 */

#define  CLIDINFO_CMPLT        0x100   /* complete caller ID message   */

#define  CLIDINFO_GENERAL      0x101   /* date,time,phone #,name       */

#define  CLIDINFO_CALLID       0x102   /* caller ID                    */

#define  CLIDINFO_FRAMETYPE    0x103   /* see frame type equates       */



/*

 * Equates for CLASS 

 */

#define  CLASSFRAME_SDM    0x04    /* Single Data Message caller ID frame   */

#define  CLASSFRAME_MDM    0x80    /* Multiple Data Message caller ID frame */

/* CLASS MDM message IDs */

#define  MCLASS_DATETIME   0x01    /* date and time w/o '/" & ':'      */

#define  MCLASS_DN         0x02    /* calling line directory number    */

#define  MCLASS_DDN        0x03    /* dialed number                    */

#define  MCLASS_ABSENCE1   0x04    /* caller ID absence: 'O' or 'P'    */

#define  MCLASS_REDIRECT   0x05    /* call frwd-universal,busy,or unanswered */

#define  MCLASS_QUALIFIER  0x06    /* 'L' for long distance            */

#define  MCLASS_NAME       0x07    /* caller name                      */

#define  MCLASS_ABSENCE2   0x08    /* name absence- 'O' or 'P'         */



/*

 * Equates for CLIP 

 */

#define  CLIPFRAME_MDM     0x80    /* Multiple Data Message caller ID frame */

/* CLIP message IDs */

#define  CLIP_DATETIME     0x01    /* date and time (w/o '/" & ':')    */

#define  CLIP_DN           0x02    /* calling line directory number    */

#define  CLIP_DDN          0x03    /* dialed number                    */

#define  CLIP_ABSENCE1     0x04    /* caller ID absence: 'O' or 'P'    */

#define  CLIP_NAME         0x07    /* caller name                      */

#define  CLIP_ABSENCE2     0x08    /* name absence- 'O' or 'P'         */

#define  CLIP_CALLTYPE     0x11    /* voice call,rng-bck-when-free call or msg waiting call */

#define  CLIP_NETMSG       0x13    /* no. of message waiting             */



/*

 * Equates for A-CLIP 

 */

#define  ACLIPFRAME_SDM    0x04    /* Single Data Message caller ID frame   */

#define  ACLIPFRAME_MDM    0x80    /* Multiple Data Message caller ID frame */

/* A-CLIP MDM message IDs */

#define  MACLIP_DATETIME   0x01    /* date and time (w/o '/" & ':'     */

#define  MACLIP_DN         0x02    /* calling line directory number    */

#define  MACLIP_DDN        0x03    /* dialed number                    */

#define  MACLIP_ABSENCE1   0x04    /* caller ID absence: 'O' or 'P'    */

#define  MACLIP_REDIRECT   0x05    /* call frwd-universal,busy,or unanswered */

#define  MACLIP_QUALIFIER  0x06    /* 'L' for long distance            */

#define  MACLIP_NAME       0x07    /* caller name                      */

#define  MACLIP_ABSENCE2   0x08    /* name absence- 'O' or 'P'         */



/*

 * Equates for Japan Caller ID  JCLIP

 */

#define  JCLIPFRAME_MDM    0x40    /* Multiple Data Message caller ID frame */

#define  JCLIP_DN          0x02    /* calling line directory number    */

#define  JCLIP_DDN         0x09    /* dialed number                    */

#define  JCLIP_ABSENCE1    0x04    /* caller ID absence: 'O' or 'P'    */

#define  JCLIP_ABSENCE2    0x08    /* name absence- 'O' or 'P'         */





/*

 * Defines for channel state values

 */

#define CS_IDLE          1     /* Channel is idle */

#define CS_PLAY          2     /* Channel is playing back */

#define CS_RECD          3     /* Channel is recording */

#define CS_DIAL          4     /* Channel is dialing */

#define CS_GTDIG         5     /* Channel is getting digits */

#define CS_TONE          6     /* Channel is generating a tone */

#define CS_STOPD         7     /* Operation has terminated */

#define CS_SENDFAX       8     /* Channel is sending a fax */

#define CS_RECVFAX       9     /* Channel is receiving a fax */

#define CS_CALL          13    /* Channel is Call Progress Mode */

#define CS_GETR2MF       14    /* Channel is Getting R2MF */

#define CS_BLOCKED       16    /* Channel is blocked */

#define CS_RECDPREPARE   17    /* Channel is preparing record and driver has not yet sent record 



/* 

 * This is a complex state composed of one of the 

 * above states and faxmode. 

 */

#define CS_FAXIO     10    /* Channel is between fax pages */



/*

 * Define a channel state for the remaining blocking commands

 */

#define CS_HOOK      11    /* A change in hookstate is in progress */

#define CS_WINK      12    /* A wink operation is in progress */

#define CS_RINGS     15    /* Call status Rings state */



/*	

 *	Added this to have the behavior if user uses getevt to

 *	wait for rings.

 */

#define	CS_WAITEVT	(CS_RINGS)



/*

 * D/4X Events and masks 

 */

#define DE_RINGS     1     /* Rings received */

#define DE_SILON     2     /* Silence on */

#define DE_SILOF     3     /* Silenec off */

#define DE_LCON      4     /* Loop current on */

#define DE_LCOF      5     /* Loop current off */

#define DE_WINK      6     /* Wink received */

#define DE_RNGOFF    7     /* Ring off event */

#define DE_DIGITS    8     /* Digit Received */

#define DE_DIGOFF    9     /* Digit tone off event */

#define DE_LCREV     13    /* Loop current reversal   */

#define DE_TONEON    17    /* Tone ON  Event Received */

#define DE_TONEOFF   18    /* Tone OFF Event Received */

#define DE_STOPRINGS 19    /* Stop ring detect state */

#define DE_VAD       20    /* Voice Energy detected */

#define DE_UNDERRUN  21    /* R4 Streaming to Board API FW underrun event. Improves streaming data to board */





/* Alt. defines for DM_SILOF, DM_LCOF are DM_SILOFF and DM_LCOFF */

/*

 * Event mask values

 */

#define DM_RINGS     ( 1 << (DE_RINGS - 1) )

#define DM_SILON     ( 1 << (DE_SILON - 1) )

#define DM_SILOF     ( 1 << (DE_SILOF - 1) )

#define DM_LCON      ( 1 << (DE_LCON - 1) )

#define DM_LCOF      ( 1 << (DE_LCOF - 1) )

#define DM_LCREV     ( 1 << (DE_LCREV - 1) )

#define DM_WINK      ( 1 << (DE_WINK - 1) )

#define DM_RNGOFF    ( 1 << (DE_RNGOFF - 1) )

#define DM_DIGITS    ( 1 << (DE_DIGITS - 1) )

#define DM_DIGOFF    ( 1 << (DE_DIGOFF - 1) )

#define DM_VADEVTS	 ( 1 << (DE_VAD - 1) )

#define DM_UNDERRUN	 ( 1 << (DE_UNDERRUN - 1) )





/*

 * Termination reasons

 */

#define T_NORML      0     /* Normal termination */

#define T_MXDIG      1     /* Max. digits received */

#define T_MXSIL      2     /* Max. silence exceeded */

#define T_MXNSIL     3     /* Max. non-silence exceeded */

#define T_LOOP       4     /* Loop current drop */

#define T_IDIG       5     /* Max. inter-digit delay exceeded */

#define T_MXSEC      6     /* Max. time elapsed */

#define T_TDIG       7     /* Terminating digit received */

#define T_PATRN      8     /* Pattern match detected */

#define T_STOP       9     /* Stopped by user */

#define T_EOD        10    /* End of data on playback */

#define T_ERR        13    /* I/O device error */





/*

 * Masked DTMF termination/initiation equates

 */

#define  DM_D        0x0001    /* Mask for DTMF d. */

#define  DM_1        0x0002    /* Mask for DTMF 1. */

#define  DM_2        0x0004    /* Mask for DTMF 2. */

#define  DM_3        0x0008    /* Mask for DTMF 3. */

#define  DM_4        0x0010    /* Mask for DTMF 4. */

#define  DM_5        0x0020    /* Mask for DTMF 5. */

#define  DM_6        0x0040    /* Mask for DTMF 6. */

#define  DM_7        0x0080    /* Mask for DTMF 7. */

#define  DM_8        0x0100    /* Mask for DTMF 8. */

#define  DM_9        0x0200    /* Mask for DTMF 9. */

#define  DM_0        0x0400    /* Mask for DTMF 0. */

#define  DM_S        0x0800    /* Mask for DTMF *. */

#define  DM_P        0x1000    /* Mask for DTMF #. */

#define  DM_A        0x2000    /* Mask for DTMF a. */

#define  DM_B        0x4000    /* Mask for DTMF b. */

#define  DM_C        0x8000    /* Mask for DTMF c. */



/*

 *  Standard DTMF Tone Ids

 */

#define TN_DTMF_1		0x8001

#define TN_DTMF_2		0x8002

#define TN_DTMF_3		0x8003

#define TN_DTMF_4		0x8004

#define TN_DTMF_5		0x8005

#define TN_DTMF_6		0x8006

#define TN_DTMF_7		0x8007

#define TN_DTMF_8		0x8008

#define TN_DTMF_9		0x8009

#define TN_DTMF_STAR		0x800e

#define TN_DTMF_0		0x8000

#define TN_DTMF_POUND		0x800f

#define TN_DTMF_A		0x800a

#define TN_DTMF_B		0x800b

#define TN_DTMF_C		0x800c

#define TN_DTMF_D		0x800d



/*

 * Macros to get the parts out of a parameter ID

 */

#define parmtype(id) (((id) >> 8) & 0xF)   /* High byte, lo nibble */

#define parmnumb(id) (((id) & 0xFF) - 1)   /* Low byte */





/*

 * D/4x Parameter Type Equates

 */

#define PT_BD        0      /* Board parameter */

#define PT_CH        1      /* Channel parameter */





/*

 * Defines used by the parameter functions

 */

#define PM_BD        0x00800000L   /* Board Level Parameter */

#define PM_CH        0x00000000L   /* Channel Level Parameter */



#define PM_DXXX      0x00000000L   /* D/xxx Parameter */

#define PM_D12X      0x01000000L   /* D/12x Parameter */

#define PM_MF40      0x02000000L   /* MF/40 Parameter */

#define PM_D12XDEV   0x04000000L



#define PM_FW        0x00000000L   /* Firmware Parameter */

#define PM_DRV       0x08000000L   /* Driver Parameter */

#define PM_LIB       0x10000000L   /* Library Parameter */

#define PM_NOIDLE    0x00400000L   /* don't do check for idle */



#define PRM_DEST     0x18000000L

#define PRM_DEVTYP   0x07000000L

#define PRM_DATTYP   0xE0000000L

#define PRM_NUM      0x0000FFFFL

#define PRM_TERM     0x007F0000L



#define PM_BYTE      0x00000000L   /* Byte */

#define PM_SHORT     0x20000000L   /* Short */

#define PM_INT       0x40000000L   /* Int */

#define PM_LONG      0x60000000L   /* Long */

#define PM_VLSTR     0x80000000L   /* Variable Length String */

#define PM_FLSTR     0xA0000000L   /* Fixed Length String */



#define PM_VLSTR_MAXLEN 128 /* Max Length for Variable Length Parms */



/*

 * CHANNEL PARAMETERS

 */

/* DTMF anti-talkoff during playback */

#define DXCH_DTMFTLK    ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0101L)



/* DTMF debounce time */

#define DXCH_DTMFDEB    ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0201L)



/* DTMF initiation bitmap */

#define DXCH_DTINITSET  ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0203L)



/* Delay after ring recvd before winking */

#define DXCH_WINKDLY    ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0204L)



/* Duration of wink (off-hook state) */

#define DXCH_WINKLEN    ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0205L)



/* Min LC on to be recognized as wink */

#define DXCH_MINRWINK   ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0206L)



/* Max LC on to be recognized as wink */

#define DXCH_MAXRWINK   ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0207L)



/* Length of DTMF digit for dialing */

#define DXCH_TTDATA     ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0208L)



/* Length of TT inter digit delay */

#define DXCH_T_IDD      ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0209L)



/* Length of ADSI alert tone */

#define DXCH_ADSIALERT_LENGTH ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x020DL)



/* Time to wait for DTMF A */

#define DXCH_ADSIALERT_ACK ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x020EL)



/* Length of DTMF to accept */

#define DXCH_ADSIACK_LENGTH ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x020FL)



/* Pause after end of DTMF */

#define DXCH_ADSIACK_FSK ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0210L)



/* Level to generate CAS tone */

#define DXCH_ADSIALERT_AMPL ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0211L)



/* MF detection mask */

#define DXCH_MFMODE     ((PM_SHORT|PM_FW|PM_MF40|PM_CH) | 0x0201L)



/* Diagnostic mask */

#define DXCH_MFDIAG     ((PM_SHORT|PM_FW|PM_MF40|PM_CH) | 0x0202L)



/* Per channel dig rate: play (D/xxx only) */

#define DXCH_PLAYDRATE  ((PM_SHORT|PM_FW|PM_D12X|PM_CH) | 0x0E01L)



/* Per channel dig rate: rec (D/xxx only)*/

#define DXCH_RECRDRATE  ((PM_SHORT|PM_FW|PM_D12X|PM_CH) | 0x0E02L)



/* DTMF detection edge select */

#define DXCH_DFLAGS     ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0801L)



/* Per channel ring count */

#define DXCH_RINGCNT    ((PM_SHORT|PM_DRV|PM_DXXX|PM_CH) | 0x0001L)



/* Enable/disable Caller ID */

#define DXCH_CALLID     ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0E04L)



/* Enable/disable Caller ID values */

#define DX_CALLIDDISABLE   0x0000   /* Disable Caller ID */

#define DX_CALLIDENABLE    0x0001   /* Enable Caller ID */



/* Enable/disable Audio line-in */

#define DXCH_AUDIOLINEIN ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0E05L)



/* Enable/disable Audio line-in values */

#define DX_LINEINDISABLE   0x0000   /* Disable audio line-in connection */

#define DX_LINEINENABLE    0x0001   /* Enable audio line-in connection */



/* DTMF Digit buffering mode */

#define DXCH_DIGBUFMODE ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0002L)



/* Per channel I/O buffer Parameters */

#define DXCH_XFERBUFSIZE ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0003L)		//sets both

#define DXCH_RXDATABUFSIZE ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0004L)	//record only

#define DXCH_TXDATABUFSIZE ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0005L)	//play only

#define DXCH_NUMRXBUFFERS ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0006L)	//num record

#define DXCH_NUMTXBUFFERS ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0007L)	//num play



//per channel tone trimming

#define DXCH_DONOTTRIMTONES ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0008L)



// Both Play and Bargein events on a barge in?

#define DXCH_BARGEINONLY  ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0009L)	//play + Bargein



//Gain Control Parameters

#define DXCH_MEDIAXMITGAIN ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x000AL)

#define DXCH_MEDIARECVGAIN ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x000BL)

#define DXCH_PSTNXMITGAIN  ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x000CL)

#define DXCH_PSTNRECVGAIN  ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x000DL)

#define DXCH_ECRXMITGAIN	((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x000EL)

#define DXCH_ECRRECVGAIN	((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x000FL)



//variable number of buffers for using buffers > the default 2 @ 32K size

#define DXCH_VARNUMBUFFERS	((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0010L)



// Silence Compressed Recording Parameter

#define DXCH_SCRFEATURE  ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0011L)

#define DXCH_SCRDISABLED	0

#define DXCH_SCRENABLED		1



// FSK Interblock Timeout for data Receive mode	

#define DXCH_FSKINTERBLKTIMEOUT  ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0012L)





// FSK Standard	

#define DXCH_FSKSTANDARD         ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0013L)



// FSK Channel Seizure

#define DXCH_FSKCHSEIZURE        ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0014L)



// FSK Mark Length

#define DXCH_FSKMARKLENGTH       ((PM_SHORT|PM_LIB|PM_DXXX|PM_CH) | 0x0015L)





/* Per channel EC TAP */

#define DXCH_EC_TAP_LENGTH ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x21BL)



/* ASR Barge-in channel parameters */

#define DXCH_BARGEIN_RECFORMAT    ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0212L)

#define DXCH_VADTIMEOUT           ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0213L)

#define DXCH_SPEECHSNR            ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0214L)

#define DXCH_SPEECHTHRESH         ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0215L)

#define DXCH_SPEECHPLAYTHRESH     ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0215L)

#define DXCH_SPEECHPLAYTRIGG      ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0216L)

#define DXCH_SPEECHNONPLAYTRIGG   ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0217L)

#define DXCH_SPEECHPLAYWINDOW     ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0218L)

#define DXCH_SPEECHNONPLAYWINDOW  ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0219L)

#define DXCH_SPEECHNONPLAYTHRESH  ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x021AL)



#define DXCH_DSPCARRIERDETECTDEBOUNCE   ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0232L)

#define DXCH_DSPCARRIERDETECTTHRESHOLD  ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0233L)



/* Per channel AGC parameters */

#define DXCH_AGC_TARGETOUTPUTLEVEL      ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0300L)

#define DXCH_AGC_MAXGAIN                ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0301L)

#define DXCH_AGC_SPEECHTHRESHOLD        ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0302L)

#define DXCH_AGC_NOISETHRESHOLD         ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0303L)

#define DXCH_AGC_MEMORYMAXIMUMSIZE      ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0304L)

#define DXCH_AGC_MEMORYSILENCERESET     ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0305L)

#define DXCH_AGC_GAININCSPEECH          ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0306L)

/* EC enable/disable */
#define DXCH_EC_ACTIVE					((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0307L)

/* Per channel Silence Detection parameter - in 10 msec units */
#define DXCH_MINSILON               ((PM_SHORT|PM_FW|PM_DXXX|PM_CH) | 0x0400L)

/* DTMF Digit buffering modes */

#define DX_DIGCYCLIC    1    /* Cyclic shift digits once buffer is full */

#define DX_DIGTRUNC     2    /* Truncate digits once buffer is full */



/* Channel I/O transfer buffer identifiers */

#define BUF_4K    0

#define BUF_8K    1

#define BUF_16K   2

#define BUF_2K    4

#define BUF_1K    5

#define BUF_1_5K  6

#define BUF_DFLT  BUF_8K





/* 

 * Board Parameters

 */

/* Size of the ping-pong PLAY buffers on board */

#define DXBD_TXBUFSIZE  ((PM_SHORT|PM_FW|PM_D12XDEV|PM_BD) | 0x00F6L)



/* Size of the ping-pong RECORD buffers on board */

#define DXBD_RXBUFSIZE  ((PM_SHORT|PM_FW|PM_D12XDEV|PM_BD) | 0x00F7L)



/* Flash character in dial string */

#define DXBD_FLASHCHR   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0301L)



/* Init delay time */

#define DXBD_INITDLY    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0302L)



/* Min time of LC on for LPD */

#define DXBD_MINPDON    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0303L)



/* Min time of LC off for LPD */

#define DXBD_MINPDOFF   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0304L)



/* Length of DTMF digit for dialing*/

#define DXBD_TTDATA     ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0305L)



/* Min time of silence on for APD */

#define DXBD_MINSLON    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0306L)



/* Min time for silence off for APD */

#define DXBD_MINSLOFF   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0307L)



/* Min time for dti signal on */

#define DXBD_MINTION    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0308L)



/* Min time for dti signal off*/ 

#define DXBD_MINTIOFF   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0309L)



/* Edge of ring detection */

#define DXBD_R_EDGE     ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x030AL)



/* Flash time during dialing */

#define DXBD_FLASHTM    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0401L)



/* Pause time during dialing */

#define DXBD_PAUSETM    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0402L)



/* Minimum off-hook time */

#define DXBD_MINOFFHKTM ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0403L)



/* Length of pulse dialing make */

#define DXBD_P_MK       ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0404L)



/* Length of pulse dialing break */

#define DXBD_P_BK       ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0405L)



/* Length of pulse inter digit delay*/

#define DXBD_P_IDD      ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0406L)



/* Length of TT inter digit delay */

#define DXBD_T_IDD      ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0407L)



/* Delay before offhook finished */

#define DXBD_OFFHDLY    ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0408L)



/* Min time for ring detection*/

#define DXBD_R_ON       ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0409L)



/* Min time for for the end of a ring*/

#define DXBD_R_OFF      ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x040AL)



/* Max wait between rings */

#define DXBD_R_IRD      ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x040BL)



/* Silence on and off debounce time */

#define DXBD_S_BNC      ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x040CL)



/* Min interpulse digit time for LPD */

#define DXBD_MINIPD     ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x040DL)



/* Min time of lc off before message*/

#define DXBD_MINLCOFF   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x040EL)



/* Max time for lc off for LPD */

#define DXBD_MAXPDOFF   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x040FL)



/* Min time for silence on for APD */

#define DXBD_MINISL     ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0410L)



/* Max time for silence off for  APD */

#define DXBD_MAXSLOFF   ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0411L)



/* Either a D/40 or D/41 */

#define DXBD_HWTYPE     ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0601L)



/* Number of channels on board */

#define DXBD_CHNUM      ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0602L)



/* System configuration */

#define DXBD_SYSCFG     ((PM_SHORT|PM_FW|PM_DXXX|PM_BD) | 0x0604L)



/*

 * NOTE: Must use PM_CH instead of PM_BD on MF40 paramters.

 */

/* Min time for MF detection */

#define DXBD_MFMINON    ((PM_SHORT|PM_FW|PM_MF40|PM_CH) | 0x0101L)



/* Duration of MF tone during dial */

#define DXBD_MFTONE     ((PM_SHORT|PM_FW|PM_MF40|PM_CH) | 0x0102L)



/* Inter-digit delay during MF dialing */

#define DXBD_MFDELAY    ((PM_SHORT|PM_FW|PM_MF40|PM_CH) | 0x0103L)



/* Duration of LKP tone during MF dialing */

#define DXBD_MFLKPTONE  ((PM_SHORT|PM_FW|PM_MF40|PM_CH) | 0x0104L)



/*

 * Board Parameter values

 */



/*

 * When a rings received message is sent (BD_R_EDGE)

 */

#define ET_RON       1   /* At the beginning of the ring */

#define ET_ROFF      2   /* At the end of the ring */





/*

 * Channel Mode values

 */

#define MD_ADPCM     0x0000    /* ADPCM data (the default) */

#define MD_PCM       0x0100    /* Mu-Law PCM data */

#define MD_FFT       0x0200    /* FFT data (debugging) */

#define MD_GAIN      0x0000    /* AGC on */

#define MD_NOGAIN    0x1000    /* AGC off */

#define PM_TONE      0x0001    /* Tone initiated play/record */

#define RM_TONE      PM_TONE

#define PM_SR6       0x2000    /* 6KHz sampling rate (digitization) */

#define PM_SR8       0x4000    /* 8KHz sampling rate (digitization) */

#define RM_SR6       PM_SR6

#define RM_SR8       PM_SR8

#define PM_ALAW      0x0020    /* Play A-Law data         */

#define RM_ALAW      PM_ALAW   /* Record data using A-Law */

#define PM_DTINIT    0x0002    /* Play with DTMF init */

#define RM_DTINIT    PM_DTINIT /* Record with DTMF init */

#define PM_DTINITSET 0x0010 | PM_DTINIT /* Play with DTMF init set */

#define RM_DTINITSET PM_DTINITSET   /* Record with DTMF init set */

#define R2_COMPELDIG 0x0400    /* R2MF Compelled signalling */

#define RM_USERTONE  0x0040

#define RM_NOTIFY    0x0004    /* record notification beep tone must be generated */

#define RM_VADNOTIFY 0x0008

#define RM_ISCR		 0x0080



/*

 * dx_stopch modes

 */

#define EV_NOSTOP    0x200     /* Have dx_stopch queue an event if channel is idle */



/*

 * Channel modes for ADSI support

 */

#define PM_RAW       0x0200    /* Raw playback mode */

#define PM_ADSI      0x0080    /* ADSI playback mode */

#define PM_ADSIALERT PM_ADSI | PM_DTINITSET | PM_TONE





#define PM_NOREADWRITE  0x0400 /* Don't perform read/writes */

#define PM_BARGEIN      0x0800 /* Enable bargein during playback */



/*

 *

 */

#define PM_BARGEIN_VADOFF 	0x1000

#define RM_BARGEIN 		0x0800



/*

 * dx_dial modes

 */

#define DX_CNGTONE	2      /* Generate CNG tone after dialing */



#endif





/* 

 * Alternate defines for DE_SILOF, DM_SILOF, DE_LCOF, DM_LCOF

 */

#define DE_SILOFF    DE_SILOF

#define DE_LCOFF     DE_LCOF

#define DM_SILOFF    DM_SILOF

#define DM_LCOFF     DM_LCOF





/*

 * Define's for Mode Checking

 */

#define MD_SELECT    (MD_PCM)     



#define EV_ASYNC     0x8000

#define EV_SYNC      0x0000





/*

 * Raw Line Status Defines

 */

#define RLS_SILENCE  0x80  /* Sil Bit in Raw Line Status */

#define RLS_DTMF     0x40  /* DTMF Signal Bit in Raw Line Status */

#define RLS_LCSENSE  0x20  /* Loop Current Sense Bit in Raw Line Status */

#define RLS_RING     0x10  /* Ring Detect Bit in Raw Line Status */

#define RLS_HOOK     0x08  /* Hook Switch Status Bit in Raw Line Status */

#define RLS_RINGBK   0x04  /* Audible Ringback Detect Bit in Raw Line Status */





/*

 * GTD Defines

 */

#define DM_TONEON    0x01  /* Tone ON Mask */

#define DM_TONEOFF   0x02  /* Tone OFF Mask */



#define TONEALL      0xFFFFFFFF   /* Enable/Disable All Tone ID's */



#define TN_SINGLE    0     /* Single Tone */

#define TN_DUAL      1     /* Dual Tone */



/*

 * Template Modes and Frequency for GTD

 */

#define TN_FREQDEV   5     /* Frequency Deviation */



#define TN_CADENCE   0x01  /* Cadence Detection */

#define TN_LEADING   0x02  /* Leading Edge Detection */

#define TN_TRAILING  0x04  /* Trailing Edge Detection */



//defines for Call Progress Analysis function

#define CA_SIT		0x01		// look for a previously defined SIT tone

#define CA_PAMD	0x02		// use a previously defined PAMD template

#define CA_PVD		0x04		// use a previously defined PVD template.





/*

 * Digit Type Values for User-Defined Tone Templates for GTD

 */

#define DG_USER1     5     /* User Defined Tone */

#define DG_USER2     6     /* User Defined Tone */

#define DG_USER3     7     /* User Defined Tone */

#define DG_USER4     8     /* User Defined Tone */

#define DG_USER5     9     /* User Defined Tone */

#define MAKE_DIGTYPE(id,digit) ((((id)<<8))|digit)





/**

 ** Speed and Volume Control Defines

 **/

#define SV_SPEEDTBL     0x01     /* Modify Speed */

#define SV_VOLUMETBL    0x02     /* Modify Volume */



#define SV_ABSPOS       0x00     /* Absolute Position */

#define SV_RELCURPOS    0x10     /* Relative to Current Position */

#define SV_TOGGLE       0x20     /* Toggle */



#define SV_PAUSE        0x40     /* Pause Play */

#define SV_RESUME       0x80     /* Resume Play */  





#define SV_WRAPMOD      0x0010

#define SV_SETDEFAULT   0x0020

#define SV_LEVEL        0x0100

#define SV_BEGINPLAY    0x0200



/*

 * Toggle Modes for Play Adjustment Conditions

 */

#define SV_TOGORIGIN    0x00  /* Toggle Between Origin and Last */

                              /* Modified Position */

#define SV_CURORIGIN    0x01  /* Reset Current Position to Origin */

#define SV_CURLASTMOD   0x02  /* Reset Current Position to Last */

                              /* Modified Position */

#define SV_RESETORIG    0x03  /* Reset Current Position and Last */

                              /* Modified State to Origin */



/*

 * Defines for dx_addspddig()

 */

#define SV_ADD10PCT   1    /* Speed up Play-Back 10 Percent */

#define SV_ADD20PCT   2    /* Speed up Play-Back 20 Percent */

#define SV_ADD30PCT   3    /* Speed up Play-Back 30 Percent */

#define SV_ADD40PCT   4    /* Speed up Play-Back 40 Percent */

#define SV_ADD50PCT   5    /* Speed up Play-Back 50 Percent */

#define SV_SUB10PCT  -1    /* Slow Down Play-Back 10 Percent */

#define SV_SUB20PCT  -2    /* Slow Down Play-Back 20 Percent */

#define SV_SUB30PCT  -3    /* Slow Down Play-Back 30 Percent */

#define SV_SUB40PCT  -4    /* Slow Down Play-Back 40 Percent */



#define SV_NORMAL    0xFF  /* Set Play-Back to Normal Speed/Volume */



/*

 * Defines for dx_addvoldig()

 */

#define SV_ADD2DB     1    /* Increase Play-Back Volume by 2db */

#define SV_ADD4DB     2    /* Increase Play-Back Volume by 4db */

#define SV_ADD6DB     3    /* Increase Play-Back Volume by 6db */

#define SV_ADD8DB     4    /* Increase Play-Back Volume by 8db */

#define SV_SUB2DB    -1    /* Decrease Play-Back Volume by 2db */

#define SV_SUB4DB    -2    /* Decrease Play-Back Volume by 4db */

#define SV_SUB6DB    -3    /* Decrease Play-Back Volume by 6db */

#define SV_SUB8DB    -4    /* Decrease Play-Back Volume by 8db */



/*

 * Defines for dx_PutStreamData() R4 Streaming to Board API

 */

#define STREAM_CONT     0

#define STREAM_EOD      1



/*

 * Defines for dx_SetWaterMark() parm id's for R4 Streaming to Board API

 */

#define LOW_MARK        1

#define HIGH_MARK       2



/*

 * Defines for CurrentState for R4 Streaming to Board API

 */

#define UNASSIGNED_STREAM_BUFFER  1

#define ASSIGNED_STREAM_BUFFER    2







#define IGNORESTATE   1



/*

 * Wave file support defines

 */

/*

 * File formats

 */

#define FILE_FORMAT_VOX    1     /* Dialogic VOX format */

#define FILE_FORMAT_WAVE   2     /* Microsoft Wave format */

#define FILE_FORMAT_NONE   3     /* No file being used */



/*

 * Sampling rate

 */

#define DRT_6KHZ           0x30  /* 6KHz */

#define DRT_8KHZ           0x40  /* 8KHz */

#define DRT_11KHZ          0x58  /* 11KHz */



/*

 * Data format

 */

#define DATA_FORMAT_DIALOGIC_ADPCM		        0x1		/* OKI ADPCM */

#define DATA_FORMAT_ALAW				        0x3		/* alaw PCM  */

#define DATA_FORMAT_G726				        0x4		/* G.726     */

#define DATA_FORMAT_MULAW				        0x7		/* mulaw PCM */

#define DATA_FORMAT_PCM					        0x8		/* PCM       */

#define DATA_FORMAT_G729A				        0x0C	/* CELP coder */

#define DATA_FORMAT_GSM610				        0x0D	/* Microsoft GSM (backward compatible*/

#define DATA_FORMAT_GSM610_MICROSOFT	        0x0D	/* Microsoft GSM */

#define DATA_FORMAT_GSM610_ETSI			        0x0E	/* ETSI standard framing */

#define DATA_FORMAT_GSM610_TIPHON		        0x0F	/* ETSI TIPHON bit order */

#define DATA_FORMAT_LC_CELP			            0x10		/* Lucent CELP Coder */

#define DATA_FORMAT_TRUESPEECH			        0x10		/* TRUESPEECH Coder */

#define DATA_FORMAT_G711_ALAW	                DATA_FORMAT_ALAW

#define DATA_FORMAT_G711_ALAW_8BIT_REV	        0x11

#define DATA_FORMAT_G711_ALAW_16BIT_REV	        0x12

#define DATA_FORMAT_G711_MULAW	                DATA_FORMAT_MULAW

#define DATA_FORMAT_G711_MULAW_8BIT_REV	        0x13

#define DATA_FORMAT_G711_MULAW_16BIT_REV	    0x14

#define DATA_FORMAT_G721	                    0x15

#define DATA_FORMAT_G721_8BIT_REV	            0x16

#define DATA_FORMAT_G721_16BIT_REV	            0x17

#define DATA_FORMAT_G721_16BIT_REV_NIBBLE_SWAP  0x18

#define DATA_FORMAT_IMA_ADPCM                   0x19 

#define DATA_FORMAT_RAW				0x1A





#define DATA_FORMAT_FFT             0xFF  /* fft data  */



/*

 * dx_gtsernum() subfunctions

 */

#define GS_SN  0x0000   /* ret. board serial #, 8 bytes ASCIIZ string */

#define GS_SSN 0x0001   /* ret. board silicon serial number, 8 bytes */





/*

 * Channel I/O status defines

 */

#ifndef IN_SERVICE

#define IN_SERVICE  0      /* Put the channel back into service */

#endif

#ifndef OUT_SERVICE

#define OUT_SERVICE 1      /* Take the channel out of service */

#endif





/*

 * DX_EBLK

 *

 * Event block structure

 * [NOTE: All user-accessible structures must be defined so as to be

 *        unaffected by structure packing.]

 */

typedef struct dx_eblk {

   unsigned short  ev_event;     /* Event that occured */

   unsigned short  ev_data;      /* Event-specific data */

   unsigned char   ev_rfu[12];   /* RFU for packing-independence */

} DX_EBLK;





/*

 * DX_CST

 *

 * Call Status Transition Structure

 * [NOTE: All user-accessible structures must be defined so as to be

 *        unaffected by structure packing.]

 */

typedef struct dx_cst {

   unsigned short cst_event;     /* CST Event */

   unsigned short cst_data;      /* Data Associated with the Event */

} DX_CST;





/*

 * Speed and Volume Control typedefs

 */



/*

 * Speed/Volume Condition Block

 */

typedef struct dx_svcb {

   unsigned short type;       /* Bit Mask */

   short          adjsize;    /* Adjustment Size */

   unsigned char  digit;      /* ASCII digit value that causes the action */

   unsigned char  digtype;    /* Digit Type (e.g. 0 = DTMF) */

} DX_SVCB;





/*

 * Speed/Volume Modification Table

 */

typedef struct dx_svmt {

  char decrease[ 10 ];        /* Ten Downward Steps */

  char origin;                /* Regular Speed and Volume */

  char increase[ 10 ];        /* Ten Upward Steps */

} DX_SVMT;





/*

 * Standard Call Progress Signal Definitions

 */

#define CP_DIAL                 dx_get_tngencad ( 1 )   

#define CP_REORDER              dx_get_tngencad ( 2 )   

#define CP_BUSY                 dx_get_tngencad ( 3 )   

#define CP_RINGBACK1            dx_get_tngencad ( 4 )   

#define CP_RINGBACK2            dx_get_tngencad ( 5 )   

#define CP_RINGBACK1_CALLWAIT   dx_get_tngencad ( 6 )   

#define CP_RINGBACK2_CALLWAIT   dx_get_tngencad ( 7 )

#define CP_RECALL_DIAL          dx_get_tngencad ( 8 )

#define CP_INTERCEPT            dx_get_tngencad ( 9 )

#define CP_CALLWAIT1            dx_get_tngencad ( 10 )

#define CP_CALLWAIT2            dx_get_tngencad ( 11 )

#define CP_BUSY_VERIFY_A        dx_get_tngencad ( 12 )

#define CP_BUSY_VERIFY_B        dx_get_tngencad ( 13 )

#define CP_EXEC_OVERRIDE        dx_get_tngencad ( 14 )

#define CP_FEATURE_CONFIRM      dx_get_tngencad ( 15 )

#define CP_MSG_WAIT_DIAL        dx_get_tngencad ( 16 ) // Same as CP_STUTTER_DIAL

#define CP_STUTTER_DIAL         dx_get_tngencad ( 16 ) // Same as CP_MSG_WAIT_DIAL

#define MAX_CP_TONES            16





/*

 * Tone Generation Template

 */

typedef struct {

   unsigned short tg_dflag;   /* Dual Tone - 1, Single Tone - 0 */

   unsigned short tg_freq1;   /* Frequency for Tone 1 (HZ) */

   unsigned short tg_freq2;   /* Frequency for Tone 2 (HZ) */

   short          tg_ampl1;   /* Amplitude for Tone 1 (db) */

   short          tg_ampl2;   /* Amplitude for Tone 2 (db) */

   short          tg_dur;     /* Duration of the Generated Tone */

                              /* Units = 10ms */

} TN_GEN;



/*

 * Cadenced Tone Generation Template

 */

typedef struct {

   unsigned char cycles;      /* Number of cycles */

   unsigned char numsegs;     /* Number of tones */

   short         offtime[4];  /* Array of durations, one for each tone */

   TN_GEN        tone[4];     /* Array of tone templates */

} TN_GENCAD;



/*

 * Tone Template Structure

 */

typedef struct {

   unsigned char  tn_dflag;      /* Dual Tone -1, Single Tone - 0 */

   unsigned char  tn_ampflg;	 /* If set use tone amplitude values  */

   unsigned short tn1_min;       /* Min. Frequency for Tone 1 (in Hz) */

   unsigned short tn1_max;       /* Max. Frequency for Tone 1 (in Hz) */

   unsigned short tn2_min;       /* Min. Frequency for Tone 2 (in Hz) */

   unsigned short tn2_max;       /* Max. Frequency for Tone 2 (in Hz) */

   unsigned short tn_quality;    /* Qualification Template Number */

   unsigned short tn_pquality;   /* Qualification Template Number for */

                                 /*  Playback */

   unsigned short tn_id;         /* Tone Template ID Number */

   unsigned char  tn_ascii;      /* ASCII Digit Value */

   unsigned char  tn_digtype;    /* Digit Type */

   unsigned short tn_rep_cnt;    /* Debounce Rep Count */

   unsigned short tnon_min;      /* Debounce Min. ON Time */

                                 /*  (in 10msec units) */

   unsigned short tnon_max;      /* Debounce Max. ON Time */

                                 /*  (in 10msec units) */

   unsigned short tnoff_min;     /* Debounce Min. OFF Time */

                                 /*  (in 10msec units) */

   unsigned short tnoff_max;     /* Debounce Max. OFF Time */

                                 /*  (in 10msec units) */

   unsigned short tn_minampl1;   /* Minimum amplitude of frequency 1 */

   unsigned short tn_maxampl1;   /* Maximum amplitude of frequency 1 */

   unsigned short tn_minampl2;   /* Minimum amplitude of frequency 2 */

   unsigned short tn_maxampl2;   /* Maximum amplitude of frequency 2 */

} TN_TMPLT;



#pragma pack(8)

typedef struct {

   unsigned int structver;			/* version of ToneSeg struct */

   unsigned short tn_dflag;      /* Dual Tone - 1, Single Tone - 0 */

   unsigned short tn1_min;       /* Min. Frequency for Tone 1 (in Hz) */

   unsigned short tn1_max;       /* Max. Frequency for Tone 1 (in Hz) */

   unsigned short tn2_min;       /* Min. Frequency for Tone 2 (in Hz) */

   unsigned short tn2_max;       /* Max. Frequency for Tone 2 (in Hz) */

   unsigned short tn_twinmin;    /* Min. Frequency for twin of dual tone (in Hz) */

   unsigned short tn_twinmax;    /*	Max. Frequency for twin of dual tone (in Hz) */

   unsigned short tnon_min; 		/* Debounce Min. ON Time (in 10msec units) */

   unsigned short tnon_max;      /* Debounce Max. ON Time (in 10msec units) */

   unsigned short tnoff_min;    	/* Debounce Min. OFF Time (in 10msec units) */

   unsigned short tnoff_max;     /* Debounce Max. OFF Time (in 10msec units) */

} TONE_SEG;



typedef struct {

   unsigned int structver;			/* version of TONE_DATA struct */

   unsigned int numofseg;			/*	Number of segments for a MultiSegment Tone */

   unsigned short tn_rep_cnt;    /* Debounce Rep Count */

   TONE_SEG toneseg[4];	

} TONE_DATA;

/*

 * Tone Information Structure

 */

typedef struct {

   unsigned short tn_freq1;   /* Actual Freq of Tone 1 detected (Hz) */

   unsigned short tn_freq2;   /* Actual Freq of Tone 2 detected (Hz) */

   unsigned short tn_on;      /* Actual On Time (in 10msec units) */

   unsigned short tn_off;     /* Actual Off Time (in 10msec units) */

   unsigned short tn_rep_cnt; /* Actual Rep Count */

   unsigned short rfu;        /* Reserved */

} TN_INFO;

/*
 * Timestamp Information Structure
 */
typedef struct {
   unsigned long  tn_TimeStamp;   /* Time stamp for tone on/off event. The timestamp
                                     will be in milliseconds from when the firmware 
                                     was downloaded on the board. */
} TN_TIMESTAMP;

/*
 * Energy Information Structure
 */
typedef struct {
	short ergy_broad;	/* Value of total energy of frame */

	short ergy_freq1;	/* Value of main frequency peak energy, 
								   if tone being tracked in frame */

	short ergy_freq2;	/* Value of 2nd frequency peak energy, 
						           if tone being tracked in frame */

	unsigned short ergy_rfu;	/* Reserved for future use*/
} FM_ENERGY;

/* IPY00093223: Default packing is applied to the DX_XPB structure within
 * the dxxxlib.h. Because of this an application that has structure member
 * alignment as 1, returns the size of the DX_XPB structure as 10 bytes
 * instead of 12 bytes. 
 *
 * Workaround: Apply packing of 8 bytes to the DX_XPB structure so that if
 * an application uses any structure member alignment, the size would be 
 * returned as 12 bytes always.
 */

/*

 * Transfer parameter block

 */

 typedef struct {

   unsigned short wFileFormat;    /* file format */

   unsigned short wDataFormat;    /* data encoding */

   unsigned long  nSamplesPerSec; /* sampling rate */

   unsigned short wBitsPerSample; /* bits per sample */

} DX_XPB;

#pragma pack()


#pragma pack(1)



/*

 * FEATURE_TABLE structure

 */

typedef struct feature_table {

   unsigned short ft_play;

   unsigned short ft_record;

   unsigned short ft_tone;

   unsigned short ft_e2p_brd_cfg;

   unsigned short ft_fax;

   unsigned short ft_front_end;

   unsigned short ft_misc;

   unsigned short ft_send;

   unsigned short ft_receive;

   unsigned int   ft_play_ext;

   unsigned int   ft_record_ext;

   unsigned short ft_device;

   unsigned short ft_rfu;

} FEATURE_TABLE;



#pragma pack ()



/*

 * Defines for features supported on a device.

 * Use with the feature table obtained from dx_getfeaturelist.

 */

/* Play and Record features */

#define FT_ADPCM           0x0001

#define FT_PCM             0x0002

#define FT_ALAW            0x0004

#define FT_ULAW            0x0008

#define FT_LINEAR          0x0010

#define FT_ITU_G_726       0x0020

#define FT_ADSI            0x0040

#define FT_RAW64KBIT       0x0080

#define FT_DRT6KHZ         0x0100

#define FT_DRT8KHZ         0x0200

#define FT_DRT11KHZ        0x0400

#define FT_FFT             0x0800

#define FT_FSK_OH          0x1000

#define FT_G729A           0x2000

#define FT_MSGSM           0x4000

#define FT_RESRVD1         0x8000

#define FT_RESRVD2         0x10000



/* Extended Play and Record features - ft_play_ext and ft_record_ext */

#define FT_TRUSPEECH		0x00000001



/* GTD and GTD features */

#define FT_GTDENABLED      0x0001

#define FT_GTGENABLED      0x0002

#define FT_CADENCE_TONE    0x0004



/* E2P features */

#define FT_CALLP           0x0002

#define FT_MF_TONE_DETECT  0x0004

#define FT_DPD             0x0020

#define FT_SYNTELLECT      0x0040

#define FT_ECR             0x0080

#define FT_CSP             0x0100

#define FT_CONFERENCE      0x0200



/* FAX features */

#define FT_FAX             0x0001

#define FT_VFX40           0x0002

#define FT_VFX40E          0x0004

#define FT_VFX40E_PLUS     0x0008

#define FT_FAX_EXT_TBL	   0x10

#define FT_RS_SHARE        0x20

#define FT_FAX_T38UDP	   0x0040 /* Communication on T38 with UDP packets */



/* FrontEnd features */

#define FT_ANALOG          0x0001

#define FT_EARTH_RECALL    0x0002

#define  FT_ROUTEABLE      0x0004

#define  FT_ISDN           0x0008

#define  FT_CAS            0x0010

#define  FT_R2MF           0x0020

#define  FT_ANALOG_CID     0x0040



/* Feature Options */

#define FT_CALLERID        0x0001

#define FT_PROMPTEDREC     0x0002     /* Prompted Record (triggered by VAD) */

#define FT_GAIN_AND_LAW    0x0004     /* TDM ASIC is Gain/Law conversion capable */

#define FT_VAD             0x0008     /* Voice Activity Detection */

#define FT_RECFLOWCONTROL  0x0010     /* Flow control on recording channels */

#define FT_CSPEXTRATSLOT   0x0020     /* Reserve extra tx tslot for CSP */



/* Fax Send Options */

#define FT_TX14400              0x0001

#define FT_TXASCII              0x0002

#define FT_SENDFAX_TXFILE_ASCII FT_TXASCII

#define FT_TXFILEMR             0x0004

#define FT_TXFILEMMR            0x0008

#define FT_TXLINEMR             0x0010

#define FT_TXLINEMMR		0x0020

#define FT_TXECM                0x0040

#define FT_TXCCTFAX             0x0080



/* Fax Receive Options */

#define FT_RX14400              0x0001

#define FT_RX12000              0x0002

#define FT_RXASCII              0x0004

#define FT_RXFILEMR             0x0008

#define FT_RXFILEMMR            0x0010

#define FT_RXLINEMR             0x0020

#define FT_RXLINEMMR            0x0040

#define FT_RXECM                0x0080



/* Fax Resource Sharing */

#define RSC_FAX            0x34

#define RSC_ASSIGNED       1

#define RSC_NOTASSIGNED    0



typedef struct ct_lawinfo {

	unsigned char	ct_xmitencoding;

	unsigned char	ct_recvencoding;

} CT_LAWINFO;





typedef struct streamStat

{

    unsigned int	version;            // Version of the structure

    unsigned int	bytesIn;            // Total number of bytes put into stream buffer

    unsigned int	bytesOut;           // Total number of bytes sent to board

    unsigned int	headPointer;        // Internal pointer to position in buffer

    unsigned int	tailPointer;        // Internal pointer to position in buffer

    unsigned int	currentState;       // Idle, streaming etc.

    unsigned int	numberOfBufferUnderruns;  // Number of times circular buffer contains less than transfer buffer size

    unsigned int	numberOfBufferOverruns;   // Number of times circular buffer is overfilled

    unsigned int	BufferSize;         // total size of circular buffer

    unsigned int	spaceAvailable;     // Space (in bytes) available in the buffer

    unsigned int	highWaterMark;      // High point in buffer used to signal an event

    unsigned int	lowWaterMark;       // Low point in buffer used to signal an event

} DX_STREAMSTAT;







/*

 * DATA TRANSFER API

 */

#define DT_ADSI		0x0001

#define DT_RAWDATA	0x0002



/*

 * ADSI data transfer modes

 */

#define ADSI_ALERT		0x000A

#define ADSI_NOALERT		0x0005

#define ADSI_ONHOOK_SEIZURE	0x0009

#define ADSI_ONHOOK_NOSEIZURE	0x000B



/*

 * ADSI data transfer structure

 */

typedef struct _ADSI_XFERSTRUC {

   UINT		cbSize;

   DWORD	dwTxDataMode;

   DWORD	dwRxDataMode;

} ADSI_XFERSTRUC;

 

/*

 * RAW data transfer modes

 */

#define RAW_64KBIT		0x0006



/*

 * RAW data transfer structure

 */

typedef struct _RAWDATA_XFERSTRUC {

   UINT		cbSize;

   DWORD	dwTxDataMode;

   DWORD	dwRxDataMode;

} RAWDATA_XFERSTRUC;







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



/*

 * Type definition for ECR Characteristic Table structure.

 */

#ifndef __DX_ECRCT__

#define __DX_ECRCT__

typedef struct dx_ecrct {

   int  ct_length;		/* Sizeof this structure */

   unsigned char ct_NLPflag;	/* NLP required during echo-cancellation? */

} DX_ECRCT;



#define SIZE_OF_ECR_CT sizeof(DX_ECRCT)

#define ECR_CT_ENABLE  0

#define ECR_CT_DISABLE 1

#endif



/*

 * Definition for Read/Write Callback Routine

 */

typedef void (*DXRWROUTINE)(DWORD, PCHAR, DWORD);





#ifdef __cplusplus

extern "C" {               // C Plus Plus function bindings

#define extern

#endif



/*

 *	Useful declaration specification.

 */

/*

 *	Module Definition file compatibility.

 */

#ifdef __USING_DEF_FILE__



#ifdef DllExport

#undef DllExport

#endif



#ifdef DllImport

#undef DllImport

#endif



#ifdef DllLinkage

#undef DllLinkage

#endif



#define DllExport

#define DllImport

#define DllLinkage extern



#else	/* __USING_DEF_FILE__	*/



#ifndef DllExport

#define DllExport	__declspec( dllexport )

#endif	/*	Dllexport	*/



#ifndef DllImport

#define DllImport __declspec( dllimport )

#endif	/*	Dllimport	*/



#ifdef _VOX_DLL

#define	DllLinkage	DllExport

#else

#define DllLinkage	DllImport

#endif



#endif	/* __USING_DEF_FILE__	*/



#ifdef __CROSS_COMPAT_LIB__

#undef DllLinkage

#define DllLinkage    

#endif



#if (defined (__BORLANDC__) || defined (__cplusplus) || defined( __STDC__ ))



/* Function Prototypes - ANSI C & C++ */



DllLinkage int __cdecl ag_getctinfo( int chDev, CT_DEVINFO *ct_devinfop);

DllLinkage int __cdecl ag_getxmitslot( int chDev, SC_TSINFO *sc_tsinfop);

DllLinkage int __cdecl ag_listen( int chDev, SC_TSINFO *sc_tsinfop );

DllLinkage int __cdecl ag_unlisten( int chDev );

DllLinkage int __cdecl dx_addspddig( int chdev, char digit, short adjval );

DllLinkage int __cdecl dx_addtone( int chdev, unsigned char digit, unsigned char digtype );

DllLinkage int __cdecl dx_addvoldig( int chdev, char digit, short adjval );

DllLinkage int __cdecl dx_adjsv( int chdev, unsigned short tabletype, 

                        unsigned short action, unsigned short adjsize );

DllLinkage int __cdecl dx_blddt( unsigned int id,   unsigned freq1, unsigned int fq1dev, 

                                        unsigned int freq2, unsigned int fq2dev,

                                        unsigned int mode );

DllLinkage int __cdecl dx_blddtcad( unsigned int ToneID, 

                        unsigned int freq1,   unsigned int fq1dev, 

                        unsigned int freq2,   unsigned int fq2dev, 

                        unsigned int OnTime,  unsigned int OnTimeDev, 

                        unsigned int OffTime, unsigned int OffTimeDev, 

                        unsigned int repcnt );

DllLinkage int __cdecl dx_bldst( unsigned int ToneID, unsigned int freq, unsigned int fqdev,

                     unsigned int mode );

DllLinkage int __cdecl dx_bldstcad( unsigned int ToneID, 

                        unsigned int freq, unsigned int fqdev,

                        unsigned int OnTime,  unsigned int OnTimeDev, 

                        unsigned int OffTime, unsigned int OffTimeDev, 

                        unsigned int repcnt );

DllLinkage void __cdecl dx_bldtngen( TN_GEN *tngenp, 

                        unsigned short freq1, unsigned short freq2,

                        short amplitude1,    short amplitude2, short duration );

DllLinkage int __cdecl dx_chgfreq( int tonetype, int fq1, int dv1, int fq2, int dv2 );

DllLinkage int __cdecl dx_chgdur( int typetype, int on, int ondv, int off, int offdv );

DllLinkage int __cdecl dx_chgrepcnt( int tonetype, int repcount );

DllLinkage int __cdecl dx_chgqualid( int qualid );

DllLinkage int __cdecl dx_close( int dev );

DllLinkage void __cdecl dx_clrcap( DX_CAP *capp );

DllLinkage int __cdecl dx_clrdigbuf( int chdev );

DllLinkage int __cdecl dx_clrsvcond( int chdev );

DllLinkage int __cdecl dx_clrtpt( DV_TPT *tptp, int size );

DllLinkage int __cdecl dx_comtest( int chdev );

DllLinkage int __cdecl dx_deltones( int chdev );

DllLinkage int __cdecl dx_dial( int chdev, const char *dialstrp, const DX_CAP *capp, unsigned short mode );

DllLinkage int __cdecl dx_distone( int chdev, int toneID, int evtMask );

DllLinkage int __cdecl dx_enbtone( int chdev, int ToneID, int evtMask );

DllLinkage int __cdecl dx_getcursv( int chdev, int *curvolp, int *curspeedp );

DllLinkage int __cdecl dx_getctinfo( int chDev, CT_DEVINFO *ct_devinfop);

DllLinkage int __cdecl dx_getdig( int chdev, const DV_TPT *tptp, DV_DIGIT *digitp, 

                        unsigned short mode );

DllLinkage int __cdecl dx_getevt( int chdev, DX_EBLK *eblkp, int timeout );

DllLinkage int __cdecl dx_getfeaturelist( int chDev, FEATURE_TABLE *feature_tablep);

DllLinkage int __cdecl dx_getparm( int dev, unsigned long parm, void * valuep );

DllLinkage int __cdecl dx_getsvmt( int ChDev, unsigned short tableType, DX_SVMT *svmtp );

DllLinkage TN_GENCAD * __cdecl dx_get_tngencad( unsigned int toneid );

DllLinkage int __cdecl dx_getuio(DX_UIO *appuiop);

DllLinkage int __cdecl dx_getxmitslot( int chDev, SC_TSINFO *sc_tsinfop);

DllLinkage int __cdecl dx_initcallp( int chdev );

DllLinkage int __cdecl dx_listen( int chDev, SC_TSINFO *sc_tsinfop );

DllLinkage int __cdecl dx_listenEx( int chDev, SC_TSINFO *sc_tsinfop, USHORT Mode );

DllLinkage int __cdecl dx_open( const char *namep, int oflags );

DllLinkage int __cdecl dx_pause( int chDev);

DllLinkage int __cdecl dx_play( int ChDev, DX_IOTT *iottp, const DV_TPT *tptp, 

                        unsigned short mode );

DllLinkage int __cdecl dx_playf(int ChDev, const char *fNamep, const DV_TPT *tptp, unsigned short mode);

DllLinkage int __cdecl dx_playtone( int ChDev, const TN_GEN *tngenp, const DV_TPT *tptp, int mode );

DllLinkage int __cdecl dx_playtoneEx( int ddd, const TN_GENCAD *tngencadp, const DV_TPT *tptp, int mode );

DllLinkage int __cdecl dx_readsram(int ChDev, PUCHAR pBuffer, int BufferSize, int Offset, PCHAR pFileName );

DllLinkage int __cdecl dx_rec(int ChDev, DX_IOTT *iottp, const DV_TPT *tptp, unsigned short mode);

DllLinkage int __cdecl dx_recf( int ChDev, const char *fNamep, const DV_TPT *tptp, unsigned short mode);

DllLinkage int __cdecl dx_resume( int chDev);

DllLinkage int __cdecl dx_route( int BdDev, int Chan, int TimeSlot );

DllLinkage int __cdecl dx_routerxtx( int ddd, int channel, int rxslot, int txslot );

DllLinkage int __cdecl dx_sendevt( int devd, unsigned long evttype, void *evtdatap, short evtlen, unsigned short flags);

DllLinkage int __cdecl dx_setchxfercnt(int chDev, int bufnum);

DllLinkage int __cdecl dx_setdevuio(int chDev, DX_UIO *devuiop, DX_UIO **retuiop);

DllLinkage int __cdecl dx_setdigtyp( int ChDev, unsigned short DigitMask );

DllLinkage int __cdecl dx_setdigbuf( int ChDev, int Mode );

DllLinkage int __cdecl dx_setevtmsk( int ChDev, unsigned int mask );

DllLinkage void __cdecl dx_setgtdamp( short Min1, short Max1, short Min2, short Max2 );

DllLinkage int __cdecl dx_sethook( int ChDev, int HookState, unsigned short mode );

DllLinkage int __cdecl dx_setiostatus(INT SrlDevice, INT Status);

DllLinkage int __cdecl dx_setparm( int Dev, unsigned long parm, const void * valuep );

DllLinkage int __cdecl dx_setrings( int devd, unsigned int nrings );



DllLinkage int __cdecl dx_GetRscStatus (int Dev, unsigned long resourceid, int *);

DllLinkage int __cdecl dx_assignresource (int Dev, unsigned long resourceid);

DllLinkage int __cdecl dx_unassignresource (int Dev, unsigned long resourceid);

DllLinkage int __cdecl dx_getdsprs (int Dev, unsigned long resourceid, char* ApplicationBuffer, long *lpTotalResources, long *NumDSP);



DllLinkage int __cdecl dx_setsvcond( int ChDev, unsigned short numBlks, const DX_SVCB *svcbp );

DllLinkage int __cdecl dx_setsvmt(int ChDev, unsigned short TableType, const DX_SVMT *svmtp,

                      unsigned short flag  );

DllLinkage int __cdecl dx_settonelen( int tonelength );

DllLinkage int __cdecl dx_setuio( DX_UIO );

DllLinkage int __cdecl dx_stopch( int ChDev, unsigned short mode );

DllLinkage int __cdecl dx_resetch( int ChDev, unsigned short mode );



DllLinkage int __cdecl dx_unlisten( int chDev );

DllLinkage int __cdecl dx_unlistenEx( int chDev, USHORT Mode );

DllLinkage int __cdecl dx_wink( int ChDev, unsigned short mode );

DllLinkage int __cdecl dx_wtring( int ChDev, int numRings, int HookState, int timeout );

#if defined (__BORLANDC__) || defined (DLGC_CLIB)

DllLinkage int __cdecl dx_fileopen ( const char * filep, int flags, int pmode);

DllLinkage int __cdecl dx_filesopen ( const char * filep, int oflag, int shflag, int pmode);

#else

DllLinkage int __cdecl dx_fileopen ( const char * filep, int flags, ...);

DllLinkage int __cdecl dx_filesopen ( const char * filep, int oflag, int shflag, ...);

#endif

DllLinkage int __cdecl dx_fileclose ( int handle );

DllLinkage long __cdecl dx_fileseek ( int handle, long offset, int origin );

DllLinkage int __cdecl dx_fileread ( int handle, void * buffer, unsigned int count );

DllLinkage int __cdecl dx_filewrite ( int handle, void * buffer, unsigned int count );

DllLinkage int __cdecl dx_filelocking ( int handle, int mode, long bytes );

DllLinkage int __cdecl dx_filechsize ( int handle, long size );

DllLinkage int __cdecl dx_filecommit ( int handle );

DllLinkage int __cdecl dx_filedup ( int handle );

DllLinkage int __cdecl dx_filedup2 ( int handle1, int handle2 );

DllLinkage int __cdecl dx_fileumask ( int pmode );

DllLinkage long __cdecl dx_filetell ( int handle );

DllLinkage int __cdecl dx_filestat ( const char * path, struct _stat * buffer );

DllLinkage int __cdecl dx_filefstat ( int handle, struct _stat * buffer );

DllLinkage long __cdecl dx_filefilelength( int handle );

DllLinkage int __cdecl dx_fileisatty( int handle );

DllLinkage int __cdecl dx_filesetmode( int handle, int mode );

DllLinkage int __cdecl dx_fileopen_osfhandle( long osfhandle, int flags );

DllLinkage int __cdecl dx_fileerrno( void );

DllLinkage int __cdecl dx_playiottdata( int ChDev,DX_IOTT * iottp,const DV_TPT * tptp, 

                                     const DX_XPB * xpbp,unsigned short mode);

DllLinkage int __cdecl dx_playwav( int ChDev, const char * fNamep, const DV_TPT *tptp, 

                                                         unsigned short mode);

DllLinkage int __cdecl dx_playvox( int ChDev, const char * fNamep, const DV_TPT *tptp, 

                                     const DX_XPB * xpbp,unsigned short mode);

DllLinkage int __cdecl dx_reciottdata( int ChDev,DX_IOTT * iottp, const DV_TPT *tptp, 

                                     DX_XPB * xpbp,unsigned short mode);

DllLinkage int __cdecl dx_recwav( int ChDev, const char * fNamep, const DV_TPT *tptp, 

                                     DX_XPB * xpbp,unsigned short mode);

DllLinkage int __cdecl dx_recvox( int ChDev, const char * fNamep, const DV_TPT *tptp, 

                                     DX_XPB * xpbp,unsigned short mode);

DllLinkage int __cdecl dx_mreciottdata( int ChDev,DX_IOTT * iottp, const DV_TPT *tptp, 

                               DX_XPB * xpbp,unsigned short mode, SC_TSINFO *lpTimeSlot);

DllLinkage int __cdecl dx_getr2mf( int Chdev, const char *src1_ptr, unsigned short src1_len,

                        const char *src2_ptr, unsigned short src2_len,

              char *dest_ptr, unsigned short dest_len, unsigned short mode);



DllLinkage int __cdecl dx_set_lasterr(int Chdev, long error);

DllLinkage int __cdecl dx_gtcallid( int Chdev, unsigned char *bufp);

DllLinkage int __cdecl dx_gtextcallid( int Chdev, int infotype, unsigned char *bufp);

DllLinkage int __cdecl dx_gtromsig ( int Chdev, char *bufp );

DllLinkage int __cdecl dx_wtcallid( int Chdev, int nrings, int timeout,unsigned char *bufp);

DllLinkage int __cdecl dx_SendReadRequest( INT SrlDevice, PCHAR Buffer,

                                   DWORD DataLength, DXRWROUTINE ReadCompletionRoutine );

DllLinkage int __cdecl dx_SendWriteRequest( INT SrlDevice, PCHAR Buffer, DWORD DataLength,

                                    DWORD BulkFlags, DXRWROUTINE WriteCompletionRoutine   );

DllLinkage int __cdecl dx_InitializeBargeIn( int Chdev, const DV_TPT * tptp,

				    DX_XPB *xpbp, USHORT RecordMode);

DllLinkage int __cdecl dx_gtsernum( int dev, int subfcn, void *buffp);

DllLinkage int __cdecl dx_TSFStatus( void );

DllLinkage int __cdecl dx_listenecr( int chDev, SC_TSINFO *sc_tsinfop );

DllLinkage int __cdecl dx_listenecrex( int chDev, SC_TSINFO *sc_tsinfop, void *dx_ecrctp );

DllLinkage int __cdecl dx_unlistenecr( int chDev );

DllLinkage int __cdecl dx_getxmitslotecr( int chDev, SC_TSINFO *sc_tsinfop);

DllLinkage int __cdecl r2_creatfsig(int chDev, int toneid);

DllLinkage int __cdecl r2_playbsig(int chDev, int backwardsig, int forwardsig, int mode);

DllLinkage int __cdecl dx_BargeIn( int chDev, DX_IOTT *lpPlayIott,

			DX_IOTT *lpRecordIott,

			const DV_TPT *lpPlayTerminations,

			const DV_TPT *lpRecordTerminations,

			DX_XPB *lpPlayParams, 

			USHORT Mode);

DllLinkage int __cdecl dx_TxIottData( int chDev, DX_IOTT *iottp, const DV_TPT *lpTerminations, int wType, 

LPVOID lpParams, USHORT mode);

DllLinkage int __cdecl dx_RxIottData( int chDev, DX_IOTT *iottp, const DV_TPT *lpTerminations, int wType, 

LPVOID lpParams, USHORT mode);

DllLinkage int __cdecl dx_TxRxIottData( int chDev, DX_IOTT *lpTxIott, const DV_TPT *lpTxTerminations, DX_IOTT 

*lpRxIott, const DV_TPT *lpRxTerminations, int wType, LPVOID lpParams, USHORT mode);

DllLinkage int __cdecl dx_cspstopch( int Chdev, unsigned short mode);



DllLinkage int __cdecl ai_open(const char* namep);

DllLinkage int __cdecl ai_close(int devh);

DllLinkage int __cdecl ai_getxmitslot(int devh, SC_TSINFO* sc_tsinfop);

DllLinkage void __cdecl dx_ResetPBXpertInitFlag( void );





DllLinkage int __cdecl dx_cacheprompt( int SrlDevice, DX_IOTT * iottp, int * prmptHdlp, unsigned short mode);

DllLinkage int __cdecl dx_getcachesize( int SrlDevice, int * cachesizep, unsigned short flags);

DllLinkage int __cdecl dx_uncacheprompt( int SrlDevice, int prmptHdl, unsigned short flags);

DllLinkage int __cdecl dx_OpenStreamBuffer( int BuffSize );

DllLinkage int __cdecl dx_CloseStreamBuffer( int hBuffer );

DllLinkage int __cdecl dx_ResetStreamBuffer( int hBuffer );

DllLinkage int __cdecl dx_PutStreamData(int hBuffer, char* pNewData, int Buffsize, int flag);

DllLinkage int __cdecl dx_GetStreamInfo( int hBuffer, DX_STREAMSTAT *pStreamStat );

DllLinkage int __cdecl dx_SetWaterMark( int hBuffer, int parm_id, int value );



DllLinkage int __cdecl dx_querytone( int SrlDevice, int toneid, TONE_DATA *lpToneData, unsigned short mode);

DllLinkage int __cdecl dx_deletetone( int SrlDevice, int toneid, unsigned short mode);

DllLinkage int __cdecl dx_createtone( int SrlDevice, int toneid, TONE_DATA *lpToneData, unsigned short mode);

DllLinkage long __cdecl dx_QueryDevice(const char* namep);



/*

 * Extended Attributes

 */

DllLinkage char * __cdecl ATDX_BDNAMEP(  INT SrlDevice );

DllLinkage char ** __cdecl ATDX_CHNAMES( INT SrlDevice );

DllLinkage long __cdecl ATDX_BDTYPE(     INT SrlDevice );

DllLinkage long __cdecl ATDX_BUFDIGS(    INT SrlDevice );

DllLinkage long __cdecl ATDX_CHNUM(      INT SrlDevice );

DllLinkage long __cdecl ATDX_DEVTYPE(    INT SrlDevice );

DllLinkage long __cdecl ATDX_DBMASK(     INT SrlDevice );

DllLinkage long __cdecl ATDX_EVTCNT(     INT SrlDevice );

DllLinkage long __cdecl ATDX_FWVER(      INT SrlDevice );

DllLinkage long __cdecl ATDX_HOOKST(     INT SrlDevice );

DllLinkage long __cdecl ATDX_LINEST(     INT SrlDevice );

DllLinkage long __cdecl ATDX_NUMCHAN(    INT SrlDevice );

DllLinkage long __cdecl ATDX_PHYADDR(    INT SrlDevice );

DllLinkage long __cdecl ATDX_PRODUCT(    INT SrlDevice );

DllLinkage long __cdecl ATDX_RINGCNT(    INT SrlDevice );

DllLinkage long __cdecl ATDX_DIGBUFMODE( INT SrlDevice );

DllLinkage long __cdecl ATDX_STATE(      INT SrlDevice );

DllLinkage long __cdecl ATDX_TERMMSK(    INT SrlDevice );

DllLinkage long __cdecl ATDX_TONEID(     INT SrlDevice );

DllLinkage long __cdecl ATDX_TRCOUNT(    INT SrlDevice );

DllLinkage long __cdecl ATDX_TRTIME(    INT SrlDevice );

DllLinkage long __cdecl ATDX_XFERBUFSIZE(INT SrlDevice );

DllLinkage long __cdecl ATDX_RXDATABUFSIZE(INT SrlDevice );



/*

 * Call Analysis Extended Attributes

 */

DllLinkage long __cdecl ATDX_ANSRSIZ(  INT SrlDevice );

DllLinkage long __cdecl ATDX_CONNTYPE( INT SrlDevice );

DllLinkage long __cdecl ATDX_CPERROR(  INT SrlDevice );

DllLinkage long __cdecl ATDX_CPTERM(   INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQDUR(   INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQDUR2(  INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQDUR3(  INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQHZ(    INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQHZ2(   INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQHZ3(   INT SrlDevice );

DllLinkage long __cdecl ATDX_FRQOUT(   INT SrlDevice );

DllLinkage long __cdecl ATDX_LONGLOW(  INT SrlDevice );

DllLinkage long __cdecl ATDX_SHORTLOW( INT SrlDevice );

DllLinkage long __cdecl ATDX_SIZEHI(   INT SrlDevice );

DllLinkage long __cdecl ATDX_DTNFAIL( INT SrlDevice );

DllLinkage long __cdecl ATDX_CRTNID( INT SrlDevice );

DllLinkage int  __cdecl dx_callp (int chdev, UINT SignalWaitTime, UINT AnswerWaitTime, UINT PVDWaitTime, 

											 UINT PVD_Id, UINT PAMD_Id, USHORT Mode);

DllLinkage int  __cdecl dx_rearm (int chdev);



/*

 * 'C' language interface functions

 */

DllLinkage long __cdecl dx_libinit( unsigned short flags );

DllLinkage long __cdecl dx_GetDllVersion( LPDWORD fileverp, LPDWORD prodverp );



DllLinkage int __cdecl dx_SetRecordNotifyBeepTone(int chdev, const TN_GENCAD *tngencadp);

DllLinkage int __cdecl dx_FileTranscode(int chdevRec, const DX_IOTT *iottpRec, const DX_XPB *xpbpRec, 

										int chdevPlay, const DX_IOTT *iottpPlay, const DX_XPB *xpbpPlay, unsigned short mode);





#else



/* Function Prototypes - Old K&R C */



DllLinkage int __cdecl ag_getctinfo();

DllLinkage int __cdecl ag_getxmitslot();

DllLinkage int __cdecl ag_listen();

DllLinkage int __cdecl ag_unlisten();

DllLinkage int __cdecl dx_addspddig();

DllLinkage int __cdecl dx_addtone();

DllLinkage int __cdecl dx_addvoldig();

DllLinkage int __cdecl dx_adjsv();

DllLinkage int __cdecl dx_blddt();

DllLinkage int __cdecl dx_blddtcad();

DllLinkage int __cdecl dx_bldst();

DllLinkage int __cdecl dx_bldstcad();

DllLinkage void __cdecl dx_bldtngen();

DllLinkage int __cdecl dx_chgfreq();

DllLinkage int __cdecl dx_chgdur();

DllLinkage int __cdecl dx_chgrepcnt();

DllLinkage int __cdecl dx_chgqualid();

DllLinkage int __cdecl dx_close();

DllLinkage void __cdecl dx_clrcap();

DllLinkage int __cdecl dx_clrdigbuf();

DllLinkage int __cdecl dx_clrsvcond();

DllLinkage int __cdecl dx_clrtpt();

DllLinkage int __cdecl dx_comtest();

DllLinkage int __cdecl dx_deltones();

DllLinkage int __cdecl dx_dial();

DllLinkage int __cdecl dx_distone();

DllLinkage int __cdecl dx_enbtone();

DllLinkage int __cdecl dx_getcursv();

DllLinkage int __cdecl dx_getctinfo();

DllLinkage int __cdecl dx_getdig();

DllLinkage int __cdecl dx_getevt();

DllLinkage int __cdecl dx_getfeaturelist();

DllLinkage int __cdecl dx_getparm();

DllLinkage int __cdecl dx_getsvmt();

DllLinkage TN_GENCAD * __cdecl dx_get_tngencad();

DllLinkage int __cdecl dx_getuio();

DllLinkage int __cdecl dx_getxmitslot();

DllLinkage int __cdecl dx_initcallp();

DllLinkage int __cdecl dx_listen();

DllLinkage int __cdecl dx_listenEx();

DllLinkage int __cdecl dx_open();

DllLinkage int __cdecl dx_pause();

DllLinkage int __cdecl dx_play();

DllLinkage int __cdecl dx_playf();

DllLinkage int __cdecl dx_playtone();

DllLinkage int __cdecl dx_playtoneEx();

DllLinkage int __cdecl dx_readsram();

DllLinkage int __cdecl dx_rec();

DllLinkage int __cdecl dx_recf();

DllLinkage int __cdecl dx_resume();

DllLinkage int __cdecl dx_route();

DllLinkage int __cdecl dx_routerxtx();

DllLinkage int __cdecl dx_sendevt();

DllLinkage int __cdecl dx_setchxfercnt();

DllLinkage int __cdecl dx_setdevuio();

DllLinkage int __cdecl dx_setdigtyp();

DllLinkage int __cdecl dx_setdigbuf();

DllLinkage int __cdecl dx_setevtmsk();

DllLinkage void __cdecl dx_setgtdamp();

DllLinkage int __cdecl dx_sethook();

DllLinkage int __cdecl dx_setiostatus();

DllLinkage int __cdecl PBXpertInit();

DllLinkage void __cdecl dx_ResetPBXpertInitFlag();

DllLinkage int __cdecl dx_setparm();

DllLinkage int __cdecl dx_GetRscStatus( );

DllLinkage int __cdecl dx_assignresource( );

DllLinkage int __cdecl dx_unassignresource( );

DllLinkage int __cdecl dx_getdsprs( );

DllLinkage int __cdecl dx_setrings();

DllLinkage int __cdecl dx_setsvcond();

DllLinkage int __cdecl dx_setsvmt();

DllLinkage int __cdecl dx_settonelen();

DllLinkage int __cdecl dx_setuio();

DllLinkage int __cdecl dx_stopch();



DllLinkage int __cdecl dx_resetch();

DllLinkage int __cdecl dx_unlisten();

DllLinkage int __cdecl dx_unlistenEx();

DllLinkage int __cdecl dx_wink();

DllLinkage int __cdecl dx_wtring();

DllLinkage int __cdecl dx_fileopen();

DllLinkage int __cdecl dx_fileclose();

DllLinkage long __cdecl dx_fileseek();

DllLinkage int __cdecl dx_fileread();

DllLinkage int __cdecl dx_filewrite();

DllLinkage int __cdecl dx_filesopen ();

DllLinkage int __cdecl dx_filelocking();

DllLinkage int __cdecl dx_filechsize();

DllLinkage int __cdecl dx_filecommit();

DllLinkage int __cdecl dx_filedup();

DllLinkage int __cdecl dx_filedup2();

DllLinkage int __cdecl dx_fileumask();

DllLinkage long __cdecl dx_filetell();

DllLinkage int __cdecl dx_filestat();

DllLinkage int __cdecl dx_filefstat();

DllLinkage long __cdecl dx_filefilelength();

DllLinkage int __cdecl dx_fileisatty();

DllLinkage int __cdecl dx_filesetmode();

DllLinkage int __cdecl dx_fileopen_osfhandle();

DllLinkage int __cdecl dx_fileerrno();

DllLinkage int __cdecl dx_playiottdata();

DllLinkage int __cdecl dx_playwav();

DllLinkage int __cdecl dx_playvox();

DllLinkage int __cdecl dx_reciottdata();

DllLinkage int __cdecl dx_recwav();

DllLinkage int __cdecl dx_recvox();

DllLinkage int __cdecl dx_mreciottdata();

DllLinkage int __cdecl dx_getr2mf();

DllLinkage int __cdecl dx_set_lasterr();

DllLinkage int __cdecl dx_gtcallid();

DllLinkage int __cdecl dx_gtextcallid();

DllLinkage int __cdecl dx_gtromsig();

DllLinkage int __cdecl dx_wtcallid();

DllLinkage int __cdecl dx_SendReadRequest();

DllLinkage int __cdecl dx_SendWriteRequest();

DllLinkage int __cdecl dx_InitializeBargeIn();

DllLinkage int __cdecl dx_gtsernum();

DllLinkage int __cdecl dx_TSFStatus();

DllLinkage int __cdecl dx_getxmitslotecr();

DllLinkage int __cdecl dx_listenecr();

DllLinkage int __cdecl dx_listenecrex();

DllLinkage int __cdecl dx_unlistenecr();

DllLinkage int __cdecl r2_creatfsig();

DllLinkage int __cdecl r2_playbsig();

DllLinkage int __cdecl dx_BargeIn();

DllLinkage int __cdecl dx_TxIottData();

DllLinkage int __cdecl dx_RxIottData();

DllLinkage int __cdecl dx_TxRxIottData();

DllLinkage int __cdecl dx_cspstopch();

DllLinkage int __cdecl ai_open();

DllLinkage int __cdecl ai_close();

DllLinkage int __cdecl ai_getxmitslot();

DllLinkage int __cdecl ag_getlaw();

DllLinkage int __cdecl dx_getlaw();

DllLinkage int __cdecl dx_cacheprompt();

DllLinkage int __cdecl dx_getcachesize();

DllLinkage int __cdecl dx_uncacheprompt();

DllLinkage int __cdecl dx_OpenStreamBuffer();

DllLinkage int __cdecl dx_CloseStreamBuffer();

DllLinkage int __cdecl dx_ResetStreamBuffer();

DllLinkage int __cdecl dx_PutStreamData();

DllLinkage int __cdecl dx_GetStreamInfo();

DllLinkage int __cdecl dx_SetWaterMark();

DllLinkage int __cdecl dx_querytone();

DllLinkage int __cdecl dx_deletetone();

DllLinkage int __cdecl dx_createtone();





/*

 * Extended Attributes

 */

DllLinkage char * __cdecl ATDX_BDNAMEP();

DllLinkage char ** __cdecl ATDX_CHNAMES();

DllLinkage long __cdecl ATDX_BDTYPE();

DllLinkage long __cdecl ATDX_BUFDIGS();

DllLinkage long __cdecl ATDX_CHNUM();

DllLinkage long __cdecl ATDX_DBMASK();

DllLinkage long __cdecl ATDX_DEVTYPE();

DllLinkage long __cdecl ATDX_EVTCNT();

DllLinkage long __cdecl ATDX_FWVER();

DllLinkage long __cdecl ATDX_HOOKST();

DllLinkage long __cdecl ATDX_LINEST();

DllLinkage long __cdecl ATDX_NUMCHAN();

DllLinkage long __cdecl ATDX_PHYADDR();

DllLinkage long __cdecl ATDX_PRODUCT();

DllLinkage long __cdecl ATDX_RINGCNT();

DllLinkage long __cdecl ATDX_DIGBUFMODE();

DllLinkage long __cdecl ATDX_STATE();

DllLinkage long __cdecl ATDX_TERMMSK();

DllLinkage long __cdecl ATDX_TONEID();

DllLinkage long __cdecl ATDX_TRCOUNT();

DllLinkage long __cdecl ATDX_TRTIME();

DllLinkage long __cdecl ATDX_XFERBUFSIZE();

DllLinkage long __cdecl ATDX_RXDATABUFSIZE();



/*

 * Call Analysis Extended Attributes

 */

DllLinkage long __cdecl ATDX_ANSRSIZ();

DllLinkage long __cdecl ATDX_CONNTYPE();

DllLinkage long __cdecl ATDX_CPERROR();

DllLinkage long __cdecl ATDX_CPTERM();

DllLinkage long __cdecl ATDX_FRQDUR();

DllLinkage long __cdecl ATDX_FRQDUR2();

DllLinkage long __cdecl ATDX_FRQDUR3();

DllLinkage long __cdecl ATDX_FRQHZ();

DllLinkage long __cdecl ATDX_FRQHZ2();

DllLinkage long __cdecl ATDX_FRQHZ3();

DllLinkage long __cdecl ATDX_FRQOUT();

DllLinkage long __cdecl ATDX_LONGLOW();

DllLinkage long __cdecl ATDX_SHORTLOW();

DllLinkage long __cdecl ATDX_SIZEHI();

DllLinkage long __cdecl ATDX_DTNFAIL();

DllLinkage long __cdecl ATDX_CRTNID();

DllLinkage int  __cdecl dx_callp ();

DllLinkage int  __cdecl dx_rearm ();

DllLinkage int __cdecl dx_SetRecordNotifyBeepTone();

DllLinkage int __cdecl dx_FileTranscode();





/*

 * 'C' language interface functions

 */

DllLinkage long __cdecl dx_libinit();

DllLinkage long __cdecl dx_GetDllVersion();



#endif



#ifdef __cplusplus

}                          // C Plus Plus function bindings

#undef extern

#endif



#endif



