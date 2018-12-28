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
 * Filename:    eclib.h 
 * DESCRIPTION: Header file for all CSP library definitions.   
 *****************************************************************************/


#ifndef _hsw_1_eclib_h_H
#define _hsw_1_eclib_h_H

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
#endif //_VOX_DLL


// errno Error codes. These codes are retrieved by calling windows GetLastError().
// Note: These error codes are NOT retrieved by ATDX/EC.. function.
#define ERROR_CSP_CANNOT_LOAD_LIBECMT				0xE0000201
#define ERROR_CSP_ADDR_DM3EC_LIBINIT_NOTFOUND		0xE0000202
#define ERROR_CSP_ADDR_OPENNOTIF_NOTFOUND			0xE0000203
#define ERROR_CSP_ADDR_CLOSENOTIF_NOTFOUND			0xE0000204
#define ERROR_CSP_CANNOT_REG_CSP_COMPTECH			0xE0000205

/**
 ** Define Event Types
 **/
#define TEC_STREAM	0xE0					//termination event for ec_stream/ec_reciottdata
#define TEC_VAD		0xE1					//event returned when VAD sees energy

/* Event mask id (internal) */
#define DE_CONVERGED       22    /* EC converged */


/* Event mask values */
#define DM_CONVERGED ( 1 << (DE_CONVERGED - 1) )


/* Event Types */
#define TEC_CONVERGED      0xE2  /* event returned when echo canceller has converged */

#define TEC_RESET		    0xE3
#define TEC_RESETERR	    0xE4

// 03/27/07 Use 0x3B00 for base value going forward.
#define  CSPEV_BASE			0x3B00				// beginning base value
#define  CSPEV_LAST_EVENT	(CSPEV_BASE | 0xFF)	// Last event value


/*
 * Error codes returned by ATDV_LASTERR()
 */
#define EEC_UNSUPPORTED	EDX_BADPROD

/* Library Parameter */
#define ECCH_XFERBUFFERSIZE  ((PM_SHORT|PM_LIB |PM_DXXX|PM_CH)  | 0x0005L)
#define ECCH_SILENCECOMPRESS ((PM_SHORT|PM_LIB |PM_DXXX|PM_CH)  | 0x0013L)
#define ECCH_TRAILINGSILENCE ((PM_SHORT|PM_LIB |PM_DXXX|PM_CH)  | 0x0014L)
#define ECCH_INITIALDATA     ((PM_SHORT|PM_LIB |PM_DXXX|PM_CH)  | 0x0015L)

/* Firmware Parameter */
#define ECCH_NLP			 ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x021CL)
#define DXCH_BARGEIN		 ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x021DL)
#define ECCH_ADAPTMODE       ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x021EL)
#define ECCH_VADINITIATED    ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x021FL)
#define ECCH_ECHOCANCELLER   ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x0220L)
#define ECCH_SVAD            ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x0225L)
#define ECCH_CONVERGE        ((PM_SHORT|PM_FW  |PM_DXXX|PM_CH)  | 0x0226L)


#define DXCH_SPEECHTRIGG        DXCH_SPEECHPLAYTRIGG
#define DXCH_SPEECHWINDOW       DXCH_SPEECHPLAYWINDOW

//stop codes
#define SENDING		0x0001
#define RECEIVING	0x0002
#define FULLDUPLEX	(SENDING | RECEIVING)

typedef enum {
	EC_INITIAL_BLK,		// A block with the initial data
								// (for ASR engine adjustments) [optional]
	EC_SPEECH_BLK,			// A block with speech
	EC_SPEECH_LAST_BLK	// Last block of speech before silence.
								// The block size could actually be 0,
								// in which case the block simply indicates
								// that silence is now being compressed, until
								// EC_SPEECH_BLK blocks are sent again 
} EC_BlockType;

typedef enum {
	EC_LAST_BLK    = 0x1	// Last block of a streaming session
} EC_BlockFlags;

typedef struct _EC_BLK_INFO {
	EC_BlockType	type;
	unsigned int	flags;		// Bitmak with EC_BlockFlags values
	unsigned int	size;
	unsigned long	timestamp;	// Timestamp of first sample of the block
} EC_BLK_INFO;


//prototypes
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
DllLinkage long __cdecl ATEC_TERMMSK (int hDev);
DllLinkage int __cdecl ec_getxmitslot (int hDev, SC_TSINFO *lpSlot);
DllLinkage int __cdecl ec_getparm (int hDev, ULONG ParmNo, void *lpValue);
DllLinkage int __cdecl ec_listen (int hDev, SC_TSINFO *lpSlot);
DllLinkage int __cdecl ec_rearm (int hDev);
DllLinkage int	__cdecl ec_reciottdata (int hDev, DX_IOTT *iottp, DV_TPT *tptp,DX_XPB *xpbp,USHORT mode);
DllLinkage int __cdecl ec_setparm (int hDev, ULONG ParmNo, void *lpValue);
DllLinkage int	__cdecl ec_stopch (int hDev, ULONG StopType, USHORT mode);
DllLinkage int	__cdecl ec_resetch (int hDev, USHORT mode);
DllLinkage int	__cdecl ec_stream (int hDev, DV_TPT *tptp, DX_XPB *xpbp,
											 int (*callback) (int, char *, UINT), USHORT mode);
DllLinkage int __cdecl ec_unlisten (int hDev);
DllLinkage int __cdecl ec_getblkinfo (EC_BLK_INFO *lpBlkInfo);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_hsw_1_eclib_h_H
