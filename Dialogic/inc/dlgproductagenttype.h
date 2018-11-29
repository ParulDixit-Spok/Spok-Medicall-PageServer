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

#ifndef __DLGPRODUCTAGENTTYPE_H__
#define __DLGPRODUCTAGENTTYPE_H__

/* Product Agent Name */

#define MAX_PRODUCT_AGENT_NAME  50

#define DETECTOR_AGENT      "DETECTOR_AGENT"
#define INITIALIZER_AGENT   "INITIALIZER_AGENT"
#define FAULTDETECTOR_AGENT "FAULTDETECTOR_AGENT"
#define CLOCKING_AGENT      "CLOCKING_AGENT"
#define PNPDETECTOR_AGENT   "PNPDETECTOR_AGENT"
#define DIAGNOSTIC_AGENT    "DIAGNOSTIC_AGENT"
#define OSLEXT_AGENT        "OSLEXT_AGENT"

//For any new technology product agent  supported must be defined here

/* Technology Name */

#define MAX_TECHNOLOGY_NAME 50

#define DM3                 "DM3"
#define SPRINGWARE          "SPRINGWARE"
#define PMAC                "PMAC"
#define COMMON              "COMMON"

enum AGENT_TECHNOLOGIES { DLG_UNKNOWN=0, DLG_DM3, DLG_SPRINGWARE, DLG_PMAC };

//define Family types
#define	DM3QUADSPAN		"QUADSPAN"
#define	DM3IPLINK		"IPLINK"

enum AGENT_FAMILIES { FAMILY_UNKNOWN=0, FAMILY_QUADSPAN, FAMILY_IPLINK };

#endif  // __DLGPRODUCTAGENTTYPE_H__

