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

//Forward declaration of GLFWVidmode
struct GLFWvidmode;

class FSMVideoMode final {
public:
   
    //Construct a VideoMode from a GLFWvideomode and the display's width and height in millimeters. [The
    //width and height are able to be queried through the GLFW function 'glfwGetMonitorPhysicalSize()']
    FSMVideoMode(const GLFWvidmode& vid, int physicalWidthMM, int physicalHeightMM) noexcept;

    ~FSMVideoMode() noexcept;

    FSMVideoMode(const FSMVideoMode&);
    FSMVideoMode(FSMVideoMode&&) noexcept;
    FSMVideoMode& operator=(const FSMVideoMode&);
    FSMVideoMode& operator=(FSMVideoMode&&) noexcept;

    //Formats this object's data into a 2-line string meant for printing to a console or log screen.
    //This object owns the underlying string data and 
    std::string toString() const noexcept;


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
    bool operator<(const FSMVideoMode&) const noexcept;

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
    bool operator>(const FSMVideoMode&) const noexcept;


    //Compares this object with another FSMVideoMode object for equality. All
    //fields (aside from physical screen dimensions) must match exactly for equality
    bool operator==(const FSMVideoMode&) const noexcept;
    //Compares this object with a GLFWvidmode struct for equality
    bool operator==(const GLFWvidmode&) const noexcept;
    //Compares this object with another FSMVideoMode object for inequality. All
    //fields (aside from physical screen dimensions) must match exactly for equality
    bool operator!=(const FSMVideoMode&) const noexcept;
    //Compares this object with a GLFWvidmode struct for inequality
    bool operator!=(const GLFWvidmode&) const noexcept;

    ///////////////////////////////////////////////////////////
    ////////         Stored Member Data Access        /////////
    ///////////////////////////////////////////////////////////

    //Returns the monitor's width (in screen coordinates) for this FSMVideoMode.
    //Note that screen coordinates are not guaranteed to match the monitor's 
    //pixels/resolution.
    int getWidth() const noexcept;
    //Returns the monitor's height (in screen coordinates) for this FSMVideoMode.
    //Note that screen coordinates are not guaranteed to match the monitor's 
    //pixels/resolution.
    int getHeight() const noexcept;

    //Returns the monitors [approximate] physical height (in millimeters)
    int getPhysicalHeightMilliMeters() const noexcept;
    //Returns the monitors [approximate] physical width (in millimeters)
    int getPhysicalWidthMilliMeters() const noexcept;
    //Returns the monitors [approximate] physical height (in inches)
    double getPhysicalHeightInches() const noexcept;
    //Returns the monitors [approximate] physical width (in inches)
    double getPhysicalWidthInches() const noexcept;
    //Returns the monitors [approximate] physical diameter (in millimeters)
    double getPhysicalDisplaySizeMillimeters() const noexcept;
    //Returns the monitors [approximate] physical diameter (in inches)
    double getPhysicalDisplaySizeInches() const noexcept;
    //Measures the DPI (measured based off monitor's height)
    double getDPI_Height() const noexcept;
    //Measures the DPI (measured based off monitor's height)
    double getDPI_Width() const noexcept;
    //Measures the DPI (measured based off monitor's height)
    double getDPI_WidthHeightAverage() const noexcept;

    //Returns the refresh rate for this videomode
    int getRefreshRate() const noexcept;

    //Returns the red bit depth for this videomode
    int getRedBitDepth() const noexcept;
    //Returns the red bit depth for this videomode
    int getGreenBitDepth() const noexcept;
    //Returns the red bit depth for this videomode
    int getBlueBitDepth() const noexcept;

    //No default constructor
    FSMVideoMode() = delete;
    //This type requires both of the display's physical height and width in order 
    //to be a complete type. Thus a constructor which does not require physical 
    //dimensions is not able to produce a complete object, hence why it is disabled.
    FSMVideoMode(const GLFWvidmode& vid) = delete; 

private:
    class VidModeImpl;
    std::unique_ptr<VidModeImpl> pVidModeImpl_;
};

#endif //FSM_VIDEO_MODE_H_

