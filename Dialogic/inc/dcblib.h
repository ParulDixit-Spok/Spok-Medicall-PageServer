/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1996-2007 Dialogic Corporation. All Rights Reserved.
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
 *        FILE: dcblib.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 *
 ****************************************************************************/

#ifndef __DCBLIB_H__
#define __DCBLIB_H__


/*
 * Ensure all structures are packed.
 */
#ifdef VME_SPAN
#else
#pragma pack(1)
#endif


#define DEV_CLASS_DCB      "DCB"

#define MAX_DCBDIGS 32 
#define DCB_DTMF     0

typedef struct dcb_digits {
   unsigned char  dsp;       /* DSP number                           */
   int            confid;    /* Conference Id                        */
   int            chan_num;  /* channel/time slot number             */
   int            chan_sel;  /* meaning of channel/time slot number  */
   int            chan_attr; /* channel attribute description        */
   unsigned char  digits[MAX_DCBDIGS+1]; /* ASCIIZ string of detected digits */
   unsigned char  dig_type;  /* Type of digit (DTMF) */
} DCB_DIGITS;

typedef struct dcb_ct {
	int confid;     /* Conference ID      */
	int chan_num;   /* Time slot number   */
	int chan_sel;   /* Time slot selector        */
} DCB_CT;

typedef struct ms_vol {
	unsigned char vol_control; /* Volume Control En/Disable             */
	unsigned char vol_up;      /* Increase Volume                       */
	unsigned char vol_reset;   /* Reset Volume                          */ 
	unsigned char vol_down;    /* Decrease Volume                       */
} MS_VOL;

typedef struct bridgecdt {
   MS_CDT cdtA;
   MS_CDT cdtB;
   unsigned int nBridgeID;
} TS_BRIDGECDT;

#define DT_DCB                    0x00008000      /* DCB device class        */
#define DCBEV_DIGIT               (DT_DCB| 0x96)  /* Digit detection event   */
#define DCBEV_CTU                 (DT_DCB| 0x97)  /* Resourcee Tbl Update event */
#define DCBEV_DELALLCONF          (DT_DCB| 0x98)  /* DeleteAllConf Event  	  */
#define DCBEV_ERREVT	 DTEV_ERREVT                 /* Define error event */
#define DCBEV_BRIDGEESTABLISHED   (DT_DCB| 0x99)  /* Bridge successful estab */
#define DCBEV_BRIDGEREMOVED       (DT_DCB| 0x9A)  /* Bridge successful remov */

#define ACTID_ON        0x01   /* Active Talker detect enable   */
#define ACTID_OFF       0x00   /* Active Talker detect disable  */

#define	ALGO_LONG		0x00	/* Active Talker Algorithm - Longest	*/
#define	ALGO_LOUD		0x01	/* Active Talker Algorithm - Loudest	*/

#define SET_EVENT       0x01   /* Used to set an event's status */
#define GET_EVENT       0x00   /* Used to get an event's status */
					   
#define CT_ENABLE               1      /* Enable Conf Tbl Event         */
#define CT_DISABLE              0      /* Disable Conf Tbl Event        */

/* Defines for DTMF detection bitmask elements */
#define CBMM_ZERO               0x0001
#define CBMM_ONE                0x0002
#define CBMM_TWO                0x0004
#define CBMM_THREE              0x0008
#define CBMM_FOUR               0x0010
#define CBMM_FIVE               0x0020
#define CBMM_SIX                0x0040
#define CBMM_SEVEN              0x0080
#define CBMM_EIGHT              0x0100
#define CBMM_NINE               0x0200
#define CBMM_STAR               0x0400
#define CBMM_POUND              0x0800
#define CBMM_A                  0x1000
#define CBMM_B                  0x2000
#define CBMM_C                  0x4000
#define CBMM_D                  0x8000
#define CBMM_ALL                0xFFFF

#define CBA_SETMSK              0x0  /* Set mask   */
#define CBA_ADDMSK              0x1  /* Add mask   */
#define CBA_SUBMSK              0x2  /* Sub mask   */

/*
 *  Standard DTMF Tone Ids for Volume Control
 */
#define VOL_DTMF_1       0x0001
#define VOL_DTMF_2       0x0002
#define VOL_DTMF_3       0x0003
#define VOL_DTMF_4       0x0004
#define VOL_DTMF_5       0x0005
#define VOL_DTMF_6       0x0006
#define VOL_DTMF_7       0x0007
#define VOL_DTMF_8       0x0008
#define VOL_DTMF_9       0x0009
#define VOL_DTMF_STAR    0x000e
#define VOL_DTMF_0       0x0000
#define VOL_DTMF_POUND   0x000f
#define VOL_DTMF_A       0x000a
#define VOL_DTMF_B       0x000b
#define VOL_DTMF_C       0x000c
#define VOL_DTMF_D       0x000d

/*
 Conference level parameters
*/
#define DCB_CNF_TONECLAMPING ((unsigned char) 1)
#define DCB_CNF_DTMF_MASK    ((unsigned char) 2)
#define DCB_CNF_NOTIFY       ((unsigned char) 3)

/*
  Possible values for parameter DCB_CNF_NOTIFY
*/
#define DCB_CNF_NOTIFY_ON   1
#define DCB_CNF_NOTIFY_OFF  0

/*
 * Restore default structure packing
 */
#ifdef VME_SPAN
#else
#pragma pack()
#endif

/*
 *	Useful declaration specification.
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

#ifdef _DTI_DLL

#ifndef  DllLinkage
#define	DllLinkage	DllExport
#endif

#else

#ifndef DllLinkage
#define DllLinkage	DllImport
#endif

#endif

#endif	/* __USING_DEF_FILE__	*/

#ifdef __CROSS_COMPAT_LIB__
#undef DllLinkage
#define DllLinkage    
#endif

/*
 * Prototypes for DCB960 library functions.
 */
#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus))
   DllLinkage int __cdecl dcb_setbrdparm(int, unsigned char, void *);     /* Set dcb Board parameter */
   DllLinkage int __cdecl dcb_getbrdparm(int, unsigned char, void *);     /* Get dcb Board parameter */

   DllLinkage int __cdecl dcb_setcnfparm(int hSrlDevice, int cnf_id, unsigned char parm_id, void * parm_value);     /* Set dcb Conference parameter   */
   DllLinkage int __cdecl dcb_getcnfparm(int hSrlDevice, int cnf_id, unsigned char parm_id, void * parm_value);     /* Get dcb Conference parameter   */

   DllLinkage int __cdecl dcb_SetPartyParm(int, int, MS_CDT *, unsigned int, void *, void *);     /* Set dcb party parameter */
   DllLinkage int __cdecl dcb_GetPartyParm(int, int, MS_CDT *, unsigned int, void *, void *);     /* Get dcb party parameter */
   DllLinkage int __cdecl dcb_estconf(int, MS_CDT *,int, int, int *);     /* Estab a conference      */
   DllLinkage int __cdecl dcb_delconf(int, int);                          /* Delete conference       */
   DllLinkage int __cdecl dcb_addtoconf(int,int, MS_CDT *);               /* Add a party to conf     */
   DllLinkage int __cdecl dcb_remfromconf(int,int, MS_CDT *);             /* Remove a party from conf*/
   DllLinkage int __cdecl dcb_getcde(int, int, MS_CDT *);                 /* Get conf. descr. elem   */
   DllLinkage int __cdecl dcb_setcde(int,int,MS_CDT *);                   /* Set conf. descr. elem   */
   DllLinkage int __cdecl dcb_monconf(int,int, long *);                   /* Monitor conference      */
   DllLinkage int __cdecl dcb_unmonconf(int, int);                        /* Unmonitor conference    */
   DllLinkage int __cdecl dcb_getatibits(int, unsigned long *);           /* Get ATI bits            */
   DllLinkage int __cdecl dcb_getrestbl(int, DCB_CT *);                   /* Get Conference table    */
   DllLinkage int __cdecl dcb_getdigitmsk(int, int ,unsigned short *);    /* Get digit mask          */
   DllLinkage int __cdecl dcb_open(char *, int);                          /* Open DCB/SC device      */
   DllLinkage int __cdecl dcb_tstcom(int, unsigned int);                  /* Test communication      */
   DllLinkage int __cdecl dcb_tstdat(int, unsigned int);                  /* Test data commtion      */
   DllLinkage int __cdecl dcb_close(int);                                 /* Close DCB/SC device     */   
   DllLinkage int __cdecl dcb_setdigitmsk(int, int, unsigned short, int); /* Set digit               */
   DllLinkage int __cdecl dcb_dsprescount(int, int *);                    /* Get config list         */
   DllLinkage int __cdecl dcb_getcnflist(int, int, int *, MS_CDT *);      /* Get DSP resource count  */
   DllLinkage int __cdecl dcb_gettalkers(int, int, int *, MS_CDT *);      /* Get Active talker       */
   DllLinkage int __cdecl dcb_evtstatus(int, int, int *);                 /* Set/Get an event's status */
   DllLinkage int __cdecl dcb_getevt(int, EV_EBLK *, int);                /* Get DCB event           */
   DllLinkage int __cdecl dcb_libinit(HINSTANCE LibHandle);               /* Init function for clib  */
   DllLinkage int __cdecl dcb_getatibitsEx(int, int*, DCB_CT*, void*);    /* Get ATI bits Extended            */
   DllLinkage int __cdecl dcb_DeleteAllConferences(int,unsigned short,void*);    /* Delete All Conferences    */
   DllLinkage int __cdecl dcb_CreateBridge( int hSrlDeviceA,
                                            int nConferenceIDA,
                                            int hSrlDeviceB,
                                            int nConferenceIDB,
                                            TS_BRIDGECDT *Bridgecdt,
                                            unsigned short unMode,
                                            void *rfu);    /* Create Conference Bridge */
   DllLinkage int __cdecl dcb_DeleteBridge( int hSrlDeviceA,
                                            int nConferenceIDA,
                                            int hSrlDeviceB,
                                            int nConferenceIDB,
                                            TS_BRIDGECDT *Bridgecdt,
                                            unsigned short unMode,
                                            void *rfu);    /* Delete Conference Bridge */
#else                                                                                                    
   DllLinkage int __cdecl dcb_setbrdparm();     /* Set dcb Board parameter   */
   DllLinkage int __cdecl dcb_getbrdparm();     /* Get dcb Board parameter   */

   DllLinkage int __cdecl dcb_setcnfparm();     /* Set dcb Conference parameter   */
   DllLinkage int __cdecl dcb_getcnfparm();     /* Get dcb Conference parameter   */

   DllLinkage int __cdecl dcb_SetPartyParm();     /* Set dcb party parameter */
   DllLinkage int __cdecl dcb_GetPartyParm();     /* Get dcb party parameter */
   DllLinkage int __cdecl dcb_estconf();        /* Establish a conference    */
   DllLinkage int __cdecl dcb_delconf();        /* Delete conference         */
   DllLinkage int __cdecl dcb_addtoconf();      /* Add a party to conf       */
   DllLinkage int __cdecl dcb_remfromconf();    /* Remove a party from conf  */
   DllLinkage int __cdecl dcb_getcde();         /* Get conf. descr. element  */
   DllLinkage int __cdecl dcb_setcde();         /* Set conf. descr. element  */
   DllLinkage int __cdecl dcb_monconf();        /* Monitor conference        */
   DllLinkage int __cdecl dcb_unmonconf();      /* Unmonitor conference      */
   DllLinkage int __cdecl dcb_getatibits();     /* Get ATI bits              */
   DllLinkage int __cdecl dcb_getrestbl();      /* Get Conference table      */
   DllLinkage int __cdecl dcb_getdigitmsk();    /* Get event mask            */
   DllLinkage int __cdecl dcb_setdigitmsk();    /* Set event mask            */
   DllLinkage int __cdecl dcb_tstcom();         /* Test communication        */
   DllLinkage int __cdecl dcb_tstdat();         /* Test data communication   */
   DllLinkage int __cdecl dcb_open();           /* Open dcb device           */
   DllLinkage int __cdecl dcb_close();          /* Close dcb device          */   
   DllLinkage int __cdecl dcb_dsprescount();    /* Get config list           */
   DllLinkage int __cdecl dcb_getcnflist();     /* Get DSP resource count    */
   DllLinkage int __cdecl dcb_gettalkers();     /* Get Active talker         */
   DllLinkage int __cdecl dcb_evtstatus();      /* Get/Set an event's status */
   DllLinkage int __cdecl dcb_getevt();         /* Get DCB/SC event          */
   DllLinkage int __cdecl dcb_libinit();        /* Init function for clib    */
   DllLinkage int __cdecl dcb_getatibitsEx();   /* Get ATI bits Extended     */
   DllLinkage int __cdecl dcb_DeleteAllConferences();    /* Delete All Conferences    */
   DllLinkage int __cdecl dcb_CreateBridge();   /* Create Conference Bridge    */
   DllLinkage int __cdecl dcb_DeleteBridge();   /* Delete Conference Bridge    */
#endif

#ifdef __cplusplus
}
#undef extern
#endif 

#endif // __DCBLIB_H__
