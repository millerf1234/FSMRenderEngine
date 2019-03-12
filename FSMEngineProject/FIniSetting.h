//
//  Object representing a single parse-able setting from a '.fini' initialization file.
//
//


#pragma once

#ifdef WORK_IN_PROGRESS

#ifndef FINI_SETTING_H_
#define FINI_SETTING_H_

#include <string>
#include <string_view>

template <typename T> 
class FIniSetting final {
public:
    FIniSetting() = delete;
    FInitSetting(std::string_view name, T defaultValue);

    

private:
    const std::string mName_;
    T mValue_;
    const T mDefaultValue_;

};


template <typename T>
FInitSetting(std::string_view name, T defaultValue) {

}


#endif //FINI_SETTING_H_

#endif //WORK_IN_PROGRESS