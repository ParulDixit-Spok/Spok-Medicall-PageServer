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
 * Filename:    dxtables.h 
 * DESCRIPTION: Voice Library header file. Include defines, events, 
 *              structures.   
 ****************************************************************************/


#ifndef __DXTABLES_H__
#define __DXTABLES_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif

/* 
 * Termination mask defines for use with ATDX_TERMMSK( )
 */
#define TM_NORMTERM           0x00000     /* Normal Termination */
#define TM_MAXDTMF            0x00001     /* Max Number of Digits Recd */
#define TM_MAXSIL             0x00002     /* Max Silence */
#define TM_MAXNOSIL           0x00004     /* Max Non-Silence */
#define TM_LCOFF              0x00008     /* Loop Current Off */
#define TM_IDDTIME            0x00010     /* Inter Digit Delay */
#define TM_MAXTIME            0x00020     /* Max Function Time Exceeded */
#define TM_DIGIT              0x00040     /* Digit Mask or Digit Type Term. */
#define TM_PATTERN            0x00080     /* Pattern Match Silence Off */
#define TM_USRSTOP            0x00100     /* Function Stopped by User */
#define TM_EOD                0x00200     /* End of Data Reached on Playback */
#define TM_TONE               0x02000     /* Tone On/Off Termination */
#define TM_BARGEIN            0x08000     /* Play terminated due to Barge-in */
#define TM_ERROR              0x80000     /* I/O Device Error */
#define TM_MAXDATA            0x100000	  /* Max Data reached for FSK */


#define IO_DEV                0x00  /* play/record from a file */
#define IO_MEM                0x08  /* play/record from memory */
#define IO_UIO                0x10  /* play/record using user I/O functions */
#define IO_STREAM             0x20  /* End of the Termination for R4 Streaming API */
#define IO_CACHED             0x40  /* play from cache */
#define IO_USEOFFSET          0x80  /* use io_offset and io_length for non-VOX */
#define IO_UNIT_TIME          0x200 /* io_offset and io_length in milliseconds */

/*
 * Defines for the TPT
 */
#define DX_MAXDTMF            1     /* Maximum Number of Digits Received */
#define DX_MAXSIL             2     /* Maximum Silence */
#define DX_MAXNOSIL           3     /* Maximum Non-Silence */
#define DX_LCOFF              4     /* Loop Current Off */
#define DX_IDDTIME            5     /* Inter-Digit Delay */
#define DX_MAXTIME            6     /* Function Time */
#define DX_DIGMASK            7     /* Digit Mask Termination */
#define DX_PMOFF              8     /* Pattern Match Silence On */
#define DX_PMON               9     /* Pattern Match Silence Off */
#define DX_DIGTYPE            11    /* Digit Type Termination */
#define DX_TONE               12    /* Tone On/Off Termination */
#define DX_MAXDATA            13    /* Maximum bytes for ADSI data*/

//FSK Defines
#define DX_FSKSTDBELLCORE	  0x01
#define DX_FSKSTDETSI		  0x02
#define DX_FSKSTDETSIAUTO	  0x03

/*
 * Defines for TPT Termination Flags
 */
#define TF_EDGE               0x00
#define TF_LEVEL              0x01
#define TF_CLREND             0x02
#define TF_CLRBEG             0x04
#define TF_USE                0x08
#define TF_SETINIT            0x10
#define TF_10MS               0x20
#define TF_FIRST              TF_CLREND
#define TF_IMMEDIATE          0x40

#define TF_MAXDTMF            (TF_LEVEL|TF_USE)
#define TF_MAXSIL             (TF_EDGE|TF_USE)
#define TF_MAXNOSIL           (TF_EDGE|TF_USE)
#define TF_LCOFF              (TF_LEVEL|TF_USE|TF_CLREND)
#define TF_IDDTIME            (TF_EDGE)
#define TF_MAXTIME            (TF_EDGE)
#define TF_DIGMASK            (TF_LEVEL)
#define TF_PMON               (TF_EDGE)
#define TF_DIGTYPE            (TF_LEVEL)
#define TF_TONE               (TF_LEVEL|TF_USE|TF_CLREND)
#define TF_MAXDATA             0


/*
 * Defines for the DV_TPT data for DX_TONE terminator
 */
#define DX_TONEON             0x0001
#define DX_TONEOFF            0x0000


/* Defines for cache prompt */
#define DX_CACHETOTAL         0x01
#define DX_CACHEREMAINING	  0x02


typedef struct dx_iott DX_IOTT;


/*
 * DX_IOTT: I/O Transfer Table structure
 */
struct dx_iott {
   unsigned short  io_type;      /* Transfer type */
   unsigned short  rfu;          /* reserved */
   int             io_fhandle;   /* File descriptor */
   char            *io_bufp;     /* Pointer to base memory */
   unsigned long   io_offset;    /* File/Buffer offset */
   long int        io_length;    /* Length of data */
   DX_IOTT         *io_nextp;    /* Pointer to next DX_IOTT if IO_LINK */
   DX_IOTT         *io_prevp;    /* (optional) Pointer to previous DX_IOTT */ 
};


#endif

