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
 *        FILE: dxlibdrv.h
 * DESCRIPTION: Common structs/defs between libraries and the SRAM PM
 **********************************************************************/

#ifndef __DXLIBDRV_H__
#define __DXLIBDRV_H__

/*
 * Miscellaneous defines
 */
#define DV_SEND                  0x07
#define DV_MAXCHANONBD           12    /* EFN: FIX LATER? * /


/*
 * FWI defines
 */
#define IO_CHNFWI                0x0000   /* Channel buffer area FWI type */
#define IO_DEVFWI                0x0001   /* Device area FWI type */
#define IO_CHNDATA               0x0100   /* Extra channel-area data */
#define IO_DEVDATA               0x0200   /* Extra dev-area data */

/*
 * Unique D/xxx H/W identifier (m.s.b. of dl_prod)
 */
#define DXXX_SIGN                0x80

/*
 * D/xxx command group identifiers
 */
#define DXXX_BD_ID               0x40
#define DXXX_CH_ID               0x30

/* 
 * Commands available to send to D/xxx (group DXX_BD_ID)
 */
#define DXXX_READFWVR_CMD        0x1   /* read FW version */
#define DXXX_READCOPY_CMD        0x2   /* reads legal information */
#define DXXX_WRITEPARM_CMD       0x5   /* update parameter */
#define DXXX_READPARM_CMD        0x6   /* read parameter */
#define DXXX_READSERL_CMD        0x7   /* read boards serial number */

/*
 * Commands available to send to D/xxx (group DXX_CH_ID)
 */
#define DXXX_ASSIGNTS_CMD        0x7   /* assign timeslot */

/* 
 * Replies available from D/xxx (group DXXX_BD_ID)
 */
#define DXXX_READFWVR_RPY        0x1   /* read FW version complete */
#define DXXX_READCOPY_RPY        0x2   /* read diagolic cpyrght cmplt */
#define DXXX_WRITEPARM_RPY       0x5   /* update parameter complete */
#define DXXX_READPARM_RPY        0x6   /* read parameter complete */
#define DXXX_ERROR               0x7   /* error in last command */
#define DXXX_READSERL_RPY        0x8   /* read boards serial number */

/* 
 * Replies available from D/xxx (group DXXX_CH_ID)
 */
#define DXXX_ASSIGNTS_RPY        0x8   /* T.S. assigned */

/*
 * Error replies 
 */
#define  DX_CHAN_BUSY            0x1   /* busy exec channel blkng fn */
#define  DX_GLOB_BUSY            0x2   /* busy exec global blkng fn */
#define  DX_INVL_CHAN_PARM       0x3   /* invalid channel param num */
#define  DX_INVL_GLOB_PARM       0x4   /* invlaid global param num */
#define  DX_INVL_SUB_CMD         0x5   /* invalid sub command number */
#define  DX_INVL_CHAN_NUM        0x6   /* invalid channel number */
#define  DX_MISS_NULL            0x7   /* no null term in char string */


/*
 * Device-specific error codes
 */
#define DLE_BADRANGE             1     /* Bad/overlapping phys mem range */
#define DLE_TIMEOUT              2     /* Timed out waiting for reply */
#define DLE_BADSIZE              3     /* Message too big or too small */
#define DLE_BADBOARD             4     /* Board missing or dead */
#define DLE_NOMEM                5     /* Can't map or allocate memory */
#define DLE_BADCMD               6     /* Invalid or undefined command */
#define DLE_BADPARAM             7     /* Invalid command parameter */
#define DLE_FWERROR              8     /* FW returned an error as reply */
#define DLE_SKIPREPLY            9     /* A required reply was skipped */
#define DLE_TOOBIG               10    /* Reply too big for user buffer */
#define DLE_BADSTATE             11    /* An invalid state was detected *

/*
 * DTI parameter info support
 */
#define DTIP_MAXPARM             12
#define DTIP_IDLESTATE           0
#define DTIP_IDLETYPE            1
#define DTIP_SIGMODE             2
#define DTIP_T1ERREVT            3
#define DTIP_PDIGEVT             4

#define DP_READ                  0
#define DP_WRITE                 1
#define DP_ADDBITS               2
#define DP_DELBITS               3

#define DTI_BDDEV                0
#define DTI_TSDEV                1


/*
 * DXXX_DATA: D4X-dependent data area for library-to-driver msgs
 */
typedef struct dxxx_data {
   ULONG    dxxx_maxrepsize;
   USHORT   dxxx_bufsz;
   USHORT   dxxx_offset;
   ULONG dxxx_rfu[2];
} DXXX_DATA;


/*
 * DX_DEVDEP: union of all D4X-dependent data areas that may be put into
 * a driver-to-library message. This union is incorporated into the 
 * DL_DRVCMD message structure.
 */
typedef union dx_devdep {
   DXXX_DATA  dv_data;
} DX_DEVDEP;


#define dx_maxrepsize            dx_devdata.dv_data.dxxx_maxrepsize
#define dx_bufsz                 dx_devdata.dv_data.dxxx_bufsz     
#define dx_offset                dx_devdata.dv_data.dxxx_offset    


/*
 * Macros for encoding and decoding device area replies.
 */
#define DV_REPENCODE(cmd,sub)   (((cmd) << 8) | (sub))
#define DV_REPDECODE(reply)     ((READ_REGISTER_UCHAR (&((reply).dl_cmd)) << 8) \
				| READ_REGISTER_UCHAR(&((reply).dl_sub)))

/*
 * Device-dependent structure for programmable stop command
 */
typedef struct {
   DX_CMND  ds_cmd;
   ULONG    ds_flag;
   ULONG    ds_fwi;
} DX_DEVSTOP;


/*
 * Device-dependendt structure for DTI parameter information.
 */
typedef struct {
   UCHAR dt_parmid;
   UCHAR dt_cmdid;
   UCHAR dt_devtype;
   UCHAR dt_rfu;
   ULONG dt_data;
} DTI_PARMINFO;


#endif


