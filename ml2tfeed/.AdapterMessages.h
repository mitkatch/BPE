/***************************************************************
		Adapter Message Source
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
// MessageId: ADAPTER_CONNECTION_STATUS_CHANGE
//
// MessageText:
//
//  Connection Status Changed, Connection: %1 %2
//
#define ADAPTER_CONNECTION_STATUS_CHANGE ((DWORD)0x40004E23L)

//
// MessageId: ADAPTER_SERVICE_STATUS_CHANGE
//
// MessageText:
//
//  Service Status Changed, Connection: %1, Service: %2
//
#define ADAPTER_SERVICE_STATUS_CHANGE    ((DWORD)0x40004E24L)

//
// MessageId: ADAPTER_CONNECTION_INVALID_INFRASTRUCTURE_TYPE
//
// MessageText:
//
//  Invalid infrastructure type %3, Connection: %1, Service: %2
//
#define ADAPTER_CONNECTION_INVALID_INFRASTRUCTURE_TYPE ((DWORD)0x40004E25L)

//
// MessageId: ADAPTER_CONNECTION_INVALID_DATA_FORMAT
//
// MessageText:
//
//  Invalid data format %3, Connection: %1, Service: %2
//
#define ADAPTER_CONNECTION_INVALID_DATA_FORMAT ((DWORD)0x40004E26L)

//
// MessageId: ADAPTER_CONNECTION_INVALID_TIMELINESS
//
// MessageText:
//
//  Invalid timeliness %3, Connection: %1, Service: %2
//
#define ADAPTER_CONNECTION_INVALID_TIMELINESS ((DWORD)0x40004E27L)

//
// MessageId: ADAPTER_CONNECTION_INVALID_RATE
//
// MessageText:
//
//  Invalid rate %3, Connection: %1, Service: %2
//
#define ADAPTER_CONNECTION_INVALID_RATE  ((DWORD)0x40004E28L)

//
// MessageId: ADAPTER_SERVICE_SECTOR_GUESSING_PARSE_ERROR
//
// MessageText:
//
//  Parse error %3, Connection: %1, Key: %2
//
#define ADAPTER_SERVICE_SECTOR_GUESSING_PARSE_ERROR ((DWORD)0x40004E84L)

//
// MessageId: ADAPTER_SERVICE_SECTOR_GUESSING_NO_SUCH_FILE
//
// MessageText:
//
//  No such file %3, Connection: %1, Key: %2
//
#define ADAPTER_SERVICE_SECTOR_GUESSING_NO_SUCH_FILE ((DWORD)0x40004E85L)

//
// MessageId: ADAPTER_SERVICE_SECTOR_GUESSING_NO_SUCH_KEY
//
// MessageText:
//
//  No such key %3, Connection: %1, Key: %2
//
#define ADAPTER_SERVICE_SECTOR_GUESSING_NO_SUCH_KEY ((DWORD)0x40004E86L)

//
// MessageId: ALM_GENERIC_TWO
//
// MessageText:
//
//  P[1]:%1, P[2]:%2
//
#define ALM_GENERIC_TWO                  ((DWORD)0x40004E87L)

