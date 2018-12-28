/**********@@@SOFT@@@WARE@@@COPY@@@RIGHT@@@**********************************
* DIALOGIC CONFIDENTIAL
*
* Copyright (C) 2003-2008 Dialogic Corporation. All Rights Reserved.
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

#ifndef NCMAPI_H
#define NCMAPI_H

#include "NCMTypes.h"
#include <winsvc.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  _NCMAPI_
#define NCM_API __declspec(dllexport)
#else
#define NCM_API __declspec(dllimport)
#endif

// --------------------------------------------------------------------------------
//  Read
// --------------------------------------------------------------------------------

NCM_API NCMRetCode      NCM_GetAllFamilies( NCMFamily ** ppncmFamily );
NCM_API NCMRetCode      NCM_GetInstalledFamilies( NCMFamily ** ppncmFamily );

NCM_API NCMRetCode      NCM_GetAllDevices( NCMFamily *, NCMDevice ** );
NCM_API NCMRetCode      NCM_GetInstalledDevices( NCMFamily *,
                                                 NCMDevice ** ppncmDevice );

NCM_API NCMRetCode      NCM_GetProperties( NCMFamily *, NCMDevice *,
                                           NCMProperty ** ppncmProp );
NCM_API NCMRetCode      NCM_GetVariables( NCMFamily *, NCMDevice *,
                                          NCMProperty *, NCMVariable ** );
NCM_API NCMRetCode      NCM_GetValue( NCMFamily *, NCMDevice *, 
                                      NCMProperty *, NCMVariable *,
                                      NCMValue ** );
NCM_API NCMRetCode      NCM_GetValueRange( NCMFamily *, NCMDevice *, 
                                           NCMProperty *, NCMVariable *,
                                           NCMValue ** );

NCM_API NCMRetCode      NCM_IsEditable( NCMFamily *, NCMDevice *, 
                                        NCMProperty *, NCMVariable *,
                                        BOOL * );

NCM_API NCMRetCode      NCM_GetErrorMsg( NCMRetCode ncmRcIn, NCMErrorMsg** );

NCM_API NCMRetCode      NCM_IsBoardEnabled( NCMFamily *,
                                               NCMDevice *,
                                               BOOL * );


// --------------------------------------------------------------------------------
//  Create, Update, Delete
// --------------------------------------------------------------------------------

NCM_API NCMRetCode      NCM_SetValue( NCMFamily *, NCMDevice *, NCMProperty *, NCMVariable *, NCMValue * );

NCM_API NCMRetCode      NCM_AddDevice( NCMFamily *family, NCMDevice *modelName, NCMDevice *uniqueName );
NCM_API NCMRetCode      NCM_DeleteEntry( NCMFamily *, NCMDevice * );

NCM_API NCMRetCode      NCM_EnableBoard( NCMFamily *,
                                            NCMDevice *,
                                            BOOL );

NCM_API NCMRetCode      NCM_DetectBoards( GL_PROG_FUNC*, int * );

//Added the function to synchronize software versions. Greg Perlovsky 08-08-00 
NCM_API NCMRetCode      NCM_GetVersionInfo(NCMSysVersion *psysver);
////////////////////////// Third Party Configuration, RChen  11/17/99////////////////////////
// --------------------------------------------------------------------------------
//  Dialogic service
// --------------------------------------------------------------------------------

NCM_API NCMRetCode      NCM_GetDlgSrvStartupMode( NCMDlgSrvStartupMode *);
NCM_API NCMRetCode      NCM_SetDlgSrvStartupMode( NCMDlgSrvStartupMode );

NCM_API NCMRetCode      NCM_GetDlgSrvState( NCMDlgSrvState * );

// Support semi-automatic mode since SR5.11 SP1 
NCM_API	NCMRetCode  	NCM_StopSystem();
NCM_API	NCMRetCode  	NCM_StartSystem();
NCM_API	NCMRetCode		NCM_GetSystemState (NCMSystemState *);

//M.Chia 2-3-99 added the function below from J.Xu
NCM_API NCMRetCode      NCM_GetDlgSrvStateEx( SERVICE_STATUS * );

NCM_API NCMRetCode      NCM_StartDlgSrv();
NCM_API NCMRetCode      NCM_StopDlgSrv();


// --------------------------------------------------------------------------------
//  Administration
// --------------------------------------------------------------------------------
NCM_API NCMRetCode  NCM_Dealloc( NCMString * pncmStr );

NCM_API NCMRetCode  NCM_GetValueEx(      IN     NCMFamily   *,
                                         IN     NCMDevice   *, 
                                         IN     NCMVariable *, 
                                         OUT    NCMValueEx  ** );

NCM_API NCMRetCode  NCM_GetValueRangeEx( IN     NCMFamily   *, 
                                         IN     NCMDevice   *, 
                                         IN     NCMVariable *, 
                                         OUT    NCMValueEx  ** );
NCM_API NCMRetCode  NCM_SetValueEx(      IN     NCMFamily   *, 
                                         IN     NCMDevice   *, 
                                         IN     NCMVariable *, 
                                         IN     NCMValueEx  * );

NCM_API NCMRetCode  NCM_DeallocValue(    IN     NCMValueEx  *  );
NCM_API NCMRetCode  NCM_DetectBoardsEx(  IN     NCM_DETECTION_INFO   *,  
                                         INOUT  NCM_DETECTION_RESULT * );

// --------------------------------------------------------------------------------
//  Attributes
// --------------------------------------------------------------------------------
NCM_API NCMRetCode  NCM_GetPropertyAttributes( IN     NCMFamily   *,
                                               IN     NCMDevice   *, 
                                               IN     NCMProperty *, 
                                               OUT    NCMPropertyAttributes * );

NCM_API NCMRetCode  NCM_GetVariableAttributes( IN     NCMFamily   *,
                                               IN     NCMDevice   *, 
                                               IN     NCMVariable *, 
                                               OUT    NCMVariableAttributes * );

//Jack Xu 7/23/98 add 2 new API functions calling start and stop boards.

NCM_API NCMRetCode NCM_StartBoard(IN NCMFamily*, IN NCMDevice*);

NCM_API NCMRetCode NCM_StopBoard(IN NCMFamily*, IN NCMDevice*); 

// ----------------------------------------------------------------------------
//  Clock Master Fallback List - Pulsepoint     4/23/99 M.Chia
// ----------------------------------------------------------------------------

NCM_API NCMRetCode  NCM_GetClockMasterFallbackList( IN      NCMDevice   *, 
                                                    OUT     int         * , 
                                                    OUT     NCMDevice   ** );

NCM_API NCMRetCode  NCM_SetClockMasterFallbackList( IN      NCMDevice   *,  
                                                    IN      NCMDevice   * );


NCM_API NCMRetCode  NCM_SetTDMBusValue(             IN      NCMDevice   *, 
                                                    IN      NCMVariable *, 
                                                    IN      NCMValue    *);

NCM_API NCMRetCode  NCM_GetTDMBusValue(             IN      NCMDevice   *, 
                                                    IN      NCMVariable *, 
                                                    IN      NCMValue    ** );

////// OpenSwitch ////////////////////// 5/24/99 ///// ///////// ///////
////////////////////////////////////////////////////////////////////////
NCM_API NCMRetCode  NCM_GetDialogicDir( IN     char *,
                                        INOUT  int  *, 
                                        OUT    char * );

// Device Mapper 12/8/00
NCM_API NCMRetCode NCM_GetAUID (IN NCMFamily*, 
                                IN  NCMDevice*,
                                OUT int *,
                                OUT long *);

NCM_API NCMRetCode NCM_GetFamilyDeviceByAUID (IN long, 
                                              OUT NCMFamily**, 
                                              OUT NCMDevice** );

NCM_API NCMRetCode NCM_ReconfigBoard( IN NCMFamily *pncmFamily, 
                                      IN NCMDevice *pncmDevice, 
                                      NCM_DETECTION_INFO * pdetectInfo, 
                                      OUT NCMDevice ** ppncmDevice);


// --------------------------------------------------------------------------------
//  Country Specific Parameters (CSP)
// --------------------------------------------------------------------------------

NCM_API NCMRetCode  NCM_GetCspCountries( NCMValue ** ncmCountries );
NCM_API NCMRetCode  NCM_GetCspCountryCode( char *countryName,
                                               NCMValue ** ppncmCountryCode 
                                         );
NCM_API NCMRetCode  NCM_GetCspCountryName( char *countryCode,
                                               NCMValue ** ppncmCountryName 
                                         );
NCM_API NCMRetCode  NCM_GetCspFeaturesVariables( char *country,
                                                 NCMVariable ** ppncmCspFeatures 
                                               );
NCM_API NCMRetCode  NCM_GetCspFeaturesValue( char *country, char *features, 
                                             NCMVariable *,
                                             NCMValue ** ppncmFeatValues
                                           );
NCM_API NCMRetCode  NCM_GetCspFeaturesValueRange( char *country, NCMVariable *,
                                                  NCMValue ** ppncmCspValRange 
                                                );

// --------------------------------------------------------------------------------
//  High Availability
// --------------------------------------------------------------------------------
NCM_API NCMRetCode NCM_IsHotSwapSystem (OUT BOOL *);
NCM_API NCMRetCode NCM_GetHotSwapBoardCount (OUT unsigned long *);

//--following new function for Trunk Configuration. Task#18618
NCM_API NCMRetCode NCM_ApplyTrunkConfiguration(NCMFamily Family, 
                                       NCMDevice Device, 
                                       NCMTrunkConfig*  pTrunkConfig,
NCMFeatureType* pMediaLoad, BYTE* pErrorMsg);


////// 3rd PARTY API////////////// 12/06/2002 ///// Santosh Deshpande ///
////////////////////////////////////////////////////////////////////////

NCM_API NCMRetCode	NCM_GetThirdPartyTechName(		 IN		NCMFamily **ppncmFamily);

NCM_API NCMRetCode NCM_AddThirdPartyDevice			(IN		NCMDevice *pDeviceName, 
													 IN		NCM_TDM_BUSCAPS TDMBusCapabilities,
													 IN		NCMMasterStatus eMasterStatus);

NCM_API NCMRetCode NCM_RemoveThirdPartyDevice		(IN		NCMDevice *pDeviceName);


NCM_API NCMRetCode NCM_GetThirdPartyDeviceBusCaps	(IN		NCMDevice *pDeviceName,
													 OUT	NCM_TDM_BUSCAPS *pTDMBusCapabilities);

NCM_API NCMRetCode NCM_AllocateTimeSlots			(IN		NCMDevice *pDeviceName, 
													 IN		int nNumTimeSlots,
													 IN		NCMTSRequestType eArbitrary,
													 IN		NCMTSReserveType ePersistent,
													 OUT	int *pnNumOfBlocks,
													 OUT	NCM_TS_BLOCK_STRUCT *pNCMTSBlock);

NCM_API NCMRetCode NCM_DeallocateTimeSlots			(IN		NCMDevice *pDeviceName, 
													 IN		int nUniqueID,
													 IN		BOOL ePersistent);

NCM_API NCMRetCode NCM_QueryTimeSlots				(IN		NCMDevice *pDeviceName, 
													 IN		int nUniqueID,
													 IN		NCMTSReserveType ePersistent,
													 OUT	int *pnNumOfBlocks,
													 OUT	NCM_TS_BLOCK_STRUCT *pNCMTSBlock);

NCM_API NCMRetCode NCM_SetThirdPartyDeviceBusCaps	(IN		NCMDevice *pDeviceName,
													 IN		NCM_TDM_BUSCAPS TDMBusCapabilities);
NCM_API NCMRetCode  NCM_RemoveBoard( IN NCMFamily *,
                                     IN NCMDevice * );

// -- New functions for save/retrieve/remove configuration file
NCM_API NCMRetCode NCM_SaveConfig( NCMFamily*  pncmFamily, 
								   NCMDevice * pncmDevice );

NCM_API NCMRetCode NCM_RemoveConfig( char* pPath,			// = "SavedConfig";
									 char* pFileName );		// = "bus_#slot#.cfg"

NCM_API NCMRetCode NCM_ApplySavedConfig(  NCMFamily *pncmFamily, 
									  NCMDevice *pncmModel,
									  NCMDevice *pncmUniqueDevice,
									  char* pFath,			// = "SavedConfig";
									  char* pFileName );	// = "bus_#slot#.cfg"

NCM_API BOOL NCM_HasSavedConfig( NCMBoardStruct * pBoardStruct );


// New API to save the complete system snapshot
NCM_API NCMRetCode NCM_SaveConfiguration(void);
// New API to Apply the Saved complete system snapshot
NCM_API NCMRetCode NCM_RestoreConfiguration(unsigned int dwTimeout);

NCM_API NCMRetCode NCM_PDKGetVariantList(NCMFamily* pncmFamily,
                                         NCMDevice* pncmUniqueDevice,
                                         DWORD *pdwTrunk,
                                         NCMPDKTrunk** ppPDKTrunkVariantList, 
                                         BYTE* pErrMsg);
	
NCM_API NCMRetCode NCM_PDKQuerySettings(NCMFamily* pncmFamily, 
                                        NCMDevice* pncmUniqueDevice, 
                                        NCMPDKTrunk** ppPDKTrunkVariantList,
                                        BYTE* pErrMsg);
	
NCM_API NCMRetCode NCM_PDKApplyVariantList(NCMFamily* pncmFamily,
                                           NCMDevice* pncmUniqueDevice, 
                                           NCMPDKTrunk* pPDKTrunkVariantList,
                                           BYTE* pErrMsg);
	
NCM_API void NCM_FreePDKVariants(NCMPDKTrunk *pPDKTrunkVarList);

#ifdef __cplusplus 
}
#endif

#endif
