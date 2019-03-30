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
