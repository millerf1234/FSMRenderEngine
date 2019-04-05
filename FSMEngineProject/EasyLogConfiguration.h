//
//  File:            EasyLogConfiguration.h
//
//  Warning Notice:
//
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//      
//           IMPORTANT:    THIS FILE DOES NOT PROVIDE ACCESS TO ANY LOGGING FUNCTIONALITY AND MUST NOT BE INCLUDED IN OTHER FILES!
//                          TO GAIN ACCESS TO THE LOGGING FUNCTIONS, PLEASE INCLUDE THE SHARED HEADER "UniversalIncludes.h"  
//          +--------------------+ 
//          |      How To        |   The FSMEngine's expected configuration is for there to be a set of common headers which
//          | Enabling Logging   |      are responsible for implementing specific sets of related functionality. Due to the universally
//          | Functionality For  |      anticipated need for writing log messages, all of the logging message functionality can be integrated 
//          |Use In Another File:|      by simply including the header file "UniversalIncludes.h".
//          +--------------------+         
//                                 
//                       [ TROUBLESHOOTING: If for some reason it appears as though "UniversalIncludes.h" is not providing the logging
//                                            functionality, chances are that the relative filepaths between the FSMEngine's internal                                
//                                            components has been modified. A possible backup to regain logging support would be to find
//                                            out the relative filepath between the "easylogging++.h" (which is the header of the
//                                            actual library) and "UniversalInclude.h" and '#include' this relative path in "UniversalInclude.h" ]
//
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
//
//
//  Please Notice:   This file contains several specific macro-function calls which are required by the library Easylogging++
//                         as part of its initialization which MUST appear only in one single location within a project. 
//
//                   Due to the restrictions this usage of specialized macros places on the manner under which this header may 
//                         be incorporated into a project, several additional preventative safety-measures have been
//                         implemented throughout this file with the intent to detect any unintended/improper inclusions
//                         of this header within the project build system. These additional preventative measures have been
//                         designed to automatically block compilation if their prerequisites fail to be satisfied. 
//                         These security features include:
//
//                                  -) MUST DEFINE THE SPECIAL MACRO AT INTENDED LOCATION OF INCLUSION
//                                        To include this file at the proper location within the codebase, there must first be defined 
//                                        the necessary pre-processor macro ' ALLOW_ELCC_CONFIGURATION ' (no quotes) to serve as a 
//                                        clear indication that this header is being included in a properly-conforming place. This
//                                        pre-processor command once recognized will be undefined by this header. 
//                                      
//                                  -) MULTIPLE PRE-PROCESSOR STATEMENTS DESIGNED TO DETECT MULTIPLE INCLUSIONS
//                                        There are several pre-processor instructions featured throughout the document designed to
//                                        detect multiple copies of this file appearing during compilation. 
//
//                   This header contains highly specialized code which consists largely of many distinct calls made into the
//                         EasyLogging++ API that are linked together by their mutually shared responsibility for exactifying both 
//                         a highly-specific-in-behavior underlying message-logging feature-set in addition to each of the [6-7] supported
//                         message logging targets used throughout the rest of the code.
//                      
//
//  Description:     The purpose of the code contained within this file serves one of these two related purposes:
//                           i) To provide all of the necessary information and perform all of the necessary actions necessary to
//                                 fully initialize and launch the EasyLogging++ library; and
//                          ii) To perform all the necessary configuration requests* plus API calls which are required in conforming the 
//                                 highly-configurable EasyLogging++ library to meet the expected specifications of the rest of the 
//                                 Application. This includes both setting up the fundamental state in addition to configuring each 
//                                 of the available LOG targets individually. 
//
//                                                                                  *This file is responsible for only configuring the available run-time settings. 
//                                                                                     There is a separate EasyLogging++ configuration file called "EasyLogging_BuildConfig.h"
//                                                                                     that handles specifying compile-time settings.
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
//                     which match the signatures of the command line arguments provided by the OS to 
//                     this Application's main() at launch. If this applications main() is expecting 
//                     to find some of its own special instructions from amongst these command line
//                     arguments, it may choose to filter them out before calling this function. I may be
//                     wrong but I believe all that really is needed here is the first argument received,
//                     but to ensure proper initialization of EasyLogger++ it is recommended to all args 
//                     received. I suspect it needs these so it may receive the filepath containing the 
//                     executable so that it is able to properly determine an appropriate relative directory
//                     for it to populate with its log files.
//                     
//
//                  After the first function has been called, all that remains is to call the configuration function:
//                      This function will make sure each log file is assigned its proper name and is set with accurate 
//                      date and time synchronized to the rest of the application so that each message is reported as 
//                      accurately as possible. 
//                      
//                    (2)           void configureEasyLogger();
//
//                  Be advised that after this second function has returned it shall be expected that all the necessary steps
//                      have been performed with interfacing with EasyLogger++ and any logging-related function calls made anywhere
//                      by this applications code will be logged appropriately. 
//                  Seeing as the code found within this class is meant entirely for setup, there should not be any need to do anything
//                      further with this file for the rest of the processes lifetime.
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
//  [Description of Implementation]
//      The methods available for configuring EasyLogging++ exist in several different forms, from loading configuration from a file to 
//          performing individual inline configuration function calls. It should be noted that the way this code performs all of the 
//          configuration is through the usage of the provided configuration object, which contains internal variables representing the
//          state of each log 'level' (target) able to receive a configuration. To use this object, we first construct it [see
//          step 1], then we provide to this object a specific configuration for each 'level' (target) [see step 2]. Finally we must
//          tell the default collection of logs that we wish to have each of its 'levels' (targets) to follow our set configuration 
//          by passing the configuration object to it with a function call [step 3]. As a bonus option before we lose this  
//          configuration object by returning from our current function scope, we can tell EasyLogging++ that we wish for this 
//          configuration object to serve as the default settings to use for any future logging objects created (these new defaults
//          will be in place for the rest of the processes runtime) [step 4]
//
//
//                                        Please read the below message 
// FINAL MATTER OF
//  IMPORTANCE      +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
//  Please Note:   (|   !!!!!  NOT ALL OF THE CONFIGURATION FOR EasyLogging++ IS DONE WITHIN THIS FILE  !!!!!   |)      
//                  +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+  
//                  | EasyLogging++ was built to be used within a wide range of projects and thus has a very    |
//                  |    high degree of flexibility to its available functionality.                             |
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
//  Date:            January 10, 2019 --    Started implementing file, got outline and file creation in place but left configuration unfinished
//                   February 27, 2019  --  Resumed work towards finishing implementation
//                   March 24, 2019   --    Moved the very confusing and hacky date-time retrieval code to its own file


#ifndef EASY_LOG_CONFIGURATION_H_
#define EASY_LOG_CONFIGURATION_H_


#pragma message("\n\n  Testing Requirement:\n\t\tA request has been made to include the header \"EasyLogCongiuration.h\" within compilation.\n" \
"\t\t  Testing preconditions for determining if requested spot of inclusion has met the necessary requirements...")
#ifdef ALLOW_ELCC_CONFIGURATION
#pragma message("  Testing Completed:\n\t\t\"EasyLogConfiguration.h\" has detected that all proper requirements have been successfully satisfied by this location." \
/* "\n\t\t  which help ensure that this point-of-inclusion is indeed the proper place to do so. \n" */ \
 "\n\t\t  As a result, the compilation will now be resumed without hindrance...\n")
#undef ALLOW_ELCC_CONFIGURATION
#else 
#pragma message("\n  Testing Failed! [See Error Report] \n\n" \
"\n \t\t\t\t[ERROR REPORT!]\n\t\t The compiler was forced to cease progress with the current compilation due to one or more detected violations against restrictions \n"                            \
"         in place from integrated security measures for the header file \"EasyLogConfiguration.h\" which exist to prevent inadvertent access to it.\n"                                             \
"       Please be aware that this file contains several macro statements which necessitate a very specific location and manner in which this header file\n"                                         \
"         can safely and legally be included in a project."                                                                                                                                         \
"\n  Suggested Solutions:\n"                                                                                                                                                                        \
"   [RECOMMENDED]   (i) To resolve this error, simply remove each of the include statements for \"EasyLogConfiguration.h\" that caused this error to trigger\n" \
"                  (ii) If you are looking to add logging-message functions to a source file, be aware that \"EasyLogConfiguration.h\" isn't the one you want. Try instead including \"UniversalIncludes.h\"\n" \
"                 (iii) Otherwise, if you actually believe you have a proper place in your codebase that you wish to use for \"EasyLogConfiguration.h\", consult its internal comments \n"\
"                        to get an understanding of which necessary requirements and steps your code can take to deactivate the triggering of this sudden-death automatic compilation error\n\n" )

//This configuration file MUST only be included at one isolated place within each easylog++-associated project. If it is known that\n \
// a place has been selected that meets these strict criteria, then define the macro listed in the documentation to disable this\n \
// compiler error message from preventing compilation.")
#error Illegal Multiple Inclusions of EasyLogg++ Configuration Header
#endif //ALLOW_ELCC_CONFIGURATION 

#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <optional>

#include "GetSystemDateTime.h"  //Used when generating a directory name for LOG files

#include "BuildSettings.h"  //Needed to check for DEBUG log feature level

#include "ThirdParty/easyloggingpp/include/easylogging++.h" 

#ifndef HAVE_ALREADY_INITIALIZED_EASYLOGGINGPP
#define HAVE_ALREADY_INITIALIZED_EASYLOGGINGPP 1
//The following macro is what most of the fuss about only having this header included once is all about... 
//It is required to appear once exactly in order for EasyLogging++ to function properly
INITIALIZE_EASYLOGGINGPP
#endif


//                  +====================================================================================+
//                  |         Functions to Call From Application to Run Setup and Configuration          |
//                  +====================================================================================+
//Please see the comments describing these two functions above under the section titled 'USAGE'
void initializeEasyLogger(int argc, char ** argv);  //Gets EasyLogger up and running. Parameters 'argc' and 'argv' should be forwarded from command line parameters passed at launch 
bool configureEasyLogger();


//---------------------------------------------------------------------------------------------
//Optional Logger State Query Functions [Only call these after calling initializeEasyLogger()]

//Report only enabled flags
std::string reportEnabledLoggerConfigurationFlags(size_t leadingIndent = 0u) noexcept;
//Report the state of every possible flag
std::string reportAllLoggerConfigurationFlags(size_t leadingIndent = 0u) noexcept;
//--------------------------------------------------------------------------------------------


//Function prototypes for some implementation functions
namespace EASYLOGPP_CONFIGURATION_INTERNAL {         
    bool checkIfAlreadyConfigured();
    std::optional<std::filesystem::path> getFilepathToLogForLevel(el::Level); //Call with 'Global' level to just set up a directory for logs
    std::optional<std::filesystem::path> getLogFileDirectory(); 
    bool generateLogFileDirectory(std::filesystem::path&);
    std::filesystem::path makeUniquePathIfOneAlreadyExistsWithThisName(const std::filesystem::path& path);
    std::string getDateTimeTag();                   //Helper function for getting the current date and time in string form
} 



void initializeEasyLogger(int argc, char ** argv) {
    START_EASYLOGGINGPP(argc, argv);
}

//Not recommended (or expected) for this function to be called more than once
bool configureEasyLogger() {
    if (EASYLOGPP_CONFIGURATION_INTERNAL::checkIfAlreadyConfigured()) {
        LOG(WARNING) << "Unable to configure logs! Logs have already been configured!\n";
        return false;
    }
    
    
    // The features and options available in EasyLogging++ are quite extensive and as a result providing the 
    //   proper configuration gets a bit complicated. To help guide the way for the implementation I first wrote
    //   out a bunch of notes outlining a few of the various sets of options and an overview of how it all works:
    //
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


   
       
    //--------------------------------------------------------------------------------------------------------//
    //   At this point, it is time to configure what we want output for each logging level to look like.      //  
    //--------------------------------------------------------------------------------------------------------//
     
    ////////////////////
    ////       Step 0       Set up a location in the filesystem for LOGs to go
    //////////////////
    std::error_code ec {}; 
    auto fp = std::filesystem::current_path(ec);
    if (ec) { 
        LOG(WARNING) << "\nOh Dear! Something went wrong!\n"
                     << "Unable to communicate with Filesystem to set up proper LOG file locations!\n"
                     << "Filesystem reported error:\n\t\"" << ec.message() << "\"\n\n";
        LOG(WARNING) << "All logging messages will be redirected to a single default LOG file with default formatting...\n\n"; 
        return false;
    } 
    
    auto possibleFileForLog = EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Global);

    if (possibleFileForLog->empty()) {
        LOG(DEBUG) << "The function call to create a filepath for logs returned an empty optional!";
        LOG(WARNING) << "\nAn issue was encountered setting up a directory for LOG files.\n"
                     << "All LOG output will be directed to the default file!\n\n";
        return false;
    }


    //Otherwise if we haven't returned yet, then we have a ready-to-go directory to place log files.
    //This also means we don't need to check the std::optional returned by 'getFilepathToLogForLevel()' for emptiness from
    //here on since we have confirmed we have a directory ready to go

    ////////////////////
    ////        Step 1      CREATE CONFIGURATION OBJECT 
    /////////////////

    el::Configurations logConfigurator;
    logConfigurator.setToDefault();  //First we set everything to default  

    //                                              +------------------+   
    //                                              | Additional NOTES |
    //                                              +------------------+
    
    
    //  -----------------------       REPORTING DATETIME FORMAT SPECIFICATION       -----------------------
    //   How to specify what information and format to include in message for reporting DATETIME information
    //
    //           The format of the DATETIME string to report goes inside the curly-braces 
    //           following the %datetime format specifier.
    //           To set a DATETIME format to be used within the output format specification string,
    //           the following syntax is used (with 'modifiers' being replaced with the actual DATETIME
    //           modifiers):
    //                         %datetime{modifiers}
    //
    //           The total length of the modifiers used shall not exceed 30 characters.
    //           Here are the available modifiers:
    //
    //                 %d       Day of month (zero-padded)
    //                 %a       Day of the week - short(Mon, Tue, Wed, Thu, Fri, Sat, Sun)
    //                 %A       Day of the week - long(Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday)
    //                 %M       Month(zero - padded)
    //                 %b       Month - short(Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec)
    //                 %B       Month - Long(January, February, March, April, May, June, July, August, September,
    //                                          October, November, December)
    //                 %y       Year - Two digit(13, 14 etc)
    //                 %Y       Year - Four digit(2013, 2014 etc)
    //                 %h       Hour(12 - hour format)
    //                 %H       Hour(24 - hour format)
    //                 %m       Minute(zero - padded)
    //                 %s       Second(zero - padded)
    //                 %g       Subsecond part(precision is configured by ConfigurationType::SubsecondPrecision)
    //                 %F       AM / PM designation
    //                 %        Escape character
    //   
    //            For reference, here is a basic example showing a DATETIME format being specified within
    //            a complete format string:
    //
    //                                       "[%level] %datetime{%d/%M} %func %msg"
    //           
    


    // -----------------------   REPORTING THREAD ID FROM WITHIN A LOG MESSAGE   -----------------------
    // Within the format string which can be assigned to each level, it is possible to use the specifier
    // '%thread' to get the current thread ID. However, when called from the main thread, this value will
    //  be empty. Thus it is recommended to have a separate logger for use when logging from threads while 
    //  the default logger will not use this specifier
 
    
    
    //  -----------------------       FILE SIZE BYTE CONVERSION REFERENCE       -----------------------
    //  Quick cheat-sheet conversion table on MegaByte sizes to equivalent number of bytes 
    //    (handy when setting a maximum log file size such as is required for each LOG below)
    //                       [Rule: Assume 1 KiloByte is 1024 Bytes]
    //   +----------------------------------------+----------------------------------------+
    //   |     1 MB    is      1048576 Bytes      |    12 MB    is     12582912 Bytes      |
    //   |     2 MB    is      2097152 Bytes      |    16 MB    is     16777216 Bytes      |
    //   |     4 MB    is      4194304 Bytes      |    32 MB    is     33554432 Bytes      |
    //   |     5 MB    is      5242880 Bytes      |    64 MB    is     67108864 Bytes      |
    //   |     8 MB    is      8388608 Bytes      |   256 MB    is    268435456 Bytes      | 
    //   |    10 MB    is     10485760 Bytes      |     1 GB    is   1073741824 Bytes      |
    //   +----------------------------------------+----------------------------------------+

     //Loggers have their behavior determined by state-representation objects called el::Configurations.
    //el:Configurations can have the following configuration values specified to determine logging behavior:
    //       
    //      ++-------------------------------------------------------------------------------------++
    //      ++---------------------------------+---------------------------------------------------++
    //      ||            (enum)               |                                                   ||
    //      ||  el::ConfigurationType::Format  |                    Effect                         ||
    //      ||                                 |                                                   ||
    //      ++---------------------------------+---------------------------------------------------++
    //      ||                                 |                                                   ||
    //      ||            Enabled              |             To log or not to log?                 ||
    //      ||                                 |                                                   ||
    //      ||            ToFile               |                To log to file?                    ||
    //      ||                                 |                                                   ||
    //      ||        ToStandardOutput         |   i.e. to log to screen/console/terminal etc...   ||
    //      ||                                 |                                                   ||
    //      ||            Format               |      Determines the format of log messages        ||
    //      ||                                 |                                                   ||
    //      ||           Filename              |   Sets where to write log files for this level ---||-----> Be aware! Full File Path expected 
    //      ||                                 |                                                   ||
    //      ||      SubsecondPrecision         |   Specifies the digits of subsecond precision ----||-----> limited to integers in range:  1 <= Precision <= 6        
    //      ||                                 |                                                   ||
    //      ||        MaxLogFileSize           | Max Log File Size (see table of byte conversions) ||
    //      ||                                 |                                                   ||
    //      ||       LogFlushThreshold         |     Log events to store before flushing buffer    ||
    //      ||                                 |                                                   ||
    //      ++_________________________________+___________________________________________________++
    //        ````````````````````````````````` ```````````````````````````````````````````````````
    //
    //     Note that these don't cover the complete set of el::ConfigurationType::Format enums. 

    ////////////////////
    ////        Step 2      SET CONFIGURATION FOR EACH LEVEL 
    /////////////////             

    //Configure Trace
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput, "0");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Format, ""
   // "[%datetime{%Y-%M-%d|%H:%m:%s.%g}]  '%fbase'  line %line                 %func()");
    "[%datetime{%Y-%M-%d|%H:%m:%s.%g}]  '%fbase'  line %line                 [%msg]");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Trace)->string());
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::SubsecondPrecision, "6");  //expects uint
    ///logConfigurator.set(el::Level::Trace, el::ConfigurationType::MillisecondsWidth, "6"); //expects uint, [alternative name for SubsecondPrecision?]
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::MaxLogFileSize, "67108864");  //expects input type as size_t  //64 MB
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::LogFlushThreshold, "256"); //expects input type as size_t
  


    //Configure Debug  (The macro 'USE_DEBUG_' would be defined in the header "BuildSettings.h")
    //NOTE: The debug log should just report debug messages as is without including additional 
    //      information such as the file location or function name it is called from. For 
    //      information like fileName or function name report using warning or error log.
#ifndef USE_DEBUG_     
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "0");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToFile, "0");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToStandardOutput, "0");
#else 
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToStandardOutput, "1");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Format, "[%level] %msg");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Filename,
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Debug)->string());
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::SubsecondPrecision, "1");

    logConfigurator.set(el::Level::Debug, el::ConfigurationType::MaxLogFileSize, "5242880");  //5 MB
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::LogFlushThreshold, "1");
#endif  //#ifndef USE_DEBUG_



    //Configure Fatal  [Be warned that logging to FATAL may trigger the executable to abort (depending on what configuration flags were set)]
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::ToStandardOutput, "1");
#ifdef USE_DEBUG_
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Format,
        "\n\n\n[%level] at (line %line '%fbase')\n"
        "<encountered %datetime{%b %d, %Y %F %g:%m:%s}> in '%func'\nMSG=%msg\n\n");
#else 
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Format, "%msg");
#endif
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Filename,
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Fatal)->string());
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::SubsecondPrecision, "6");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::MaxLogFileSize, "2048"); //There probably won't be that many fatal messages... I hope
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::LogFlushThreshold, "1");  //Try 0?



    //Configure Error    
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput, "1");
#ifdef USE_DEBUG_
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Format, 
        "\n\n[FSMEngine %level]\n  Location: '%fbase':%line\n  Message: %msg\n\n");
#else 
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Format,
        "\n\n[FSMEngine %level]\nError Message: %msg\n\n");
#endif 
    
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Error)->string());
    logConfigurator.set(el::Level::Error, el::ConfigurationType::SubsecondPrecision, "3");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::MaxLogFileSize, "1048576"); //1048576 Bytes = 1 MB
    logConfigurator.set(el::Level::Error, el::ConfigurationType::LogFlushThreshold, "1"); 



    //Configure Warning
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::ToStandardOutput, "1");
#ifdef USE_DEBUG_
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Format, 
        "\n[%level Time = %datetime{%h:%m:%s}]  '%func'\n{line %line '%fbase'}\nMSG: %msg");
#else 
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Format, "\n\n[%level]\n%msg\n");
#endif
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Warning)->string());
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::SubsecondPrecision, "3");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::MaxLogFileSize, "1048576"); //1048576 Bytes = 1 MB
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::LogFlushThreshold, "1");



    //Configure Info
    logConfigurator.set(el::Level::Info, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::ToStandardOutput, "1");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::Format, "%msg");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Info)->string());
    logConfigurator.set(el::Level::Info, el::ConfigurationType::SubsecondPrecision, "1");
    logConfigurator.set(el::Level::Info, el::ConfigurationType::MaxLogFileSize, "5242880");  //5242880 Bytes = 5 MB
    logConfigurator.set(el::Level::Info, el::ConfigurationType::LogFlushThreshold, "8"); //Flush after every 8 logs


    //Configure Verbose             [Verbose Logging is Disabled]
    logConfigurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "0");

	

    //Configure Unknown   (Unknown is never used, it is probably safe to delete this configuration)
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::ToFile, "0");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::ToStandardOutput, "1");
	goto SKIP_LINE_THAT_CAUSES_BUG;
	//BUG 
	//For some reason this next line causes an unhandled exception to be thrown within easylogger++
	//which didn't occur in VisualStudio 2017 but now is occuring in VisualStudio 2019
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Format, "[%level] %msg");
	SKIP_LINE_THAT_CAUSES_BUG:
    //logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Filename, 
    //    EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Unknown)->string());
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::SubsecondPrecision, "3");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::MaxLogFileSize, "1048576"); //1048576 Bytes = 1 MB
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::LogFlushThreshold, "1");



    ////////////////////
    ////        Step 3      Assign Configuration to default logger object
    /////////////////          
    el::Loggers::reconfigureLogger("default", logConfigurator);

    ////////////////////
    ////        Step 4      Make this configuration the default for any future loggers that may be created
    /////////////////          
    el::Loggers::setDefaultConfigurations(logConfigurator);


    return true;
}


//The configuration flags being reported here can be found in a macro in the "EasyLogging_BuildConfig.h" file.
std::string reportEnabledLoggerConfigurationFlags(size_t leadingIndent) noexcept {
    LOG(TRACE) << __FUNCTION__;
    std::string indent;
    for (int i = 0; i < leadingIndent; i++)
        indent.append(" ");
    std::ostringstream enabledConfigFlags;
    if (el::Loggers::hasFlag(el::LoggingFlag::NewLineForContainer))
        enabledConfigFlags << indent << "NewLineForContainer\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::AllowVerboseIfModuleNotSpecified))
        enabledConfigFlags << indent << "AllowVerboseIfModuleNotSpecified\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::LogDetailedCrashReason))
        enabledConfigFlags << indent << "LogDetailedCrashReason\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog))
        enabledConfigFlags << indent << "DisableApplicationAbortOnFatalLog\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::ImmediateFlush))
        enabledConfigFlags << indent << "ImmediateFlush \n";
    if (el::Loggers::hasFlag(el::LoggingFlag::StrictLogFileSizeCheck))
        enabledConfigFlags << indent << "StrictLogFileSizeCheck\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::ColoredTerminalOutput))
        enabledConfigFlags << indent << "ColoredTerminalOutput\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::MultiLoggerSupport))
        enabledConfigFlags << indent << "MultiLoggerSupport\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::DisablePerformanceTrackingCheckpointComparison))
        enabledConfigFlags << indent << "DisablePerformanceTrackingCheckpointComparison\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::DisableVModules))
        enabledConfigFlags << indent << "DisableVModules\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::DisableVModulesExtensions))
        enabledConfigFlags << indent << "DisableVModulesExtensions\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::HierarchicalLogging))
        enabledConfigFlags << indent << "HierarchicalLogging\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::CreateLoggerAutomatically))
        enabledConfigFlags << indent << "CreateLoggerAutomatically\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::AutoSpacing))
        enabledConfigFlags << indent << "AutoSpacing \n";
    if (el::Loggers::hasFlag(el::LoggingFlag::FixedTimeFormat))
        enabledConfigFlags << indent << "FixedTimeFormat\n";
    if (el::Loggers::hasFlag(el::LoggingFlag::IgnoreSigInt))
        enabledConfigFlags << indent << "IgnoreSigInt\n";
    return enabledConfigFlags.str();
}

std::string reportAllLoggerConfigurationFlags(size_t leadingIndent) noexcept {
    LOG(TRACE) << __FUNCTION__;

    std::string indent;
    for (int i = 0; i < leadingIndent; i++)
        indent.append(" ");
    std::ostringstream configFlags;

    configFlags << "\nNewLineForContainer                               ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::NewLineForContainer);

    configFlags << "\nAllowVerboseIfModuleNotSpecified                  ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::AllowVerboseIfModuleNotSpecified);

    configFlags << "\nLogDetailedCrashReason                            ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::LogDetailedCrashReason);

    configFlags << "\nDisableApplicationAbortOnFatalLog                 ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);

    configFlags << "\nImmediateFlush                                    ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::ImmediateFlush);

    configFlags << "\nStrictLogFileSizeCheck                            ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::StrictLogFileSizeCheck);

    configFlags << "\nColoredTerminalOutput                             ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::ColoredTerminalOutput);

    configFlags << "\nDisablePerformanceTrackingCheckpointComparison    ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::DisablePerformanceTrackingCheckpointComparison);

    configFlags << "\nDisableVModules                                   ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::DisableVModules);
        
    configFlags << "\nDisableVModulesExtensions                         ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::DisableVModulesExtensions);

    configFlags << "\nHierarchicalLogging                               ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::HierarchicalLogging);

    configFlags << "\nCreateLoggerAutomatically                         ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::CreateLoggerAutomatically);

    configFlags << "\nAutoSpacing                                       ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::AutoSpacing);

    configFlags << "\nFixedTimeFormat                                   ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::FixedTimeFormat);

    configFlags << "\nIgnoreSigInt                                      ";
    configFlags << indent << el::Loggers::hasFlag(el::LoggingFlag::IgnoreSigInt);

    return configFlags.str();
}





namespace EASYLOGPP_CONFIGURATION_INTERNAL {       

    bool checkIfAlreadyConfigured() {
        static bool hasBeenConfigured = false;
        if (hasBeenConfigured)
            return true;
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
                logFilepath = logFilepath.make_preferred(); //Sets all of the '\' and '/' to the OS-preferred style (Windows is '\')
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
                    logFilepath += "Unkown.log";
                    break;
                default:
                    logFilepath += "Default.log";
                    break;
                }
                return std::make_optional<std::filesystem::path>(logFilepath);
            }
        }
    }

    std::optional<std::filesystem::path> getLogFileDirectory() {
           
        //The first time this function is called, an empty filepath to the directory in which 
        //all the LOG files will reside is created.
        static std::filesystem::path logDirectory{};
             
        //By declaring this path object as static, all of its state will remain extant after
        //this function returns. This means a unique filepath only needs to be generated once.
        //Assuming a valid path is generated during the first time this function is called, all 
        //subsequent calls merely need to check the state of this path object to see if a path
        //has already been set
               
        if (std::filesystem::exists(logDirectory)) { 
            return std::make_optional<std::filesystem::path>(logDirectory); 
        }
          
        if (generateLogFileDirectory(logDirectory)) {
            return std::make_optional<std::filesystem::path>(logDirectory);
        }
        else {
                logDirectory.clear();
                fprintf(stderr,  "\nAn error occurred while making a directory for logging files!\n");
                return std::nullopt;
            }
    }



    bool generateLogFileDirectory(std::filesystem::path& directory) {

        std::error_code ec;
        ec.clear();
        directory = std::filesystem::current_path(ec);
        if (ec) {
            fprintf(stderr, "\nError!\nUnable to communicate with filesystem to set up directory for log files!\n"
                 "The OS reports error: \n  %s\n\n",  ec.message().c_str());
            return false;
        }
        else { //we're in business
            directory = directory.lexically_normal();
            directory = directory.string() + "/LOGS/"; //All logs will exist in this directory
            directory = directory.make_preferred(); //Ensures each '/' and '\' in filepath are all set to OS-preferred separator 

            //Now it is time to append a unique-at-runtime identifier tag for a directory to hold every 
            //log that will be generated each time this program is run. Date and Time make for a good tag to use 
            std::string timetagString = getDateTimeTag();

            //Finally we can build the filepath
            directory = directory.string() + timetagString;

            //Create the new directory (there is a chance this may fail if the directory already exists)
            if (!std::filesystem::create_directories(directory, ec)) {
                if (ec) { return false; }
                //Perhaps this directory already exists for some reason. 
                //Let's try to slightly modify the filepath to make it unique...
                directory = makeUniquePathIfOneAlreadyExistsWithThisName(directory);
                if (!std::filesystem::create_directories(directory, ec)) {
                    fprintf(stderr, "\nAn error occurred while making a directory for logging files!\n");
                    return false;
                }
            }
        }
        return true;
    }
    
    
    std::filesystem::path makeUniquePathIfOneAlreadyExistsWithThisName(const std::filesystem::path& path) {
        std::string pathStr = path.string();
        std::filesystem::path uniquePath(pathStr + std::string("_1")); //Then append an identifier
        if (!std::filesystem::exists(uniquePath)) { //See if appending "_1" gave us a unique path...
            return uniquePath; //the path is unique, return our unique path
        }
        //Otherwise, our first attempt at creating the unique path failed, so let's keep trying 
        //until we can create one that is definitely unique
        int directoryCounter = 1;
        std::filesystem::directory_iterator dirIter(uniquePath);
        do {
            directoryCounter++;
            std::string tag = "_";
            tag.append(std::to_string(directoryCounter));
            uniquePath = path.string() + tag; 
        } while (std::filesystem::exists(uniquePath));
          
        ///printf("\n[LOG CONFIG DEBUG] Had to iterate through %d existing directories before\n"
        ///    "a unique name was found!\n", directoryCounter);
        ///printf("[LOG CONFIG DEBUG] Log Files will be placed in directory \"%s\"\n",
        ///    uniquePath.string().c_str());
          
        //At last, we have our uniquePath
        return uniquePath;
    }

    //The idea of this function is to get the current time and date from the operating 
    //system and format it into a std::string object with human-readable formatting to be
    //used in formulating a syntactically valid filepath. Unfortunately this task is easier
    //said then done, especially when building with modern MSVC. 
    std::string getDateTimeTag() {
        
        return getDateTimeStringTagFromOS();

        //Use preprocessor to use date+time at time-of-compilation
        std::stringstream tag;
        tag /*<< "TheImplementationForGettingDateAndTimeIsBeingReimplementedInASeperateFile"*/ << __DATE__ << __TIME__;
        std::string tagString = tag.str();
        auto tagIter = tagString.begin();
        for (; tagIter != tagString.end(); tagIter++) {
            switch (*tagIter) {
            default:
                break;
            case (' '):
                *tagIter = '_';
                break;
            case (','):
                *tagIter = '_';
                break;
            case (':'):
                *tagIter = '_';
                break;
            }
        }
        return tagString;

    }

}  //namespace EASYLOGPP_CONFIGURATION_INTERNAL


#else 
//else the Header Guard test failed so this file was included multiple times within the project
//which isn't allowed because it makes use of the macro INITIALIZE_EASYLOGGINGPP 

#pragma message("Detected that "EasyLogConfiguration.h" has been included in more than one place within the project. \n" \
"Please be aware \"EasyLogConfiguration.h\" is not meant to be included more than once. To gain access to the the logging \n" \
"functionality please make sure to include the header \"UniversalIncludes.h\", which has no restrictions on how many \n" \
"places it is allowed to appear within the codebase.\n" )
#error Compilation Error! 

#endif //EASY_LOG_CONFIGURATION_H_

