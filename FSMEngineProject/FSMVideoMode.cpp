//
// File:                       FSMVideoMode.cpp
//
//
// Description:               Implementation for class which provides Object-Oriented encapsulation for a
//                            GLFWvideomode.
//
// Dependencies:             GLFW 3.0 or newer -- This class is dependent upon the GLFW type
//                                                'GLFWvideomode' which is defined as follows:
//                                                          struct GLFWvidmode {
//                                                           //The width, in screen coordinates, of the video mode.
//                                                             int width;
//                                                           //The height, in screen coordinates, of the video mode.
//                                                             int height;
//                                                           //The bit depth of the red channel of the video mode.
//                                                             int redBits;
//                                                           //The bit depth of the green channel of the video mode.
//                                                             int greenBits;
//                                                           //The bit depth of the blue channel of the video mode.
//                                                             int blueBits;
//                                                           //The refresh rate, in Hz, of the video mode.
//                                                             int refreshRate;
//                                                         };
//                                                   GLFW also provides a separate function which queries a monitors 
//                                                   physical dimensions, which is expected to be used to provide the 
//                                                   physical width and height to this object's constructor.
//
// Programmer:                Forrest Miller
//
// Date:                       First implemented on December 14, 2018
//                             Reimplemented using more encapsulated style on March 3, 2019
//

#include "FSMVideoMode.h"
#include "GraphicsLanguageFramework.h"
#include "UniversalIncludes.h"
#include <sstream>

using VMInternal::VidModeImpl;

//Implement all of the FSMVideoMode functions by just having them call the 
//corresponding VidModeImpl versions

FSMVideoMode::FSMVideoMode(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM) {
    pVidModeImpl_ = std::make_unique<VidModeImpl>(vid, physicalWidthMM, physicalHeightMM);
}
FSMVideoMode::~FSMVideoMode() = default;
FSMVideoMode::FSMVideoMode(const FSMVideoMode& that) {
    pVidModeImpl_ = std::make_unique<VidModeImpl>(*(that.pVidModeImpl_));
}
FSMVideoMode::FSMVideoMode(FSMVideoMode&& that) noexcept {
    *this = std::move(that);
}
FSMVideoMode& FSMVideoMode::operator=(const FSMVideoMode& that) {
    if (this != &that) {
        pVidModeImpl_ = std::make_unique<VidModeImpl>(*(that.pVidModeImpl_));
    }
    return *this;
}
FSMVideoMode& FSMVideoMode::operator=(FSMVideoMode&& that) noexcept {
    if (this != &that) {
        pVidModeImpl_ = std::move(that.pVidModeImpl_);
    }
    return *this;
}


std::string FSMVideoMode::toString() const {
    return pVidModeImpl_->toString();
}

bool FSMVideoMode::operator<(const FSMVideoMode& that) const noexcept {
    return (*(this->pVidModeImpl_) < *(that.pVidModeImpl_));
}

bool FSMVideoMode::operator>(const FSMVideoMode& that) const noexcept {
    return (*(this->pVidModeImpl_) > *(that.pVidModeImpl_));
}

bool FSMVideoMode::operator==(const FSMVideoMode& that) const noexcept {
    return (*(this->pVidModeImpl_) == *(that.pVidModeImpl_));
}

bool FSMVideoMode::operator==(const GLFWvidmode& that) const noexcept {
    return (*(this->pVidModeImpl_) == that);
}

bool FSMVideoMode::operator!=(const FSMVideoMode& that) const noexcept {
    return (*(this->pVidModeImpl_) != *(that.pVidModeImpl_));
}

bool FSMVideoMode::operator!=(const GLFWvidmode& that) const noexcept {
    return (*(this->pVidModeImpl_) != that);
}

int FSMVideoMode::getWidth() const noexcept { return pVidModeImpl_->getWidth(); }

int FSMVideoMode::getHeight() const noexcept { return pVidModeImpl_->getHeight(); }


int FSMVideoMode::getPhysicalHeightMilliMeters() const noexcept {
    return pVidModeImpl_->getPhysicalHeightMilliMeters();
}

int FSMVideoMode::getPhysicalWidthMilliMeters() const noexcept {
    return pVidModeImpl_->getPhysicalWidthMilliMeters();
}

double FSMVideoMode::getPhysicalHeightInches() const noexcept {
    return pVidModeImpl_->getPhysicalHeightInches();
}

double FSMVideoMode::getPhysicalWidthInches() const noexcept {
    return pVidModeImpl_->getPhysicalWidthInches();
}

double FSMVideoMode::getPhysicalDisplaySizeMillimeters() const noexcept {
    return pVidModeImpl_->getPhysicalDisplaySizeMillimeters();
}

double FSMVideoMode::getPhysicalDisplaySizeInches() const noexcept {
    return pVidModeImpl_->getPhysicalDisplaySizeInches();
}

double FSMVideoMode::getDPI_Height() const noexcept {
    return pVidModeImpl_->getDPI_Height();
}

double FSMVideoMode::getDPI_Width() const noexcept {
    return pVidModeImpl_->getDPI_Width();
}

double FSMVideoMode::getDPI_WidthHeightAverage() const noexcept {
    return pVidModeImpl_->getDPI_WidthHeightAverage();
}

int FSMVideoMode::getRefreshRate() const noexcept {
    return pVidModeImpl_->getRefreshRate();
}

int FSMVideoMode::getRedBitDepth() const noexcept {
    return pVidModeImpl_->getRedBitDepth();
}
int FSMVideoMode::getGreenBitDepth() const noexcept {
    return pVidModeImpl_->getGreenBitDepth();
}
int FSMVideoMode::getBlueBitDepth() const noexcept {
    return pVidModeImpl_->getBlueBitDepth();
}



////////////////////////////////////////////////////////////////////////////////
/////////////////    It is all VidModeImpl below this point    /////////////////
////////////////////////////////////////////////////////////////////////////////



namespace VMInternal {
    
    //All public functionality in this class will match function signatures and behaviors as 
    //listed in the FSMVideomode class
    class VidModeImpl final {
    public:
        VidModeImpl() = delete;
        VidModeImpl(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM);
        ~VidModeImpl() = default;

        VidModeImpl(const VidModeImpl&);
        VidModeImpl(VidModeImpl&&) noexcept;
        VidModeImpl& operator=(const VidModeImpl&);
        VidModeImpl& operator=(VidModeImpl&&) noexcept;

        std::string toString() const;
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


    //Constructor
    VidModeImpl::VidModeImpl(const GLFWvidmode& vid, int physicalWidthMM,
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

    //These should be fine just being their default version since the internal
    //data is all just POD types
    VidModeImpl::VidModeImpl(const VidModeImpl& that) = default;
    VidModeImpl::VidModeImpl(VidModeImpl&& that) noexcept = default;
    VidModeImpl& VidModeImpl::operator=(const VidModeImpl& that) = default;
    VidModeImpl& VidModeImpl::operator=(VidModeImpl&& that) noexcept = default;

    std::string VidModeImpl::toString() const {
        std::stringstream vidMode;
        vidMode << "DPI(height)=" << getDPI_Height() << ", DPI(width)=" << getDPI_Width() << ", DPI(avg)=" << getDPI_WidthHeightAverage();
        vidMode << "  Refresh Rate: " << mRefreshRate_ << " hz,\t  [in screen coord] h: " << mHeight_ << ", w: ";
        vidMode << mWidth_ << "\n\tColor Bit Depth:  R=" << mRedBits_ << " bits, G=" << mGreenBits_ << " bits, B=";
        vidMode << mBlueBits_ << " bits";
        return vidMode.str();
    }

    bool VidModeImpl::operator<(const VidModeImpl& that) const noexcept {

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

    bool VidModeImpl::operator>(const VidModeImpl& that) const noexcept {
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

    bool VidModeImpl::operator==(const VidModeImpl& that) const noexcept {
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
    bool VidModeImpl::operator==(const GLFWvidmode& that) const noexcept {
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


    bool VidModeImpl::operator!=(const VidModeImpl& that) const noexcept {
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
    bool VidModeImpl::operator!=(const GLFWvidmode& that) const noexcept {
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

    //int VidModeImpl::getWidth() const { return mWidth_; }
    //int VidModeImpl::getHeight() const { return mHeight_; }
  
    double VidModeImpl::getPhysicalHeightInches() const noexcept {
        return (static_cast<double>(mPhysicalHeightMM_) / MILLIMETERS_PER_INCH);
    }

    double VidModeImpl::getPhysicalWidthInches() const noexcept {
        return (static_cast<double>(mPhysicalWidthMM_) / MILLIMETERS_PER_INCH);
    }

    double VidModeImpl::getPhysicalDisplaySizeMillimeters() const {
        double heightSquared = pow(static_cast<double>(mHeight_), 2.0);
        double widthSquared = pow(static_cast<double>(mWidth_), 2.0);
        return sqrt(heightSquared + widthSquared);
    }
    double VidModeImpl::getPhysicalDisplaySizeInches() const {
        double heightSquared = pow(getPhysicalHeightInches(), 2.0);
        double widthSquared = pow(getPhysicalWidthInches(), 2.0);
        return sqrt(heightSquared + widthSquared);
    }
    double VidModeImpl::getDPI_Height() const {
        double heightInches = getPhysicalHeightInches();
        if (heightInches == 0) {
            return 0;
        }
        else {
            return (static_cast<double>(mHeight_) / getPhysicalHeightInches());
        }
    }
    double VidModeImpl::getDPI_Width() const {
        double widthInches = getPhysicalWidthInches();
        if (widthInches == 0) {
            return 0;
        }
        else {
            return (static_cast<double>(mWidth_) / getPhysicalWidthInches());
        }
    }
    double VidModeImpl::getDPI_WidthHeightAverage() const {
        return ((getDPI_Height() + getDPI_Width()) / 2.0);
    }


} //namespace VMInternal






#ifdef USE_OLD_VIDEOMODE_CODE

//File:                         VideoMode.cpp
//
//Description:                 Class that holds the information pertaining to one of a monitors available VideoModes. 
//                             Is based off the GLFW type GLFWvidmode. See header file for more detail.
//
//Programmer:                  Forrest Miller
//Date:                        December 14, 2018

#include "VideoMode.h"

#include <sstream>

//namespace FSMEngineInternal {

	VideoMode::VideoMode(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM) {
		mPhysicalHeightMM_ = physicalHeightMM;
		mPhysicalWidthMM_ = physicalWidthMM;
		mWidth_ = vid.width;
		mHeight_ = vid.height;
		mRedBits_ = vid.redBits;
		mGreenBits_ = vid.greenBits;
		mBlueBits_ = vid.blueBits;
		mRefreshRate_ = vid.refreshRate;
	}

	std::string VideoMode::toString() const {
		std::stringstream vidMode;
		vidMode << "DPI(height)=" << getDPI_Height() << ", DPI(width)=" << getDPI_Width() << ", DPI(avg)=" << getDPI_WidthHeightAverage();
		vidMode	<< "  Refresh Rate: " << mRefreshRate_ << " hz,\t  [in screen coord] h: " << mHeight_ << ", w: ";
		vidMode << mWidth_ << "\n\tColor Bit Depth:  R=" << mRedBits_ << " bits, G=" << mGreenBits_ << " bits, B=";
		vidMode << mBlueBits_ << " bits";
		return vidMode.str();
	}

	//Comparison steps are performed in the following order: 
	//    (1)Width, (2)Height, (3)Refresh Rate, (4)Sum of Bit Depths, (5)Green bit depth,
	//           (6)Blue bit depth, (7)Red bit depth
	bool VideoMode::operator<(const VideoMode& that) const {
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

	//Comparison steps are performed in the following order: 
	//    (1)Width, (2)Height, (3)Refresh Rate, (4)Sum of Bit Depths, (5)Green bit depth,
	//           (6)Blue bit depth, (7)Red bit depth
	bool VideoMode::operator>(const VideoMode& that) const {
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


	bool VideoMode::operator==(const VideoMode& that) const {
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
	
	bool VideoMode::operator==(const GLFWvidmode& that) const {
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
	
	bool VideoMode::operator!=(const VideoMode& that) const {
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

	bool VideoMode::operator!=(const GLFWvidmode& that) const {
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

	
	int VideoMode::getWidth() const {
		return mWidth_;
	}
	
	int VideoMode::getHeight() const {
		return mHeight_;
	}

	int VideoMode::getPhysicalHeightMilliMeters() const {
		return mPhysicalHeightMM_;
	}
	
	int VideoMode::getPhysicalWidthMilliMeters() const {
		return mPhysicalWidthMM_;
	}
	
	double VideoMode::getPhysicalHeightInches() const {
		return (static_cast<double>(mHeight_) / MILLIMETERS_PER_INCH);
	}
	
	double VideoMode::getPhysicalWidthInches() const {
		return (static_cast<double>(mWidth_) / MILLIMETERS_PER_INCH);
	}
	
	double VideoMode::getPhysicalDisplaySizeMillimeters() const {
		double heightSquared = pow(static_cast<double>(mHeight_), 2.0);
		double widthSquared = pow(static_cast<double>(mWidth_), 2.0);
		return sqrt(heightSquared + widthSquared);
	}
	
	double VideoMode::getPhysicalDisplaySizeInches() const {
		double heightSquared = pow(getPhysicalHeightInches(), 2.0);
		double widthSquared = pow(getPhysicalWidthInches(), 2.0);
		return sqrt(heightSquared + widthSquared);
	}

	double VideoMode::getDPI_Height() const {
		double heightInches = getPhysicalHeightInches();
		if (heightInches == 0) {
			return 0;
		}
		else {
			return (static_cast<double>(mHeight_) / getPhysicalHeightInches());
		}	
	}

	double VideoMode::getDPI_Width() const {
		double widthInches = getPhysicalWidthInches();
		if (widthInches == 0) {
			return 0;
		}
		else {
			return (static_cast<double>(mWidth_) / getPhysicalWidthInches());
		}
	}

	double VideoMode::getDPI_WidthHeightAverage() const {
		return ((getDPI_Height() + getDPI_Width()) / 2.0 );
	}

	int VideoMode::getRefreshRate() const {
		return mRefreshRate_;
	}

	
	int VideoMode::getRedBitDepth() const {
		return mRedBits_;
	}
	
	int VideoMode::getGreenBitDepth() const {
		return mGreenBits_;
	}
	
	int VideoMode::getBlueBitDepth() const {
		return mBlueBits_;
	}


//} //namespace FSMEngineInternal


#endif //USE_OLD_VIDEOMODE_CODE