/***************************************************************
		SSLED_MP_Adapter Message Source
****************************************************************/
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: SSLED_MP_ADAPTER_CONFIG_TREE_NOT_FOUND
//
// MessageText:
//
//  Config Tree for the SSLED_MP_Adapter is not found, using default configuration
//
#define SSLED_MP_ADAPTER_CONFIG_TREE_NOT_FOUND ((DWORD)0x400014D3L)

//
// MessageId: SSLED_MP_ADAPTER_INIT_SUCCEEDED
//
// MessageText:
//
//  The SSLED_MP_Adapter initialization succeeded
//
#define SSLED_MP_ADAPTER_INIT_SUCCEEDED  ((DWORD)0x400014D4L)

//
// MessageId: SSLED_MP_ADAPTER_INIT_FAILED
//
// MessageText:
//
//  The SSLED_MP_Adapter initialization failed : %1
//  
//
#define SSLED_MP_ADAPTER_INIT_FAILED     ((DWORD)0xC00014D5L)

//
// MessageId: SSLED_MP_ADAPTER_CONFIG_INFO
//
// MessageText:
//
//  Using the following SSLED_MP_Adapter configuration: %1
//  
//
#define SSLED_MP_ADAPTER_CONFIG_INFO     ((DWORD)0x400014D6L)

//
// MessageId: SSLED_MP_ADAPTER_REQUEST_QUEUE_FULL
//
// MessageText:
//
//  SSLED_MP_Adapter Request Queue is full
//  
//
#define SSLED_MP_ADAPTER_REQUEST_QUEUE_FULL ((DWORD)0xC00014D7L)

//
// MessageId: SSLED_MP_ADAPTER_DESTROYED
//
// MessageText:
//
//  SSLED_MP_Adapter is destroyed
//  
//
#define SSLED_MP_ADAPTER_DESTROYED       ((DWORD)0x400014D8L)

//
// MessageId: SSLED_MP_ADAPTER_INTEREST_SPEC_NOT_SUPPORTED
//
// MessageText:
//
//  The InterestSpec Type is not supported by the SSLED_MP_Adapter: %1
//  
//
#define SSLED_MP_ADAPTER_INTEREST_SPEC_NOT_SUPPORTED ((DWORD)0x800014D9L)

//
// MessageId: SSLED_MP_CONNECTION_CONFIG_TREE_NOT_FOUND
//
// MessageText:
//
//  Config Tree for the SSLED_MP_Connection is not found, using default configuration : %1
//  
//
#define SSLED_MP_CONNECTION_CONFIG_TREE_NOT_FOUND ((DWORD)0xC00014DAL)

//
// MessageId: SSLED_MP_CONNECTION_TYPE_INVALID
//
// MessageText:
//
//  Invalid or missing "connectionType" for SSLED_MP_Connection "%1"
//  
//
#define SSLED_MP_CONNECTION_TYPE_INVALID ((DWORD)0xC00014DBL)

//
// MessageId: SSLED_MP_CONNECTION_INIT_FAILED
//
// MessageText:
//
//  Initialization of SSLED_MP_Connection "%1"  failed
//  
//
#define SSLED_MP_CONNECTION_INIT_FAILED  ((DWORD)0xC00014DCL)

//
// MessageId: SSLED_MP_CONNECTION_CONFIG_INFO
//
// MessageText:
//
//  Using the following  configuration for SSLED_MP_Connection "%1": %2
//  
//
#define SSLED_MP_CONNECTION_CONFIG_INFO  ((DWORD)0x400014DDL)

//
// MessageId: SSLED_MP_CONNECTION_DESTROYED
//
// MessageText:
//
//  SSLED_MP_Connection "%1" destroyed
//  
//
#define SSLED_MP_CONNECTION_DESTROYED    ((DWORD)0x400014DEL)

//
// MessageId: SSLED_MP_CONNECTION_INIT_SSLLIB_FAIL
//
// MessageText:
//
//  SSLED_MP_Connection "%1" init SSL lib failed due to %2
//  
//
#define SSLED_MP_CONNECTION_INIT_SSLLIB_FAIL ((DWORD)0xC00014DFL)

//
// MessageId: SSLED_MP_SESSION_GET_SSLPROPERTY_FAIL
//
// MessageText:
//
//  SSLED Client Session "%1" "%2" fail due to %3
//  
//
#define SSLED_MP_SESSION_GET_SSLPROPERTY_FAIL ((DWORD)0xC00014E0L)

//
// MessageId: SSLED_MP_SESSION_SSLPROPERTY_INFO
//
// MessageText:
//
//  SSLED_MP_Connection "%1" "%2" value is %3
//  
//
#define SSLED_MP_SESSION_SSLPROPERTY_INFO ((DWORD)0x400014E1L)

//
// MessageId: SSLED_MP_CONNECTION_INIT_RECONNECT
//
// MessageText:
//
//  Initialization of SSLED_MP_Connection "%1"  failed, will retry in %2 milliseconds
//  
//
#define SSLED_MP_CONNECTION_INIT_RECONNECT ((DWORD)0x400014E2L)

//
// MessageId: SSLED_MP_CONNECTION_SESSION_EVENT
//
// MessageText:
//
//  SSLED_MP_Connection ssl session event : %1
//  
//
#define SSLED_MP_CONNECTION_SESSION_EVENT ((DWORD)0x400014E3L)

//
// MessageId: SSLED_MP_ADAPTER_MANAGED_PUBLISHING_FAILED
//
// MessageText:
//
//  Managed Publishing request failed for %1, error: %2
//  
//  
//
#define SSLED_MP_ADAPTER_MANAGED_PUBLISHING_FAILED ((DWORD)0x800014E4L)

//
// MessageId: SSLED_MP_DATA_DICTIONARY_INFO
//
// MessageText:
//
//  SSLED_MP data dictionary information: %1
//  
//
#define SSLED_MP_DATA_DICTIONARY_INFO    ((DWORD)0x400014E5L)

//
// MessageId: SSLED_MP_CONNECTION_SSL_POST_FAILED
//
// MessageText:
//
//  SSLED_MP_Connection failed to post SSL event : %1
//  
//
#define SSLED_MP_CONNECTION_SSL_POST_FAILED ((DWORD)0xC00014E6L)

//
// MessageId: SSLED_MP_SESSION_ACCEPTED
//
// MessageText:
//
//  SSLED_MP_Connection accepts client session, channel ID: %1
//  
//
#define SSLED_MP_SESSION_ACCEPTED        ((DWORD)0x400014E7L)

//
// MessageId: SSLED_MP_SESSION_CREATATION_FAILED
//
// MessageText:
//
//  SSLED_MP_Connection failed to create publish session 
//  
//
#define SSLED_MP_SESSION_CREATATION_FAILED ((DWORD)0xC00014E8L)

//
// MessageId: SSLED_MP_CONNECTION_GET_SSLPROPERTY_FAIL
//
// MessageText:
//
//  SSLED_MP_Connection "%1" "%2" fail due to %3
//  
//
#define SSLED_MP_CONNECTION_GET_SSLPROPERTY_FAIL ((DWORD)0xC00014E9L)

//
// MessageId: SSLED_MP_PUBDRV_SESSION_CLIENT_NO_PERM_DATA
//
// MessageText:
//
//  SSLED_MP_Adapter - Permission Information Publication message has been attempted. Client session does not expect permission data publications. Publication discarded.
//  
//  
//
#define SSLED_MP_PUBDRV_SESSION_CLIENT_NO_PERM_DATA ((DWORD)0x400014EAL)

//
// MessageId: SSLED_MP_CONNECTION_DISPATCH_EVENT_FAIL
//
// MessageText:
//
//  SSLED_MP_Connection - SAPI Dispatch Event returns negative value
//  
//
#define SSLED_MP_CONNECTION_DISPATCH_EVENT_FAIL ((DWORD)0x400014EBL)

//
// MessageId: SSLED_MP_PUBDRV_MOUNT_SRC_LOAD_DOWNGRADE
//
// MessageText:
//
//  SSLED_MP_Connection - Client session requested Source Load information on mount call, but connection configuration disables src load information for connection.
//  
//
#define SSLED_MP_PUBDRV_MOUNT_SRC_LOAD_DOWNGRADE ((DWORD)0x400014ECL)

//
// MessageId: SSLED_MP_PUBDRV_MOUNT_ENTITLEMENTS_DOWNGRADE
//
// MessageText:
//
//  SSLED_MP_Connection - Client session requested Entitlements Data information on mount call, but connection configuration disables entitlement data information for connection.
//  
//
#define SSLED_MP_PUBDRV_MOUNT_ENTITLEMENTS_DOWNGRADE ((DWORD)0x400014EDL)

//
// MessageId: SSLED_MP_SESSION_MAX_LIMIT
//
// MessageText:
//
//  SSLED_MP_Connection - New session rejected. Reach max sessions limit 
//  
//
#define SSLED_MP_SESSION_MAX_LIMIT       ((DWORD)0xC00014EEL)

