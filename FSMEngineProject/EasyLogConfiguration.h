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
//          |      How To        |   The FSMEngine project's expected configuration is for there to be a set of common headers which
//          | Enabling Logging   |      are responsible for implementing specific sets of related functionality. Due to the universally
//          | Functionality For  |      anticipated need for writing log messages, all of the logging message functionality can be integrated 
//          |Use In Another File:|      by simply including the header file "UniversalIncludes.h".
//          +--------------------+         
//                                 
//                       [ TROUBLESHOOTING: If for some reason it appears as though "UniversalIncludes.h" is not providing the logging
//                                            functionality, chances are that the relative filepaths between the FSMEngine's internal                                
//                                            components has been modified. A possible backup to regain logging support would be to find
//                                            out the relative filepath between the "easylogging++.h" (which is the header of the
//                                            actual library) and "UniversalInclude.h" and '#inlcude' this relative path in "UniversalInclude.h" ]
//
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!               !WARNING!
//    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
//
//
//  Please Notice:   This file contains several specific macro-function calls which are required by the library Easylogging++
//                         as part of its initalization which MUST appear only in one single location within a project. 
//
//                   Due to the restrictions this usage of specialized macros places on the manner under which this header may 
//                         be incorperated into a project, several additional preventative safety-measures have been
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
//                         message logging targets used throughout the rest of the app.
//                      
//
//                   As a result of the both the uniquely-specialized requirements imposed upon this file's usage that must restrict
//                         the permissible ways in which it may be integrated into a project on top of the both highly-specific tasks 
//                         and highly-fundamental position it occupies within the codebase, the author 'recommends as essential' that
//                         changes/modifications/updates be avoided or kept to a minimun. No changes should be made until all of the
//                         documentation for this file be read and understood and the impact of those changes fully understood. This
//                         includes performing even the smallest of changes that might occur within typical every-day refactoring.
//
//                   It is given that chances are high that any necessary changes when performed after having undergone the proper due 
//                         diligence and made under thoughtful care can be safely made to this code. However, keep in mind when 
//                         contemplating a change the ease with which it could very well impact an expected behavior or lead to major
//                         outcomes in program behavior due to the fundamental positioning and wide-spread reliance the entire codebase
//                         expects from this code.
//                                                                                           
//
//  Background:      One of the initial features built into the FSMEngine project was a robust multi-leveled series of message logs, 
//                         each which is made available for every source file to use. It was decided very early on that this feature 
//                         could best be provided through the use of a third party library called "EasyLogging++" which was built to
//                         provide this exact feature while having the bonus of allowing for a high degree of configurability.
//
//  Description:     The purpose of the code contained within this file serves one of these two related purposes:
//                           i) To provide all of the necessary information and perform all of the necessary actions necessary to
//                                 fully initialize and launch the EasyLogging++ library; and
//                          ii) To perform all the necessary configuratory requests* plus API calls which are required in conforming the 
//                                 highly-configurable EasyLogging++ library to meet the expected specifications of the rest of the 
//                                 Application. This includes both setting up the fundamental state in addition to configuring each 
//                                 of the available LOG targets individually. 
//
//                                                                                  *This file is responsible for only configuring the available run-time settings. 
//                                                                                     There is a seperate EasyLogging++ configuration file called "EasyLogging_BuildConfig.h"
//                                                                                     that handles specifing compile-time settings.
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
//  [Description of Implementation]
//      The methods available for configuring EasyLogging++ exist in several different forms, from loading configuration from a file to 
//          performing individual inline configuration function calls. It should be noted that the way this code performs all of the 
//          configuration is through the usage of the provided configuration object, which contains internal variables representing the
//          state of each log 'level' (target) able to recieve a configuation. To use this object, we first construct it [see
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
//  Date:            January 10, 2019 --    Started implementing file, got outline and file creation in place but left configuration unfinished
//                   February 27, 2019  --  Resumed work towards finishing implementation


#ifndef EASY_LOG_CONFIGURATION_H_
#define EASY_LOG_CONFIGURATION_H_

#pragma message("  Testing Requirement:\n\t\tA request has been made to include the header \"EasyLogCongiuration.h\" within compilation.\n" \
"\t\t  Testing preconditions for determining if requested spot of inclusion has met the necessary requirements...")
#ifdef ALLOW_ELCC_CONFIGURATION
#pragma message("  Testing Completed:\n\t\t\"EasyLogConfiguration.h\" has detected that all proper requirements have been successfully satisfied by this location." \
/* "\n\t\t  which help ensure that this point-of-inclusion is indeed the proper place to do so. \n" */ \
 "\n\t\t  As a result, the compilation will now be resumed without hinderance...\n")
#undef ALLOW_ELCC_CONFIGURATION
#else 
#pragma message("\n  Testing Failed! [See Error Report] \n\n" \
"\n \t\t\t\t[ERROR REPORT!]\n\t\t The compiler was forced to cease progress with the current compilation due to one or more detected violations against restrictions \n" \
"         in place from integrated security measures for the header file \"EasyLogConfiguration.h\" which exist to prevent inadvertant access to it.\n" \
"       Please be aware that this file contains several macro statements which necessitate a very specific location and manner in which this header file\n" \
"         can safely and legally be included in a project." \
"\n  Suggested Solutions:\n" \
"   [RECOMMENDED]   (i) To resolve this error, simply remove each of the include statements for \"EasyLogConfiguration.h\" that caused this error to trigger\n" \
"                  (ii) If you are looking to add logging-message functions to a source file, be aware that \"EasyLogConfiguration.h\" isn't the one you want. Try instead including \"UniversalIncludes.h\"\n" \
"                 (iii) Otherwise, if you actually believe you have a proper place in your codebase that you wish to use for \"EasyLogConfiguration.h\", consult its internal comments \n"\
"                        to get an understanding of which necessary requirements and steps your code can take to deactivate the triggering of this sudden-death automatic compilation error\n\n" )

//This configuration file MUST only be included at one isolated place within each easylog++-assocaited project. If it is known that\n \
// a place has been selected that meets these strict criteria, then define the macro listed in the documentation to disable this\n \
// compiler error message from preventing compilation.")
#error Illegal Multiple Inclusions of EasyLogg++ Configuration Header
#endif //ALLOW_ELCC_CONFIGURATION 


#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <optional>
#include <ctime>

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
        LOG(WARNING) << "\nOh Dear! Something went wrong!\n"
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
    

    //                                              +------------------+   
    //                                              | Additional NOTES |
    //                                              +------------------+
    
    
    //  -----------------------       REPORTING DATETIME FORMAT SPECIFICATION       -----------------------
    //   How to specifiy what information and format to include in message for reporting DATETIME information
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
    //  be empty. Thus it is recommended to have a seperate logger for use when logging from threads while 
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



    ////////////////////
    ////        Step 2      SET CONFIGURATION FOR EACH LEVEL 
    /////////////////             

    //Configure Trace
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput, "0");
    //logConfigurator.set(el::Level::Trace, el::ConfigurationType::Format, "[%level] %datetime{%h:%m:%s,%g} [Thread=%thread] {%loc} msg=%msg");
    //logConfigurator.set(el::Level::Trace, el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s.%g} [%level] '%func'\n%fbase line %line  %msg");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Format, ""
        "\n< < < < < < < < < < < < < < < <[%level]> > > > > > > > > > > > > > > >\n"//"\n          <TRACE>              <TRACE>              <%level>\n"
        "+~~~~~~~~~~~~~~~~~~~~~~~~~+   file: '%fbase' line %line\n"
        "| %datetime{%Y-%M-%d %H:%m:%s.%g} |   func: %func\n"
        "+~~~~~~~~~~~~~~~~~~~~~~~~~+    msg: \"%msg\"");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Trace)->string());
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::SubsecondPrecision, "6");
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::MaxLogFileSize, "5242880");  //expects input type as size_t  //5 MB
    logConfigurator.set(el::Level::Trace, el::ConfigurationType::LogFlushThreshold, "16"); //expects input type as size_t
  
    //Configure Debug  (The macro 'USE_DEBUG_' would be defined in the header "BuildSettings.h")
#ifndef USE_DEBUG_     
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "0");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToFile, "0");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToStandardOutput, "0");
#else 
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::ToStandardOutput, "1");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Format, "\n"
        "`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`\n"
        "  [FSMEngine %level]\n"
        "  Location: '%fbase':%line\n  Message: %msg\n"
        "`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`'`\n");
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::Filename,
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Debug)->string());
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::SubsecondPrecision, "1");

    logConfigurator.set(el::Level::Debug, el::ConfigurationType::MaxLogFileSize, "5242880");  //5 MB
    logConfigurator.set(el::Level::Debug, el::ConfigurationType::LogFlushThreshold, "1");
#endif 

    //Configure Fatal  [Be warned that logging to FATAL will trigger the executable to abort (i.e. crash)]
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::ToStandardOutput, "1");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Format, "\n\n\n[%level] at (line %line '%fbase') <encountered %datetime{%b %d, %Y %F %g:%m:%s}> in '%func'\nMSG=%msg");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::Filename,
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Fatal)->string());
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::SubsecondPrecision, "6");
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::MaxLogFileSize, "2048"); //There probably won't be that many fatal messages... I hope
    logConfigurator.set(el::Level::Fatal, el::ConfigurationType::LogFlushThreshold, "1");  //Try 0?

    //Configure Error
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput, "1");
    //logConfigurator.set(el::Level::Error, el::ConfigurationType::Format, "[%level Time=%datetime{%h:%m:%s}]  %func\n(line %line '%fbase') %msg");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Format, "\n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
        "~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERRO\n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
        "  [FSMEngine %level]\n"
        "  Location: '%fbase':%line\n  Message: %msg\n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
        "ROR!~~~~~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERROR! ~~~~~~ ERROR! ~~\n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::Filename, 
        EASYLOGPP_CONFIGURATION_INTERNAL::getFilepathToLogForLevel(el::Level::Error)->string());
    logConfigurator.set(el::Level::Error, el::ConfigurationType::SubsecondPrecision, "3");
    logConfigurator.set(el::Level::Error, el::ConfigurationType::MaxLogFileSize, "1048576"); //1048576 Bytes = 1 MB
    logConfigurator.set(el::Level::Error, el::ConfigurationType::LogFlushThreshold, "1"); 

    //Configure Warning
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::ToFile, "1");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::ToStandardOutput, "1");
    logConfigurator.set(el::Level::Warning, el::ConfigurationType::Format, "\n[%level Time = %datetime{%h:%m:%s}]  '%func'\n{line %line '%fbase'}\nMSG: %msg");
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

    //Configure Verbose  [Verbose is Disabled]
    logConfigurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "0");

    //Configure Unknown
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Enabled, "1");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::ToFile, "0");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::ToStandardOutput, "1");
    logConfigurator.set(el::Level::Unknown, el::ConfigurationType::Format, "[UNKNOWN] %msg");
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


    //Test the loggers... 

    LOG(TRACE) << "BEFORE INFO THO!";

    LOG(INFO) << "\nThe 'info' log will probably get the most usage!";
    LOG(INFO) << "Thus when multiple messages such as these appear!";
    LOG(INFO) << "They should feel natural.";
    LOG(INFO) << "And not present too much clutter to render them difficult to read in bulk...";
    LOG(INFO) << "It also should be possible to encounter messages";
    LOG(INFO) << "                         formatted to be written over multiple lines ";
    LOG(INFO) << "                                                               to be extra fancy.";
    LOG(INFO) << "And possibly end with new lines [or not...]\n";
    LOG(INFO) << "And now for a test of the other available logs...\n\n";
    
    LOG(TRACE);
    
    LOG(TRACE) //<< "\n\n------------------------------------------------------------------------\n"
        << "Testing the Trace Log!!! WOOT!";
        //<< "\n------------------------------------------------------------------------\n\n";
    
    LOG(DEBUG) << "Testing the DEBUG Log!!! WOOT! [DEBUG Test!]";


    LOG(INFO) << "And now for some of the more serious logs...\n\n";


    LOG(WARNING) << "A message of importance from the warning log is this!\n";

    LOG(ERROR) << "An even more important message notifying of an error this one is!";


    //Be aware that logging to FATAL will call abort which basically records the log message and then on-purpose crashes the program
    //LOG(FATAL) << "The following major issue has been encountered resulting in a Fatality!\n";

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
                    //[[fallthrough]];
                default:
                    logFilepath += "Default.log";
                    break;
                }
                return std::make_optional<std::filesystem::path>(logFilepath);
            }
        }
    }

    //I am still searching for a more modern algorithm to do this... Had a few false positives for
    //things to try that made me start rewriting the function only to discover that the new techniques
    //flat out didn't work or they secretly relied on what I was doing in the old implementation anyways. 
#if defined USE_MORE_MODERN_ALGORITHM_TO_GET_DATE_TIME
    std::optional<std::filesystem::path> getLogFileDirectory() {
        //Static filepath will be initialized first time this function is called.
        //After returning it will remain initialized throughout the remaining lifetime
        //of the program, thus allowing us to access is again simply by calling this function
        static std::filesystem::path logDirectory{}; 

        //We can tell if there is already a path to our directory for holding out logs by just seeing it 
        //the filepath is empty (i.e. just created)
        if (!(logDirectory.empty())) { //If filepath is not empty, we already have created one and can return it
            return std::make_optional<std::filesystem::path>(logDirectory);
        }
        else { //no filepath yet exists 
            //Thus we must create one
            std::error_code ec;
            ec.clear(); 
            //This sets logDirectory to the same filepath as from where we launched our '.exe' 
            logDirectory = std::filesystem::current_path(ec); 
            if (ec) {
                LOG(ERROR) << "Unable to communicate with filesystem to set up directory for log files!\n"
                    << "The OS reports error: " << ec.message() << "\n\n";
                return std::nullopt;
            }
            else { //we're in business
                logDirectory = logDirectory.lexically_normal(); //Do this step just in case anything is weird about our filepath
                logDirectory = logDirectory.string() + "\\LOGS\\"; //We are going to want to move into a subdirectory titled 'LOGS'
                logDirectory = logDirectory.make_preferred(); //Ensures each '/' and '\' in filepath are all set to OS-preferred separator 


                //Now it is time to append a unique identifier onto the end of it. Date and time 
                //will pretty reliably be unique enough for our purposes.
                //An example tag could look like: 2019_060_09_46_32      [year_dayOfYear_Hour_Min_Sec]
                std::stringstream tag; //Tag is to be built into stringstream
               

                //OOps it turns out that the 'modern' implementation I thought didn't rely upon std::localtime,
                //but I was wrong. It just was really good at hiding the fact that it used it. 
    
            
            
            }
        }
    
    }

#else 
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
#endif // USE_MORE_MODERN_ALGORITHM_TO_GET_DATE_TIME


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

