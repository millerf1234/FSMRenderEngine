//  File:            EasyLogConfiguration.h
//
//  Description:     Set's up the EasyLogging++ third party logging utility.
//                   This file should be included only once in the entire project,
//                   preferably early in the program's launch.
//
//  IMPORTANT:       This file should only be included once in the entire solution.
//                   TO be safe, only include this file in a single '.cpp' file in the rest of the project.
//                   Having multiple instances of this file will be bad.
//
//
//  See also:        EasyLogging++ has additional settings that are specified using macros which
//                   must be defined inside the header file "easylogging++.h". There should
//                   be a section near the top of that file where these are set.
//
//
//  Reference:       https://github.com/zuhd-org/easyloggingpp/blob/master/README.md#quick-start
//
//  Programmer:      Forrest Miller
//  Date:            January 10, 2019
//

#pragma once

#ifndef EASY_LOG_CONFIGURATION_H_
#define EASY_LOG_CONFIGURATION_H_

#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <optional>

#include "ThirdParty/easyloggingpp/include/easylogging++.h" 

INITIALIZE_EASYLOGGINGPP


namespace EASYLOGPP_CONFIGURATION_INTERNAL {         //Function prototypes for some implementation functions
    bool checkIfAlreadyConfigured();
    std::optional<std::filesystem::path> getFilepathToLogForLevel(el::Level); //Call with 'Global' level to just set up a directory for logs
    std::optional<std::filesystem::path> getLogFileDirectory(); 
} 

void initializeEasyLogger(int argc, char ** argv) {
    START_EASYLOGGINGPP(argc, argv);
}

//Not recommended (or expected) for this function to be called more than once
void configureEasyLogger() {
    if (EASYLOGPP_CONFIGURATION_INTERNAL::checkIfAlreadyConfigured()) {
        LOG(WARNING) << "Unable to configure logs! Logs have already been configured!\n";
        return;
    }
    
    
    //This is a bit complicated, so I first wrote out a bunch of notes on how it all works
   
    //  +==========================================================================================+
    //  |    When the logging library is first set up, we are provided with 2 'logger' objects     |
    //  +==========================================================================================+
    //        These loggers are:
    //           -)  Default logger (ID: default)                  //This logger will be called every time through the macro  'LOG(level)'
    //           -)  Performance logger (ID: performance)          //This logger must be targeted using the macro 'CLOG(level, ID)'
    // 
    //        It should be noted that it is possible at any time* to create a new logger, all that is needed is to                                                                      *Well, almost any time...
    //         make a log call like so:                                                                                                                                                    It turns out this won't work if a logger with that name
    //                   CLOG(level, "nameOfNewLogger") << "string stuff";                //This will create a new logger called "nameOfNewLogger"                                          had already been created and then deleted.
    //                                         
    //
    // Logging messages can be sent to the following 'Level's  (I would call them 'Targets' in my previous projects)
    // +====================+==========================================================================================================================================================================+
    // |       Level        |     Description                                                                                                                                                          |
    // +====================+==========================================================================================================================================================================+                                                      |
    // |       Trace        |   Information that can be useful to back - trace certain events - mostly useful than debug logs.                                                                         |
    // |       Debug        |   Informational events most useful for developers to debug application. Only applicable if NDEBUG is not defined(for non - VC++) or _DEBUG is defined(for VC++).          |
    // |       Fatal        |   Very severe error event that will presumably lead the application to abort.                                                                                            |
    // |       Error        |   Error information but will continue application to keep running.                                                                                                       |
    // |      Warning       |   Information representing errors in application but application will keep running.                                                                                      |
    // |       Info    	    |   Mainly useful to represent current progress of application.                                                                                                            |
    // | Verbose [DISABLED] | [DISABLED] Information that can be highly useful and vary with verbose logging level. Verbose logging is not applicable to hierarchical logging.       [DISABLED]        |
    // |      Unknown       |   Only applicable to hierarchical logging and is used to turn off logging completely.                                                                                    |
    // +====================+==========================================================================================================================================================================+
    //
    //   Each of the above level's can have a unique format assigned to their output. It is also possible to assign output format
    //     to them all at once by configuring the configuration object and then assigning it to the level target:
    // +~~~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
    // |      Global        |    Generic level that represents all levels. Useful when setting global configuration for all levels.    |         
    // +~~~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
    //
    //Please be aware that this entire project is using hierarchical logging, which will cause the Verbose 'Level' to be disabled


    //Loggers have their behavior determined by state-representing objects called el::Configurations.
    //el:Configurations can have the following configuration values specified to determine logging behavior:
    //       
    //      ++-----------------------------------------------------------------------------------++
    //      ++---------------------------------+-------------------------------------------------++
    //      ||            (enum)               |                                                 ||
    //      ||  el::ConfigurationType::Format  |                   Effect                        ||
    //      ||                                 |                                                 ||
    //      ++---------------------------------+-------------------------------------------------++
    //      ||                                 |                                                 ||
    //      ||            Enabled              |            To log or not to log?                ||
    //      ||                                 |                                                 ||
    //      ||            ToFile               |               To log to file?                   ||
    //      ||                                 |                                                 ||
    //      ||        ToStandardOutput         |  i.e. to log to screen/console/terminal etc...  ||
    //      ||                                 |                                                 ||
    //      ||            Format               |     Determines the format of log messages       ||
    //      ||                                 |                                                 ||
    //      ||           Filename              |  Sets where to write log files for this level --||-----> Be aware! Full File Path expected 
    //      ||                                 |                                                 ||
    //      ||      SubsecondPrecision         |  Specifies the digits of subsecond precision----||-----> limited to integers in range:  1 <= Precision <= 6        
    //      ||                                 |                                                 ||
    //      ||        MaxLogFileSize           |                    Duh                          ||
    //      ||                                 |                                                 ||
    //      ||       LogFlushThreshold         |   log events to store before flushing buffer    ||
    //      ||                                 |                                                 ||
    //      ++________________________________ +_________________________________________________++
    //     ```````````````````````````````````  ````````````````````````````````````````````````
    //
    //     Note that these don't cover the complete set of el::ConfigurationType::Format enums. 
    
       
    //--------------------------------------------------------------------------------------------------------//
    //   At this point, it is time to configure what we want output for each logging level to look like.      //  
    //--------------------------------------------------------------------------------------------------------//
     
    ////////////////////
    ////       Step 0       Set up a location in the filesystem for LOGs to go
    //////////////////
    std::error_code ec {}; 
    auto fp = std::filesystem::current_path(ec);
    if (ec) { 
        LOG(WARNING) << "\nSomething went wrong!\n"
                     << "Unable to communicate with Filesystem to set up proper LOG file locations!\n"
                     << "Filesystem reported error:\n\t\"" << ec.message() << "\"\n\n";
        LOG(WARNING) << "All logging messages will be redirected to default LOG file with default formatting...\n\n";
        return;
    } 
    
    auto possibleFileForLog = EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Global);

    if (possibleFileForLog->empty()) {
        LOG(WARNING) << "\nAn issue was encountered setting up a directory for LOG files.\n"
                     << "All LOG output will be directed to the default file!\n\n";
        return;
    }


    //Otherwise if we haven't returned yet, then we have a ready-to-go directory to place log files.
    //This also means we don't need to check the std::optional returned by 'getFilepathToLogForLevel()' for emptiness from
    //here on since we have confirmed we have a directory ready to go

    ////////////////////
    ////        Step 1      CREATE CONFIGURATION OBJECT 
    /////////////////

    el::Configurations logConfigurator;
    logConfigurator.setToDefault();  //First we set everything to default  
    
    /// /////////////TEST DELETE ME LATER AT SOME POINT. TEST DEBUG LOG BEHAVIOR WHEN BUILDING FOR RELEASE////////
    LOG(DEBUG) << "\nWill This print?\n\n";
    LOG(TRACE) << "Will This print?\n";
        return;
    /// .................................................................................................////////////////
        

    ///NOTE on how to specify datetime format:   FORMAT               = "%datetime{%d/%M} %func %msg"

    ////////////////////
    ////        Step 2      SET CONFIGURATION FOR EACH LEVEL 
    /////////////////             

    //Configure Trace
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput, "false");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Format, "");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Trace)->string());
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::LogFlushThreshold, "true");
  
    //Configure Debug  
    //TODO check to see if NDEBUG or _DEBUG is defined or else this log won't exist
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToStandardOutput, "true");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Format, "true");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Filename,
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Debug)->string());
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::LogFlushThreshold, "true");

    //Configure Fatal
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::ToStandardOutput, "true");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Format, "true");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Filename,
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Fatal)->string());
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::LogFlushThreshold, "true");

    //Configure Error
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput, "true");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Format, "true");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Error)->string());
    logConfigurator.set(el::Level::Error, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::LogFlushThreshold, "true");

    //Configure Warning
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::ToStandardOutput, "true");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Format, "true");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Warning)->string());
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::LogFlushThreshold, "true");

    //Configure Info
    logConfigurator.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::ToStandardOutput, "true");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::Format, "true");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Info)->string());
    logConfigurator.set(el::Level::Info, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::LogFlushThreshold, "true");

    //Configure Verbose  [Verbose is Disabled]
    logConfigurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "false");

    //Configure Unknown
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Enabled, "true");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::ToFile, "true");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::ToStandardOutput, "true");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Format, "true");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Unknown)->string());
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::SubsecondPrecision, "true");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::MaxLogFileSize, "true");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::LogFlushThreshold, "true");



    ////////////////////
    ////        Step 3      Assign Configuration to default logger object
    /////////////////          
    el::Loggers::reconfigureLogger("default", logConfigurator);

    ////////////////////
    ////        Step 4      Make this configuration the default for any future loggers that may be created
    /////////////////          
    el::Loggers::setDefaultConfigurations(logConfigurator);


}



namespace EASYLOGPP_CONFIGURATION_INTERNAL {       
    bool checkIfAlreadyConfigured() {
        static bool hasBeenConfigured = false;
        if (hasBeenConfigured) {
            return true;
        }
        else {
            hasBeenConfigured = true;
            return false;
        }
    }

    std::optional<std::filesystem::path> getFilepathToLogForLevel(el::Level level) {
        auto possiblyTheLogFileDirectory = getLogFileDirectory(); //This gives us a std::optional which may 
        if (!possiblyTheLogFileDirectory.has_value()) {           //have the filepath for where to create log files
            return std::nullopt;  //If we weren't about to get a filepath, return empty std::optional
        }
        else {
            if (level == el::Level::Global) { //If called with parameter 'Global', then just return the directory 
                return possiblyTheLogFileDirectory;
            }
            else {
                std::filesystem::path logFilepath = (*possiblyTheLogFileDirectory).string() + "/";
                logFilepath = logFilepath.make_preferred(); //Sets all of the '\' and '/' to the OS-preferred style
                switch (level) {
                case el::Level::Info:
                    logFilepath += "Info.log";
                    break;
                case el::Level::Debug:
                    logFilepath += "Debug.log";
                    break;
                case el::Level::Error:
                    logFilepath += "Errors.log";
                    break;
                case el::Level::Fatal:
                    logFilepath += "Fatal.log";
                    break;
                case el::Level::Trace:
                    logFilepath += "Trace.log";
                    break;
                case el::Level::Warning:
                    logFilepath += "Warnings.log";
                    break;
                case el::Level::Unknown:
                    [[fallthrough]];
                default:
                    logFilepath += "Default.log";
                    break;
                }
                return std::make_optional<std::filesystem::path>(logFilepath);
            }
        }
    }


    //I apologize in advance for how confusing this function's implementation is
    std::optional<std::filesystem::path> getLogFileDirectory() {
        static std::filesystem::path logDirectory {}; //Static filepath will be initialized first time this function is called.
        //                                         //This filepath will then remain constant throughout the lifetime of the program

        if (!(logDirectory.empty())) { //If there is already a directory for log files created
            return std::make_optional<std::filesystem::path>(logDirectory); 
        }
        else { //else there hasn't been a filepath set yet for log files to be placed
            //create one
            std::error_code ec; 
            ec.clear();         
            logDirectory = std::filesystem::current_path(ec);
            if (ec) {
                LOG(ERROR) << "Unable to communicate with filesystem to set up directory for log files!\n"
                    << "The OS reports error: " << ec.message() << "\n\n";
                return std::nullopt;
            }
            else { //we're in business
                logDirectory = logDirectory.lexically_normal();
                logDirectory = logDirectory.string() + "/LOGS/";
                logDirectory = logDirectory.make_preferred(); //Ensures each '/' and '\' in filepath are all set to OS-preferred separator 
                //LOG(INFO) << "\nCurrent working directory is: " << logDirectory.string() << "\n"; //Print it out to ensure sanity

                //Now it is time to append a unique identifier onto the end of it. Date and time will be unique (hopefully)
             
                std::stringstream tag; //Tag is to be built into stringstream
                auto timetag = std::chrono::system_clock::now(); //This is the system clock. However to get it into a format that will make a nice filepath tag,
                time_t timetag2 = (std::chrono::system_clock::to_time_t(timetag)); //we have to do some crazy conversions from all these different time representations.
                                                                   
                /////////////////////
                //
                //    HACK   Use of deprecated function
                //
                //MSVC considers the C function 'localtime' to be deprecated since it is not thread safe and (i think) can cause buffer overflow. It is intended to be 
                //replaced with 'localtime_s()' on windows or localtime_r() on POSIX-compliant. To use localtime_s though there needs to be some macros defined, which
                //are:
                //    check to make sure ' __STDC_LIB_EXT1__ ' is defined be the implementation
                //    define ' __STDC_WANT_LIB_EXT1__ '  before the header 'time.h' is included.
                //https://en.cppreference.com/w/c/chrono/localtime
                //https://docs.microsoft.com/en-us/cpp/preprocessor/warning?view=vs-2017
                //
                // OR  just do this hack until C++20 provides a much easier way to do this
                //
#pragma warning( push )
#pragma warning( disable : 4996 ) 
                auto timetagC = std::localtime(&timetag2); //I'm not really sure what is going on but it works
#pragma warning ( pop )
                //End of hack
                /////////////////////

                //  Reference: https://en.cppreference.com/w/cpp/io/manip/put_time
                //Now we can extract the information we need
                tag << std::put_time(timetagC, "%Y_%j_%H_%M_%S"); //Puts the 'day of the year', 'year', 'hour', 'minute', and 'second' into a stringstream
                
                std::string timetagString = tag.str();
                ///LOG(INFO) << "\nTimetag string is: " << timetagString;
                //Remove any spaces that may have sneaked their way into the path
                timetagString.erase(remove(timetagString.begin(), timetagString.end(), ' '), timetagString.end());
                ///LOG(INFO) << "\nTimetag string after removing spaces is: " << timetagString;

                //Finally we can build the filepath
                logDirectory = logDirectory.string() + timetagString;

                //Create the new directory
                if (!std::filesystem::create_directories(logDirectory)) {
                    LOG(WARNING) << "\nAn error occurred while making a directory for logging files!\n";
                    return std::nullopt;
                }
                else {  //YAY we did it!
                    ///LOG(INFO) << "\nLog files for this program will be stored in folder:\n\t" << logDirectory.string();
                    return std::make_optional<std::filesystem::path>(logDirectory);
                }
            }
        }
    }
}






#endif //EASY_LOG_CONFIGURATION_H_

