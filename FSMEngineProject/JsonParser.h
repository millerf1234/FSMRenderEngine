

#pragma once

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <string>
#include <sstream>
#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf
#include <string_view>
#include "UniversalIncludes.h"
#include "..\jsoncpp\include\json.h"
#include "FSMGlobalInvariants.h"
#include "FSMInternalGlobalState.h"


class JsonParser {
public:
    JsonParser() : mVal1(0), nameVal1("Val1") {
        LOG(TRACE) << __FUNCTION__;
        std::ostringstream constructorMsg;
        constructorMsg << "\n\n\n\n\n\n\n\n\n";
        constructorMsg << "Performing JSON Parse Test!\n"
            << "  In the Constructor for JsonParser, the values\n"
            << "  of mVal1 and nameVal1 are: \n"
            << "            mVal1   is   " << mVal1 << "\n"
            << "           nameVal1 is   " << nameVal1 << "\n"
            << "  Now Leaving constructor...\n\n";

        LOG(INFO) << constructorMsg.str();
    }


    int mVal1;
    std::string nameVal1;


    void parseTestJson() {
        LOG(TRACE) << __FUNCTION__;

        //Json::Reader reader;
        
        if constexpr (true)
            return;

        std::filesystem::path path = FSMEngineInternal::pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + FSMEngineInvariant::RELATIVE_PATH_TO_INIT_CONFIG_FILE.string();
        path = path.lexically_normal();

        try {
            if (!std::filesystem::exists(path)) {
                std::string msg("No File Exists at ");
                msg.append(path.string());
                msg.append("\n");
                reportParseFail(msg);
                return;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            (void)e;
            std::string msg("An error occured in Filesystem while verifying existance of file ");
            msg.append(path.string());
            msg.append("\n");
            reportParseFail(msg);
            return;
        }

        //If we made it this far, then it must be true that our path
        //to the JSON file represents a valid file

        std::filebuf fb;
        if (fb.open(path.string(), std::ios::in)) {
            std::istream is(&fb);
            //BLARGH!!!
            fb.close();
        }
    }


private:
    void reportParseFail(std::string_view msg) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        LOG(INFO) << "Failed to Parse JSON because " << msg;
    }

};



#endif //JSON_PARSER_H_
