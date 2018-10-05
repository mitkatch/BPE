/***************************************************************
		Session Layer Message Source
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
// MessageId: SLM_GENERIC_ONE
//
// MessageText:
//
//  %1
//
#define SLM_GENERIC_ONE                  ((DWORD)0x400003E8L)

//
// MessageId: SLM_GENERIC_TWO
//
// MessageText:
//
//  P[1]:%1, P[2]:%2
//
#define SLM_GENERIC_TWO                  ((DWORD)0x400003E9L)

//
// MessageId: SLM_INITIALIZED_ONE
//
// MessageText:
//
//  Initialization Complete for %1
//
#define SLM_INITIALIZED_ONE              ((DWORD)0x400003EAL)

//
// MessageId: SLM_UNEXPECTED_ONE
//
// MessageText:
//
//  Unexpected error: %1
//
#define SLM_UNEXPECTED_ONE               ((DWORD)0xC00003EBL)

