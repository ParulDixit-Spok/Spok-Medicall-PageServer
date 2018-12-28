#pragma once

// definesS.h

// defines that are Shared/Included in both project types
//  pageserver (xn, xo, xa), and configuration utility.


// the CONFIG_FILE_SIGNATURE and  CONFIG_FILE_TERMINATOR must be exactly 
//  8 characters in length surrounded by double quotes.
#define CONFIG_FILE_SIGNATURE  "A55A00FF"

// The CONFIG_FILE_VERSION is of the same format as
//  the application version string.  this string will
//  be changed each time the data structure of the config file
//  changes. 
//  It is used to prevent the PageServer from reading an older
//   format configuration file.


// the CONFIG_FILE_VERSION must be exactly 
//  9 characters in length surrounded by double quotes.
//#define CONFIG_FILE_VERSION "4-20-0089"	
#define CONFIG_FILE_VERSION "4-20-0096"	

// config file history of the CONFIG_FILE_VERSION constant.
//	
// "3-00-0028" - Original release with filesize, signature, checksum tests.
//
// "3-00-0030" - added check box option to "page line options" screen for
//				 "flush digit buffer" to facilitate overdial when pagers
//               are "covered-by". make this box un-checked, and the digit
//               buffer will not be cleared between state machine states.
//
// "3-00-0035" - removed btrieve list option, this variable was not used 
//               anywhere within the project.

// "3-00-0036" - added option to use or not use the 
//				 "Canned Alpha Messages" file XTPGMSG.XTD. 

// "4-00-0045" - added option to use or not use the 
//				 new database field names xnRead vs Read.
//

// added "upgrade" feature to pagecfg.exe between 4-00-0045 and 4-10-0071

// "4-10-0071" - added options to support Impaired service announcements 
//				 when paging vendors are known to have delays or are down.
//
// "4-10-0078" - The Action Reminder "Action Ids" are now configurable parameters.
//				 standard/default values are:
//
//               "SUPERVISOR" alarms:
//               " 8" - Comm Port Trouble, misc, cancelled
//
//				 "UNIT" alarms:
//               "20" - Keep Alive
//               "21" - Application Exit
//               "22" - Queue Rename Trouble
//
// "4-20-0084" - added ability to write or not write to the xtend message base
//               when requests are received via "Tap In" serial port.
//
// "4-20-0086" - changed the values of some constants that must agree between
//               the pageconfig application, and pageserver application, 
//               no parameter file size changes, just "meaining" of constants.
//
// "4-20-0089" - added exit for restart parameters 2005-11-10.
//                
// "4-20-0096" - added Queue Access action reminder for BMC 2006-04-21.
//                


#define MAXDIG         30  // max number of dtmf digits expected
                           //   during the get dtmf state (ST_GET_EXT)        



#define INCOMPLETE		2

// COMM (serial rs-232) port types.
// keep in sync with CommDeviceDescription[] in globals.cpp
#define COMM_NOT_ACTIVE			0		// NO
#define ZETRON15                1		// MTR
#define ZETRON2000              2		// ZTN
#define ZETRON16                3		// 16Z
#define PRONET_IPT              4		// IPT
#define IXO_MODEM               5		// XOM
#define TAP                     6		// TAP
#define IXO_ACCEPT              7       // IXO - smartmodem control - tap in.
#define PAGE_ACCEPT             8       // PAG
#define IXO_DISCARD				9       // NUL - a variation of IXO protocol where the request is discarded.
#define GROUPCALL               10		// GRP
#define TAP_ARCH_UVA            11		// UVA
#define IXO_ACCEPT_DIRECT       12      // IXD - direct connected - tap in.
#define IXX_DIRECT    			13		// IXX - same as IXD but looking up the "PagerId" received by TAP port
										//       in the PatientId column of Xkm table.
#define TAP_TYPE2    			14		// TA2 - Tap type 2, respond better to NAK when expecting ACK 
#define TAP_TYPE3    			15		// TA3 - Tap type 3, this one does not send <CR> until there is 
										//       info in the page queue to be sent.
#define COMM_NOT_DEFINED        16		// keep in sync with text in function
										//  DataScopeDlgProc()

// IVR port types.
// keep in sync with IVRDeviceDescription[] in globals.c
#define IVR_NOT_ACTIVE				 0	// "-  "
#define IVR_PAGE_IN					 1	// "IN "
#define IVR_PAGE_OUT				 2	// "OUT"
#define IVR_BUSY_OUT				 3	// "B/O"
#define IVR_CATS					 4	// "CAT"
#define IVR_METROMEDIA				 5	// "MET"
#define IVR_VOICE_AUDIO				 6	// "VAO"
#define IVR_TYPE_07					 7
#define IVR_TYPE_08					 8
#define IVR_TYPE_09					 9
#define IVR_TYPE_10					10
#define IVR_TYPE_11					11
#define IVR_TYPE_12					12
#define IVR_TYPE_13					13
#define IVR_TYPE_14					14
#define IVR_TYPE_15					15
#define IVR_RST						16	// "RST"
#define IVR_PAGE_STATUS				17	// "STA"
#define IVR_GROUP_CALL				18	// "GRP"
#define IVR_TYPE_19					19
#define IVR_SELF_PARK_AND_PAGE		20	// "KPG"	self park and page.
#define IVR_DPG						21	// "DPG"	
#define IVR_RS2						22	// "RS2"
#define IVR_TYPE_23					23
#define IVR_TYPE_24					24
#define IVR_TYPE_25					25
#define IVR_TYPE_26					26
#define IVR_DST						27	// "DST"
#define IVR_NOT_DEFINED				28	


//
/*      Shared Modes                                                        */
/*                                                                          */
#define ACTIVE                  1       /* Start active, cooperatively shares */
#define BACKUP                  2       /* Start inactive, cooperatively share*/
#define MASTER                  3       /* Start active, dictate when slave   */
                                        /* should activate/deactivate */
#define SLAVE                   4       /* Start inactive, listen to master   */


#define XKM_TABLE				0		// XKM.BTR
#define FRQ_TABLE				1		// XNFRQ.BTR
#define PAGERS_TABLE			2		// PAGERS.BTR
#define MESSAGE_TABLE			3		// MESSAGES.BTR
#define MESSAGE_LINE_TABLE		4		// MSGLINES.BTR
#define MESSAGE_COUNT_TABLE		5		// MSGCOUNT.BTR
#define MESSAGE_DELIVER_TABLE	6		// MSDELIRE.BTR
#define ON_CALL_TABLE			7		// NOOC.BTR
#define LIST_TABLE				8		// XTPGLST.BTR

//
//		Database options
//
#define XN_BTRIEVE				0		// Btrieve databse engine
#define XN_ADO					1		// ADO database engine

// max number of rs-232 serial communication ports.
//  all loops and structures having to do with the number of 
//  comm ports defined depend on this #define.

//  remember to enable/disable buttons in configuration utility
#define MAX_COMMPORTS 34	

// number of items listed in the configuration utility
//   for each comm port.
#define MAX_COMMPORT_QUEUE_ITEMS 20

// max number of Radio Common Carriers (RCC) that may be defined.
#define MAX_RCC 10

// max number of Interactive Voice Response (IVR) dialogic ports that may be defined.
#define MAX_IVRPORTS 96

// max number of paging "Queue Files" that may be defined. 
//  note that many places use just 31 instead of this constant.
#define MAX_QUEUE_FILES 31

// size of the receive buffers for TAP messages.
#define TAP_MESSAGE_BUFFER_SIZE 8000

#define HIGH_PRIORITY_LIST_QUEUE		 0			// don't be fooled... many places in the source
#define LOW_PRIORITY_LIST_QUEUE			29			//  code don't use these constants.



