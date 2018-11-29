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
 * Filename:    dxcallp.h
 * DESCRIPTION: Voice Library header file. Include defines, events, 
 *              structures, and function prototypes.   
 ****************************************************************************/


#ifndef __DXCALLP_H__
#define __DXCALLP_H__

#ifdef __STANDALONE__            /* List of required header files */
#endif


#define DX_CALLP              1   /* Turn on Call Analysis */

/*
 * Defines for Call Analysis modes (ca_intflg field)
 */
#define DX_OPTEN              1   /* Enable Operator Intercept with Connect */
#define DX_OPTDIS             2   /* Disable Operator Intercept */
#define DX_OPTNOCON           3   /* Enable Operator Intercept w/o Connect */
#define DX_PVDENABLE          4   /* Enable PVD */
#define DX_PVDOPTEN           5   /* Enable PVD with OPTEN */
#define DX_PVDOPTNOCON        6   /* Enable PVD with OPTNOCON */
#define DX_PAMDENABLE         7   /* Enable PAMD */
#define DX_PAMDOPTEN          8   /* Enable PAMD with OPTEN */

/*
 * Call Analysis termination type.
 */
#define CR_BUSY               7        /* Line busy */
#define CR_NOANS              8        /* No answer */
#define CR_NORB               9        /* No ringback */
#define CR_CNCT               10       /* Call connected */
#define CR_CEPT               11       /* Operator intercept */
#define CR_STOPD              12       /* Call analysis stopped */
#define CR_NODIALTONE         17       /* No dialtone detected */
#define CR_FAXTONE            18       /* Fax tone detected */
#define CR_ERROR              0x100    /* Call analysis error */

/*
 * Tone ID types
 */
#define TID_FIRST             250
#define TID_DIAL_LCL          250
#define TID_DIAL_INTL         251
#define TID_DIAL_XTRA         252
#define TID_BUSY1             253
#define TID_RNGBK1            254
#define TID_BUSY2             255
#define TID_RNGBK2            256

#define TID_DISCONNECT	      257
#define TID_FAX1              258
#define TID_FAX2              259
#define TID_LAST              259  /* last in springware */

#define	TID_SIT_NC	      260
#define TID_SIT_NO_CIRCUIT (TID_SIT_NC)
#define	TID_SIT_IC	      261
#define TID_SIT_OPERATOR_INTERCEPT (TID_SIT_IC)
#define	TID_SIT_VC	      262
#define TID_SIT_VACANT_CIRCUIT (TID_SIT_VC)
#define	TID_SIT_RO	      263
#define TID_SIT_REORDER_TONE (TID_SIT_RO)
#define	TID_SIT_ANY	      264

#define	TID_SIT_NC_INTERLATA  265
#define TID_SIT_NO_CIRCUIT_INTERLATA (TID_SIT_NC_INTERLATA)
#define	TID_SIT_RO_INTERLATA  266
#define TID_SIT_REORDER_TONE_INTERLATA (TID_SIT_RO_INTERLATA)
#define	TID_SIT_IO	      267
#define TID_SIT_INEFFECTIVE_OTHER (TID_SIT_IO)

#define	TID_CUSTOM_SIT1	      268
#define	TID_CUSTOM_SIT2	      269
#define	TID_CUSTOM_SIT3	      270
#define	TID_CUSTOM_SIT4	      271
#define	TID_CUSTOM_SIT5	      272
#define	TID_CUSTOM_SIT6	      273
#define	TID_CUSTOM_SIT7	      274
#define	TID_CUSTOM_SIT8	      275
#define	TID_CUSTOM_SIT9	      276
#define	TID_CUSTOM_SIT10	  277
#define	TID_CUSTOM_SIT11	  278
#define	TID_CUSTOM_SIT12	  279
#define	TID_CUSTOM_SIT13	  280
#define	TID_CUSTOM_SIT14	  281
#define	TID_CUSTOM_SIT15	  282


/*
 * Call Analysis Errors as returned by ATDX_CPERROR()
 */
#define CR_MEMERR             1  /* Out of Memory when Creating Temporary */
                                 /*  SIT Tone Templates */
#define CR_TMOUTON            2  /* Timeout Waiting for SIT Tone */
#define CR_TMOUTOFF           3  /* SIT Tone too long */
#define CR_UNEXPTN            4  /* Unexpected SIT Tone */
#define CR_MXFRQERR           5  /* Invalid ca_maxtimefrq field in DX_CAP */
#define CR_UPFRQERR           6  /* Invalid Upper Frequency Selection */
#define CR_LGTUERR            7  /* Lower Freq. Greater than Upper Freq. */
#define CR_OVRLPERR           8  /* Overlap in Selected SIT Tones */

/*
 * Connection types ( ATDX_CONNTYPE() )
 */
#define CON_CAD               1  /* Cadence Break */
#define CON_LPC               2  /* Loop Current Drop */
#define CON_PVD               3  /* Positive Voice Detect */
#define CON_PAMD              4  /* Positive Answering Machine Detect */
#define CON_DIGITAL				5	/* connect to pbx */

/*
 * PAMD equates
 */
#define PAMD_FULL             1           /* FULL decision on PAMD */
#define PAMD_QUICK            2           /* QUICK decision on PAMD */
#define PAMD_ACCU             3           /* ACCU decision on PAMD */
#define PAMD_ACCURACY         PAMD_ACCU   /* Accuracy decision on PAMD */

#define PAMD_QUAL1TMP         11 /* Qualification template 1 for PAMD */
#define PAMD_QUAL2TMP         12 /* Qualification template 1 for PAMD */

/*
 * Ensure all structures are packed.
 */
#pragma pack(1)


/*
 * DX_CAP
 *
 * Call Analysis parameters
 * [NOTE: All user-accessible structures must be defined so as to be
 *        unaffected by structure packing.]
 */
typedef struct dx_cap {
   unsigned short ca_nbrdna;     /* # of rings before no answer. */
   unsigned short ca_stdely;     /* Delay after dialing before analysis. */
   unsigned short ca_cnosig;     /* Duration of no signal time out delay. */
   short		  ca_lcdly;      /* Delay after dial before lc drop connect */
   unsigned short ca_lcdly1;     /* Delay after lc drop con. before msg. */
   unsigned short ca_hedge;      /* Edge of answer to send connect message. */
   unsigned short ca_cnosil;     /* Initial continuous noise timeout delay. */
   unsigned short ca_lo1tola;    /* % acceptable pos. dev of short low sig. */
   unsigned short ca_lo1tolb;    /* % acceptable neg. dev of short low sig. */
   unsigned short ca_lo2tola;    /* % acceptable pos. dev of long low sig. */
   unsigned short ca_lo2tolb;    /* % acceptable neg. dev of long low sig. */
   unsigned short ca_hi1tola;    /* % acceptable pos. dev of high signal. */
   unsigned short ca_hi1tolb;    /* % acceptable neg. dev of high signal. */
   unsigned short ca_lo1bmax;    /* Maximum interval for shrt low for busy. */
   unsigned short ca_lo2bmax;    /* Maximum interval for long low for busy. */
   unsigned short ca_hi1bmax;    /* Maximum interval for 1st high for busy */
   unsigned short ca_nsbusy;     /* Num. of highs after nbrdna busy check. */
   unsigned short ca_logltch;    /* Silence deglitch duration. */
   unsigned short ca_higltch;    /* Non-silence deglitch duration. */
   unsigned short ca_lo1rmax;    /* Max. short low  dur. of double ring. */
   unsigned short ca_lo2rmin;    /* Min. long low  dur. of double ring. */
   unsigned short ca_intflg;     /* Operator intercept mode. */
   unsigned short ca_intfltr;    /* Minimum signal to qualify freq. detect. */
   unsigned short rfu1;          /* reserved for future use */ 
   unsigned short rfu2;          /* reserved for future use */
   unsigned short rfu3;          /* reserved for future use */
   unsigned short rfu4;          /* reserved for future use */
   unsigned short ca_hisiz;      /* Used to determine which lowmax to use. */
   unsigned short ca_alowmax;    /* Max. low before con. if high >hisize. */
   unsigned short ca_blowmax;    /* Max. low before con. if high <hisize. */
   unsigned short ca_nbrbeg;     /* Number of rings before analysis begins. */
   unsigned short ca_hi1ceil;    /* Maximum 2nd high dur. for a retrain. */
   unsigned short ca_lo1ceil;    /* Maximum 1st low dur. for a retrain. */
   unsigned short ca_lowerfrq;   /* Lower allowable frequency in hz. */
   unsigned short ca_upperfrq;   /* Upper allowable frequency in hz. */
   unsigned short ca_timefrq;    /* Total duration of good signal required. */
   unsigned short ca_rejctfrq;   /* Allowable % of bad signal. */
   unsigned short ca_maxansr;    /* Maximum duration of answer. */
   unsigned short ca_ansrdgl;    /* Silence deglitching value for answer. */
   unsigned short ca_mxtimefrq;  /* max time for 1st freq to remain in bounds */
   unsigned short ca_lower2frq;  /* lower bound for second frequency */
   unsigned short ca_upper2frq;  /* upper bound for second frequency */
   unsigned short ca_time2frq;   /* min time for 2nd freq to remains in bounds */
   unsigned short ca_mxtime2frq; /* max time for 2nd freq to remain in bounds */
   unsigned short ca_lower3frq;  /* lower bound for third frequency */
   unsigned short ca_upper3frq;  /* upper bound for third frequency */
   unsigned short ca_time3frq;   /* min time for 3rd freq to remains in bounds */
   unsigned short ca_mxtime3frq; /* max time for 3rd freq to remain in bounds */
   unsigned short ca_dtn_pres;   /* Length of a valid dial tone (def=1sec) */
   unsigned short ca_dtn_npres;  /* Max time to wait for dial tone (def=3sec)*/
   unsigned short ca_dtn_deboff; /* The dialtone off debouncer (def=100ms) */
   unsigned short ca_pamd_failtime; /* Wait for AMD/PVD after cadence break(default=4sec)*/
   unsigned short ca_pamd_minring;  /* min allowable ring duration (def=1.9sec)*/
   unsigned char ca_pamd_spdval; /* Set to 2 selects quick decision (def=1) */
   unsigned char ca_pamd_qtemp;  /* The Qualification template to use for PAMD */
   unsigned short ca_noanswer;   /* time before no answer after first ring (default=30sec) */
   unsigned short ca_maxintering;   /* Max inter ring delay before connect (8 sec) */
} DX_CAP;


 /*******************************************************
 *                                                      *
 * Qualification Template Structure : INTERNAL use only *
 *                                                      *
 *******************************************************/
 
typedef struct {
     unsigned short int  q_nfrm;      /* no. of frames required to verify prior to energy drop */
     unsigned short int  q_snr0;      /* value of SNR required to begin suspecting tone */
     unsigned short int  q_snr1;      /* value of SNR required for all frames not dropping */
     unsigned short int  q_b0;        /* value of Broad required to begin suspecting tone */
     unsigned short int  q_b1;        /* value of Broad required for all frames not dropping */
     unsigned short int  q_trat;      /* maximum value of twist allowed */
     unsigned short int  q_qmax;      /* if fail talkoff, but pass qual qmax times, suspect */
     unsigned short int  q_tnfrm;     /* number of past frames to test talkoff with */
     unsigned short int  q_tflg;      /* talkoff flag */
     unsigned short int  q_minfrm;    /* min number of frames a tone must be present to verify */
     unsigned short int  q_zidflg;    /* zero inter digit delay flag */
     unsigned short int  q_oldprm;    /* set to 1 if old debounce parameters are to be used    */
   } TONE_QUAL;

typedef struct {
     unsigned short int  qminsnr;     /* minimum allowable SNR for voice */
     unsigned long  int  qmaxsnr;     /* maximum allowable SNR for voice */
     unsigned short int  maxpk;       /* maximum number of peaks for voice */
     unsigned short int  maxring;     /* maximum number of frames for ringback not voice */
     unsigned long  int  ringthres;   /* signal to noise ratio for ringback */
     unsigned short int  pvdwin;      /* number of frames in a window sample */
     unsigned long  int  pvdthresh;   /* minimum energy for voice */
     unsigned short int  pvdrblow;    /* lower frequency of ringback */
     unsigned short int  pvdrbhig;    /* upper frequency of ringback */
   } PVD_QUAL;

/*
 *	AMD Quality template structure
 */		
typedef struct {                         
   unsigned short int maxansiz;         /* Size of answer #1 */ 
   unsigned short int maxans2;
   unsigned short int maxans3;
   unsigned short int lohiss;           /* Low hiss (noise) range */ 
   unsigned short int hihiss;           /* High hiss (noise) */ 
   unsigned short int bhparm;           /* noise below hiss ratio */ 
   unsigned short int cvthr1;           /* cv. threshold #1 */ 
   unsigned short int cvthr2;
   unsigned short int maxcvth;          /* Maximum cv threshold */ 
   unsigned short int nmaxbrod;         /* Maximum broad band energy - noise */ 
   unsigned short int nmaxerg;          /* Maximum total energy - noise */ 
   unsigned short int maxsil;
   unsigned short int voice_thres;
   unsigned short int sil_thres;
   unsigned short int bandf_low;        /* Frequency Band filter, lower limit in hz */ 
   unsigned short int bandf_high;       /* upper limit in hz */ 
} AMD_QUAL;

typedef union {
   TONE_QUAL gen_qual;
   PVD_QUAL  pvd_qual;
   AMD_QUAL  amd_qual;
} TN_QLT;


#ifdef __cplusplus
extern "C" {           // C Plus Plus function bindings
#define extern
#endif

#if (defined (__cplusplus) || defined( __STDC__ ))  /* Function Prototypes - ANSI C & C++ */

extern int __cdecl dx_getqualtmplate(int ChDev, int Qualid, TN_QLT * Qltp);
extern int __cdecl dx_setqualtmplate(int ChDev, int Qualid, TN_QLT * Qltp);
extern int __cdecl dx_getextqualtmplate(int ChDev, int Qualid, int Size, TN_QLT * Qltp);
extern int __cdecl dx_setextqualtmplate(int ChDev, int Qualid, int Size, TN_QLT * Qltp);
#else
extern int __cdecl dx_getqualtmplate();
extern int __cdecl dx_setqualtmplate();
extern int __cdecl dx_getextqualtmplate();
extern int __cdecl dx_setextqualtmplate();
#endif

#ifdef __cplusplus
}                      // C Plus Plus function bindings
#undef extern
#endif


/*
 * Restore default structure packing
 */
#pragma pack()


#endif

