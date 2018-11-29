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
 * Filename:    dm3dtilib.h 
 * DESCRIPTION: Header File for Dialogic Windows library.   
 *****************************************************************************/
 
#include "dtilib.h"

#define SPR_TYPE  0x1010
#define DM3_TYPE  0x2020

#ifndef __DM3DTILIB_H__
#define __DM3DTILIB_H__

#ifdef VME_SPAN
#else
#pragma pack(1)
#endif



/*
 *  Module Definition file compatibility.
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

#else   /* __USING_DEF_FILE__   */

#ifndef DllExport
#define DllExport   __declspec( dllexport )
#endif  /*  Dllexport   */

#ifndef DllImport
#define DllImport __declspec( dllimport )
#endif  /*  Dllimport   */

#ifdef _DTI_DLL

#ifndef DllLinkage
#define DllLinkage  DllExport
#endif

#else

#ifndef DllLinkage
#define DllLinkage  DllImport
#endif

#endif

#endif  /* __USING_DEF_FILE__   */

#ifdef __CROSS_COMPAT_LIB__
#undef  DllLinkage
#define DllLinkage    
#endif

/*
 * Prototypes for all library functions.
 */
#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif


/// DM3

#if (defined(__STDC__) || defined(__cplusplus) || defined(__BORLANDC__))
char *DM3_ATDT_BDSGBIT(int);
long  DM3_ATDT_BDMODE(int),
				  DM3_ATDT_DNLDVER(int),
				  DM3_ATDT_IDLEST(int),
				  DM3_ATDT_ROMVER(int),
				  DM3_ATDT_STATUS(int),
				  DM3_ATDT_TSMODE(int),
				  DM3_ATDT_TSSGBIT(int),
				  DM3_ATDT_FWTYPE(int),
				  DM3_ATDT_BRDTYPE(int),
				  DM3_ATDT_STATE(int);
int   DM3_dt_close(int),
				  DM3_dt_dial(int, char *, unsigned int),
				  DM3_dt_getctinfo(int, CT_DEVINFO *),
				  DM3_dt_getevtmsk(int, int, unsigned short *),
				  DM3_dt_getparm(int, unsigned long, void *),
				  DM3_dt_getxmitslot(int, SC_TSINFO *),
				  DM3_dt_listen(int, SC_TSINFO *),
				  DM3_dt_mtfcn(int, unsigned char, unsigned int),
				  DM3_dt_open(char *, int),
				  DM3_dt_rundiag(int, unsigned int, char *),
				  DM3_dt_setalrm(int, unsigned int),
				  DM3_dt_setevtmsk(int, int, unsigned short, int),
				  DM3_dt_setidle(int, unsigned int),
				  DM3_dt_setparm(int, unsigned long, void *),
				  DM3_dt_setsigmod(int, unsigned int),
				  DM3_dt_settssig(int, unsigned short, int),
				  DM3_dt_settssigsim(int, unsigned short),
				  DM3_dt_tstcom(int, unsigned int),
				  DM3_dt_tstdat(int, unsigned int),
				  DM3_dt_unlisten(int),
				  DM3_dt_xmitalrm(int, unsigned char, unsigned int),
				  DM3_dt_xmittone(int, unsigned int),
				  DM3_dt_xmitwink(int, unsigned int),
				  DM3_dt_route(int, int, int, int),
				  DM3_dt_getevt(int, EV_EBLK *, int),
				  DM3_dt_setiostatus(int, int);
void  DM3_dt_set_lasterr(int, int);
	  
/* Clib interface functions */
int  dm3dt_libinit(HINSTANCE LibHandle);     /* Init function for clib lib */
int DM3_dt_QueryDevice (char *szName);
int DM3_dt_getstatistics(int hSrlDevice, TSdtStatisticsList* statisticsList, unsigned int mode);
#else
char *DM3_ATDT_BDSGBIT();
long  DM3_ATDT_BDMODE(),
				  DM3_ATDT_DNLDVER(),
				  DM3_ATDT_IDLEST(),
				  DM3_ATDT_ROMVER(),
				  DM3_ATDT_STATUS(),
				  DM3_ATDT_TSMODE(),
				  DM3_ATDT_TSSGBIT(),
				  DM3_ATDT_FWTYPE(int),
				  DM3_ATDT_BRDTYPE(int),
				  DM3_ATDT_STATE(int);
 int   DM3_dt_close(),
				  DM3_dt_dial(),
				  DM3_dt_getctinfo(),
				  DM3_dt_getevtmsk(),
				  DM3_dt_getparm(),
				  DM3_dt_getxmitslot(),
				  DM3_dt_listen(),
				  DM3_dt_mtfcn(),
				  DM3_dt_open(),
				  DM3_dt_rundiag(),
				  DM3_dt_setalrm(),
				  DM3_dt_setevtmsk(),
				  DM3_dt_setidle(),
				  DM3_dt_setparm(),
				  DM3_dt_setsigmod(),
				  DM3_dt_settssig(),
				  DM3_dt_settssigsim(),
				  DM3_dt_tstcom(),
				  DM3_dt_tstdat(),
				  DM3_dt_unlisten(),
				  DM3_dt_xmitalrm(),
				  DM3_dt_xmittone(),
				  DM3_dt_xmitwink(),
				  DM3_dt_route(),
				  DM3_dt_getevt(),
				  DM3_dt_setiostatus();
void  DM3_dt_set_lasterr();
/* Clib interface functions */
int  dm3dt_libinit(); 
int DM3_dt_QueryDevice ();
int DM3_dt_getstatistics();
#endif

#ifdef __cplusplus

}
#undef extern
#endif

#ifdef VME_SPAN
#else
#pragma pack()
#endif

#endif
