/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1999-2007 Dialogic Corporation. All Rights Reserved.
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
/************************************************************************
 **   FILENAME:          prtmslib.h
 **	  Author:			 Rajesh Girotra
 **   CREATED:           2/2/2000
 **   LAST MODIFIED:     2/2/2000
 **   VERSION:           1.0
 **   MODULE:            D42LIB.C
 **   DESCRIPTION:       Dialogic D/42 Library Compatability Module header file
 **

/************************************************************************/

#ifndef __PRTMSLIB_H__
#define __PRTMSLIB_H__

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

#include "prtmsd300lib.h"
#include "dtilib.h"
#include "protimslib.h"
#include "d42lib.h"


#define PRTMS_MAX_BOARD		8 /*Max Board supported for protims */
#define	TYP_D300				0x0013	/* This is for NEC PROTIMS on D/300 PCI-E1 */

/* Additional Board parameters */
#define D4BD_VPEMULTYP		0x0003

/* Additional channel parameter */
#define D4CH_CALLINGID			0x0001
#define D4CH_CALLEDID			0x0002
#define D4CH_DM_RINGS			0x001f

/*
 * Function prototypes for all library functions
 */

#if (defined (__cplusplus) || defined( __STDC__ ))

#ifdef __cplusplus
extern "C" {            // C Plus Plus function bindings
#endif

int		d42_Open(int *,char *,int);
int		d42_Close(int);
int		d42_Init(int, DLINK *, int *);
int		d42_StartLoopBack(int,DLINK *);
int 	d42_StopLoopBack(int, DLINK *, PRTMS_LBACK *);
int		d42_GetDLinkState(int, DLINK *,int *);
int 	d42_StartTrace(int, char *);
int 	d42_StopTrace(int);
int 	d42_GetFrame(int, L2_BLK * );
int 	d42_SndFrame(int, L2_BLK *);
int 	Dx_dial(int,char *,DX_CAP *,unsigned short);
int 	Dx_wtring(int, int, int, int);
int		Dx_sethook(int, int,unsigned short);
int		Dx_open(char *, char *);
int		Dx_close(int );
int		PRTMS_LASTERR(int );
int		Dx_setevtmsk(int, unsigned int);
char *	PRTMS_ERRMSGP(int);

#ifdef __cplusplus
}
#endif


#else

#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable funcs to be called from C++
#endif


DllExport
int	d42_Open();
DllExport
int	d42_Close();
DllExport
int	d42_Init();
DllExport
int	d42_StartLoopBack();
DllExport
int	d42_StopLoopBack();
DllExport
int	d42_GetDLinkState();
DllExport
int	d42_StartTrace();
DllExport
int	d42_StopTrace();
DllExport
int	d42_GetFrame( );
DllExport
int	d42_SndFrame();
DllExport
int	Dx_dial();
DllExport
int Dx_open();
DllExport
int Dx_close();
DllExport
int Dx_sethook();
DllExport
int Dx_wtring();
DllExport
int	PRTMS_LASTERR( );
DllExport
int Dx_setevtmsk();
DllExport
char * PRTMS_ERRMSGP();

#ifdef __cplusplus
}
#endif

#endif

#endif /* __PRTMSLIB_H__ */
