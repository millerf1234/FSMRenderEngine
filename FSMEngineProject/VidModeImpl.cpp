//Implementation file for the implementation class. This file is the
//most farthest removed from the public interface of the 4 files (2
//'.cpp' and 2 '.h' used for the VideoMode type. For more details on
// the FSMVideoMode type, please refer to the header FSMVideoMode which
// is what was designed to be seen by the general public. 

#include "VidModeImpl.h"


    //Constructor
FSMVideoMode::VidModeImpl::VidModeImpl(const GLFWvidmode& vid, int physicalWidthMM,
    int physicalHeightMM) : mPhysicalWidthMM_(physicalWidthMM),
    mPhysicalHeightMM_(physicalHeightMM) {

    //Set this object's remaining private fields based off the values of the passed in GLFWvidmode.
    mWidth_ = vid.width;
    mHeight_ = vid.height;
    mRefreshRate_ = vid.refreshRate;
    mRedBits_ = vid.redBits;
    mGreenBits_ = vid.greenBits;
    mBlueBits_ = vid.blueBits;
}

//These should be fine just using a default implementation since the internal
//data is all just POD types
FSMVideoMode::VidModeImpl::VidModeImpl(const  FSMVideoMode::VidModeImpl& that) = default;
FSMVideoMode::VidModeImpl::VidModeImpl(FSMVideoMode::VidModeImpl&& that) noexcept = default;
FSMVideoMode::VidModeImpl&  FSMVideoMode::VidModeImpl::operator=(const  FSMVideoMode::VidModeImpl& that) = default;
FSMVideoMode::VidModeImpl&  FSMVideoMode::VidModeImpl::operator=(FSMVideoMode::VidModeImpl&& that) noexcept = default;

//todo: Rewrite this function to produce a better formated string containing only the most relevant details of this particular video mode
std::string FSMVideoMode::VidModeImpl::toString() const noexcept {
    std::ostringstream vidMode;
    vidMode << "   Video Mode Details:\n";
    vidMode << "\t  DPI(height)=" << getDPI_Height() << ", DPI(width)=" << getDPI_Width() << ", DPI(avg)=" << getDPI_WidthHeightAverage();
    vidMode << "\n\t  Refresh Rate: " << mRefreshRate_ << " hz,\t  [in screen coord] height: " << mHeight_ << ", width: ";
    vidMode << mWidth_ << "\n\t  Color Bit Depth:  R=" << mRedBits_ << " bits, G=" << mGreenBits_ << " bits, B=";
    vidMode << mBlueBits_ << " bits";
    return vidMode.str();
}

bool  FSMVideoMode::VidModeImpl::operator<(const  FSMVideoMode::VidModeImpl& that) const noexcept {

    //Perform comparison steps as outlined in header
    if (mWidth_ < that.mWidth_)
        return true;
    else if (mWidth_ > that.mWidth_)
        return false;
    else {
        if (mHeight_ < that.mHeight_)
            return true;
        else if (mHeight_ > that.mHeight_)
            return false;
        else {
            if (mRefreshRate_ < that.mRefreshRate_)
                return true;
            else if (mRefreshRate_ > that.mRefreshRate_)
                return false;
            else {
                //Compute sum of bit depths
                int bitDepthThis = mRedBits_ + mGreenBits_ + mBlueBits_;
                int bitDepthThat = that.mRedBits_ + that.mGreenBits_ + that.mBlueBits_;
                if (bitDepthThis < bitDepthThat)
                    return true;
                else if (bitDepthThis > bitDepthThat)
                    return false;
                else {
                    //At this point chances are quite likely that both VideoModes are equal.
                    //Let's save time by ruling out this case:
                    if (*this == that)
                        return false;
                    else { //Hmm else there actually is a difference. Let's find it
                        if (mGreenBits_ < that.mGreenBits_)
                            return true;
                        else if (mGreenBits_ > that.mGreenBits_)
                            return false;
                        else {
                            if (mBlueBits_ < that.mBlueBits_)
                                return true;
                            else if (mBlueBits_ > that.mBlueBits_)
                                return false;
                            else {
                                if (mRedBits_ < that.mRedBits_)
                                    return true;
                                else if (mRedBits_ > that.mRedBits_)
                                    return false;
                                else
                                    return false;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool  FSMVideoMode::VidModeImpl::operator>(const  FSMVideoMode::VidModeImpl& that) const noexcept {
    //Perform comparison steps as outlined in header
    if (mWidth_ < that.mWidth_)
        return false;
    else if (mWidth_ > that.mWidth_)
        return true;
    else {
        if (mHeight_ < that.mHeight_)
            return false;
        else if (mHeight_ > that.mHeight_)
            return true;
        else {
            if (mRefreshRate_ < that.mRefreshRate_)
                return false;
            else if (mRefreshRate_ > that.mRefreshRate_)
                return true;
            else {
                //Compute sum of bit depths
                int bitDepthThis = mRedBits_ + mGreenBits_ + mBlueBits_;
                int bitDepthThat = that.mRedBits_ + that.mGreenBits_ + that.mBlueBits_;
                if (bitDepthThis < bitDepthThat)
                    return false;
                else if (bitDepthThis > bitDepthThat)
                    return true;
                else {
                    //At this point chances are quite likely that both VideoModes are equal.
                    //Let's save time by ruling out this case:
                    if (*this == that)
                        return false;
                    else { //Hmm else there actually is a difference. Let's find it
                        if (mGreenBits_ < that.mGreenBits_)
                            return false;
                        else if (mGreenBits_ > that.mGreenBits_)
                            return true;
                        else {
                            if (mBlueBits_ < that.mBlueBits_)
                                return false;
                            else if (mBlueBits_ > that.mBlueBits_)
                                return true;
                            else {
                                if (mRedBits_ < that.mRedBits_)
                                    return false;
                                else if (mRedBits_ > that.mRedBits_)
                                    return true;
                                else
                                    return false;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool  FSMVideoMode::VidModeImpl::operator==(const  FSMVideoMode::VidModeImpl& that) const noexcept {
    if (this == &that) { return true; }
    //else
    if (mRefreshRate_ == that.mRefreshRate_) {
        if (mWidth_ == that.mWidth_) {
            if (mHeight_ == that.mHeight_) {
                if ((mRedBits_ == that.mRedBits_) && (mGreenBits_ == that.mGreenBits_) && (mBlueBits_ == that.mBlueBits_)) {
                    return true;
                }
            }
        }
    }
    return false;
}
bool  FSMVideoMode::VidModeImpl::operator==(const GLFWvidmode& that) const noexcept {
    if (mWidth_ == that.width) {
        if (mHeight_ == that.height) {
            if (mRefreshRate_ == that.refreshRate) {
                if ((mRedBits_ == that.redBits) && (mGreenBits_ == that.greenBits) && (mBlueBits_ == that.blueBits)) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool  FSMVideoMode::VidModeImpl::operator!=(const  FSMVideoMode::VidModeImpl& that) const noexcept {
    if (this == &that) { return false; }
    //else
    if (mRefreshRate_ == that.mRefreshRate_) {
        if (mWidth_ == that.mWidth_) {
            if (mHeight_ == that.mHeight_) {
                if ((mRedBits_ == that.mRedBits_) && (mGreenBits_ == that.mGreenBits_) && (mBlueBits_ == that.mBlueBits_)) {
                    return false;
                }
            }
        }
    }
    return true;
}
bool  FSMVideoMode::VidModeImpl::operator!=(const GLFWvidmode& that) const noexcept {
    if (mWidth_ == that.width) {
        if (mHeight_ == that.height) {
            if (mRefreshRate_ == that.refreshRate) {
                if ((mRedBits_ == that.redBits) && (mGreenBits_ == that.greenBits) && (mBlueBits_ == that.blueBits)) {
                    return false;
                }
            }
        }
    }
    return true;
}

double  FSMVideoMode::VidModeImpl::getPhysicalHeightInches() const noexcept {
    return (static_cast<double>(mPhysicalHeightMM_) / MILLIMETERS_PER_INCH);
}

double  FSMVideoMode::VidModeImpl::getPhysicalWidthInches() const noexcept {
    return (static_cast<double>(mPhysicalWidthMM_) / MILLIMETERS_PER_INCH);
}

double  FSMVideoMode::VidModeImpl::getPhysicalDisplaySizeMillimeters() const noexcept {
    double heightSquared = pow(static_cast<double>(mHeight_), 2.0);
    double widthSquared = pow(static_cast<double>(mWidth_), 2.0);
    return sqrt(heightSquared + widthSquared);
}

double  FSMVideoMode::VidModeImpl::getPhysicalDisplaySizeInches() const noexcept {
    double heightSquared = pow(getPhysicalHeightInches(), 2.0);
    double widthSquared = pow(getPhysicalWidthInches(), 2.0);
    return sqrt(heightSquared + widthSquared);
}
double  FSMVideoMode::VidModeImpl::getDPI_Height() const noexcept {
    double heightInches = getPhysicalHeightInches();
    if (heightInches == 0) {
        return 0;
    }
    else {
        return (static_cast<double>(mHeight_) / getPhysicalHeightInches());
    }
}
double  FSMVideoMode::VidModeImpl::getDPI_Width() const noexcept {
    double widthInches = getPhysicalWidthInches();
    if (widthInches == 0) {
        return 0;
    }
    else {
        return (static_cast<double>(mWidth_) / getPhysicalWidthInches());
    }
}
double  FSMVideoMode::VidModeImpl::getDPI_WidthHeightAverage() const noexcept {
    return ((getDPI_Height() + getDPI_Width()) / 2.0);
}


