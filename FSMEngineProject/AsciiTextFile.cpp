//
// File:                 AsciiTextFile.h
//
//
//  READ ME:    So in these constructors near the top of the file, the trick being done is 
//              to use the 'str_view' type as a decoy parameter for uniquely identifying the
//              delegating constructor. By giving it this unusual signature, it makes sure that
//              the delegating constructor is able to fully run before any of the other public 
//              constructors can create their implementations (which could throw, which is bad 
//              if the constructor hasn't created a complete type yet).
//              Now I am highly suspicious that in pretty much any other circumstance it would be 
//              highly inadvisable to create these temporary objects which are created here from the
//              'std::string_view' constructor and 'std::filesystem::file' constructor because
//              what's happening is the str_view object is getting created from a local type which 
//              leaves scope almost instantaneously. Now this is just asking for the code in the 
//              delegating constructor to cause undefined behavior, but I think I can get away 
//              with it here because the delegating constructor doesn't actually do anything with
//              its parameter. 
//
#include "UniversalIncludes.h"
#include "AsciiTextFile.h"
#include "AsciiTextFileImpl.h"


///////////////////////////////////////////////////////////////////////
//                     DELEGATING CONSTRUCTOR
//Private Constructor that every other constructor delegates to. This one 
//makes sure object gets fully initialized
AsciiTextFile::AsciiTextFile(str_view path) {
    LOG(TRACE) << __FUNCTION__;
    (void)path; //path here would qualify for [[MAYBE_UNUSED]]
    pAsciiTextFileImpl_ = nullptr; 
}
///////////////////////////////////////////////////////////////////////


AsciiTextFile::AsciiTextFile(const char* path) noexcept(false) :
    AsciiTextFile(str_view(path)) { //Explicitly call the delegating constructor
    LOG(TRACE) << __FUNCTION__;
    LOG(DEBUG) << "Used Private Delegating Constructor To Ensure Fully Initialized AsciiTextFile Object";
    pAsciiTextFileImpl_ = std::make_unique<AsciiTextFileImpl>(path);
}

AsciiTextFile::AsciiTextFile(const std::string& path) noexcept(false) :
    AsciiTextFile(str_view(path)) { //Explicitly call the delegating constructor
    LOG(TRACE) << __FUNCTION__;
    LOG(DEBUG) << "Used Private Delegating Constructor To Ensure Fully Initialized AsciiTextFile Object";
    pAsciiTextFileImpl_ = std::make_unique<AsciiTextFileImpl>(path);
}

AsciiTextFile::AsciiTextFile(std::string_view path) noexcept(false) :
    AsciiTextFile(str_view(path.data())) {/*path.data() creates a temporary string that leaves
                                       scope almost instantaneously, but since the str_view being created never  
                                       is used within the delegating constructor this shouldn't ever cause
                                       undefined behavior... [See note near top of file] */
    LOG(TRACE) << __FUNCTION__;
    LOG(DEBUG) << "Used Private Delegating Constructor To Ensure Fully Initialized AsciiTextFile Object";
    pAsciiTextFileImpl_ = std::make_unique<AsciiTextFile::AsciiTextFileImpl>(path);
}

AsciiTextFile::AsciiTextFile(std::filesystem::path path) noexcept(false) :
    AsciiTextFile(str_view(path.string())) {/*Same as the other constructor above, a temporary string is 
                                            used to construct the str_view but since the data in the str_view 
                                            is never access then it shouldn't be an issue...*/
    LOG(TRACE) << __FUNCTION__;
    LOG(DEBUG) << "Used Private Delegating Constructor To Ensure Fully Initialized AsciiTextFile Object";
    pAsciiTextFileImpl_ = std::make_unique<AsciiTextFile::AsciiTextFileImpl>(path);
}


AsciiTextFile::~AsciiTextFile() { 
    LOG(TRACE) << __FUNCTION__;
}




AsciiTextFile::AsciiTextFile(const AsciiTextFile& that) noexcept {
    LOG(TRACE) << __FUNCTION__;
    pAsciiTextFileImpl_ = std::make_unique<AsciiTextFile::AsciiTextFileImpl>(*(that.pAsciiTextFileImpl_));
}

AsciiTextFile::AsciiTextFile(AsciiTextFile&& that) noexcept {
    LOG(TRACE) << __FUNCTION__;
    *this = std::move(that); //Call the move assignment operator
}

AsciiTextFile& AsciiTextFile::operator=(const AsciiTextFile& that) noexcept { 
    LOG(TRACE) << __FUNCTION__;
    if (this != &that) {
        pAsciiTextFileImpl_ = std::make_unique<AsciiTextFile::AsciiTextFileImpl>(*(that.pAsciiTextFileImpl_));
    }
    return *this;
}

AsciiTextFile& AsciiTextFile::operator=(AsciiTextFile&& that) noexcept {
    LOG(TRACE) << __FUNCTION__;
    if (this != &that) {
        pAsciiTextFileImpl_ = std::move(that.pAsciiTextFileImpl_); //Move over the implementation unique_ptr
    }
    return *this;
}




const std::filesystem::path& AsciiTextFile::path() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->path());
}

str_view AsciiTextFile::getViewOfFullFileText() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->getViewOfFullFileText());
}

std::string AsciiTextFile::getFullCopyOfFileText() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->getFullCopyOfFileText());
}

size_t AsciiTextFile::countNumberOfLines() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->countNumberOfLines());
}

size_t AsciiTextFile::countNumberOfLinesThatBeginWith(char c) const noexcept {
    LOG(TRACE) << "\nCalled With Char '" << c << "'    \n" <<__FUNCTION__ << "\n\n";
    return (pAsciiTextFileImpl_->countNumberOfLinesThatBeginWith(c));
}

void AsciiTextFile::getLinesThatBeginWithCharacter(char c, std::vector<size_t>& lines) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    pAsciiTextFileImpl_->getLinesThatBeginWithCharacter(c, lines);
}

size_t AsciiTextFile::getLineLength(size_t line) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->getLineLength(line));
}

str_view AsciiTextFile::getViewOfLine(size_t line) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->getViewOfLine(line));
}

std::string AsciiTextFile::getCopyOfLine(size_t line) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->getCopyOfLine(line));
}

size_t AsciiTextFile::getFileSize() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return (pAsciiTextFileImpl_->getFileSize());
}


