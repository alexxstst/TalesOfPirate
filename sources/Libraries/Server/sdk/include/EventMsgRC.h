//Application=0xFFF)
//LanguageNames=(English=0x1:MSG00001)
//LanguageNames=(British=0x809:MSG00809)
//LanguageNames=(French=0x40c:MSG0040c)
//
#define CAT_STARTING 1
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
// MessageId: 0x00000001L (No symbolic name defined)
//
// MessageText:
//
//  
//


#define CAT_RUNNING 2
//
// MessageId: 0x00000002L (No symbolic name defined)
//
// MessageText:
//
//  
//


#define CAT_STOPPING 3
//
// MessageId: 0x00000003L (No symbolic name defined)
//
// MessageText:
//
//  
//


//
//
// MessageId: EVT_SUCCESS
//
// MessageText:
//
//  %1
//
#define EVT_SUCCESS                      ((DWORD)0x00001000L)

//
// MessageId: EVT_INFO
//
// MessageText:
//
//  %1
//
#define EVT_INFO                         ((DWORD)0x40001001L)

//
// MessageId: EVT_WARNING
//
// MessageText:
//
//  %1
//
#define EVT_WARNING                      ((DWORD)0x80001002L)

//
// MessageId: EVT_ERROR
//
// MessageText:
//
//  %1
//
#define EVT_ERROR                        ((DWORD)0xC0001003L)

