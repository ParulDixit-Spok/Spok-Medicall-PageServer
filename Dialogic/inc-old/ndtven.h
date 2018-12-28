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
 *        FILE: ndtven.h
 *     VERSION: 
 * DESCRIPTION: Module header file for the BRI/DTI veneer library.
 *
 *****************************************************************************/

#ifndef __NDTVEN_H__
#define __NDTVEN_H__

#ifdef VME_SPAN
#else
#pragma pack(1)
#endif

/*
 * Structure to hold pointers to library functions for all supported devices.
 */
typedef struct {

#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus) || defined(_BORLANDC))
             int    (*dt_getctinfo)(int, CT_DEVINFO *);
             int    (*dt_assignxmitslot)(int, SC_TSINFO *, USHORT);
             int    (*dt_getxmitslot)(int, SC_TSINFO *);
             int    (*dt_unassignxmitslot)(int, USHORT);
             int    (*dt_getparm)(int, ULONG, void *);
             int    (*dt_setparm)(int, ULONG, void *);
             int    (*dt_listen)(int, SC_TSINFO *);
             int    (*dt_unlisten)(int);
             int    (*dt_tstcom)(int, int);
             int    (*dt_tstdat)(int, int);
#else
             int    (*dt_getctinfo)();
             int    (*dt_assignxmitslot)();
             int    (*dt_getxmitslot)();
             int    (*dt_unassignxmitslot)();
             int    (*dt_getparm)();
             int    (*dt_setparm)();
             int    (*dt_listen)();
             int    (*dt_unlisten)();
             int    (*dt_tstcom)();
             int    (*dt_tstdat)();
#endif

#ifdef __cplusplus
}
#undef extern
#endif

} LIBBRI_FUNC, *LIBBRI_FUNC_PTR;


/*
 * Prototypes for BRI card library functions.
 */
#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus))
DllLinkage int   bri_getctinfo(int, CT_DEVINFO *),
                 bri_assignxmitslot(int, SC_TSINFO *, USHORT),
                 bri_getxmitslot(int, SC_TSINFO *),
                 bri_unassignxmitslot(int, USHORT),
                 bri_getparm(int, ULONG, void *),
                 bri_setparm(int, ULONG, void *),
                 bri_listen(int, SC_TSINFO *),
                 bri_unlisten(int),
                 bri_tstcom(int, int),
                 bri_tstdat(int, int);
#else
DllLinkage       bri_getctinfo(),
                 bri_assignxmitslot(),
                 bri_getxmitslot(),
                 bri_unassignxmitslot(),
                 bri_getparm(),
                 bri_setparm(),
                 bri_listen(),
                 bri_unlisten(),
                 bri_tstcom(),
                 bri_tstdat();
#endif

/*
 * Prototypes for SPAN-DTI card library functions.
 */
#if (defined(__STDC__) || defined(__cplusplus))
int   dti_getctinfo(int, CT_DEVINFO *),
      dti_assignxmitslot(int, SC_TSINFO *, USHORT),
      dti_getxmitslot(int, SC_TSINFO *),
      dti_unassignxmitslot(int, USHORT),
      dti_getparm(int, ULONG, void *),
      dti_setparm(int, ULONG, void *),
      dti_listen(int, SC_TSINFO *),
      dti_unlisten(int);
      dti_tstcom(int, int),
      dti_tstdat(int, int);
#else
int   dti_getctinfo(),
      dti_assignxmitslot(),
      dti_getxmitslot(),
      dti_unassignxmitslot(),
      dti_getparm(),
      dti_setparm(),
      dti_listen(),
      dti_unlisten(),
      dti_tstcom(),
      dti_tstdat();
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
