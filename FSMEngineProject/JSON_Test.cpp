


#include "JSON_Test.h"

#include <sstream>

#include "UniversalIncludes.h"




JSON_Test::JSON_Test() {
    LOG(TRACE) << __FUNCTION__;
}


JSON_Test::~JSON_Test() noexcept {
    LOG(TRACE) << __FUNCTION__;
}


void JSON_Test::parseFile(std::filesystem::path targetJSON) noexcept {
    LOG(TRACE) << __FUNCTION__;

    std::error_code ec { };
    const bool isDirectory = std::filesystem::is_directory(targetJSON, ec);
    if (ec) {
        LOG(INFO) << "Error occured while opening file: ";
        LOG(INFO) << targetJSON;
        LOG(INFO) << ec.message();
        return;
    }
    
    if (isDirectory) {
        LOG(INFO) << "\nUnable to Parse Specified JSON file because ";
        LOG(INFO) << "the \'file\' is actually a directory!";
        return;
    }

    auto filesize = std::filesystem::file_size(targetJSON, ec);
    if (ec) {
        LOG(INFO) << "Error occured while opening file: ";
        LOG(INFO) << targetJSON;
        LOG(INFO) << ec.message();
        return;
    }
    else
        LOG(INFO) << "Size of the JSON file is: " << filesize;
    
    //if (targetJSON.is_absolute())
        ///
    //else 
        ///

    std::ostringstream msg;
    msg << "Parsing file " << targetJSON.filename() << " as a JSON file...\n";
    msg << "  [Full Filepath: \"" << targetJSON.generic_string() << "\"]\n\n";

    LOG(INFO) << msg.str();
}