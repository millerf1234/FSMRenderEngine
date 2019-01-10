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

#ifndef EASY_LOG_CONFIGURATION_H_
#define EASY_LOG_CONFIGURATION_H_

#include "ThirdParty/easyloggingpp/include/easylogging++.h" 
INITIALIZE_EASYLOGGINGPP

void configureEasyLogger(int argc, char ** argv) {
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations defaultConf;
    defaultConf.setToDefault();

    defaultConf.set(el::Level::Info,
        el::ConfigurationType::Format, "%datetime %level %msg");
   


}

#endif //EASY_LOG_CONFIGURATION_H_