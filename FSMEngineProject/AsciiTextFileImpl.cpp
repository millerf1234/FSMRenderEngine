//
// File:                  AsciiTextFileImpl.cpp
//
//
//                 This is where the implementation is 


//#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "str_view.hpp"//Custom Version which guarantees each view is terminated with '\0' //#include <string_view>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <system_error>

#include "AsciiTextFile.h"
#include "AsciiTextFileImpl.h"
#include "UniversalIncludes.h"

static char EMPTY_STRING[1u] = "";

AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl() {
    LOG(TRACE);
    mPath_ = "";
    mFileText_ = "";
    mFileTextLineCount_ = 0;
}


AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl(const char* path) noexcept(false) : AsciiTextFileImpl() {
    LOG(TRACE);
    mPath_ = std::filesystem::path(path);
    mPath_.make_preferred();
    LOG(DEBUG) << "Preparing to Load File " << path;
    if (!std::filesystem::exists(mPath_)) {
        LOG(DEBUG) << "Loading Failed! Unable to Locate File " << path;
        throw std::invalid_argument("Unable to locate a file at the specified path");
    }
    else {
        LOG(DEBUG) << "Confirmed Existence of a File at the Specified Location. Loading File...";
        loadFile();
        LOG(DEBUG) << "Populating Internal Data Structure Representing File Layout";
        parseTextToPopulateLineOffsets();
    }
}

AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl(const std::string& path) noexcept(false) : AsciiTextFileImpl() {
    LOG(TRACE);
    mPath_ = std::filesystem::path(path);
    mPath_.make_preferred();
    LOG(DEBUG) << "Preparing to Load File " << path;
    if (!std::filesystem::exists(mPath_)) {
        LOG(DEBUG) << "Loading Failed! Unable to Locate File " << path;
        throw std::invalid_argument("Unable to locate a file at the specified path");
    }
    else {
        LOG(DEBUG) << "Confirmed Existence of a File at the Specified Location. Loading File...";
        loadFile();
        LOG(DEBUG) << "Populating Internal Data Structure Representing File Layout";
        parseTextToPopulateLineOffsets();
    }
}

AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl(std::string_view path) noexcept(false) : AsciiTextFileImpl() {
    LOG(TRACE);
    mPath_ = std::filesystem::path(path);
    mPath_.make_preferred();
    LOG(DEBUG) << "Preparing to Load File " << path;
    if (!std::filesystem::exists(mPath_)) {
       LOG(DEBUG) << "Loading Failed! Unable to Locate File " << path;
       throw std::invalid_argument("Unable to locate a file at the specified path");
    }
    else {
        LOG(DEBUG) << "Confirmed Existence of a File at the Specified Location. Loading File...";
        loadFile();
        LOG(DEBUG) << "Populating Internal Data Structure Representing File Layout";
        parseTextToPopulateLineOffsets();
    }
}

AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl(std::filesystem::path path) noexcept(false) : AsciiTextFileImpl() {
    LOG(TRACE);
    mPath_ = path;
    mPath_.make_preferred();
    LOG(DEBUG) << "Preparing to Load File " << path;
    if (!std::filesystem::exists(mPath_)) {
        LOG(DEBUG) << "Loading Failed! Unable to Locate File " << path;
        throw std::invalid_argument("Unable to locate a file at the specified path");
    }
    else {
        LOG(DEBUG) << "Confirmed Existence of a File at the Specified Location. Loading File...";
        loadFile();
        LOG(DEBUG) << "Populating Internal Data Structure Representing File Layout";
        parseTextToPopulateLineOffsets();
    }
}



AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl(const AsciiTextFile::AsciiTextFileImpl& that) noexcept {
    LOG(TRACE);
    mPath_ = that.mPath_;
    mFileTextLineCount_ = that.mFileTextLineCount_;

    //Reserve space for string before copying
    mFileText_.reserve(that.mFileText_.length());
    mFileText_ = that.mFileText_;
    
    //Also reserve space in vector before copying
    mLineOffsets_.reserve(that.mLineOffsets_.size());
    std::copy(begin(that.mLineOffsets_), end(that.mLineOffsets_), std::back_inserter(mLineOffsets_));
}

AsciiTextFile::AsciiTextFileImpl::AsciiTextFileImpl(AsciiTextFile::AsciiTextFileImpl&& that) noexcept {
    LOG(TRACE);
    mPath_ = that.mPath_;
    mFileTextLineCount_ = that.mFileTextLineCount_;

    //Swap the contents of the string and vector
    mFileText_.swap(that.mFileText_);
    mLineOffsets_.swap(that.mLineOffsets_);
}

AsciiTextFile::AsciiTextFileImpl& AsciiTextFile::AsciiTextFileImpl::operator=(const AsciiTextFile::AsciiTextFileImpl& that) noexcept {
    LOG(TRACE);
    if (this != &that) {
        mPath_ = that.mPath_;
        mFileTextLineCount_ = that.mFileTextLineCount_;

        //Reserve space for string before copying
        mFileText_.reserve(that.mFileText_.length());
        mFileText_ = that.mFileText_;

        //Also reserve space in vector before copying
        mLineOffsets_.reserve(that.mLineOffsets_.size());
        std::copy(begin(that.mLineOffsets_), end(that.mLineOffsets_), std::back_inserter(mLineOffsets_));
    }
    return *this;
}
AsciiTextFile::AsciiTextFileImpl& AsciiTextFile::AsciiTextFileImpl::operator=(AsciiTextFile::AsciiTextFileImpl&& that) noexcept {
    LOG(TRACE);
    if (this != &that) {
        mPath_ = that.mPath_;
        mFileTextLineCount_ = that.mFileTextLineCount_;
                                                                     
        mFileText_.swap(that.mFileText_);
        mLineOffsets_.swap(that.mLineOffsets_);
    }
    return *this;
}



AsciiTextFile::AsciiTextFileImpl::~AsciiTextFileImpl() noexcept { LOG(TRACE); }


const std::filesystem::path& AsciiTextFile::AsciiTextFileImpl::path() const noexcept {
    LOG(TRACE);
    return mPath_;
}

str_view AsciiTextFile::AsciiTextFileImpl::getViewOfFullFileText() const noexcept {
    LOG(TRACE);
    str_view fullTextView(mFileText_);
    return fullTextView;
}

std::string AsciiTextFile::AsciiTextFileImpl::getFullCopyOfFileText() const noexcept {
    LOG(TRACE);
    
    // OPTION A
    //return mFileText_;

    // OPTION B
    std::string fullTextCopy;
    fullTextCopy.reserve(mFileText_.length());
    mFileText_.copy(&fullTextCopy[0], mFileText_.length());
    assert(mFileText_.size() == fullTextCopy.size());
    return fullTextCopy;
}

size_t AsciiTextFile::AsciiTextFileImpl::countNumberOfLines() const noexcept {
    LOG(TRACE);
    return mLineOffsets_.size();
}

//The vector of line offsets must be complete and accurate for this function to operate properly
size_t AsciiTextFile::AsciiTextFileImpl::countNumberOfLinesThatBeginWith(char c) const noexcept {
    LOG(TRACE);
    const char * fileText = mFileText_.c_str();
    int lineCounter = 0;
    const char * lineIter;

    for (size_t i = 0; i < mLineOffsets_.size(); i++) {
        lineIter = (fileText + mLineOffsets_[i].offset);
        if (*lineIter == c) {
            lineCounter++;
        }
        //Eat up the tabs and whitespace 
        else if ((*lineIter == ' ') || (*lineIter == '\t')) {
            while ((*lineIter == ' ') || (*lineIter == '\t')) {
                lineIter++;
            } //Check for match with specified character
            if (*lineIter == c) {
                lineCounter++;
            }
        }
    }
    return lineCounter;
}

//The vector of line offsets must be complete and accurate for this function to operate properly
void AsciiTextFile::AsciiTextFileImpl::getLinesThatBeginWithCharacter(char c, std::vector<int>& lines) const noexcept {
    LOG(TRACE);
    const char* fileText = mFileText_.c_str();
    const char* lineIter;

    for (size_t i = 0; i < mLineOffsets_.size(); i++) {
        lineIter = (fileText + mLineOffsets_[i].offset);
        if (*lineIter == c) {
            lines.push_back(i);
        }
        //Eat up the tabs and whitespace 
        else if ((*lineIter == ' ') || (*lineIter == '\t')) {
            while ((*lineIter == ' ') || (*lineIter == '\t')) {
                lineIter++;
            } //Check for match with specified character
            if (*lineIter == c) {
                lines.push_back(i);
            }
        }
    }
}

//The vector of line offsets must be complete and accurate for this function to operate properly
size_t AsciiTextFile::AsciiTextFileImpl::getLineLength(size_t line) const noexcept {
    LOG(TRACE);
    if (line > mLineOffsets_.size())
        return static_cast<size_t>(0u);
    else
        return mLineOffsets_[line].lineLength;
}

//The vector of line offsets must be complete and accurate for this function to operate properly
str_view AsciiTextFile::AsciiTextFileImpl::getViewOfLine(size_t line) const noexcept {
    LOG(TRACE);
    if (line > mLineOffsets_.size())
        return str_view(EMPTY_STRING);
    else {
        //Create a view of the whole string from which to extract the substring view so as to avoid 
        str_view full(mFileText_);  //constructing an unnecessary std::string temporary
        str_view viewOfSpecificLine = full.substr(mLineOffsets_[line].offset, mLineOffsets_[line].lineLength);
        return viewOfSpecificLine;
    }
}

//The vector of line offsets must be complete and accurate for this function to operate properly
std::string AsciiTextFile::AsciiTextFileImpl::getCopyOfLine(size_t line) const noexcept {
    LOG(TRACE);
    if (line > mLineOffsets_.size())
        return std::string(EMPTY_STRING);
    else 
        return mFileText_.substr(mLineOffsets_[line].offset, mLineOffsets_[line].lineLength);
    
}

size_t AsciiTextFile::AsciiTextFileImpl::getFileSize() const noexcept {
    LOG(TRACE);
    size_t filesize = 0u;
    std::filesystem::_File_size(mPath_, filesize);
    return filesize;
}




void AsciiTextFile::AsciiTextFileImpl::loadFile() {
    LOG(TRACE);

    //Choose one of the following options

    //OPTION 1  -- C FILE I/O
    loadUsingCFunc();
    
    //OPTION 2  -- C++ IFStream 
    //loadUsingIFStream();
}

void AsciiTextFile::AsciiTextFileImpl::loadUsingCFunc() {
    LOG(TRACE);
    LOG(INFO) << "File Is Being Loaded Using The C FileIO API";
    std::string path = mPath_.string();
#pragma warning( push )
#pragma warning( disable : 4996 ) 
    std::FILE *fp = std::fopen(path.c_str(), "rt"); //the 't' is ignored but signify's file should be open as text (and not as binary)
#pragma warning( pop )
    if (fp) {
        std::fseek(fp, 0L, SEEK_END);
        mFileText_.resize(std::ftell(fp));
        std::rewind(fp);
        std::fread(&mFileText_[0], 1u, mFileText_.size(), fp);
        std::fclose(fp);   
        LOG(INFO) << "File Has Been Loaded Into Application's Memory";
    }
    else
        throw(errno);
}

void AsciiTextFile::AsciiTextFileImpl::loadUsingIFStream() {
    LOG(TRACE);
    LOG(INFO) << "File Is Being Loaded Using C++'s FStream";
    std::ifstream inFile{ mPath_ }; //Open a file stream from the filepath 
    mFileText_ = { std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>() };
    if (mFileText_.length() > 0u) {
        //(Implementation note: Adding a newline to the string's end reduces edge conditions)
        mFileText_ += '\n'; //(Side note: I added this after most of class was already implemented) 
    }
    LOG(INFO) << "File Has Been Loaded Into Application's Memory";
}



void AsciiTextFile::AsciiTextFileImpl::parseTextToPopulateLineOffsets()  {
    LOG(TRACE);
    mLineOffsets_.clear(); //Clear any pre-existing line offset data
    if (mFileText_.length() > 0u) {
        size_t lineNumberCounter = 0u;
        std::string::iterator textIterator;
        //Find the location of the first newLine character
        textIterator = std::find(mFileText_.begin(), mFileText_.end(), '\n');
        if (textIterator == mFileText_.end()) {
            addNewLineCharacterToFileTextIfOnlyOneLineOfTextWasRead(); //Add a newline to the text so that 'line 0' can be well defined
            //Note that our iterator may have been invalidated 
            //after modifying the string, so we must reacquire it
            textIterator = std::find(mFileText_.begin(), mFileText_.end(), '\n');
        }
        //So we know at this point that we have textIterator placed on the first newline 
        //that occurs in the file. Thus we can add position 0 as a special case (in that 
        //it's offset isn't to a newline char) before entering the parsing loop.

        //Add location 0 for line 0 to the vector
        mLineOffsets_.emplace_back(lineNumberCounter++, 0, std::distance(mFileText_.begin(), textIterator) + 1);

        //parse the rest of the lines of text
        while ((++textIterator) != mFileText_.end()) { //Repeat until we run out of lines
            std::string::iterator startOfCurrentLine = textIterator;
            textIterator = std::find(startOfCurrentLine, mFileText_.end(), '\n');

            //If we reach the end without encountering a newline
            if (textIterator == mFileText_.end()) {
                textIterator--;
                mLineOffsets_.emplace_back(lineNumberCounter++,                                      //Line number
                    std::distance(mFileText_.begin(), startOfCurrentLine),    //Offset
                    std::distance(startOfCurrentLine, textIterator) + 1);     //Line length
                continue; //Next iteration will cause exit condition to trigger
            }
            else if (startOfCurrentLine == textIterator) { //If the line consists solely of a single newline char
                mLineOffsets_.emplace_back(lineNumberCounter++, std::distance(mFileText_.begin(), startOfCurrentLine), 1);
            }
            else { //We encountered a new line that isn't at the end of the text
                mLineOffsets_.emplace_back(lineNumberCounter++,                                      //Line number
                    std::distance(mFileText_.begin(), startOfCurrentLine),    //Offset
                    std::distance(startOfCurrentLine, textIterator) + 1);     //Line Length
            }
        }
    }
    else {//file is empty of data
        LOG(WARNING) << "The File " << mPath_.string() << " was acquired from the OS successfully but no text was able to be read from it!";
        mLineOffsets_.emplace_back(0u, 0u, 1u);
    }
    mFileTextLineCount_ = mLineOffsets_.size();
}


void AsciiTextFile::AsciiTextFileImpl::addNewLineCharacterToFileTextIfOnlyOneLineOfTextWasRead() noexcept {
    LOG(TRACE);
    if (mFileText_.length() > 0) {
        std::string::iterator it;
        //Try to find a newline character in the file-text
        it = std::find(mFileText_.begin(), mFileText_.end(), '\n');
        if (it == mFileText_.end()) //If no newlines are in the filetext
            mFileText_ += '\n'; //Add a newline at the end of the one liner program

        if (mFileTextLineCount_ == 0)
            mFileTextLineCount_ = 1; //Set mFileTextLineCount_ to the proper value 
    }
}