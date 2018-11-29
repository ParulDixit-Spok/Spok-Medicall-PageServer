/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1993-2007 Dialogic Corporation. All Rights Reserved.
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
 *        FILE: ntapi.h
 *     VERSION: Windows Nt (Native) Production 1
 * DESCRIPTION: DTI Library Module header file for the veneer library.
 *              This library requires the Generic Driver & SRL Version 2.
 *****************************************************************************/

#ifndef __NDTAPI_H__
#define __NDTAPI_H__

#ifdef VME_SPAN
#else
#pragma pack(1)
#endif

/*
 * Signaling event mask values to be sent down to the firmware.
 */
#define DTMM_AONMSK   0x0001
#define DTMM_AOFFMSK  0x0002
#define DTMM_BONMSK   0x0004
#define DTMM_BOFFMSK  0x0008
#define DTMM_WINKMSK  0x0010
#define DTMM_CONMSK   0x0100
#define DTMM_COFFMSK  0x0200
#define DTMM_DONMSK   0x0400
#define DTMM_DOFFMSK  0x0800


/*
 * Structure to hold device descriptor and device tag identifying type of device
 */
typedef struct {
   int ddd;
   int tag;
   int next;
} DESC_TAG;


/*
 * Structure to hold pointers to library functions for all supported devices.
 */
typedef struct {

#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus))
   char *((*ATDT_BDSGBIT)(int));
   long   (*ATDT_BDMODE)(int);
   long   (*ATDT_DNLDVER)(int);
   long   (*ATDT_IDLEST)(int);
   long   (*ATDT_ROMVER)(int);
   long   (*ATDT_STATUS)(int);
   long   (*ATDT_TSMODE)(int);
   long   (*ATDT_TSSGBIT)(int);
   int    (*dt_assignxmitslot)(int, SC_TSINFO *, USHORT);
   int    (*dt_close)(int);
   int    (*dt_dial)(int, char *, int);
   int    (*dt_getctinfo)(int, CT_DEVINFO *);
   int    (*dt_getevtmsk)(int, int, USHORT *);
   int    (*dt_getparm)(int, ULONG, void *);
   int    (*dt_getxmitslot)(int, SC_TSINFO *);
   int    (*dt_listen)(int, SC_TSINFO *);
   int    (*dt_mtfcn)(int, UCHAR, int);
   int    (*dt_open)(char *, int);
   int    (*dt_rundiag)(int, UINT, char *);
   int    (*dt_setalrm)(int, UINT);
   int    (*dt_setevtmsk)(int, int, USHORT, int);
   int    (*dt_setidle)(int, UINT);
   int    (*dt_setparm)(int, ULONG, void *);
   int    (*dt_setsigmod)(int, UINT);
   int    (*dt_settssig)(int, USHORT, int);
   int    (*dt_settssigsim)(int, USHORT);
   int    (*dt_strtdn)(int);
   int    (*dt_tstcom)(int, int);
   int    (*dt_tstdat)(int, int);
   int    (*dt_unassignxmitslot)(int, USHORT);
   int    (*dt_unlisten)(int);
   int    (*dt_xmitalrm)(int, BYTE, UINT);
   int    (*dt_xmittone)(int, UINT);
   int    (*dt_xmitwink)(int, int);
#ifdef NT_MLTITHRD
   int    (*dt_getevt)(int, EV_EBLK *, int);
#endif
   void   (*dt_set_lasterr(int, int);
#else
   char *((*ATDT_BDSGBIT)());
   long   (*ATDT_BDMODE)();
   long   (*ATDT_DNLDVER)();
   long   (*ATDT_IDLEST)();
   long   (*ATDT_ROMVER)();
   long   (*ATDT_STATUS)();
   long   (*ATDT_TSMODE)();
   long   (*ATDT_TSSGBIT)();
   int    (*dt_assignxmitslot)();
   int    (*dt_close)();
   int    (*dt_dial)();
   int    (*dt_getctinfo)();
   int    (*dt_getevtmsk)();
   int    (*dt_getparm)();
   int    (*dt_getxmitslot)();
   int    (*dt_listen)();
   int    (*dt_mtfcn)();
   int    (*dt_open)();
   int    (*dt_rundiag)();
   int    (*dt_setalrm)();
   int    (*dt_setevtmsk)();
   int    (*dt_setidle)();
   int    (*dt_setparm)();
   int    (*dt_setsigmod)();
   int    (*dt_settssig)();
   int    (*dt_settssigsim)();
   int    (*dt_strtdn)();
   int    (*dt_tstcom)();
   int    (*dt_tstdat)();
   int    (*dt_unassignxmitslot)();
   int    (*dt_unlisten)();
   int    (*dt_xmitalrm)();
   int    (*dt_xmittone)();
   int    (*dt_xmitwink)();
#ifdef NT_MLTITHRD
   int    (*dt_getevt)();
#endif
   void   (*dt_set_lasterr)();
#endif

#ifdef __cplusplus
}
#undef extern
#endif

} LIB_FUNC, *LIB_FUNC_PTR;


/*
 * Prototypes for DTI card library functions.
 */
#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus))
char *oATDT_BDSGBIT(int);
long  oATDT_BDMODE(int),
      oATDT_DNLDVER(int),
      oATDT_IDLEST(int),
      oATDT_ROMVER(int),
      oATDT_STATUS(int),
      oATDT_TSMODE(int),
      oATDT_TSSGBIT(int);
int   odt_assignxmitslot(int, SC_TSINFO *, USHORT),
      odt_close(int),
      odt_dial(int, char *, int),
      odt_getctinfo(int, CT_DEVINFO *),
      odt_getevtmsk(int, int, USHORT *),
      odt_getparm(int, ULONG, void *),
      odt_getxmitslot(int, SC_TSINFO *),
      odt_listen(int, SC_TSINFO *),
      odt_mtfcn(int, UCHAR, int),
      odt_open(char *, int),
      odt_rundiag(int, UINT, char *),
      odt_setalrm(int, UINT),
      odt_setevtmsk(int, int, USHORT, int),
      odt_setidle(int, UINT),
      odt_setparm(int, ULONG, void *),
      odt_setsigmod(int, UINT),
      odt_settssig(int, USHORT, int),
      odt_settssigsim(int, USHORT),
      odt_strtdn(int),
      odt_tstcom(int, int),
      odt_tstdat(int, int),
      odt_unassignxmitslot(int, USHORT),
      odt_unlisten(int),
      odt_xmitalrm(int, BYTE, UINT),
      odt_xmittone(int, UINT),
      odt_xmitwink(int, int);
#ifdef NT_MLTITHRD
      odt_getevt(int, EV_EBLK *, int);
#endif
      odt_set_lasterr(int, int);
#else
char *oATDT_BDSGBIT();
long  oATDT_BDMODE(),
      oATDT_DNLDVER(),
      oATDT_IDLEST(),
      oATDT_ROMVER(),
      oATDT_STATUS(),
      oATDT_TSMODE(),
      oATDT_TSSGBIT();
int   odt_assignxmitslot(),
      odt_close(),
      odt_dial(),
      odt_getctinfo(),
      odt_getevtmsk(),
      odt_getparm(),
      odt_getxmitslot(),
      odt_listen(),
      odt_mtfcn(),
      odt_open(),
      odt_rundiag(),
      odt_setalrm(),
      odt_setevtmsk(),
      odt_setidle(),
      odt_setparm(),
      odt_setsigmod(),
      odt_settssig(),
      odt_settssigsim(),
      odt_strtdn(),
      odt_tstcom(),
      odt_tstdat(),
      odt_unassignxmitslot(),
      odt_unlisten(),
      odt_xmitalrm(),
      odt_xmittone(),
      odt_xmitwink();
#ifdef NT_MLTITHRD
      odt_getevt();
#endif
      odt_set_lasterr();
#endif

/*
 * Prototypes for SPAN-DTI card library functions.
 */
#if (defined(__STDC__) || defined(__cplusplus))
char *nATDT_BDSGBIT(int);
long  nATDT_BDMODE(int),
      nATDT_DNLDVER(int),
      nATDT_IDLEST(int),
      nATDT_ROMVER(int),
      nATDT_STATUS(int),
      nATDT_TSMODE(int),
      nATDT_TSSGBIT(int);
int   ndt_assignxmitslot(int, SC_TSINFO *, USHORT),
      ndt_close(int),
      ndt_dial(int, char *, int),
      ndt_getctinfo(int, CT_DEVINFO *),
      ndt_getevtmsk(int, int, USHORT *),
      ndt_getparm(int, ULONG, void *),
      ndt_getxmitslot(int, SC_TSINFO *),
      ndt_listen(int, SC_TSINFO *),
      ndt_mtfcn(int, UCHAR, int),
      ndt_open(char *, int),
      ndt_rundiag(int, UINT, char *),
      ndt_setalrm(int, UINT),
      ndt_setevtmsk(int, int, USHORT, int),
      ndt_setidle(int, UINT),
      ndt_setparm(int, ULONG, void *),
      ndt_setsigmod(int, UINT),
      ndt_settssig(int, USHORT, int),
      ndt_settssigsim(int, USHORT),
      ndt_strtdn(int),
      ndt_tstcom(int, int),
      ndt_tstdat(int, int),
      ndt_unassignxmitslot(int, USHORT),
      ndt_unlisten(int),
      ndt_xmitalrm(int, BYTE, UINT),
      ndt_xmittone(int, UINT),
      ndt_xmitwink(int, int);
#ifdef NT_MLTITHRD
      ndt_getevt(int, EV_EBLK *, int);
#endif
      ndt_set_lasterr(int, long);
#else
char *nATDT_BDSGBIT();
long  nATDT_BDMODE(),
      nATDT_DNLDVER(),
      nATDT_IDLEST(),
      nATDT_ROMVER(),
      nATDT_STATUS(),
      nATDT_TSMODE(),
      nATDT_TSSGBIT();
int   ndt_assignxmitslot(),
      ndt_close(),
      ndt_dial(),
      ndt_getctinfo(),
      ndt_getevtmsk(),
      ndt_getparm(),
      ndt_getxmitslot(),
      ndt_listen(),
      ndt_mtfcn(),
      ndt_open(),
      ndt_rundiag(),
      ndt_setalrm(),
      ndt_setevtmsk(),
      ndt_setidle(),
      ndt_setparm(),
      ndt_setsigmod(),
      ndt_settssig(),
      ndt_settssigsim(),
      ndt_strtdn(),
      ndt_tstcom(),
      ndt_tstdat(),
      ndt_unassignxmitslot(),
      ndt_unlisten(),
      ndt_xmitalrm(),
      ndt_xmittone(),
      ndt_xmitwink();
#ifdef NT_MLTITHRD
      ndt_getevt();
#endif
     ndt_set_lasterr();
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
