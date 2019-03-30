//
// File:            GetSystemDateTime.h
//
// Description:     Contains the necessary functionality for getting the date and time from the OS. 
//                
// Exceptions:      The constructor for this type may throw an exception
// 
//
// Reference:       
//                  [MSVC]
//                  https://docs.microsoft.com/en-us/cpp/c-runtime-library/time-management?view=vs-2017
// Warning:         This is unfortunately not a very portable task, which is why this functionality has
//                  been placed in its own header/implementation file. 
//
// 2019_083_00_56_52

#pragma once

#ifndef GET_SYSTEM_DATE_TIME_H_
#define GET_SYSTEM_DATE_TIME_H_

#include <string>
#include <optional>


std::string getDateTimeStringTagFromOS() noexcept;

namespace GetDateTimeStringInternal {
	//Gets the month as a string based off a numerical value. Following the Windows convention,
	//January is month 1 and December is month 12. Passing in a 0 will give December.
	inline std::string getMonthName(size_t month) {
		month = month % 12;
		std::string monthStr;
		switch (month) {
		case 0u:
			monthStr = "December";
			break;
		case 1u:
			monthStr = "January";
			break;
		case 2u:
			monthStr = "February";
			break;
		case 3u:
			monthStr = "March";
			break;
		case 4u:
			monthStr = "April";
			break;
		case 5u:
			monthStr = "May";
			break;
		case 6u:
			monthStr = "June";
			break;
		case 7u:
			monthStr = "July";
			break;
		case 8u:
			monthStr = "August";
			break;
		case 9u:
			monthStr = "September";
			break;
		case 10u:
			monthStr = "October";
			break;
		case 11u:
			monthStr = "November";
			break;
		default:   //Default case should be physically impossible
			monthStr = "SystemIsBuggy";
			break;
		}
		return monthStr;
	}
} //namespacce GetDateTimeStringInternal



//I decided a class such as this isn't worth writing at this point
//
//class SystemDateTime {
//public:
//    SystemDateTime() : mYear_(1970), mMonth_(Month::January), mMonthNum_(1),
//        mDay_(Day::Saturday), mDayOfMonth_(1), mDayOfYear_(1), mHour_(12), mMinute_(0),
//        mSecond_(0), mMillisecond_(0) {
//    
//    }
//    enum class Month { January, February, March, April, May, June, July, August, September, October, November, December };
//    enum class Day { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};
//    
//
//private:
//    int mYear_;
//    Month mMonth_;
//    int mMonthNum_;
//    Day mDay_;
//    int mDayOfMonth_;
//    int mDayOfYear_;
//    
//    int mHour_;
//    int mMinute_;
//    int mSecond_;
//    int mMillisecond_;
//};




#endif //GET_SYSTEM_DATE_TIME_H_ 