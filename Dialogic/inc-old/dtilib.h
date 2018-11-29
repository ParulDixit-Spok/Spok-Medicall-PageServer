/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2005-2007 Dialogic Corporation. All Rights Reserved.
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
 * Filename:    dtilib.h 
 * DESCRIPTION: Header File for Dialogic Windows library..   
 *****************************************************************************/

// 08/06/99 [VJ] Added DTI CAS functions
//06-08-2000 [cjl] add CT_BMH100 to bus defines
//10-05-2000 [cjl] sync CT_ defines with voice.

#ifndef __DTILIB_H__
#define __DTILIB_H__

#ifdef VME_SPAN
#else
#pragma pack(1)
#endif

#include "ctinfo.h"

/*
 * Device specific identifiers
 */
#define DT_IO  0x00000000    /* I/O interface dev class */
#define DT_DTI 0x00002000    /* DTI device class */

#define DI_DTI100   (DT_IO|DT_DTI|100)       /* Device is a DTI/100 */
#define DI_DTI100CH (DT_IO|DT_DTI|DT_TS|100) /* Device is a DTI/100 ts */

#define DI_DTI101   (DT_IO|DT_DTI|101)       /* Device is a DTI/101 */
#define DI_DTI101CH (DT_IO|DT_DTI|DT_TS|101) /* Device is a DTI/101 ts */

#define DI_DTI124   (DT_IO|DT_DTI|124)       /* Device is a DTI/124 */
#define DI_DTI124CH (DT_IO|DT_DTI|DT_TS|124) /* Device is a DTI/124 ts */

#define DI_DTI212   (DT_IO|DT_DTI|212)       /* Device is a DTI/212 */
#define DI_DTI212CH (DT_IO|DT_DTI|DT_TS|212) /* Device is a DTI/212 ts */

#define DI_DTI211   (DT_IO|DT_DTI|211)       /* Device is a DTI/211 */
#define DI_DTI211CH (DT_IO|DT_DTI|DT_TS|211) /* Device is a DTI/211 ts */

#define TYP_DTI     0x04        /* Digital telephony interface device */


/*
 * Error return codes
 */
#define DTI_ENOIDLE 0x0001     /* DTI/1xx not idle                     */
#define DTI_EINTERR 0x0002     /* Driver internal error                */
#define DTI_ETIMOUT 0x0003     /* Driver timed-out waiting for ACK/NAK */
#define DTI_ENOMEM  0x0004     /* Driver ran out of memory for reply   */

/*
 * Defines for Sync mode and async mode
 */
#define EV_ASYNC     0x8000
#define EV_SYNC      0x0000

/*
 * Events for dt_setevtmsk() and dt_getevtmsk()
 */
#define DTG_T1ERREVT 0x01         /* T1 error event */
#define DTG_E1ERREVT DTG_T1ERREVT /* E1 error event */
#define DTG_SIGEVT   0x02         /* Signaling bit event */
#define DTG_PDIGEVT  0x03         /* Pulse digit event */

/*
 * Action values for dt_setevtmsk() and dt_settssig() functions
 */
#define DTA_SETMSK   0      /* set all mask bits & clear all others */
#define DTA_ADDMSK   1      /* set all mask bits & ignore all others */
#define DTA_SUBMSK   2      /* clr all mask bits & ignore all others */

/*
 * Enable/disable state flags
 */
#define DTIS_ENABLE  1      /* enable DTI feature */
#define DTIS_DISABLE 0      /* disable DTI feature */

/*
 * Transmit/receive state flags
 */
#define BOTH         0x00
#define TRANSMIT     0x01
#define RECEIVE      0x02 

/*                   
 * Generic flags to enable or diable features
 */
#ifndef ON
#define ON              0x01   /* Enable feature  */
#endif

#ifndef OFF
#define OFF             0x00   /* disable feature */
#endif

/*
 * Signaling mode flags
 */
#define DTM_SIGINS   0       /* Inserted signaling mode */
#define DTM_TRANSP   1       /* Transparent signaling mode */

/*
 * Signaling bit flags
 */
#define DTB_ABIT   0x01                   /* A bit flag */
#define DTB_BBIT   0x02                   /* B bit flag */
#define DTB_CBIT   0x04                   /* C bit flag */
#define DTB_DBIT   0x08                   /* D bit flag */
#define DTC_ABIT   0x10                   /* A bit changed */
#define DTC_BBIT   0x20                   /* B bit changed */
#define DTC_CBIT   0x40                   /* C bit changed */
#define DTC_DBIT   0x80                   /* D bit changed */
#define DTB_AON   (DTC_ABIT | DTB_ABIT)   /* A bit SET */
#define DTB_AOFF   DTC_ABIT               /* A bit CLEAR */
#define DTB_BON   (DTC_BBIT | DTB_BBIT)   /* B bit SET */
#define DTB_BOFF   DTC_BBIT               /* B bit CLEAR */
#define DTB_CON   (DTC_CBIT | DTB_CBIT)   /* C bit SET */
#define DTB_COFF   DTC_CBIT               /* C bit CLEAR */
#define DTB_DON   (DTC_DBIT | DTB_DBIT)   /* D bit SET */
#define DTB_DOFF   DTC_DBIT               /* D bit CLEAR */

/*
 * Idle value flags
 */
#define  IDLE_7F        0       /* idle value is 7F - T1 */
#define  IDLE_FF        1       /* idle value is FF - T1 */
#define  IDLE_54        IDLE_7F /* idle value is 54 - E1 */
#define  IDLE_D5        IDLE_FF /* idle value is D5 - E1 */
   
/*
 * Shook state flags
 */
#define  DT_ONHOOK      0
#define  DT_OFFHOOK     1

/*
 * Timeslot status 
 */
#define  DTST_INACTIVE  0
#define  DTST_DIAL      1
#define  DTST_BUSY      1
#define  DTST_WINK      2
#define  DTST_WTCALL    3

/*
 *-------Alarm handling modes
 */
#define  DTA_NONE      0x00      /* No alarm handling */
#define  DTA_TERM      0x01      /* Terminate alarm handling mode */
#define  DTA_DROP      0x02      /* Drop and insert alarm handling */
#define  DTA_MAX       0x02      /* Maximum value for alarm handling mode */

/*
 * Alarms
 */
#define YELLOW         0x00    /* Yellow alarm */
#define BLUE           0x01    /* Blue Alarm */
#define DEA_REMOTE     YELLOW  /* Remote alarm */
#define DEA_UNFRAMED1  BLUE    /* Unframed all 1's */
#define DEA_SIGNALALL1 0x02    /* Signalling all 1's */
#define DEA_DISTANTMF  0x03    /* Distant Multi Frame alarm */

/*
 * Unsolicited DTI event types
 */
#define DTEV_RCVPDG   (DT_DTI | 0x48) /* Received a pulse digit */
#define DTEV_T1ERRC   (DT_DTI | 0x49) /* T1 error condition event */
#define DTEV_E1ERRC    DTEV_T1ERRC    /* E1 error condition event == T1 error */
#define DTEV_COMRSP   (DT_DTI | 0x4A) /* Successful com test */
#define DTEV_DATRSP   (DT_DTI | 0x4B) /* Response to data test */
#define DTEV_PDDONE   (DT_DTI | 0x4C) /* Pulse dial complete */
#define DTEV_SIG      (DT_DTI | 0x4D) /* Signalling event */
#define DTEV_RETDIAG  (DT_DTI | 0x4E) /* Diagnostic complete */
#define DTEV_WINKCPLT (DT_DTI | 0x4F) /* Wink complete */
#define DTEV_MTFCNCPT (DT_DTI | 0x50) /* Multi-tasking func complete */
#define DTEV_CLKFAIL  (DT_DTI | 0x51) /* Clock FallBack Event */
#define DTEV_CASTEVT  (DT_DTI | 0x52) /* CAS DTI template matched event */
#define DTEV_CASSENDENDEVT (DT_DTI | 0x53) /* CAS DTI template send finished event */
#define DTEV_ERREVT   (DT_DTI | 0xF0) /* Error condition event */
#define DTEV_OUTSERVICE (DT_DTI | 0xF1) /* Device out of Service event */
#define DTEV_GETSTATISTICS (DT_DTI | 0xF2) /* Get Statistics completed successfully  */
#define DTEV_GETSTATISTICSFAILED (DT_DTI | 0xF3) /* Get Statistics failed */


/* Define device class */
#define DEV_CLASS_DTI           "DTI"

/*
 *-------Successful return value
 */
#define EDT_SUCC        0x00  /* Message acknowledged */

/*
 * Driver error codes
 */
#define EDT_RANGEERR    0x01  /* Bad/overlapping physical mem range */
#define EDT_TMOERR      0x02  /* Timed out waiting for reply */
#define EDT_SIZEERR     0x03  /* Message too big or too small */
#define EDT_BADBRDERR   0x04  /* Board missing or dead */
#define EDT_NOMEMERR    0x05  /* Can't map or allocate memory */
#define EDT_BADCMDERR   0x06  /* Invalid or undefined command */
#define EDT_PARAMERR    0x07  /* Invalid command parameter */
#define EDT_FWERR       0x08  /* FW returned an error as reply */
#define EDT_SKIPRPLYERR 0x09  /* A required reply was skipped */
#define EDT_NOIDLEERR   0x0A  /* Timeslot is not in idle state */
#define EDT_SYSTEM      0x0B  /* Unix system error - actual error in errno */
#define EDT_BADDEV      0x0C  /* Bad device error */
#define EDT_BUSY        0x0E  /* Device BUSY */
#define EDT_OUTSERVICE  0x0F  /* Device is OUT_(of)_SERVICE */

/*
 *    HMP Error Codes
 */
#define EDT_HSIBRIDGEERR 0x10  /* Error occurred when creating HSI bridge connections */

/*
 *	DM3 Error
 */
#define EDT_DM3_CLEO    0x81  /* CLEO Return Error */
#define EDT_DM3_SRREG   0x82  /* SRL register Device Error */

/*
 *-------DTI communications protocol error codes
 */
#define EDT_DATTO       0x91  /* Data reception timed out */
#define EDT_MBUND       0x92  /* Not enough bytes for a mb req. */
#define EDT_MBOVR       0x93  /* Message was too long */
#define EDT_MBTERM      0x94  /* Term byte other than FEH or FFH */
#define EDT_MBPORT      0x95  /* Rcvd mb data on prt other than 0 or 1 */
#define EDT_MBINV       0x96  /* 1st byte appeared on data port */
#define EDT_BADPORT     0x97  /* 1st byte appeared on reserved port */

/*
 *-------DTI message processing error codes
 */
#define EDT_INVCFG      0xA0  /* Invalid cfg area or EEPROM cfg data */
#define EDT_INVMSG      0xA1  /* Invalid message */
#define EDT_MBFMT       0xA2  /* Wrong # bytes for desired mb req. */
#define EDT_MSGCNT      0xA3  /* Count received didn't match actual */
#define EDT_CHKSUM      0xA4  /* Bad chksum */
#define EDT_NOTDNLD     0xA5  /* Not downloaded */
#define EDT_MBIMM       0xA6  /* Received an immediate term */
#define EDT_INVTS       0xA7  /* Bad timeslot */
#define EDT_INVBD       0xA8  /* Wrong board */
#define EDT_SIGINS      0xA9  /* Insertion signaling not enabled */
#define EDT_BADGLOB     0xAA  /* Bad global parameter number */
#define EDT_TSASN       0xAB  /* Time slot already assigned */
#define EDT_BADVAL      0xAC  /* Bad value */
#define EDT_STARTED     0xAD  /* Can't start when already started */
#define EDT_PDACT       0xAE  /* Already Pulse Dialing */
#define EDT_TSBSY       0xAE  /* Time Slot is busy (multi-tasking func) */
#define EDT_PDOFFHK     0xAF  /* Must be in offhook idle state to pd. */
#define EDT_PDSIG       0xB0  /* Can't disable insertion when pd */
#define EDT_MTSIG       0xB0  /* Can't disable insertion when multi-tsking */
#define EDT_BADCNT      0xB1  /* Count of bytes requested is bad */
#define EDT_ADDRS       0xB2  /* Bad address */
#define EDT_SIGTO       0xB3  /* xmt/rcv did not update in time */
#define EDT_NOTSACS     0xB4  /* Can't use tsacs on this device */
#define EDT_NOCLK       0xB5  /* No PEB clocking source */
#define EDT_DTTSTMOD    0xB6  /* In test mode; cannot set board mode */
#define EDT_BITBSY      0xB7  /* Bit is busy, i.e. already set */
#define EDT_INVSIGST    0xB8  /* Invalid signalling state */
#define EDT_NOWTCALL    0xB9  /* Not waiting for a call */
#define EDT_NOTPEBMODE  0xBA  /* Not in PEB mode */
#define EDT_WTCALL      0xBB  /* Wait for call in progress */
#define EDT_DEVIOERR    0xBC  /* Internal Device I/O Control Error */
#define EDT_BRGEPTYERR  0xBD  /* Error Encountered in bridge party transaction */
//#define EDT_CASTERR     0xBC  /* CAS DTI error */

/*
 *-------SPAN SCSA mode message processing error codes
 */
#define EDT_SH_LIBBSY     0x51 /* Switching Handler(SH) library is busy */
#define EDT_SH_BADINDX    0x52 /* Invalid SH index number */
#define EDT_SH_LIBNOTINIT 0x53 /* SH library has not been initialized */
#define EDT_SH_NOCLK      0x54 /* Clock fallback failed */
#define EDT_SH_MISSING    0x56 /* SH is not present */
#define EDT_SH_BADMODE    0x5A /* Invalid bus mode */
#define EDT_SH_BADLCLTS   0x5F /* Invalid local timeslot number */
#define EDT_SH_BADTYPE    0x60 /* Invalid local timeslot type */
#define EDT_SH_BADEXTTS   0x61 /* External ts unsupported at current clk rate */
#define EDT_SH_LCLTSCNCT  0x62 /* Local ts already connected to SCbus */
#define EDT_SH_LCLDSCNCT  0x63 /* Local ts already disconnected from SCbus */


/*
 *--------General values
 */
#define DTV_NUMTS       24    /* Number of timeslots */
#define DTV_NUMBD       4     /* Max number of boards */
#define DTV_MAXMSG      48    /* Max bytes in a message */

/*
 *--------Pulse dialing variables
 */
#define DTV_MAXPDCHR    0x1F  /* Maximum pulse digit chars. */
#define DTV_FLASHCHR    '&'   /* Default flash char (can be changed) */
#define DTV_DELAYCHR    ','   /* Delay character (not changable) */

/*
 *-------Board status masks
 */
#define DTS_DNLDERR     0x01  /* Bit 0 0=ok, 1=err */
#define DTS_DNLD        0x02  /* Bit 1 0=not dwnld, 1=dwnld */
#define DTS_DNLDSTRT    0x04  /* Bit 2 0=not started, 1=started */
#define DTS_CLKBIT1     0x08  /* Bit 3 - clock source bit 1 */
#define DTS_CLKBIT2     0x10  /* Bit 4 - clock source bit 0 */
#define DTS_RLOOPBIT    0x20  /* Remote loopback bit 0=off, 1=on */

/*
 *-------The results of run diagnostic on DTI/1xx
 */
#define  DTDE_NOERR    0x00      /* No errors */
#define  DTDE_MEMTST   0x01      /* Memory test failed. */
#define  DTDE_LIUFAIL  0x02      /* Read/Write to LIU failed. */
#define  DTDE_RIMR     0x03      /* Read of RIMR reg. failed. */
#define  DTDE_BVCR     0x04      /* Read of BVCR reg. failed. */
#define  DTDE_ECR      0x05      /* Read of ECR  reg. failed. */
#define  DTDE_CCR      0x06      /* Read of CCR  reg. failed. */
#define  DTDE_RCR      0x07      /* Read of RCR  reg. failed. */
#define  DTDE_TCR      0x08      /* Read of TCR  reg. failed. */
#define  DTDE_TIR1     0x09      /* Read of TIR1 reg. failed. */
#define  DTDE_TIR2     0x0A      /* Read of TIR2 reg. failed. */
#define  DTDE_TIR3     0x0B      /* Read of TIR3 reg. failed. */
#define  DTDE_TTR1     0x0C      /* Read of TTR1 reg. failed. */
#define  DTDE_TTR2     0x0D      /* Read of TTR2 reg. failed. */
#define  DTDE_TTR3     0x0E      /* Read of TTR3 reg. failed. */
#define  DTDE_RMR1     0x0F      /* Read of RMR1 reg. failed. */
#define  DTDE_RMR2     0x10      /* Read of RMR2 reg. failed. */
#define  DTDE_RMR3     0x11      /* Read of RMR3 reg. failed. */
#define  DTDE_XCVRPAT  0x12      /* Read of XCVR reg. failed pattern test. */
#define  DTDE_ROMCHK   0x13      /* Bad rom check sum. */

/*
 *-------The results of run diagnostic on DTI/2xx.
 */
#define  D2DE_NOERR    0x00      /* No errors. */
#define  D2DE_MEMTST   0x01      /* Memory test failed. */
#define  D2DE_LIUFAIL  0x02      /* Read/Write to LIU failed. */
#define  D2DE_XCVRFAIL 0x03      /* Read of XCVR reg. failed pattern test. */
#define  D2DE_ROMCHK   0x04      /* Bad rom check sum. */
#define  D2DE_BRDCFG   0x05      /* Invalid board configuration data. */
#define  D2DE_INVEE    0x06      /* Invalid EEPROM data. */

/*
 *--------Message Mask equates.
 */
#define DTMM_AON   (DTC_ABIT | DTB_ABIT)   /* Mask for A went to on msgs. */
#define DTMM_AOFF  (DTC_ABIT | 0x0100)     /* Mask for A went to off msgs. */
#define DTMM_BON   (DTC_BBIT | DTB_BBIT)   /* Mask for B went to on msgs. */
#define DTMM_BOFF  (DTC_BBIT | 0x0200)     /* Mask for B went to off msgs. */
#define DTMM_CON   (DTC_CBIT | DTB_CBIT)   /* Mask for C went to on msgs. */
#define DTMM_COFF  (DTC_CBIT | 0x0400)     /* Mask for C went to off msgs. */
#define DTMM_DON   (DTC_DBIT | DTB_DBIT)   /* Mask for D went to on msgs. */
#define DTMM_DOFF  (DTC_DBIT | 0x0800)     /* Mask for D went to off msgs. */
#define DTMM_WINK   0x1000                 /* Mask for A bit wink msgs. */

/*
 *--------Masks for timeslot's transmit and receive signalling bits.
 */
#define  DTSG_XMTA     0x01      /* Value of transmit A. */
#define  DTSG_XMTB     0x02      /* Value of transmit B. */
#define  DTSG_XMTC     0x04      /* Value of transmit C. */
#define  DTSG_XMTD     0x08      /* Value of transmit D. */
#define  DTSG_RCVA     0x10      /* Value of receive A.  */
#define  DTSG_RCVB     0x20      /* Value of receive B.  */
#define  DTSG_RCVC     0x40      /* Value of receive C.  */
#define  DTSG_RCVD     0x80      /* Value of receive D.  */

/*
 *--------DRP routing masks.
 */
#define  DTDRP_XMT     0x01      /* DRP routing for transmit. */
#define  DTDRP_RCV     0x02      /* DRP routing for receive.  */

/*
 *-------Global parameter entry numbers.
 */
#define  DTG_MINPDON   0x00      /* Minimum pulse dial reception on. */
#define  DTG_MINPDOFF  0x01      /* Minimum pulse dial reception off. */
#define  DTG_MAXPDOFF  0x02      /* Maximum interdigit reception delay. */
#define  DTG_MINIPD    0x03      /* Maximum pulse digit reception off. */
#define  DTG_P_MK      0x04      /* Send pulse make time. */
#define  DTG_P_BK      0x05      /* Send pulse break time. */
#define  DTG_P_IDD     0x06      /* Send pulse interdigit delay. */
#define  DTG_PDDLFTIM  0x07      /* Flash time. */
#define  DTG_PDFLASH   0x08      /* Flash character. */
#define  DTG_CABTYPE   0x09      /* Cable type (LIU). */
#define  DTG_OOFMAX    0x0A      /* Out of frame errors, maximum (<=15). */
#define  DTG_FECSMAX   0x0B      /* Frame error count saturation. */
#define  DTG_FECRMAX   DTG_FECSMAX/* Max FECR error count (<=255) */
#define  DTG_ESECMAX   0x0B      /* Extended superframe Err. cnt max <=15. */
#define  DTG_BPVCMAX   0x0C      /* Bipolar violation count max <=255. */
#define  DTG_RDEBON    0x0D      /* Receive debounce on value, in 10ms units. */
#define  DTG_RDEBOFF   0x0E      /* Receive debounce off value,in 10ms units. */
#define  DTG_PCDEAD    0x0F      /* Signalling dead parameter. */
#define  DTG_RCVTOL    0x10      /* Low word of rcv timeout (& ack). */
#define  DTG_RCVTOH    0x11      /* Hi byte of 3 byte rcv & ack timeout. */
#define  DTG_CODESUPR  0x12      /* Suppression value (b8zs,bit7 or tran) */
#define  DTG_WINKMAX   0x13      /* Wink time, in 10ms units. */
#define  DTG_REDTIME   0x14      /* RLOS -> red alarm transition time. */
#define  DTG_RCOVRTM   0x15      /* Red alarm recovery time. */
#define  DTG_PDPSTIM   0x16      /* Pulse dialing pause time */
#define  DTG_WINKMIN   0x17      /* Wink time, in 10ms units. */
#define  DTG_WINKLEN   0x18      /* Wink length(xmt), in 10ms units. */
#define  DTG_PREWINK   0x19      /* Pre-wink delay in 10ms units. */
#define  DTG_CRCENABL  0x1A      /* CRC4 switch (ENABLE or DISABLE) */
#define  DTG_CECRMAX   0x1B      /* Max CRC4 error count <= 255. */
#define  DTG_ECRRSTTM  0x1C      /* Err cnt register reset time in 100ms unts */
#define  DTG_RXTXIDLE  0x1D      /* Receive & Trnsmit idle patterns */
#define  DTG_SEIZESIG  0x1E      /* Line Seizure and Response */
#define  DTG_DVPRMMAX  0x1E      /* Max value of device parameter */

#define  DTG_SETCLK    0x80      /* Set the clock source. */
#define  DTG_SETBDMD   0x81      /* Set the mode. */
#define  DTG_IDLTYP    0x82      /* Set the idle type. */
#define  DTG_DSERRC    0x83      /* Disable error condition mask */
#define  DTG_ENERRC    0x84      /* Enable the error condition mask. */
#define  DTG_SETBLOCKINGMASK 0x90	/* Set alarms as blocking */
#define  DTG_EVTMASK   0x91			/* Set the event mask */

/*
 *-------T1 Error conditions values.
 */
#define  DTT1_OOF			0x00    /* Out of frame error; count saturation. */
#define  DTT1_LOS			0x01    /* Initial loss of signal detection. */
#define  DTT1_DPM			0x02    /* Driver performance monitor. */
#define  DTT1_BPVS			0x03    /* Bipolar violation count saturation. */
#define  DTT1_ECS			0x04    /* Error count saturation. */
#define  DTT1_RYEL			0x05    /* Receive yellow alarm. */
#define  DTT1_RCL			0x06    /* Receive carrier loss. */
#define  DTT1_FERR			0x07    /* Frame bit error. */
#define  DTT1_B8ZSD			0x08    /* Bipolar eight zero substitution dtct. */
#define  DTT1_RBL			0x09    /* Receive blue alarm. */
#define  DTT1_RLOS			0x0A    /* Receive loss of sync. */
#define  DTT1_RED			0x0B    /* Got a red alarm condition. */
#define  DTT1_LOOPBACK_CFA  0x100	/* Diagnostic mode on the line trunk */
#define  DTT1_DCHAN_CFA		0x101   /* D channel out of service */

#define  DTT1_OOFOK    (DTT1_OOF + 0x10)  /* Out of frame restored. */
#define  DTT1_LOSOK    (DTT1_LOS + 0x10)  /* Signal restored. */
#define  DTT1_DPMOK    (DTT1_DPM + 0x10)  /* Driver perf. mon. restored. */
#define  DTT1_BPVSOK   (DTT1_BPVS + 0x10) /* Restored BPVS. */
#define  DTT1_ECSOK    (DTT1_ECS + 0x10)  /* Error count saturation restored. */
#define  DTT1_RYELOK   (DTT1_RYEL + 0x10) /* Receive yellow alarm restored. */
#define  DTT1_RCLOK    (DTT1_RCL + 0x10)  /* Receive carrier loss restored. */
#define  DTT1_FERROK   (DTT1_FERR + 0x10) /* Frame bit error restored. */
#define  DTT1_B8ZSDOK  (DTT1_B8ZSD + 0x10) /* B8ZS recovered */
#define  DTT1_RBLOK    (DTT1_RBL + 0x10)  /* Receive blue alarm restored. */
#define  DTT1_RLOSOK   (DTT1_RLOS + 0x10) /* Receive loss of sync restored. */
#define  DTT1_REDOK    (DTT1_RED + 0x10)  /* Red alarm condition recovered. */
#define  DTT1_LOOPBACK_CFAOK (DTT1_LOOPBACK_CFA + 0x10)  /* Diagnostic mode on the line trunk recovered */
#define  DTT1_DCHAN_CFAOK    (DTT1_DCHAN_CFA + 0x10)     /* D channel out of service recovered */

/*
 * E1 Error Conditions 
 */
#define DTE1_LOS			0x01   /* Received loss of signal */
#define DTE1_DPM			0x02   /* Driver performance monitor failure */
#define DTE1_BPVS			0x03   /* Bipolar violation count saturation. */
#define DTE1_ECS			0x04   /* Error count saturation */
#define DTE1_RRA			0x05   /* Received remote alarm */
#define DTE1_FSERR			0x07   /* Received frame sync error */
#define DTE1_RUA1			0x09   /* Received unframed all 1's */
#define DTE1_RLOS			0x0A   /* Received loss of sync */
#define DTE1_RED   			0x0B   /* Received a read alarm */
#define DTE1_MFSERR			0x0C   /* Received multi frame sync error */
#define DTE1_RSA1			0x0D   /* Received signalling all 1's */
#define DTE1_RDMA			0x0E   /* Received distant multi-frame alarm */
#define DTE1_CECS			0x0F   /* CRC4 error count saturation */
#define DTE1_LOOPBACK_CFA	0x100  /*Diagnostic mode on the line trunk */
#define DTE1_DCHAN_CFA		0x101  /* D channel out of service */
#define DTE1_CRC_CFA		0x102  /* Timeslot 16 CRC failure */

#define DTE1_LOSOK          0x11   /* Received loss of signal recovered */
#define DTE1_DPMOK          0x12   /* Driver perf. monitor failure recovered */
#define DTE1_BPVSOK         0x13   /* Bipolar violation cnt saturation recovered */
#define DTE1_ECSOK          0x14   /* Error count saturation recovered */
#define DTE1_RRAOK          0x15   /* Received remote alarm recovered */
#define DTE1_FSERROK        0x17   /* Received frame sync error recovered */
#define DTE1_RUA1OK         0x19   /* Received unframed all 1's recovered */
#define DTE1_RLOSOK         0x1A   /* Received loss of sync recovered */
#define DTE1_REDOK			0x1B   /* Received a read alarm recovered */
#define DTE1_MFSERROK       0x1C   /* Received multi frame sync error recovered */
#define DTE1_RSA1OK         0x1D   /* Received signalling all 1's recovered */
#define DTE1_RDMAOK         0x1E   /* Recved. dist. multi-frame alarm recovered */
#define DTE1_CECSOK         0x1F   /* CRC4 error count saturation recovered */
#define DTE1_LOOPBACK_CFAOK 0x110  /*Diagnostic mode on the line trunk recovered */
#define DTE1_DCHAN_CFAOK    0x111  /* D channel out of service recovered */
#define DTE1_CRC_CFAOK	    0x112  /* Timeslot 16 CRC failure recovered */

/*
 *-------Masks for enabling and disabling error conditions.
 */
#define  DTEC_OOF			0x8000   /* OOF condition mask */
#define  DTEC_LOS			0x0100   /* Loss of Signal. */
#define  DTEC_DPM			0x0200   /* Driver perf. mon. restored. */
#define  DTEC_BPVS			0x0080   /* Bipolar violation count saturation. */
#define  DTEC_BVCS			0x0080   /* Bipolar violation count saturation. */
#define  DTEC_ECS			0x0040   /* Error count saturation. */
#define  DTEC_RYEL			0x0020   /* Receive yellow alarm. */
#define  DTEC_RCL			0x0010   /* Receive carrier loss. */
#define  DTEC_RCLX			0x0010   /* Receive carrier loss. */
#define  DTEC_FERR			0x0008   /* Frame bit error. */
#define  DTEC_B8ZSD			0x0004   /* Bipolar eight zero subs detection. */
#define  DTEC_RBL			0x0002   /* Receive blue alarm. */
#define  DTEC_RLOS			0x0001   /* Receive loss of sync. */
#define  DTEC_RED			0x0400   /* Red alarm condition */
#define  DTEC_LOOPBACK_CFA	0x10000  /* Diagnostic mode on the line trunk */
#define  DTEC_DCHAN_CFA		0x20000  /* D channel out of service */

#define  DEEC_LOS			0x0100   /* Loss of signal notification */
#define  DEEC_DPM			0x0200   /* Driver performance monitor */
#define  DEEC_BPVS			0x0080   /* Bipolar violation count saturation. */
#define  DEEC_ECS			0x0040   /* Error count saturation notification */
#define  DEEC_RRA			0x0020   /* Receive remote alarm notification */
#define  DEEC_FSERR			0x0008   /* Receive frame sync error notification */
#define  DEEC_RUA1			0x0002   /* Receive unframed all 1's notification */
#define  DEEC_RLOS			0x0001   /* Receive loss of sync notification */
#define  DEEC_RED			0x0400   /* Received a read alarm */
#define  DEEC_MFSERR		0x0800   /* Multi-frame sync error notification */
#define  DEEC_RSA1			0x1000   /* Receive signalling all 1's notification */
#define  DEEC_RDMA			0x2000   /* Receive distant multi-frame alarm */
#define  DEEC_CECS			0x4000   /* CRC4 error count saturation */
#define  DEEC_LOOPBACK_CFA	0x10000  /* Diagnostic mode on the line trunk */
#define  DEEC_DCHAN_CFA		0x20000  /* D channel out of service */
#define  DEEC_CRC_CFA		0x40000  /* Timeslot 16 CRC failure recovered */

/*
 *-------Suppresion values.
 */
#define  DTSP_TRAN     0x00      /* Suppression is transparent. */
#define  DTSP_B8ZS     0x01      /* Suppression is B8ZS. */
#define  DTSP_BIT7     0x02      /* Suppression is Bit 7. */
#define  DTSP_MAX      0x02

/*
 *-------Board modes.
 */
#define  DTMD_NORMAL   0x00      /* Normal mode. */
#define  DTMD_XCVRLB   0x01      /* Tranceiver local loop back mode. */
#define  DTMD_LIULLB   0x02      /* LIU local loop back mode. */
#define  DTMD_LIURLB   0x03      /* LIU remote loop back mode. */
#define  DTMD_MAX      0x03

/*
 *--------What to do if the pc is dead, bit masks.
 */
#define  DTD_SNDIDLE   0x01      /* Send idle bit (0=NO, 1=YES). */
#define  DTD_IDLEVAL   0x02      /* Idle value (0=7F, 1=FF). */
#define  DTD_STXSIG    0x04      /* Set all xmt signaling (0=NO,1=YES). */
#define  DTD_SIGVAL    0x08      /* Set signaling to (0=0,1=1). */

/*
 *--------Cable line length values for DTI/1xx.
 */
#define  DTLL_2048MHZ  0x00      /* 2.048 Mhz. */
#define  DTLL_MI440    0x01      /* MAT & ICOT length 220-440 ft. */
#define  DTLL_MI655    0x02      /* MAT & ICOT length 440-655 ft. */
#define  DTLL_MIANDAP  0x03      /* M & I len. 0-220 & A & P 0-133 ft. */
#define  DTLL_AP266    0x04      /* ABAM & PIC length 133-266 ft. */
#define  DTLL_AP399    0x05      /* ABAM & PIC length 266-399 ft. */
#define  DTLL_AP533    0x06      /* ABAM & PIC length 399-533 ft. */
#define  DTLL_AP655    0x07      /* ABAM & PIC length 533-655 ft. */
#define  DTLL_MAX      0x08      /* Number of choices. */

/* 
 * DTI/211 cable line length values.
 */
#define  DTLL_G703     0x00      /* CCITT rec. G.703 (2.048MHz). */
#define  DTLL_FCC68    0x02      /* FCC part 68 option A (CSU). */
#define  DTLL_ANSIT1   0x03      /* ANSI T1.403 (CSU). */
#define  DTLL_133ABAM  0x03      /* 0-133 feet DSX-1 ABAM (default). */
#define  DTLL_266ABAM  0x04      /* 133-266 feet DSX-1 ABAM. */
#define  DTLL_399ABAM  0x05      /* 266-399 feet DSX-1 ABAM. */
#define  DTLL_533ABAM  0x06      /* 399-533 feet DSX-1 ABAM. */
#define  DTLL_655ABAM  0x07      /* 533-266 feet DSX-1 ABAM. */

/* 
 * SPAN-DTI cable line length values.
 */
#define  DTLL_000      0x00      /* 000-110 ft. */
#define  DTLL_110      0x01      /* 110-220 ft. */
#define  DTLL_220      0x03      /* 220-330 ft. */
#define  DTLL_330      0x02      /* 330-440 ft. */
#define  DTLL_440      0x06      /* 440-550 ft. */
#define  DTLL_550      0x07      /* 550-655 ft. */
#define  DTLL_655      0x05      /* > 655 ft. */
#define  DTLL_SQUARE   0x04      /* Square pulse */

/*
 *--------Clocking values.
 */
#define  DTC_NOCLK     0x00      /* No clocking. */
#define  DTC_LOOP      0x01      /* Loop timing. */
#define  DTC_EXT       0x02      /* Clock from the expansion bus. */
#define  DTC_IND       0x03      /* 1.544 Mhz - independant timing. */
#define  DTC_MAX       0x03      /* Maximum clock value. */

/*
 * Function ids for multitasking function dt_mtfcn()
 */
#define DEMT_WTCALL  0x02   /* Wait for line seizurer */
#define DEMT_ABORT   0x01   /* Abort the multitasking function */

/* Data Structures for Dm3 PSTN Statistics Information */

typedef enum
{
	dtStatisticsType_Invalid = 0, /* No statistics to be collected */
	dtStatisticsType_Layer1,	  /* All Layer 1 Statistics */
	dtStatisticsType_Max
}dtStatisticsType;

typedef enum
{
	dtStatisticsMode_Invalid = 0, /* No statistics Mode */
	dtStatisticsMode_Clear,		  /* Clear statistics counters */
	dtStatisticsMode_Preserve,    /* Preserve statistics counters */
	dtStatisticsMode_Max
}dtStatisticsMode;

typedef enum
{
	dtLayer1StatisticsId_Invalid = 0,
	dtLayer1StatisticsId_LCV = 1,	/* Line Coding Violations(LCV)           */
	dtLayer1StatisticsId_PCV,		/* Path Coding Violations(PCV)           */
	dtLayer1StatisticsId_ES,		/* Errored Seconds(ES)                   */
	dtLayer1StatisticsId_SES,		/* Severely Errored Seconds(SES)         */
	dtLayer1StatisticsId_UAS,		/* Unavailable Seconds(UAS)              */
	dtLayer1StatisticsId_BES,		/* Bursty Errored Seconds(BES)           */
	dtLayer1StatisticsId_LOFC,		/* Loss of Frame Count(LOFC)             */
	dtLayer1StatisticsId_CSS,		/* Controlled Slip Seconds(CSS)          */
	dtLayer1StatisticsId_SEFS,		/* Severly Errored Framing Seconds(SEFS) */
	dtLayer1StatisticsId_LES,		/* Line Errored Seconds(LES)             */
	dtLayer1StatisticsId_Max
}dtLayer1StatisticsId;

typedef struct SdtLayer1Statistics
{
	unsigned int m_nVersion; /* Version of this structure */
	dtLayer1StatisticsId	m_Layer1StatisticsId;
	unsigned int			m_nIntervalTotal;
	unsigned int			m_nCurrentIntervalTimer;
	unsigned int			m_nCurrentValue;
	unsigned int			m_nPreviousValue;
}TSdtLayer1Statistics;
#define TSdtLayer1Statistics_VER(v) ((sizeof(TSdtLayer1Statistics) << 16) | (0xFFFF & v))
#define TSdtLayer1Statistics_VERSION_0 TSdtLayer1Statistics_VER(0)

typedef struct SdtStatisticsList
{
	unsigned int m_nVersion; /* Version of this structure */
	dtStatisticsType  m_StatisticsType;   /* Statistics Type  */
	dtStatisticsMode  m_StatisticsMode;   /* Statistics Mode  */
	unsigned int m_nStatisticsCount;	  /* Statistics Count */
	union
	{
		TSdtLayer1Statistics m_Layer1Statistics[dtLayer1StatisticsId_Max];
	}m_Stats;
} TSdtStatisticsList;
#define TSdtStatisticsList_VER(v) ((sizeof(TSdtStatisticsList) << 16) | (0xFFFF & v))
#define TSdtStatisticsList_VERSION_0 TSdtStatisticsList_VER(0)

/*
 * Type definition for SCBus Timeslots information structure.
 */
#ifndef __SC_TSINFO__
#define __SC_TSINFO__
typedef struct sc_tsinfo {
   unsigned long  sc_numts;
   long          *sc_tsarrayp;
} SC_TSINFO;
#endif

/* Defines for dt_setiostatus() device states */
#ifndef DEVICE_IN_SERVICE
#define DEVICE_IN_SERVICE 0
#endif
#ifndef DEVICE_OUT_SERVICE
#define DEVICE_OUT_SERVICE 1
#endif

/*
 * Flag mask for dt_open() oflags argument.
 */
#define DTOPEN_LCON_DISABLE 0x01

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

// 08/06/99 VJ Added for CAS DTI functions
/*
 * Includes for CAS DTI template definitions.
 */
#include "dticas.h"

/*
 * Prototypes for all library functions.
 */
#ifdef __cplusplus
extern "C" {   // C++ func bindings to enable C funcs to be called from C++
#define extern
#endif

#if (defined(__STDC__) || defined(__cplusplus) || defined(__BORLANDC__))
DllLinkage  char* __cdecl ATDT_BDSGBIT(int);
DllLinkage  long  __cdecl ATDT_BDMODE(int),
				  ATDT_DNLDVER(int),
				  ATDT_IDLEST(int),
				  ATDT_ROMVER(int),
				  ATDT_STATUS(int),
				  ATDT_TSMODE(int),
				  ATDT_TSSGBIT(int),
				  ATDT_FWTYPE(int),
				  ATDT_BRDTYPE(int),
				  ATDT_STATE(int);
DllLinkage  int   __cdecl dt_castmgmt(int, void *, void *),	   /* Added by VJ 08/06/99 */
				  __cdecl dt_close(int),
				  dt_dial(int, char *, unsigned int),
				  dt_getctinfo(int, CT_DEVINFO *),
				  dt_getevtmsk(int, int, unsigned short *),
				  dt_getparm(int, unsigned long, void *),
				  dt_getxmitslot(int, SC_TSINFO *),
				  dt_listen(int, SC_TSINFO *),
				  dt_mtfcn(int, unsigned char, unsigned int),
				  dt_open(char *, int),
				  dt_rundiag(int, unsigned int, char *),
				  dt_setalrm(int, unsigned int),
				  dt_setevtmsk(int, int, unsigned short, int),
				  dt_setidle(int, unsigned int),
				  dt_setparm(int, unsigned long, void *),
				  dt_setsigmod(int, unsigned int),
				  dt_settssig(int, unsigned short, int),
				  dt_settssigsim(int, unsigned short),
				  dt_tstcom(int, unsigned int),
				  dt_tstdat(int, unsigned int),
				  dt_unlisten(int),
				  dt_xmitalrm(int, unsigned char, unsigned int),
				  dt_xmittone(int, unsigned int),
				  dt_xmitwink(int, unsigned int),
				  dt_route(int, int, int, int),
				  dt_getevt(int, EV_EBLK *, int),
				  dt_setiostatus(int, int),
				  dt_getstatistics(int hSrlDevice, TSdtStatisticsList* statisticsList, unsigned int mode);
DllLinkage  void  __cdecl dt_set_lasterr(int, int);
	  
/* Clib interface functions */
DllLinkage  long  __cdecl dt_libinit(unsigned short),
				  dt_GetDllVersion(LPDWORD, LPDWORD);

#else
DllLinkage  char* __cdecl ATDT_BDSGBIT();
DllLinkage  long  __cdecl ATDT_BDMODE(),
				  ATDT_DNLDVER(),
				  ATDT_IDLEST(),
				  ATDT_ROMVER(),
				  ATDT_STATUS(),
				  ATDT_TSMODE(),
				  ATDT_TSSGBIT(),
				  ATDT_FWTYPE(int),
				  ATDT_BRDTYPE(int),
				  ATDT_STATE(int);
DllLinkage  int   __cdecl dt_castmgmt(),		 /* Added by VJ 08/06/99 */
				  dt_close(),
				  dt_dial(),
				  dt_getctinfo(),
				  dt_getevtmsk(),
				  dt_getparm(),
				  dt_getxmitslot(),
				  dt_listen(),
				  dt_mtfcn(),
				  dt_open(),
				  dt_rundiag(),
				  dt_setalrm(),
				  dt_setevtmsk(),
				  dt_setidle(),
				  dt_setparm(),
				  dt_setsigmod(),
				  dt_settssig(),
				  dt_settssigsim(),
				  dt_tstcom(),
				  dt_tstdat(),
				  dt_unlisten(),
				  dt_xmitalrm(),
				  dt_xmittone(),
				  dt_xmitwink(),
				  dt_route(),
				  dt_getevt(),
				  dt_setiostatus(),
				  dt_getstatistics();
DllLinkage  void  __cdecl dt_set_lasterr();
/* Clib interface functions */
DllLinkage  long  __cdecl dt_libinit(),
				  dt_GetDllVersion();

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
