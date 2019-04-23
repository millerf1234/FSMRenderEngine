#pragma once

#ifndef JSON_TEST_H_
#define JSON_TEST_H_

#include <string_view>
#include <filesystem>
#include "json.h"

class JSON_Test {
public:
    JSON_Test();
    ~JSON_Test() noexcept;

    void parseFile(std::filesystem::path) noexcept;

private:
    std::vector<std::filesystem::path> mParsedFiles_;

};

#endif //JSON_TEST_H_