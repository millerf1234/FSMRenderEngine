// File:                          FSMVideoMode.h
// Class:                         FSMVideoMode
// QuickInfo:   
//       Default Constructor      DISABLED
//       Copy Operations          ENABLED
//       Move Operations          ENABLED
//
// Description:                   Provides a C++ Object Oriented encapsulation and interface for the
//                                type 'GLFWvidmode' which originates as a struct within GLFW's C API. 
//                                A GLFWvideomode is a collection of properties belonging to a monitor/display
//                                that represent the properties the display would operate under if specified to
//                                use said VideoMode. It is highly likely for a display to have multiple available
//                                videomodes to choose from.
//                      
//                                Each  VideoMode of a monitor contains the following set of properties:
//                                      - Width and Height of videomonitor measured in screen coordinates 
//                                      - Width and Height  physical size (millimeters)
//                                      - Refresh Rate  (in Hz)
//                                      - Red bit depth, Green bit depth, Blue bit depth      
//
//
// Dependencies:                  GLFW 3.0 or newer -- This class is dependent upon the GLFW defined datastructure
//                                                     type 'GLFWvideomode'            
//
// Note on Bit Depths:            GLFW 3.3 documentation mentions bit depths can be modified through hints? This 
//                                  has not been tested
//
// Programmer:                    Forrest Miller
// Date:                          December 2018    -- Original class designed and implemented
//
// Modifications:                 January 2, 2019  -- Added comparison operators '>' and '<' 
//                                March 3, 2019    -- Complete Rewrite of class to match the newer implementation style of using PIMPL idiom
//
// Fun Fact:                      This class was the first class rewritten to demonstrate this new style that [should] be 
//                                 commonly used to implement most of the FSMEngine components.


#ifndef FSM_VIDEO_MODE_H_
#define FSM_VIDEO_MODE_H_

#include <memory>
#include <string>
//#include <string_view>


//Forward declaration of GLFWVidmode
struct GLFWvidmode;

namespace VMInternal {
    class VidModeImpl;
}

class FSMVideoMode final {
public:
   
    //Construct a VideoMode from a GLFWvideomode and the display's width and height in millimeters. [The
    //width and height are able to be queried through the GLFW function 'glfwGetMonitorPhysicalSize()']
    FSMVideoMode(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM);

    ~FSMVideoMode();

    FSMVideoMode(const FSMVideoMode&);
    FSMVideoMode(FSMVideoMode&&) noexcept;
    FSMVideoMode& operator=(const FSMVideoMode&);
    FSMVideoMode& operator=(FSMVideoMode&&) noexcept;

    //Formats this object's data into a 2-line string meant for printing to a console or log screen.
    //This object owns the underlying string data and 
    std::string toString() const;


    //////////////////////////////////////////
    //////     Comparison Operators     //////
    //////////////////////////////////////////

    //Assigns an well-defined ordering to the set of all FSMVideoModes. Expression evaluation 
    //proceeds by performing the following comparisons, with each evaluation after the first
    //only being reached after the event of a tie from the previous:
    //   1) Compare Widths                         
    //   2) Compare Heights
    //   3) Compare Refresh Rates
    //   4) Compare sum of Red, Green and Blue bit depths
    //   5) Compare Green bit depths
    //   6) Compare Blue bit depths
    //   7) Compare Red bit depths
    //In the event that both of the FSMVideoModes are equivalent, 'false' will be returned 
    //by default
    bool operator<(const FSMVideoMode&) const;

    //Assigns an well-defined ordering to the set of all FSMVideoModes. Expression evaluation 
    //proceeds by performing the following comparisons, with each evaluation after the first
    //only being reached after the event of a tie from the previous comparison:
    //   1) Compare Widths                         
    //   2) Compare Heights
    //   3) Compare Refresh Rates
    //   4) Compare sum of Red, Green and Blue bit depths
    //   5) Compare Green bit depths
    //   6) Compare Blue bit depths
    //   7) Compare Red bit depths
    //In the event that both of the FSMVideoModes are equivalent, 'false' will be returned 
    //by default
    bool operator>(const FSMVideoMode&) const;


    //Compares this object with another FSMVideoMode object for equality. All
    //fields (aside from physical screen dimensions) must match exactly for equality
    bool operator==(const FSMVideoMode&) const;
    //Compares this object with a GLFWvidmode struct for equality
    bool operator==(const GLFWvidmode&) const;
    //Compares this object with another FSMVideoMode object for inequality. All
    //fields (aside from physical screen dimensions) must match exactly for equality
    bool operator!=(const FSMVideoMode&) const;
    //Compares this object with a GLFWvidmode struct for inequality
    bool operator!=(const GLFWvidmode&) const;

    ///////////////////////////////////////////////////////////
    ////////         Stored Member Data Access        /////////
    ///////////////////////////////////////////////////////////

    //Returns the monitor's width (in screen coordinates) for this FSMVideoMode.
    //Note that screen coordinates are not guarenteed to match the monitor's 
    //pixels/resolution.
    int getWidth() const;
    //Returns the monitor's height (in screen coordinates) for this FSMVideoMode.
    //Note that screen coordinates are not guarenteed to match the monitor's 
    //pixels/resolution.
    int getHeight() const;

    //Returns the monitors [approximate] physical height (in millimeters)
    int getPhysicalHeightMilliMeters() const;
    //Returns the monitors [approximate] physical width (in millimeters)
    int getPhysicalWidthMilliMeters() const;
    //Returns the monitors [approximate] physical height (in inches)
    double getPhysicalHeightInches() const;
    //Returns the monitors [approximate] physical width (in inches)
    double getPhysicalWidthInches() const;
    //Returns the monitors [approximate] physical diameter (in millimeters)
    double getPhysicalDisplaySizeMillimeters() const;
    //Returns the monitors [approximate] physical diameter (in inches)
    double getPhysicalDisplaySizeInches() const;
    //Measures the DPI (measured based off monitor's height)
    double getDPI_Height() const;
    //Measures the DPI (measured based off monitor's height)
    double getDPI_Width() const;
    //Measures the DPI (measured based off monitor's height)
    double getDPI_WidthHeightAverage() const;

    //Returns the refresh rate for this videomode
    int getRefreshRate() const;

    //Returns the red bit depth for this videomode
    int getRedBitDepth() const;
    //Returns the red bit depth for this videomode
    int getGreenBitDepth() const;
    //Returns the red bit depth for this videomode
    int getBlueBitDepth() const;

    FSMVideoMode() = delete;
    FSMVideoMode(const GLFWvidmode& vid) = delete;

private:
    std::unique_ptr<VMInternal::VidModeImpl> pVidModeImpl_;
};

#endif //FSM_VIDEO_MODE_H_





#ifdef USE_OLD_VIDEOMODE_CODE

//File:                          VideoMode.h
//
//Description:                   Built around the GLFW struct GLFWvidmode, this class provides a wrapper 
//                               for tracking the properties of a single available video mode that is
//                               supported by a connected monitor. 
//
//                               Each  VideoMode of a monitor contains the following properties:
//                                      - Width and Height in screen coordinates
//                                      - Width and Height in physical size (millimeters or inches)
//                                      - Refresh Rate   (in Hz)
//                                      - Red bit depth, Green bit depth, Blue bit depth           
//
//
//
// Note on Bit Depths:   GLFW 3.3 documentation mentions bit depths can be modified through hints? This 
//                               has not been tested
//
//Programmer:                    Forrest Miller
//Date:                          December 2018
//
//Modifications:                 January 2, 2019  -- Added comparison operators '>' and '<' 

#pragma once

#ifndef VIDEO_MODE_H_
#define VIDEO_MODE_H_

#include "LoggingMessageTargets.h"
#include "GlobalIncludes.h"       //GLFWvideomode
#include "GlobalConstants.h"   //Defines the constant used to convert millimeters to inches (for DPI calculation)

//namespace FSMEngineInternal {

	//This class is based off the GLFW defined type GLFWvidmode. It includes some extra 
	//capabilities from the basic GLFWvidmode struct.
	class VideoMode final {
	public:
		VideoMode() = delete;
		VideoMode(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM);

		~VideoMode() = default;

		VideoMode(const VideoMode&) = default;
		VideoMode(VideoMode&&) = default;
		VideoMode& operator=(const VideoMode&) = default;
		VideoMode& operator=(VideoMode&&) = default;

		//Formats this object's data into a 2-line string meant for printing to a console or log screen
		//                                          [untested, format may need editing]
		std::string toString() const;


		//////////////////////////////////////////
		//////     Comparison Operators     //////
		//////////////////////////////////////////

		//Assigns an well-defined ordering to the set of all VideoModes. Expression evaluation 
		//proceeds by performing the following comparisons, with each evaluation after the first
		//only being reached after the event of a tie from the previous:
		//   1) Compare Widths                         
		//   2) Compare Heights
		//   3) Compare Refresh Rates
		//   4) Compare sum of Red, Green and Blue bit depths
		//   5) Compare Green bit depths
		//   6) Compare Blue bit depths
		//   7) Compare Red bit depths
		//In the event that both of the VideoModes are equivalent, 'false' will be returned 
		//by default
		bool operator<(const VideoMode&) const;

		//Assigns an well-defined ordering to the set of all VideoModes. Expression evaluation 
		//proceeds by performing the following comparisons, with each evaluation after the first
		//only being reached after the event of a tie from the previous comparison:
		//   1) Compare Widths                         
		//   2) Compare Heights
		//   3) Compare Refresh Rates
		//   4) Compare sum of Red, Green and Blue bit depths
		//   5) Compare Green bit depths
		//   6) Compare Blue bit depths
		//   7) Compare Red bit depths
		//In the event that both of the VideoModes are equivalent, 'false' will be returned 
		//by default
		bool operator>(const VideoMode&) const;
		

		//Compares this object with another VideoMode object for equality. All
		//fields (aside from physical screen dimensions) must match exactly for equality
		bool operator==(const VideoMode&) const;
		//Compares this object with a GLFWvidmode struct for equality
		bool operator==(const GLFWvidmode&) const;
		//Compares this object with another VideoMode object for inequality. All
		//fields (aside from physical screen dimensions) must match exactly for equality
		bool operator!=(const VideoMode&) const;
		//Compares this object with a GLFWvidmode struct for inequality
		bool operator!=(const GLFWvidmode&) const;

		///////////////////////////////////////////////////////////
		////////         Stored Member Data Access        /////////
		///////////////////////////////////////////////////////////

		//Returns the monitor's width (in screen coordinates) for this videomode.
		//Note that screen coordinates are not guarenteed to match the monitor's 
		//pixels/resolution.
		int getWidth() const; 
		//Returns the monitor's height (in screen coordinates) for this videomode.
		//Note that screen coordinates are not guarenteed to match the monitor's 
		//pixels/resolution.
		int getHeight() const;

		//Returns the monitors [approximate] physical height (in millimeters)
		int getPhysicalHeightMilliMeters() const;
		//Returns the monitors [approximate] physical width (in millimeters)
		int getPhysicalWidthMilliMeters() const;
		//Returns the monitors [approximate] physical height (in inches)
		double getPhysicalHeightInches() const;
		//Returns the monitors [approximate] physical width (in inches)
		double getPhysicalWidthInches() const;
		//Returns the monitors [approximate] physical diameter (in millimeters)
		double getPhysicalDisplaySizeMillimeters() const;
		//Returns the monitors [approximate] physical diameter (in inches)
		double getPhysicalDisplaySizeInches() const;
		//Measures the DPI (measured based off monitor's height)
		double getDPI_Height() const;
		//Measures the DPI (measured based off monitor's height)
		double getDPI_Width() const;
		//Measures the DPI (measured based off monitor's height)
		double getDPI_WidthHeightAverage() const;

		//Returns the refresh rate for this videomode
		int getRefreshRate() const;

		//Returns the red bit depth for this videomode
		int getRedBitDepth() const;
		//Returns the red bit depth for this videomode
		int getGreenBitDepth() const;
		//Returns the red bit depth for this videomode
		int getBlueBitDepth() const;

	private:
		int mWidth_, mHeight_; //Measured in screen coordinates
		int mPhysicalWidthMM_, mPhysicalHeightMM_; //measured in millimeters, not guarenteed to be accurate.
		int mRefreshRate_;
		int mRedBits_, mGreenBits_, mBlueBits_;
	};


//} //namespace FSMEngineInternal

#endif //VIDEO_MODE_H_



#endif //USE_OLD_VIDEOMODE_CODE