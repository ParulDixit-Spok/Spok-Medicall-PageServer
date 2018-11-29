/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 1993-2007 Dialogic Corporation. All Rights Reserved.
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
/**********************************************************************
*
*   C Header:       pdk_log.h
*   Instance:       2
*   Description:    
*   %date_created:  Tue Apr 09 13:23:22 2002 %
*
**********************************************************************/

#ifndef _2_pdk_log_h_H
#define _2_pdk_log_h_H

////////////////////////////////////////////////////////////////////////////////////////
//
// The following data strutures are used for GC_Start function or  
//
/////////////////////////////////////////////////////////////////////////////////////////

///
// data structire for PDK_START_STRUCT
//
typedef struct
{
    char    *   cclib_name;
    char    *   start_parameters;
} PDK_START_STRUCT, *PDK_START_STRUCTP;

///
// Enumerate all enable flags for different log levels
// The enable flag implies including flags with smaller values
// e.g., ENABLE_INFO  = enable LOGLEVEL_INFO and LOGLEVEL_DEBUG 
//

typedef enum
{
    ENABLE_DEBUG    =   1,      // DEBUG level and above
    ENABLE_INFO     =   2,      // INFO level and above
    ENABLE_ALERT    =   3,      // ALERT level and above
    ENABLE_WARNING  =   4,      // WARNING level and above
    ENABLE_FATAL    =   5      // FATAL level
} LogLevel_Enable;


///
// Enumerate all enable flags for different services (32 bits) 
//
typedef enum
{
    ALL_SERVICES    =   0xFFFFFFFF,     // all services enabled
    USRAPP_ENABLE   =   0x00000001,     // User appliction
    GCAPI_ENABLE    =   0x00000002,     // GC API (function name and events)
    GCXLTR_ENABLE   =   0x00000004,     // GC translation
    LINEADMIN_ENABLE=   0x00000008,     // Line administrator
    CHANNEL_ENABLE  =   0x00000010,     // Channel
    LOADER_ENABLE   =   0x00000020,     // GC loader
    CALL_ENABLE     =   0x00000040,     // Call
    R2MF_ENABLE     =   0x00000080,     // R2fm service
    TONE_ENABLE     =   0x00000100,     // Tone service
    CAS_ENABLE      =   0x00000200,     // Cas service
    TIMER_ENABLE    =   0x00000400,     // Timer service
    SDL_ENABLE      =   0x00000800,     // SDL
    SRL_ENABLE      =   0x00001000,     // SRL 
    ERRHNDLR_ENABLE =   0x00002000,     // Error handler
    LOGGER_ENABLE   =   0x00004000,     // Logger
    RTCM_ENABLE     =   0x00008000,     // RTCM 
    GCLIB_ENABLE    =   0x00010000,     // GCLIB
    IPCCLIB_ENABLE  =   0x00020000      // IP H.323 CCLIB
} Service_Enable;

typedef enum
{
    ON_FATAL    =   0x0000,     // Dump the cache to log file when there is a FATAL record 
    WHEN_FULL   =   0x0001,     // Dump the cache to log file when the cache size is full
    THREAD_OFF  =   0x0000,     // Not create a dump thread to do dumping
    THREAD_ON   =   0x0002      // Create a dump thread to do dumping
} CacheDump_Masks;


////////////////////////////////////////////////////////////////////////////////////////
//
// The following data strutures are used for GC_PDK_Extension 
//
/////////////////////////////////////////////////////////////////////////////////////////

///
// Enumerate all function number used in PDK_XTEN_LOG_FUNC
//
typedef enum
{ 
        PDK_FUNC_LOG        =   1,
        PDK_FUNC_DUMPLOG    =   2,
        PDK_EXT_FUNC_LOG    =   3
} PDK_XTEN_FUNCNUM;


// Enumerate all logging levels used in PDK_XTEN_LOG_FUNC. Each log record is grouped or
// classified under one of these log levels.
typedef enum 
{
        PDK_LOGLEVEL_DEBUG  =   1,      // DEBUG level
        PDK_LOGLEVEL_INFO   =   2,      // INFO level
        PDK_LOGLEVEL_ALERT  =   3,      // ALERT level
        PDK_LOGLEVEL_WARNING=   4,      // WARNING level
        PDK_LOGLEVEL_FATAL  =   5       // FATAL level
}PDK_LOG_LEVEL;

// Enumerate all services used in PDK_XTEN_LOG_FUNC.  Each log record must be grouped or
// classified under one of these services.
typedef enum 
{ 
        PDK_SERVICE_USRAPP  =   1,      // USRAPP service
        PDK_SERVICE_GCAPI   =   2       // FCAPI service
}PDK_SERVICE;

///
// Data structure for PDK_XTEN_LOG_FUNC
// The board_num and channel_num fields are only used by non-PDK protocols. They
// are not mandatory, but can be used for indicating the board and channel #s.
//
typedef struct
{
    PDK_XTEN_FUNCNUM    func_no;    // Function number (defined in PDK_XTEN_FUNCNUM)
    char *              log_data;   // The data to log
    PDK_LOG_LEVEL       log_level;  // Log level
    PDK_SERVICE         service;    // Service id
    char *              file_name;  // File name
    long                line_num;   // Line number
    long                board_num;   // Board # - only used by non-PDK protocols 
    long                channel_num; // Channel # - only used by non-PDK protocols 
} PDK_XTEN_LOG_FUNC;


#endif 
