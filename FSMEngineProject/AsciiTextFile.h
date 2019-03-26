// File:                          AsciiTextFile.h
// Class:                         AsciiTextFile
// QuickInfo:   
//       Default Constructor      DISABLED  [Filepath is implemented as an invariant]
//       Copy Operations          ENABLED   [Might not be very efficient for large files]
//       Move Operations          ENABLED   [Recommended method for transporting these objects]
//
//
//  Description:                 Wrapper file for an Ascii Text File. Useful for 
//                               Acquiring and parsing the contained Ascii data.
//
//  Class Pre-Condition:         This class requires a std::filesystem::path or string-type equivalent to 
//                               be provided at construction. If provided with an invalid path, the 
//                               constructor will throw an exception. (See Warning Below)
//
//  Dependencies:                Depends on the C++17 library <filesystem> for handling
//                               file acquisition and tracking.
//                               Also depends on a third-party library that implements a custom 
//                               type called str_view which behaves very similar to C++17's 
//                               <string_view> with the difference being str_view guarantees to 
//                               always be terminated with a '\0' [see references]. 
//                               
//
//  Object Behavior Note:       Consider the following scenario: 
//                                An object of this type is created using a filepath for a valid
//                                file (i.e. the constructor returns without throwing an exception) but 
//                                then during this object's lifetime the file is somehow moved/deleted to
//                                where the filepath is no longer valid. If this event occurs then the
//                                behavior of any subsequent calls to any of this objects copy/move 
//                                constructors/operations (which are declared as 'noexcept') will behave as
//                                though the file still exists and will result in the copying/moving of all 
//                                object-local data rather then cause std::terminate to get called.
//                               
//                               
//  References:                - How to best parse a large ASCII file (circa 2011)
//                                 http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
//                                 
//
//                             - Guidance on using C++17's string_view within interface
//                                 https://softwareengineering.stackexchange.com/questions/364093/when-should-i-use-string-view-in-an-interface
//                                  [From what I have read now on several sights about where to use 
//                                   std::sting_view] Basically, as a rule of thumb, std::string_view 
//                                    should only be used as a function parameter. 
//                                    [I plan to experiment around with this to see if this really is the case...]
//
//                             - Custom String_view class "str_view.hpp" github page:
//                                 https://github.com/sawickiap/str_view
//
//
//              Forrest Miller
//              March 2019


#pragma once

#ifndef ASCII_TEXT_FILE_H_
#define ASCII_TEXT_FILE_H_

//#include <iostream>
//#include <fstream>
#include <filesystem>
#include <string>
#include "str_view.hpp"//Custom Version which guarantees each view is terminated with '\0' //#include <string_view>
//#include <algorithm>
//#include <vector>
//#include <stdexcept>
//#include <system_error>



// +--------------------------------------------------------------------------------------------------------+    
// |   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   EXCEPTION HAZARD   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   |    
// +--------------------------------------------------------------------------------------------------------+    
// |         THE CONSTRUCTORS FOR THIS TYPE CAN AND WILL THROW EXCEPTIONS! EXCEPTIONS WILL BE THROWN        |    
// |            IF ANY ISSUE OCCURS DURING CONSTRUCTION, WHICH INCLUDES BEING CONSTRUCTED WITH A            |    
// |        PATH TO A NON-EXISTANT FILE, A NON-ASCII FILE OR BEING UNABLE TO READ ANY PART OF THE FILE!     |    
// |                                                                                                        |   
// |  Likely Exceptions To Be Thrown Include:                                                               |     
// |                                                                                                        |      
// |              std::invalid_argument                      [For Non-Existent File Path]                   |      
// |               std::system_error              [If OS Reports an Error through the filesystem library]   |      
// |                                                                                                        |  
// |                                                                                                        |
// |    LUCKILY, EXCEPTIONS ARE ONLY THROWN BY CONSTRUCTORS. SO ONCE                                        |
// |    THE OBJECT IS CONSTRUCTED, ALL FURTHER USAGE IS GUARENTEED TO NEVER THROW AN EXCEPTION              |
// +--------------------------------------------------------------------------------------------------------+

class AsciiTextFile final {
public:
    AsciiTextFile() = delete;
    //-------------------------------------------------------
    //                      Constructors
    //-------------------------------------------------------

    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(const char* path) noexcept(false);

    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(const std::string& path) noexcept(false);       //Prefer std::string_view constructor

    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(std::string_view path) noexcept(false);
    
    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(std::filesystem::path path) noexcept(false);

    //-------------------------------------------------------
    //                      Destructor
    //-------------------------------------------------------
    
    ~AsciiTextFile() noexcept;

    //-------------------------------------------------------
    //                      Copy/Move
    //-------------------------------------------------------

    AsciiTextFile(const AsciiTextFile& that) noexcept;
    AsciiTextFile(AsciiTextFile&& that) noexcept;
    AsciiTextFile& operator=(const AsciiTextFile& that) noexcept;
    AsciiTextFile& operator=(AsciiTextFile&& that) noexcept;

    
    //-------------------------------------------------------
    //                      Accessors
    //-------------------------------------------------------

    //Returns this object's filepath
    const std::filesystem::path& path() const noexcept;
    
    //Returns a guaranteed Null-Terminated str_view to this object's
    //internally stored text.
    //Please be aware that this returned str_view does not own any of 
    //the string data, so destroying this object then attempting to 
    //use that returned view is UNDEFINED BEHAVIOR!
    //So, like, ya know, don't be careless with this function and all 
    //will be well...
    str_view getViewOfFullFileText() const noexcept;

    //Creates a copy of this object's internally stored text and returns it as a
    //string. Please be advised that this operation may be fairly expensive
    //depending on the stored file size. 
    std::string getFullCopyOfFileText() const noexcept;

    //Counts the number of lines of text total in the file. Lines are delineated
    //by newline feed characters '\n'. An completely empty file will return 0, and 
    //a file with some ASCII characters but no newlines will return 1.
    size_t countNumberOfLines() const noexcept;

    //Returns the number of lines that begin with the specified character.
    //When determining the start of a line, both tabs and spaces will be 
    //skipped over (unless the parameter c is itself a space or tab). 
    size_t countNumberOfLinesThatBeginWith(char c) const noexcept;

    //Adds onto the back of the vector provided as parameter the number of each line
    //that begins with the specified character, ignoring spaces and tabs at the start. 
    void getLinesThatBeginWithCharacter(char c, std::vector<size_t>& lines) const noexcept;

    //Retrieves the number of characters on the specified line. If the parameter
    //'line' does not correspond to a valid line number, this function returns 0.
    //Note that a newline character at the end of the line will be included as 
    //part of the line length.
    //Lines start at line 0 at the top of the file.
    size_t getLineLength(size_t line) const noexcept;

    //Returns a str_view for a requested line number. Please be aware that
    //the returned view from this function does not own the data it is viewing.
    //Requesting a line that is beyond the end of the file will result in a
    //str_view containing only the '\0' character.
    str_view getViewOfLine(size_t line) const noexcept;

    //Retrieves the line of text at the specified line. Line indexing starts at 0.
    //Straying beyond the end of the file's bounds will return an empty string.
    std::string getCopyOfLine(size_t line) const noexcept;

    //Returns the size of the file in [INSERT_UNITS_HERE_ONCE_I_CAN_FIGURE_THIS_OUT] 
    // (it will be in bits/bytes)
    size_t getFileSize() const noexcept;




    //-------------------------------------------------------
    //                  Utility Function(s)
    //-------------------------------------------------------

    /*   No Utility Functions Are In Existence Currently   */

private:
    //This serves as the delegating constructor which allows
    //for all of the other public constructors to know the object
    //exists as a complete type so they can avoid having to deal with
    //any potentially thorny issues which can occur when throwing exceptions
    //from not-fully-initialized objects.
    explicit AsciiTextFile(str_view path); 

    //PIMPL
    class AsciiTextFileImpl;
    std::unique_ptr<AsciiTextFileImpl> pAsciiTextFileImpl_;
};

#endif //ASCII_TEXT_FILE_H_
