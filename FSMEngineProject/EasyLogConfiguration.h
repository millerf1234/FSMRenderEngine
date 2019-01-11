//  File:            EasyLogConfiguration.h
//
//  Description:     Set's up the EasyLogging++ third party logging utility.
//                   This file should be included only once in the entire project,
//                   preferably early in the program's launch.
//
//  IMPORTANT:       This file should only be included once in the entire solution.
//                   TO be safe, only include this file in a single '.CPP' file.
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

//https://docs.microsoft.com/en-us/cpp/preprocessor/warning?view=vs-2017
#pragma warning( push )
#pragma warning( disable : 4996 )

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
    std::optional<std::filesystem::path> getFilepathToLogForLevel(el::Level);
    std::optional<std::filesystem::path> getLogFileDirectory(); 
} 

void initializeEasyLogger(int argc, char ** argv) {
    START_EASYLOGGINGPP(argc, argv);
}

//Not recommended (or expected) for this function to be called more than once
void configureEasyLogger() {

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
    // Logging messages can be sent to the following 'Level's (I would call them 'Targets' in my previous projects)
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
    //Please be aware that chances are this entire project is using hierarchical logging, which will cause the Verbose 'Level' to be disabled

    //Loggers have their behavior determined by state-representing objects called el::Configurations.
    //el:Configurations can have the following configuration values specified to determine logging behavior:
    //       
    //      ++-----------------------------------------------------------------------------------++
    //      ++`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`+_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_++
    //      ||            (enum)               |                                                 ||
    //      ||  el::ConfigurationType::Format  |                   Effect                        ||
    //      ||                                 |                                                 ||
    //      ++_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_+`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`-_-`++
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
    //   At this point, it is time to configure what we want output for each logging level to look like.      \\  
    //--------------------------------------------------------------------------------------------------------//
     
    ////////////////////
    ////       Step 0       Get the current working directory filepath 
    //////////////////
    std::error_code ec; //Make an error code
    ec.clear();         //Clear it
    auto fp = std::filesystem::current_path(ec);
    if (ec) {
        LOG(ERROR) << "Unable to communicate with filesystem to set up proper project log file targets!\n";
        return;
    } 
    else {
        LOG(INFO) << "\nCurrent Working Directory is: " << fp.c_str();
    }

    EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Trace);

    ////////////////////
    ////        Step 1      CREATE CONFIGURATION OBJECT 
    /////////////////

    el::Configurations logConfigurator;
    logConfigurator.setToDefault();  //First we set everything to default  
    

    ////////////////////
    ////        Step 2      SET CONFIGURATION FOR LEVEL:   TRACE
    /////////////////             



    ////////////////////////
    // Set Global format: //
    ////////////////////////
    //defaultConf.set(el::Level::Trace,
    //   el::ConfigurationType::Format, "%datetime");// %level %msg");
    ///
    ///
    ///
    ///defaultConf.set(el::Level::Info,
    ///    el::ConfigurationType::Format, "%datetime");// %level %msg");
   

}



namespace EASYLOGPP_CONFIGURATION_INTERNAL {       

    std::optional<std::filesystem::path> getFilepathToLogForLevel(el::Level) {
        auto possiblyTheLogFileDirectory = getLogFileDirectory(); //This gives us a std::optional which may 
        if (!possiblyTheLogFileDirectory.has_value()) {           //have the filepath for where to create log files
            return std::nullopt;  //If we weren't about to get a filepath, return empty std::optional
        }
        else {
            std::filesystem::path logFilepath = *possiblyTheLogFileDirectory;
            return std::make_optional<std::filesystem::path>(logFilepath);
        }
    }


    //I apologize in advance for how confusing this function's implementation is
    std::optional<std::filesystem::path> getLogFileDirectory() {
        static std::filesystem::path logDirectory; //Static filepath will be initialized first time this function is called.
        //                                         //This filepath will then remain constant throughout the lifetime of the program

        if (!(logDirectory.empty())) {
            return std::make_optional<std::filesystem::path>(logDirectory);
        }
        else { //else there hasn't been a filepath set yet for log files
            //create one
            std::error_code ec; 
            ec.clear();         
            logDirectory = std::filesystem::current_path(ec);
            if (ec) {
                LOG(ERROR) << "Unable to communicate with filesystem to set up directory for log files!\n"
                    << "The OS reports error: " << ec.message() << "\n";
                return std::nullopt;
            }
            else { //we're in business
                logDirectory = logDirectory.lexically_normal();
                logDirectory = logDirectory.string() + "/LOGS/";
                logDirectory = logDirectory.make_preferred(); //Make sure the path is in good form
                LOG(INFO) << "\nCurrent working directory is: " << logDirectory.string() << "\n"; //Print it out to ensure sanity

                //Now it is time to append a unique identifier onto the end of it
                std::stringstream tag; //Tag is to be built into stringstream
                auto timetag = std::chrono::system_clock::now(); //This is the system clock. However to get it into a format that will make a nice filepath tag,
                time_t timetag2 = (std::chrono::system_clock::to_time_t(timetag)); //we have to do some crazy conversions from all these different time representations.
                auto timetagC = std::localtime(&timetag2); //I'm not really sure what is going on but it works

                //  Reference: https://en.cppreference.com/w/cpp/io/manip/put_time
                //Now we can extract the information we need
                tag << std::put_time(timetagC, "%Y_%j_%H_%M_%S"); //Puts the 'day of the year', 'year', 'hour', 'minute', and 'second' into a stringstream
                
                std::string timetagString = tag.str();
                LOG(INFO) << "\nTimetag string is: " << timetagString;
                timetagString.erase(remove(timetagString.begin(), timetagString.end(), ' '), timetagString.end());
                LOG(INFO) << "\nTimetag string after removing spaces is: " << timetagString;

                //Finally we can build the filepath
                logDirectory = logDirectory.string() + timetagString;

                //Create the new directory
                if (!std::filesystem::create_directories(logDirectory)) {
                    LOG(INFO) << "\nAn error occurred making a directory for logging files to go!\n";
                    return std::nullopt;
                }
                else {  //YAY we did it!
                    LOG(INFO) << "\nLog files for this program will be stored in folder:\n\t" << logDirectory.string();
                    return std::make_optional<std::filesystem::path>(logDirectory);
                }
            }
        }
    }
}






#endif //EASY_LOG_CONFIGURATION_H_

#pragma warning ( pop )