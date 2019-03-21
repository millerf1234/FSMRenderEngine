// File:           FilepathWrapper.h
// Class:          FilepathWrapper
// Namespace:      N/A
// Date:           October 11, 2018    
// Programmer:     Forrest Miller
//
// Description:    This class wraps a file-path to keep track of a FilePaths 
//                 state (file exists, file extension) and provides a basic interface.
//				   
// Backstory:      So this isn't the first Filepath wrapper class I have written and used, 
//                 though this one does happen to be the first one to take some advantage of
//                 C++17's <filesystem> library features. 
//                 Actually since the <filesystem> library released, much of this class's purpose 
//                 has left, since it's really just serving as a (probably much slower) alternative
//                 to all the functionality provided by the built-in std::filesystem::path.  
//
//
//
// Windows File-system/System-Clock Note: 
//              I copied the following post by Billy Robert O'Neal III found at
//   https://developercommunity.visualstudio.com/content/problem/251213/stdfilesystemfile-time-type-does-not-allow-easy-co.html
//
//                   "system_clock / time_t are inappropriate for storing file times, as they
//                    can represent neither the range, nor the resolution, of timestamps you
//                    can get from the file system. Windows stores file times using the FILETIME
//                    structure, which is a 64 bit value of 100ns intervals from January 1, 1601.
//                    What's worse is that this 1601 date is fairly common to see given that it's
//                    the all zeros value, and it is far before the earliest date representable with
//                    time_t. std::filesystem can't change the reality of the underlying platform. On
//                    POSIX systems, time_t is similarly insufficient, as their filesystem APIs return
//                    struct timespec instances which can represent nanos (though at least the seconds
//                    part of the timespec can be used as a time_t discarding some information)."
//
//                    Moral of the story:  Until C++20, it is not possible (or advisable) to convert between
//                                         std::filesystem::file_time_type and time_t. 
//                    See also: https://en.cppreference.com/w/cpp/filesystem/file_time_type
//


#pragma once

#ifndef FILEPATH_WRAPPER_H_
#define FILEPATH_WRAPPER_H_

//If <string_view> is not available, then simply uncomment the following macro
#define FILEPATH_WRAPPER_NO_STRING_VIEW_USEAGE 1

//If <filesystem> is not available, then simple uncomment the following macro 
#define FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE 1

//Must have <optional> to use this type.

#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>     //for tracking  'last_write_time'
#include <optional>  //To protect against uninitialized FileWriteTime
#ifndef FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE
#include <filesystem> 
#endif //FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE
#ifndef FILEPATH_WRAPPER_NO_STRING_VIEW_USEAGE 
#include <string_view>
#endif //FILEPATH_WRAPPER_NO_STRING_VIEW_USEAGE


static inline constexpr int NOT_FOUND = -1;

class FilepathWrapper final {
public:
    FilepathWrapper() = delete;
    explicit FilepathWrapper(const char * fp);
    explicit FilepathWrapper(const std::string& fp);

#ifndef FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE
    explicit FilepathWrapper(std::filesystem::path fp) : FilepathWrapper((fp.make_preferred()).string()) { ; }
#endif //FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE
#ifndef FILEPATH_WRAPPER_NO_STRING_VIEW_USEAGE
    explicit FilepathWrapper(const std::string_view fp) : FilepathWrapper(fp.data()) { ; }
#endif //FILEPATH_WRAPPER_NO_STRING_VIEW_USEAGE

    FilepathWrapper(const FilepathWrapper&);
    FilepathWrapper(FilepathWrapper&&) noexcept;
    ~FilepathWrapper() = default;

    FilepathWrapper& operator=(const FilepathWrapper&);
    FilepathWrapper& operator=(FilepathWrapper&&) noexcept;

    //Returns the file path as a std::string
    std::string filepath() const { return mPath_; }

    //Checks to see if the file at the file-path exists
    bool exists() const { return mFileExists_; }

    //Checks to see if a file extension was detected in the file path
    bool extensionExists() const { return mExtensionExists_; }

    //Returns the extension of the stored file path. The extension is defined as 
    //all of the characters occurring after the last '.' character in the 
    //string, not including the '.' itself. If no '.' character is present,
    //an empty string will be returned
    std::string extension() const;

    //Queries this object to determine if it was able to record the time of last modification
    //to its wrapped file. This is necessary because it is not guaranteed that all operating systems
    //will play nicely with having the age of all of their files queried and may fail to return a valid
    //value. The function hasUpdatedFileAvailable() will always return false if this function returns false.
    inline bool isAbleToDiscoverUpdatesToFile() const { return bool(mLastWriteTime_); } //Explicitly call operator bool on std::optional mLastWriteTime_

    //Attempts to query the file located at this object's stored file-path to see if there 
    //is an updated version of the file available. Will only return true if a newer version
    //of the file is detected, otherwise will always return false. This function will update 
    //this object's local copy of the file's most recent time of modification, so calling this
    //function more than once will result in it returning false for all calls past the first call
    //until the file gets modified again.
    bool hasUpdatedFileAvailable() noexcept;

    /////////////////////////////////
    //   Static Utility Functions  //
    /////////////////////////////////

    //Returns empty string if no file extension is found
    static std::string findAndExtractFileExtension(const std::string& fp);

    //Checks to see if the file string corresponds to an actual file on the system
    static bool file_exists(const char * fp);


    /////////////////////////////////
    //   C++17 Functions  //
    /////////////////////////////////
#ifndef FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE
    //Does not check to see if this object's path is valid, simple converts this object's
    //internal path into a std::filesystem::path
    explicit operator std::filesystem::path() const {
        std::filesystem::path thePath{ mPath_ };
        return thePath;
    }
    //Checks with the operating system to determine the system time when a file was most 
    //recently updated. Will print out an error message if the OS encounters a problem 
    //querying the file and return false, in which case DO NOT USE THE file_time_type variable!
    static bool getTimeOfFilesMostRecentUpdate(const std::string& fp, std::filesystem::file_time_type& lastUpdateTime);
#else 
    inline explicit operator void() const { ; }
    //No support for the <filesystem> library on this platform. Function will always return 
    //false
    static bool constexpr getTimeOfFilesMostRecentUpdate(const std::string& fp, void */*std::filesystem::file_time_type&*/ lastUpdateTime) {
        return false;
    }
#endif  //FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE





private:
    std::string mPath_;
    bool mFileExists_;
    std::string mExtension_; //The ".xxxx" at the end of a filepath (does not necessarily exist for all filepaths)
    bool mExtensionExists_;

#ifndef FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE
    //bool mHasLastWriteTimeAvailableForComparison_; //boolean flag to ensure that mLastWriteTime_ has an accurate value.
    std::optional<std::filesystem::file_time_type> mLastWriteTime_; //Records the system time when the file was last updated. Can be used to 
    //                                               //tell if a file has an updated version compared with the currently loaded version.
#endif //FILEPATH_WRAPPER_NO_STD_FILESYSTEM_USAGE

    /////////////////////////////////
    //   Private Helper Functions  //
    /////////////////////////////////
    //void initialize();  //Delegating constructors are now used instead


    //Returns -1 if no period is found in the path
    static int getIndexOfLastPeriodInString(const std::string&);

    };


#endif //FILEPATH_WRAPPER_H_


