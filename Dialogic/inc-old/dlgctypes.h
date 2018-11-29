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

#ifndef __DLGCTYPES_H__
#define __DLGCTYPES_H__

#ifdef __cplusplus
#include <string>
#include <list>
#endif

enum OperationalStatusValues
{
    NOT_APPLICABLE_STATUS=0,
    PHYSICAL_PRESENT_NOT_CONFIGURED_STATUS,
    PHYSICAL_PRESENT_CONFIGURED_STATUS,
    CONFIGURED_NOT_PHYSICAL_PRESENT_STATUS,
    ONLINE_STATUS,
    ONLINE_DIAGNOSTIC_STATUS,
    OFFLINE_DIAGNOSTIC_STATUS
};

enum AdminStatusValues
{
    BOARD_DISABLE_ADMIN_STATUS=0,
    BOARD_ENABLE_ADMIN_STATUS
};

enum ClockDaemonModeValues
{
    CLOCK_DAEMON_DISABLED=0,
    CLOCK_DAEMON_ENABLED
};

enum DiagnosticModeValues
{
    DIAGNOSTIC_MEMORY_DUMP=0,
    DIAGNOSTIC_POST
};

enum TrunkStatusValues
{
    TRUNK_GREEN_ALARM_STATUS=0,
    TRUNK_RED_ALARM_STATUS,
    TRUNK_YELLOW_ALARM_STATUS,
    TRUNK_EXTERNAL_LOSS_OF_SIGNAL_STATUS
};

enum TrunkTypeValues
{
    TRUNK_UNDEFINED=0,
    TRUNK_T1,
    TRUNK_E1,
    TRUNK_ISDN,
    TRUNK_ANALOG
};

enum TIMESLOTTYPE
{
    TSD_CONTIGUOUS_ONLY,
    TSD_CONTIGUOUS_OPTIONAL
};

typedef enum DLG_DRIVER_TYPES
{
    WEREWOLF_DRIVER = 0,
    DM3_DRIVER,
    PMAC_DRIVER,
    SPRINGWARE_DRIVER
};

/* TypeDefs */

/// Unique identifier within domain.
typedef long    AUID;

/// Identifies the shelf within a node.
typedef long    SHELFID;

/// Identifies the type of shelf (cPCI/PCI) within a node.
typedef short   SHELFTYPE;

/// Physical cPCI slot number.
typedef long    PHYSCIALSLOTNUMBER;

/// PCI bus identificaion.
typedef long    PCIBUSID;

/// PCI slot number.
typedef long    PCISLOTNUMBER;

/// Physical trunk number on a board.
typedef short   TRUNKNUMBER;

/// Trunk Type
typedef short   TRUNKTYPE;

/// Operational status of the trunk.
typedef short   TRUNKOPSTATUS;

/// Unique identification of a TDM bus within a domain.
typedef AUID    TDMBUSAUID;

/// TDM bus type ( CT_BUSTYPE_MVIP, CT_BUSTYPE_SCBUS, CT_BUSTYPE_H100, CT_BUSTYPE_H110 ).
typedef short   TDMBUSTYPE;

/// TDM bus clocking rate ( CT_CLOCKRATE_NONE, CT_CLOCKRATE_2MHZ, CT_CLOCKRATE_4MHZ, CT_CLOCKRATE_8MHZ ).
typedef short   CLOCKRATE;

/// Network reference clocking rate ( CT_CLOCKRATE_8KHZ, CT_CLOCKRATE_1_536MHZ, CT_CLOCKRATE_1_544MHZ, CT_CLOCKRATE_2_048MHZ ).
typedef short   NETREFCLOCKRATES;

typedef short   OPERATIONALSTATUS;

typedef short   ADMINSTATUS;

typedef short   FAULTSTATUSTYPE;

typedef long    DLGTYPE;

typedef long    DLGSTATUS;

typedef long    PHYSICALID;

typedef long    LOGICALID;

typedef long    CLAIMID;

typedef long    TIMESLOTNUMBER;

typedef long    DLGSYSRESULTTYPE;

#ifdef __cplusplus

typedef std::list<AUID> AUIDLIST;
typedef AUIDLIST::iterator AUIDLIST_ITER;

/// Technology family of the product (Quadspan, IPLink, etc...) 
typedef std::string CTPLATFORMFAMILY;

/// Technology of the product (DM3, PMAC, SPRINGWARE ).
typedef std::string CTPLATFORM;

/// Serial number of the product
typedef std::string SERIALNUMBER;

/// IP Address xxx.xxx.xxx.xxx or machine name
typedef std::string IPADDRESS;

typedef std::list<IPADDRESS> NODEADDRESSLIST;
typedef NODEADDRESSLIST::iterator NODEADDRESSLIST_ITER;

/// IP Address xxx.xxx.xxx.xxx or machine name
typedef char*  CHIPADDRESS;

/// Obsolete Technology family of the product (Quadspan, IPLink, etc...) 
typedef std::string DEVICEFAMILY;

/// Obsolete Technology of the product (DM3, PMAC, SPRINGWARE, etc... ).
typedef std::string DEVICETECHNOLOGY;

typedef std::string PRODUCTID;

#endif


#ifdef __cplusplus

//generic constants
const   short	CT_UNRESOLVED   = -1;
const   short   CT_UNDEFINED    = -2;

const	short   UNDEFINED_OS    = 0; 
const	short   LINUX_OS        = 1; 
const	short   WINDOWS_OS	    = 2; 

const	short   AUTO_NONE  		= 0; 
const	short   AUTO_START      = 1; 
const	short   AUTO_DETECT	    = 2; 
const	short   AUTO_INIT       = 3; 

//clocking constants
// Valid Clocking Models
const	short	CT_NORMAL		=	0;
const	short	CT_HOLDOVER		=	2;
const	short	CT_FREE_RUN		=	3;
const	short	CT_AUTO_HOLDOVER=	6;
const	short	CT_AUTO_FREE_RUN=	7;

//Valid Clocking lines (BitMasks)
const	short	CT_CLKA			=	0x0001;
const	short	CT_CLKB			=	0x0002;
const	short	CT_DIS			=	0x0003;

//Valid Roles and Capabilities ( BitMasks)
const	short	CT_PRIMARY		=	0x0001;
const	short	CT_SECONDARY	=	0x0002;
const	short	CT_SLAVE		=	0x0004;
const	short	CT_REFERENCE	=	0x0008;

//CT Bus types (BitMasks)
const	short	CT_BUSTYPE_MVIP		=	0x0001;
const	short	CT_BUSTYPE_SCBUS	=	0x0002;
const	short	CT_BUSTYPE_H100		=	0x0004;
const	short	CT_BUSTYPE_H110		=	0x0008;

//PCM  TDM BUS Encoding
const	short   UNDEFINED_ENCODING	= 0x0000;
const	short   MULAW_ENCODING		= 0x0001;
const	short   ALAW_ENCODING		= 0x0002;

// Master Clock Rates (BitMasks)
const	long	CT_CLOCKRATE_NONE	=	0x00000000;
const	long	CT_CLOCKRATE_2MHZ	=	0x00000001;
const	long	CT_CLOCKRATE_4MHZ	=	0x00000002;
const	long	CT_CLOCKRATE_8MHZ	=	0x00000004;

// Netref Clock Rates (BitMasks)
const	long	CT_CLOCKRATE_8KHZ	    =	0x00000008;
const	long	CT_CLOCKRATE_1_536MHZ	=	0x00000010;
const	long	CT_CLOCKRATE_1_544MHZ	=	0x00000020;
const	long	CT_CLOCKRATE_2_048MHZ	=	0x00000040;

// Valid Clock Sources and Network clock sources
const	long	CT_DISABLED		=       0xFFFFFFFF;
const	long	CT_DNI_1		=	0x00000001;
const	long	CT_DNI_2		=	0x00000002;
const	long	CT_DNI_3		=	0x00000003;
const	long	CT_DNI_4		=	0x00000004;
const	long	CT_DNI_5		=	0x00000005;
const	long	CT_DNI_6		=	0x00000006;
const	long	CT_DNI_7		=	0x00000007;
const	long	CT_DNI_8		=	0x00000008;
const	long	CT_DNI_9		=	0x00000009;
const	long	CT_DNI_10		=	0x0000000A;
const	long	CT_DNI_11		=       0x0000000B;
const	long	CT_DNI_12		=	0x0000000C;
const	long	CT_DNI_13		=	0x0000000D;
const	long	CT_DNI_14		=	0x0000000E;
const	long	CT_DNI_15		=	0x0000000F;
const	long	CT_DNI_16		=	0x00000010;
const   long    CT_MAXDNI               =       0x0000FFFF;

const	long	CT_INT_CLK		=	0x00010000;
const	long	CT_NETREF		=	0x00010001;
const	long	CT_NETREF2		=	0x00010002;

//pmac constants
const	short	MAX_NETWORK_PROPS   =   4;

// Valid IP Versions (BitMasks)
const	short	IP_VERSION_IPv4		=	0x0000;
const	short	IP_VERSION_IPv6		=	0x0001;

// Valid Ethernet Load Sharing Mode
const	short	ETHERNET_LS_MODE_NOT_USED	=	0x0000;
const	short	ETHERNET_LS_MODE_LOADSHARE	=	0x0001;
const	short	ETHERNET_LS_MODE_PRIMARY	=	0x0002;
const	short	ETHERNET_LS_MODE_BACKUP		=	0x0003;

// Valid Ethernet Failover Modes
const	short	ETHERNET_FAILOVER_NO	=	0x0000;
const	short	ETHERNET_FAILOVER_YES	=	0x0001;

// Valid UDP Use Checksum
const	short	UDP_CHECKSUM_SET	    =	0x0000;
const	short	UDP_CHECKSUM_TRANSMIT	=	0x0001;

// Valid DHCP Modes
const	short	DHCP_MODE_DISABLED	=	0x0000;
const	short	DHCP_MODE_ENABLED	=	0x0001;

// Valid DTMF H.245 Mode
const	short	H245MODE_REPORTING_OFF	=	0;
const	short	H245MODE_REPORTING_ON	=	1;

// Valid Codec values
const	long	DLG_CODEC_NONE		    =	0x00000000;
const	long	DLG_CODEC_G711_40BYTES	=	0x00000001;
const	long	DLG_CODEC_G711_80BYTES	=	0x00000002;
const	long	DLG_CODEC_G711_160BYTES	=	0x00000003;
const	long	DLG_CODEC_G711_240BYTES	=	0x00000004;
const	long	DLG_CODEC_G723_53KBPS	=	0x00000018;
const	long	DLG_CODEC_G723_63KBPS	=	0x00000019;
const	long	DLG_CODEC_G729A         =	0x00000020;
const	long	DLG_CODEC_G726_40BYTES	=	0x00000038;
const	long	DLG_CODEC_G723_80BYTES	=	0x00000039;
const	long	DLG_CODEC_G723_240BYTES	=	0x0000003A;

// Valid Echo Tail Length Values
const	long	ECHO_TAIL_NO_CANCELLATION	= 0x00000000;
const	long	ECHO_TAIL_LENGTH	        = 0x00000001;
const	long	ECHO_TAIL_LENGTH16	        = 0x00000002;
const	long	ECHO_TAIL_LENGTH32	        = 0x00000003;
const	long	ECHO_TAIL_LENGTH64	        = 0x00000004;
const	long	ECHO_TAIL_LENGTH128	        = 0x00000005;
const	long	ECHO_TAIL_LENGTH96	        = 0x00000006;

//Fault Status Bit Mask Values
const   short   DLG_NO_FAULT            =   0x0000;
const   short   DLG_INIT_FAULT          =   0x0001;
const   short   DLG_CP_FAULT            =   0x0002;
const   short   DLG_SP_FAULT            =   0x0004;
const   short   DLG_TDM_ALINE_FAULT     =   0x0008;
const   short   DLG_TDM_BLINE_FAULT     =   0x0010;
const   short   DLG_TDM_SCBUS_FAULT     =   0x0020;
const   short   DLG_TDM_MASTERPLL_FAULT =   0x0040;
const   short   DLG_TDM_NETWORK_FAULT   =   0x0080;
const   short   DLG_ETHERNET_FAULT      =   0x0100;
const   short   DLG_RESERVED_FAULT      =   0x4000;

#endif

// Retired Definitions and Typedefs

/**********************************************
 * Definitions and Typedefs In This Area Are  *
 * Should Not Be Used Since They Are Retired  *
 **********************************************/

typedef long    ShelfID;
typedef short   ShelfType;
typedef long    PhysicalSlotNumber;
typedef long    PciBusId;
typedef long    PciSlotNumber;
typedef short   TrunkNumber;
typedef	short   TrunkOpStatus;
typedef AUID    TDMBusAuid;
typedef short   TDMBusType;
typedef short   ClockRate;
typedef short   NetrefClockRates;
typedef short   OperationalStatus;
typedef short   AdminStatus;
typedef short   FaultStatusType;
typedef long    DlgType;
typedef long    DlgStatus;
typedef long    PhysicalID;
typedef long    LogicalID;
typedef	short   ClaimId;
typedef long    TimeslotNumber;
typedef long    DlgSysResultType;
typedef char*   chIPAddress;

#ifdef __cplusplus

typedef std::string CTPlatformFamily;
typedef std::string CTPlatform;
typedef std::string SerialNumber;
typedef std::string IPAddress;
typedef std::string DeviceFamily;
typedef std::string DeviceTechnology;
typedef std::string ProductID;

#endif

// End Retired Defintions and Typedefs

#endif //__DLGCTYPES_H__

