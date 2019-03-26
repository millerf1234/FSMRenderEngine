//
//  File:         GetSystemDateTime.cpp
//
// Warning!     Very OS-Dependent implementations lay ahead. While an effort has been
//              made to keep code portable, this code is only guaranteed to work on my 
//              computer in MSVC 2017 compiling as x64. 
//
//
//

#include "GetSystemDateTime.h"

#if 0

//--------------------------------------------------------------------------------------------------------------
//STEP 1     FIGURE OUT COMPILER AND OS DETAILS SO AS TO FIGURE OUT HOW BEST TO PROCEED WITH IMPLEMENTATION
//--------------------------------------------------------------------------------------------------------------

#ifdef _MSC_VER 
//Windows recommends to instead just include the entire <Windows.h> header, but the specific headers are:
#include <sysinfoapi.h>  //Provides function 'GetSystemTime()'     //see: https://docs.microsoft.com/en-us/windows/desktop/api/sysinfoapi/nf-sysinfoapi-getsystemtime  
#include <minwinbase.h>  //Provides struct SYSTEMTIME              //see: https://docs.microsoft.com/en-us/windows/desktop/api/minwinbase/ns-minwinbase-systemtime  
constexpr const bool USE_WINDOWS_API_TO_GET_TIME_STRING = true;
#ifdef _WIN64
//If platform is 64-bit, it is best to play it safe when converting pointers within the win32 API (I think)
//see: https://docs.microsoft.com/en-us/windows/desktop/WinProg64/the-tools
#include <Basetsd.h>  //Provides macros for safely dealing with both 32 and 64 bit pointers
constexpr const bool USE_64_BIT_CONVERSION_FUNCTIONS_FOR_WINDOWS_API_CALLS = true;
#else 
constexpr const bool USE_64_BIT_CONVERSION_FUNCTIONS_FOR_WINDOWS_API_CALLS = false;
#endif //_WIN64
#else 
//Add a check for POSIX and then use POSIX functions to get time from OS?
#include <ctime>
constexpr const bool USE_WINDOWS_API_TO_GET_TIME_STRING = false;
#endif //_MSC_VER


#endif //0