//
//  Object representing a single parse-able setting from a '.fini' initialization file.
//
//


#pragma once

#ifndef FINIT_SETTING_H_
#define FINIT_SETTING_H_



/*
#   Welcome to the settings file. This is where the configuration for the
# FSMEngineProject is set. The format for this file is loosely based off
# the '.ini' file format which is commonly seen in other programs.
#
#
#   Below this header you can find a collection of all of the various
# configuration settings which affect the runtime behavior of this
# program. Before editing settings, it is suggested you read the
# following formatting rules:
#
#   (-) Any line beginning with a '#' is a comment and will be
#         ignored during parsing.
#
#   (-) Names for each setting are caps sensitive, so it is suggested
#         to not edit any settings names as they already appear.
#
#   (-) Each Setting has a unique name identifier, which starts with
#         a letter which identifies what type of variable that setting
#         is expecting to parse. Here are the rules for each:
#              'b' stands for boolean and should be only 0 or 1
#              's' stands for string and should be restricted to
#                     ASCII characters only. Spaces are allowed. Please
#                     enclose the string in double quotes, and to add a
#                     double quote to the string, use the escape sequence
#                     ' \" '. To add a backslash, double it up like
#                     ' \\ '.
#              'i' stands for integer and thus can be any positive
#                     or negative whole number (without being too
#                     ridiculously large). No commas please!
#              'u' stands for unsigned. This means any non-negative
#                     integer (again without making it excessively
#                     large).
#              'f' stands for floating point. This is a number that can
#                     be either positive or negative, and is allowed to
#                     contain a single decimal point (aka 'period') to
#                     have it be a non-whole number. Please note that
#                     this program uses 32-bit precision for this type,
#                     so no need to provide excessive precision when
#                     setting these values.
#              'c' stands for character. This should be restricted to a
#                     single ASCII character surrounded by single quotes.
#
#
#   (-) Certain settings may be more restrictive than settings of their
#         type allow. These should be commented where they apply.
#
#
#   (-) Settings are organized by category. Any line starting with a '['
#          character represents the start of a new category. These exist
#          solely to help human readers and will be ignored by this
#          programs parser as if they were a comment.
#
#
#   (-) Typically spaces will be ignored throughout the document where-
#          ever they might appear, but then again this parser isn't going
#          to be winning any "Worlds-Greatest-Parser awards anytime soon
#          so don't be surprised if confusing or ambiguous syntax causes
#          it to choke.
#
#
#   (-) One last thing, one of the primary design rules for this format
#          that anything that fails to parse will be replaced with a new
#          defaulted value for that setting. Thus don't worry if you mess
#          up, because the worst that will happen is that your setting
#          will be ignored and replaced with a properly-formated setting.
#          This goes for anything this parser doesn't agree with reading.
#          So if something seems off, just check this settings file and
#          you should be able to get a pretty clear sense of if you are
#          obeying these syntax rules properly or not.


*/

#include <string_view>
#include <string>

class Setting {
public:
    Setting() = delete;
    Setting(const Setting&) = delete;
    Setting& operator=(const Setting&) = delete;


    explicit Setting(std::string_view name);
    explicit Setting(char * name);
    explicit Setting(const char * name)
    explicit Setting(const std::string& name);
    
    virtual ~Setting() = 0;


private:
    bool mFound_;
    const std::string mName_;

};





#ifdef OLD

#include <string>
#include <string_view>
//#include <optional>

template <typename T> 
class FInitSettingPrototype final {
public:
    FInitSettingPrototype() = delete;
    FInitSettingPrototype(std::string_view name, const T& defaultValue);

    // FInitSettingPrototype(std::string_view name, const T& defaultValue, const T& parsedValue);

    FInitSettingPrototype(const FInitSettingPrototype&);
    FInitSettingPrototype(FInitSettingPrototype&&);

    const char* name() const { return mName_.c_str(); }
    std::string nameStr() const { return mName_; }

    T defaultValue() const { return mDefaultValue_; }


    //No Copying (by operator) allowed!
    //operator= FInitSettingPrototype(const FInitSettingPrototype&) = delete;
    //operator= FInitSettingPrototype(FInitSettingPrototype&&) = delete;

private:
    const std::string mName_;
    //std::optional<T> mValue_;
    const T mDefaultValue_;

};

template <typename T>
FInitSettingPrototype<T>::FInitSettingPrototype(std::string_view name, const T& defaultValue) :
    mName_( name ), mDefaultValue_( defaultValue ) {
}

template <typename T>
FInitSettingPrototype<T>::FInitSettingPrototype(const FInitSettingPrototype& that) :
    mName_(that.mName_), mDefaultValue_(that.mDefaultValue_) {
}

template<typename T> 
FInitSettingPrototype<T>::FInitSettingPrototype(FInitSettingPrototype&& that) :
    mName_(that.mName_), mDefaultValue_(that.mDefaultValue_) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T> 
class FInitSetting final {
    FInitSetting() = delete;
    FInitSetting(std::string_view name, const T& defaultValue, const T& value) :
    mDefaultPrototype_(name, defaultValue), mValue_(value) {
    }

    FInitSetting(const FInitSettingPrototype<T>& defaultPrototype, const T& value) :
        mDefaultPrototype_(defaultPrototype), mValue_(value) {
    }

    explicit operator bool() const { return (mDefaultPrototype_.defaultValue == mValue_); }

    T value() const {
        return mValue_;
    }

private:
    T mValue_;
    FInitSettingPrototype<T> mDefaultPrototype_;

};

#endif //OLD

#endif //FINIT_SETTING_H_

