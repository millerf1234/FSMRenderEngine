//
//  File:         GetSystemDateTime.cpp
//
//

#include "GetSystemDateTime.h"

#include <string>
#include <iostream> 
#include <sstream>
#include <stdexcept>

//This function will need to be ported if platform ever is changed to be something besides Windows
#include <Windows.h>


using namespace GetDateTimeStringInternal;


std::string getDateTimeStringTagFromOS() noexcept {

	static const std::string UNDERSCORE("_");
	std::ostringstream dateTimeStr;
	
	LPSYSTEMTIME lpSystemTime = nullptr;
	SYSTEMTIME localTime;

	lpSystemTime = &localTime;
	GetLocalTime(lpSystemTime);
	dateTimeStr << getMonthName(static_cast<size_t>(localTime.wMonth)) << UNDERSCORE << localTime.wDay 
		<< UNDERSCORE << localTime.wYear << localTime.wHour << UNDERSCORE << localTime.wMinute;
	return dateTimeStr.str();
}


//
//#if 0
//
////--------------------------------------------------------------------------------------------------------------
////STEP 1     FIGURE OUT COMPILER AND OS DETAILS SO AS TO FIGURE OUT HOW BEST TO PROCEED WITH IMPLEMENTATION
////--------------------------------------------------------------------------------------------------------------
//
//#ifdef _MSC_VER 
////Windows recommends to instead just include the entire <Windows.h> header, but the specific headers are:
//#include <sysinfoapi.h>  //Provides function 'GetSystemTime()'     //see: https://docs.microsoft.com/en-us/windows/desktop/api/sysinfoapi/nf-sysinfoapi-getsystemtime  
//#include <minwinbase.h>  //Provides struct SYSTEMTIME              //see: https://docs.microsoft.com/en-us/windows/desktop/api/minwinbase/ns-minwinbase-systemtime  
//constexpr const bool USE_WINDOWS_API_TO_GET_TIME_STRING = true;
//#ifdef _WIN64
////If platform is 64-bit, it is best to play it safe when converting pointers within the win32 API (I think)
////see: https://docs.microsoft.com/en-us/windows/desktop/WinProg64/the-tools
//#include <Basetsd.h>  //Provides macros for safely dealing with both 32 and 64 bit pointers
//constexpr const bool USE_64_BIT_CONVERSION_FUNCTIONS_FOR_WINDOWS_API_CALLS = true;
//#else 
//constexpr const bool USE_64_BIT_CONVERSION_FUNCTIONS_FOR_WINDOWS_API_CALLS = false;
//#endif //_WIN64
//#else 
////Add a check for POSIX and then use POSIX functions to get time from OS?
//#include <ctime>
//constexpr const bool USE_WINDOWS_API_TO_GET_TIME_STRING = false;
//#endif //_MSC_VER
//
//
//#endif //0


//
//   Old code:
//
//	//The idea of this function is to get the current time and date from the operating 
//	//system and format it into a std::string object with human-readable formatting to be
//	//used in formulating a syntactically valid filepath. Unfortunately this task is easier
//	//said then done, especially when building with modern MSVC. 
//std::string getDateTimeTag() {
//
//	return getDateTimeStringTagFromOS();
//
//	//Use preprocessor to use date+time at time-of-compilation
//	std::stringstream tag;
//	tag /*<< "TheImplementationForGettingDateAndTimeIsBeingReimplementedInASeperateFile"*/ << __DATE__ << __TIME__;
//	std::string tagString = tag.str();
//	auto tagIter = tagString.begin();
//	for (; tagIter != tagString.end(); tagIter++) {
//		switch (*tagIter) {
//		default:
//			break;
//		case (' '):
//			*tagIter = '_';
//			break;
//		case (','):
//			*tagIter = '_';
//			break;
//		case (':'):
//			*tagIter = '_';
//			break;
//		}
//	}
//	return tagString;
//#if 0
//	//The formated timetag is built using a stringstream
//	std::stringstream tag;
//
//	if constexpr (USE_WINDOWS_API_TO_GET_TIME_STRING) {
//		//The Windows API is used to get the system time in a struct which looks like:
//		//   typedef struct _SYSTEMTIME {
//		//       WORD wYear;
//		//       WORD wMonth;
//		//       WORD wDayOfWeek;
//		//       WORD wDay;
//		//       WORD wHour;
//		//       WORD wMinute;
//		//       WORD wSecond;
//		//       WORD wMilliseconds;
//		//   } SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
//		FILETIME timeFromOS;
//
//		// GetSystemTime(&timeFromOS);
//
//	}
//	else {
//		//We can use C++11's <chrono> library to get the current time from the system clock.
//		std::chrono::time_point timepointFromSystemClock = std::chrono::system_clock::now();
//
//		//Here is where things start to get a bit hairy. Unfortunately the main purpose of <chrono>
//		//is to provide a utility for measuring durations of time and (as of C++17) does not
//		//contain functionality to allow direct conversions of its types into usable date-time strings.                   (C++20 is expected to expand the available functionality of <chrono>)
//		//So the best <chrono> can offer us is a function to convert our timepoint to a time_t,
//		//which is a very old-school type which dates back to the C darkages (well, actually to
//		//1988 and the first POSIX standard).                                                                         //see: https://en.wikipedia.org/wiki/Unix_time#History
//
//
//		const time_t timetag2 = (std::chrono::system_clock::to_time_t(timepointFromSystemClock));
//
//
//
//#if 0
//
//		const time_t const* constTimeTag2 = &timetag2;
//#ifdef _MSC_VER
//		//MSVC has deprecated many C functions and replaced them with their own '_s' versions. Since we are to use the C function 'localtime()' here to get 
//		//the date and time from the OS, we check to see if our compiler is MSVC and if so, then we must use 'localtime_s()'.
//		tm timetagC;
//		errno_t err = 0; //0 means no error
//		err = localtime_s(&timetagC, constTimeTag2);
//		if (err != 0) {
//			fprintf(stderr, "\n\nError occurred within CRT function 'localtime_s()'! Error code is %d!\n"
//				"For more information on error codes, do Internet search for meanings\n"
//				"of variables of type 'errno_t'.\n\n", err);
//		}
//#else
//		auto timetagC = std::localtime(&timetag2);
//#endif
//
//		//                /////////////////////
//		//                //
//		//                //    HACK   Use of deprecated function
//		//                //
//		//                //MSVC considers the C function 'localtime' to be deprecated since it is not thread safe and (i think) can cause buffer overflow. It is intended to be 
//		//                //replaced with 'localtime_s()' on windows or localtime_r() on POSIX-compliant. To use localtime_s though there needs to be some macros defined, which
//		//                //are:
//		//                //    check to make sure ' __STDC_LIB_EXT1__ ' is defined be the implementation
//		//                //    define ' __STDC_WANT_LIB_EXT1__ '  before the header 'time.h' is included.
//		//                //https://en.cppreference.com/w/c/chrono/localtime
//		//                //https://en.cppreference.com/w/cpp/chrono/c/localtime
//		//                //https://docs.microsoft.com/en-us/cpp/preprocessor/warning?view=vs-2017
//		//                //
//		//                // OR  just do this hack until C++20 provides a much easier way to do this 
//		//                //
//		//                // UPDATE: Instead of disabling warnings to make this function call, the macro '#define _CRT_SECURE_NO_WARNINGS 1' has been added to the BuildSettings.h file
//		//                //         which allows for use of the deprecated C functions which otherwise block compilation
//		////#pragma warning( push )
//		////#pragma warning( disable : 4996 ) 
//		//                auto timetagC = std::localtime(&timetag2); 
//		////#pragma warning ( pop )
//		//                //End of hack
//		//                /////////////////////
//		//
//		//                //  Reference: https://en.cppreference.com/w/cpp/io/manip/put_time
//		//                //Now we can extract the information we need
//					   // tag << put_time(timetagc, "%Y_%j_%H_%M_%S");
//		tag << std::put_time(&timetagC, "%Y_%j_%H_%M_%S"); //Puts the 'day of the year', 'year', 'hour', 'minute', and 'second' into a stringstream
//#endif //0
//		return tag.str();
//	}
//#endif //0
//
//}