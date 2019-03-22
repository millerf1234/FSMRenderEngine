// File:                          AsciiTextFile.h
// Class:                         AsciiTextFile
// QuickInfo:   
//       Default Constructor      DISABLED  [Filepath is implemented as an invariant]
//       Copy Operations          ENABLED   [Might not be very efficient for large files]
//       Move Operations          ENABLED   [Recommended method for transporting these objects]
//
//
//
//  Description:                 Wrapper file for an Ascii Text File. Useful for 
//                               Acquiring and parsing the contained Ascii data.
//
//  Class Pre-Condition:         This class requires a std::filesystem::path or string-type equivalent to 
//                               be provided at construction. If provided with an invalid path, the 
//                               constructor will throw an exception.
//
//           CONSTRUCTOR CAN AND WILL THROW EXCEPTION FOR INVALID FILEPATHS!
//               Specifically, a std::invalid_argument exception will be thrown.
//
//  Dependencies:                Depends on the C++17 library <filesystem> for handling
//                               file acquisition and tracking.
//                               Also depends on the C++17 library <string_view>, but
//                               removing this dependency should prove to be trivial.
//
//              Forrest Miller
//              March 2019


#pragma once

#ifndef ASCII_TEXT_FILE_H_
#define ASCII_TEXT_FILE_H_

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <algorithm>
#include <vector>
#include <stdexcept>


class AsciiTextFile final {
public:
    
    //-------------------------------------------------------
    //                      Constructors
    //-------------------------------------------------------

    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(const char* path) noexcept(false);
    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(const std::string& path) noexcept(false);
    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(std::string_view path) noexcept(false);
    //Will throw a std::invalid_argument exception if provided an invalid filepath
    AsciiTextFile(std::filesystem::path path) noexcept(false);


   
    ~AsciiTextFile() noexcept;

private:


    //This serves as the delegating constructor which allows
    //for all of the other public constructors to know the object
    //exists as a complete types when they might throw an exception.
    AsciiTextFile(); 
};

#endif //ASCII_TEXT_FILE_H_
