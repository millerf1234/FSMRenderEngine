//
// File:            GetSystemDateTime.h
//
// Description:     Contains the necessary functionality for getting the date and time from the OS. 
//                
// Exceptions:      The constructor for this type may throw an exception
// 
// Warning:         This is unfortunately not a very portable task, which is why this functionality has
//                  been placed in its own header/implementation file. 
//
// 2019_083_00_56_52

#pragma once

#ifndef GET_SYSTEM_DATE_TIME_H_
#define GET_SYSTEM_DATE_TIME_H_

#include <string>
#include <optional>

class SystemDateTime {
public:
    SystemDateTime() : mYear_(1970), mMonth_(Month::January), mMonthNum_(1),
        mDay_(Day::Saturday), mDayOfMonth_(1), mDayOfYear_(1), mHour_(12), mMinute_(0),
        mSecond_(0), mMillisecond_(0) {
    
    }
    enum class Month { January, February, March, April, May, June, July, August, September, October, November, December };
    enum class Day { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};
    

private:
    int mYear_;
    Month mMonth_;
    int mMonthNum_;
    Day mDay_;
    int mDayOfMonth_;
    int mDayOfYear_;
    
    int mHour_;
    int mMinute_;
    int mSecond_;
    int mMillisecond_;
};




#endif //GET_SYSTEM_DATE_TIME_H_ 