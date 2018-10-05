/***************************************************************
		SASS3_Adapter Message Source
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
// MessageId: SASS3_ADAPTER_CONFIG_TREE_NOT_FOUND
//
// MessageText:
//
//  Config Tree for the SASS3_Adapter is not found, using default configuration
//
#define SASS3_ADAPTER_CONFIG_TREE_NOT_FOUND ((DWORD)0x40000FA1L)

//
// MessageId: SASS3_ADAPTER_CANNOT_SET_MAINLOOP_TIMER
//
// MessageText:
//
//  Cannot set the mainloop timer, error: %1
//
#define SASS3_ADAPTER_CANNOT_SET_MAINLOOP_TIMER ((DWORD)0xC0000FA2L)

//
// MessageId: SASS3_ADAPTER_MAINLOOP_RETURNED_ERROR
//
// MessageText:
//
//  Mainloop call returned with the error: %1
//
#define SASS3_ADAPTER_MAINLOOP_RETURNED_ERROR ((DWORD)0xC0000FA3L)

//
// MessageId: SASS3_ADAPTER_CANNOT_REMOVE_MAINLOOP_TIMER
//
// MessageText:
//
//  Cannot remove main loop timer, error: %1
//
#define SASS3_ADAPTER_CANNOT_REMOVE_MAINLOOP_TIMER ((DWORD)0xC0000FA4L)

//
// MessageId: SASS3_ADAPTER_REQUEST_QUEUE_FULL
//
// MessageText:
//
//  SASS3_Adapter Request Queue is full
//
#define SASS3_ADAPTER_REQUEST_QUEUE_FULL ((DWORD)0xC0000FA5L)

//
// MessageId: SASS3_ADAPTER_CANNOT_RESET_MAINLOOP_TIMER
//
// MessageText:
//
//  Cannot reset the mainloop timer, error: %1
//
#define SASS3_ADAPTER_CANNOT_RESET_MAINLOOP_TIMER ((DWORD)0xC0000FA6L)

//
// MessageId: SASS3_ADAPTER_CANNOT_STOP_MAINLOOP
//
// MessageText:
//
//  Cannot stop main loop timer, error: %1
//
#define SASS3_ADAPTER_CANNOT_STOP_MAINLOOP ((DWORD)0xC0000FA7L)

//
// MessageId: SASS3_ADAPTER_UNSUPPORTED_MESSAGE_TYPE
//
// MessageText:
//
//  The Message Type is not supported by the SASS3_Adapter: %1
//
#define SASS3_ADAPTER_UNSUPPORTED_MESSAGE_TYPE ((DWORD)0x80000FA8L)

//
// MessageId: SASS3_ADAPTER_INTEREST_SPEC_NOT_SUPPORTED
//
// MessageText:
//
//  The InterestSpec Type is not supported by the SASS3_Adapter: %1
//
#define SASS3_ADAPTER_INTEREST_SPEC_NOT_SUPPORTED ((DWORD)0x80000FA9L)

//
// MessageId: SASS3_ADAPTER_CONFIG_INFO
//
// MessageText:
//
//  Using the following SASS3_Adapter configuration: %1
//
#define SASS3_ADAPTER_CONFIG_INFO        ((DWORD)0x80000FAAL)

//
// MessageId: SASS3_ADAPTER_INIT_SUCCEEDED
//
// MessageText:
//
//  The SASS3_Adapter initialization succeeded
//
#define SASS3_ADAPTER_INIT_SUCCEEDED     ((DWORD)0x40000FABL)

//
// MessageId: SASS3_ADAPTER_INIT_FAILED
//
// MessageText:
//
//  The SASS3_Adapter initialization failed
//
#define SASS3_ADAPTER_INIT_FAILED        ((DWORD)0xC0000FACL)

//
// MessageId: SASS3_ADAPTER_MARKET_DATA_ITEM_SUB_FAILED
//
// MessageText:
//
//  MarketDataItemSub open request failed for %1, error: %2
//
#define SASS3_ADAPTER_MARKET_DATA_ITEM_SUB_FAILED ((DWORD)0x80000FADL)

//
// MessageId: SASS3_ADAPTER_MARKET_DATA_DICT_SUB_FAILED
//
// MessageText:
//
//  MarketDataDictSub open request failed, error: %1
//
#define SASS3_ADAPTER_MARKET_DATA_DICT_SUB_FAILED ((DWORD)0x80000FAEL)

//
// MessageId: SASS3_ADAPTER_DESTROYED
//
// MessageText:
//
//  The SASS3_Adapter destroyed
//
#define SASS3_ADAPTER_DESTROYED          ((DWORD)0xC0000FAFL)

//
// MessageId: SASS3_ADAPTER_CANNOT_INCREASE_MAINLOOP_TIMER
//
// MessageText:
//
//  Increase MainLoop timer to adjust for slow consumer/fast producer failed, error: %1
//
#define SASS3_ADAPTER_CANNOT_INCREASE_MAINLOOP_TIMER ((DWORD)0x80000FB0L)

//
// MessageId: SASS3_ADAPTER_MARKET_DATA_DICT_SUB_FAILED_TIMEOUT
//
// MessageText:
//
//  MarketDataDictSub open request failed, warning: %1
//
#define SASS3_ADAPTER_MARKET_DATA_DICT_SUB_FAILED_TIMEOUT ((DWORD)0x80000FB1L)

//
// MessageId: SASS3_CONNECTION_CONFIG_INFO
//
// MessageText:
//
//  Using the following  configuration for SASS3_Connection "%1": %2
//
#define SASS3_CONNECTION_CONFIG_INFO     ((DWORD)0x80001004L)

//
// MessageId: SASS3_CONNECTION_INIT_SUCCEEDED
//
// MessageText:
//
//  Initialization of SASS3_Connection "%1" succeeded
//
#define SASS3_CONNECTION_INIT_SUCCEEDED  ((DWORD)0x80001005L)

//
// MessageId: SASS3_CONNECTION_INIT_RECONNECT
//
// MessageText:
//
//  Initialization of SASS3_Connection "%1"  failed, will retry in %2 milliseconds
//
#define SASS3_CONNECTION_INIT_RECONNECT  ((DWORD)0x80001006L)

//
// MessageId: SASS3_CONNECTION_DESTROYED
//
// MessageText:
//
//  SASS3_Connection "%1" destroyed
//
#define SASS3_CONNECTION_DESTROYED       ((DWORD)0x80001007L)

//
// MessageId: SASS3_CONNECTION_CONFIG_TREE_NOT_FOUND
//
// MessageText:
//
//  Config Tree for SASS3_Connection "%1" is not found
//
#define SASS3_CONNECTION_CONFIG_TREE_NOT_FOUND ((DWORD)0xC0001008L)

//
// MessageId: SASS3_CONNECTION_TYPE_INVALID
//
// MessageText:
//
//  Invalid or missing "connectionType" for SASS3_Connection "%1"
//
#define SASS3_CONNECTION_TYPE_INVALID    ((DWORD)0xC0001009L)

//
// MessageId: SASS3_CONNECTION_LISTENER_WO_SUBSCRIBER
//
// MessageText:
//
//  For SASS3_Connection "%1" listener_RV_Connection "%2" is configured, when no subscriber_RV_Connection is configured
//
#define SASS3_CONNECTION_LISTENER_WO_SUBSCRIBER ((DWORD)0xC000100AL)

//
// MessageId: SASS3_CONNECTION_EQUAL_SUBSCRIBER_AND_LISTENER
//
// MessageText:
//
//  For SASS3_Connection "%1" listener_RV_Connection and subscriber_RV_Connection cannot be the same ("%2")
//
#define SASS3_CONNECTION_EQUAL_SUBSCRIBER_AND_LISTENER ((DWORD)0xC000100BL)

//
// MessageId: SASS3_CONNECTION_INIT_FAILED
//
// MessageText:
//
//  Initialization of SASS3_Connection "%1"  failed
//
#define SASS3_CONNECTION_INIT_FAILED     ((DWORD)0xC000100CL)

//
// MessageId: SASS3_CONNECTION_NO_CONFIG
//
// MessageText:
//
//  Configuration for SASS3_Connection "%1"  does not have subscriberRV_Connection, or publisherRV_Connection, or contributorRV_Connection
//
#define SASS3_CONNECTION_NO_CONFIG       ((DWORD)0xC000100DL)

//
// MessageId: RV_CONNECTION_CONFIG_INFO
//
// MessageText:
//
//  Using the following configuration for RV_Connection "%1": %2 
//
#define RV_CONNECTION_CONFIG_INFO        ((DWORD)0x80001068L)

//
// MessageId: RV_CONNECTION_INIT_SUCCEEDED
//
// MessageText:
//
//  Initialization of RV_Connection "%1" succeeded. Connection parameters: %2
//
#define RV_CONNECTION_INIT_SUCCEEDED     ((DWORD)0x80001069L)

//
// MessageId: RV_CONNECTION_INIT_RECONNECT
//
// MessageText:
//
//  Initialization of RV_Connection "%1"  failed with error: %2, will retry in %3 milliseconds. Connection parameters: %4
//
#define RV_CONNECTION_INIT_RECONNECT     ((DWORD)0x8000106AL)

//
// MessageId: RV_CONNECTION_DESTROYED
//
// MessageText:
//
//  RV_Connection "%1" destroyed. Connection parameters: %2
//
#define RV_CONNECTION_DESTROYED          ((DWORD)0x8000106BL)

//
// MessageId: RV_CONNECTION_RV_TERM_FAILED
//
// MessageText:
//
//  RV_Connection "%1" could not close RV Session, error: %2. Connection parameters: %3
//
#define RV_CONNECTION_RV_TERM_FAILED     ((DWORD)0x8000106CL)

//
// MessageId: RV_CONNECTION_CONFIG_TREE_NOT_FOUND
//
// MessageText:
//
//  Config Tree for RV_Connection "%1" is not found
//
#define RV_CONNECTION_CONFIG_TREE_NOT_FOUND ((DWORD)0xC000106DL)

//
// MessageId: RV_CONNECTION_TYPE_INVALID
//
// MessageText:
//
//  Invalid or missing "connectionType" for RV_Connection "%1"
//
#define RV_CONNECTION_TYPE_INVALID       ((DWORD)0xC000106EL)

//
// MessageId: RV_CONNECTION_INIT_FAILED
//
// MessageText:
//
//  Initialization of RV_Connection "%1"  failed. Connection parameters: %2 
//
#define RV_CONNECTION_INIT_FAILED        ((DWORD)0xC000106FL)

//
// MessageId: SASS3_ADAPTER_GENERIC_SUB_FAILED
//
// MessageText:
//
//  GenericSub open request failed, error: %1
//
#define SASS3_ADAPTER_GENERIC_SUB_FAILED ((DWORD)0x80001070L)

//
// MessageId: SASS3_ADAPTER_SUBSCRIBE
//
// MessageText:
//
//  Subscribing to "%1"
//
#define SASS3_ADAPTER_SUBSCRIBE          ((DWORD)0x40001071L)

//
// MessageId: SASS3_ADAPTER_PUBLISH
//
// MessageText:
//
//  Publishing to "%1"
//
#define SASS3_ADAPTER_PUBLISH            ((DWORD)0x40001072L)

//
// MessageId: SASS3_ADAPTER_CONTRIBUTE
//
// MessageText:
//
//  Contributing to "%1
//
#define SASS3_ADAPTER_CONTRIBUTE         ((DWORD)0x40001073L)

//
// MessageId: SASS3_ADAPTER_CANNOT_USE_FLAVOURED_MODE
//
// MessageText:
//
//  Unable to use RVD in flavoured mode. Ticket Files will be used. Error = %1
//
#define SASS3_ADAPTER_CANNOT_USE_FLAVOURED_MODE ((DWORD)0x40001074L)

//
// MessageId: SASS3_MFEED_DATA_DICTIONARY_INFO
//
// MessageText:
//
//  SASS3 download full Market Feed Data dictionary : %1
//
#define SASS3_MFEED_DATA_DICTIONARY_INFO ((DWORD)0x40001075L)

//
// MessageId: SASS3_ADAPTER_STATUS_MAP_LOG_CHANGED_ONLY
//
// MessageText:
//
//  The following is status that have been changed for the SASS3_Adapter: %1
//
#define SASS3_ADAPTER_STATUS_MAP_LOG_CHANGED_ONLY ((DWORD)0x40001076L)

//
// MessageId: SASS3_ADAPTER_STATUS_MAP_LOG_FULL
//
// MessageText:
//
//  The following is full status map for the SASS3_Adapter: %1
//
#define SASS3_ADAPTER_STATUS_MAP_LOG_FULL ((DWORD)0x40001077L)

//
// MessageId: SASS3_ADAPTER_NOT_SUPPORT_DICT_QFORM
//
// MessageText:
//
//  RFA currently does not read QForm Data Dict from file.
//
#define SASS3_ADAPTER_NOT_SUPPORT_DICT_QFORM ((DWORD)0x40001078L)

