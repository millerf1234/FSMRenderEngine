
//Contains some of the implementation for the FSMException and FSMNamedException types

#include "FSMException.h"

//Function Prototype for an implementation utility function
std::string getNameStringForExcpt(FSMNamedException::ExceptionName) noexcept;


FSMNamedException::NamedException::NamedException(const NamedException& that) noexcept : mName_(that.mName_), mNameStr_(that.mNameStr_) {
    LOG(TRACE) << __FUNCTION__;
}

FSMNamedException::NamedException::NamedException(NamedException&& that) noexcept : mName_(that.mName_), mNameStr_(that.mNameStr_) {
    LOG(TRACE) << __FUNCTION__;
}

FSMNamedException::NamedException& FSMNamedException::NamedException::operator=(const NamedException& that) noexcept {
    LOG(TRACE) << __FUNCTION__;
    mName_ = that.mName_;
    mNameStr_ = that.mNameStr_;
}

FSMNamedException::NamedException& FSMNamedException::NamedException::operator=(NamedException&& that) noexcept {
    LOG(TRACE) << __FUNCTION__;
    mName_ = that.mName_;
    mNameStr_ = that.mNameStr_;
}

FSMNamedException::NamedException::NamedException(const ExceptionName excptName) noexcept : mName_(excptName) {
    LOG(TRACE) << __FUNCTION__;
    mNameStr_ = getNameStringForExcpt(excptName);
}

FSMNamedException::NamedException::~NamedException() noexcept {
    LOG(TRACE) << __FUNCTION__;
}




//         Operators

bool FSMNamedException::NamedException::operator==(const NamedException& that) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (mName_ == that.mName_);
}

bool FSMNamedException::NamedException::operator!=(const NamedException& that) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (mName_ != that.mName_);
}




//         Accessors

//Get the string that contains this FSMNamedException's exception name
std::string FSMNamedException::NamedException::nameStr() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return mNameStr_;
}

//Get the enum value representing the name of this FSMNamedException's
//named exception
FSMNamedException::ExceptionName FSMNamedException::NamedException::name() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return mName_;
}





//////////////////////////////////////////////
////    Helper Function Implementation    ////
//////////////////////////////////////////////



std::string getNameStringForExcpt(FSMNamedException::ExceptionName name) noexcept {
    LOG(TRACE) << __FUNCTION__;
    std::string nameStr;

    switch (name) {
    
    default:
        nameStr = "UNIMPLEMENTED EXCEPTION NAME!";
        break;

    case FSMNamedException::ExceptionName::LOGGING_INIT_FAILURE:
        nameStr = "LOGGING_INIT_FAILURE";
        break;

    case FSMNamedException::ExceptionName::FILESYSTEM_ERROR:
        nameStr = "FILESYSTEM_ERROR";
        break;
    
    case FSMNamedException::ExceptionName::NO_GL_DRIVER:
        nameStr = "NO_GL_DRIVER";
        break;

    case FSMNamedException::ExceptionName::NO_MONITORS_DETECTED:
        nameStr = "NO_MONITORS_DETECTED";
        break;

    case FSMNamedException::ExceptionName::INVALID_MONITOR_INDEX:
        nameStr = "INVALID_MONITOR_INDEX";
        break;

        //More to come in the near future...


    
    }

    return nameStr;
}



