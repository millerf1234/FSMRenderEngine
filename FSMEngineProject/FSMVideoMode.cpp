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
#include "VidModeImpl.h"

//Implement all of the FSMVideoMode functions by just having them call the 
//corresponding VidModeImpl versions

FSMVideoMode::FSMVideoMode(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM) noexcept {
    pVidModeImpl_ = std::make_unique<FSMVideoMode::VidModeImpl>(vid, physicalWidthMM, physicalHeightMM);
}
FSMVideoMode::~FSMVideoMode() = default;
FSMVideoMode::FSMVideoMode(const FSMVideoMode& that) {
    pVidModeImpl_ = std::make_unique<FSMVideoMode::VidModeImpl>(*(that.pVidModeImpl_));
}
FSMVideoMode::FSMVideoMode(FSMVideoMode&& that) noexcept {
    *this = std::move(that); //Call the move assignment operator
}
FSMVideoMode& FSMVideoMode::operator=(const FSMVideoMode& that) {
    if (this != &that) {
        pVidModeImpl_ = std::make_unique<FSMVideoMode::VidModeImpl>(*(that.pVidModeImpl_));
    }
    return *this;
}
FSMVideoMode& FSMVideoMode::operator=(FSMVideoMode&& that) noexcept {
    if (this != &that) {
        pVidModeImpl_ = std::move(that.pVidModeImpl_);
    }
    return *this;
}


std::string FSMVideoMode::toString() const noexcept {
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

glm::ivec3 FSMVideoMode::getRGBBitDepth() const noexcept {
    return pVidModeImpl_->getRGBBitDepth();
}

//And that is all there is to implementing the VideoMode interface!



