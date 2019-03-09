//I was having issues with implementing the entire VidModeImpl class inside 
//the FSMVideoMode '.cpp' file. So I am moving it all over to be here in its
//own file. This is probably for the best anyways...

#pragma once
#ifndef VID_MODE_IMPL_H_
#define VID_MODE_IMPL_H_

#include "FSMVideoMode.h"
#include "GraphicsLanguageFramework.h"
#include "UniversalIncludes.h"
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
/////////////////    It is all VidModeImpl below this point    /////////////////
////////////////////////////////////////////////////////////////////////////////

    //All public functionality in this class will match function signatures and behaviors as 
    //listed in the FSMVideomode class
class FSMVideoMode::VidModeImpl final {
public:
    FSMVideoMode::VidModeImpl() = delete;
    VidModeImpl(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM);
    ~VidModeImpl() noexcept = default;

    FSMVideoMode::VidModeImpl(const FSMVideoMode::VidModeImpl&);
    FSMVideoMode::VidModeImpl(FSMVideoMode::VidModeImpl&&) noexcept;
    FSMVideoMode::VidModeImpl& operator=(const  FSMVideoMode::VidModeImpl&);
    FSMVideoMode::VidModeImpl& operator=(FSMVideoMode::VidModeImpl&&) noexcept;

    std::string toString() const noexcept;
    bool operator<(const VidModeImpl&) const noexcept;
    bool operator>(const VidModeImpl&) const noexcept;
    bool operator==(const VidModeImpl&) const noexcept;
    bool operator==(const GLFWvidmode&) const noexcept;
    bool operator!=(const VidModeImpl&) const noexcept;
    bool operator!=(const GLFWvidmode&) const noexcept;

    int getWidth() const noexcept { return mWidth_; }
    int getHeight() const noexcept { return mHeight_; }
    int getPhysicalHeightMilliMeters() const noexcept { return mPhysicalHeightMM_; }
    int getPhysicalWidthMilliMeters() const noexcept { return mPhysicalWidthMM_; }
    double getPhysicalHeightInches() const noexcept;
    double getPhysicalWidthInches() const noexcept;
    double getPhysicalDisplaySizeMillimeters() const noexcept;
    double getPhysicalDisplaySizeInches() const noexcept;
    double getDPI_Height() const noexcept;
    double getDPI_Width() const noexcept;
    double getDPI_WidthHeightAverage() const noexcept;

    int getRefreshRate() const noexcept { return mRefreshRate_; }
    int getRedBitDepth() const noexcept { return mRedBits_; }
    int getGreenBitDepth() const noexcept { return mGreenBits_; }
    int getBlueBitDepth() const noexcept { return mBlueBits_; }

private:
    static constexpr const double MILLIMETERS_PER_INCH = 25.4; //Used in screen DPI computations 
    static constexpr const double INCHES_PER_MILLIMETER = 0.0393701;

    int mWidth_, mHeight_; //Measured in screen coordinates
    int mPhysicalWidthMM_, mPhysicalHeightMM_; //measured in millimeters, not guaranteed to be accurate.
    int mRefreshRate_;
    int mRedBits_, mGreenBits_, mBlueBits_;
};

#endif //VID_MODE_IMPL_H_