//
//  File:            EasyLogConfiguration.h
//
//  Please Notice:   This file contains several specific macro-function calls required by the library Easylogging++
//                       as part of its initalization which MUST only appear in one single location within a project. 
//
//                   Due to the restrictions these specialized macros place on the possible locations this header may  
//                         be incorperated into an Application, several additional preventative safety-measures have been
//                         implemented throughout this file in order to detect unintended/improper inclusion of this header 
//                         within the build system which will automatically block compilation as soon as a requirement is broken 
//                         or fails to be satisfied. 
//                         These security features include:
//
//                   As a result of the specialized requirements imposed upon this file with regard to its unique
//                         restrictions which accompany its manner of integration into a project, it is 
//                         recommended as essential by the author that all of the following documentation be  
//                         read and understood before attempting to perform any refactoring or changes to the 
//                         code.     
//                                    [ I intend to do my part in this bargin by not getting too rambly, but know that 
//                                      currently this has not been known as an area of strength for me... ]
//
//
//  Description:    The purpose of the code contained within this file is to provide the implementaion
//                     for an Application built with the FSMEngine to initialize and configure all of 
//                     its specific requirements and specifications for usage of the EasyLogging++ library. The functions defined witin this 
//                     file provides to the EasyLogging++ library all the necessary configuration details for every one of the logs up and 
//                     running. The intent is for these tasks to be performed as part of the startup
//                     and this should be one of the first thing that happens as part of the launch and 
//                     initialization of this application.
//
//  
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//      
//           IMPORTANT:    THIS FILE DOES NOT PROVIDE ACCESS TO ANY LOGGING FUNCTIONALITY AND MUST NOT BE INCLUDED IN OTHER FILES!
//                    ------>  TO GAIN ACCESS TO THE LOGGING FUNCTIONS, PLEASE INCLUDE THE SHARED HEADER "UniversalIncludes.h"  <--------
//          +--------------------+                    
//    HOW   | Enabling Logging   |   The project should be set up so that any file can gain access to all of 
//     TO   | Functionality For  |      the logging functionality for code in the other source/header files, the functionality should be 
//          |Use In Another File:|     enabled as part of the commonly used header file "UniversalIncludes.h" 
//          +--------------------+         
//                                 
//                       [ TROUBLESHOOTING: If logging functionality is not found within "UniversalIncludes.h", try searching
//                                             for an internal header titled "elcc_includer.h" which should be burried somewhere 
//                                             within this project's files. This other header is meant to be the internal header                                    
//                                             that imports all of the logging functions ]  
//   Remember
//  IMPORTANT:       This file must only be included in one location within the rest of the entire codebase. 
//
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//      
//
//
//
//
//
//                  +====================================================================================+
//                  |                                      USAGE                                         |
//                  +====================================================================================+
// Guidelines for
//  Including In
//     Project:     Because of the strict rules limiting the number of times this file is allowed to
//                     be included by any of the other code in a project, as a safety feature it is 
//                     required of the file designated for including this file to define the 
//                     the macro "ALLOW_ELCC_CONFIGURATION" (minus the quotes).
//                     This file will undefine this macro once it has successfully detected it or if  
//                     it fails to find a definition for said macro will force a halt to compilation 
//                     and report an error.
#ifdef ALLOW_ELCC_CONFIGURATION
#pragma message("\n\t\t\tEasyLogConfiguration has detected proper usage of the macro ensuring \n\t\t\t" \
 "it is being included into the project in a manner which meets its strict requirements.\n" \
 "\n\t\t\t\t\t\tAllowing the compilation to proceed without hinderance\n")
#undef ALLOW_ELCC_CONFIURATION
#else 
#pragma message("ERROR! Unable to continue with compilation due to the possibly unsafe multiple-inclusions of the file EasyLogConfiguration.h \n \
 This configuration file MUST only be included at one isolated place within each easylog++-assocaited project. If it is known that\n \
 a place has been selected that meets these strict criteria, then define the macro listed in the documentation to disable this\n \
 compiler error message from preventing compilation.")
#error Illegal Multiple Inclusions of EasyLogg++ Configuration Header
#endif
//
//  How-to Invoke:  
//                  The proper usage of the code implemented in this file is accomplished through 
//                     placing two function calls in a specific order. The first function will
//                     start the initial processes associated with running the EasyLogging++ library and 
//                     has the following signature:
//
//                    (1)           void initializeEasyLogger(int argc, char ** argv);
//
//                  Please observe that this function is meant to be invoked with two parameters
//                     which match the signatures of the commandline arguments provided by the OS to 
//                     this Application's main() at launch. If this applications main() is expecting 
//                     to find some of its own specical instructions from amongst these command line
//                     arguments, it may choose to filter them out before calling this function. I may be
//                     wrong but I believe all that really is needed here is the first argument recieved,
//                     but to ensure proper initialization of EasyLogger++ it is recommended to all args 
//                     recieved. I suspect it needs these so it may recieve the filepath containing the 
//                     executable so that it is able to properly determine an appropriate relative directory
//                     for it to populate with its log files.
//                     
//
//                  After the first function has been called, all that remains is to call the configuration function:
//                      This function will make sure each log file is assigned its proper name and is set with accurate 
//                      date and time synchronized to the rest of the application so that each message is reported as 
//                      accuratly as possible. 
//                      
//                    (2)           void configureEasyLogger();
//
//                  Be advised that after this second function has returned it shall be expected that all the necessary steps
//                      have been performed with interfacing with EasyLogger++ and any logging-related function calls made anywhere
//                      by this applcaitions code will be logged appropriatly. 
//                  Seeing as the code found within this class is meant entirely for setup, there should not be any need to do anything
//                      futher with this file for the rest of the processes lifetime.
//
//
//
//   There is some additional important information regarding the implementation and interfacing with the here that     
//
//
//                  
//  +=================================================================================================================================+
//  |                                   Some Additional Details for Future Maintainers                                                |
//  +=================================================================================================================================+
//
//                                        Please read the below message 
// FINAL MATTER OF
//  IMPORTANCE      +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
//  Please Note:   (|   !!!!!  NOT ALL OF THE CONFIGURATION FOR EasyLogging++ IS DONE WITHIN THIS FILE  !!!!!   |)      
//                  +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+  
//                  | EasyLogging++ was built to be used within a wide range of projects and thus has a very    |
//                  |    high degree of customizability and flexability to its available functionality.         |
//                  |    Easylogging++ is able to allow for this flexability by both having the option to have  |
//                  |    certain features enabled or disabled based off the presence or abscence of some        |
//                  |    various macros during the compilation step while also through the settings available   |
//                  |    to be tweaked during run-time (this latter part being what the code in this file does).|
//                  | This file here is only in charge of providing configuration to the EasyLogging++ library  |
//                  |    as a part of this programs run process. If there is a need to make changes to what     |
//                  |    settings are being built into the EasyLogging++ library,  with the a file              | 
//                  |    known as 'EasyLogging_BuildConfig.h'. To make sure the macros are actually having      |
//                  |    their desired impact be sure to look into the 'easylogging++.h' header file and make   |
//                  |    sure there has been added a line which consists on an include statement for this       |
//                  |    settings file.                                                                         |
//                  +-------------------------------------------------------------------------------------------+
//
//  easylogging++ reference page:       https://github.com/zuhd-org/easyloggingpp/blob/master/README.md#quick-start
//
//  Programmer:      Forrest Miller
//  Date:            January 10, 2019
//                   February 27, 2019  --  Came back and tried to finish implementation


#ifndef EASY_LOG_CONFIGURATION_H_
#define EASY_LOG_CONFIGURATION_H_

#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <optional>
#include <ctime>

#include "ThirdParty/easyloggingpp/include/easylogging++.h" 
//#include "FSMEngine_GlobalConfigurationMacrosForEasyLoggingPP.h"

#ifndef HAVE_ALREADY_INITIALIZED_EASYLOGGINGPP
#define HAVE_ALREADY_INITIALIZED_EASYLOGGINGPP INITIALIZE_EASYLOGGINGPP
#endif

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
    // +====================+==========================================================================================================================================================================+                                                      
    // |       Trace        |   Information that can be useful to back - trace certain events - mostly useful than debug logs.                                                                         |
    // |       Debug        |   Informational events most useful for developers to debug application. Only applicable if NDEBUG is not defined(for non - VC++) or _DEBUG is defined(for VC++).         |
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


    //Loggers have their behavior determined by state-representitive objects called el::Configurations.
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
        LOG(WARNING) << "All logging messages will be redirected to a single default LOG file with default formatting...\n\n";
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



#else 
//else the Header Guard test failed so this file was included multiple times within the project
//which isn't allowed because it makes use of the macro INITIALIZE_EASYLOGGINGPP 

#error Compilation Error! \
Detected that "EasyLogConfiguration.h" has been included in more than one place within the project. \
Please be aware "EasyLogConfiguration.h" is not meant to be included more than once. To gain access to the the logging \
functionality please make sure to include the header "UniversalIncludes.h", which has no restrictions on how many \
places it is allowed to appear within the codebase.


#endif //EASY_LOG_CONFIGURATION_H_

