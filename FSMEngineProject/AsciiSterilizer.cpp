
//This code was never completed 

#include "AsciiSterilizer.h"


#if 0

//Determines if a sequence of characters constitute a valid target
//for purging from a source
bool AsciiSterilizer::validateCharSeq(std::string_view seq) noexcept {
    if (seq.length() < static_cast<size_t>(2u))
        return false;

    return true;
}


 AsciiSterilizer::AsciiSterilizer(std::string_view targetValidCharSeq) :
     mTargetCharSeq_(targetValidCharSeq) {
        LOG(TRACE) << __FUNCTION__;
        if (!validateCharSeq(mTargetCharSeq_))
            throw FSMNamedException(FSMNamedException::NamedException::INVALID_ASCII_SEQ,
                "Invalid Char Sequence!\n");
    }


AsciiSterilizer::AsciiSterilizer(const AsciiSterilizer& that) :
 mTargetCharSeq_(that.mTargetCharSeq_) {
    LOG(TRACE) << __FUNCTION__;

}


AsciiSterilizer::AsciiSterilizer(AsciiSterilizer&& that) noexcept :
    mTargetCharSeq_(that.mTargetCharSeq_) {
     LOG(TRACE) << __FUNCTION__;
}




AsciiSterilizer::~AsciiSterilizer() noexcept {
    LOG(TRACE) << __FUNCTION__;
}

#endif 