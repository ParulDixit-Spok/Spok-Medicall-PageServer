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
 
/******************************************************************************** 
 * FILENAME:    scxlib.h
 * DESCRIPTION: SCX160 Defines, Structures & Function Prototypes 
 ********************************************************************************/

#ifndef __SCXLIB_H__ /* Avoid Redundant Includes */
#define __SCXLIB_H__

#include "ctinfo.h"

#define CT_PIDSCX160          0x00003101  /* SCX160 Device Product ID       */
#define DEV_CLASS_SCX         "SCX"


/*
 *-------SCX160 Board Parameters
 */
#define SCXBP_DRIVER_ERR_THRESH  0x02    /* SCX160 Driver Error Threshold   */
#define SCXBP_SCX_BER_HISTORY    0x03    /* SCX160 Bit-Error-Rate History   */
#define SCXBP_FIFO_ERR_THRESH    0x04    /* SCX160 FIFO Error Threshold     */
#define SCXBP_SCX_CLOCK_MODE     0x05    /* SCXBus Clock Mode               */
#define SCXBP_SC_CLOCK_MODE      0x09    /* SCBus Clock Mode                */
#define SCXBP_SC_MAP             0x0E    /* SCBus Datastream Alloc. Map     */
#define SCXBP_SCX_MAP            0x0F    /* SCXBus Datastream Alloc. Map    */
#define SCXBP_CLK_STATE_BYTE     0x28    /* SCX160 Clock State              */
#define SCXBP_SCX_BOARD_STAT     0x29    /* SCX160 Board Status             */
#define SCXBP_SCX_FIFO_STAT      0x2C    /* SCX160 FIFO Status              */
#define SCXBP_SCX_BER_STAT       0x2D    /* SCX160 Bit-Error-Rate Status    */
#define SCXBP_CLK_HISTORY        0x2E    /* SCX160 Clock History            */
#define SCXBP_CLOCK_REF          0x2F    /* SCX160 Clock Reference          */
#define SCXBP_SC_DR_ERR_CNT      0x30    /* SCX160 SC Bus Driver Err Count  */
#define SCXBP_SCX_DR_ERR_CNT     0x31    /* SCX160 SCX Bus Driver Err Count */
#define SCXBP_SC_FIFO_OVER_CNT   0x32    /* SCX160 SC FIFO Overflow Count   */
#define SCXBP_SCX_FIFO_OVER_CNT  0x33    /* SCX160 SCX FIFO Overflow Count  */
#define SCXBP_SC_FIFO_UNDR_CNT   0x34    /* SCX160 SC FIFO Underflow Count  */
#define SCXBP_SCX_FIFO_UNDR_CNT  0x35    /* SCX160 SCX FIFO Underflow Count */
#define SCXBP_SC_FIFO_SKEW_CNT   0x36    /* SCX160 SC FIFO Skew Count       */
#define SCXBP_SCX_FIFO_SKEW_CNT  0x37    /* SCX160 SCX FIFO Skew Count      */


/*
 *-------SCBus/SCXBus Clock Mode Parameters 
 *       (SCXBP_SCX_CLOCK_MODE, SCXBP_SC_CLOCK_MODE)
 */
#define SCXCM_MASTER             0x10    /* Clock Master                    */
#define SCXCM_SLAVE              0x20    /* Slave Clock                     */
#define SCXCM_FALLBACK_MASTER    0x30    /* Fallback Clock Master           */


/*
 *-------SCX160 Clock Reference Parameters (SCXBP_CLOCK_REF)
 */
#define SCXCS_INTERNAL           0x00    /* Use Internal Clock              */
#define SCXCS_SCBUS              0x01    /* Derive Clock from SCBus         */
#define SCXCS_SCXBUS_RIGHT       0x02    /* Derive Clock from SCXBus Rt Clk */
#define SCXCS_SCXBUS_LEFT        0x03    /* Derive Clock from SCXBus Lt Clk */
#define SCXCS_EXTERNAL           0x04    /* Use External Clock              */


/*
 *-------SCX160 Event Code
 */
#define SCXEV_SCBUS_CLOCK        0x01    /* SCBus Clock Event               */
#define SCXEV_SCXBUS_CLOCK       0x02    /* SCXBus Clock Event              */
#define SCXEV_EXT_CLOCK          0x04    /* External Clock Event            */
#define SCXEV_SCBUS_DRIVER       0x08    /* SCBus Bus Driver Event          */
#define SCXEV_SCXBUS_DRIVER      0x10    /* SCXBus Bus Driver Event         */
#define SCXEV_FIFO               0x20    /* SCX160 FIFO Event               */
#define SCXEV_SCBUS_HDLC         0x40    /* SCBus HDLC Event                */
#define SCXEV_SCXBUS_HDLC        0x80    /* SCXBus HDLC Event               */

// 03/27/07 Use 0x3C00 for base value going forward.
#define  SCXEV_BASE			0x3C00				// beginning base value
#define  SCXEV_LAST_EVENT	(SCXEV_BASE | 0xFF)	// Last event value


/*
 *-------SCX160 Clock History & State 
 *       (SCXBP_CLK_HISTORY, SCXBP_CLK_STATE_BYTE)
 */
#define SCXST_SCX_RCLOCK         0x01    /* SCXBus Right Clock State        */
#define SCXST_SCX_LCLOCK         0x02    /* SCXBus Left Clock State         */
#define SCXST_SC_CLOCK           0x04    /* SCBus Clock State               */
#define SCXST_SC_CLOCK_FAIL_SIG  0x08    /* SCBus Clock Fail Signal State   */
#define SCXST_EXTCLOCK           0x10    /* SCX160 External Clock State     */
#define SCXST_PLL_REF            0x20    /* SCX160 PLL Reference State      */
#define SCXST_INTCLOCK           0x40    /* SCX160 Internal Clock State     */

/*
 *-------SCXBus Clock Recovery Bitmask
 *  
 *       This bitmask is applicable to the SCXBP_CLK_HISTORY parameter
 *       only.  This bitmask is used to indicate that a T_SCXERROR 
 *       event was generated due to the recovery of a clock failure.
 *       This bitmask in NOT applicable to the SCXBP_CLK_STATE_BYTE
 *       parameter.
 */
#define SCXST_CLK_RECOVER        0x80    /* SCX160 Clock Recovery Event     */


/*
 *-------SCX160 Board State Return Data Structure
 */
typedef struct {
   unsigned char board_status;           /* Board Status                    */
   unsigned char self_test_code;         /* Self Test Return Code           */
   unsigned char board_type;             /* Board Type                      */
} tSCX_BRDST;


/*
 *-------SCX160 Board State Bitmask (SCXBP_SCX_BOARD_STAT)
 */
#define SCXBS_SELF_TEST          0x01    /* SCX160 Board Self Test Results  */
                                         /* 0=Pass, 1=Fail                  */
#define SCXBS_DOWNLOAD           0x02    /* SCX160 Board Download State     */
                                         /* 0=Not Downloaded, 1=Downloaded  */
#define SCXBS_RUN_STATE          0x04    /* SCX160 Firmware Run State       */
                                         /* 0=Running ROM, 1=Running RAM    */
#define SCXBS_SCX_CLOCK_STATE    0x38    /* SCXBus Clock State (Bits 4-6)   */
#define SCXBS_SC_CLOCK_STATE     0xC0    /* SCBus Clock State  (Bits 7-8)   */


/*
 *-------SCXBus Clock Status (SCXBS_SCX_CLOCK_STATE)
 */
#define SCX_MASTER               0x00    /* SCXBus Clock Master             */
#define SCX_WAIT_SLAVE           0x08    /* SCXBus Clock Master Waiting To  */
                                         /* Become Slave                    */
#define SCX_SLAVE                0x10    /* SCXBus Clock Slave              */
#define SCX_WAIT_FB_MASTER       0x18    /* SCXBus Clock Slave Waiting To   */
                                         /* Become Fallback Master          */
#define SCX_FB_MASTER            0x20    /* SCXBus Clock Fallback Master    */
#define SCX_WAIT_MASTER          0x28    /* SCXbus Clock Fallback Master    */
                                         /* Waiting to Become Master        */
#define SCX_WAIT_FB_SLAVE        0x30    /* SCXBus Fallback Master Waiting  */
                                         /* To Become Slave                 */

/*
 *-------SCBus Clock Status (SCXBS_SC_CLOCK_STATE)
 */
#define SC_MASTER                0x00    /* SCBus Clock Master              */
#define SC_WAIT_MASTER           0x40    /* Waiting to Become Master        */
#define SC_SLAVE                 0x80    /* SCBus Clock Slave               */
#define SC_FB_MASTER             0xC0    /* SCBus Clock Fallback Master     */


/*
 *-------SCX160 FIFO Status Bitmask (SCXBP_SCX_FIFO_STAT)
 */
#define SCXFS_SC_POS_SLIP        0x01    /* SCBus FIFO Positive Slip Exceed.*/
#define SCXFS_SC_NEG_SLIP        0x02    /* SCBus FIFO Negative Slip Exceed.*/
#define SCXFS_SCX_POS_SLIP       0x04    /* SCXBus FIFO Pos. Slip Exceeded  */
#define SCXFS_SCX_NEG_SLIP       0x08    /* SCXBus FIFO Neg. Slip Exceeded  */
#define SCXFS_SC_SKEW            0x10    /* SCBus FIFO Skew Exceeded        */
#define SCXFS_SCX_SKEW           0x20    /* SCXBus FIFO Skew Exceeded       */


/*
 *-------SCX160 Bit-Error-Rate Status Bitmask (SCXBP_SCX_BER_STAT)
 */
#define SCXBE_SC_BER             0x01    /* SCBus BER Threshold Exceeded    */
#define SCXBE_SCX_BER            0x02    /* SCXBus BER Threshold Exceeded   */


/*
 *-------SCX160 Event Masks for scx_clrevtmsk(), scx_setevtmsk()
 *       and scx_getevtmsk().
 */
#define SCXMM_SC_CLOCK           0x0001  /* SCX160 SCBus Clock Event Mask   */
#define SCXMM_SCX_CLOCK          0x0004  /* SCX160 SCXBus Clock Event Mask  */
#define SCXMM_EXT_CLOCK          0x0100  /* SCX160 External Clock Event Mask*/
#define SCXMM_SC_DRIVER          0x0002  /* SCX160 SCBus Driver Event Mask  */
#define SCXMM_SCX_DRIVER         0x0200  /* SCX160 SCXBus Driver Event Mask */
#define SCXMM_FIFO               0x0008  /* SCX160 FIFO Message Event Mask  */
#define SCXMM_SC_HDLC            0x0010  /* SCX160 SCBus HDLC Event Mask    */
#define SCXMM_SCX_HDLC           0x0400  /* SCX160 SCXBus HDLC Event Mask   */


/*
 *-------SCX160 Event Code(s)
 */
#define TSCX_ALARM              0x40C8  /* SCX160 Alarm Event               */

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
 * Function prototypes for SCX160 library functions.
 */
#if (defined(__STDC__) || defined(__cplusplus) || defined(__BORLANDC__))
#ifdef __cplusplus
extern "C" {
#endif

DllLinkage int __cdecl scx_close(short int devh);
DllLinkage int __cdecl scx_clrevtmsk(short int devh, unsigned short int mask);
DllLinkage int __cdecl scx_getbrdparm(short int devh, unsigned char param, void *valuep);
DllLinkage int __cdecl scx_getctinfo(short int devh, CT_DEVINFO *devinfop);
DllLinkage int __cdecl scx_getevtmsk(short int devh, unsigned short int *bitmskp);
DllLinkage int __cdecl scx_getver(short int devh);
DllLinkage int __cdecl scx_gtbdattr(short int devh, tSCX_BRDST *datbufp);
DllLinkage int __cdecl scx_open(char *name, short int oflags);
DllLinkage int __cdecl scx_setbrdparm(short int devh, unsigned char param, void *parmvalp);
DllLinkage int __cdecl scx_setevtmsk(short int devh, unsigned short int mask);
DllLinkage int __cdecl scx_tstcom(short int devh, short int tmo);
DllLinkage int __cdecl scx_tstdat(short int devh, short int tmo);
DllLinkage int __cdecl scx_libinit(HINSTANCE LibHandle);     /* Init function for clib lib */

#ifdef __cplusplus
}
#endif

#else

DllLinkage int __cdecl scx_close();
DllLinkage int __cdecl scx_clrevtmsk();
DllLinkage int __cdecl scx_getbrdparm();
DllLinkage int __cdecl scx_getctinfo();
DllLinkage int __cdecl scx_getevtmsk();
DllLinkage int __cdecl scx_getver();
DllLinkage int __cdecl scx_gtbdattr();
DllLinkage int __cdecl scx_open();
DllLinkage int __cdecl scx_setbrdparm();
DllLinkage int __cdecl scx_setevtmsk();
DllLinkage int __cdecl scx_tstcom();
DllLinkage int __cdecl scx_tstdat();
DllLinkage int __cdecl scx_libinit();                        /* Init function for clib lib */

#endif


#endif   /* __SCXLIB_H__ */
