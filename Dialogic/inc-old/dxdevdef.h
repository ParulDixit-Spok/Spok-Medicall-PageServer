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
 *        FILE: dxdevdef.h
 * DESCRIPTION: Device definitions header file
 **********************************************************************/
//01-10-2000 [cjl] task [3091] add IBI daughter board mask.

#ifndef __DXDEVDEF_H__
#define __DXDEVDEF_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif


/*
 * Miscellaneous defines
 */
#define PGRPHSHIFT               4        /* Shift for paragraphs-to-bytes */
#define MFBDCH                   1        /* Ch # for board MF/40 commands */
#define ADPCM_SIL                0x80     /* ADPCM silence value */
#define DX_NOPORT                0xFFFF   /* IO port not used */

#define SR_STOPREQUEST           0x0002
#define DL_LASTBUFFULL           0x0080   /* May need to put elsewhere */


/*
 * Defines for D4XE support.
 */
#define D4XE_LOCATOR             0xE0     /* D4XE locator offset in sram */
#define D4XE_REARM 		 0xE2
#define D4XE_MAXLOC              0xFF     /* D4xE locator max. value */
#define D4XE_DATA                0xED     /* interrupt rearm write data */


/*
 * Device features 
 */
#define FTR_VFS                  0x01  /* Voice store/forward capability */
#define FTR_CALLP                0x02  /* Call progress */
#define FTR_MF                   0x04  /* MF digit detection */
#define FTR_VR                   0x08  /* Voice recognition */
#define FTR_FAX                  0x10  /* Fax capabilitis */
#define FTR_PBXSL                0x20  /* Cnct to Northern Telecom SL1 PABX */
#define FTR_PBXSX                0x40  /* Cnct to Mitel PBX */
#define FTR_ROUTE                0x80  /* Board can route timeslots */

/*
 * Base technology type
 */
#define TYP_SPRINGBOARD          0x01  /* Springboard device */
#define TYP_DX                   0x02  /* D/4x or D/2x based device */

/*
 * Types of devices 
 */
#define TYP_BOARD                0x01  /* Board device */
#define TYP_CHANNEL              0x02  /* Channel device */
#define TYP_XAXIS                0x04  /* AMX x-axis device */
#define TYP_TIMESLOT             0x08  /* Timeslot device */


/*
 * Board type (BD_HWTYPE)
 */
#define TYP_D40                  0     /* D/40 functions alone */
#define TYP_D41                  1     /* D/40 functions and call analysis */

/*
 * Hardware configuration (BD_SYSCFG)
 */
#define CFG_LOOP                 0     /* Loop start interface in use */
#define CFG_DTI                  1     /* DTI/1xx installed */


/* Bitmask defines for daughterboard id's */
#define DB_MF40ID                0x0001      /* MF40 Daughterboard */
#define DB_FAXID                 0x0002      /* FAX Daughterboard */
#define DB_SLID                  0x0004      /* SL Daughterboard */
#define DB_SXID                  0x0008      /* SX Daughterboard */
#define DB_NSID                  0x0010      /* NS Daughterboard */
#define DB_VRID                  0x0020      /* VR Daughterboard */
#define DB_NE2ID                 0x0040		/* NEC Daughterboard */
#define DB_M1ID                  0x0080		/* M1 Daughterboard */
#define DB_PAID                  0x0100		/* PA Daughterboard */
#define DB_BRICKID					0x0200		/* Digital Brick Daughterboard */
#define DB_D82ID						0x0400		/* Chameleon Daughter board */
#define DB_IBIID						0x0800		/* IBI capable board */


/*
 * Device identifier defines
 */
#define DT_MEM      (0x00100000)    /* Shared RAM device class */
#define DT_SBA      (0x00200000)    /* SBA device class */
#define DT_SPAN     (0x00800000)    /* Span Card device class */

#define DT_DXXX     (0x00001000)    /* D/XXX device class */
#define __DT_DTI__      (0x00002000)    /* DTI device class */
#define DT_AMX      (0x00004000)    /* AMX device class */
#define DT_VRXXX    (0x00008000)    /* VR/XXX device class */

#define DT_CH       (0x00000100)    /* channel device class */
#define DT_TS       (0x00000200)    /* timeslot device class */

#define DT_DSP      (0x00000400)    /* DSP device class */

#define DT_DXBD     (DT_MEM|DT_DXXX)        /* DXX board device class */
#define DT_DXCH     (DT_MEM|DT_DXXX|DT_CH)  /* DXX channel device class */

#define DT_NDTBD    (DT_MEM|__DT_DTI__)         /* DTI board device class */
#define DT_NDTTS    (DT_MEM|__DT_DTI__|DT_TS)   /* DTI channel device class */


#define DI_D20      (DT_MEM|DT_DXXX|20)                 /* Dev = D/20 */
#define DI_D20BD    DI_D20                              /* Dev = D/20 bd */
#define DI_D20CH    (DT_MEM|DT_DXXX|DT_CH|20)           /* Dev = D/20 ch */

#define DI_D21      (DT_MEM|DT_DXXX|21)                 /* Dev = D/21 */
#define DI_D21BD    DI_D21                              /* Dev = D/21 bd */
#define DI_D21CH    (DT_MEM|DT_DXXX|DT_CH|21)           /* Dev = D/21 ch */

#define DI_D40      (DT_MEM|DT_DXXX|40)                 /* Dev = D/40 */
#define DI_D40BD    DI_D40                              /* Dev = D/40 bd */
#define DI_D40CH    (DT_MEM|DT_DXXX|DT_CH|40)           /* Dev = D/40 ch */

#define DI_D41      (DT_MEM|DT_DXXX|41)                 /* Dev = D/41 */
#define DI_D41BD    DI_D41                              /* Dev = D/41 bd */
#define DI_D41CH    (DT_MEM|DT_DXXX|DT_CH|41)           /* Dev = D/41 ch */

#define DI_D120     (DT_SBA|DT_MEM|DT_DXXX|120)         /* Dev = D/120 */
#define DI_D120CH   (DT_SBA|DT_MEM|DT_DXXX|DT_CH|120)   /* Dev = D/120 ch */

#define DI_D121     (DT_SBA|DT_MEM|DT_DXXX|121)         /* Dev = D/121 */
#define DI_D121CH   (DT_SBA|DT_MEM|DT_DXXX|DT_CH|121)   /* Dev = D/121 ch */

#define DI_VR40     (DT_MEM|DT_VRXXX|40)                /* Dev = VR/40 */
#define DI_VR40CH   (DT_MEM|DT_VRXXX|DT_CH|40)          /* Dev = VR/40 ch */

#define DI_VR121    (DT_SBA|DT_MEM|DT_VRXXX|121)        /* Dev = VR/120 */
#define DI_VR121CH  (DT_SBA|DT_MEM|DT_VRXXX|DT_CH|121)  /* Dev = VR/120 ch */

#define DI_VR160    (DT_SBA|DT_MEM|DT_VRXXX|160)        /* Dev = VR/160 */
#define DI_VR160CH  (DT_SBA|DT_MEM|DT_VRXXX|DT_CH|160)  /* Dev = VR/160 ch */

#define DI_VR160C   (DT_SBA|DT_MEM|DT_VRXXX|161)        /* Dev = VR/160C */
#define DI_VR160CCH (DT_SBA|DT_MEM|DT_VRXXX|DT_CH|161)  /* Dev = VR/160C ch */


#define DI_AMX80    (DT_AMX|80)                         /* Dev = AMX/80 */
#define DI_AMX80CH  (DT_AMX|DT_CH|80)                   /* Dev = AMX/80 ch */

#define DI_AMX81    (DT_AMX|81)                         /* Dev = AMX/81 */
#define DI_AMX81CH  (DT_AMX|DT_CH|81)                   /* Dev = AMX/81 ch */


#define DI_SPAN     (DT_SPAN|DT_DXXX)       /* Span card virtual D/4x board */
#define DI_T1SPAN   (DT_SPAN|__DT_DTI__|24)     /* Span card T1 board */
#define DI_E1SPAN   (DT_SPAN|__DT_DTI__|30)     /* Span card E1 board */

#define DI_DCB      (DT_SPAN|__DT_DTI__|DT_DSP)          /* DCB board */
#define DI_DCB320   (DT_SPAN|__DT_DTI__|DT_DSP|32)       /* DCB320 board */
#define DI_DCB640   (DT_SPAN|__DT_DTI__|DT_DSP|64)       /* DCB640 board */
#define DI_DCB960   (DT_SPAN|__DT_DTI__|DT_DSP|96)       /* DCB960 board */

#define	DTI_DEVNUM		2

typedef enum {
   DI_UNKNOWN,
   DI_D4XD,
   DI_D4XE,
   DI_SBD,
   DI_SBD200,
   DI_SPANCARD
} BoardTypes_e;


#endif

