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

/***********************************************************************
 *        FILE: faxlib.h
 * DESCRIPTION: Header File for DIALOGIC Windows NT library
 *
 **********************************************************************/

#ifndef __FAXLIB_H__
#define __FAXLIB_H__


/*
 * Fax channel parameters
 */

#define	PM_FXXX      	0x03000000	/* Identify as a FAX parameter */

/* 
 * Defines to indicate source and type of parameter (in dxxxlib.h)
 */
#define	FX_LBCPRM_SHT   (PM_LIB|PM_CH|PM_FXXX|PM_SHORT) 

/* Retry count (default=0, max=DF_RETRY3). The DF_RETRYx define can be
 * ORed with DF_RETRYDCN to cause a disconnect when max retries are reached.
 * ex:  DF_RETRY3|DF_RETRYDCN
 */
#define FC_RETRYCNT (FX_LBCPRM_SHT|0x0001) 

/* Defines for retransmission capabilities */
#define  DF_RETRYDCN    0x8000   /* Disconnect on max retries */
#define  DF_NORETRY     0x0      /* (default) */
#define  DF_RETRY1      0x1      /* retry once */
#define  DF_RETRY2      0x2      /* retry twice */
#define  DF_RETRY3      0x3      /* retry three times */

/* 
 * Inter-page D cmd sent by Transmitter in between pages
 * for multi-page TIFF documents.
 * Values: DFC_AUTO, DFC_MPS, DFC_EOM. Default: DFC_EOM 
 * NOTE: The default is maintained at DFC_EOM for backward compatibility.
 *       It is recommended that this parameter be set to DFC_AUTO.
 *       DFC_AUTO allows the fax library to automatically determine the 
 *       inter-page phase D value during a fax transmission.
 */
#define FC_SENDCONT (FX_LBCPRM_SHT|0x0002) 

/*
 * Delimiter for fx_rcvfax() for reception of TIFF documents. 
 * Allows the application to store each received page in seperate TIFF files.
 * Values: DFS_MPS, DFS_EOM, DFS_ALL (for both MPS & EOM).
 * Default: DFC_EOP -- store all received pages in single TIFF file and 
 * complete fx_rcvfax()  only after all pages have been received.
 */
#define FC_ENDDOC  (FX_LBCPRM_SHT|0x0003)

/* 
 * Local and Remote Id parameters.  The NULL terminated id string can 
 * have a maximum length of 20 characters plus NULL termination.
 */
#define FC_LOCALID   (PM_FW|PM_CH|PM_FXXX|PM_VLSTR|0x010)
#define FC_REMOTEID  (PM_FW|PM_CH|PM_FXXX|PM_VLSTR|0x011)

/* 
 * Parmeters for controlling percentage of acceptable bad scan lines received.
 * FC_RTN - default 15 - 15% bad scan lines before RTN sent in Phase D.
 * FC_RTP - default  5 -  5% bad scan lines before RTP sent in Phase D.
 */
#define FC_RTN       (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x012)
#define FC_RTP       (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x013)
#define FC_T1               (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x003)
#define FC_T3A				(PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x006)
#define FC_T4				(PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x007)
#define FC_T5				(PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x008)
#define FC_CNG              (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x00B)
#define FC_BCNG             (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x00C)
#define FC_MDM_RX_LVL		(PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x002B)   /* RX Soft modem rx level FR2416 */
/* 
 * Parameter for selecting the preferred baud rate for transmission.
 */
#define FC_TXBAUDRATE   (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x014)

/* Valid values for Transmission baud rate */
#define   DF_MAXBAUD     0xFFFF       /* Set to maximum supported rate */
#define   DF_14400BAUD   14400   
#define   DF_12200BAUD   12200   
#define   DF_9600BAUD    9600 
#define   DF_7200BAUD    7200
#define   DF_4800BAUD    4800
#define   DF_2400BAUD    2400

/* 
 * Parameter for selecting the baud rate for reception.
 * Valid values for FC_RXBAUDRATE are DF_14400BAUD, DF_9600BAUD,
 * DF_7200BAUD, DF_4800BAUD.
 */
#define FC_RXBAUDRATE   (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x018)

/*
 * Firmware parameter to select preferred transmit line encoding scheme:
 * Default value - DF_MMR. Other supported values - DF_MH, DF_MR
 * OR with DF_ECM to use ECM (Error Correcting Mode).
 */
#define FC_TXCODING   (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x01F)

#define DF_ECM     0x8000  /* OR with FC_TXCODING value to use ECM */

/*
 * Firmware parameter to specify the desired subaddress(es).
 */
#define FC_TXSUBADDR  (PM_FW|PM_CH|PM_FXXX|PM_VLSTR|0x027)

/*
 * Firmware parameter to read the remote station subaddress string.
 */
#define FC_REMOTESUBADDR  (PM_FW|PM_CH|PM_FXXX|PM_VLSTR|0x028)

/*
 * Firmware parameter to select Receive storage encoding scheme:
 * Default value - DF_MH. Other supported values - DF_MMR
 */
#define FC_RXCODING   (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x020)

/* 
 * Parameter to set zero- or one- based page numbering for TIFF/F files
 * read by fx_sendfax().  Default is zero based (TF_BASE0) as defined by
 * TIFF/F specifications.  Some applications may create TIFF/F files that
 * do not adhere to zero based page numbering specifiaction. For such
 * cases, this parameter must be set to TF_AUTOPG so that the fax library
 * may automatically determine the correct page numbering scheme.
 * NOTE: Applications must always use zero based page numbering (i.e first page
 * is page 0, second page is page 1 etc.) when specifying pages in the DF_IOTT.
 * The fax library will internally adjust the page number based on the 
 * value of the FC_TFPGBASE parameter.
 */
#define FC_TFPGBASE  (FX_LBCPRM_SHT|0x0010)

/*
 * Values for the TIFF/F page base parameter
 */
#define     TF_BASE0    0    /* Zero base TIFF/F file (default) */
#define     TF_BASE1    1    /* One base TIFF/F file */
#define     TF_AUTOPG   2    /* Automatic page scheme handling */

/* Parameter to set TIFF/F tag checking level */
#define FC_TFTAGCHECK (FX_LBCPRM_SHT|0x0011)

/* Font parameters for SoftFax rendering of ASCII documents */
#define FC_FONT0			(PM_LIB|PM_CH|PM_FXXX|PM_INT|0x0007)
#define FC_FONT3			(PM_LIB|PM_CH|PM_FXXX|PM_INT|0x0008)

/* Parameter for enabling ASCII to MH conversion in host host software or firmware */
#define  FC_SOFTFAX     (PM_LIB|PM_CH|PM_FXXX|PM_INT|0x0009)

/*
 * TIFF/F tag checking level values
 */
#define    TF_MAXTAGS   0    /* check all mandatory tags (default) */
#define    TF_MINTAGS   1    /* check only essential tags */

/*
 * Fax Page Header Parameters: The parameters control the information
 * that is displayed at the top of every transmitted fax page (FAX/4x).
 */
 
/* Fax Header attributes parameter */
#define FC_HDRATTRIB     (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x017)

/* values for header attributes */
#define DF_HDRINSERT     0x00  /* Hdr is inserted before image (default) */
#define DF_HDRDISABLE    0x01  /* Hdr is disabled */
#define DF_HDRUNDERLINE  0x04  /* Hdr is underlined */
#define DF_HDRBOLD       0x08  /* Hdr is displayed in bold typeface */
#define DF_HDRFMT1       0x00  /* Hdr format 1 (default) */
#define DF_HDRFMT2       0x10  /* Hdr format 2 (use FC_HDRUSER2) */

/* Starting page number parameter: (default = 1) */
#define FC_HDRSTARTPAGE   (PM_FW|PM_CH|PM_FXXX|PM_SHORT|0x021)


/*
 * Parameters for Header Format 1.
 */
#define DF_SF_DISABLED	0  /* Disable host base ASCII to MH conversion */
#define DF_SF_ENABLED  1   /* Enable  host base ASCII to MH conversion */

/* Date/Time field parameters */

/* Date format parameter for internal date field generation */
#define  FC_HDRDATEFMT     (FX_LBCPRM_SHT|0x0004)

/* Values for date format */
#define DF_HDRDATEFMT_0  0  /* disable internal date/time generation */
#define DF_HDRDATEFMT_1  1  /* Date appears as "MM-DD-YYYY"  (US) */
#define DF_HDRDATEFMT_2  2  /* Date appears as "MM/DD/YYYY"  (US) */
#define DF_HDRDATEFMT_3  3  /* Date appears as "DD-MM-YYYY"  (Europe) */
#define DF_HDRDATEFMT_4  4  /* Date appears as "DD/MM/YYYY"  (Europe) */
#define DF_HDRDATEFMT_5  5  /* Date appears as "YYYY-DD-MM"  (Japan) */
#define DF_HDRDATEFMT_6  6  /* Date appears as "YYYY/DD/MM"  (Japan) */
#define DF_HDRDATEFMT_15 15 /* Date appears as "Fri Sep 13 1986" */

/* Time format parameter for internal time field generation */
#define  FC_HDRTIMEFMT     (FX_LBCPRM_SHT|0x0005)

/* Values for date format */
#define DF_HDRTIMEFMT_0  0  /* disable internal date/time generation */
#define DF_HDRTIMEFMT_1  1  /* Time appears as "HH:MM am/pm" (12hr format) */
#define DF_HDRTIMEFMT_2  2  /* Time appears as "HH:MM" (24hr format) */

/* 
 * User formatted Date/Time string parameter - application provides a 
 * string (max 27 chars + null termination) which is directly used in 
 * Date/Time field.  Disable internal generation by setting one or both
 * of the Date/Time format parameters (above) to format 0.
 */
#define FC_HDRDATETIME   (PM_FW|PM_CH|PM_FXXX|PM_VLSTR|0x015)

/*
 * User string for fax header (max 32 chars + null):
 * Appears between the date/time and page number fields.
 */
#define FC_HDRUSER       (PM_FW|PM_CH|PM_FXXX|PM_VLSTR|0x016)

/*
 * Parameters for Header format 2. 
 * If the application wishes to configure the entire page header string
 * the FC_HDRATTRIB parameter must be set to DF_HDRFMT2 and the 
 * FC_HDRUSER2 parameter set to the string to be displayed.
 * The FC_HDRUSER2 may contain %R and %P to display the remote id and
 * page number.
 */

/* User string for entire Fax Header (128 bytes max) */
#define FC_HDRUSER2      (PM_LIB|PM_CH|PM_FXXX|PM_VLSTR|0x06)

#define FC_TXNSF		 (PM_LIB|PM_CH|PM_FXXX|PM_VLSTR|0x07)

/* End of Fax Page Header parameters */ 

/*
 * Defines for Phase D status values
 */
/* Command's from transmitter - returned by ATFX_PHDCMD() */
#define   DFS_EOP       0     /* End FAX session            */
#define   DFS_MPS       1     /* More pages to follow       */
#define   DFS_EOM       2     /* End of msg, more pgs to follow */
#define   DFS_POLL      3     /* A poll request was sent */
#define   DFS_PRI_EOP   16    /* Request for Operator intervention (PRI-EOP) */
#define   DFS_PRI_MPS   17    /* Request for Operator intervention (PRI-MPS) */
#define   DFS_PRI_EOM   18    /* Request for Operator intervention (PRI-EOM) */

/* Replies from receiver - returned by ATFX_PHDRPY() */
#define   DFS_MCF     1     /* Message Confirmation       */        
#define   DFS_RTN     2     /* Retrain negative           */
#define   DFS_RTP     3     /* Retrain positive           */
#define   DFS_PIP     4     /* Procedure interrupt positive */
#define   DFS_PIN     5     /* Procedure interrupt negative */

/* 
 * Values for FC_ENDDOC parameter
 *  DFS_MPS, DFS_EOM, DFS_ALL, default DFS_EOP 
 */
#define   DFS_ALL     5

/* Phase B status bitmap - returned by ATFX_BSTAT() */
#define  DFS_REMOTEID   0x0100     /* Remote id is available */
#define  DFS_NSF        0x0200     /* NSF is available */
#define  DFS_DCS        0x0400     /* DCS information is available */
#define  DFS_DIS        0x0400     /* DIS information is available */
#define  DFS_REMOTESUBADDR 0x0800  /* subaddress info is available */

/* FAX channel base hardware type - returned by ATFX_CHTYPE() */
#define  DFS_FAX120 0x0000        /* FAX/120 channel */
#define  DFS_FAXDM3 0X0001        /* Dm3 fax channel */
#define  DFS_FAX40  0x3400        /* VFX/40 channel */
#define  DFS_FAX40E 0x3401        /* VFX/40E channel */
#define  DFS_FAX40EPLUS 0x3402    /* VFX/40ESC+ channel */
#define  DFS_FAX40EPLUSREX 0x3403 /* VFX/40ESC+REX channel */
#define  DFS_D41JCT 0x3404        /* D41JCT channel, Basic SoftFax with resource sharing */
#define  DFS_FAX41JCT 0x3405 /* VFX/41JCT channel, Enhanced SoftFax with resource sharing */

/* Line enconding sheme - returned by ATFX_CODING() */
#define   DFS_MH         0      /* 1-D Group 3 Modified Huffmann encoding */
#define   DFS_MR         1      /* 2-D Group 3, T.4 Modified Read encoding */
#define   DFS_MMR        2      /* T.6 Modified Modified Read encoding */

/* Phase B ECM status - returned by ATFX_ECM() */
#define  DFS_ECM_UNAVAILABLE    0       /* ECM unavailable enabled */
#define  DFS_ECM		        1       /* ECM enabled */
#define  DFS_NOECM              2       /* ECM not enabled */

/*
 *  Bits for the rcvflag/sndflag arguments to fx_rcvfax()/fx_sendfax()
 */

/* Enable polling during receive (rcvflag) */
#define   DF_NOPOLL     0x0000     /* Reject a poll */
#define   DF_POLL       0x0002     /* Accept a poll */

/* Enable Remote Terminal Verification (sndflag) */
#define   DF_ENABLE_RTV 0x1000     /* Enable RTV */

/* Enable phase B & D events (rcvflag/sndflag) */
#define   DF_PHASEB     0x0004     /* Generate Phase B event */
#define   DF_PHASED     0x0008     /* Generate Phase D event */

/* Enable/Issue operator intervention (rcvflag/sndflag) */
#define  DF_ISSUE_VRQ   0x0100     /* Issue a voice request */
#define  DF_ACCEPT_VRQ  0x0200     /* Accept a voice request */

/* Receive width selection - set maximum width for reception (rcvflag) */
#define  DF_1728MAX     0x0020     /* Maximum receive width is 1728 pels */
#define  DF_2048MAX     0x0040     /* Maximum receive width is 2048 pels */
                                   /* can recv 1728/2048 */
#define  DF_2432MAX     0x0000     /* Maximum receive width is 2432 pels */
                                   /* can recv 1728/2048/2432 */
                                   /* default */

/* Receive length selection - set maximum length of page (rcvflag) */
#define  DF_A4MAXLEN    0x10000    /* Maximum receive length is A4 size */
#define  DF_B4MAXLEN    0x20000    /* Maximum receive length is B4 size */
#define  DF_NOMAXLEN    0x40000    /* Unlimited receive length */

/* Enable/Disable of T.30 subaddressing information (rcvflag) */
#define  DF_NOSUBADDRDEL 0x0        /* Do not delimit with Phase B msg. */
#define  DF_SUBADDRDEL   0x400000   /* Delimit with T.30 SUB Pase B msg. */

/* 
 * Transmission Resolution Selection (sndflag).
 * If neither of these bits is set, the DF_IOTTs are transmitted at
 * the resolutions at which the documents are stored.
 */
#define  DF_TXRESLO			0x080000   /* Transmit all data in coarse resln */
#define  DF_TXRESHI			0x100000   /* Transmit all data in fine resln */
#define  DF_TXRES_300_300   0x040000   /* Transmit all data in 300(horizontal) x 300(Vertical) resolution */ 
#define  DF_TXRES_200_400   0x020000   /* Transmit all data in 200(horizontal) x 400(Vertical) resolution */ 
/* 
 * Storage Resolution Selection during receive (rcvflag).
 * If neither of these bits are set, the received file
 * is stored at the resolution at which the data is received.
 */
#define  DF_RXRESLO			0x080000   /* Store all received data in coarse resln */
#define  DF_RXRESHI			0x100000   /* Store all received data in fine resln */
#define  DF_RXRES_300_300   0x040000   /* Store all received data in 300(horizontal) x 300(Vertical) resolution */
#define  DF_RXRES_200_400	0x020000   /* Store all received data in 200(horizontal) x 400(Vertical) resolution */

/*
 * T.30 Subaddress Fax routing.
 */
#define  DF_TXSUBADDR    0x200000   /* Send a fax with subaddress info. */

/* 
 *  Fax Library error codes returned by ATDV_LASTERR( )
 */
#define   EFX_NOTIMP	  0x0100   /* Function not implemented */
#define   EFX_POLLED      0x0101   /* Received poll from remote */
#define   EFX_NOPOLL      0x0102   /* Remote did not accept poll */
#define   EFX_COMPAT      0x0103   /* Hardware incapable of transmitting at
                                    * specified width and resolution
                                    */
#define   EFX_BADTIF      0x0104    /* Incorrect TIFF/F format */
#define   EFX_BADTAG      0x0105    /* Incorrect values for TIFF/F tags */
#define   EFX_NOPAGE      0x0106    /* Specified page missing in TIFF/F file */
#define   EFX_DISCONNECT  0x0107    /* Remote has disconnected */
#define   EFX_RETRYDCN    0x0108    /* Disconnected after specifed retries */
#define   EFX_BADIOTT     0x0109    /* DF_IOTT has incorrect entries */
#define   EFX_BADPAGE     0x010A    /* Not a valid page in TIFF/F file */
#define	  EFX_NOSTATE     0x010B    /* Initial state not set */
#define   EFX_BADSTATE    0x010C    /* Incorrect value for initial state */
#define   EFX_MAXCHAN     0x010D  /* async case: max chan capacity reached */
#define   EFX_NOMEM       0x010E  /* cannot allocate memory for more pages */
#define   EFX_INVALARG    0x010f   /* illegal argument to function */
#define   EFX_INVALFUNC   0x0110   /* illegal call to function - calling
				    *  fx_initstat() when fax in progress or
				    * ATFX_xxx() before a fax session
				    */
#define   EFX_NOFAX       0x0111    /* no fax capability on this board */
#define   EFX_NOTIDLE     0x0112    /* channel is not idle */
#define   EFX_BADPARM     0x0113    /* Invalid value for fax parameter */
#define   EFX_BADTFHDR    0x0114    /* Bad TIFF/F header */
#define   EFX_UNSUPPORTED 0x0115    /* Unsupported feature */
#define   EFX_RDFWVER     0x0116    /* Error reading firmware version */
#define   EFX_NSFBUFF     0x0118    /* NSF buffer less than 2 bytes long */
#define   EFX_NODATA      0x0119    /* Requested data not available */
#define   EFX_NOTIMESLOT  0x011A    /* No timeslot assigned to Fax channel */
#define	  EFX_INVALID_FONT 0x011B   /* Invalid font in font file */

#define   EFX_CMDDATA     0x01fb   /* Invalid cmd/data sent to Firmware */
#define   EFX_FWERROR     0x01fc   /* Firmware Error */
#define   EFX_BADPHASE    0x01fd   /* unexpected phase transition */
#define   EFX_DRVERROR    0x01fe   /* Error in Driver */
#define   EFX_LIBERROR    0x01ff   /* Error in library state machine */

/*
 * Initial protocol states - set with fx_initstat()
 */
#define     DF_RX       0      /* Receiver    */
#define     DF_TX       1      /* Transmitter */
#define		DF_T38GW	2      /* V17/T38 GW  */
typedef struct df_txnsf {
    unsigned char length;
    unsigned char nsf[255];
} DF_TXNSF;

/* 
 * The DF_ASCIIDATA structure - for use with transmission of ASCII data
 * A pointer to this structure may be passed in the DF_IOTT structure for
 * transmitting an ASCII file. Default values are assumed if no DF_ASCIIDATA
 * is specified.
 */

typedef struct {
   unsigned short pagelength;     /* Page length */
   unsigned short pagepad;        /* Pad with blank scan lines to page end */
   unsigned short topmargin;      /* Top margin */
   unsigned short botmargin;      /* Bottom margin */
   unsigned short leftmargin;     /* Left margin */
   unsigned short rightmargin;    /* Right margin */
   unsigned short linespace;       /* Spacing between Ascii lines */
   unsigned short font;            /* Font selection */
   unsigned short tabstops;        /* Number of tab stops on line */
   unsigned char  units;           /* Units for specifying margins/lengths */
   unsigned char  flags;          /* Reserved for future use */
} DF_ASCIIDATA;

/* Defines for the DF_ASCIIDATA structure */

/* Line spacing */
/* Font dependent line spacing */

#define DF_SINGLESPACE  2  /* Single spacing (default) */
#define DF_DOUBLESPACE  4  /* Double spacing */
#define DF_TRIPLESPACE  6  /* Triple spacing */
#define DF_HALFSPACE    1  /* 1/2 spacing - add (+) to DF_SINGLESPACE
                            *  or DF_DOUBLESPACE
                            */

/* Alternate line spacing equates */
/* Font independent line spacing */

#define DF_6LPI       0x8000  /* 6 lines per inch */
#define DF_4LPI       0x8001  /* 4 lines per inch */
#define DF_3LPI       0x8002  /* 3 lines per inch */
#define DF_2_4LPI     0x8003  /* 2.4 lines per inch */
#define DF_8LPI       0x8004  /* 8 lines per inch */

/* font */
#define  DF_FONT_0   0   /* Normal 10 pitch, 6 lines/inch (default) */
#define  DF_FONT_3   3   /* Compressed 17 pitch, 8 lines/inch */


/* pagepad */
#define  DF_PAD      1   /* Pad to end of page with blank lines (default) */
#define  DF_NOPAD    0   /* No padding */

/* Units.   1/10th inch default */
#define  DF_UNITS_IN10   0  /* Margins and page length in 1/10th inch units */
#define  DF_UNITS_MM     1  /* Margins and page length in millimeter units  */
#define  DF_PELS         2  /* Vertical - coarse resolution scanlines */
                            /* Horizontal - pixels */

/*
 * The DF_IOTT structure 
 */
typedef struct df_iott DF_IOTT;

struct df_iott {
   unsigned long  io_offset;      /* Start page or file offset */
   unsigned long  io_length;      /* Number of pages or length of data */
   char          *io_bufferp;     /* Memory transfer start location */
   DF_IOTT       *io_prevp;       /* Pointer to the previous iott entry */
   DF_IOTT       *io_nextp;       /* Pointer to the next iott entry */
   void          *io_datap;       /* Pointer to additional data associated
				     with io_datatype */
   int            io_fhandle;     /* File descriptor */
   unsigned short io_type;        /* Entry type */
   unsigned short io_datatype;    /* Transmit data type */
   unsigned short io_phdcont;     /* Phase D TX value for last page */
   unsigned short io_width;       /* Image width (raw/ascii mode only) */
   unsigned char  io_resln;       /* Image data resolution (raw mode only) */
   unsigned char  io_coding;      /* Image encoding used (raw mode only) */
   unsigned char  rfu[2];         /* Reserved for future use */
};

/* defines for offset and length as first_page and page_count */
#define   io_firstpg     io_offset
#define   io_pgcount     io_length

/* 
 * Defines for DF_IOTT data structure
 */
/* 
 * Phase D Commands to transmit -- io_phdcont field 
 * Note: Old equates DFC_MPS/DFC_EOM are maintained for backward 
 * compatibility. These equates are obsoleted by DFC_AUTO.
 * Applications are recommended to use DFC_AUTO.
 */
#define   DFC_EOP     0x0      /* End of Procedure - no more pages */
#define   DFC_MPG     0x6      /* Merge page data from next IOTT */
#define   DFC_AUTO    0xFF     /* Automatic phase D determination */


/* Defines for io_datatype */
#define   DF_TIFF      0x0
#define   DF_RAW       0x1
#define   DF_ASCII     0x3     

/* 
 * The fields given below are relevant only for transmission of 
 * RAW image files (io_datatype = DF_RAW) and ASCII files 
 * (io_datatype = DF_ASCII).  For TIFF/F files, this
 * information is embedded in the TIFF/F file itself.
 * For ASCII files, the io_coding field is ignored.
 */

/* 1.  Defines for io_width (also returned by ATFX_WIDTH()) */
#define   DF_WID1728   1728      /* 1728 pels per line */
#define   DF_WID2048   2048      /* 2048 pels per line */
#define   DF_WID2432   2432      /* 2432 pels per line */
#define   DF_WID2592   2592      /* Only for Vertical Resolution DF_RES_300_300, 2592 pels per line */

/* 2.  Defines for io_resln field (also returned by ATFX_RESLN()) */
#define   DF_RESLO       98       /* Lo (coarse) resolution */
#define   DF_RESHI       196      /* Hi (fine) resolution   */
#define   DF_RES_300_300 300	  /* 300(horizontal) x 300(Vertical) resolution */
#define   DF_RES_200_400 400	  /* 200(horizontal) x 400(Vertical) resolution */

/* 3.  Defines for io_coding field (also returned by ATFX_CODING()) */
#define   DF_MH         0      /* 1-D Group 3 Modified Huffmann encoding */
#define   DF_MR         1      /* 2-D Group 3, T.4 Modified Read encoding */
#define   DF_MMR        2      /* T.6 Modified Modified Read encoding */

/*
 * Phase E status values for ATFX_ESTAT()
 */

/* Status values returned to transmitter */

#define EFX_T1EXPTX	  100  /* Timer T1 expired waiting for message */  
#define EFX_NODISTX       101  /* Got other than DIS while waiting for DIS */  
#define EFX_GOTDCNTX      102  /* Got DCN while waiting for DIS */  
#define EFX_BADDCSTX      103  /* Bad response to DCS, training */  
#define EFX_PHBDEADTX     104  /* No response to DCS, training or TCF */  
#define EFX_NOISETX       105  /* Too much noise training at 2400 bps */  
#define EFX_BADPGTX       106  /* DCN response after sending page */  
#define EFX_PHDDEADTX     107  /* No response after sending page */  
#define EFX_RXCOMP        109  /* Remote site is not receive compatible */  
#define EFX_INVALRSPTX    110  /* No valid response after sending page */  
#define EFX_NOFINERECTX   111  /* Remote cannot receive fine resln documents */
#define EFX_COMMERRTX     190  /* Transmit communication error */
#define EFX_NXTCMDTX      197  /* Timeout waiting for next send page cmd */  
#define EFX_NOWIDTHTX     202  /* Remote cannot receive at specified width */
#define EFX_ECMRNRTX      210  /* Timer T5 expired, receiver not ready */
#define EFX_ECMPHDTX      211  /* Invalid ECM response from receiver */
#define EFX_INVALMMRTX    213    /* Invalid input MMR data */


/* Status values returned to receiver */

#define EFX_TXCOMP        108  /* Remote site is not transmit compatible */  
#define EFX_T2EXPRX       112  /* Timer T2 expired waiting for NSS/DCS/MCF */
#define EFX_T2EXPMPSRX    113  /* Timer T2 expired waiting for next Fax page */
#define EFX_T2EXPDCNRX    114  /* Timer T2 expired waiting for DCN */
#define EFX_T2EXPDRX      115  /* Timer T2 expired waiting for Phase D */
#define EFX_INVALCMDRX    116  /* Unexpected command after page received */
#define EFX_T2EXPFAXRX    117  /* Timer T2 expired waiting for fax page */
#define EFX_T2EXPRRRX     118  /* Timer T2 expired waiting for RR command */
#define EFX_T1EXPRX	  120  /* Timer T1 expired waiting for message */  
#define EFX_GOTDCSRX	  121  /* DCS received while waiting for DTC */
#define EFX_WHYDCNRX	  127  /* Unexpected DCN while waiting for DCS/DIS */  
#define EFX_DCNDATARX	  128  /* Unexpected DCN while wtng for Fax data */  
#define EFX_DCNFAXRX      129  /* Unexpected DCN while wtng for EOM/EOP/MPS */
#define EFX_DCNPHDRX      130  /* Unexpected DCN after EOM/MPS sequence */  
#define EFX_DCNNORTNRX    131  /* DCN after requested retransmission */  
#define EFX_DCNRRDRX      132  /* Unexpected DCN after RR/RNR sequence */
#define EFX_COMMERRRX     191  /* Receive communication error */
#define EFX_NOFAXRX	  193  /* Timed out waiting for first line */  
#define EFX_NOEOLRX	  194  /* Timed out waiting for EOL */  
#define EFX_NOCARRIERRX	  195  /* Lost carrier during Fax receive */  
#define EFX_NXTCMDRX	  198  /* Timed out wtng for next receive page cmd */
#define EFX_PNSUCRX	  205  /* No PN_SUCCESS returned by modem during rcv */
#define EFX_ECMPHDRX      212  /* Invalid ECM response from transmitter */


/* General status values returned to receiver or transmitter */
#define EFX_BUSYCHN        140  /* Request to start Fax while channel busy */  
#define EFX_ABORTCMD       199  /* Stop Fax command received */  
#define EFX_CHIPNORESP     200  /* Fax modem not responding */  
#define EFX_CEDTONE        201  /* Remote CED tone is longer than 5 secs */  
#define EFX_OPINTFAIL      203  /* Operator intervention failed */  
#define EFX_HDLCCARR       204  /* Excessive HDLC carrier */  

/* V17/T38 Gateway Error values from ATFX_ESTAT */

#define EFX_SIGNALTIMEOUT	214		// Signal timeout - no data or events received during GW session
#define EFX_DCNTIMEOUT		215		// DCN timeout - GW session almost complete but no DCN received
#define EFX_BADIPADDRESS	216		// Bad IP address - T38 subsystem did not get remote IP address - check R4 application
#define EFX_CTBUSERROR		217		// CTBus error w/TDM portion of GW session - check R4 application


/* 
 * Fax events returned to the application 
 */
#define  TFX_FAXSEND   0x0A1      /* Send fax complete */
#define  TFX_FAXRECV   0x0A2      /* Receive fax complete */
#define  TFX_FAXERROR  0x0A3      /* Error event for Fax */
#define  TFX_PHASEB    0x0A4      /* Phase B event */
#define  TFX_PHASED    0x0A5      /* Phase D event */
#define  TFX_LOADFONT  0x0A6      /* Font loading complete */
#define  TFX_T38GW     0x0A9	  /* V17/T38 GW Completion event */

// 03/27/07 Use 0x3A00 as base value for event codes going forward.
#define  FAXEV_BASE			0x3A00				// beginning base value
#define  FAXEV_LAST_EVENT	(FAXEV_BASE | 0xFF)	// Last event value


/* 
 * Fax termination reasons returned as a bitmap by ATFX_TERMMSK() 
 */
#define  TM_FXTERM     0x01       /* Normal termination */
#define  TM_POLLED     0x02       /* Poll received from remote */
#define  TM_VOICEREQ   0x04       /* Voice request issued/received */


/*
 * The DF_DIS and DF_DCS structures for returning T.30 DCS and DIS
 * messages. Refer T.30 specifications for details. 
 */
typedef struct {
    char dis_data[10];     /* DIS information in LSB format */
} DF_DIS;

typedef struct {
    char dcs_data[10];     /* DCS information in LSB format */
} DF_DCS;

/* 
 * User defined IO functions - read()/write()/seek() 
 */
#if (defined (__cplusplus) || defined( __STDC__ ) || defined (__BORLANDC__))

typedef struct df_uio {
   /* User Defined replacement for read()	*/
   int (* u_read)(int fd, char *bufferp, unsigned count);

   /* User Defined replacement for write()	*/
   int (* u_write)(int fd, char *bufferp, unsigned count);

   /* User Defined replacement for lseek()	*/
   int (* u_seek)(int fd, long offset, int mode);

} DF_UIO;

#else

typedef struct df_uio {
   int (* u_read)();		/* User Defined replacement for read()	*/
   int (* u_write)();		/* User Defined replacement for write()	*/
   int (* u_seek)();		/* User Defined replacement for lseek()	*/
} DF_UIO;

#endif


/* Parameter to cause fax session to be aborted on certain events */
#define FC_STOPCH  (FX_LBCPRM_SHT|0x0001f)


/* 
 * Events on which a session is stopped. Set FC_STOPCH to a bitmap of the 
 * values given below.  Note: dx_setevtmsk() must be used
 * to enable the occurrence of these events.  The events will be intercepted
 * by the fax library and a stop issued to the channel depending on FC_STOPCH.
 */
#define DF_LCOFF   1   /* stop the fax on a loop current drop */



/* Undocumented feature for pre-version 2.0 fax applications ONLY:
 * Backward compatability parameter for old method of indicating a poll.
 * Pre-version 2.0 fax library applications already using the polling feature
 * will need to set this parameter to DF_OLDMODE to activate the old mechanism
 * (fx_rcvfax() returns an error and the error code is set to EFX_POLLED).
 *  By default, a poll is now indicated via the ATFX_TERMMSK() which returns
 * a bitmap of termination reasons.
 */
#define FC_BACKCOMPAT  ((PM_LIB|PM_CH|PM_FXXX|PM_SHORT)|0x000F)

#define DF_NEWMODE   0  /* default */
#define DF_OLDMODE   1  /* Activate pre-version 2.0 poll mechanism */

#define TFX_ORIGINATE	0x0A8	// Dis event was received. Dcs on hold
#define DF_JPEG_GREY		3	// set ECM and T6
#define DF_JPEG_COLOR	4	// set ECM and T6

 /* 
 * Fax Library Function Declarations 
 */
#ifdef __cplusplus
extern "C" {				// C Plus Plus function bindings
#define extern
#endif

#if (defined (__cplusplus) || defined( __STDC__ ) || defined (__BORLANDC__))

/* Fax Library Function Prototypes - ANSI C & C++ */

extern const char * __cdecl fx_getversion ( int dev, int flag );
extern int __cdecl fx_getcopyright ( int dev, char *buffer );
extern int __cdecl fx_loadfontfile ( int dev, char *fontfilename, unsigned long ulMode );
extern int __cdecl fx_open(const char * name, int oflags);
extern int __cdecl fx_close(int dev);
extern int __cdecl fx_stopch(int dev, int mode);
extern int __cdecl fx_initstat(int dev, int state);
extern int __cdecl fx_sendfax(int dev, DF_IOTT * iotp, unsigned long sndflag);
extern int __cdecl fx_rcvfax(int dev, const char * faxname, unsigned long rcvflag);
extern int __cdecl fx_rcvfax2(int dev, int fd, unsigned long rcvflag);
extern int __cdecl fx_setparm(int dev, unsigned long parm, const void * valuep);
extern int __cdecl fx_getparm(int dev, unsigned long parm, void * valuep);
extern void __cdecl fx_setuio(DF_UIO df_uio);
extern void __cdecl fx_setiott(DF_IOTT *iotp, int fhandle, unsigned short dtype,
                         unsigned short cont);
extern int __cdecl fx_getDIS(int, DF_DIS *);
extern int __cdecl fx_getDCS(int, DF_DCS *);
extern int __cdecl fx_getNSF(int, unsigned short, char *);
extern int __cdecl fx_loadfont(int devd, int fontfh, unsigned long mode);
extern int __cdecl fx_rtvContinue(int iDev, int iContinue);
extern int __cdecl fx_getctinfo(int iDev, CT_DEVINFO* ctinfo);
extern int __cdecl fx_originate(int dev, int mode);

#ifdef _WIN32
extern long __cdecl fx_libinit(unsigned short flag);
extern long __cdecl fx_GetDLLVersion(LPDWORD fileverp, LPDWORD prodverp);
#endif

/* Fax convenience functions */

extern int __cdecl fx_sendtiff(const char * faxname, unsigned long firstpg,
                        unsigned long pgcount, unsigned short phdcont);
extern int __cdecl fx_sendraw(const char * faxname, unsigned short width,
                        unsigned char resln, unsigned short phdcont);
extern int __cdecl fx_sendascii(const char * faxname, unsigned short phdcont);

/* Fax Extended Attributes */

extern long __cdecl ATFX_SPEED(int dev);
extern long __cdecl ATFX_WIDTH(int dev);
extern long __cdecl ATFX_RESLN(int dev);
extern long __cdecl ATFX_PGXFER(int dev);
extern DF_IOTT * __cdecl ATFX_LASTIOTT(int dev);
extern DF_IOTT * __cdecl ATFX_BADIOTT(int dev);
extern long __cdecl ATFX_BADPAGE(int dev);
extern long __cdecl ATFX_ESTAT(int dev);
extern long __cdecl ATFX_BSTAT(int dev);
extern long __cdecl ATFX_PHDCMD(int dev);
extern long __cdecl ATFX_PHDRPY(int dev);
extern long __cdecl ATFX_TERMMSK(int dev);
extern long __cdecl ATFX_BSTAT(int dev);
extern const char * __cdecl ATFX_FXVERSION(int dev);
extern long __cdecl ATFX_TFBADTAG(int dev);
extern long __cdecl ATFX_TFPGBASE(int dev);
extern long __cdecl ATFX_TFNOTAG(int dev);
extern long __cdecl ATFX_RTNPAGES(int dev);
extern long __cdecl ATFX_SCANLINES(int dev);
extern long __cdecl ATFX_BADSCANLINES(int dev);
extern long __cdecl ATFX_CODING(int dev);
extern long __cdecl ATFX_CHTYPE(int dev);
extern long __cdecl ATFX_TRCOUNT(int dev);
extern long __cdecl ATFX_STATE(int dev);
extern long __cdecl ATFX_ECM(int dev);

#ifdef _WIN32
long __cdecl _faxGetDllVersion(char *szFileName,PDWORD pdwFileVerMajor,
				                    PDWORD pdwFileVerMinor, PDWORD pdwProductVerMajor,
				                    PDWORD pdwProductVerMinor);
#endif
                                                            
/* Fax SCBus functions */
extern int __cdecl fx_listen(int dev, SC_TSINFO * sc_tsinfop);
extern int __cdecl fx_unlisten(int dev);
extern int __cdecl fx_getxmitslot(int dev, SC_TSINFO * sc_tsinfop);

#else

/* Fax Library Function prototypes -- old K&R C */
extern const char * __cdecl fx_getversion (  );
extern int __cdecl fx_getcopyright ( );
extern int __cdecl fx_loadfontfile ( );


extern int __cdecl fx_open();
extern int __cdecl fx_close();
extern int __cdecl fx_stopch();
extern int __cdecl fx_initstat();
extern int __cdecl fx_sendfax();
extern int __cdecl fx_rcvfax();
extern int __cdecl fx_rcvfax2();
extern int __cdecl fx_setparm();
extern int __cdecl fx_getparm();
extern void __cdecl fx_setuio();
extern void __cdecl fx_setiott();
extern int __cdecl fx_getDIS();
extern int __cdecl fx_getDCS();
extern int __cdecl fx_getNSF();
extern int __cdecl fx_loadfont();
extern int __cdecl fx_rtvContinue();
extern int __cdecl fx_getctinfo();
extern int __cdecl fx_originate();

#ifdef _WIN32
extern long __cdecl fx_libinit();
extern long __cdecl fx_GetDLLVersion();
#endif

/* Fax convenience functions */
extern int __cdecl fx_sendtiff();
extern int __cdecl fx_sendraw();
extern int __cdecl fx_sendascii();

 /* Function declarations for Fax extended attributes */

extern long __cdecl ATFX_SPEED();
extern long __cdecl ATFX_WIDTH();
extern long __cdecl ATFX_RESLN();
extern long __cdecl ATFX_PGXFER();
extern DF_IOTT * __cdecl ATFX_LASTIOTT();
extern DF_IOTT * __cdecl ATFX_BADIOTT();
extern long __cdecl ATFX_BADPAGE();
extern long __cdecl ATFX_ESTAT();
extern long __cdecl ATFX_BSTAT();
extern long __cdecl ATFX_PHDCMD();
extern long __cdecl ATFX_PHDRPY();
extern long __cdecl ATFX_TERMMSK();
extern long __cdecl ATFX_BSTAT();
extern char * __cdecl ATFX_FXVERSION();
extern long __cdecl ATFX_TFBADTAG();
extern long __cdecl ATFX_TFPGBASE();
extern long __cdecl ATFX_TFNOTAG();
extern long __cdecl ATFX_RTNPAGES();
extern long __cdecl ATFX_SCANLINES();
extern long __cdecl ATFX_BADSCANLINES();
extern long __cdecl ATFX_CODING();
extern long __cdecl ATFX_CHTYPE();
extern long __cdecl ATFX_TRCOUNT(); 
extern long __cdecl ATFX_STATE();   
extern long __cdecl ATFX_ECM();   
                             
/* Fax SCBus functions */
extern int  __cdecl fx_listen();
extern int  __cdecl fx_unlisten();
extern int  __cdecl fx_getxmitslot();

                        
#endif

#ifdef __cplusplus
}				// C Plus Plus function bindings
#undef extern
#endif

/*
 *  EQUATES FOR BACKWARD COMPATIBILITY
 */
/* old error codes */
#define   EF_POLLED      0x0101
#define   EF_NOPOLL      0x0102
#define   EF_COMPAT      0x0103
#define   EF_BADTIF      0x0104
#define   EF_BADTAG      0x0105
#define   EF_NOPAGE      0x0106
#define   EF_DISCONNECT  0x0107
#define   EF_NOINFO      0x0108
#define   EF_BADIOTT     0x0109
#define   EF_BADPAGE     0x010A
#define	  EF_NOSTATE     0x010B
#define   EF_BADSTATE    0x010C
#define   EFX_SUBPGRESLN 0x0117 

/* Phase E status value for backward compatibility */
#define EFX_BUSY       140  /* Request to start Fax while channel busy */  
#define EFX_COMERRRX      191  /* Receive communication error */

/* old equate for encoding scheme */
#define   DF_1DIM       DF_MH   /* 1-Dimensional encoding */

/* old equates for image width */
#define   DF_LEN1728   DF_WID1728      /* 1728 pels per line */
#define   DF_LEN2048   DF_WID2048      /* 2048 pels per line */
#define   DF_LEN2432   DF_WID2432      /* 2432 pels per line */

/* Obsolete phase D commands - maintained for backward compatibility */
#define   DFC_MPS     0x2      /* More pages at same resln/width  */
#define   DFC_EOM     0x4      /* More pages, renogiate resln/width etc. */

/* Fax Page Header - Unsupported features below */
#define DF_HDROVERWRT    0x02  /* Hdr overwrites first few scan lines of img */
#endif