/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2000-2007 Dialogic Corporation. All Rights Reserved.
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

#include <stdio.h> 

/* Maximum number of tones defined in a tone set */
#define MAX_TONES               10
#define MAX_DNLD_TONESETS       10

/***************************************
 * TSF_KEY - key for Toneset in open file
 ***************************************/
typedef int TSF_KEY;

/***************************************
 * TSF_RET - Return code for all TSFIO APIs
 ***************************************/
typedef int TSF_RET;


/*TSFIO error codes*/
#define TSF_SUCCESS			0
#define TSF_FAILURE			1
#define TSF_INVALID_KEY			2
#define TSF_INVALID_FILETYPE		3
#define TSF_FEATURE_DISABLED		4
#define TSF_FILE_EXISTS			5
#define TSF_NO_MEM			6
#define TSF_TONESET_DELETED		7
#define TSF_INVALID_FILENAME		8
#define TSF_FILE_NOT_CLOSED		9
#define TSF_READONLY			10
#define TSF_ACTIVATION_FAIL		11
#define TSF_INVALID_PARAMETER		12
#define TSF_FILE_NOT_OPENED		13
#define TSF_FILE_NOT_CONSOLIDATED   	14
#define TSF_OVERFLOW_KEYLIST		15
 
/* Return codes for consolidation operation*/
# define TSF_CONSOLIDATION_SUCCESS          TSF_SUCCESS /* The consolidation was successful and written to the disk */

# define TSF_CONSOLIDATION_TOO_MANY		21 /* More than 10 PBX's were chosen for build */

# define TSF_CONSOLIDATION_OVERLAP		22 /* Cannot resolve a conflict between ringback and busy templates 								given the list of PBX's to consolidate a template for. */

# define TSF_CONSOLIDATION_QUAL_TEMP		23 /* Across a given template definition for the set of PBX's, there is
						      not a common qualification template id. */

# define TSF_CONSOLIDATION_SINGLE_DUAL		24 /* Across a given template definition for the set of PBX's, there are
						      both single and dual tone definitions */

# define TSF_CONSOLIDATION_FREQ_TOO_WIDE	25 /* The deviation of the frequency component of a template is over 
						      COMP_MAX_FREQ_DEV. */

# define TSF_CONSOLIDATION_INVALID_DTONE	26 /* A dial tone definition in the list of PBX's is not valid.  It contains
						      a cadence definition.  This excludes a debounce 
						      (0 offtime and negative ontimedeviation).*/

# define TSF_CONSOLIDATION_GENERAL		27 /* This error will be returned if a binary file API function failed */

# define TSF_CONSOLIDATION_NO_DATA		28 /* A consolidation was requested and there are no entries in the build list */

# define TSF_CONSOLIDATION_BLANK_ENTRY		29 /* A toneset in the build list has no toneset data.  An entry has 0's for
						      freq1 and freq2 tone definitions */

# define TSF_CONSOLIDATION_CAD_CONT_MIX		30 /*Failed to consolidate a set of template definitions that contain a mix
						     of cadenced and non cadenced definitions.*/

#define TSF_CONSOLIDATION_DEFAULTS_NOT_SET	31 /* Default tone information requested to include in consolidation process
							 but not set */

#define TSF_CONSOLIDATION_DEFAULTS_NOT_REQUESTED	32 /* Default tone information are set but not requested to Include
							      in compilation clear those */


/* Define data structure used to provide the ability to pass parameters
   to the consolidate function.  Currently contains only one field. */
typedef struct
{
   int dial_tone_id;
   int exclude_default_defs; /* Use the macros TRUE/FALSE */
} TSF_CONSOLIDATIONOPTIONS;


/****************************************
 * TSF file Header information
 ****************************************/

typedef struct 
{
    unsigned short  consolidateflag; /* TRUE/FALSE - If file contains data   */
    unsigned short  tonesetcount;   /* Total # of TONESET entries           */
    unsigned short  maxtones;       /* Total # of TONE_INFO entries per ts  */
    unsigned short  dnldtscount;    /* Total # of DNLD_TSET entries         */
    unsigned short  gtdtscount;     /* Provisional                          */
    unsigned short  tonesetsize;    /* Size of TONESET structure            */
    unsigned short  dnldsize;       /* Size of Download Toneset structure   */
    unsigned short  gtdtssize;      /* Provisional                          */
    unsigned short  tonesetsinconsolidation;/* Total # of tonesets in build         */
    unsigned char   tid_dt;         /* Tone ID in build for Dialtone        */
} TSF_FILE_INFO;


/****************************************
 * TONESET_ID Structure
 ****************************************/
#define TONESET_STRING_SIZE 64

typedef struct 
{
    unsigned char   toneset_name  [TONESET_STRING_SIZE]; /* Manufacturer of name     */
    unsigned char   toneset_model [TONESET_STRING_SIZE]; /* Model of name            */
    unsigned short  isConsolidated;            /* Included in build flag   */

}TONESET_ID;

/*Tone information structure*/
typedef struct
{
    unsigned short  tone_id;        /* Tone ID                         */
    TN_DESC         tn_desc;        /* Tone description structure      */
    TN_AMP          tn_amp;         /* Tone amplitude structure        */

} TONE_INFO;
/*
 * Tone information for all MAX_TONES
 */
typedef struct TONESETINFO
{
	TONE_INFO toneinfo[MAX_TONES];
}TONESETINFO;

#ifdef WIN32  /*For windows*/

#ifdef LIBTSFIO_EXPORTS
#define LIBTSFIOAPI __declspec(dllexport)
#else
#define LIBTSFIOAPI   __declspec( dllimport )
#endif

#define TSFIO_CONV   __cdecl

#else    /*for Linux*/

#define LIBTSFIOAPI  extern
#define TSFIO_CONV
#define BOOL int 

#endif  /*end ifdef WIN32*/

#ifdef __cplusplus
extern "C" {
#define extern
#endif


LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_OpenFile         ( char *filename, TSF_FILE_INFO *pfile_info, unsigned short rfu);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_SaveFile         ( char *filename, unsigned short overwriteflag);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_ActivateFile (char *filename);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_CloseFile        ( void );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetFileInformation      ( TSF_FILE_INFO *pfile_info );

LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_AddToneSet       ( TSF_KEY *pkey, TONESET_ID *ptonesetid, TONESETINFO *ptonesetinfo );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_DeleteToneSet    ( TSF_KEY key);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetToneSet   ( TSF_KEY key, TONESETINFO *ptonesetinfo );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetToneSetName   (TSF_KEY key, TONESET_ID *ptonesetid );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_ModifyToneSet    ( TSF_KEY key, TONESET_ID *ptonesetid, TONESETINFO *ptonesetinfo );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_DuplicateToneSet ( TSF_KEY key, TONESET_ID *ptonesetid, TSF_KEY *pduplicatedkey );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetNumberOfToneSets(unsigned short *numoftonesets);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetToneSetKeys ( TSF_KEY *keylist, unsigned short *sizeoflist );

LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_ConsolidateToneSets (TSF_CONSOLIDATIONOPTIONS *consops, TSF_KEY *keylist, unsigned int listsize);
LIBTSFIOAPI void TSFIO_CONV  tsf_ClearCosolidationOptions(TSF_CONSOLIDATIONOPTIONS *consops);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetConsolidatedToneSet  ( TONESETINFO *ptonesetinfo );
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_ClearDefaultToneSetToConsolidate(void);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_SetDefaultToneSetToConsolidate(TONESETINFO *ptoneinfoset);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetDefaultToneSetInConsolidation(TONESETINFO *ptoneinfoset);
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_GetNumberOfToneSetsConsolidated(unsigned short *pnumoftonesets);
LIBTSFIOAPI TSF_RET  TSFIO_CONV tsf_GetConsolidatedToneSetKeys ( TSF_KEY *pkeylist, unsigned short *sizeoflist );

/*
Here TSF file is the tsf file set in DCM by user or Activeated tsf file by tsf_ActivateFile().
 The API takes care of initializing and exiting the library and opening and closing tsf file.
*/
LIBTSFIOAPI TSF_RET TSFIO_CONV tsf_ReadTSFInformation( TONESETINFO *ptoneinfoset, BOOL *iIsDisconnectEnabled, DX_CAP *lpDefaultCAP);

#pragma pack ( )

#ifdef __cplusplus
}
#undef extern
#endif
