/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1991-2007 Dialogic Corporation. All Rights Reserved.
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
 *        FILE: dxxxlow.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 **********************************************************************/

#ifndef __DXXXLOW_H__
#define __DXXXLOW_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif

/*
 * Define module name for diagnositic library and diag.h
 */
#define MODULE_NAME	"VOICE"

/*
 * Defines
 */

#define TN_DEFQUAL   6  /* Default Tone Qualification Template ID */
#define TN_DEFPQUAL  6  /* Default Tone Playback Qualification Template ID */


/*
 * Device Defines for Commands and Replies
 */
#define DL_DEVCMD             0x01  /* D/xxx Device Command Number */
#define DL_DEVERROR           0x07  /* D/xxx Error Reply - Generic */


/*
 * COMMANDS
 */

/*
 * GTD Commands - PC to D/4xD
 */
#define TN_CREATE             0x10  /* Create Tone Template */
#define TN_DELETE             0x11  /* Delete Tone Templates */
#define TN_SETTONEMSG         0x12  /* Set Tone Message Status */
#define TN_UPDQUAL            0x13  /* Update Qualification Template */
#define TN_READQUAL           0x14  /* Read Qualification Template */
#define TN_LASTTERM           0x15  /* Report Last Tone Terminatoin */

/*
 * GTG Commands
 */
#define TN_GENTONE            0x16  /* General Tone Generation */
#define TN_GENTONECAD         0x1b  /* Cadenced Tone Generation */

/*
 * Speed and Volume Commands - PC to D/4xD and D/12x and D/8x
 */
#define SV_ADJPLAY            0x17  /* Adjust Play */
#define SV_SETPLAYADJ         0x18  /* Set Play Adjustment Conditions */
#define SV_GETPLMODTBL        0x19  /* Read Play Modification Table */
#define SV_UPDPLMODTBL        0x1A  /* Update Play Modification Table */


/*
 * REPLIES
 */

/*
 * GTD Replies - D/4xD to PC
 */
#define TN_CREATECMPLT        TN_CREATE      /* Create Tone Templ Cmplt */
#define TN_DELETECMPLT        TN_DELETE      /* Delete Tone Templ Cmplt */
#define TN_SETMSGCMPLT        TN_SETTONEMSG  /* Set Tone Msg Status Cmplt */
#define TN_UPDQUALCMPLT       TN_UPDQUAL     /* Update Qual Templ Cmplt  */
#define TN_GETQUALCMPLT       TN_READQUAL    /* Read Qual Template Cmplt */
#define TN_TONEON             0x15           /* Tone On Event Msg */
#define TN_TONEOFF            0x16           /* Tone Off Event Msg */
#define TN_LASTERMCMPLT       0x17           /* Report Last Tone Term Cmplt */

/*
 * GTG Replies
 */
#define TN_GENTONECMPLT       0x18  /* General Tone Generation Cmplt */
#define TN_GENTONECADCMPLT    0x1d  /* Cadenced Tone Generation Cmplt */

/* FR 3269: Add silence ON and silence OFF events */

#define TN_SILON			  0x1E
#define TN_SILOFF			  0x1F

/*
 * Speed and Volume Control Replies - D/4xD and D/12x and D/8x to PC
 */
#define SV_ADJPLAYCMPLT       0x19  /* Adjust Play Complete */
#define SV_SETPLADJCMLT       0x1A  /* Set Play Adjustment Conns Cmplt */
#define SV_GETPLTBLCMLT       0x1B  /* Read Play Mod Table Complete */
#define SV_UPDPLTBLCMLT       0x1C  /* Update Play Mod Table Complete */


/*
 * GTD and Speed and Volume Control commands can only be
 * executed on either D/4xD or a SpringBoard
 *
 * Bit 4 if set => D/4xD
 * Bit 7 if set => Springboard
 */
#define   SPRINGBOARD   ( ( 0x1 << 4 ) | ( 0x1 << 7 ) )   


/*
 * GTD typedefs
 *
 * These structures are here as they are not supported at the moment
 */
/* Moved TN_TMPLT and TN_INFO to dxxxlib.h */

#if 0
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
#endif

typedef struct {
   unsigned int	nwreply_tnid;	/* The Network reply tone ID, 0 if not used */
   unsigned int	t1;	/* Max time to wait for end of thats in progress tone */
   unsigned int	t2;	/* Max time to wait between R2MF digits */
   unsigned int	t3;	/* Max time to wait for the first R2MF digit */
   unsigned char mask;	/* A bit mask */
} R2MF_INIT;

/*
 * Defined for enhanced R2MF functionality
 */
#define    PC_GETR2MF   0x1B   /* Initiate get R2MF functionality */
#define    DL_GETR2MF   0x1B   /* get R2MF functionality complete */

#endif


