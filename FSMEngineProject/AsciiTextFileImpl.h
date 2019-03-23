
//This is the header for the AsciiTextFile Implementation 


#pragma once

#ifndef ASCII_TEXT_FILE_IMPL_H_
#define ASCII_TEXT_FILE_IMPL_H_

#include <vector>

#include "AsciiTextFile.h"


class AsciiTextFile::AsciiTextFileImpl {
public:
    AsciiTextFile::AsciiTextFileImpl();
    AsciiTextFile::AsciiTextFileImpl(const char* path) noexcept(false);
    AsciiTextFile::AsciiTextFileImpl(const std::string& path) noexcept(false);      
    AsciiTextFile::AsciiTextFileImpl(std::string_view path) noexcept(false);
    AsciiTextFile::AsciiTextFileImpl(std::filesystem::path path) noexcept(false);

    AsciiTextFile::AsciiTextFileImpl(const AsciiTextFile::AsciiTextFileImpl& that) noexcept;
    AsciiTextFile::AsciiTextFileImpl(AsciiTextFile::AsciiTextFileImpl&& that) noexcept;
    AsciiTextFile::AsciiTextFileImpl& operator=(const AsciiTextFile::AsciiTextFileImpl& that) noexcept;
    AsciiTextFile::AsciiTextFileImpl& operator=(AsciiTextFile::AsciiTextFileImpl&& that) noexcept;

    ~AsciiTextFileImpl() noexcept;

    const std::filesystem::path& path() const noexcept;
    str_view getViewOfFullFileText() const noexcept;
    std::string getFullCopyOfFileText() const noexcept;
    size_t countNumberOfLines() const noexcept;
    size_t countNumberOfLinesThatBeginWith(char c) const noexcept;
    void getLinesThatBeginWithCharacter(char c, std::vector<int>& lines) const noexcept;
    size_t getLineLength(size_t line) const noexcept;
    str_view getViewOfLine(size_t line) const noexcept;
    std::string getCopyOfLine(size_t line) const noexcept;
    size_t getFileSize() const noexcept;

private:
    std::filesystem::path mPath_;
    std::string mFileText_;
    int mFileTextLineCount_;

    struct NewLineLocation { //The start of a non-empty filetext string will be considered 
        size_t lineNumber;              //'location 0' representing line number 0
        size_t offset;
        size_t lineLength;
        //Constructor
        NewLineLocation(size_t number, size_t offset, size_t lineLength = 0u) : 
            lineNumber(number), offset(offset), lineLength(lineLength) { ; }
    };

    std::vector<NewLineLocation> mLineOffsets_;


    //Helper implementation functions
    void loadFile();
    void loadUsingCFunc();
    void loadUsingIFStream();

    //Populates the vector of line offsets
    void parseTextToPopulateLineOffsets();

    void addNewLineCharacterToFileTextIfOnlyOneLineOfTextWasRead() noexcept;
};



#endif //ASCII_TEXT_FILE_IMPL_H_
  
  
  