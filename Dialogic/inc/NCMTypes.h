/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2000-2008 Dialogic Corporation. All Rights Reserved.
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

#ifndef NCMTYPES_H
#define NCMTYPES_H

#include <wtypes.h>

#define NCM_MAX_EXT_ERR_SIZE 500
#define PROTOCOL_LENGTH 25
#define MEDIA_LOAD_LENGTH 25
#define PATH_MAX 260
#define MAXTRUNKS 50

//Used to return the character for the protocol group
static char *ProtocolGroupNumbers[] =
    { "", "1", "2", "3", "4", "5", "6"};

/* Protocol Grouping definition. */
typedef enum 
{
    NCMPROTOCOL_GROUP1 = 1,
    NCMPROTOCOL_GROUP2 = 2,
    NCMPROTOCOL_GROUP3 = 3,
    NCMPROTOCOL_GROUP4 = 4,
    NCMPROTOCOL_GROUP5 = 5,
    NCMPROTOCOL_GROUP6 = 6
}NCMPROTOCOLGROUP_E;


// 
typedef struct NCMString
{
	char              *name;
    struct NCMString  *next;
} NCMString; 

typedef NCMString NCMFamily, NCMDevice, NCMProperty, NCMVariable, 
		NCMValue, NCMErrorMsg;

typedef struct _NCMFeatureType {
	char MediaLoad[MEDIA_LOAD_LENGTH];
}NCMFeatureType;

/* Protocol and the protocol group. */
typedef struct _NCMTRUNKPROTOCOL_S
{
    char                protocolSetting[PROTOCOL_LENGTH];
    NCMPROTOCOLGROUP_E     protgrp;
    char                trunktype[PROTOCOL_LENGTH];
}NCMTRUNKPROTOCOL_S;


//
//typedef int NCMRetCode;	// 0 (success);  non-zero(failure) === error code
// -- OR --
typedef enum			// No error code; therefore, we need a mechanism for more details
{
	NCM_SUCCESS=0,
	NCME_GENERAL,				// General error
	NCME_DUP_DEVICE,			// Attempt to add a duplicate device
	NCME_INVALID_BUFF,			// Received an invalid buffer
	NCME_SETTING_DEFAULTS,		// Error occurred while setting default values
	NCME_BAD_DATA_TYPE,			// Data type of variable is incorrect or indeterminable
	NCME_BAD_DATA_LOC,			// Destination of data (i.e., global, 
								// family, or device level) could not 
								//		be determined
	NCME_DATA_NOT_FOUND,		// Data not found
	NCME_GLOAD_LIB,				// Error loading GENLOAD library
	NCME_DM3_LIB,				// Error loading DM3 library
	NCME_REG_CALLBK,			// Error registering Call back function
	                            //	 with GENLOAD
	NCME_BRD_DETECT,			// Error auto-detecting board(s)
	NCME_NO_RESOURCES,			// No system resources available
	NCME_NO_INF,				// INF files could not be found
	NCME_MEM_ALLOC,				// Memory allocation error
	NCME_BAD_INF,				// Error parsing INF file
	NCME_OPENING_SCM,			// Error opening Service Control Manager
	NCME_OPENING_DLGC_SVC,		// Error opening Dialogic Service
	NCME_STARTING_DLGC_SVC,		// Error starting Dialogic Service
	NCME_STOPPING_DLGC_SVC,		// Error stopping Dialogic Service
	NCME_QUERY_SVC_STATUS,		// Error querying Service Status
	//////////////////////////////////////////// 1/27/98 //////////////////
	//  add an error code for failure of changing the servie status
	NCME_CHANGE_SVC_STATUS,			// Error changing Service Status
	NCME_UNKNOWN_SERVICE_TYPE,		// Unknown service type
	NCME_INVALID_DEVICE,			// Invalid device name
	NCME_INVALID_FAMILY,			// Invalid family name
	NCME_INVALID_INPUTS,			// Invalid inputs
	NCME_DETECTOR_LIB_NOT_FOUND,	// Error loading detector library
	NCME_DETECTOR_FCN_NOT_FOUND,	// Error getting detector function
	//Jack Xu 7/21/98 add error code to support the CTBB technology. 
	NCME_CTBB_LIB,					//Error loading ctbbface.dll
	NCME_CTBB_DEVICESDETECTED,		//Error calling CTBB_DevicesDetected()
	//M.Chia 5-10-99
	NCME_CTBB_USERAPPLY,			//Error calling CTBB_UserApply()
	NCME_CTBB_TRANSACTION_ERROR,	//Error on CTBB Transaction
	NCME_CTBB_TRANSACTION_OVERFLOW, //Error CTBB error Transaction Overflow calling CTBB_UserApply()
	NCME_DLGSERVICE_LIB,            //Error loading DlgServiceResults.dll
	//M.Chia 2-18-99
	NCME_ACCESS_DENIED,				//Denied access error
		//// Open Switch ///////////////////////////////////////// Lisa Yang //////////////////
	NCME_ADD_DEVICE,				// failed in adding the device	
	NCME_MULTI_PCDS,				// multiple pcd files exist
	NCME_MODEL_NAME,				// Error getting model name
	NCME_SET_BUSCAPABILITIES,		// Error setting bus capabilities
	// DM3 Device Detector Error Values	
	NCME_BUFFER_TOO_SMALL,			//	buffer is too small
	NCME_INVALID_ARG,				// Invalid version
	NCME_PCD_SELECTION,				// Error no PCD file selection function
									// with DM3 boards
	NCME_SAVE_CONFIG,				// Failed in saving the configuration	
	NCME_REUSE_CONFIG,
	NCME_SP,
	NCME_OUTSIDE_VARIABLE_DOMAIN,			// Failed in reuse the saved configuration
	NCME_DEPENDENT_SERVICES_RUNNING, // stop service failed because dep services are running
	NCME_EXTENDED_ERROR,			// Extended error; check returned error buffer for details
	NCME_SYSTEMERROR,
	NCME_REMOTE_REG_ERROR,			//error opening "Dialogic" key in remote machine registry.
	NCME_OS_VERSION,				// Error getting the version for the running OS
	NCME_REMOVE_BOARD,				// Error removing board
	NCME_TRUNK_CONFIG_FILE_NOT_FOUND, //-- if associated CONFIG file not found during trunk config.
	NCME_TRUNK_CONFIG_FILE_PARSE, //-- if CONFIG file has no entry for Trunk Configuration
	NCME_TRUNK_CONFIG_PROCESS_CREATION, //--if Create Process failed for trunk config
	NCME_TRUNK_CONFIG_SPECIFIC , //--if Process Returned Specific Error
	NCME_TRUNK_CONFIG_FILE_ACTIVITY,  //-- if couldn't create or delete the temp file.
	NCME_TRUNK_CONFIG_QUARTET1, //--The protocols in the quartet 1 is not the same(not either T1 or E1)
	NCME_TRUNK_CONFIG_QUARTET2, //--The protocols in the quartet 2 is not the same(not either T1 or E1)
	NCME_TRUNK_CONFIG_QUARTET3, //--The protocols in the quartet 3 is not the same(not either T1 or E1)
	NCME_TRUNK_CONFIG_QUARTET4, //--The protocols in the quartet 4 is not the same(not either T1 or E1)
	NCME_TRUNK_CONFIG_INVALID_PROTOCOL, //--INVALID protocol. Does not contain either (T1) or (E1) in the protocol value; or group
	NCME_TRUNK_CONFIG_PROTOCOL_MISMATCH, //--Protocol Mismatch.
	// 3rd Party timeslot api
	NCME_UNAVAILABLE_TIMESLOT,		// request timeslot doesn't exist
	NCME_RELEASE_TIMESLOT,			// the time slots could not be released
	NCME_MISSING_BUS_CAPABILITIES,	// Bus capabilities is not set for 3rd party board 
	NCME_FAIL_TO_SET_PRIMARY,		// Fail to Set Primary Master
	NCME_FAIL_TO_SET_SECONDARY,		// Fail to Set Secondary Master
	NCME_FAIL_TO_CONFIGURE_BUS,		// Fail to confiure Bus.
	NCME_CREATING_DEFAULT_PCD_FILE, // Error creating defaultPCD.cfg file
	NCME_DEFAULT_NOT_FOUND,			// Default PCD file name was not found
	NCME_SAVE_FAILED,				//-- Failed to save the System Snapshot information
	NCME_RESTORE_FAILED,			//-- Failed to restore the System Snapshot information
	NCME_SYSTEMCONFIG_NOT_FOUND,	//-- System Snapshot information is not available to be restored
	NCME_BUSCONFIG_NOT_FOUND,		//-- Bus Configuration was not stored as a part of System Configuration
	NCM_NOT_ALL_BOARDS_DETECTED_TIMEDOUT,	//-- Did not detect all the boards in the time user app specified
	NCME_LAST
} NCMRetCode;

#define NCM_END_OF_DATA		(HRESULT) -1L  // marks end of data

// Create a definition equivalent to the code
//    returned when a value cannot be found in
//    registry
#define ENGE_VALUE_NOT_FOUND	(HRESULT) 2L 
//

typedef enum 
{
	NCM_DLGSRV_AUTO=1,
	NCM_DLGSRV_MANUAL,
	NCM_DLGSRV_DISABLED,
	NCM_DLGSRV_STARTUP_UNDEFINED,
	NCM_DLGSRV_SEMIAUTO
} NCMDlgSrvStartupMode;


// System State since SR5.11 SP1

typedef enum 
{
	NCM_SYSTEM_START_PENDING=1,
	NCM_SYSTEM_STOP_PENDING,
	NCM_SYSTEM_STOPPED,
	NCM_SYSTEM_RUNNING,
	NCM_SYSTEM_HALTED,
	NCM_SYSTEM_STATE_UNDEFINED
} NCMSystemState;

//typedef enum 
//{
//	NCM_DLGSRV_PENDING=1,
//	NCM_DLGSRV_RUNNING,
//	NCM_DLGSRV_HALTED,
//	NCM_DLGSRV_STATE_UNDEFINED
//} NCMDlgSrvState;
typedef DWORD NCMDlgSrvState;


//////////////////////////////////
// GENLOAD-specific defines
//////////////////////////////////

// Typedef for the callback function used by
//    GENLOAD's board detection function.
typedef     int (GL_PROG_FUNC)( UINT uiPercent, const char *work );

// GENLOAD return code definitions
#define GLOAD_SUCCESS	0

#define IN 
#define OUT
#define INOUT

// --------------------------- Board Detection ------------------------------------
//
// percentageCompleted: address of the integer variable that the boards detection fcn will 
//            fill to indicate percentage completed
// message: null-terminated character string containing some meaningful msg with regards
//                                to the percentage completed (e.g., "Detected Board #5")
typedef int ( NCM_CALLBACK_FCN ) ( OUT UINT percentageCompleted, OUT const char *message );
//

//
typedef struct _NCM_DETECTION_DETAILS	// User <- API
{
	int		structSize;
	int 	numDetectors;				// number of detectors
	int 	numBoardsDetected[256];     // number of boards detected by the detector
	int 	returnCode[256];		    // detector return code
	char	returnMsg[64][256];	    // detector returned message
	char	detector[64][256];	        // board detector name 
} NCM_DETECTION_DETAILS;
//
typedef struct _NCM_DETECTION_RESULT	// User <- API
{
	int						structSize;       	    // sizeof NCM_DETECTION_RESULT
	int						totalDetectedBoards;	// total number of boards detected 
	NCM_DETECTION_DETAILS	returnInfo;             // detection returned info.
} NCM_DETECTION_RESULT;
//
typedef struct _NCM_DETECTOR_INFO		// API -> Detector
{
	int 				structSize;		// sizeof(NCM_DETECTOR_INFO)
	NCM_CALLBACK_FCN  * callbackFcn;    // address of the call back function 
} NCM_DETECTOR_INFO;
// 
typedef struct _NCM_DETECTOR_RESULT		// API <- Detector
{
	int   	structSize;      	        // sizeof(NCM_DETECTOR_RESULT)
	int   	numBoardsDetected;			// number of boards detected by the boards detection 
	char	returnMsg[256];             // detection returned message
} NCM_DETECTOR_RESULT;
//
typedef int (NCM_DETECTOR_FCN) (IN NCM_DETECTOR_INFO *, OUT NCM_DETECTOR_RESULT *);
//

typedef struct _NCM_CTRLSERVICEINFO		// RChen 1/28/99
{
	int					structSize;	           // sizeof(NCM_CTRLSERVICEINFO)
	char*					configToolName;        // customer toolname with path 
	char*             args[10];			     // array for function spawnv()
} NCM_CTRLSERVICEINFO;

//
// --------------------------- NCMValueEx -----------------------------------------
typedef enum 
{ 
	UNDEFINED=0, 
	NUMERIC, 
	ALPHANUMERIC,
	NCMFILE   			// to be used for filenames
} NCMDataType;
//
typedef struct _NCMValueEx
{
	int					  structSize;	// sizeof(NCMValueEx)
	NCMDataType			  dataType;		// an enumerated type which will signify the type of data 
	void				* dataValue;	// a buffer which holds the data
	int 				  dataSize;		// size of the buffer
	struct _NCMValueEx	* next;
} NCMValueEx;

// Variable Attributes
typedef enum 
 {
	NCM_DOMAIN_UNDEFINED = -1,
	NCM_DOMAIN_OPEN,
	NCM_DOMAIN_CLOSE
 } NCMVariableDomainType;

typedef enum 
{
	NCM_VIS_UNDEFINED = -1,
	NCM_VARIABLE_VISIBLE,
	NCM_VARIABLE_HIDDEN
} NCMVariableVisibleType;

typedef enum 
{
	NCM_ACC_UNDEFINED = -1,
	NCM_VARIABLE_RW,
	NCM_VARIABLE_RO
} NCMVariableEditType;

typedef struct _NCMVariableAttributes
{
	unsigned int		    structSize;
	NCMDataType				dataType;
    int						radix;
	NCMVariableDomainType   domainType;
	NCMVariableVisibleType  visibleType;
	NCMVariableEditType	    editType;
} NCMVariableAttributes; 

// Property Attributes
typedef enum
{
	NCM_PROPERTY_UNDEFINED = -1,
	NCM_PROPERTY_VISIBLE,
	NCM_PROPERTY_HIDDEN
} NCMPropertyAttributes;

///////// OpenSwitch ///////////////////// 5/20/99 //// Lisa Yang ///////////////////

// Event object used for API send notification to NCM GUI to update the installed devices tree list

#define		GUIEvent  "GUIEvent"

// Type define for detector
#define		NCM_MAX_BOARDS	32
#define 	NCM_MAX_SERNUM_LEN	64


#define		NCM_MAX_FILEDESC 	 2*MAX_PATH		// Max length of a file description

#define		NCM_DEVINFO_VER_0100		0x00010000 // NCMDevInfo structure version
#define		NCM_FILEINFO_VER_0100		0x00010000 // NCMDevInfo structure version
#define		NCM_DEVINFO_VER_0200		0x00020000 //--task# 13080
#define		NCM_TIMESLOT_VER_0100		0x00010000 // NCM_TS_BLOCK_STRUCT structure version
#define		NCM_BUSCAPS_VER_0100		0x00010000 // NCM_TDM_BUSCAPS structure version

#define		DM3FamilyName "DM3" //--task #18238
#define		RozettaFamilyName  "Switching Resource" //--task# 13080.
#define		PMACFamilyName  "NetStructure IPT Board Series" //--task# 16313, 17520
//--following enum added for task# 13080.
typedef enum {
	NCM_BOARD_FAMILY_DM3,
	NCM_BOARD_FAMILY_ROZETTA,
	NCM_BOARD_FAMILY_PMAC
}NCM_BOARD_FAMILY_TYPE;


typedef struct _NCMDevInfo
{
	DWORD 			version;
	char 	 		InstanceLabel[256];
	unsigned long 	InstanceNumber;
	unsigned long	LogicalID;
	unsigned long 	IRQLevel;
	unsigned long 	intVector;
	LARGE_INTEGER 	PLXAddr;
	unsigned long 	PLXLength;
	LARGE_INTEGER	SRAMAddr;
	unsigned long 	SRAMLength;
	unsigned long 	busNumber;
	unsigned long 	slotNumber;
	unsigned long 	dlgcOUI;
	UCHAR 			primBoardId[4];
	UCHAR			secBoardId[4];
	unsigned long	modelNumber;
	unsigned long	sramSize;
	unsigned long	locatorId;
	UCHAR			serialNumber[NCM_MAX_SERNUM_LEN]; 
	unsigned long	curState; 
	BOOL			startPending; 
	unsigned long	shelfID;
	unsigned long 	subnetID;
	unsigned long  physicalState;
	unsigned long	PhysicalSlotNumber;
	NCM_BOARD_FAMILY_TYPE  boardFamily;//--task# 13080
} NCMDevInfo;


typedef struct _NCMFileInfo
{
	DWORD 			version;
	char			fileName[MAX_PATH];
	char			fileDesc[NCM_MAX_FILEDESC];
	char			modelName[MAX_PATH];
} NCMFileInfo;


// NCM PCD file callback function:
typedef int (NCM_PCDFILE_SELECTION_FCN) (IN NCMFileInfo *fileList, IN size_t numFiles, 
										IN NCMDevInfo devInfo, OUT int * index);

typedef struct _NCM_DETECTION_INFO		// User -> API
{
	int					structSize;	        // sizeof(NCM_DETECTION_INFO)
//	char				boardDetector[256];	// specifying the board detector, (e.g., "DM3", "Genload")
	NCM_CALLBACK_FCN  * callbackFcn;        // address of the call back function 
	NCM_PCDFILE_SELECTION_FCN *   pcdFileSelectionFcn;
					      // address of  PCD file callback function

} NCM_DETECTION_INFO;

// to store system info

typedef struct _NCMSysVersion 
{
	char szOSName[MAX_PATH];
	char szOSVersion[MAX_PATH];
	char szOSBuild[MAX_PATH];
	char szOSType[MAX_PATH];	
	char szOSSvcPack[MAX_PATH];
	char szDSSVersion[MAX_PATH];
	char szDSSRelease[MAX_PATH];
	char szDSSBuild[MAX_PATH];
	char szDSSSvcPack[MAX_PATH];
	
} NCMSysVersion;




// DM3 detector function prototype
typedef int (MC_DETECT_DEVICES) (INOUT size_t *pBuffersize, INOUT NCMDevInfo * pDevList);
typedef int (MC_GET_PCDFILE_NAMES)(IN int modelNum, INOUT size_t* pBufferSize, 
								   INOUT NCMFileInfo *pFileList);
typedef int (MC_GET_MODEL_NAME) (INOUT NCMFileInfo *pfileList);
typedef int (SET_BUSCAPABILITIES) (IN NCMFamily *pncmFamily, IN NCMDevice *pncmDevice);
typedef int (MC_GET_BOARD_CONFIG_DATA)(IN  DWORD dwPhysicalId,
									   OUT NCMDevInfo *pDevList,
									   IN  DWORD dwBoardType );

//--Following typedef is added for TDM Bus Admin, Reference Fallback. Hari 11/01/00
typedef  struct _NCMNetRef
{
	char Device[MAX_PATH];//--For storing the device name
    int   NetInterface ;  //--For storing the Network Interface Number
}NCMNetRef;

// -- define new board info structure to share with PMAC and Mercconfig
typedef struct _NCMBoardStruct
{
	unsigned long 	busNumber;
	unsigned long 	slotNumber;
	unsigned long	numofModelNumber;
	unsigned long	*pModelNumberList;
	UCHAR			serialNumber[NCM_MAX_SERNUM_LEN]; 
	NCM_BOARD_FAMILY_TYPE  boardFamily;
} NCMBoardStruct;

//--following is a structure for Trunk configuration.Task#18618
typedef struct _NCMTrunkConfig
{
	char * 		TrunkName; //--name of the trunk such as "Trunks 1-4"
	char * 		TrunkValue; //--value for the trunk such as "4ess"
	struct _NCMTrunkConfig 	* next;
} NCMTrunkConfig;



// 3rd Party Timeslot API Related Structures

typedef struct _NCM_TS_BLOCK_STRUCT
{
	int  	version;
	int 	start_time_slot;
	int 	number_of_time_slots;
} NCM_TS_BLOCK_STRUCT;

typedef enum
{
	NO_UNIQUE_ID = -3
} NCMStartTimeSlot;

typedef enum 
{
	NCM_TIMESLOT_ARBITRARY = 1,
	NCM_TIMESLOT_SPECIFIC
} NCMTSRequestType;

typedef enum 
{
	NCM_TIMESLOT_PERSISTENT = 1,
	NCM_TIMESLOT_TRANSIENT,
	NCM_TIMESLOT_ALL
}NCMTSReserveType;

typedef enum
{
	NCM_PRIMARY		= 1,
	NCM_SECONDARY,
	NCM_SLAVE
}NCMMasterStatus;


typedef struct 
{
    long structVersion;
    BOOL bH100MasterCapable;
    BOOL bH100SlaveCapable;
    BOOL bH110MasterCapable;
    BOOL bH110SlaveCapable;
    BOOL bScbusMasterCapable;
    BOOL bScbusSlaveCapable;
    BOOL bMvipMasterCapable;
    BOOL bMvipSlaveCapable;
    BOOL bScbus2MhzCapable;
    BOOL bScbus4MhzCapable;
    BOOL bScbus8MhzCapable;
}NCM_TDM_BUSCAPS;

typedef struct _TimeSlotRange
{
    unsigned short minTimeSlot;
    unsigned short maxTimeSlot;

#ifdef __cplusplus
    _TimeSlotRange(unsigned short minSlot = 0xFFFF, unsigned short maxSlot = 0xFFFF)
    : minTimeSlot(minSlot), maxTimeSlot(maxSlot) {}
#endif
} TimeSlotRange;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~PDK Configuration
#define CFG_FILE_BUF_SIZE 5*MAX_PATH
#define	VARIANT_DELIMITER ";"
#define _DIGITS "0123456789"

#define TRUNK_COUNT "Number of Network Interfaces"
	typedef struct _NCMPDKTrunkVariantType
	{
		char* TrunkPDKValue; //--value for the trunk such as "ar.cdp"
		struct _NCMPDKTrunkVariantType* nextTrunkPDKValue; //variant to PDK variant mapping
		
	} NCMPDKTrunkVariantType;
	
	typedef struct _NCMPDKTrunk
	{
		char *TrunkName; //--name of the trunk such as "Trunks 1"
		NCMPDKTrunkVariantType* TrunkPDKValue; //--value for the trunk such as "ar.cdp"
//		NCMPDKTrunkVariantType* TrunkVariantType; //--value for the trunk such as "ar.cdp"
		
		struct _NCMPDKTrunk* pNextTrunkName; // points to the next trunk name to PDK variant mapping
	} NCMPDKTrunk;
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~PDK Configuration END

#endif
