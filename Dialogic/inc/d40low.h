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
 *        FILE: d40low.h
 * DESCRIPTION: 
 *        DATE: 
 *          BY: 
 *
 **********************************************************************/
//01-27-2000 [cjl] add IBI CST event and Merge DOS change.
//03-08-2000 [cjl] add IBI VAD event IEN mask.
//12-01-2000 [cjl] add vad parms on the fly and rearm commands

#ifndef __D40LOW_H__
#define __D40LOW_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif


/*
 * Misc. values
 */
#define  NULLSPTR      0x00FFFF  /* null silence pointer */

/*
 * reset complete values
 */
#define  DLR_SUCC      0x000     /* board was successfully reset */
#define  DLR_MEMFAIL   0x001     /* board failed memory test */
#define  DLR_REGFAIL   0x002     /* board failed register test */
#define  DLR_FLAGFAIL  0x003     /* board failed flags test */
#define  DLR_CHKSUM    0x004     /* board failed checksum test */
#define  DLR_PRODID    0x005     /* invalid product ID code */
#define  DLR_CHNUM     0x006     /* invalid number of channels */

/*
 * Equates for Dialog/4x to PC command codes.
 * The following messages are enabled by the set_IEN command.
 */
#define  DL_B0FULL     0x001     /* buffer 0 full */
#define  DL_B1FULL     0x002     /* buffer 1 full */
#define  DL_B0EMPTY    0x003     /* buffer 0 empty */
#define  DL_B1EMPTY    0x004     /* buffer 1 empty */

/*
 * Equates for Dialog/4x to PC command codes.
 * The following message is enabled by the 'old' param. of the PC_INITBUF cmd.
 */
#define  DL_DIGITRDY   0x000     /* digit collected */

/*
 * Equates for Dialog/4x to PC command codes.
 * The following messages are always enabled.
 */
#define  DL_RECORD     0x008     /* record complete */
#define  DL_RECCMPLT   DL_RECORD /* duplicate name for driver */
#define  DL_PLAYBACK   0x007     /* playback complete */
#define  DL_PLAYCMPLT  DL_PLAYBACK /* duplicate name for driver */
#define  DL_DIAL       0x005     /* dial complete */
#define  DL_BEEP       0x00A     /* beep complete */

#define  DL_COMTEST    0x00B     /* communication test */
#define  DL_ACK        0x00C     /* communication test acknowledgement */

#define  DL_CST        0x006     /* call status transition */

#define  DL_ERROR      0x009     /* PC to DIALOG/4X command error */

#define  DL_MTST       0x00D     /* memory test complete */

/*
 * New equates for Dialog/4x to PC command codes supported by firmware ver.60.
 * The following messages are always enabled.
 */
#define  DL_RPTBUF     0x15      /* digit buffer count */
#define  DL_GETDIGITS  0x10      /* get digits complete */
#define  DL_UPDPARM    0x16      /* update of parameters complete */
#define  DL_GETPARM    0x13      /* current parameters */
#define  DL_WINK       0x17      /* wink complete */
#define  DL_TIMER      0x11      /* timer reached zero */
#define  DL_WATCHDOG1  0x12      /* 1st watchdog timeout */
#define  DL_WATCHDOG2  0x14      /* 2nd watchdog timeout */
#define  DL_PRRECORD   0x1C      /* prompted record */
#define  DL_PR_REC_CMPLT DL_PRRECORD  /* prompted record */
#define  DL_PRSTATUS   0x1D      /* prompted record */
#define  DL_PR_REC_STATUS DL_PRSTATUS  /* prompted record */

#define  MAXDLCMD      0x1D      /* maximum number of DIALOG/4X commands */

/*
 * Special equates for new Dialog/4x to PC command codes to replace certain
 * CST event flavors.
 */
#define  DL_OFFHOOKC   0x30      /* offhook complete */
#define  DL_ONHOOKC    0x31      /* onhook complete */
#define  DL_CALL       0x32      /* call analysis complete */

#define  DL_GETR2MF    0x1B
/*
 * Special equates for simulated Dialog/4x to PC command codes to support
 * driver handling of play and record.
 */
#define  DL_BUFEMPTY   0x40      /* virtual buffer empty */
#define  DL_BUFFULL    0x41      /* virtual buffer full */

/*
 * Equates for PC to Dialog/4x command codes.
 * The following commands do not cause the board to send a DL reply msg.
 */
#define  PC_SETIEN     0x007     /* set the interrupt enable mask */
#define  PC_SETRINGS   0x005     /* set number of rings for cst event */
#define  PC_PULSE      0x00B     /* enable pulse digit collection */

/*
 * Equates for PC to Dialog/4x command codes.
 * The following commands cause the board to send a DL reply msg.
 */
#define  PC_RECORD     0x001     /* place channel in record mode */
#define  PC_PLAYBACK   0x002     /* place channel in playback mode */
#define  PC_DIAL       0x006     /* dial ASCIIZ string in buf0 */
#define  PC_BEEP       0x008     /* start beeping */

#define  PC_COMTEST    0x009     /* communication test */
#define  PC_ACK        0x00A     /* communication test response */

#define  PC_OFFHOOK    0x004     /* place channel offhook */
#define  PC_ONHOOK     0x003     /* place channel onhook */

#define  PC_STOP       0x000     /* stop operation in progress */

/*
 * New equates for PC to Dialog/4x command codes supported by firmware ver.60.
 * The following commands do not cause the board to send a DL reply msg.
 */
#define  PC_SETTERMS   0x14      /* set terminating conditions */
#define  PC_CLRHIST    0x19      /* clear termination history */
#define  PC_INITBUF    0x10      /* initialize digit buffer */
#define  PC_CLRBUF     0x11      /* clear digit buffer */

/*
 * New equates for PC to Dialog/4x command codes supported by firmware ver.60.
 * The following commands cause the board to send a DL reply msg.
 */
#define  PC_RPTBUF     0x12      /* report digit buffer count */
#define  PC_GETDIGITS  0x13      /* get digits */
#define  PC_UPDPARM    0x17      /* update parameters */
#define  PC_GETPARM    0x18      /* get current parameters */
#define  PC_WINK       0x1a      /* initiate a wink command */
#define  PC_TIMER      0x15      /* start timer */
#define  PC_WATCHDOG   0x16      /* start watchdog */
#define  PC_PRRECORD   0x1C      /* prompted record */
#define	PC_SETVAD	  0x1F		/* set vad parm */
#define  PC_GETVAD	  0x20		/* Get VAD parm */
#define	PC_REARM		  0x21		/* rearm the vad */
//#define  MAXPCCMD      0x1C      /* maximum number of PC commands */

/*
 * Equates for dx_generic()
 */
#define SH_UI_GENERIC_READ	0x0
#define SH_UI_GENERIC_WRITE	0x1

#define SH_UI_CMD	0x7  /* universal fw cmd */

#define PC_RW_QSLAC	0XDE /* set/get QSLAC Coefficients */
#define DL_RW_QSLAC PC_RW_QSLAC /* set/get QSLAC Coefficients reply */

/*
 * Equates for PC to Dialog/4x at boot up time.
 */
#define  REQ_RESET     0x00FF    /* clear, reset and go */

#define  REQ_MTST      0x00FE    /* do the memory test and stop */

/*
 * Call status transition data equates.
 * The following CST's are enabled by the set_IEN command.
 */
#define  CST_OFFHOOK    0x001    /* offhook transition complete */
#define  CST_HOOK_DONE  CST_OFFHOOK /* duplicate name for driver */
#define  CST_ONHOOK     0x006    /* onhook transition complete */
#define  CST_ONH_DONE   CST_ONHOOK /* duplicate name for driver */
#define  CST_RINGS      0x002    /* rings received */
#define  CST_WINKRCVD   0x00E    /* wink received */
#define  CST_RNGOFF     0x00F    /* ring off messages */

#define  CST_LCOFF      0x003    /* loop current off */
#define  CST_LCON       0x00D    /* loop current on */
#define  CST_LCREV      0x020    /* loop current reversal */
#define  CST_SILENCEOFF 0x004    /* silence off */
#define  CST_SILENCEON  0x005    /* silence on */
#define  CST_VAD			0x013		/* voice activity detected */

/*
 * Call status transition data equates.
 * The following CST's are always enabled.
 */
#define CST_BUSY        0x007    /* line busy  (call analysis) */
#define CST_NOANS       0x008    /* no answer (call analysis) */
#define CST_NORB        0x009    /* no ringback (call analysis) */
#define CST_CNCT        0x00A    /* call connected (call analysis) */
#define CST_CEPT        0x00B    /* operator intercept (call analysis) */
#define CST_STOPD       0x00C    /* call analysis stopped (call analysis) */
#define CST_NO_DIALTONE 0x011
#define CST_FAXTONE     0x012
#define CST_DISCONNECT  0x022    /* D/42-NS Disconnect Message (call analysis) */

/*
 * Interrupt enable(IEN) bits, if bit is 1(on) then int is enabled, 0 is
 * disabled for set_ien message.
 * The following bits enable DL reply messages.
 */
#define  IEN_BUFEMPTY   0x00001  /* enable buffer empty messages */
#define  IEN_BUFFULL    0x00002  /* enable buffer full messages */

/*
 * Interrupt enable(IEN) bits, if bit is 1(on) then int is enabled, 0 is
 * disabled for set_ien message.
 * The following bits enable flavors of the DL_CST message.
 */
#define  IEN_OFFHOOK    0x00020  /* enable offhook complete messages */
#define  IEN_ONHOOK     0x00100  /* enable onhook complete messages */
#define  IEN_RINGS      0x00010  /* enable rings received messages */
#define  IEN_WINKRCVD   0x00800  /* enable wink received messages */
#define  IEN_RNGOFF     0x01000  /* enable ring off messages */

#define  IEN_LCOFF      0x00008  /* enable loop current drop messages */
#define  IEN_LCON       0x00400  /* enable loop current on messages */
#define  IEN_SILENCEOFF 0x00080  /* enable silence off messages */
#define  IEN_SILENCEON  0x00040  /* enable silence on messages */
#define  IEN_LCREVON    0x02000  /* enable loop current reversal messages */
#define  IEN_LCREVOFF   0x04000  /* enable loop current reversal messages */
#define  IEN_LCREV      (IEN_LCREVON | IEN_LCREVOFF)
#define	IEN_VADEVTS		0x00200	/* enable VAD Event messages */

/*
 * Interrupt enable(IEN) bits, if bit is 1(on) then int is enabled, 0 is
 * disabled for set_ien message.
 * The following bit has no effect on DL reply messages.
 */
#define  IEN_DTMF       0x00004  /* enable DTMF digit collection */

/*
 * pdd reception type - data value for PC_PULSE message
 */
#define  PDD_OFF       0x000     /* turn pulse digit detect off */
#define  PDD_ON        0x001     /* turn pulse digit detect on */


/*
 * Set digit type in the DL_DIGITRDY (digit ready) message.
 */
#define DG_DTMF         0x0000   /* DTMF Digit */
#define DG_LPD          0x0001   /* Loop Pulse Digit  */
#define DG_APD          0x0002   /* Audio Pulse Digit */
#define DG_DPD          0x0002   /* Dial Pulse Digit */
#define DG_MF           0x0003   /* MF Digit */

#define DM_DTMF         (1 << DG_DTMF)
#define DM_LPD          (1 << DG_LPD)
#define DM_APD          (1 << DG_APD)
#define DM_MF           (1 << DG_MF)
#define DM_DPD          0x10      /* Enable DPD detection */
#define DM_DPDZ         0x20      /* Enable Zero-Trained DPD Detection */


/*
 * Record complete reason codes
 */
#define  RCR_STOP      0x002     /* record stopped */
#define  RCR_DTMF      0x003     /* record terminated by DTMF */

/*
 * Play complete reason codes
 */
#define  PCR_MAXBYTE   0x001     /* playback reached maximum bytes */
#define  PCR_STOP      0x002     /* playbacked stopped */
#define  PCR_DTMF      0x003     /* playback terminated by DTMF */

/*
 * Beep complete reason codes
 */
#define  BCR_MAXCNT    0x001     /* beep reached maximum repetions */
#define  BCR_STOP      0x002     /* beep stopped */
#define  BCR_DTMF      0x003     /* beep terminated by DTMF */

/*
 * Record type codes - this field is bitmapped
 */
#define  RT_SIMARK     0x001     /* mark silence start/stop events */
#define  RT_TTINIT     0x002     /* initiate record with DTMF */
#define  RT_TTTERM     0x004     /* terminate record with DTMF */
#define  RT_TDTONE     0x008     /* play tone before record initiation */
#define  RT_INITSET    0x110     /* touch tone initiation set */

/*
 * Playback type codes - this field is bitmapped
 */
#define  PT_TTINIT     0x002     /* initiate playback with DTMF */
#define  PT_TTTERM     0x004     /* terminate playback with DTMF */
#define  PT_TDTONE     0x008     /* play tone before playback initiation */
#define  PT_INITSET    0x110     /* touch tone initiation set */

/*
 * Beep type codes - this field is bitmapped
 */
#define  BT_TTINIT     0x002     /* initiate beep with DTMF */
#define  BT_TTTERM     0x004     /* terminate beep with DTMF */

/*
 * DTMF flags (see dl_dflags) - the edge on which to send DTMF ready message
 */
#define  DF_MKBK1      0x001     /* for channel 1 */
#define  DF_MKBK2      0x002     /* for channel 2 */
#define  DF_MKBK3      0x004     /* for channel 3 */
#define  DF_MKBK4      0x008     /* for channel 4 */

/*
 * Flag for operator intercept part of call progress
 */
#define  OPTEN         0x001     /* enable operator intercept w/connect */
#define  OPTDIS        0x002     /* disable operator intercept */
#define  OPTCON        0x003     /* enable operator intercept w/o connect */

#define  H_REDGE       0x001     /* rising edge detection of "hello" */
#define  H_FEDGE       0x002     /* falling edge detection of "hello" */

#define  ANSRDIS       0x00FFFF  /* disable answer deglitching */

#define  DLPRODA       00        /* an A series D/4x */
#define  DLPRODB       11        /* a B series D/4x */

/*
 * Success or failure indicators of update parameters function
 */
#define  UPD_OK        00        /* parameters updated successfully */
#define  UPD_FAIL      11        /* parameters not updated successfully */

/*
 * ROM type definition bit masked
 */
#define  ROM_CCITT     11        /* CCITT dial table used for DTMF dialing */


/*
 * This SRCAP is passed as an offset to buffer 1
 */
#define  SRCAP_SIZE  0x200
struct SRCAP {
   unsigned char ca_nbrdna;      /* # of rings before no answer */
   unsigned short int ca_stdely;  /* Delay after dialing before analysis */
   unsigned short int ca_cnosig;  /* Duration of no signal time out delay */
   unsigned short int ca_lcdly;  /* Delay after dial before lcdrop connect */
   unsigned short int ca_lcdly1;  /* Delay after lc drop con. before msg. */
   unsigned char ca_hedge;       /* Edge of answer to send connect message */
   unsigned short int ca_cnosil;  /* Initial continuous noise timeout delay */
   unsigned char ca_lo1tola;     /* acceptable tolerance above nominal 1st */
   unsigned char ca_lo1tolb;     /* acceptable tolerance below nominal 1st  */
   unsigned char ca_lo2tola;     /* acceptable tolerance above nominal 2nd */
   unsigned char ca_lo2tolb;     /* acceptable tolerance below nominal 2nd  */
   unsigned char ca_hi1tola;     /* acceptable tolerance above nominal 1st */
   unsigned char ca_hi1tolb;     /* acceptable tolerance below nominal 1st */
   unsigned short int ca_lo1bmax;  /* maximum interval of 1st low for busy */
   unsigned short int ca_lo2bmax;  /* maximum interval of 2nd low for busy */
   unsigned short int ca_hi1bmax;  /* maximum interval of 1st high for busy */
   unsigned char ca_nsbusy;      /* Num. of highs after nbrdna busy check */
   unsigned short int ca_logltch;  /* Silence deglitch duration */
   unsigned short int ca_higltch;  /* Nonsilence deglitch duration */
   unsigned short int ca_lo1rmax;  /* Max. short low  dur. of double ring */
   unsigned short int ca_lo2rmin;  /* Min. inter ring delay */
   unsigned char ca_intflg;      /* Operator intercept mode */
   unsigned char ca_intfltr;     /* the time required to qualify frequency */

   unsigned short int ca_frqmin;  /* lower limit for frequency detction */
   unsigned short int ca_frqmax;  /* upper limit for frequency detection */
   unsigned short int ca_devmax;  /* upper limit for frequency deviation */
   unsigned short int ca_smpsiz;  /* number of samples used in freq analysis */

   unsigned short int ca_hisiz;  /* Used to determine which lowmax to use */
   unsigned short int ca_alowmax;  /* Max. low before con. if high >hisize */
   unsigned short int ca_blowmax;  /* Max. low before con. if high <hisize */

   unsigned char ca_nbrbeg;      /* Number of rings before analysis begins */
   unsigned short int ca_hi1ceil;  /* Maximum 2nd high dur. for a retrain */
   unsigned short int ca_lo1ceil;  /* Maximum 1st low dur. for a retrain */

   unsigned short int ca_lowerfrq;  /* lower boundary, units in hz */
   unsigned short int ca_upperfrq;  /* upper boundary, units in hz */
   unsigned char ca_timefrq;     /* length of signal required, .01 sec's */
   unsigned char ca_rejctfrq;    /* % of frequency allowed out of bounds */

   unsigned short int ca_maxansr;  /* maximum allowable duration of ansr */
   unsigned short int ca_ansrdgl;  /* used for deglitching end of ansr */

   unsigned char srcap_rfu[SRCAP_SIZE - 0x1C9];
   unsigned char srcap_end;      /* end of srcap block */
   };


/*
 * Call analysis parameters returned in buffer 1
 */
#define  SRCAR_SIZE  0x20        /* the size of the returned call prog. */
struct SRCAR {
   unsigned char cr_termtype;    /* last call progress term. for this ch. */
   unsigned char cr_frqout;      /* percent of frequency out of bounds */
   unsigned short int cr_frqherz;  /* frequency detected in hertz */
   unsigned short int cr_sizehigh;  /* duration of nonsilence in 10ms units */
   unsigned short int cr_shortlow;  /* duration of shorter silence  */
   unsigned short int cr_longlow;  /* duration of longer silence  */
   unsigned short int cr_ansrsize;  /* duration of answer in 10ms units */
#if   RAMFLAG
   unsigned char srcar_rfu[SRCAR_SIZE+0x8000-0x0C-1]; /* reserved for fut use */
#else
   unsigned char srcar_rfu[SRCAR_SIZE-0x0C-1]; /* reserved for fut use */
#endif
   unsigned char srcar_end;       /* end of srcar block */
   };


/*
 * structure for shared ram control ports
 */
#if   RAMFLAG
   #define  BUFBASE    0x8200       /* beginning of channel buffer space */
#else
   #define  BUFBASE    0x200        /* beginning of channel buffer space */
#endif


typedef    struct     {
#if   RAMFLAG
   unsigned char srcp_fil1[0x80f0]; /* since we can't use an org in a struc */
#else
   unsigned char srcp_fil2[0x0f0];  /* since we can't use an org in a struc */
#endif
   unsigned char dl_msgrdy;      /*pc interrupts dl, pc reads to clear int */
   unsigned char pc_msgrdy;      /*dl interrupts pc, dl reads to clear int */
   unsigned char srcp_fil3[0x0C];  /* filler */
   unsigned char clear_reset;    /* pc to dl start running command */
   unsigned char set_reset;      /* stop the DL4X */
   unsigned char dl_req;         /* dl request issued flag - indicates  */
                                 /* which board caused last interrupt,  */
                                 /* set to channel number */
   unsigned char pc_req;         /* pc request issued flag - set to channel */
                                 /* number */
   unsigned char dl_flash;       /* flash char in dialing string */
   unsigned short int dl_ftime;  /* flash time in .01 sec increments */
   unsigned short int dl_ptime;  /* pause time in .01 sec increments */
   unsigned short int dl_drate;  /* digitizing rate in hz */
   unsigned short int dl_rfu1;   /* reserved */
   unsigned short int dl_pmk;    /* length of pulse dialing make */
   unsigned char dl_bdtype;      /* board type. */
   unsigned char dl_rfu[4];      /* reserved for future use */
   unsigned short int dl_bufsize;  /* 512 for now */
   unsigned char dl_num_ch;      /* 4 for now */
   unsigned short int dl_seglist[4];  /*  */
   unsigned char dl_testresult;  /*  */
   unsigned char dl_version[4];  /* version level of the microcode */
   unsigned short int dl_pbrk;   /* length of pulse dialing break */
   unsigned short int dl_pdelay;  /* length of pulse inter digit delay  */
   unsigned short int dl_ttime;  /* length of TT inter digit delay */
   unsigned short int dl_offhdly;  /* delay before offhook. */
   unsigned short int dl_minron;  /* max # of ticks for 1 ring on cycle  */
   unsigned short int dl_minroff;  /* max # of ticks for 1 ring off cycle */
   unsigned short int dl_maxrwait;  /* max wait between rings, before ring  */
                                    /* count is */
   unsigned short int dl_sbounce;  /* silence debounce value in .01 sec  */
                                   /* increments */
   unsigned char dl_rfu3[16];    /* reserved */
   unsigned char dl_initdly;     /* delay before TT initiated I/O begins */

   unsigned char dl_rfu2[112];   /* Reserved. */

   unsigned char dl_dbtm1;       /* DTMF debounce time ch 1 */
   unsigned char dl_dbtm2;       /* DTMF debounce time ch 2 */
   unsigned char dl_dbtm3;       /* DTMF debounce time ch 3 */
   unsigned char dl_dbtm4;       /* DTMF debounce time ch 4 */
   unsigned char dl_dflags;      /* used to set DTMF mk\brk message */
   unsigned short int dl_dftm1;  /* DTMF filter time ch 1 */
   unsigned short int dl_dftm2;  /* DTMF filter time ch 1 */
   unsigned short int dl_dftm3;  /* DTMF filter time ch 1 */
   unsigned short int dl_dftm4;  /* DTMF filter time ch 1 */
   unsigned char dl_redge;       /* the rings received msg */
/*
 * The following debounces from silence off to silence on transition
 */
   unsigned short int dl_srecdb1;  /* silence record debounce interval ch 1 */
   unsigned short int dl_srecdb2;  /* silence record debounce interval ch 2  */
   unsigned short int dl_srecdb3;  /* silence record debounce interval ch 3 */
   unsigned short int dl_srecdb4;  /* silence record debounce interval ch 4 */

   unsigned short int dl_dtinit1;  /* DTMF initiator set for channel 1 */
   unsigned short int dl_dtinit2;  /* DTMF initiator set for channel 2 */
   unsigned short int dl_dtinit3;  /* DTMF initiator set for channel 3 */
   unsigned short int dl_dtinit4;  /* DTMF initiator set for channel 4 */

   unsigned char dl_minpdon;     /* min time for lc on to be a valid pulse */
   unsigned char dl_minpdoff;    /* min time for lc off to be a valid pulse */
   unsigned short int dl_minipd;  /* min interpulse digit time for lc on  */
   unsigned short int dl_minlcoff;  /* min time of lc off for CST_CUR_ON  */

   unsigned char dl_ttdata;      /* duration of DTMF digit in .01 sec */
   unsigned short int dl_maxpdoff;  /* max time for pd make */

   unsigned char dl_minslon;     /* Xmin time for sl on to be a valid pulse */
   unsigned char dl_minsloff;    /* Xmin time for sl off to be a valid pulse */
   unsigned short int dl_minisl;  /* Xmin interpulse digit time for sl on  */
   unsigned short int dl_maxsloff;  /* Xmax time for sl make */

   unsigned short int dl_mtstcntl;  /* low order part of memcnt for diag1 */
   unsigned short int dl_mtstcnth;  /* high order part of memcnt for diag1 */

   unsigned char dl_syscfg;      /* system configuration for attachments */
   unsigned char dl_product;     /* major board type */

   unsigned char dl_mintion;     /* min time required for xti signal */
   unsigned char dl_mintioff;    /* min time required for xti signal off */
   unsigned char dl_rfu4;        /* reserved */
   unsigned char dl_rfu5;        /* reserved */
   unsigned short int dl_devarea; /* device area buffer */
   unsigned short int dl_onoff;   /* delay after going on-hook, before can go off-hook */
   unsigned char srcp_rfu[BUFBASE+(0x1EA+4)];  /* reserved - must be set to zero */
   unsigned char srcp_end;       /* end of srcp block */
   }SRCP;


#endif

