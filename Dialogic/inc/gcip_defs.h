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
/**********************************************************************
*
*   C Header:       gcip_defs.h
*   Instance:       gc_1
*   Description:    
*   %created_by:    crooksj %
*   %date_created:  Fri Aug 20 16:05:57 2004 %
*   %created_by:    crooksj %
*   %date_created:  Fri Aug 20 16:05:57 2004 %
*
**********************************************************************/
#ifndef _gc_1_gcip_defs_h_H
#define _gc_1_gcip_defs_h_H

/* Define device class */
#define DEV_CLASS_IPT "IPT"

/* Defines for IP_ADDR and IP_ADDRP struct to identify version as IPv4 or IPv6: */
#define IPVER4 0                /* IPv4 address */
#define IPVER6 1                /* IPv6 address */

/* Generic parameters to enable/disable features */  
#define IP_DISABLE	0
#define IP_ENABLE 	1

/* Define for IP_ADDR struct to specify obtaining current host IP address internally: */
#define IP_RETRIEVE_HOST_IPADDR        0             /* current host IP address */


/* Defines for IPCCLIB_START_DATA struct: */
#define IP_CFG_NO_CALLS                0             /* used in element h323_max_calls to prevent H.323
                                                        module from loading or in element sip_max_calls  
                                                        to prevent SIP module from loading. */
#define IP_CFG_MAX_AVAILABLE_CALLS     -2            /* used in elements total_max_calls or h323_max_calls or 
                                                        sip_max_calls to indicate maximum available resources */
#define IP_CFG_DEFAULT                 -1            /* used to initialize IPCCLIB_START_DATA struct via memset 
                                                        indicate default values. */
#define IP_MAX_DTMF_DIGITS             0x20          /* maximum length of digit buffer */
#define IP_REG_MULTICAST_DEFAULT_ADDR  "0.0.0.0"     /* default multicast registration address */

#define IP_REG_SERVER_ADDR_LENGTH      64            /* server address length in characters */
#define IP_REG_CLIENT_ADDR_LENGTH      128           /* client address length in characters */


/* Defines for specifying Supplementary Service support in sup_serv_mask bitmask within IP_VIRTBOARD  */
#define IP_SUP_SERV_DISABLED           0x00          /* All Supplementary Services disabled (default) */
#define IP_SUP_SERV_CALL_XFER          0x01          /* Call Transfer enabled (blind and supervised)  */


/* Defines for exposing additional message information support in 'h323_msginfo_mask' bitmask within IP_VIRTBOARD */
#define IP_H323_MSGINFO_DISABLE        0x00          /* All configuration and retrieval of */
                                                     /* additiona linfo disabled (default) */
#define IP_H323_MSGINFO_ENABLE         0x01          /* Configuration and retrieval of IEs enabled */

#define IP_H323_ANNEXMMSG_ENABLE       0x02          /* Mask to enable reception of Annex M
                                                        Messages in the RV stack */														
#define IP_H323_FASTSTART_CODERS_IN_OFFERED 0x04     /* Mask to enable the attching of FastStart
													  * coder information to received Offered events
													  * on H.323 calls. (Does *not* effect the setting
													  * of outgoing H.323 FastStart coder info.) */
#define IP_H323_DELAY_H245                  0x08     /* Mask to prevent h245Address node from being placed
													  * into our outgoing H.323 'setup' messages. 
													  * By default we will place this node 
													  * into our outgoing 'setup' messages. */
#define IP_H323_RETRIEVE_UUIE_ENABLE		0x10	 /* Mask for h323_msginfo_mask to enable retrieval of UUIE. */


/* The maximum GC parm data length supported by IP CCLIB. Valid values are 
 * 255 <= IPCCLIB_START_DATA.max_parm_data_size <= IP_CFG_PARM_DATA_MAXLEN.  
 * The default value, using IP_CFG_DEFAULT, is 255 bytes. Any non-valid value 
 * will result in an error during IP CCLIB initialization.
 */
#define IP_CFG_PARM_DATA_MAXLEN        4096

/*
 * enumIPCCLIBMediaMode used in structure IPCCLIB_START_DATA  
 * to configure the libraries media behavior.
 */
typedef enum
{
      MEDIA_OPERATIONAL_MODE_1PCC,
      MEDIA_OPERATIONAL_MODE_3PCC
} enumIPCCLIBMediaMode;

/*
 *   IP specific Error codes 
 */
typedef enum {
    /* IPERR_OK | Successful completion.*/
    IPERR_OK,

    /* IPERR_MEMORY | Memory allocation failure. */
    IPERR_MEMORY,

    /* IPERR_NOT_IMPLEMENTED | The function or property call has not been implemented. This 
     * differs from IPERR_UNSUPPORTED in that there is the implication that this is an early 
     * release which intends to implement the feature or function.
     */
    IPERR_NOT_IMPLEMENTED,

    /* IPERR_UNSUPPORTED | This function or property call is unsupported in this configuration 
     * or implementation of stack.  This differs from IPERR_NOT_IMPLEMENTED in that it implies 
     * no future plan to support this feature of property.
     */
    IPERR_UNSUPPORTED,

    /* IPERR_UNAVAILABLE | The requested data is unavailable.
     */
    IPERR_UNAVAILABLE,

    /* IPERR_BAD_PARAM| Call failed because of a bad parameter.
     */
    IPERR_BAD_PARAM,

    /* IPERR_INTERNAL | Internal error occurred.
     */ 
    IPERR_INTERNAL,

    /* IPERR_INVALID_PROPERTY | Property ID is invalid.
     */
    IPERR_INVALID_PROPERTY,

    /* IPERR_SERVER_REGISTRATION_FAILED | Attempt to register with the registration & 
     * admission server failed.
     */
    IPERR_SERVER_REGISTRATION_FAILED,

    /* IPERR_INVALID_STATE | Invalid state to make this call.
     */
    IPERR_INVALID_STATE,

    /* IPERR_DUP_CONF_ID | A conference ID was specified which matches an existing 
     * conference ID for another conference.
     */
    IPERR_DUP_CONF_ID,

    /* IPERR_DESTINATION_UNKNOWN | Failure to locate the host with the address given
     */
    IPERR_DESTINATION_UNKNOWN,

    /* IPERR_LOCAL_INTERNAL_PROXY_ADDR | Local internal proxy specified could not be 
     * resolved to a valid IP address or domain name.
     */
    IPERR_LOCAL_INTERNAL_PROXY_ADDR,

    /* IPERR_REMOTE_PROXY_ADDR | Remote proxy specified could not be resolved to a valid IP 
     * address or domain name.
     */
    IPERR_REMOTE_PROXY_ADDR,

    /* IPERR_PROXY_GATEWAY_ADDR | Two intermediate addresses were already specified in the 
     * local internal proxy and remote proxy addresses. The gateway address in this case 
     * cannot be used.
     */
    IPERR_PROXY_GATEWAY_ADDR,

    /* IPERR_DIAL_ADDR_MUST_BE_ALIAS | The address being dialed in this case may not be an 
     * IP address or domain name.  It must be an alias because two intermediate addresses 
     * have already been specified i.e. Local Proxy, Remote Proxy and Gateway Addr
     */
    IPERR_DIAL_ADDR_MUST_BE_ALIAS,

    /* IPERR_UNRESOLVABLE_DEST_ADDRESS | No GW or GK or Proxy is specified so the destination 
     * address must be a valid resolvable address. In the case of IP based call control the 
     * address specified should be an IP address or a resolvable host or domain name.
     */
    IPERR_UNRESOLVABLE_DEST_ADDRESS,

    /* IPERR_COPYING_OR_RESOLVING_ALIAS | An error occurred while copying the alias. The error 
     * could be the result of a memory allocation failure or it could be an invalid alias 
     * format.
     */
    IPERR_COPYING_OR_RESOLVING_ALIAS,

    /* IPERR_IP_ADDRESS_NOT_AVAILABLE | Network socket layer reports that the IP address is 
     * not available.  This can happen if the system does not have a correctly configured 
     * IP address.
     */
    IPERR_IP_ADDRESS_NOT_AVAILABLE,

    /* IPERR_INVALID_PHONE_NUMBER | The phone number given is invalid
     */
    IPERR_INVALID_PHONE_NUMBER,

    /* IPERR_INVALID_URL_ADDRESS | The URL address given is invalid
     */
    IPERR_INVALID_URL_ADDRESS,

    /* IPERR_INVALID_EMAIL_ADDRESS | The email address given is invalid
     */
    IPERR_INVALID_EMAIL_ADDRESS,

    /* IPERR_INVALID_DOMAIN_NAME | The domain name given is invalid
     */
    IPERR_INVALID_DOMAIN_NAME,

    /* IPERR_INVALID_DEST_ADDRESS | The destination address did not conform to the type 
     * specified.
     */
    IPERR_INVALID_DEST_ADDRESS,

    /* IPERR_INVALID_ADDRESS_TYPE | The address type specified did not map to any known 
     * address type.
     */
    IPERR_INVALID_ADDRESS_TYPE,

    /* IPERR_CALLER_ID | Unable to allocate or copy caller ID string. 
     */
    IPERR_CALLER_ID,

    /* IPERR_ADDRESS_RESOLUTION | Unable to resolve address to a valid IP address. 
     */
    IPERR_ADDRESS_RESOLUTION,

    /* IPERR_NULL_ADDRESS | Addess given is NULL
     */
    IPERR_NULL_ADDRESS,

    /* IPERR_INVALID_IP_ADDRESS | the IP address given is invalid
     */
    IPERR_INVALID_IP_ADDRESS,

    /* IPERR_NO_INTERSECTING_CAPABILITIES | No intersecting capability found. 
     */
    IPERR_NO_INTERSECTING_CAPABILITIES,

    /* IPERR_NO_CAPABILITIES_SPECIFIED | No capabilities have been specified yet. They must 
     * either be pre-configured in the configuration file or they must be set using an 
     * extended capability API.
     */
    IPERR_NO_CAPABILITIES_SPECIFIED,

    /* IPERR_HOST_NOT_FOUND | Could not reach the party with the given host address. 
     */
    IPERR_HOST_NOT_FOUND,

    /* IPERR_FRAMESPERPACKET_NOT_SUPP | Setting frames per packet is not supported on the 
     * specified audio capability. 
     */
    IPERR_FRAMESPERPACKET_NOT_SUPP,

    /* IPERR_DLL_LOAD_FAILED | Dynamic load of a dll failed. 
     */
    IPERR_DLL_LOAD_FAILED,

    /* IPERR_INCOMING_CALL_HANDLE | The handle passed as the incoming call handle does not 
     * refer to a valid incoming call.
     */
    IPERR_INCOMING_CALL_HANDLE,

    /* IPERR_NOT_MULTIPOINT_CAPABLE | The call cannot be accepted into  a multipoint conference 
     * because there is no known multipoint controller, or the peer in a point-to-point 
     * conference is not multipoint capable.
     */
    IPERR_NOT_MULTIPOINT_CAPABLE,

    /* IPERR_UNICODE_TO_ASCII | Unable to convert the string or character from unicode or 
     * wide character format to ascii.
     */
    IPERR_UNICODE_TO_ASCII,

    /* IPERR_INVALID_DTMF_CHAR | Invalid DTMF char sent.
     */
    IPERR_INVALID_DTMF_CHAR,

    /* IPERR_NO_DTMF_CAPABILITY |Remote endpoint does not have DTMF capability.
     */
    IPERR_NO_DTMF_CAPABILITY,

    /* IPERR_DTMF_PENDING | Already in a DTMF generate state.
     */
    IPERR_DTMF_PENDING,

    /* IPERR_UNDELETED_OBJECTS | The object being deleted has child objects which haven't 
     * been deleted.
     */
    IPERR_UNDELETED_OBJECTS,

    /* IPERR_STILL_REGISTERED | The address object being deleted is
     * still registered and can't be deleted until it is unregistered.
     */
    IPERR_STILL_REGISTERED,

    /* IPERR_PENDING_RENEGOTIATION | A batched channel renegotiation is already pending.
     * This implementation does not support queuing of batched renegotiation.
     */
    IPERR_PENDING_RENEGOTIATION,

    /* IPERR_MULTIPLE_CAPS | Attaching a channel with multiple capabilities is not supported 
     * by this stack or it is not supported in this mode.
     */
    IPERR_MULTIPLE_CAPS,

    /* IPERR_INVALID_CAPS | Channel open or response failed due to invalid capabilities.
     */
    IPERR_INVALID_CAPS,

    /* IPERR_MULTIPLE_DATATYPES | Attaching a channel with multiple  data types (such as audio 
     * and video) is not permitted.  All media types proposed for a single channel must be 
     * of the same type.
     */
    IPERR_MULTIPLE_DATATYPES,

    /* IPERR_CANT_CLOSE_CHANNEL | As a result of the circumstances under which this channel 
     * was opened, it cannot be closed.  This could occur for some protocols in the scenario 
     * when channels are opened before the call is connected.  In this case the channels 
     * should be closed and deleted after hangup.
     */
    IPERR_CANT_CLOSE_CHANNEL,

    /* IPERR_NULL_ALIAS | The alias specified is NULL or empty.
     */
    IPERR_NULL_ALIAS,

    /* IPERR_INVALID_ID | An invalid ID was specified.
     */
    IPERR_INVALID_ID,

    /* IPERR_UNKNOWN_API_GUID | This is the result of either passing in a bogus GUID or one 
     * that is not found in the current DLL or executable.
     */
    IPERR_UNKNOWN_API_GUID,

    /* IPERR_ADDRESS_IN_USE | The address specified is already in use.  
     * For IP networks, this will usually occur if an attempt is made 
     * to open a socket with a port that is already in use. 
     */
    IPERR_ADDRESS_IN_USE,

    /* IPERR_UNRESOLVABLE_HOST_NAME) | The host or domain name could 
     * not be resolved to a valid address.  This will usually occur 
     * if the host or domain name is not valid or is not accessible 
     * over the existing network.
     */
    IPERR_UNRESOLVABLE_HOST_NAME,

    /* IPERR_INVALID_HOST_NAME | The host name given is invalid
     */
    IPERR_INVALID_HOST_NAME,

    /* IPERR_UNINITIALIZED | The stack has not been initialized
     */
    IPERR_UNINITIALIZED,

    /* IPERR_COPYING_OCTET_STRING | Unable to copy octet string. 
     */
    IPERR_COPYING_OCTET_STRING,

    /* IPERR_INVDEVNAME | Invalid device name */
    IPERR_INVDEVNAME,

    /* IPERR_TIMEOUT | Timeout occured while executing an internal function. */
    IPERR_TIMEOUT, 

    /* IPERR_INVALID_MEDIA_HANDLE | Specified media handle different from already attached media handle*/
    IPERR_INVALID_MEDIA_HANDLE,

    /* IPERR_MEDIA_NOT_ATTACHED | No media resource was attached to the specified linedevice */
    IPERR_MEDIA_NOT_ATTACHED,

    /* IPERR_CHANNEL_ACTIVE | Media channel is already active. */
    IPERR_CHANNEL_ACTIVE,

    /* IPERR_NO_AVAILABLE_PROPOSALS | No available proposals to respond to */
    IPERR_NO_AVAILABLE_PROPOSALS,

    /* IPERR_NOANSWER | Timeout due to no answer from peer */
    IPERR_NOANSWER,

    /* IPERR_PEER_REJECT | Peer has rejected the call placed from this endpoint */
    IPERR_PEER_REJECT,

    /* IPERR_GC_INVLINEDEV | Invalid line device. */
    IPERR_GC_INVLINEDEV,

    /* IPERR_SUP_SERV_DISABLED | Supplemental service not enabled. */
    IPERR_SUP_SERV_DISABLED,

    /* IPERR_TXRXRESOURCESINSUFF | Insufficient Tx and Rx resources. */
    IPERR_TXRXRESOURCESINSUFF,

    /* The specified value for */
    IPERR_INVALID_PARM_DATA_SIZE,

    /* Sdp Answer Missing In Application Final Responce */
    IPERR_SDP_ANSWER_MISSING,
    
    /* Resource is not Licensed. The IPCCLIB library can not obtain authorization 
       from the products license authority to use a resource (SIP signaling port 
       or 3PCC library instance).  */
    IPERR_RESOURCE_NOT_LICENSED,

    /* Embedded media not available in 3PCC mode. GC media API requests will be rejected */
    IPERR_NO_EMBEDDED_MEDIA_IN_3PCC,

    /* INVALID in 1PCC mode. 3PCC API requests will be rejected in 1PCC mode configuration */
    IPERR_INVALID_IN_1PCC,

    /* Bad Sdp Content. Reject APIs that complete transactions when an SDP offer was received 
       and no SDP answer was generated.  */
    IPERR_BAD_SDP_CONTENT,

    IP_NUM_ERR
} eIP_ERR_TYPE;


/*
 *  Result Values passed up in GC Events
 */

typedef enum {
    /* IPEC_addrRegistrationFailed | registration with the registration/admission server failed.
     */
    IPEC_addrRegistrationFailed=IP_NUM_ERR,

    /* IPEC_addrListenFailed | stack was unable to register to listen for incoming calls.
     */                                                         
    IPEC_addrListenFailed,

    /* IPEC_CHAN_REJECT_unspecified | No cause for rejection specified.
     */
    IPEC_CHAN_REJECT_unspecified,

    /* IPEC_CHAN_REJECT_dataTypeNotSupported | The terminal was not capable of 
     * supporting the dataType indicated in OpenLogicalChannel.
     */
    IPEC_CHAN_REJECT_dataTypeNotSupported,

    /* IPEC_CHAN_REJECT_dataTypeNotAvailable | The terminal was not capable of supporting the 
     * dataType indicated in OpenLogicalChannel simultaneously with the dataTypes of logical 
     * channels that are already open.
     */
    IPEC_CHAN_REJECT_dataTypeNotAvailable,

    /* IPEC_CHAN_REJECT_unknownDataType | The terminal did not understand the 
     * dataType indicated in OpenLogicalChannel.
     */
    IPEC_CHAN_REJECT_unknownDataType,

    /* IPEC_CHAN_REJECT_insuffientBandwdith | The channel could not be opened 
     * because permission to use the requested bandwidth for the logical channel was denied.
     */
    IPEC_CHAN_REJECT_insuffientBandwdith,

    /* IPEC_CHAN_REJECT_unsuitableReverseParameters | This shall only be used to 
     * reject a bi-directional logical channel request when the only reason for rejection is 
     * that the requested parameters are inappropriate. 
     */
    IPEC_CHAN_REJECT_unsuitableReverseParameters,

    /* IPEC_CHAN_REJECT_dataTypeALCombinationNotSupported | The terminal was not 
     * capable of supporting the dataType indicated in OpenLogicalChannel simultaneously with 
     * the Adaptation Layer type indicated in H223LogicalChannelParameters
     */
    IPEC_CHAN_REJECT_dataTypeALCombinationNotSupported,

    /* IPEC_CHAN_REJECT_multicastChannelNotAllowed | Multicast Channel could not be opened
     */
    IPEC_CHAN_REJECT_multicastChannelNotAllowed,

    /* IPEC_CHAN_REJECT_separateStackEstablishmentFailed | A request to run the 
     * data portion of a call on a separate stack failed.
     */
    IPEC_CHAN_REJECT_separateStackEstablishmentFailed,

    /* IPEC_CHAN_REJECT_invalidSessionID | Attempt by slave to set SessionID when 
     * opening a logical channel to the master
     */
    IPEC_CHAN_REJECT_invalidSessionID,

    /* IPEC_CHAN_REJECT_masterSlaveConflict | Attempt by slave to open logical 
     * channel in which the master has determined a conflict may occur.
     */
    IPEC_CHAN_REJECT_masterSlaveConflict,

    /* IPEC_CHAN_REJECT_waitForCommunicationMode | Attempt to open logical channel 
     * before MC has transmitted the CommunicationModeCommand
     */
    IPEC_CHAN_REJECT_waitForCommunicationMode,

    /* IPEC_CHAN_REJECT_invalidDependentChannel | Attempt to open logical channel 
     * with a dependent channel specified which is not present
     */
    IPEC_CHAN_REJECT_invalidDependentChannel,

    /* IPEC_CHAN_REJECT_replacementForRejected | A logical channel of the type 
     * attempted cannot be opened using the replacement For parameter.The transmitter may 
     * wish to re-try by firstly closing the logical channel which was to be replaced, 
     * and then opening the replacement.
     */
    IPEC_CHAN_REJECT_replacementForRejected,

    /* IPEC_CALL_END_timeout : A callback was received because a local timer expired
     */
    IPEC_CALL_END_timeout,

    /* IPEC_InternalError: An internal error occured while executing asynchronously*/
    IPEC_InternalError,

    /* IPEC_INFO_NONE_NOMORE: No more digits are available */
    IPEC_INFO_NONE_NOMORE,

    /* IPEC_INFO_PRESENT_MORE: The requested digits are now available. More/additional digits are available */
    IPEC_INFO_PRESENT_MORE,

    /* IPEC_INFO_PRESENT_ALL: The requested digits are now available */
    IPEC_INFO_PRESENT_ALL,

    /* IPEC_INFO_NONE_TIMEOUT: No digits are available - timed out */
    IPEC_INFO_NONE_TIMEOUT,

    /* IPEC_INFO_SOME_NOMORE: Only some digits are available, no more digits will be received */
    IPEC_INFO_SOME_NOMORE,

    /* IPEC_INFO_SOME_TIMEOUT: Only some digits are available - timed out */
    IPEC_INFO_SOME_TIMEOUT,

    /* IPEC_NO_MATCHING_CAPABILITIES: No intersection was found between proposed and matching capabilites */
    IPEC_NO_MATCHING_CAPABILITIES,

    /* The Alias used to register with the registration and admission server is already registered.  This failure typically results if the endpoint is already registered.  It could also occur with some servers if a registration is 
        attempted too soon after unregistering using the same alias.*/
    IPEC_REG_FAIL_duplicateAlias,
    
    /* Server registration failed due to an invalid call signalling address specified.*/
    IPEC_REG_FAIL_invalidCallSigAddress,
    
    /* The local host address specified for communicating with the server is invalid.*/
    IPEC_REG_FAIL_invalidAddress,
    
    /*The alias specified did not conform to the format rules for the type of alias
       that is was indicated to be.*/
    IPEC_REG_FAIL_invalidAlias,
    
    /*An invalid terminal type was specified by with the registration request.*/
    IPEC_REG_FAIL_invalidTermType,
    
    /* The transport type of the local host's address is not supported by the server.*/
    IPEC_REG_FAIL_invalidTransport,
    
    /* The registration request announced a transpo rt QOS that was not supported by the server.*/
    IPEC_REG_FAIL_qosNotSupported,
    
    /* Registration permission has expired.  Re-registration should be performed again.*/
    IPEC_REG_FAIL_reRegistrationRequired,
    
    /* The server rejected the registration request due to unavailability of resources. 
      This typically will occur if the server already has reached the maximum number of registrations it was configured to accept.*/
    IPEC_REG_FAIL_resourcesUnavailable,
    
    /*The server denied access for security reasons.
    This can occur if the password supplied doesn't match the password 
    on file for the alias being registered.*/
    IPEC_REG_FAIL_securityDenied,
    
    /*The server refused to allow registration for an unknown reason.*/
    IPEC_REG_FAIL_unknown,

    /*The server has gone down or is no longer responding. */
    IPEC_REG_FAIL_serverDown,

	/* NOTE: Additional result values related to Registration are located
	 *       further down in this enum. They are identifiable by their 
	 *       'IPEC_REG_FAIL_' prefix. 
	 */

    /*Attempt to call gc_media_StartSession() after establishing media channel returned error.
    */
    IPEC_MEDIA_startSessionFailed,

    /* Attempt to establish or terminate Tx channel with attached capabilities failed. The application is expected to keep the Rx
      capabilities unchanged in the next call to gc_AttachEx().*/
    IPEC_MEDIA_TxFailed,

    /* Attempt to establish or terminate Rx channel with attached capabilities failed. The application is expected to keep the Tx
      capabilities unchanged in the next call to gc_AttachEx().*/
    IPEC_MEDIA_RxFailed,

    /* Attempts to establish or terminate Tx and Rx channels with attached capabilities failed */
    IPEC_MEDIA_TxRxFailed,

    /* Attempts to establish Tx channel with attached capabilities failed. Status of other media channel is unavailable.
      Relevant with GCEV_MEDIA_REJ.*/
    IPEC_MEDIA_OnlyTxFailed,

    /* Attempts to establish Rx channel with attached capabilities failed. Status of other media channel is unavailable. 
      Relevant with GCEV_MEDIA_REJ.*/
    IPEC_MEDIA_OnlyRxFailed,

    /* Attempts to establish Tx channel with attached capabilities failed*/
    IPEC_MEDIA_TxRequired,
    
    /* Attempts to establish Rx channel with attached capabilities failed*/
    IPEC_MEDIA_RxRequired,

    /* Both channels have failed to open*/
    IPEC_TxRx_Fail,
    
    /* tx channel failed to open because of timeout*/
    IPEC_Tx_FailTimeout,
    
    /*rx channel failed to open because of timeout*/
    IPEC_Rx_FailTimeout,
    
    /* tx channel failed to open because of unknown reasons*/
    IPEC_Tx_Fail,
    
    /* rx channel failed to open because of unknown reasons*/
    IPEC_Rx_Fail,
    
    /* both channels failed because of timeout*/
    IPEC_TxRx_FailTimeout,
    
    /* both channels rejected because of unknown reasons*/
    IPEC_TxRx_Rej,
    
    /* opening of tx channel rejected because of unknown reasons*/
    IPEC_Tx_Rej,
    
    /* opening of rx channel rejected because of unknown reasons*/
    IPEC_Rx_Rej,
    
    /*channel failed to open/close because of unspecified reason*/
    IPEC_CHAN_FAILURE_unspecified,
    
    /*channel failure to open/close because of timeout*/
    IPEC_CHAN_FAILURE_timeout,
    
    /*     channel failure to open/close because of limited  resources*/
    IPEC_CHAN_FAILURE_localResources,
    
    /*both channels failed to open because of unspecified reason  */
    IPEC_FAIL_TxRx_unspecified,
    
    /* tx failed because of unspecified reason and rx because of timeout  */
    IPEC_FAIL_TxUnspecifiedRxTimeout,
    
    /* tx failed because of unspecified reason and rx because of insufficient resources */
    IPEC_FAILTxUnspecifiedRxResourceUnsuff,
    
    /*  rx failed because of unspecified reason and tx because of timeout*/
    IPEC_FAIL_RxUnspecifiedTxTimeout,
    
    /*  rx failed because of unspecified reason and tx because of insufficient resources*/
    IPEC_FAIL_RXUnspecifiedTxResourceUnsuff,
    
    /*  tx failed because of timeout and rx because of unspecified reason*/
    IPEC_FAIL_TxTimeoutRxUnspecified,
    
    /* tx and rx failed because of timeout*/
    IPEC_FAIL_TxRxTimeout,
    
    /*  tx failed because of timeout and rx because of insufficient resources*/
    IPEC_FAIL_TxTimeoutRxResourceUnsuff,
    
    /* rx failed because oftimeout and tx because of unspecified reason */
    IPEC_FAIL_RxTimeoutTXUnspecified,
    
    /*tx failed because of timeout and rx because of insufficient resources */
    IPEC_FAIL_RxTimeoutTxResourceUnsuff,

    /*tx failed because of insufficient resourcces and rx because of unspecified resources*/
    IPEC_FAIL_TxResourceUnsuffRxUnspecified,
    
    /*tx failed because of insufficient resources and rx because of timeout */
    IPEC_FAIL_TxResourceUnsuffRxTimeout,

    /* tx and rx failed because of insufficient resources*/
    IPEC_FAIL_TxRxResourceUnsuff,

    /* tx failed because of unspecified reason and rx because of insufficient resources*/
    IPEC_FAIL_RxResourceUnsuffTxUnspecified,

    /* tx failed because of timeout and rx because of insufficient resources*/
    IPEC_FAIL_RxResourceUnsuffTxTimeout,
    
    /* Incoming call is result of transfer*/
    IPEC_XFER_TO,

	
	/* NOTE: The following 7 result values are related to Registration. 
	 */
	/* Indicates that there was a mismatch between the internal IP CCLib
	   data, and the data contained in the Registrar's response. */
	IPEC_REG_FAIL_serverResponseDataMismatch,

    /* Indicates a REGISTER transaction is currently in progress with the
	   specified Registrar and A.O.R.  A new request to this same Registrar
	   and A.O.R. can not be gererated at this time, try again after the
	   current pending request completes. */
	IPEC_REG_FAIL_registrationTransactionInProgress,

    /* Indicates the RadVision Stack ran out of resources to process this 
       request. */
	IPEC_REG_FAIL_insufficientInternalResources,

    /* Indicates that there was an internal IP CCLib error encountered 
	   while attempting to formulate an outgoing REGISTER request. */
    IPEC_REG_FAIL_internalError,

    /* Indicates that an "expires=" Contact: header parameter value 
	   was invalid for the current operation.  */
    IPEC_REG_FAIL_invalidExpires,

    /* Indicates that there was an Timeout error waiting for a 
	   REGISTER response from the Registsrar.  */
    IPEC_REG_FAIL_responseTimeout,    
    
    /* Indicates that a network error prevented the REGISTER request 
	   from being sent.  */	
	IPEC_REG_FAIL_networkError,


    /* Invalid in 3PCC */
    IPEC_NoMediaIn3PCC,

    /* Invalid in 1PCC mode */
    IPEC_NoSdpIn1PCC,

    /* Subscription Expired */
    IPEC_SUBS_EXPIRED,

    /* No NOTIFY was received before timeout */
    IPEC_NO_NOTIFY_TIME_OUT,

    /* Subscription transaction time out */
    IPEC_SUBS_REASON_TRANSC_TIME_OUT,


	/* Indicates that a SIP Header was too big for the current max
	   parameter size specified during gc_Start() (defaults to 255),
	   and so truncation of the value happened. */
    IPEC_SipHeaderTruncation,


    /* Enumeration of values for Internal Disconnect Reasons. */
    IPEC_InternalReasonMin = 1000,      /* 0x3e8 (Internal Start marker) */
    IPEC_InternalReasonBusy,            /* 0x3e9 (1001) ) */
    IPEC_InternalReasonCallCompletion,  /* 0x3ea (1002) */
    IPEC_InternalReasonCanceled,        /* 0x3eb (1003) */
    IPEC_InternalReasonCongestion,      /* 0x3ec (1004) */
    IPEC_InternalReasonDestBusy,        /* 0x3ed (1005) */
    IPEC_InternalReasonDestAddrBad,     /* 0x3ee (1006) */
    IPEC_InternalReasonDestOutOfOrder,  /* 0x3ef (1007) */
    IPEC_InternalReasonDestUnobtainable,/* 0x3f0 (1008) */
    IPEC_InternalReasonForward,         /* 0x3f1 (1009) */
    IPEC_InternalReasonIncompatible,    /* 0x3f2 (1010) */
    IPEC_InternalReasonIncomingCall,    /* 0x3f3 (1011) */
    IPEC_InternalReasonNewCall,         /* 0x3f4 (1012) */
    IPEC_InternalReasonNoAnswer,        /* 0x3f5 (1013) */
    IPEC_InternalReasonNormal,          /* 0x3f6 (1014) */
    IPEC_InternalReasonNetworkAlarm,    /* 0x3f7 (1015) */
    IPEC_InternalReasonPickUp,          /* 0x3f8 (1016) */
    IPEC_InternalReasonProtocolError,   /* 0x3f9 (1017) */
    IPEC_InternalReasonRedirection,     /* 0x3fa (1018) */
    IPEC_InternalReasonRemoteTermination, /* 0x3fb (1019) */
    IPEC_InternalReasonRejection,       /* 0x3fc (1020) */
    IPEC_InternalReasonSIT,             /* 0x3fd(1021) */
    IPEC_InternalReasonSITCustIrreg,    /* 0x3fe (1022) */
    IPEC_InternalReasonSITNoCircuit,    /* 0x3ff (1023) */
    IPEC_InternalReasonSITReorder,      /* 0x400 (1024) */
    IPEC_IncomingTransfer,              /* 0x401 (1025) */
    IPEC_InternalReasonUnavailable,     /* 0x402 (1026) */
    IPEC_InternalReasonUnknown,         /* 0x403 (1027) */
    IPEC_InternalReasonUnallocatedNumber,   /* 0x404 (1028) */
    IPEC_InternalReasonNoRoute,         /* 0x405 (1029) */
    IPEC_InternalReasonNumberChanged,   /* 0x406 (1030) */
    IPEC_InternalReasonOutOfOrder,      /* 0x407 (1031) */
    IPEC_InternalReasonInvalidFormat,   /* 0x408 (1032) */
    IPEC_InternalReasonChanUnavailable, /* 0x409 (1033) */
    IPEC_InternalReasonChanUnacceptable,/* 0x40a (1034) */
    IPEC_InternalReasonChanNotImplemented,      /* 0x40b (1035) */
    IPEC_InternalReasonNoChan,          /* 0x40c (1036) */
    IPEC_InternalReasonNoResponse,      /* 0x40d (1037) */
    IPEC_InternalReasonFacilityNotSubscribed,   /* 0x40e (1038) */
    IPEC_InternalReasonFacilityNotImplemented,  /* 0x40f (1039) */
    IPEC_InternalReasonServiceNotImplemented,   /* 0x410 (1040) */
    IPEC_InternalReasonBarredInbound,   /* 0x411 (1041) */
    IPEC_InternalReasonBarredOutbound,  /* 0x412 (1042) */
    IPEC_InternalReasonDestIncompatible,/* 0x413 (1043) */
    IPEC_InternalReasonBearerCapUnavailable, /* 0x414 (1044) */
    IPEC_InternalReasonMax,             /* 0x415 (1045) Internal End marker */    

    /* See ITU-T H.225 reasons */
    IPEC_H2250ReasonMin = 1999,         /* 0x7cf ( H2250Reason Start Marker */
    IPEC_H2250ReasonNoBandwidth,        /* 0x7d0 (2000) */
    IPEC_H2250ReasonGatekeeperResource, /* 0x7d1 (2001) */
    IPEC_H2250ReasonUnreachableDestination,     /* 0x7d2 (2002) */
    IPEC_H2250ReasonDestinationRejection,       /* 0x7d3 (2003) */
    IPEC_H2250ReasonInvalidRevision,    /* 0x7d4 (2004) */
    IPEC_H2250ReasonNoPermision,        /* 0x7d5 (2005) */
    IPEC_H2250ReasonUnreachableGatekeeper,      /* 0x7d6 (2006) */
    IPEC_H2250ReasonGatewayResource,    /* 0x7d7 (2007) */
    IPEC_H2250ReasonBadFormatAddress,   /* 0x7d8 (2008) */
    IPEC_H2250ReasonAdaptiveBusy,       /* 0x7d9 (2009) */
    IPEC_H2250ReasonInConf,             /* 0x7da (2010) */
    IPEC_H2250ReasonUndefinedReason,    /* 0x7db (2011) */
    IPEC_H2250ReasonFacilityCallDeflection, /* 0x7dc (2012) */
    IPEC_H2250ReasonSecurityDenied,     /* 0x7dd (2013) */
    IPEC_H2250ReasonCalledPartyNotRegistered, /* 0x7de (2014) */        
    IPEC_H2250ReasonCallerNotregistered,/* 0x7df (2015) */
    IPEC_H2250ReasonMax,                /* 0x7e0 (2016(H2250Reason End marker) */

    /* H.450 Cause values */
    IPEC_H450UserNotSubscribed = 2500,     /* 0x9c4 (2500) */
    IPEC_H450RejectedByNetork,             /* 0x9c5 (2501) */
    IPEC_H450RejectedByUser,               /* 0x9c6 (2502) */
    IPEC_H450NotAvailable,                 /* 0x9c7 (2503) */
    IPEC_H450InsufficientInformation,      /* 0x9c8 (2504) */
    IPEC_H450InvalidUserNumber,            /* 0x9c9 (2505) */
    IPEC_H450InvalidCallState,             /* 0x9ca (2506) */
    IPEC_H450BasicServiceNotProvided,      /* 0x9cb (2507) */
    IPEC_H450NotIncomingCall,              /* 0x9cc (2508) */
    IPEC_H450SuppServInteractionNotAllowed,/* 0x9cd (2509) */
    IPEC_H450ResourceUnavailble,           /* 0x9ce (2510) */
    IPEC_H450CallFailure,                  /* 0x9cf (2511) */
    IPEC_H450ProceedureError,              /* 0x9d0 (2512) */

    /* H.450.2 Errors */
    IPEC_H4502InvalidReroutingNumber,      /* 0x9d1 (2513) */
    IPEC_H4502UnrecognizedCallIdentity,    /* 0x9d2 (2514) */
    IPEC_H4502EstablishmentFailure,        /* 0x9d3 (2515) */
    IPEC_H4502Unspecified,                 /* 0x9d4 (2516) */
    IPEC_H4502CTAbandon,                   /* 0x9d5 (2517) */
    IPEC_H4502TRTSEUnspecified,            /* 0x9d6 (2518) */
    IPEC_H4502TRTSENotAvailable,           /* 0x9d7 (2519) */
    IPEC_H4502TRTSEInvalidCallState,       /* 0x9d8 (2520) */
    IPEC_H4502TRTSESuppServInteractionNotAllowed,  /* 0x9d9 (2521) */

    /* H.450.2 Timeouts */
    IPEC_H450CTT1Timeout,                  /* 0x9da (2522) */
    IPEC_H450CTT2Timeout,                  /* 0x9db (2523) */
    IPEC_H450CTT3Timeout,                  /* 0x9dc (2524) */
    IPEC_H450CTT4Timeout,                  /* 0x9dd (2525) */

    /* Q.931 Cause values */
    IPEC_Q931CauseMin = 3000,           /* 0xbb8 (Q931Cause Start marker See ITU-T Q.931) */
    IPEC_Q931Cause01UnassignedNumber = 3001,    /* 0xbb9 */
    IPEC_Q931Cause02NoRouteToSpecifiedTransitNetwork = 3002, /* 0xbba */
    IPEC_Q931Cause03NoRouteToDestination = 3003,/* 0xbbb */
    IPEC_Q931Cause06ChannelUnacceptable = 3006, /* 0xbbe */
    IPEC_Q931Cause07CallAwardedAndBeingDeliveredInAnEstablishedChannel = 3007, /* 0xbbf */
    IPEC_Q931Cause16NormalCallClearing = 3016,  /* 0xbc8 */
    IPEC_Q931Cause17UserBusy = 3017,            /* 0xbc9 */
    IPEC_Q931Cause18NoUserResponding = 3018,    /* 0xbca */
    IPEC_Q931Cause19UserAlertingNoAnswer = 3019,/* 0xbcb */
    IPEC_Q931Cause21CallRejected = 3021,        /* 0xbcd */
    IPEC_Q931Cause22NumberChanged = 3022,       /* 0xbce */
    IPEC_Q931Cause26NonSelectedUserClearing = 3026, /* 0xbd2 */
    IPEC_Q931Cause27DestinationOutOfOrder = 3027,   /* 0xbd3 */
    IPEC_Q931Cause28InvalidNumberFormatIncompleteNumber = 3028, /* 0xbd4 */
    IPEC_Q931Cause29FacilityRejected = 3029,        /* 0x bd5 */
    IPEC_Q931Cause30ResponseToSTATUSENQUIRY = 3030, /* 0xbd6 */
    IPEC_Q931Cause31NormalUnspecified = 3031,       /* 0xbd7 */
    IPEC_Q931Cause34NoCircuitChannelAvailable = 3034, /* 0xbda */
    IPEC_Q931Cause38NetworkOutOfOrder = 3038,       /* 0xbde */
    IPEC_Q931Cause41TemporaryFailure = 3041,        /* 0xbe1 */
    IPEC_Q931Cause42SwitchingEquipmentCongestion = 3042,/* 0xbe2 */
    IPEC_Q931Cause43AccessInformationDiscarded = 3043,  /* 0xbe3 */
    IPEC_Q931Cause44RequestedCircuitChannelNotAvailable = 3044, /* 0xbe4 */
    IPEC_Q931Cause47ResourceUnavailableUnspecified = 3047,  /* 0xbe7 */
    IPEC_Q931Cause57BearerCapabilityNotAuthorized = 3057,   /* 0xbf1 */
    IPEC_Q931Cause58BearerCapabilityNotPresentlyAvailable = 3058, /* 0xbf2 */
    IPEC_Q931Cause63ServiceOrOptionNotAvailableUnspecified = 3063, /* 0xbf7 */
    IPEC_Q931Cause65BearerCapabilityNotImplemented = 3065,  /* 0xbf9 */
    IPEC_Q931Cause66ChannelTypeNotImplemented = 3066,       /* 0xbfa */
    IPEC_Q931Cause69RequestedFacilityNotImplemented = 3069, /* 0xbfd */
    IPEC_Q931Cause70OnlyRestrictedDigitalInformationBearerCapabilityIsAvailable=3070, /* 0xbfe*/      
    IPEC_Q931Cause79ServiceOrOptionNotImplementedUnspecified = 3079, /* 0xc07 */
    IPEC_Q931Cause81InvalidCallReferenceValue = 3081,       /* 0xc09 */
    IPEC_Q931Cause82IdentifiedChannelDoesNotExist = 3082,   /* 0xc0a */
    IPEC_Q931Cause83AsuspendedCallExistsButThisCallIdentityDoesNot = 3083, /* 0xcob */
    IPEC_Q931Cause84CallIdentityInUse = 3084,               /* 0xc0c */
    IPEC_Q931Cause85NoCallSuspended = 3085,                 /* 0xc0d */
    IPEC_Q931Cause86CallHavingTheRequestedCallIdentityHasBeenCleared = 3086, /* 0xc0e */
    IPEC_Q931Cause88IncompatibleDestination = 3088,         /* 0xc10 */
    IPEC_Q931Cause91InvalidTransitNetworkSelection = 3091,  /* 0xc13 */
    IPEC_Q931Cause95InvalidMessageUnspecified = 3095,       /* 0xc17 */
    IPEC_Q931Cause96MandatoryInformationElementIsMissing = 3096,    /* 0xc18 */
    IPEC_Q931Cause97MessageTypeNonExistentOrNotImplemented = 3097,  /* 0xc19 */
    IPEC_Q931Cause100InvalidInformationElementContents = 3100,      /* 0xc1c */
    IPEC_Q931Cause101MessageNotCompatibleWithCallState = 3101,      /* 0xc1d */
    IPEC_Q931Cause102RecoveryOnTimeExpiry = 3102,           /* 0xc1e */
    IPEC_Q931Cause111ProtocolErrorUnspecified = 3111,       /* 0xc27 */
    IPEC_Q931Cause127InterworkingUnspecified = 3127,        /* 0xc37 */
    IPEC_Q931CauseMax, /* 0xc38 Q931Cause End marker */

    /* H.245 Cause values */
    IPEC_H245ChannelFailure_Min = 3500,                     /* 0xdac (3500) */
    /* H.245 Channel establishment failed because of a transport error. */
    IPEC_H245EstChannelFailure_TransportError = 3501,       /* 0xdad (3501) */
    /* H.245 Channel establishment failed because remote side rejected a H.245 establishment request. */
    IPEC_H245EstChannelFailure_RemoteReject = 3502,         /* 0xdae (3502) */
    /* H.245 Channel establishment failed because TerminalCapabilitySet procedures failed. */
    IPEC_H245EstChannelFailure_TCSError = 3503,             /* 0xdaf (3503) */
    /* H.245 Channel establishment failed because MasterSlave negotiation failed. */
    IPEC_H245EstChannelFailure_MSDError = 3504,             /* 0xdb0 (3504) */
    IPEC_H245ChannelFailure_Max,                     

    /* Non ReleaseComplete call disconnect reasons */
    /* Call disconnected because of H.245 transport error */
    IPEC_H245TransportError = 3700,                         /* 0xe74 (3700) */
    /* Call disconnected because of Q.931 transport error */
    IPEC_Q931TransportError = 3701,                         /* 0xe75 (3701) */
    /* Call disconnected because of invalid Q.931 message received */
    IPEC_InvalidQ931Message = 3702,                         /* 0xe76 (3702) */
    /* Call disconnected because of Q.931 T303 Setup response timeout */
    IPEC_Q931T303SetupTimeout = 3703,                       /* 0xe77 (3703) */

    /* RAS Cause values */
    IPEC_RASReasonMin=4000,                                 /* 0xfa0 */
    IPEC_RASReasonResourceUnavailable,                      /* 0xfa1 */
    IPEC_RASReasonInsufficientResources,                    /* 0xfa2 */
    IPEC_RASReasonInvalidRevision,                          /* 0xfa3 */
    IPEC_RASReasonInvalidCallSignalAddress,                 /* 0xfa4 */
    IPEC_RASReasonInvalidIPEC_RASAddress,                   /* 0xfa5 */
    IPEC_RASReasonInvalidTerminalType,                      /* 0xfa6 */
    IPEC_RASReasonInvalidPermission,                        /* 0xfa7 */
    IPEC_RASReasonInvalidConferenceID,                      /* 0xfa8 */
    IPEC_RASReasonInvalidEndpointID,                        /* 0xfa9 */
    IPEC_RASReasonCallerNotRegistered,                      /* 0xfaa */
    IPEC_RASReasonCalledPartyNotRegistered,                 /* 0xfab */
    IPEC_RASReasonDiscoveryRequired,                        /* 0xfac */
    IPEC_RASReasonDuplicateAlias,                           /* 0xfad */
    IPEC_RASReasonTransportNotSupported,                    /* 0xfae */
    IPEC_RASReasonCallInProgress,                           /* 0xfaf */
    IPEC_RASReasonRouteCallToGatekeeper,                    /* 0xfb0 */
    IPEC_RASReasonRequestToDropOther,                       /* 0xfb1 */
    IPEC_RASReasonNotRegistered,                            /* 0xfb2 */
    IPEC_RASReasonUndefined,                                /* 0xfb3 */
    IPEC_RASReasonTerminalExcluded,                         /* 0xfb4 */
    IPEC_RASReasonNotBound,                                 /* 0xfb5 */
    IPEC_RASReasonNotCurrentlyRegistered,                   /* 0xfb6 */
    IPEC_RASReasonRequestDenied,                            /* 0xfb7 */
    IPEC_RASReasonLocationNotFound,                         /* 0xfb8 */
    IPEC_RASReasonSecurityDenial,                           /* 0xfb9 */
    IPEC_RASTransportQOSNotSupported,                       /* 0xfba */
    IPEC_RASResourceUnavailable,                            /* 0xfbb */
    IPEC_RASInvalidAlias,                                   /* 0xfbc */
    IPEC_RASPermissionDenied,                               /* 0xfbd */
    IPEC_RASQOSControlNotSupported,                         /* 0xfbe */
    IPEC_RASIncompleteAddress,                              /* 0xfbf */
    IPEC_RASFullRegistrationRequired,                       /* 0xfc0 */
    IPEC_RASRouteCallToSCN,                                 /* 0xfc1 */
    IPEC_RASAliasesInconsistent,                            /* 0xfc2 */
    IPEC_RASReasonMax,                                      /* 0xfc3 */

    /* SIP response code values */
    IPEC_SIPReasonStatusMin = 5000,                            /* 0x1388 */
    IPEC_SIPReasonStatus300MultipleChoices = 5300,             /* 0x14b4 */
    IPEC_SIPReasonStatus301MovedPermanently = 5301,            /* 0x14b5 */
    IPEC_SIPReasonStatus302MovedTemporarily = 5302,            /* 0x14b6 */
    IPEC_SIPReasonStatus303SeeOther = 5303,                    /* 0x14b7*/
    IPEC_SIPReasonStatus305UseProxy = 5305,                    /* 0x1469 */
    IPEC_SIPReasonStatus380AlternativeService = 5380,          /* 0x1504 */
    IPEC_SIPReasonStatus400BadRequest = 5400,                  /* 0x1518 */
    IPEC_SIPReasonStatus401Unauthorized = 5401,                /* 0x1519 */
    IPEC_SIPReasonStatus402PaymentRequired = 5402,             /* 0x151a */
    IPEC_SIPReasonStatus403Forbidden = 5403,                   /* 0x151b */
    IPEC_SIPReasonStatus404NotFound = 5404,                    /* 0x151c */
    IPEC_SIPReasonStatus405MethodNotAllowed =5405,             /* 0x151d */
    IPEC_SIPReasonStatus406NotAcceptable = 5406,               /* 0x151e */
    IPEC_SIPReasonStatus407ProxyAuthenticationRequired = 5407, /* 0x151f */
    IPEC_SIPReasonStatus408RequestTimeout = 5408,              /* 0x1520 */
    IPEC_SIPReasonStatus410Gone = 5410,                        /* 0x1522 */
    IPEC_SIPReasonStatus413RequestEntityTooLarge = 5413,       /* 0x1525 */
    IPEC_SIPReasonStatus414RequestUriTooLong = 5414,           /* 0x1526 */
    IPEC_SIPReasonStatus415UnsupportedMediaType = 5415,        /* 0x1527 */
    IPEC_SIPReasonStatus416UnsupportedURIScheme = 5416,        /* 0x1528 */
    IPEC_SIPReasonStatus420BadExtension = 5420,                /* 0x153c */
    IPEC_SIPReasonStatus421ExtensionRequired = 5421,           /* 0x153d */
    IPEC_SIPReasonStatus422SessionIntervalTooSmall = 5422,     /* 0x153e */
    IPEC_SIPReasonStatus423IntervalTooBrief = 5423,            /* 0x153f */
    IPEC_SIPReasonStatus480TemporarilyUnavailable = 5480,      /* 0x1568 */
    IPEC_SIPReasonStatus481CallTransactionDoesNotExist = 5481, /* 0x1569 */
    IPEC_SIPReasonStatus482LoopDetected = 5482,                /* 0x156a */
    IPEC_SIPReasonStatus483TooManyHops = 5483,                 /* 0x156b */
    IPEC_SIPReasonStatus484AddressIncomplete =5484,            /* 0x156c */
    IPEC_SIPReasonStatus485Ambiguous = 5485,                   /* 0x156d */
    IPEC_SIPReasonStatus486BusyHere = 5486,                    /* 0x156e */
    IPEC_SIPReasonStatus487RequestTerminated = 5487,           /* 0x156f */
    IPEC_SIPReasonStatus488NotAcceptableHere = 5488,           /* 0x1570 */
    IPEC_SIPReasonStatus491RequestPending = 5491,              /* 0x1573 */
    IPEC_SIPReasonStatus493Undecipherable = 5493,              /* 0x1575 */
    IPEC_SIPReasonStatus500ServerInternalError = 5500,         /* 0x157c */
    IPEC_SIPReasonStatus501NotImplemented = 5501,              /* 0x157d */
    IPEC_SIPReasonStatus502BadGateway = 5502,                  /* 0x157e */
    IPEC_SIPReasonStatus503ServiceUnavailable = 5503,          /* 0x157f */
    IPEC_SIPReasonStatus504ServerTimeout = 5504,               /* 0x1580 */
    IPEC_SIPReasonStatus505VersionNotSupported = 5505,         /* 0x1581 */
    IPEC_SIPReasonStatus513MessageTooLarge = 5513,             /* 0x1589 */
    IPEC_SIPReasonStatus600BusyEverywhere = 5600,              /* 0x15e0 */
    IPEC_SIPReasonStatus603Decline = 5603,                     /* 0x15e3 */
    IPEC_SIPReasonStatus604DoesNotExistAnywhere = 5604,        /* 0x15e4 */
    IPEC_SIPReasonStatus606NotAcceptable = 5606,               /* 0x15e6 */
    IPEC_SIPReasonStatusBYE = 5800,                            /* 0x16a8 */
    IPEC_SIPReasonStatusCANCEL = 5801,                         /* 0x16a9 */
    /*MIME failure code, use undefined response code in RFC */
    IPEC_SIPReasonStatusMIMEPoolEmpty = 5802 ,                 /* 0x16aa */
    IPEC_SIPReasonStatusMIMEBuffTooSmall = 5803,               /* 0x16ab */
    IPEC_InternalHardwareFailure = 5804,                       /* 0x16ac */
    IPEC_SIPReasonStatusMax,

    IP_NUM_ERR_EC
} eIP_EC_TYPE;


/* Conversion Macros | Use them to convert between IPT/GC defines (IPEC_xxx enumerator)
 * and the standard values defined in H.225 and Q.931
 */

#define H2250_REASON_TO_IPTGC_REASON(H2250Reason, RetVal)\
RetVal = H2250Reason + IPEC_H2250ReasonMin + 1;\
if((RetVal<=IPEC_H2250ReasonMin) || ((0-RetVal)<=(0-IPEC_H2250ReasonMax))){\
RetVal=0xffffffff;\
}


#define IPTGC_REASON_TO_H2250_REASON(IPTGCReason, RetVal)\
if((IPTGCReason<=IPEC_H2250ReasonMin) || ((0-IPTGCReason)<=(0-IPEC_H2250ReasonMax))){\
RetVal=0xffffffff;\
}\
else{\
RetVal= IPTGCReason - IPEC_H2250ReasonMin-1;\
}


#define Q931_CAUSE_TO_IPTGC_CAUSE(Q931Cause, RetVal){\
RetVal = Q931Cause + IPEC_Q931CauseMin;\
switch(RetVal){\
case IPEC_Q931Cause01UnassignedNumber:\
case IPEC_Q931Cause02NoRouteToSpecifiedTransitNetwork:\
case IPEC_Q931Cause03NoRouteToDestination:\
case IPEC_Q931Cause06ChannelUnacceptable:\
case IPEC_Q931Cause07CallAwardedAndBeingDeliveredInAnEstablishedChannel:\
case IPEC_Q931Cause16NormalCallClearing:\
case IPEC_Q931Cause17UserBusy:\
case IPEC_Q931Cause18NoUserResponding:\
case IPEC_Q931Cause19UserAlertingNoAnswer:\
case IPEC_Q931Cause21CallRejected:\
case IPEC_Q931Cause22NumberChanged:\
case IPEC_Q931Cause26NonSelectedUserClearing:\
case IPEC_Q931Cause27DestinationOutOfOrder:\
case IPEC_Q931Cause28InvalidNumberFormatIncompleteNumber:\
case IPEC_Q931Cause29FacilityRejected:\
case IPEC_Q931Cause30ResponseToSTATUSENQUIRY:\
case IPEC_Q931Cause31NormalUnspecified:\
case IPEC_Q931Cause34NoCircuitChannelAvailable:\
case IPEC_Q931Cause38NetworkOutOfOrder:\
case IPEC_Q931Cause41TemporaryFailure:\
case IPEC_Q931Cause42SwitchingEquipmentCongestion:\
case IPEC_Q931Cause43AccessInformationDiscarded:\
case IPEC_Q931Cause44RequestedCircuitChannelNotAvailable:\
case IPEC_Q931Cause47ResourceUnavailableUnspecified:\
case IPEC_Q931Cause57BearerCapabilityNotAuthorized:\
case IPEC_Q931Cause58BearerCapabilityNotPresentlyAvailable:\
case IPEC_Q931Cause63ServiceOrOptionNotAvailableUnspecified:\
case IPEC_Q931Cause65BearerCapabilityNotImplemented:\
case IPEC_Q931Cause66ChannelTypeNotImplemented:\
case IPEC_Q931Cause69RequestedFacilityNotImplemented:\
case IPEC_Q931Cause70OnlyRestrictedDigitalInformationBearerCapabilityIsAvailable:\
case IPEC_Q931Cause79ServiceOrOptionNotImplementedUnspecified:\
case IPEC_Q931Cause81InvalidCallReferenceValue:\
case IPEC_Q931Cause82IdentifiedChannelDoesNotExist:\
case IPEC_Q931Cause83AsuspendedCallExistsButThisCallIdentityDoesNot:\
case IPEC_Q931Cause84CallIdentityInUse:\
case IPEC_Q931Cause85NoCallSuspended:\
case IPEC_Q931Cause86CallHavingTheRequestedCallIdentityHasBeenCleared:\
case IPEC_Q931Cause88IncompatibleDestination:\
case IPEC_Q931Cause91InvalidTransitNetworkSelection:\
case IPEC_Q931Cause95InvalidMessageUnspecified:\
case IPEC_Q931Cause96MandatoryInformationElementIsMissing:\
case IPEC_Q931Cause97MessageTypeNonExistentOrNotImplemented:\
case IPEC_Q931Cause100InvalidInformationElementContents:\
case IPEC_Q931Cause101MessageNotCompatibleWithCallState:\
case IPEC_Q931Cause102RecoveryOnTimeExpiry:\
case IPEC_Q931Cause111ProtocolErrorUnspecified:\
case IPEC_Q931Cause127InterworkingUnspecified:\
break;\
default:\
RetVal=0xffffffff;\
}\
}


#define IPTGC_CAUSE_TO_Q931_CAUSE(IPTGCCause, RetVal){\
switch(IPTGCCause){\
case IPEC_Q931Cause01UnassignedNumber:\
case IPEC_Q931Cause02NoRouteToSpecifiedTransitNetwork:\
case IPEC_Q931Cause03NoRouteToDestination:\
case IPEC_Q931Cause06ChannelUnacceptable:\
case IPEC_Q931Cause07CallAwardedAndBeingDeliveredInAnEstablishedChannel:\
case IPEC_Q931Cause16NormalCallClearing:\
case IPEC_Q931Cause17UserBusy:\
case IPEC_Q931Cause18NoUserResponding:\
case IPEC_Q931Cause19UserAlertingNoAnswer:\
case IPEC_Q931Cause21CallRejected:\
case IPEC_Q931Cause22NumberChanged:\
case IPEC_Q931Cause26NonSelectedUserClearing:\
case IPEC_Q931Cause27DestinationOutOfOrder:\
case IPEC_Q931Cause28InvalidNumberFormatIncompleteNumber:\
case IPEC_Q931Cause29FacilityRejected:\
case IPEC_Q931Cause30ResponseToSTATUSENQUIRY:\
case IPEC_Q931Cause31NormalUnspecified:\
case IPEC_Q931Cause34NoCircuitChannelAvailable:\
case IPEC_Q931Cause38NetworkOutOfOrder:\
case IPEC_Q931Cause41TemporaryFailure:\
case IPEC_Q931Cause42SwitchingEquipmentCongestion:\
case IPEC_Q931Cause43AccessInformationDiscarded:\
case IPEC_Q931Cause44RequestedCircuitChannelNotAvailable:\
case IPEC_Q931Cause47ResourceUnavailableUnspecified:\
case IPEC_Q931Cause57BearerCapabilityNotAuthorized:\
case IPEC_Q931Cause58BearerCapabilityNotPresentlyAvailable:\
case IPEC_Q931Cause63ServiceOrOptionNotAvailableUnspecified:\
case IPEC_Q931Cause65BearerCapabilityNotImplemented:\
case IPEC_Q931Cause66ChannelTypeNotImplemented:\
case IPEC_Q931Cause69RequestedFacilityNotImplemented:\
case IPEC_Q931Cause70OnlyRestrictedDigitalInformationBearerCapabilityIsAvailable:\
case IPEC_Q931Cause79ServiceOrOptionNotImplementedUnspecified:\
case IPEC_Q931Cause81InvalidCallReferenceValue:\
case IPEC_Q931Cause82IdentifiedChannelDoesNotExist:\
case IPEC_Q931Cause83AsuspendedCallExistsButThisCallIdentityDoesNot:\
case IPEC_Q931Cause84CallIdentityInUse:\
case IPEC_Q931Cause85NoCallSuspended:\
case IPEC_Q931Cause86CallHavingTheRequestedCallIdentityHasBeenCleared:\
case IPEC_Q931Cause88IncompatibleDestination:\
case IPEC_Q931Cause91InvalidTransitNetworkSelection:\
case IPEC_Q931Cause95InvalidMessageUnspecified:\
case IPEC_Q931Cause96MandatoryInformationElementIsMissing:\
case IPEC_Q931Cause97MessageTypeNonExistentOrNotImplemented:\
case IPEC_Q931Cause100InvalidInformationElementContents:\
case IPEC_Q931Cause101MessageNotCompatibleWithCallState:\
case IPEC_Q931Cause102RecoveryOnTimeExpiry:\
case IPEC_Q931Cause111ProtocolErrorUnspecified:\
case IPEC_Q931Cause127InterworkingUnspecified:\
RetVal=IPTGCCause-IPEC_Q931CauseMin;\
break;\
default:\
RetVal=0xffffffff;\
}\
}


#define SIP_RESPONSE_TO_IPTGC_CAUSE(SipResponse,RetVal)\
if(300 <= SipResponse && 1000 > SipResponse) \
{ RetVal = SipResponse + IPEC_SIPReasonStatusMin;\
} else { RetVal = SipResponse;}

#define IPTGC_CAUSE_TO_SIP_RESPONSE(IPTGCCause, RetVal){\
switch(IPTGCCause){\
   case IPEC_SIPReasonStatus300MultipleChoices:\
   case IPEC_SIPReasonStatus301MovedPermanently:\
   case IPEC_SIPReasonStatus302MovedTemporarily:\
   case IPEC_SIPReasonStatus303SeeOther:\
   case IPEC_SIPReasonStatus305UseProxy:\
   case IPEC_SIPReasonStatus380AlternativeService:\
   case IPEC_SIPReasonStatus400BadRequest:\
   case IPEC_SIPReasonStatus401Unauthorized:\
   case IPEC_SIPReasonStatus402PaymentRequired:\
   case IPEC_SIPReasonStatus403Forbidden:\
   case IPEC_SIPReasonStatus404NotFound:\
   case IPEC_SIPReasonStatus405MethodNotAllowed:\
   case IPEC_SIPReasonStatus406NotAcceptable:\
   case IPEC_SIPReasonStatus407ProxyAuthenticationRequired:\
   case IPEC_SIPReasonStatus408RequestTimeout:\
   case IPEC_SIPReasonStatus410Gone:\
   case IPEC_SIPReasonStatus413RequestEntityTooLarge:\
   case IPEC_SIPReasonStatus414RequestUriTooLong:\
   case IPEC_SIPReasonStatus415UnsupportedMediaType:\
   case IPEC_SIPReasonStatus416UnsupportedURIScheme:\
   case IPEC_SIPReasonStatus420BadExtension:\
   case IPEC_SIPReasonStatus421ExtensionRequired:\
   case IPEC_SIPReasonStatus422SessionIntervalTooSmall: \
   case IPEC_SIPReasonStatus423IntervalTooBrief:\
   case IPEC_SIPReasonStatus480TemporarilyUnavailable:\
   case IPEC_SIPReasonStatus481CallTransactionDoesNotExist:\
   case IPEC_SIPReasonStatus482LoopDetected:\
   case IPEC_SIPReasonStatus483TooManyHops:\
   case IPEC_SIPReasonStatus484AddressIncomplete:\
   case IPEC_SIPReasonStatus485Ambiguous:\
   case IPEC_SIPReasonStatus486BusyHere:\
   case IPEC_SIPReasonStatus487RequestTerminated:\
   case IPEC_SIPReasonStatus488NotAcceptableHere:\
   case IPEC_SIPReasonStatus491RequestPending:\
   case IPEC_SIPReasonStatus493Undecipherable:\
   case IPEC_SIPReasonStatus500ServerInternalError:\
   case IPEC_SIPReasonStatus501NotImplemented:\
   case IPEC_SIPReasonStatus502BadGateway:\
   case IPEC_SIPReasonStatus503ServiceUnavailable:\
   case IPEC_SIPReasonStatus504ServerTimeout:\
   case IPEC_SIPReasonStatus505VersionNotSupported:\
   case IPEC_SIPReasonStatus513MessageTooLarge:\
   case IPEC_SIPReasonStatus600BusyEverywhere:\
   case IPEC_SIPReasonStatus603Decline:\
   case IPEC_SIPReasonStatus604DoesNotExistAnywhere:\
   case IPEC_SIPReasonStatus606NotAcceptable:\
   case IPEC_SIPReasonStatusBYE:\
   case IPEC_SIPReasonStatusCANCEL:\
   case IPEC_SIPReasonStatusMIMEPoolEmpty:\
   case IPEC_SIPReasonStatusMIMEBuffTooSmall:\
RetVal=IPTGCCause-IPEC_SIPReasonStatusMin;\
break;\
default:\
RetVal=0xffffffff;\
}\
}

/****************************************************************
 *                      definitions                             *
 ****************************************************************/

/* Defines for Direction Codes 
 * The following ID values are used for specifying or indicating direction for 
 * the IP_CAPABILITY structure.
 */
/* IP_CAP_DIR_LCLTRANSMIT | This value indicates a transmit capability for the 
 * local end point. 
 *
 * IP_CAP_DIR_LCLRECEIVE | This value indicates a receive capability for the 
 * local end point.
 *
 * IP_CAP_DIR_LCLTXRX | This value indicates a bi-directional capability for 
 * the local end point.
 *
 * IP_CAP_DIR_RMTTRANSMIT | This value indicates a transmit capability for the 
 * remote peer. 
 *
 * IP_CAP_DIR_RMTRECEIVE | This value indicates a receive capability for the 
 * remote peer.
 *
 * IP_CAP_DIR_RMTTXRX | This value indicates a  bi-directional capability for 
 * the remote peer.
 */

#define IP_CAP_DIR_LCLTRANSMIT         0x01
#define IP_CAP_DIR_LCLRECEIVE          0x02
#define IP_CAP_DIR_LCLTXRX             0x03
#define IP_CAP_DIR_LCLSENDONLY         0x04
#define IP_CAP_DIR_LCLRECVONLY         0x05
#define IP_CAP_DIR_LCLRTPINACTIVE      0x06
#define IP_CAP_DIR_LCLRTPRTCPINACTIVE  0x07

#define IP_CAP_DIR_RMTTRANSMIT         0x10
#define IP_CAP_DIR_RMTRECEIVE          0x11
#define IP_CAP_DIR_RMTTXRX             0x12
#define IP_CAP_DIR_RMTSENDONLY         0x13
#define IP_CAP_DIR_RMTRECVONLY         0x14
#define IP_CAP_DIR_RMTRTPINACTIVE      0x15
#define IP_CAP_DIR_RMTRTPRTCPINACTIVE  0x16


#define IP_CAP_DIR_FR1677_STOP         0x2000
#define IP_CAP_DIR_FR1677_RESTART      0x2001


/* IP_USE_STANDARD_PAYLOADTYPE | Use this value in the "payload_type" field of 
 * the IP_xxx_CAPABILITY structures in case you want to use the standard 
 * payload type for the selected coder, and not specify your own payload type 
 */
#define IP_USE_STANDARD_PAYLOADTYPE    0xff



/****************************************************************
 *                      FTE Extension Ids                       *
 ****************************************************************/

/*
 * IPEXTID_SENDMSG | This extension is used for sending Q931 and 
 * nonstandard messages. The sets supported are IPSET_MSG_H245, 
 * IPSET_MSG_Q931 and IPSET_MSG_RAS.
 *
 * IPEXTID_GETINFO | This extension is used for retrieving call
 * related information 
 *
 * IPEXTID_MEDIAINFO | This extension is used in GCEV_EXTENSION events 
 * for notification of the initiation and termiation of of media streaming
 * in transmit or receive directions  In the case of media streaming
 * connection notification, the datatype of the parameter is IP_CAPABILITY
 * and consists of the coder configuration which resulted from the 
 * capability exchange with the remote peer.
 *
 * IPEXTID_SEND_DTMF | This extension is used for sending DTMF digits.
 *
 * IPEXTID_RECEIVE_DTMF | This extension is used in GCEV_EXTENSION events 
 * upon detection of DTMF digits.
 *
 * IPEXTID_IPPROTOCOL_STATE | This extension is used in GCEV_EXTENSION events 
 * for notification of intermediate protocol states, i.e. Q.931 and H.245
 * session connections and disconnections, and the initation and termination
 * of media streaming.
 *
 * IPEXTID_FOIP | This extension is used in GCEV_EXTENSION events 
 * for notification of information related to fax.
 *
 * IPEXTID_RECEIVEMSG | This extension is used when Q931, H245, RAS and 
 * nonstandard messages are received. 
 *
 * IPEXTID_CHANGEMODE | This extension is used to change call mode including
 * codec switch for both H323 and SIP 
 * nonstandard messages are received.
 */
#define IPEXTID_SENDMSG             0x01
#define IPEXTID_GETINFO             0x02
#define IPEXTID_MEDIAINFO           0x03
#define IPEXTID_SEND_DTMF           0x04
#define IPEXTID_RECEIVE_DTMF        0x05
#define IPEXTID_IPPROTOCOL_STATE    0x06
#define IPEXTID_FOIP                0x07
#define IPEXTID_RECEIVEMSG          0x08
#define IPEXTID_CHANGEMODE          0x09
#define IPEXTID_LOCAL_MEDIA_ADDRESS	0x0a

/******************************************************************
 *    Set and Parameter IDs                                       *
 ******************************************************************/
#define BASE_SETID                  GC_IP_TECH_SET_MIN /* 0x3000 */

/* Parameters to be used with GCSET_CHAN_CAPABILITY |
 *
 * IPPARM_LOCAL_CAPABILITY | This parameter id is used to pass the local capabilities
 * to be associated with a linedev, or a subset thereof (when being used to communicate
 * selected capabilities for a call. Value for this parameter id is an IP_CAPABILITY structure.
 */
#define IPPARM_LOCAL_CAPABILITY                 0x02

 
/* 
 * IPSET_LOCAL_ALIAS | Set id for setting local aliases 
 *
 * IPSET_SUPPORTED_PREFIXES | Set id for setting local prefixes for Gateway 
 * applications 
 */
#define IPSET_LOCAL_ALIAS           BASE_SETID+1 
#define IPSET_SUPPORTED_PREFIXES    BASE_SETID+2

/* parm ids under IPSET_LOCAL_ALIAS and IPSET_SUPPORTED_PREFIXES */
#define IPPARM_ADDRESS_TRANSPARENT  0x00
#define IPPARM_ADDRESS_DOT_NOTATION 0x01
#define IPPARM_ADDRESS_H323_ID      0x02
#define IPPARM_ADDRESS_PHONE        0x03
#define IPPARM_ADDRESS_URL          0x04
#define IPPARM_ADDRESS_EMAIL        0x05

/* 
 * These sets are used with the IPEXTID_SENDMSG extension. 
 * IPSET_MSG_H245 | This set id will encapsulate all the parameters required 
 * to send an H245 message.
 *
 * IPSET_MSG_Q931 | This set id will encapsulate all the parameters required 
 * to send an Q931 message.
 *
 * IPSET_MSG_REGISTRATION | This set id will encapsulate all the parameters required 
 * to send a message to the gatekeeper (H.323) or registrar (SIP).
 */
#define IPSET_MSG_H245              BASE_SETID+3
#define IPSET_MSG_Q931              BASE_SETID+4
#define IPSET_MSG_REGISTRATION      BASE_SETID+5

/* Use these parameters with IPSET_MSG_H245, IPSET_MSG_Q931, IPSET_MSG_REGISTRATION sets */
#define IPPARM_MSGTYPE              0x00


/* Message Types | The following values for IPPARM_MSGTYPE */
/* Note all messages must be null-terminated and have a maximum length of 128 characters.
 *
 * IP_MSGTYPE_H245_UNDEFINED | Used to indicate that the message type specified is 
 * not defined.
 *
 * IP_MSGTYPE_H245_REQUEST | H.245 request.
 *
 * IP_MSGTYPE_H245_RESPONSE | H.245 response
 * 
 * IP_MSGTYPE_H245_COMMAND | H.245 command.
 *
 * IP_MSGTYPE_H245_INDICATION | H.245 indicaion.
 */
#define IP_MSGTYPE_H245_UNDEFINED       0x00
#define IP_MSGTYPE_H245_REQUEST         0x01
#define IP_MSGTYPE_H245_RESPONSE        0x02
#define IP_MSGTYPE_H245_COMMAND         0x03
#define IP_MSGTYPE_H245_INDICATION      0x04

/*
 * IP_MSGTYPE_Q931_FACILITY | Q931 Facility message.
 * 
 * IP_MSGTYPE_Q931_INFORMATION | Q931 Information message.
 *
 * IP_MSGTYPE_Q931_PROGRESS | Q931 Progress message.
 */
#define IP_MSGTYPE_Q931_FACILITY        0x10
#define IP_MSGTYPE_Q931_INFORMATION     0x11
#define IP_MSGTYPE_Q931_PROGRESS        0x12

/*
 * IP_MSGTYPE_REG_NONSTD | Nonstandard registration message.
 */
#define IP_MSGTYPE_REG_NONSTD           0x20

/* Set Ids for Information Element Types
 * IPSET_VENDORINFO | Information element containing vendor information
 */
#define IPSET_VENDORINFO                BASE_SETID+6

/* parm IDs defined under IPSET_VENDORINFO */
/* IPPARM_H221NONSTD | Contains Country code, extension code and manufacturer code
 * see the IP_H221NONSTANDARD structure.
 * 
 * IPPARM_VENDOR_PRODUCT_ID | Vendor product Id.
 *
 * IPPARM_VENDOR_VERSION_ID | Vendor product version Id.
 */
#define IPPARM_H221NONSTD                           0x01
#define IPPARM_VENDOR_PRODUCT_ID                    0x02
#define IPPARM_VENDOR_VERSION_ID                    0x03

#define MAX_PRODUCT_ID_LENGTH                         32
#define MAX_VERSION_ID_LENGTH                         32



/* Set Ids for Information Element Types
 *
 * IPSET_NONSTANDARDDATA | Information element containing non-standard data
 *
 * IPSET_NONSTANDARDCONTROL | Information element containing non-standard control data
 *
 * IPSET_STIMULUSCONTROL | Information element containing stimulus control data
 */
#define IPSET_NONSTANDARDDATA           BASE_SETID+7
#define IPSET_NONSTANDARDCONTROL        BASE_SETID+8
#define IPSET_STIMULUSCONTROL           BASE_SETID+9

/* parm IDs defined under IPSET_NONSTANDARDDATA, IPSET_NONSTANDARDCONTROL and
 * IPSET_STIMULUSCONTROL
 *
 * IPPARM_NONSTANDARDDATA_DATA : String containing the non-standard data if any was 
 * supplied. If no non-standard data was provided, this parameter should not be
 * present in the parm block.
 *
 * IPPARM_NONSTANDARDDATA_OBJID : Octet string (data type unsigned int[]) 
 * containing the non-standard object ID if any was supplied.  If no 
 * non-standard object ID was provided, this parameter should not be present 
 * in the parm block.
 *
 * IPPARM_USE_NONSTANDARDDATA | This property is used to specify whether the 
 * currently set non-standard data is to be used.
 * It is important to use this property since there is no method provided to clear 
 * the non-standard data. If non-standard data is set and this use property is set 
 * to TRUE, the non-standard data will continue to be used for any and all of the 
 * functions using non standard data until the use is set to FALSE.
 *
 * IPPARM_H221NONSTANDARD | This property is used to specify H.221 for delivery of 
 * nonstandard information elements.
 */

#define IPPARM_NONSTANDARDDATA_DATA                 0x01
#define IPPARM_NONSTANDARDDATA_OBJID                0x02
#define IPPARM_USE_NONSTANDARDDATA                  0x03
#define IPPARM_H221NONSTANDARD                      0x04

#define MAX_NS_PARAM_DATA_LENGTH                     255
#define MAX_NS_PARAM_OBJID_LENGTH                     40

/* parm IDs defined under IPSET_STIMULUSCONTROL 
 *
 * IPPARM_STIMULUSCONTROL_ISTEXT : Add this parameter if the format of the message is text based
 *
 * IPPARM_STIMULUSCONTROL_H248MESSAGE : String containing an H.248 message if 
 * any was supplied. If no H.248 message was provided, this parameter should
 * not be present in the parm block.
 */

#define IPPARM_STIMULUSCONTROL_ISTEXT               0x10
#define IPPARM_STIMULUSCONTROL_H248MESSAGE          0x11



/* Set Ids for Information Element Types
 *
 * IPSET_USERINPUTINDICATION | Information element containing user input data
 */
#define IPSET_USERINPUTINDICATION           BASE_SETID+10


/* parm IDs defined under IPSET_USERINPUTINDICATION 
 *
 * IPPARM_UII_ALPHANUMERIC : User input is alphanumeric
 */
#define IPPARM_UII_ALPHANUMERIC                     0x15
#define MAX_USER_INPUT_INDICATION_LENGTH             256


/* Set Ids for Information Element Types
 *
 * IPSET_CONFERENCE | Information element containing conference information
 */
#define IPSET_CONFERENCE                    BASE_SETID+11

/* parm IDs defined under IPSET_CONFERENCE 
 *
 * IPPARM_CONFERENCE_GOAL : Conference functionality to achieve
 *
 * IPPARM_CONFERENCE_ID :  Conference identifier of size sixteen octets
 */
#define IPPARM_CONFERENCE_GOAL                      0x20
#define IPPARM_CONFERENCE_ID                        0x21

#define IP_CONFERENCE_ID_LENGTH                       16

/* Possible values for IPPARM_CONFERENCE_GOAL */
#define IP_CONFERENCEGOAL_UNDEFINED                 0x00
#define IP_CONFERENCEGOAL_CREATE                    0x01
#define IP_CONFERENCEGOAL_JOIN                      0x02
#define IP_CONFERENCEGOAL_INVITE                    0x03
#define IP_CONFERENCEGOAL_CAP_NEGOTIATION           0x04
#define IP_CONFERENCEGOAL_SUPPLEMENTARY_SRVC        0x05



/* Set Id for retrieving call information
 * IPSET_CALLINFO | Set used for setting/getting call information
 */
#define IPSET_CALLINFO                      BASE_SETID+12

/* parm IDs defined under IPSET_CALLINFO 
 *
 * IPPARM_CONNECTIONMETHOD : Defines the connection method - slow start or fast start
 *
 * IPPARM_H245TUNNELING : Enable/disable H245 tunneling
 *
 * IPPARM_DISPLAY : Null-terminated display string with max length of 82 characters.
 *
 * IPPARM_USERUSER_INFO : Set/get user to user information with max length of 131 octets
 *
 * IPPARM_PHONELIST : Defines phonelist
 *
 * IPPARM_RTCPINFO : Retrieves the RTCP information/statistics
 *
 * IPPARM_CALLDURATION : Duration of call
 *
 * IPPARM_CALLID : Call identifier of size sixteen octets
 *
 * IPPARM_PREF_DISC_REASON : Specifes the preferred disconnect reason reporting as Q.931 or H.225
 *
 * IPPARM_PRESENTATION_IND : Specifies the presentation indicator to be used in SETUP message
 *
 * IPPARM_MEDIAWAITFORCONNECT : specifies the field in SETUP message.
 *
 * IPPARM_FASTSTART_MANDATORY_H245CH : Allows application to receive GCEV_CONNECTED/GCEV_ANSWERED
 *                                     event after FastStart H.225 channel establishment. 
 *                                     Establishment of H.245 channel is not required(optional)
 *
 * IPPARM_OFFERED_FASTSTART_CODER : Holds information describing an offered FastStart coder,
 *                                  in an IP_CAPABILITY struct, that arrived on an incoming
 *                                  INVITE or SETUP message.  (Only used to deliver info to
 *                                  the application when it receives an incoming call, *not*
 *                                  used to set FastStart capabilities that app's outgoing
 *                                  calls will offer.)
 */
#define IPPARM_CONNECTIONMETHOD           0x00
#define IPPARM_H245TUNNELING              0x01
#define IPPARM_DISPLAY                    0x02
#define IPPARM_USERUSER_INFO              0x03
#define IPPARM_PHONELIST                  0x04
#define IPPARM_RTCPINFO                   0x05
#define IPPARM_CALLDURATION               0x06
#define IPPARM_CALLID                     0x07
#define IPPARM_PREF_DISC_REASON           0x08
#define IPPARM_BEARERCAP                  0x09
#define IPPARM_CHANID                     0x0A
#define IPPARM_FACILITY                   0x0B
#define IPPARM_PROGRESS_IND               0x0C
#define IPPARM_PRESENTATION_IND           0x0D
#define IPPARM_MEDIAWAITFORCONNECT        0x0E
#define IPPARM_FASTSTART_MANDATORY_H245CH 0x0F
#define IPPARM_OFFERED_FASTSTART_CODER    0x10
#define IPPARM_UUIE_ASN1				  0x11
/* FR 2558 support */
#define IPPARM_CGPN_TYPE_OF_NUMBER         0x13
#define IPPARM_CDPN_TYPE_OF_NUMBER         0x14
#define IPPARM_CGPN_NUMBERING_PLAN_ID      0x15
#define IPPARM_CDPN_NUMBERING_PLAN_ID      0x16
#define IPPARM_CGPN_SCREENING_INDICATOR    0x17
#define IPPARM_CGPN_PRESENTATION_INDICATOR 0x18

/* FR 2554 support */
#define IPPARM_SIP_TRANSPORT_ADDR          0x19
#define IP_SIP_TA_VERSION                  0x000

#define IP_PREFER_Q931_REASONS          0x01
#define IP_PREFER_H225_REASONS          0x02


#define MAX_USERUSER_INFO_LENGTH         131
#define MAX_DISPLAY_LENGTH                82

#define MAX_IP_SIP_CALLID_LENGTH         255
#define MAX_IP_H323_CALLID_LENGTH         16
#define MAX_IP_CALLID_LENGTH             MAX_IP_SIP_CALLID_LENGTH  /* For allocating sufficient buffers when protocol is unknown */

/* Backwards compatibility - Older CALLID length define (when only H.323 was supported). */
#define IP_CALLID_LENGTH                 MAX_IP_CALLID_LENGTH  /* For safety this older define was set equal to the new MAX size */


/* values for IPPARM_CONNECTIONMETHOD parameter */
#define IP_CONNECTIONMETHOD_FASTSTART   0x00
#define IP_CONNECTIONMETHOD_SLOWSTART   0x01

/* values for IPPARM_H245TUNNELING parameter */
#define IP_H245TUNNELING_OFF            0x00
#define IP_H245TUNNELING_ON             0x01

/* values for IPPARM_PRESENTATION_IND */
#define IP_PRESENTATIONALLOWED			0x00
#define IP_PRESENTATIONRESTRICTED		0x01

/* value for IPPARM_BEARERCAP and IPPARM_FACILITY parameters */
#define MAX_IE_LENGTH                    255

/* values for IPPARM_FASTSTART_MANDATORY_H245CH parameter */
#define IP_FASTSTART_MANDATORY_H245CH_OFF   0x00
#define IP_FASTSTART_MANDATORY_H245CH_ON    0x01

/* Set Id for retrieving media information
 * IPSET_MEDIA_STREAM | Set used for retrieving media information such as RTP/RTCP
 * addresses
 */
#define IPSET_MEDIA_STREAM              BASE_SETID+13

/* parm IDs defined under IPSET_MEDIA_STREAM 
 * IPPARM_LOCAL_MEDIA_STREAM |  This parameter id is used to send/receive local RTP/RTCP stream information.
 * The value buffer would contain an IP_MEDIA_STREAM_INFO structure.
 */
#define IPPARM_LOCAL_MEDIA_STREAM               0x01



/* Set Id for DTMF related parameters for suppressing or sending of DTMF digits.
 */
#define IPSET_DTMF                      BASE_SETID+14

/* parm IDs defined under IPSET_DTMF
 * IPPARM_SUPPORT_DTMF_BITMASK |  This parameter id is used to specify the unsigned char 
 * bitmask which specifies which DTMF transmission methods are to be supported.
 */
#define IPPARM_SUPPORT_DTMF_BITMASK             0x01

/* Bits to enable support of DTMF method for IPPARM_SUPPORT_DTMF_BITMASK unsigned char bitmask:
 */
#define IP_DTMF_TYPE_USERINPUT_SIGNAL           0x01
#define IP_DTMF_TYPE_RFC_2833                   0x02
#define IP_DTMF_TYPE_INBAND_RTP                 0x04
#define IP_DTMF_TYPE_ALPHANUMERIC               0x08

/* parm IDs defined under IPSET_DTMF.  The data field for these parameters is char[] which 
 * contains the digits to transmit to the remote endpoint.  
 *
 * IPPARM_DTMF_RFC_2833 |  Used to initiate generating DTMF via RFC 2833 message. 
 *
 * IPPARM_DTMF_ALPHANUMERIC | Used to initiate generating DTMF via H.245 User Input Indication
 * Alphanumeric message.
 *
 * IPPARM_DTMF_SIGNAL | Used to initiate generating DTMF via H.245 User Input Indication
 * Signal.
 *
 * IPPARM_DTMF_SIGNAL_UPDATE | Used to initiate generating DTMF via H.245 User Input Indication
 * Signal Update.
 *
 * IPPARM_DTMF_RFC2833_PAYLOAD_TYPE | Used to specify RFC2833 RTP payload type.  Data field is 
 * unsigned char with a valid range of 96-127.
 */
#define IPPARM_DTMF_RFC_2833                    0x02
#define IPPARM_DTMF_ALPHANUMERIC                IPPARM_UII_ALPHANUMERIC /* 0x15 */
#define IPPARM_DTMF_SIGNAL                      0x03
#define IPPARM_DTMF_SIGNAL_UPDATE               0x04
#define IPPARM_DTMF_RFC2833_PAYLOAD_TYPE        0x05

/* Set Id for notification of intermediate protocol states, i.e. Q.931 and H.245
 * connections and disconnections.  Notification is via GCEV_EXTENSION event of extension 
 * ID IPEXTID_IPPROTOCOL_STATE.
 */
#define IPSET_IPPROTOCOL_STATE          BASE_SETID+15

/* parm IDs defined under IPSET_IPPROTOCOL_STATE.  The data value field for all 
 * parameters in this set will be unused.
 *
 * IPPARM_SIGNALING_CONNECTED |  Q.931 or SIP signaling connection established.  
 * In case of Q.931, CONNECT or CONNECTACK received at caller or callee, respectively.
 *
 * IPPARM_SIGNALING_DISCONNECTED |  Q.931 or SIP signaling session disconnected.  
 * In case of Q.931, RELEASE or RELEASECOMEPLETE received at terminator or its peer, 
 * respectively.
 *
 * IPPARM_CONTROL_CONNECTED |  H.245 connection established. Logical Channels operations 
 * can be performed (i.e. Request Mode).
 *
 * IPPARM_CONTROL_DISCONNECTED |  H.245 session disconnected, i.e. EndSession received or sent.
 *
 * IPPARM_EST_CONTROL_FAILED |  H.245 connection establishment failed. H.245 channel is not 
 * available for duration of call.
 *
*/
#define IPPARM_SIGNALING_CONNECTED              0x01
#define IPPARM_SIGNALING_DISCONNECTED           0x02
#define IPPARM_CONTROL_CONNECTED                0x03
#define IPPARM_CONTROL_DISCONNECTED             0x04
#define IPPARM_EST_CONTROL_FAILED               0x05


/* Set Id for notification of the status and configuration information of transmit or receive
 * directions of media streaming. Notification is via GCEV_EXTENSION event of extension ID
 * IPEXTID_MEDIAINFO.
 */
#define IPSET_MEDIA_STATE               BASE_SETID+16

/* parm IDs defined under IPSET_MEDIA_STATE.  For XXX_CONNECTED parm IDs, the datatype of the  
 * parameter is IP_CAPABILITY and consists of the coder configuration which resulted
 * from the capability exchange with the remote peer. For XXX_DISCONNECTED parm IDs, the data 
 * value field is unused.
 *
 * IPPARM_TX_CONNECTED |  Streaming initiated in transmit direction.  The datatype of the  
 * parameter is IP_CAPABILITY and consists of the coder configuration which resulted
 * from the capability exchange with the remote peer.
 *
 * IPPARM_TX_DISCONNECTED |  Streaming terminated in transmit direction. The data 
 * value field is unused.
 *
 * IPPARM_RX_CONNECTED |  Streaming initiated in receive direction.  The datatype of the  
 * parameter is IP_CAPABILITY and consists of the coder configuration which resulted
 * from the capability exchange with the remote peer.
 *
 * IPPARM_RX_DISCONNECTED |  Streaming termintated in receive direction. The data 
 * value field is unused.
 */
#define IPPARM_TX_CONNECTED                     0x01
#define IPPARM_TX_DISCONNECTED                  0x02
#define IPPARM_RX_CONNECTED                     0x03
#define IPPARM_RX_DISCONNECTED                  0x04
#define IPPARM_TX_SENDONLY                      0x05
#define IPPARM_TX_INACTIVE                      0x06
#define IPPARM_RX_RECVONLY                      0x07
#define IPPARM_RX_INACTIVE                      0x08


/* Set Id for notification of fax tones detected on TDM. Notification is via GCEV_EXTENSION
 * event of extension ID IPEXTID_FOIP.
 */
#define IPSET_TDM_TONEDET               BASE_SETID+17

/* parm IDs defined under IPSET_TDM_TONEDET identifying type of tone detected.
 *
 * IPPARM_TDMDET_CED |  CED tone detected on TDM. The data value field is unused.
 *
 * IPPARM_TDMDET_CNG |  CNG tone detected on TDM. The data value field is unused.
 *
 * IPPARM_TDMDET_V21 |  V.21 tone detected on TDM. The data value field is unused.
 *
 * IPPARM_TDMDET_CNG_ENABLE |  enable CNG tone detection on TDM for fax switch.
 *
 * IPPARM_TDMDET_CNG_DISABLE |  disable CNG tone detection on TDM for fax switch.
 */
#define IPPARM_TDMDET_CED                       0x01
#define IPPARM_TDMDET_CNG                       0x02
#define IPPARM_TDMDET_V21                       0x03
#define IPPARM_TDMDET_CNG_ENABLE                0x04
#define IPPARM_TDMDET_CNG_DISABLE               0x05


/* Set Id for notification of fax tones detected on T.38. Notification is via GCEV_EXTENSION
 * event of extension ID IPEXTID_FOIP.
 */
#define IPSET_T38_TONEDET               BASE_SETID+18

/* parm IDs defined under IPSET_T38_TONEDET identifying type of tone detected.
 *
 * IPPARM_T38DET_CED |  CED tone detected on T38. The data value field is unused.
 *
 * IPPARM_T38DET_CNG |  CNG tone detected on T38. The data value field is unused.
 *
 * IPPARM_T38DET_V21 |  V.21 tone detected on T38. The data value field is unused.
 */
#define IPPARM_T38DET_CED                       0x01
#define IPPARM_T38DET_CNG                       0x02
#define IPPARM_T38DET_V21                       0x03


/* Set Id for notification of the T.38 capability frame of type as identified via the
 * parameter ID.  Notification is via GCEV_EXTENSION event of extension ID IPEXTID_FOIP.
 */
#define IPSET_T38CAPFRAMESTATUS         BASE_SETID+19

/* parm IDs defined under IPSET_T38CAPFRAMESTATUS identifying T.38 capability frame status.
 *
 * IPPARM_T38CAPFRAME_TX_DIS_DTC |  The parameter value includes the cap frame status struct (TBD).
 *
 * IPPARM_T38CAPFRAME_TX_DCS |  The parameter value includes the cap frame status struct (TBD).
 *
 * IPPARM_T38CAPFRAME_TX_CTC |  The parameter value includes the cap frame status struct (TBD).
 *
 * IPPARM_T38CAPFRAME_RX_DIX_DTC |  The parameter value includes the cap frame status struct (TBD).
 *
 * IPPARM_T38CAPFRAME_RX_DCS |  The parameter value includes the cap frame status struct (TBD).
 *
 * IPPARM_T38CAPFRAME_RX_CTC |  The parameter value includes the cap frame status struct (TBD).
 *
 */
#define IPPARM_T38CAPFRAME_TX_DIS_DTC           0x01
#define IPPARM_T38CAPFRAME_TX_DCS               0x02
#define IPPARM_T38CAPFRAME_TX_CTC               0x03
#define IPPARM_T38CAPFRAME_RX_DIX_DTC           0x04
#define IPPARM_T38CAPFRAME_RX_DCS               0x05
#define IPPARM_T38CAPFRAME_RX_CTC               0x06


/* Set Id for notification of the T.38 Info frame of type as identified via the
 * parameter ID.  Notification is via GCEV_EXTENSION event of extension ID IPEXTID_FOIP.
 */
#define IPSET_T38INFOFRAMESTATUS        BASE_SETID+20

/* parm IDs defined under IPSET_T38INFOFRAMESTATUS identifying T.38 capability frame status.
 *
 * IPPARM_T38INFOFRAME_TX_SUB |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_RX_SUB |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_TX_SEP |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_RX_SEP |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_TX_PWD |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_RX_PWD |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_TX_TSI |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_RX_TSI |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_TX_CSI |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_RX_CSI |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_TX_CIG |  The parameter value includes the Info frame buffer.
 *
 * IPPARM_T38INFOFRAME_RX_CIG |  The parameter value includes the Info frame buffer.
 */
#define IPPARM_T38INFOFRAME_TX_SUB              0x01
#define IPPARM_T38INFOFRAME_RX_SUB              0x02
#define IPPARM_T38INFOFRAME_TX_SEP              0x03
#define IPPARM_T38INFOFRAME_RX_SEP              0x04
#define IPPARM_T38INFOFRAME_TX_PWD              0x05
#define IPPARM_T38INFOFRAME_RX_PWD              0x06
#define IPPARM_T38INFOFRAME_TX_TSI              0x07
#define IPPARM_T38INFOFRAME_RX_TSI              0x08
#define IPPARM_T38INFOFRAME_TX_CSI              0x09
#define IPPARM_T38INFOFRAME_RX_CSI              0x0a
#define IPPARM_T38INFOFRAME_TX_CIG              0x0b
#define IPPARM_T38INFOFRAME_RX_CIG              0x0c


/* Set Id for notification of the T.38 HDLC frame of type as identified via the
 * parameter ID.  Notification is via GCEV_EXTENSION event of extension ID IPEXTID_FOIP.
 */
#define IPSET_T38HDLCFRAMESTATUS        BASE_SETID+21

/* parm IDs defined under IPSET_T38HDLCFRAMESTATUS identifying T.38 HDLC frame status.
 *
 * IPPARM_T38HDLCFRAME_TX |  The parameter value includes the HDLC frame buffer.
 *
 * IPPARM_T38HDLCFRAME_RX |  The parameter value includes the HDLC frame buffer.
 */
#define IPPARM_T38HDLCFRAME_TX                  0x01
#define IPPARM_T38HDLCFRAME_RX                  0x02


/* Parameter value for specifying registration service in parameter 
 * GCSET_SERVREQ:PARM_REQTYPE in functions gc_ReqService or gc_RespService 
 */
#define IP_REQTYPE_REGISTRATION                 0x01

/* Set Id for the registration operation with a gatekeep (H.323) or registrar (SIP).
 */
#define IPSET_REG_INFO                      BASE_SETID+22

/* parm IDs defined under IPSET_REG_INFO identifying the registration operation
 *
 * IPPARM_OPERATION_REGISTER |  Register with the gatekeeper/registrar using the method
 *                              identified by the value field.
 *
 * IPPARM_OPERATION_DEREGISTER |  Deregister with the gatekeeper/registrar using the method
 *                                identified by the value field.
 */
#define IPPARM_OPERATION_REGISTER               0x01
#define IPPARM_OPERATION_DEREGISTER             0x02

#define IP_REG_SET_INFO                         0x01  /* Register: Overwrite existing information */
#define IP_REG_ADD_INFO                         0x02  /* Register: Append existing information */
#define IP_REG_DELETE_BY_VALUE                  0x03  /* Register: Delete by value */
#define IP_REG_MAINTAIN_LOCAL_INFO              0x04  /* Deregister: Maintain local information */
#define IP_REG_DELETE_ALL                       0x05  /* Deregister: Erase local information */
#define IP_REG_QUERY_INFO                       0x06  /* Register: Query current information */



/* 
 * IPPARM_REG_ADDRESS |  Registration address structure IP_REGISTER_ADDRESS (declared in.gcip.h)
 */
#define IPPARM_REG_ADDRESS                      0x03

/* 
 * IPPARM_REG_STATUS |  Status of registration operation, being confirmed or rejected.
 *                      Notification is via GCEV_SERVICERESP event.
 */
#define IPPARM_REG_STATUS                       0x04

#define IP_REG_REJECTED                         0x00
#define IP_REG_CONFIRMED                        0x01

/* 
 * IPPARM_REG_TYPE   |  Register as Gateway or Terminal.
 */
#define IPPARM_REG_TYPE                         0x05

#define IP_REG_GATEWAY                          0x00
#define IP_REG_TERMINAL                         0x01



/* This parameter ID, used in conjunction with the IPSET_REG_INFO set ID, allows applications to be    
   notified of the Service ID that a particular GCEV_SERVICERESP or GCEV_TASKFAIL event is 
   associated with. */
#define IPPARM_REG_SERVICEID                    0x06



/* This parameter ID, used in conjunction with the IPSET_REG_INFO set ID, allows applications to control    
   whether a SIP REGISTER binding gets auto refreshed or not. */
#define IPPARM_REG_AUTOREFRESH                  0x07

/* This value, used for the IPPARM_REG_AUTOREFRESH parameter ID, allows applications to mandate    
   that a SIP REGISTER binding gets auto refreshed.  NOTE: Auto refresh is the default action. */
#define IP_REG_AUTOREFRESH_ENABLE              0x00

/* This value, used for the IPPARM_REG_AUTOREFRESH parameter ID, allows applications to mandate    
   that a SIP REGISTER binding not be auto refreshed. */
#define IP_REG_AUTOREFRESH_DISABLE             0x01





/* Set Id to for enabling or disabling of GCEV_EXTENSION unsolicited notification events. 
 * To be used with parm ids GCPARM_GET_MSK, GCACT_SETMSK, GCACT_ADDMSK, and GCACT_SUBMSK and
 * associated bitmask value of size GC_VALUE_LONG (4 octets)
 */
#define IPSET_EXTENSIONEVT_MSK              BASE_SETID+23

/* 
 * bitmask settings for parm IDs GCPARM_GET_MSK, GCACT_SETMSK, GCACT_ADDMSK, and GCACT_SUBMSK 
 */
#define EXTENSIONEVT_DTMF_USERINPUT_SIGNAL      0x01
#define EXTENSIONEVT_DTMF_RFC2833               0x02
#define EXTENSIONEVT_DTMF_ALPHANUMERIC          0x04
#define EXTENSIONEVT_SIGNALING_STATUS           0x08
#define EXTENSIONEVT_STREAMING_STATUS           0x10
#define EXTENSIONEVT_T38_STATUS                 0x20
#define EXTENSIONEVT_CALL_PROGRESS				0x40


/* Set Id to identify protocol in scenarios which support multiple protocols.  
   The scope of this set and parameter ID pair apply to all subsequent parameters,
   i.e., GC_PARM_DATA structures, in the GC_PARM_BLK.
 */
#define IPSET_PROTOCOL                      BASE_SETID+24

/*
 * IPPARM_PROTOCOL_BITMASK |  Bitmask specifying intended protocol for registration information.
 */
#define IPPARM_PROTOCOL_BITMASK                 0x01

#define IP_PROTOCOL_SIP                         0x01
#define IP_PROTOCOL_H323                        0x02


/* Set Id for setting and getting parameter values
 * IPSET_CONFIG | This set Id is used for configuring general parameters.
 */
#define IPSET_CONFIG                         BASE_SETID+25

/* parm IDs defined under IPSET_CONFIG for configuration
 * IPPARM_CONFIG_TOS |  Type of Service Parameter. Values range from 0-255
 */
#define IPPARM_CONFIG_TOS                       0x01

/* IPPARM_OPERATING_MODE | Operating mode for device can be automatic mode(default)
 * or manual mode. manual mode is required for T38 fax server
 */
#define IPPARM_OPERATING_MODE                   0x02
#define IP_T38_AUTOMATIC_MODE       0x01
#define IP_T38_MANUAL_MODE          0x02                    /* switch thru GCEV_EXTENSION event */
#define IP_AUTOMATIC_MODE           IP_T38_AUTOMATIC_MODE   /* old defines for compatibility */
#define IP_MANUAL_MODE              IP_T38_MANUAL_MODE      /* old defines for compatibility */
#define IP_T38_MANUAL_MODIFY_MODE   0x04                    /* switch thru GCEV_REQ_MODIFY_CALL event */

/* IPPARM_CALL_XFER_HOLD | enable SIP tranfer with hold funtion. This is for
 * Intervoice feature request of putting call on hold by set 0.0.0.0 IP address
 * before sending REFER
 */
#define IPPARM_CALL_XFER_HOLD                   0x03
/* IPPARM_REGISTER_SIP_HEADER | SIP header to register for monitoring
 * Value set is a char* containing the name of the SIP header.
 */
#define IPPARM_REGISTER_SIP_HEADER              0x04

/* IPPARM_AUTHENTICATION_CONFIGURE | This parameter is used to add or 
 * modify authentication configuration.
 * IPPARM_AUTHENTICATION_REMOVE | This parameter is used to remove 
 * authentication configuration.
 */
#define IPPARM_AUTHENTICATION_CONFIGURE         0x05
#define IPPARM_AUTHENTICATION_REMOVE            0x06

#define IPPARM_IPMPARM							0x07

/* IPPARM_1PCC_REJECT_VIDEO | enable GC reject incoming offer with video 
 * media descriptor
 */
#define IPPARM_1PCC_REJECT_VIDEO                0x08

/* IPPARM_EXCLUDE_G723_BITVAD | exclude G723 bitrate and vad fmtp attribute
 * in SDP
 */
#define IPPARM_EXCLUDE_G723_BITVAD              0x09

/* IPPARM_1PCC_OPTIONS_RESPONSE_NO_SDP | enable responding OPTIONS without SDP 
 * in 1PCC mode
 */
#define IPPARM_1PCC_OPTIONS_RESPONSE_NO_SDP     0x0a

/* IPPARM_H323_AUTO_PROGRESS_DISABLE | do not automatically send PROGRESS
 * message after ALERTING message 
 */
#define IPPARM_H323_AUTO_PROGRESS_DISABLE       0x0b   

/* IPPARM_SIP_FAXTOAUDIO_AUTO_REINVITE_DISABLE | do not automatically send a reinvite
 * message after a switch from Fax to Audio media occurs, typically when a fax session
 * ends. 
 */
#define IPPARM_SIP_FAXTOAUDIO_AUTO_REINVITE_DISABLE 0x0c    
#define IPPARM_SIP_FAXTOAUDIO_AUTO_REINVITE_ENABLE  0x0d    

/* IPPARM_FORCE_REMOTE_SIP_TRANSPORT | If remote contact transport is other than TCP or
 * UDP, application can force the unsupported transport to be either UDP or TCP. 
 * There is no guarantee this may work because remote UA may enforce the transport rule.
 */
#define IPPARM_SIP_FORCE_REMOTE_CONTACT_TRANSPORT       0x0e  
#define IP_SIP_FORCE_REMOTE_CONTACT_TRANSPORT_UDP       0x01
#define IP_SIP_FORCE_REMOTE_CONTACT_TRANSPORT_TCP       0x02   

/* IPPARM_SIP_AUDIOTOFAX_AUTO_REINVITE_DISABLE | do not automatically send a reinvite
 * message after a switch from Audio to FAX media occurs, typically when a audio session
 * ends. 
 */
#define IPPARM_SIP_AUDIOTOFAX_AUTO_REINVITE_DISABLE 0x0f    
#define IPPARM_SIP_AUDIOTOFAX_AUTO_REINVITE_ENABLE  0x10               

/* IPPARM_SIP_AUDIOFAX_SWITCH_FAILURE_ROLLBACK |  enable or disable parameter to rollback to previous media	
 * on Re-INVITE failure rather than disconnecting the call.
 */
#define IPPARM_SIP_AUDIOFAX_SWITCH_FAILURE_ROLLBACK 0x13
/* Set Id for setting and getting parameter values
 * IPSET_TRANSACTION | This set Id is used for configuring general parameters.
 */
#define IPSET_TRANSACTION                    BASE_SETID+26

/* parm IDs defined under IPSET_TRANSACTION for configuration
 * IPPARM_TRANSACTION_ID |  Transaction Id. This value is of type (void *) and 
 * is filled in by the application. 
 */
#define IPPARM_TRANSACTION_ID                   0x01


/* Set ID for SIP message information
 * IPSET_SIP_MSGINFO | This Set ID is used configuration and retrieval
 * of SIP related message information (e.g. Request URI).
 */
#define IPSET_SIP_MSGINFO                    BASE_SETID+27

/* Different Parm IDs that can be used with SetID IPSET_SIP_MSGINFO */

#define IPPARM_REQUEST_URI                      0x01
#define IPPARM_CONTACT_URI                      0x02
#define IPPARM_FROM_DISPLAY                     0x03
#define IPPARM_TO_DISPLAY                       0x04
#define IPPARM_CONTACT_DISPLAY                  0x05
#define IPPARM_REFERRED_BY                      0x06
#define IPPARM_REPLACES                         0x07
#define IPPARM_CONTENT_DISPOSITION              0x08
#define IPPARM_CONTENT_ENCODING                 0x09
#define IPPARM_CONTENT_LENGTH                   0x0a	
#define IPPARM_CONTENT_TYPE                     0x0b	
#define IPPARM_REFER_TO                         0x0c	
#define IPPARM_DIVERSION_URI                    0x0d	
#define IPPARM_EVENT_HDR                        0x0e	
#define IPPARM_EXPIRES_HDR                      0x0f	
#define IPPARM_CALLID_HDR                       0x10	
#define IPPARM_SIP_HDR                          0x11	
#define IPPARM_FROM                             0x12
#define IPPARM_TO                               0x13
#define IPPARM_SIP_HDR_REMOVE                   0x14



/* Defines for exposing additional message information support in 'sip_msginfo_mask' bitmask within IP_VIRTBOARD
 * (Defaults to all disabled...) */
/* Mask to activate SIP standard headers */
#define IP_SIP_MSGINFO_ENABLE                   0x01
/* Mask to activate SIP-T/MIME feature */
#define IP_SIP_MIME_ENABLE                      0x02
/* Mask to activate the attching of FastStart coder
 * information to received Offered events on SIP calls. 
 * (Does *not* effect setting of outgoing SIP FastStart coder info.) */
#define IP_SIP_FASTSTART_CODERS_IN_OFFERED   0x04

#define IP_SIP_TRANSADDR_ENABLE 0x08   // Mask to enable receipt of SIP remote
                                       // transport address for incoming INVITE/REINVITE 

/* Maximum Allowable Length of individual SIP Message Info fields */
#define IP_REQUEST_URI_MAXLEN                 255
#define IP_CONTACT_URI_MAXLEN                 255
#define IP_FROM_DISPLAY_MAXLEN                255
#define IP_TO_DISPLAY_MAXLEN                  255
#define IP_CONTACT_DISPLAY_MAXLEN             255
#define IP_REFERRED_BY_MAXLEN                 255
#define IP_REPLACES_MAXLEN                    255
#define IP_CONTENT_DISPOSITION_MAXLEN         255
#define IP_CONTENT_ENCODING_MAXLEN            255
#define IP_CONTENT_LENGTH_MAXLEN              255
#define IP_CONTENT_TYPE_MAXLEN                255
#define IP_REFER_TO_MAXLEN                    255
#define IP_DIVERSION_URI_MAXLEN               255
#define IP_EVENT_HDR_MAXLEN                   255
#define IP_EXPIRES_HDR_MAXLEN                 255
#define IP_CALLID_HDR_MAXLEN                  255
#define IP_FROM_MAXLEN                        255
#define IP_TO_MAXLEN                          255
#define IP_SIP_HDR_MAXLEN       	          255


/* Set ID for fax over IP
 * IPSET_FOIP | This Set ID is used to set or get T38 related parameters
 */      
#define IPSET_FOIP                              BASE_SETID+28  

/* parm IDs defined under IPSET_FOIP for configuration
 * IPPARM_T38_CONNECT | connect media handle with fax handle
 * IPPARM_T38_DISCONNECT | disconnect media handle from fax handle
 * IPPARM_T38_OFFERED | T38 only call is offered
 */  
#define IPPARM_T38_CONNECT                      0x1
#define IPPARM_T38_DISCONNECT                   0x2
#define IPPARM_T38_OFFERED                      0x3
    

/* Set ID for codec switch during a call
 * IPSET_SWITCH_CODEC | This Set ID is used to configure codec switch
 */
#define IPSET_SWITCH_CODEC                      BASE_SETID+29

/* parm IDs defined under IPSET_SWITCH_CODEC for configuration
 * IPPARM_T38_INITIATE | initiate T38 switch from audio
 * IPPARM_AUDIO_INITIATE | initiate audio switch from t38
 * IPPARM_T38_REQUESTED | incoming t38 switch request from audio
 * IPPARM_AUDIO_REQUESTED  | incoming audio switch request from t38
 * IPPARM_READY         | media is ready
 * IPPARM_ACCEPT        | accept incoming t38/audio switch request
 * IPPARM_REJECT        | reject incoming t38/audio switch request
 */
#define IPPARM_T38_INITIATE                     0x1
#define IPPARM_AUDIO_INITIATE                   0x2
#define IPPARM_T38_REQUESTED                    0x3
#define IPPARM_AUDIO_REQUESTED                  0x4
#define IPPARM_READY                            0x5
#define IPPARM_ACCEPT                           0x6
#define IPPARM_REJECT                           0x7

/* Set ID for MIME information in SIP message
 * IPSET_MIME | This Set ID is used to set or get MIME information
 * IPSET_MIME_200OK_TO_BYE | This Set ID is used to set MIME in 200OK to BYE
 */
#define IPSET_MIME                      BASE_SETID+30
#define IPSET_MIME_200OK_TO_BYE         BASE_SETID+31

/* parm IDs defined under IPSET_MIME for configuration
 * IPPARM_MIME_PART                 | MIME part pointer to GC_PARM_BLK
 * IPPARM_MIME_PART_BODY_SIZE       | MIME part body size
 * IPPARM_MIME_PART_TYPE            | MIME part type string
 * IPPARM_MIME_PART_HEADER          | MIME part header string
 * IPPARM_MIME_PART_BODY            | MIME part pointer to memory buffer
 */
#define IPPARM_MIME_PART                        0x1
#define IPPARM_MIME_PART_BODY_SIZE              0x2
#define IPPARM_MIME_PART_TYPE                   0x3
#define IPPARM_MIME_PART_HEADER                 0x4
#define IPPARM_MIME_PART_BODY                   0x5

/* Set ID for SIP response code
 * IPSET_SIP_RESPONSE_CODE | SIP response code
 */
#define IPSET_SIP_RESPONSE_CODE                 BASE_SETID+32

/* parm IDs defined under IPSET_SIP_RESPONSE_CODE for configuration
 * IPPARM_BUSY_RESP_CODE | busy response code for rejecting incoming call
 */
#define IPPARM_BUSY_RESP_CODE                   0x1
#define IPPARM_BUSY_REASON                      IPPARM_BUSY_RESP_CODE

/* parm IDs defined under IPSET_SIP_RESPONSE_CODE for configuration
 * IPPARM_ACCEPT_RESP_CODE | response code used for gc_AcceptCall
 */
#define IPPARM_ACCEPT_RESP_CODE                 0x2

/* Used ONLY to deliver received SIP response Status-Codes to 
 * the application.  Can NOT be used to set outgoing response 
 * Status-Codes (above defines can be used for that purpose).
 */
#define IPPARM_RECEIVED_RESPONSE_STATUS_CODE    0x40




/* Set ID for H323 response code
 * IPSET_H323_RESPONSE_CODE | H323 response code
 */
#define IPSET_H323_RESPONSE_CODE                BASE_SETID+33

/* parm IDs defined under IPSET_H323_RESPONSE_CODE 
 * for configuration IPPARM_BUSY_CAUSE | busy cause for rejecting incoming call
 */
#define IPPARM_BUSY_CAUSE                       0x1

/*for reporting RTP addresses of endpoints on connect */
#define IPSET_RTP_ADDRESS                       BASE_SETID+34
#define IPPARM_LOCAL                            0
#define IPPARM_REMOTE                           1

/* Set ID for SIP message types handed by GCEV_EXTENSION
 * IPSET_MSG_SIP | This Set ID is used to set or get the SIP message type.
 */
#define IPSET_MSG_SIP                           BASE_SETID+35
#define IPPARM_MSG_SIP_RESPONSE_CODE			1

/*
 * IP_MSGTYPE_SIP_SUBSCRIBE           | SIP Subscribe message.
 * IP_MSGTYPE_SIP_SUBSCRIBE_ACCEPT    | Response to SIP Subscribe message with a 200 OK.
 * IP_MSGTYPE_SIP_SUBSCRIBE_REJECT    | Response to SIP Subscribe message with a failure.
 * IP_MSGTYPE_SIP_SUBSCRIBE_EXPIRE    | Expiration of Subscribe
 * IP_MSGTYPE_SIP_NOTIFY              | SIP Notify message.
 * IP_MSGTYPE_SIP_NOTIFY_ACCEPT       | Response to SIP Notify message with a 200 OK.
 * IP_MSGTYPE_SIP_NOTIFY_REJECT       | Response to SIP Notify message with a failure.
 * IP_MSGTYPE_SIP_INFO                | SIP Info message
 * IP_MSGTYPE_SIP_INFO_ACCEPT         | Response to SIP Info message with a 200 OK.
 * IP_MSGTYPE_SIP_INFO_REJECT         | Response to SIP Info message with a failure.
 */
#define IP_MSGTYPE_SIP_SUBSCRIBE                 0x1
#define IP_MSGTYPE_SIP_SUBSCRIBE_ACCEPT          0x2
#define IP_MSGTYPE_SIP_SUBSCRIBE_REJECT          0x3
#define IP_MSGTYPE_SIP_SUBSCRIBE_EXPIRE          0x4
#define IP_MSGTYPE_SIP_NOTIFY                    0x5
#define IP_MSGTYPE_SIP_NOTIFY_ACCEPT             0x6
#define IP_MSGTYPE_SIP_NOTIFY_REJECT             0x7
#define IP_MSGTYPE_SIP_INFO                      0x8
#define IP_MSGTYPE_SIP_INFO_OK                   0x9
#define IP_MSGTYPE_SIP_INFO_FAILED               0xa
//Options support
#define IP_MSGTYPE_SIP_OPTIONS                   0xb
#define IP_MSGTYPE_SIP_OPTIONS_OK                0xc
#define IP_MSGTYPE_SIP_OPTIONS_FAILED            0xd

// Re-INVITE support
#define IPPARM_SIP_METHOD                        IPPARM_MSG_SIP_RESPONSE_CODE
#define IP_MSGTYPE_SIP_CANCEL                    0xe

/* Macros to set the terminal_type in the IP_VIRTBOARD data structure */
#define IP_TT_TERMINAL  50
#define IP_TT_GATEWAY   60



/* IP specific operations for the gc_GetCallInfo() function ('info_id' parameter values) */
#define DESTINATION_ADDRESS_SIP        0x201
#define ORIGINATION_ADDRESS_SIP        0x202
#define IP_CALLID                      0x203  /* retrieve the unique, protocol specific, identifier of the current call */


/* Maxumim buffer lengths required for IP specific gc_GetCallInfo() operations (i.e. CALLID) */
#define IP_SIP_CALLIDSIZE     256    /* When retrieving CALLID info on a SIP call */
#define IP_H323_CALLIDSIZE     16    /* When retrieving CALLID info on a H.323 call */
#define IP_CALLIDSIZE         IP_SIP_CALLIDSIZE  /* For allocating sufficient buffers when protocol is unknown */
/*Multiple IP Addresses */
#define IPSET_IP_ADDRESS                           BASE_SETID+36
#define IPPARM_SET_ADDRESS                         0x1

/*tcp retry*/
#define IP_SIP_METHODSIZE     20  /*for retrieving the method on a request error */
#define IPSET_SIP_REQUEST_ERROR                    BASE_SETID+37
#define IPPARM_SIP_DNS_CONTINUE                    0x1 /*generic request*/
#define IPPARM_SIP_SVC_UNAVAIL                     0x2 /*generic request*/

/*values for the above parms*/
#define IP_SIP_REQUEST_NETWORK_ERROR               0x0
#define IP_SIP_REQUEST_TIMEOUT                     0x1
#define IP_SIP_REQUEST_503_RCVD                    0x2
#define IP_SIP_REQUEST_RETRY_FAILED                0x3
#define IP_SIP_REQUEST_FAILED                      0x4 /*catchall*/
typedef enum
{
   ENUM_REQUEST_RETRY_UNDEFINED=0,
   ENUM_REQUEST_RETRY_NONE, 
   ENUM_REQUEST_RETRY_DNS,         
   ENUM_REQUEST_RETRY_FORCEDTCP,
   ENUM_REQUEST_RETRY_ALL
} EnumSIP_RequestRetry;


/* Following defines added for IP CCLIB Tunneled Signaling Message support */
#define IPSET_TUNNELEDSIGNALMSG                    BASE_SETID+38

#define IPPARM_TUNNELEDSIGNALMSG_CONTENT           0x01
#define IPPARM_TUNNELEDSIGNALMSG_PROTOCOL_OBJID    0x02
#define IPPARM_TUNNELEDSIGNALMSG_ALTERNATEID       0x03
#define IPPARM_TUNNELEDSIGNALMSG_NSDATA_DATA       0x04
#define IPPARM_TUNNELEDSIGNALMSG_NSDATA_OBJID      0x05
#define IPPARM_TUNNELEDSIGNALMSG_NSDATA_H221NS     0x06
#define IPPARM_TUNNELEDSIGNALMSG_PROTOCOL_OBJECTID 0x07
#define IPPARM_TSM_CONTENT_EVENT		           0x08

/* Following defines are used to indicate the type of event the TSM content is associated with */
#define TSM_CONTENT_OFFERED						   0x01
#define TSM_CONTENT_PROCEEDING			           0x02
#define TSM_CONTENT_ALERTING                       0x03
#define TSM_CONTENT_CONNECTED                      0x04
#define TSM_CONTENT_DISCONNECTED	               0x05
#define TSM_CONTENT_EXTENSION                      0x06

#define MAX_TSM_POID_PARAM_LENGTH                  128
#define MAX_TSM_ALTID_VARS_LENGTH                  64
#define MAX_TSM_OBJID_VARS_LENGTH                  64

#define IP_TUNNELPROTOCOL_ALTID_VERSION            0x000
#define IP_TUNNELPROTOCOL_OBJECTID_VERSION         0x000


/* 3PCC SDP Support */
#define IPSET_SDP                                  BASE_SETID+39
#define IPPARM_SDP_OFFER                           0x1
#define IPPARM_SDP_ANSWER                          0x2
#define IPPARM_SDP_OPTION_OFFER                    0x3
#define IPPARM_SDP_OPTION_ANSWER                   0x4

/* Set ID for SIP Session Timer
 * IPSET_SIP_SESSION_TIMER | This Set ID is used to configure sip session timer
 */
#define IPSET_SIP_SESSION_TIMER                    BASE_SETID+40

/* parm IDs defined under IPSET_SIP_SESSION_TIMER for configuration
 * IPPARM_SESSION_EXPIRES | Session-Expires timer value in seconds
 * IPPARM_MIN_SE | Min-SE timer value in seconds
 * IPPARM_REFRESHER_PREFERENCE | refresher preference
 * IPPARM_REFRESH_METHOD  | SIP method for refresh
 * IPPARM_REFRESH_WITHOUT_REMOTE_SUPPORT  | execute session timer without remote support 'timer'
 * IPPARM_REFRESH_WITHOUT_PREFERENCE   | execute session timer without set preference
 * IPPARM_TERMINATE_CALL_WHEN_EXPIRES  | terminate call when session is about to expire
 */
#define IPPARM_SESSION_EXPIRES                  0x1
#define IPPARM_MIN_SE                           0x2
#define IPPARM_REFRESHER_PREFERENCE             0x3
#define IPPARM_REFRESH_METHOD                   0x4
#define IPPARM_REFRESH_WITHOUT_REMOTE_SUPPORT   0x5
#define IPPARM_REFRESH_WITHOUT_PREFERENCE       0x6
#define IPPARM_TERMINATE_CALL_WHEN_EXPIRES      0x7

/* values for IPPARM_REFRESHER_PREFERENCE parameter */
#define IP_REFRESHER_LOCAL                      0x00
#define IP_REFRESHER_REMOTE                     0x01
#define IP_REFRESHER_DONT_CARE                  0x02

/* values for IPPARM_REFRESH_METHOD parameter */
#define IP_REFRESH_REINVITE                        0x00
#define IP_REFRESH_UPDATE                          0x01

/* values for IPPARM_REFRESH_WITHOUT_REMOTE_SUPPORT parameter */
#define IP_REFRESH_WITHOUT_REMOTE_SUPPORT_DISABLE  0x00
#define IP_REFRESH_WITHOUT_REMOTE_SUPPORT_ENABLE   0x01

/* values for IPPARM_REFRESH_WITHOUT_PREFERENCE parameter */
#define IP_REFRESH_WITHOUT_PREFERENCE_DISABLE      0x00
#define IP_REFRESH_WITHOUT_PREFERENCE_ENABLE       0x01

/* values for IPPARM_TERMINATE_CALL_WHEN_EXPIRES parameter */
#define IP_TERMINATE_CALL_WHEN_EXPIRES_DISABLE     0x00
#define IP_TERMINATE_CALL_WHEN_EXPIRES_ENABLE      0x01



/* Following defines added for IP CCLIB Lan Cable disconnect alarms */
#define MAX_IPCCLIB_ALARMS 1  /* Currently only LAN cable disconnect alarm */



#endif


