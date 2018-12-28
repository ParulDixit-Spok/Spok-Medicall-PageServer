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
 * Filename:    dticas.h 
 * DESCRIPTION: DTI CAS template definitions..   
 *****************************************************************************/

/*****************************************************************************
 *	REVISION HISTORY:
 *	 Who			  Version		When		What
 *	 Cris Ivan		      		1997    Initial
 *  Kamil Shahab              2000    Added support for 3 new commands
 *  BJ                        2003    Replace the error code defines (PT 30735)
 *	NP						  2003    Add error defines for backward compat.
 *****************************************************************************/

#ifndef __DTICAS
#define __DTICAS

typedef struct t_create_transition_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	BYTE PreTransCode;
	BYTE PostTransCode;
	WORD PreTransInterval;
	WORD PreTransIntervalNom; /* [ci]+ send template adition */
	WORD PostTransInterval;
	WORD PostTransIntervalNom; /* [ci]+ send template adition */
} DTCAS_CREATE_TRANSITION_MSG;

typedef DTCAS_CREATE_TRANSITION_MSG *P_DTCAS_CREATE_TRANSITION_MSG;

typedef struct t_create_pulse_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	BYTE OffPulseCode;
	BYTE OnPulseCode;
	WORD PrePulseInterval;
	WORD PrePulseIntervalNom; /* [ci]+ send template adition */
	WORD PulseIntervalMin;
	WORD PulseIntervalNom;
	WORD PulseIntervalMax;
	WORD PostPulseInterval;
	WORD PostPulseIntervalNom; /* [ci]+ send template adition */
} DTCAS_CREATE_PULSE_MSG;

typedef DTCAS_CREATE_PULSE_MSG *P_DTCAS_CREATE_PULSE_MSG;

typedef struct t_create_train_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	BYTE OffPulseCode;
	BYTE OnPulseCode;
	WORD PreTrainInterval;
	WORD PreTrainIntervalNom; /* [ci]+ send template adition */
	WORD PulseIntervalMin;
	WORD PulseIntervalNom;
	WORD PulseIntervalMax;
	WORD InterPulseIntervalMin;
	WORD InterPulseIntervalNom;
	WORD InterPulseIntervalMax;
	WORD PostTrainInterval;
	WORD PostTrainIntervalNom; /* [ci]+ send template adition */
} DTCAS_CREATE_TRAIN_MSG;

typedef DTCAS_CREATE_TRAIN_MSG *P_DTCAS_CREATE_TRAIN_MSG;

typedef struct t_create_reply_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	WORD result;
} DTCAS_REPLY_MSG;

typedef DTCAS_REPLY_MSG *P_DTCAS_REPLY_MSG;

typedef struct t_delete_template_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
} DTCAS_DELETE_TEMPLATE_MSG;

typedef DTCAS_DELETE_TEMPLATE_MSG *P_DTCAS_DELETE_TEMPLATE_MSG;

typedef struct t_ena_dis_template_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
} DTCAS_ENA_DIS_TEMPLATE_MSG;

typedef DTCAS_ENA_DIS_TEMPLATE_MSG *P_DTCAS_ENA_DIS_TEMPLATE_MSG;

/* Kamil Shahab(11/22/1999): Additional Structure defined to add new commands to CAS 
   management function */
typedef struct t_get_template_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
} DTCAS_GET_TEMPLATE_MSG;

typedef DTCAS_GET_TEMPLATE_MSG *P_DTCAS_GET_TEMPLATE_MSG;

typedef struct t_get_next_template_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
} DTCAS_GET_NEXT_TEMPLATE_MSG;

typedef DTCAS_GET_NEXT_TEMPLATE_MSG *P_DTCAS_GET_NEXT_TEMPLATE_MSG;

typedef struct t_clear_all_template_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
} DTCAS_CLEAR_ALL_TEMPLATE_MSG;

typedef DTCAS_CLEAR_ALL_TEMPLATE_MSG *P_DTCAS_CLEAR_ALL_TEMPLATE_MSG;

typedef struct t_get_template_reply_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	WORD result;
	union template_reply_msg {
		DTCAS_CREATE_TRANSITION_MSG   transition;
		DTCAS_CREATE_PULSE_MSG        pulse;
		DTCAS_CREATE_TRAIN_MSG        train;
    } tmplt_info;
} DTCAS_GET_TEMPLATE_REPLY_MSG;

typedef DTCAS_GET_TEMPLATE_REPLY_MSG *P_DTCAS_GET_TEMPLATE_REPLY_MSG;
/* End addition dated 1/27/2000 */

typedef struct t_detected_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	WORD result;
	WORD count;
	DWORD timestamp;
} DTCAS_DETECTED_MSG;

typedef DTCAS_DETECTED_MSG *P_DTCAS_DETECTED_MSG;

/* [ci] send template addition */
typedef struct t_transmit_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	BYTE pulse_count;
	BYTE sequence_count;
} DTCAS_TRANSMIT_MSG;

typedef DTCAS_TRANSMIT_MSG *P_DTCAS_TRANSMIT_MSG;

typedef struct t_end_transmit_msg
{
	BYTE msg_code;
	BYTE rfu;
	WORD template_id;
	BYTE pulse_count;
	BYTE sequence_count;
	DWORD timestamp;
} DTCAS_END_TRANSMIT_MSG;

typedef DTCAS_END_TRANSMIT_MSG *P_DTCAS_END_TRANSMIT_MSG;
/* [ci] send template addition */

/* Message code definitions. */

/* Commands: */
#define	DTCAS_CREATE_TRANSITION     0
#define	DTCAS_CREATE_PULSE          1
#define	DTCAS_CREATE_TRAIN          2
#define	DTCAS_DELETE_TEMPLATE       3
#define	DTCAS_ENABLE_TEMPLATE       4
#define	DTCAS_DISABLE_TEMPLATE      5
#define	DTCAS_TRANSMIT_TEMPLATE     6
#define	DTCAS_GET_TEMPLATE          7   /* Get a template definition (1/27/2000) */
#define	DTCAS_GET_NEXT_TEMPLATE     8   /* Get a definition of next template (1/27/2000) */
#define	DTCAS_CLEAR_ALL_TEMPLATE    9   /* Clear the template queue (1/27/2000) */

/* Replies: */
#define	DTCAS_CREATE_TRANSITION_COMPLETE    0
#define	DTCAS_CREATE_PULSE_COMPLETE         1
#define	DTCAS_CREATE_TRAIN_COMPLETE         2
#define	DTCAS_DELETE_TEMPLATE_COMPLETE      3
#define	DTCAS_ENABLE_TEMPLATE_COMPLETE      4
#define	DTCAS_DISABLE_TEMPLATE_COMPLETE     5
#define	DTCAS_TRANSMIT_TEMPLATE_COMPLETE    6
#define	DTCAS_GET_TEMPLATE_COMPLETE         7   /* Template definition received (1/27/2000) */
#define	DTCAS_GET_NEXT_TEMPLATE_COMPLETE    8   /* Definition of next template received (1/27/2000) */
#define	DTCAS_CLEAR_ALL_TEMPLATE_COMPLETE   9   /* Clearing template queue completed (1/27/2000) */

/*
 * Note: The result codes 1-15 are not implemented yet and are reserved for
 *       future use.
 */

/* Result Codes: */
#define DTCAS_ERR_SIGNAL_ID					DTCAS_ERR_DUPLICATETEMPLATE /* Identical template creation requested */
#define DTCAS_ERR_SIGNAL_TABLE_FULL			DTCAS_ERR_TEMPLATETABLEFULL  /* The signal definition table is full */
#define DTCAS_ERR_POSTINT_LTE_ITIMX			0x25 /* The Post Interval Nom  <= Inter-Train Interval Max		*/
#define DTCAS_ERR_ITIMX_LTE_IPIMX			0x26 /* The Inter-Train Max is <= Inter-Pulse Max		*/
#define DTCAS_ERR_INT_TRAIN_MIN				0x27 /* The Inter-Train Min Time cannot be 0	*/
#define DTCAS_ERR_ITIMN_GTE_ITINM			0x28 /* The Inter-Train Min is >= Inter-Train Nom*/
#define DTCAS_ERR_ITINM_GTE_ITIMX			0x29 /* The Inter-Train Nom is >= Inter-Train Max*/
#define DTCAS_ERR_INT_PULSE_MIN				0x30 /* The Inter-Pulse Min Time cannot be 0*/
#define DTCAS_ERR_IPIMN_GTE_IPINM			0x31 /* The Inter-Pulse Min is >= Inter-Pulse Nom*/
#define DTCAS_ERR_IPINM_GTE_IPIMX			0x32 /* The Inter-Pulse Nom is >= Inter-Pulse Max*/
#define DTCAS_ERR_PULSE_MIN					0x33 /* The Pulse Min Time cannot be 0		*/
#define DTCAS_ERR_PIMN_GTE_PINM				0x34 /* The Pulse Min is >= Pulse Nom			*/
#define DTCAS_ERR_PINM_GTE_PIMX				0x35 /* The Pulse Nom is >= Pulse Max			*/
#define DTCAS_ERR_PREI_LTE_PREINM			0x36 /* The Pre Interval time (min) >= Pre Interval time nom	*/
#define DTCAS_ERR_POSTI_LTE_POSTINM			0x37 /* The Post Interval time (min) >= Post Interval time nom */
#define DTCAS_ERR_TEMPLATE_TABLE_EMPTY		0x38 /* The Template table is empty - no templates defined		*/
#define DTCAS_ERR_EMPTY_SIGPAT_TABLE		0x39 /* Applies to a get next template command	*/
#define DTCAS_ERR_END_TMPL_TABLE			0x17/* Next template was not found (1/27/2000) */
#define DTCAS_ERR_SIGNAL_ENABLED			0x0f  /* The signal is already enabled. */
#define DTCAS_ERR_SIGNAL_NOT_ENABLED		0x3a	/* Attempt to disable a signal pattern that's not enabled*/
#define DTCAS_ERR_TEMPLATE_NOT_DEFINED		0x3b	/* Template was not defined		*/
#define DTCAS_ERR_POSTINT_LTE_IPIMX			0x3c	/* The Train Post-Interval is <= the inter pulse interval*/
#define DTCAS_ERR_INVALID_TEMPLATE_REQ		0x3d	/* Template to create is not a Seq, Train, Pulse, Transit*/
#define DTCAS_ERR_PULSE_MIN_EQ_ZERO			DTCAS_ERR_PULSE_MIN 
#define DTCAS_ERR_PULSE_NOM_EQ_ZERO			0x3f	/* The Pulse time nom is zero		*/
#define DTCAS_ERR_PULSE_MAX_EQ_ZERO			0x4a	/* The Pulse time max is zero		*/
#define DTCAS_ERR_INTR_PULSE_MIN_EQ_ZERO	DTCAS_ERR_INT_PULSE_MIN /* The Inter-Pulse-interval min time is zero*/
#define DTCAS_ERR_INTR_PULSE_NOM_EQ_ZERO	0x4c	/* The Inter-Pulse-interval nom time is zero*/
#define DTCAS_ERR_INTR_PULSE_MAX_EQ_ZERO	0x4d	/* The Inter-Pulse-interval max time is zero	*/
#define DTCAS_ERR_INTR_TRAIN_MIN_EQ_ZERO	DTCAS_ERR_INT_TRAIN_MIN	 /* The Inter-Train-interval min time is zero*/
#define DTCAS_ERR_INTR_TRAIN_NOM_EQ_ZERO	0x4f	/* The Inter-Train-interval min time is zero */
#define DTCAS_ERR_INTR_TRAIN_MAX_EQ_ZERO	0x5a	/* The Inter-Train-interval min time is zero*/
#define DTCAS_ERR_PRE_INTRV_EQ_ZERO			0x5b  /* The Pre template time (really a min) is zero	*/
#define DTCAS_ERR_PRE_INTRV_NOM_EQ_ZERO		0x5c	/* The Pre template nom time is zero		*/
#define DTCAS_ERR_TEMPLATESENDNOTDONE   	0x16  /* Sending of a template is not finished yet */
#define	DTCAS_ERR_INVALIDSUBCOMMAND			0x12	/* The subcommand is invalid */

/*
 *------------------------------------------------------------------------------------------------- 
 * The following error codes are included for backwards compatabilty (SR 5.1.1 FP1 and earlier.
 * Application developers should upgrade their applications to utilize the new result codes 
 * defined above.
 *-------------------------------------------------------------------------------------------------
 */
#define	DTCAS_OK_DUPLICATETEMPLATE     	 0   /* Identical template creation requested */
#define	DTCAS_ERR_INTPULSEINTMIN         1  /* The minimum inter-pulse interval is invalid */
#define	DTCAS_ERR_INTPULSEINTMAX         2  /* The maximum inter-pulse interval is invalid */
#define	DTCAS_ERR_INTPULSEINTNOM         3  /* The nominal inter-pulse interval is invalid */
#define	DTCAS_ERR_POSTPULSEINT           4  /* The Post-Pulse interval is invalid */
#define	DTCAS_ERR_POSTTRAININT           5  /* The Post-Train interval is invalid */
#define	DTCAS_ERR_POSTTRANSCODE          6  /* The Post-Transition Code is invalid */
#define	DTCAS_ERR_POSTTRANSINT           7  /* The Post-Transition interval is out-of-range */
#define	DTCAS_ERR_PULSEINTMAX            8  /* The maximum pulse interval is invalid */
#define	DTCAS_ERR_PULSEINTMIN            9  /* The minimum pulse interval is invalid */
#define	DTCAS_ERR_PULSEINTNOM           10  /* The nominal pulse interval is invalid */
#define	DTCAS_ERR_PREPULSEINT           11  /* The Pre-Pulse interval is invalid */
#define	DTCAS_ERR_PRETRAININT           12  /* The Pre-Train interval is invalid. */
#define	DTCAS_ERR_PRETRANSCODE          13  /* The Pre-Transition Code is invalid */
#define	DTCAS_ERR_PRETRANSINT           14  /* The Pre-Transition interval is out-of-range */
#define	DTCAS_ERR_SIGNALENABLED         15  /* The signal is already enabled. */
#define	DTCAS_ERR_DUPLICATETEMPLATE     16  /* The duplicate template in table */
#define	DTCAS_ERR_TEMPLATETABLEFULL     17  /* The signal definition table is full */
#define	DTCAS_ERR_TEMPLATENOTFOUND      19  /* The template was not found */
#define DTCAS_ERR_TEMPLATESENDSTOP      21  /* Signaling send in a template send was stopped */
#define	DTCAS_ERR_NEXT_TEMPLATENOTFOUND 23  /* Next template was not found (1/27/2000) */

#endif /* __DTICAS */
