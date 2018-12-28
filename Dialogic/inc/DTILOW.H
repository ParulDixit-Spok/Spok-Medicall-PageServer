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
 * FILENAME:    dtilow.h
 * DESCRIPTION:  
 ********************************************************************************/

/********************************************************************/
/* 	The equates in this file are useful for driver writers. */
/********************************************************************/

/* DTI values (DT_) */
/*
 *-------The message protocol encodes the message in one byte.
 *	The encoding is as follows:
 *	Bit 7: 		1=Board level, 0=Timeslot level.
 *	Bit 6: 		1=Single byte message, 0=Multibyte message.
 *	Bit 5-0:	if bit 6 is 0 then bits 5-0 are the message
 *			number.
 *			if bit 6 is 1 then bits 6,5 and 4 are the 
 *			port number, bits 2-0 are the message number and
 *			bit 3 is not used.
 */

/*
 *-------Values for defining the message protocol encoded byte.
 */
#define  DT_BRDLEVEL   0x80      /* Board level msgs have hi bit set. */
#define  DT_TSLEVEL    0x00      /* Timeslot level msgs have hi bit clr. */
#define  DT_SB7        0x70      /* Port 7 single byte message. */
#define  DT_SB6        0x60      /* Port 6 single byte message. */
#define  DT_SB5        0x50      /* Port 5 single byte message. */
#define  DT_MB         0x00      /* Multibyte message. */

/*
 *-------DTI protocol values.  These are the values to AND with to
 *	unencode the encoded byte.
 */
#define  DTP_PORT      0x70      /* Value to AND with to get port. */
#define  DTP_MBMSG     0x3F      /* Value to AND with to get mb msg #. */
#define  DTP_SBMSG     0x07      /* Value to AND with to get sb msg #. */
#define  DTP_BDTS      0x80      /* Value to AND with to get brd or ts. */
#define  DTP_ISMB      0x40      /* Value to AND with to test for mb/sb. */

/*
 *-------These are values to AND with the actual values taken from the
 *	single byte port.
 */
#define  DTP_GTMSG     0x0E0     /* AND with real SB to get msg num. */
#define  DTP_SBTS      0x1F      /* AND with real SB to get ts num. */

/*
 *-------Host to Target requests (DTH_)
 */
/* 	The following abbreviations are used in the names of the messages: */
/*
 *	RD=Read, WRT=Write, DS=Disable, EN=Enable, RET=return, PD=Pulse dial
 *	RN=Run
 */
/* 	BD=For the entire board, TS=for a timeslot. */
/*
 *	MD=Mode, SG=Signaling, DAT=DATA, IN=Information, DN=Download, ID=Idle
 *	TST=Test, GLOB=Global parameter, MM=Message mask, ERRC=Error conditions
 *	TR=Transparent, INS=Insertion, ST=Status, DRP=Drop
 */

/*
 *-------Multibyte board level messages.
 */
#define  DTH_DATTST    (DT_BRDLEVEL + DT_MB + 0)  /* Perform datatest. */
#define  DTH_DATRSP    (DT_BRDLEVEL + DT_MB + 1)  /* Respond to target datatest. */
#define  DTH_STRTDN    (DT_BRDLEVEL + DT_MB + 2)  /* Start downloaded code. */
#define  DTH_WRTRAM    (DT_BRDLEVEL + DT_MB + 3)  /* Write to memory. */
#define  DTH_RDRAM     (DT_BRDLEVEL + DT_MB + 4)  /* Read from memory. */
#define  DTH_SETGLOB   (DT_BRDLEVEL + DT_MB + 5)  /* Set global parameter. */
#define  DTH_GETGLOB   (DT_BRDLEVEL + DT_MB + 6)  /* Get global parameter. */
#define  DTH_SETBDSG   (DT_BRDLEVEL + DT_MB + 7)  /* Set all signaling bits. */
#define  DTH_SETBDMD   (DT_BRDLEVEL + DT_MB + 8)  /* Set board mode. */
#define  DTH_DSBDDRP   (DT_BRDLEVEL + DT_MB + 9)  /* Disable all drop routing. */
#define  DTH_ENBDDRP   (DT_BRDLEVEL + DT_MB + 10)  /* Enable all drop routing. */
#define  DTH_DSTSDRP   (DT_BRDLEVEL + DT_MB + 11)  /* Dis. drop rout. for time slot. */
#define  DTH_ENTSDRP   (DT_BRDLEVEL + DT_MB + 12)  /* Ena. drop rout. for time slot. */
#define  DTH_DSBDMM    (DT_BRDLEVEL + DT_MB + 14)  /* Disable msgs via msg mask. */
#define  DTH_ENBDMM    (DT_BRDLEVEL + DT_MB + 15)  /* Enable msgs via msg. mask. */
#define  DTH_DSERRC    (DT_BRDLEVEL + DT_MB + 16)  /* Disable error conditions. */
#define  DTH_ENERRC    (DT_BRDLEVEL + DT_MB + 17)  /* Enable error conditions. */
#define  DTH_SETCLK    (DT_BRDLEVEL + DT_MB + 18)  /* Set the clock source. */
#define  DTH_SETWDOG   (DT_BRDLEVEL + DT_MB + 19)  /* Set up watchdog timer. */
#define  DTH_RDROM     (DT_BRDLEVEL + DT_MB + 20)  /* Read rom memory. */
#define  DTH_SETALRM   (DT_BRDLEVEL + DT_MB + 21)  /* Set alarm handling mode. */
#define  DTH_MAXMBBD   22        /* Max # of mb brd msgs. */

/*
 *-------Length of mb board level commands.  A one byte of data mb msg
 *	has a size of 3. (DTHL_)
 */
#define  DTHL_DATTST   0x0A
#define  DTHL_DATRSP   0x0A
#define  DTHL_STRTDN   0x04
#define  DTHL_WRTRAM   0x08      /* Length of write mem. struct - msg is larger. */
#define  DTHL_RDRAM    0x05
#define  DTHL_SETGLOB  0x05
#define  DTHL_GETGLOB  0x03
#define  DTHL_SETBDSG  0x08
#define  DTHL_SETBDMD  0x03
#define  DTHL_DSBDDRP  0x03
#define  DTHL_ENBDDRP  0x03
#define  DTHL_DSTSDRP  0x04
#define  DTHL_ENTSDRP  0x06
#define  DTHL_DSBDMM   0x04
#define  DTHL_ENBDMM   0x04
#define  DTHL_DSERRC   0x04
#define  DTHL_ENERRC   0x04
#define  DTHL_SETCLK   0x03
#define  DTHL_SETWDOG  0x0A
#define  DTHL_RDROM    0x05
#define  DTHL_SETALRM  0x03

/*
 *-------Multibyte timeslot level messages.
 */
#define  DTH_DSTSMM    (DT_TSLEVEL + DT_MB + 0)  /* Disable timeslot messages. */
#define  DTH_ENTSMM    (DT_TSLEVEL + DT_MB + 1)  /* Enable timeslot messages. */
#define  DTH_PDDIGS    (DT_TSLEVEL + DT_MB + 2)  /* Pulse dial digits. */
#define  DTH_MAXMBTS   3         /* Max # of mb ts msgs. */

/*
 *-------Size of mb timeslot level messages.
 */
#define  DTHL_DSTSMM   0x04
#define  DTHL_ENTSMM   0x04
#define  DTHL_PDDIGS   0x03

/*
 *-------Single byte, board level port 7 messages.
 */
#define  DTH_COMTST    (DT_BRDLEVEL + DT_SB7 + 0)  /* Perform comtest. */
#define  DTH_COMRSP    (DT_BRDLEVEL + DT_SB7 + 1)  /* Respond to target comtest. */
#define  DTH_GETBDST   (DT_BRDLEVEL + DT_SB7 + 2)  /* Get board status. */
#define  DTH_RNDIAG    (DT_BRDLEVEL + DT_SB7 + 3)  /* Run diagnostics. */
#define  DTH_STOPBD    (DT_BRDLEVEL + DT_SB7 + 4)  /* Stop the board. */
#define  DTH_GETDNIN   (DT_BRDLEVEL + DT_SB7 + 5)  /* Get down load info. */
#define  DTH_GETALRM   (DT_BRDLEVEL + DT_SB7 + 6)  /* Get alarm handling mode. */
#define  DTH_MAXSB7BD  7         /* Max # of sb p7 brd msgs. */

/*
 *-------Single byte, board level port 6 messages.
 */
#define  DTH_GETSGMD   (DT_BRDLEVEL + DT_SB6 + 0)  /* Get signaling mode. */
#define  DTH_SENDBLU   (DT_BRDLEVEL + DT_SB6 + 1)  /* Transmit blue alarm. */
#define  DTH_STOPBLU   (DT_BRDLEVEL + DT_SB6 + 2)  /* Stop transmitting blue alarm. */
#define  DTH_SENDYEL   (DT_BRDLEVEL + DT_SB6 + 3)  /* Transmit yellow alarm. */
#define  DTH_STOPYEL   (DT_BRDLEVEL + DT_SB6 + 4)  /* Stop xmitting yellow alarm. */
#define  DTH_GETBDSG   (DT_BRDLEVEL + DT_SB6 + 5)  /* Get signaling from all ts. */
#define  DTH_GETBDMD   (DT_BRDLEVEL + DT_SB6 + 7)  /* Get mode. */
#define  DTH_MAXSB6BD  8         /* Max # of sb p6 brd msgs. */

/*
 *-------Single byte, board level port 5 messages.
 */
#define  DTH_SETBDTR   (DT_BRDLEVEL + DT_SB5 + 0)  /* Set all ts to transparent sig. */
#define  DTH_SETBDINS  (DT_BRDLEVEL + DT_SB5 + 1)  /* Set all ts to insertion sig. */
#define  DTH_SETID7F   (DT_BRDLEVEL + DT_SB5 + 2)  /* Set idle to 7FH. */
#define  DTH_SETIDFF   (DT_BRDLEVEL + DT_SB5 + 3)  /* Set idle to FFH. */
#define  DTH_DSBDID    (DT_BRDLEVEL + DT_SB5 + 4)  /* Disable idle on all ts. */
#define  DTH_ENBDID    (DT_BRDLEVEL + DT_SB5 + 5)  /* Enable idle on all ts. */
#define  DTH_DSSNDINT  (DT_BRDLEVEL + DT_SB5 + 6)  /* Disable snd int to pc on rcv. */
#define  DTH_ENSNDINT  (DT_BRDLEVEL + DT_SB5 + 7)  /* Ensable snd int to pc on rcv. */
#define  DTH_MAXSB5BD  8         /* Max # of sb p5 brd msgs. */

/*
 *-------Single byte, timeslot level port 7 messages.
 */
#define  DTH_SETAOFF   (DT_TSLEVEL + DT_SB7 + 0)  /* Set A bit off. */
#define  DTH_SETAON    (DT_TSLEVEL + DT_SB7 + 1)  /* Set A bit on. */
#define  DTH_SETBOFF   (DT_TSLEVEL + DT_SB7 + 2)  /* Set B bit off. */
#define  DTH_SETBON    (DT_TSLEVEL + DT_SB7 + 3)  /* Set B bit on. */
#define  DTH_TONEOFF   (DT_TSLEVEL + DT_SB7 + 4)  /* Turn dig. milliwatt tone off. */
#define  DTH_TONEON    (DT_TSLEVEL + DT_SB7 + 5)  /* Turn dig. milliwatt tone on. */
#define  DTH_MAXSB7TS  6         /* Max # of sb p7 ts msgs. */

/*
 *-------Single byte, timeslot level port 6 messages.
 */
#define  DTH_DSPD      (DT_TSLEVEL + DT_SB6 + 0)  /* Disable pulse dialing. */
#define  DTH_ENPD      (DT_TSLEVEL + DT_SB6 + 1)  /* Enable pulse dialing. */
#define  DTH_GETMM     (DT_TSLEVEL + DT_SB6 + 3)  /* Get the message mask. */
#define  DTH_GETTSST   (DT_TSLEVEL + DT_SB6 + 4)  /* Get timeslot status. */
#define  DTH_GETTSSG   (DT_TSLEVEL + DT_SB6 + 5)  /* Get the timeslots sig bits. */
#define  DTH_MAXSB6TS  6         /* Max # of sb p6 ts msgs. */

/*
 *-------Single byte, timeslot level port 5 messages.
 */
#define  DTH_SETTSTR   (DT_TSLEVEL + DT_SB5 + 0)  /* Set ts to transparent sig. */
#define  DTH_SETTSINS  (DT_TSLEVEL + DT_SB5 + 1)  /* Set ts to insertion sig. */
#define  DTH_DSTSID    (DT_TSLEVEL + DT_SB5 + 2)  /* Disable idle on timeslot. */
#define  DTH_ENTSID    (DT_TSLEVEL + DT_SB5 + 3)  /* Enable idle on timeslot. */
#define  DTH_MAXSB5TS  4         /* Max # of sb p5 ts msgs. */

/*
 *-------Target to Host Requests (DTT_).
 */

/*
 *-------Multibyte, board level messages.
 */
#define  DTT_DATRSP    (DT_BRDLEVEL + DT_MB + 0)  /* Respond to host datatest. */
#define  DTT_DATTST    (DT_BRDLEVEL + DT_MB + 1)  /* Perform datatest. */
#define  DTT_RETBDST   (DT_BRDLEVEL + DT_MB + 2)  /* Return board status. */
#define  DTT_RETDIAG   (DT_BRDLEVEL + DT_MB + 3)  /* Return diagnostic result. */
#define  DTT_RETRAM    (DT_BRDLEVEL + DT_MB + 4)  /* Return external memory. */
#define  DTT_RETSG     (DT_BRDLEVEL + DT_MB + 5)  /* Return signaling bits. */
#define  DTT_RETGLOB   (DT_BRDLEVEL + DT_MB + 6)  /* Return global parameter. */
#define  DTT_RETBDMD   (DT_BRDLEVEL + DT_MB + 7)  /* Return mode. */
#define  DTT_T1ERRC    (DT_BRDLEVEL + DT_MB + 8)  /* T1 error cond. occurred. */
#define  DTT_BRDERR    (DT_BRDLEVEL + DT_MB + 9)  /* Board level error occurred. */
#define  DTT_RETDNIN   (DT_BRDLEVEL + DT_MB + 11)  /* Return download info. */
#define  DTT_RETSGMD   (DT_BRDLEVEL + DT_MB + 12)  /* Return signaling mode. */
#define  DTT_RETROM    (DT_BRDLEVEL + DT_MB + 13)  /* Return rom read. */
#define  DTT_RETALRM   (DT_BRDLEVEL + DT_MB + 14)  /* Return alarm handling mode. */
#define  DTT_MAXMBBD   16        /* Max # of mb brd msgs. */


/*
 *-------Sizes of mb board level messages.
 *	A one byte of data mb msg's size = 3 (DTTL_).
 */
#define  DTTL_DATRSP   0x0A
#define  DTTL_DATTST   0x0A
#define  DTTL_RETBDST  0x0B
#define  DTTL_RETDIAG  0x03
#define  DTTL_RETRAM   0x03
#define  DTTL_RETSG    0x0E
#define  DTTL_RETGLOB  0x05
#define  DTTL_T1ERRC   0x03
#define  DTTL_RETBDMD  0x03
#define  DTTL_BRDERR   0x03
#define  DTTL_RETDNIN  0x06
#define  DTTL_RETSGMD  0x05
#define  DTTL_RETROM   0x03
#define  DTTL_RETALRM  0x03

/*
 *-------Multibyte, timeslot level messages.
 */
#define  DTT_TSERR     (DT_TSLEVEL + DT_MB + 0)  /* Timeslot error occured. */
#define  DTT_RCVPDG    (DT_TSLEVEL + DT_MB + 1)  /* Pulse digit received. */
#define  DTT_RETMM     (DT_TSLEVEL + DT_MB + 3)  /* Return message mask. */
#define  DTT_RETTSST   (DT_TSLEVEL + DT_MB + 4)  /* Return timeslot status. */
#define  DTT_RETTSSG   (DT_TSLEVEL + DT_MB + 5)  /* Return timeslot sig bits. */
#define  DTT_MAXMBTS   6         /* Max # of mb ts msgs. */

/*
 *-------Sizes of mb timeslot level messages.
 *	A one byte of data mb msg's size = 3.
 */
#define  DTTL_TSERR    0x03
#define  DTTL_RCVPDG   0x03
#define  DTTL_RETMM    0x04
#define  DTTL_RETTSST  0x03
#define  DTTL_RETTSSG  0x03


/*
 *-------Single byte, board level port 7 messages.
 */
#define  DTT_COMRSP    (DT_BRDLEVEL + DT_SB7 + 0)  /* Respond to comtest. */
#define  DTT_COMTST    (DT_BRDLEVEL + DT_SB7 + 1)  /* Perform comtest. */
#define  DTT_REMLPBK   (DT_BRDLEVEL + DT_SB7 + 2)  /* Went into remote loopback. */
#define  DTT_OVRFLW    (DT_BRDLEVEL + DT_SB7 + 3)  /* Overflow of buffer. */
#define  DTT_RLPBKOK   (DT_BRDLEVEL + DT_SB7 + 4)  /* Came out of remote loopback. */
#define  DTT_MAXSB7BD  5         /* Max # of sb brd p7 msgs. */

/*
 *-------Single byte, timeslot level port 7 messages.
 */
#define  DTT_AOFF      (DT_TSLEVEL + DT_SB7 + 0)  /* Bit A went off (to 0). */
#define  DTT_AON       (DT_TSLEVEL + DT_SB7 + 1)  /* Bit A went on (to 1). */
#define  DTT_BOFF      (DT_TSLEVEL + DT_SB7 + 2)  /* Bit B went off (to 0). */
#define  DTT_BON       (DT_TSLEVEL + DT_SB7 + 3)  /* Bit B went on (to 1). */
#define  DTT_WINK      (DT_TSLEVEL + DT_SB7 + 4)  /* Wink occurred. */
#define  DTT_MAXSB7TS  5         /* Max # of sb ts p7 msgs. */

/*
 *-------Single byte, timeslot level port 6 messages.
 */
#define  DTT_PDDONE    (DT_TSLEVEL + DT_SB6 + 2)  /* Done pulse dialing. */
#define  DTT_MAXSB6TS  3         /* Max # of sb ts p6 msgs. */

/*
 *--------Timeslot status.
 */
#define  DTV_OK        0x00      /* Timeslot is not busy. */
#define  DTV_PD        0x01      /* Timeslot is pulse dialing. */


/*
 *--------Termination response codes.
 */
#define  DTR_ABT       0x0FE     /* Abort termination. */
#define  DTR_TERM      0x0FF     /* Normal termination. */

/*
 *-------Board status masks.
 */
#define  DTS_CLK1      0x08      /* Hi bit of clock src value. */
#define  DTS_CLK0      0x10      /* Low of clock src value. */
#define  DTS_RLPBKSW   0x20      /* Switch state-remote(NOTE:0=YES, 1=NO). */

/*
 *--------Board type.
 */
#define  BDDTI         3         /* 3=DTI. */
#define  BDTYPE        BDDTI
