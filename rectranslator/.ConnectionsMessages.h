/***************************************************************
		Connection Message Source
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
// MessageId: CONNECTIONS_CONNECTION_CONFIG_TREE_NOT_FOUND
//
// MessageText:
//
//  Config Tree for Connection "%1" is not found
//
#define CONNECTIONS_CONNECTION_CONFIG_TREE_NOT_FOUND ((DWORD)0xC0002711L)

//
// MessageId: CONNECTIONS_CONNECTION_TYPE_INVALID
//
// MessageText:
//
//  Invalid or missing "connectionType" for Connection "%1"
//
#define CONNECTIONS_CONNECTION_TYPE_INVALID ((DWORD)0xC0002712L)

//
// MessageId: CONNECTIONS_FAIL_TO_LOADLIBRARY
//
// MessageText:
//
//  Load library for "%1" failed, the system error message is "%2"
//  
//
#define CONNECTIONS_FAIL_TO_LOADLIBRARY  ((DWORD)0xC0002713L)

