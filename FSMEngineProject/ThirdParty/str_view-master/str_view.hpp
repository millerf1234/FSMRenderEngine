/*
str_view - null-termination-aware string-view class for C++.

Author:  Adam Sawicki - http://asawicki.info - adam__DELETE__@asawicki.info
Version: 1.1.1, 2019-01-22
License: MIT

Documentation: see README.md and comments in the code below.

# Version history

- Version 1.1.1, 2019-01-22
  Added missing const to copy_to(), to_string() methods.
- Version 1.1.0, 2018-09-11
  Added missing const to substr() method.
- Version 1.0.0, 2018-07-18
  First version.

# License

Copyright 2018 Adam Sawicki

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#pragma once

#include <string>
#include <atomic>
#include <algorithm> // for min, max
#include <memory> // for memcmp

#include <cassert>
#include <cstring>
#include <cstdint>

#ifdef min
#undef min
#endif

inline size_t tstrlen(const char* sz) { return strlen(sz); }
inline size_t tstrlen(const wchar_t* sz) { return wcslen(sz); }
inline void tstrcpy(char* dst, size_t dstCapacity, const char* src) { strcpy_s(dst, dstCapacity, src); }
inline void tstrcpy(wchar_t* dst, size_t dstCapacity, const wchar_t* src) { wcscpy_s(dst, dstCapacity, src); }
inline int tstrncmp(const char* lhs, const char* rhs, size_t count) { return strncmp(lhs, rhs, count); }
inline int tstrncmp(const wchar_t* lhs, const wchar_t* rhs, size_t count) { return wcsncmp(lhs, rhs, count); }
inline int tstrnicmp(const char* lhs, const char* rhs, size_t count) { return _strnicmp(lhs, rhs, count); }
inline int tstrnicmp(const wchar_t* lhs, const wchar_t* rhs, size_t count) { return _wcsnicmp(lhs, rhs, count); }

template<typename CharT>
class str_view_template
{
public:
    typedef CharT CharT;
    typedef std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>> StringT;

    /*
    Initializes to empty string.
    */
    inline str_view_template();
    
    /*
    Initializes from a null-terminated string.
    Null is acceptable. It means empty string.
    */
    inline str_view_template(const CharT* sz);
    /*
    Initializes from not null-terminated string.
    Null is acceptable if length is 0.
    */
    inline str_view_template(const CharT* str, size_t length);
    /*
    Initializes from string with given length, with explicit statement that it is null-terminated.
    Null is acceptable if length is 0.
    */
    struct StillNullTerminated { };
    inline str_view_template(const CharT* str, size_t length, StillNullTerminated);
    
    /*
    Initializes from an STL string.
    length can exceed actual str.length(). It then spans to the end of str.
    */
    inline str_view_template(const StringT& str, size_t offset = 0, size_t length = SIZE_MAX);

    // Copy constructor.
    inline str_view_template(const str_view_template<CharT>& src, size_t offset = 0, size_t length = SIZE_MAX);
    // Move constructor.
    inline str_view_template(str_view_template<CharT>&& src);
    
    inline ~str_view_template();

    // Copy assignment operator.
    inline str_view_template<CharT>& operator=(const str_view_template<CharT>& src);
    // Move assignment operator.
    inline str_view_template<CharT>& operator=(str_view_template<CharT>&& src);

    /*
    Exchanges the view with that of rhs.
    */
    inline void swap(str_view_template<CharT>& rhs) noexcept;

    /*
    Returns the number of characters in the view. 
    */
    inline size_t length() const;
    /*
    Returns the number of characters in the view. 
    Usage of this method is not recommended because its name may be misleading -
    it may suggest size in bytes not in characters.
    */
    inline size_t size() const { return length(); }
    /*
    Checks if the view has no characters, i.e. whether length() == 0.
    It may be more efficient than checking length().
    */
    inline bool empty() const;
    /*
    Returns a pointer to the underlying character array.
    The pointer is such that the range [data(); data() + length()) is valid and the values in it
    correspond to the values of the view. 
    If empty() == true, returned pointer is undefined.
    */
    inline const CharT* data() const { return m_Begin; }
    /*
    Returns an iterator to the first character of the view.
    If empty() == true, returned pointer may or may not be null, but always begin() == end().
    */
    inline const CharT* begin() const { return m_Begin; }
    /*
    Returns an iterator to the character following the last character of the view.
    This character acts as a placeholder, attempting to access it results in undefined behavior. 
    */
    inline const CharT* end() const { return m_Begin + length(); }
    /*
    Returns first character in the view.
    The behavior is undefined if empty() == true.
    */
    inline const CharT front() const { return *m_Begin; }
    /*
    Returns last character in the view.
    The behavior is undefined if empty() == true. 
    */
    inline const CharT back() const { return m_Begin[length() - 1]; }
    
    inline CharT operator[](size_t index) const { return m_Begin[index]; }
    inline CharT at(size_t index) const { return m_Begin[index]; }

    /*
    Returns null-terminated string with contents of this object.
    Possibly an internal copy.
    */
    inline const CharT* c_str() const;

    /*
    Returns a view of the substring [offset, offset + length).
    length can exceed actual length(). It then spans to the end of this string.
    */
    inline str_view_template<CharT> substr(size_t offset = 0, size_t length = SIZE_MAX) const;

    /*
    Copies the substring [offset, offset + length) to the character string pointed to by dst.
    If pointer string ends before length is reached, string is copied to the end.
    Null character is not added past the end of destination.
    Returns number of characters copied.
    */
    inline size_t copy_to(CharT* dst, size_t offset = 0, size_t length = SIZE_MAX) const;

    inline void to_string(StringT& dst, size_t offset = 0, size_t length = SIZE_MAX) const;

    /*
    Compares this with rhs lexicographically.
    Returns negative value, 0, or positive value, depending on the result.
    
    Comparison is made using functions like strncmp, so they don't compare characters
    past '\0' if it's present in the string.
    */
    inline int compare(const str_view_template<CharT>& rhs, bool case_sensitive = true) const;

    inline bool operator==(const str_view_template<CharT>& rhs) const { return compare(rhs) == 0; }
    inline bool operator!=(const str_view_template<CharT>& rhs) const { return compare(rhs) != 0; }
    inline bool operator< (const str_view_template<CharT>& rhs) const { return compare(rhs) <  0; }
    inline bool operator> (const str_view_template<CharT>& rhs) const { return compare(rhs) >  0; }
    inline bool operator<=(const str_view_template<CharT>& rhs) const { return compare(rhs) <= 0; }
    inline bool operator>=(const str_view_template<CharT>& rhs) const { return compare(rhs) >= 0; }

    /*
    Checks if the string view begins with the given prefix.

    If the string view is shorter than the prefix, returns false.
    If prefix is empty, returns true.
    */
    inline bool starts_with(CharT prefix, bool case_sensitive = true) const;
    inline bool starts_with(const str_view_template<CharT>& prefix, bool case_sensitive = true) const;

    /*
    Checks if the string view ends with the given suffix.

    If the string view is shorter than the suffix, returns false.
    If suffix is empty, returns true.
    */
    inline bool ends_with(CharT suffix, bool case_sensitive = true) const;
    inline bool ends_with(const str_view_template<CharT>& suffix, bool case_sensitive = true) const;

    /*
    Finds the first substring equal to the given character sequence.
    pos - position at which to start the search.
    Returns position of the first character of the found substring, or SIZE_MAX if no such substring is found.
    If substr is empty, returns pos.
    */
    inline size_t find(CharT ch, size_t pos = 0) const;
    inline size_t find(const str_view_template<CharT>& substr, size_t pos = 0) const;

    /*
    Finds the last substring equal to the given character sequence.
    pos - position at which to start the search.
    Returns position of the first character of the found substring, or SIZE_MAX if no such substring is found.
    If substr is empty, returns pos.
    */
    inline size_t rfind(CharT ch, size_t pos = SIZE_MAX) const;
    inline size_t rfind(const str_view_template<CharT>& substr, size_t pos = SIZE_MAX) const;

    /*
    Finds the first character equal to any of the characters in the given character sequence. 
    pos - position at which to start the search.
    Returns position of the first occurrence of any character of the substring,
    or SIZE_MAX if no such character is found.
    If chars is empty, returns SIZE_MAX.
    */
    inline size_t find_first_of(const str_view_template<CharT>& chars, size_t pos = 0) const;
    /*
    Finds the last character equal to one of characters in the given character sequence.
    The search considers only the interval [0; pos].
    If the character is not present in the interval, SIZE_MAX will be returned.
    If chars is empty, returns SIZE_MAX.
    */
    inline size_t find_last_of(const str_view_template<CharT>& chars, size_t pos = SIZE_MAX) const;
    /*
    Finds the first character NOT equal to any of the characters in the given character sequence. 
    pos - position at which to start the search.
    Returns position of the first occurrence of any character not of the substring,
    or SIZE_MAX if no such character is found.
    If chars is empty, returns SIZE_MAX.
    */
    inline size_t find_first_not_of(const str_view_template<CharT>& chars, size_t pos = 0) const;
    /*
    Finds the last character NOT equal to one of characters in the given character sequence.
    The search considers only the interval [0; pos].
    If the character is not present in the interval, SIZE_MAX will be returned.
    If chars is empty, returns SIZE_MAX.
    */
    inline size_t find_last_not_of(const str_view_template<CharT>& chars, size_t pos = SIZE_MAX) const;

private:
    /*
    SIZE_MAX means unknown.
    */
    mutable std::atomic<size_t> m_Length;
    
    const CharT* m_Begin;

    /*
    1 means pointed string is null-terminated by itself.
    Any others bits set mean pointer to array with null-terminated copy.
    */
    mutable std::atomic<uintptr_t> m_NullTerminatedPtr;
};

typedef str_view_template<char> str_view;
typedef str_view_template<wchar_t> wstr_view;

template<typename CharT>
inline str_view_template<CharT>::str_view_template() :
	m_Length(0),
	m_Begin(nullptr),
	m_NullTerminatedPtr(0)
{
}

template<typename CharT>
inline str_view_template<CharT>::str_view_template(const CharT* sz) :
	m_Length(sz ? SIZE_MAX : 0),
	m_Begin(sz),
	m_NullTerminatedPtr(sz ? 1 : 0)
{
}

template<typename CharT>
inline str_view_template<CharT>::str_view_template(const CharT* str, size_t length) :
	m_Length(length),
	m_Begin(length ? str : nullptr),
	m_NullTerminatedPtr(0)
{
}

template<typename CharT>
inline str_view_template<CharT>::str_view_template(const CharT* str, size_t length, StillNullTerminated) :
	m_Length(length),
	m_Begin(nullptr),
	m_NullTerminatedPtr(0)
{
    if(length)
    {
        m_Begin = str;
        m_NullTerminatedPtr = 1;
    }
    assert(m_Begin[m_Length] == (CharT)0); // Make sure it's really null terminated.
}

template<typename CharT>
inline str_view_template<CharT>::str_view_template(const StringT& str, size_t offset, size_t length) :
	m_Length(0),
	m_Begin(nullptr),
	m_NullTerminatedPtr(0)
{
	assert(offset <= str.length());
    m_Length = std::min(length, str.length() - offset);
    if(m_Length)
    {
        if(m_Length == str.length() - offset)
        {
            m_Begin = str.c_str() + offset;
            m_NullTerminatedPtr = 1;
        }
        else
            m_Begin = str.data() + offset;
    }
}

template<typename CharT>
inline str_view_template<CharT>::str_view_template(const str_view_template<CharT>& src, size_t offset, size_t length) :
	m_Length(0),
	m_Begin(nullptr),
	m_NullTerminatedPtr(0)
{
    // Source length is unknown, constructor doesn't limit the length - it may remain unknown.
    if(src.m_Length == SIZE_MAX && length == SIZE_MAX)
    {
        m_Length = SIZE_MAX;
        m_Begin = src.m_Begin + offset;
        assert(src.m_NullTerminatedPtr == 1);
        m_NullTerminatedPtr = 1;
    }
    else
    {
        const size_t srcLen = src.length();
	    assert(offset <= srcLen);
        m_Length = std::min(length, srcLen - offset);
        if(m_Length)
        {
            m_Begin = src.m_Begin + offset;
            if(src.m_NullTerminatedPtr == 1 && m_Length == srcLen - offset)
                m_NullTerminatedPtr = 1;
        }
    }
}

template<typename CharT>
inline str_view_template<CharT>::str_view_template(str_view_template<CharT>&& src) :
	m_Length(src.m_Length.exchange(0)),
	m_Begin(src.m_Begin),
	m_NullTerminatedPtr(src.m_NullTerminatedPtr.exchange(0))
{
	src.m_Begin = nullptr;
}

template<typename CharT>
inline str_view_template<CharT>::~str_view_template()
{
    uintptr_t v = m_NullTerminatedPtr;
	if(v > 1)
		delete[] (CharT*)v;
}

template<typename CharT>
inline str_view_template<CharT>& str_view_template<CharT>::operator=(const str_view_template<CharT>& src)
{
	if(&src != this)
    {
        uintptr_t v = m_NullTerminatedPtr;
		if(v > 1)
			delete[] (CharT*)v;
		m_Begin = src.m_Begin;
		m_Length = src.m_Length.load();
		m_NullTerminatedPtr = src.m_NullTerminatedPtr == 1 ? 1 : 0;
    }
	return *this;
}

template<typename CharT>
inline str_view_template<CharT>& str_view_template<CharT>::operator=(str_view_template<CharT>&& src)
{
	if(&src != this)
    {
        uintptr_t v = m_NullTerminatedPtr;
		if(v > 1)
			delete[] (CharT*)v;
		m_Begin = src.m_Begin;
		m_Length = src.m_Length.exchange(0);
		m_NullTerminatedPtr = src.m_NullTerminatedPtr.exchange(0);
		src.m_Begin = nullptr;
    }
	return *this;
}

template<typename CharT>
inline void str_view_template<CharT>::swap(str_view_template<CharT>& rhs) noexcept
{
    const size_t rhsLength = rhs.m_Length.load();
    const size_t lhsLength = m_Length.exchange(rhsLength);
    rhs.m_Length.store(lhsLength);

    std::swap(m_Begin, rhs.m_Begin);

    const uintptr_t rhsNullTerminatedPtr = rhs.m_NullTerminatedPtr.load();
    const uintptr_t lhsNullTerminatedPtr = m_NullTerminatedPtr.exchange(rhsNullTerminatedPtr);
    rhs.m_NullTerminatedPtr.store(lhsNullTerminatedPtr);
}

template<typename CharT>
inline size_t str_view_template<CharT>::length() const
{
    size_t len = m_Length;
    if(len == SIZE_MAX)
    {
        assert(m_NullTerminatedPtr == 1);
        len = tstrlen(m_Begin);
        // It doesn't matter if other thread does it at the same time.
        // It will atomically set it to the same value.
        m_Length = len;
    }
    return len;
}

template<typename CharT>
inline bool str_view_template<CharT>::empty() const
{
    size_t len = m_Length;
    if(len == SIZE_MAX)
    {
        // Length is unknown. String is null-terminated.
        // We still don't need to know the length. We just peek first character.
        assert(m_NullTerminatedPtr == 1);
        return m_Begin == nullptr || *m_Begin == (CharT)0;
    }
    return len == 0;
}

template<typename CharT>
inline const CharT* str_view_template<CharT>::c_str() const
{
    static const CharT nullChar = (CharT)0;
	if(empty())
		return &nullChar;
    uintptr_t v = m_NullTerminatedPtr;
	if(v == 1)
    {
        //assert(m_Begin[length()] == (CharT)0); // Make sure it's really null terminated.
		return m_Begin;
    }
	if(v == 0)
    {
        // Not null terminated, so length must be known.
        assert(m_Length != SIZE_MAX);
        CharT* nullTerminatedCopy = new CharT[m_Length + 1];
        assert(((uintptr_t)nullTerminatedCopy & 1) == 0); // Make sure allocated address is even.
		memcpy(nullTerminatedCopy, begin(), m_Length * sizeof(CharT));
		nullTerminatedCopy[m_Length] = (CharT)0;

        uintptr_t expected = 0;
        if(m_NullTerminatedPtr.compare_exchange_strong(expected, (uintptr_t)nullTerminatedCopy))
            return nullTerminatedCopy;
        else
        {
            // Other thread was quicker to set his copy to m_NullTerminatedPtr. Destroy mine, use that one.
            delete[] nullTerminatedCopy;
            return (const CharT*)expected;
        }
    }
	return (const CharT*)v;
}

template<typename CharT>
inline size_t str_view_template<CharT>::copy_to(CharT* dst, size_t offset, size_t length) const
{
    const size_t thisLen = this->length();
    assert(offset <= thisLen);
    length = std::min(length, thisLen - offset);
    memcpy(dst, m_Begin + offset, length * sizeof(CharT));
    return length;
}

template<typename CharT>
inline void str_view_template<CharT>::to_string(StringT& dst, size_t offset, size_t length) const
{
    const size_t thisLen = this->length();
    assert(offset <= thisLen);
    length = std::min(length, thisLen - offset);
    dst.assign(m_Begin + offset, m_Begin + (offset + length));
}

template<typename CharT>
inline str_view_template<CharT> str_view_template<CharT>::substr(size_t offset, size_t length) const
{
    // Length can remain unknown.
    if(m_Length == SIZE_MAX && length == SIZE_MAX)
    {
        assert(m_NullTerminatedPtr == 1);
        return str_view_template<CharT>(m_Begin + offset);
    }

    const size_t thisLen = this->length();
    assert(offset <= thisLen);
	length = std::min(length, thisLen - offset);
	// Result will be null-terminated.
	if(m_NullTerminatedPtr == 1 && length == thisLen - offset)
		return str_view_template<CharT>(m_Begin + offset, length, StillNullTerminated());
	// Result will not be null-terminated.
	return str_view_template<CharT>(m_Begin + offset, length);
}

template<typename CharT>
inline int str_view_template<CharT>::compare(const str_view_template<CharT>& rhs, bool case_sensitive) const
{
    const size_t lhsLen = length();
    const size_t rhsLen = rhs.length();
    const size_t minLen = std::min(lhsLen, rhsLen);

    if(minLen > 0)
    {
        const int result = case_sensitive ?
            tstrncmp(data(), rhs.data(), minLen) :
            tstrnicmp(data(), rhs.data(), minLen);
        if(result != 0)
            return result;
    }

    if(lhsLen < rhsLen)
        return -1;
    if(lhsLen > rhsLen)
        return 1;
    return 0;
}

template<typename CharT>
inline bool str_view_template<CharT>::starts_with(CharT prefix, bool case_sensitive) const
{
    if(!empty())
    {
        if(case_sensitive)
            return *m_Begin == prefix;
        return tstrnicmp(m_Begin, &prefix, 1) == 0;
    }
    return false;
}

template<typename CharT>
inline bool str_view_template<CharT>::starts_with(const str_view_template<CharT>& prefix, bool case_sensitive) const
{
    const size_t prefixLen = prefix.length();
    if(length() >= prefixLen)
    {
        const int cmpResult = case_sensitive ?
            tstrncmp(m_Begin, prefix.m_Begin, prefixLen) :
            tstrnicmp(m_Begin, prefix.m_Begin, prefixLen);
        return cmpResult == 0;
    }
    return false;
}

template<typename CharT>
inline bool str_view_template<CharT>::ends_with(CharT suffix, bool case_sensitive) const
{
    const size_t thisLen = length();
    if(thisLen > 0)
    {
        if(case_sensitive)
            return m_Begin[thisLen - 1] == suffix;
        return tstrnicmp(m_Begin + (thisLen - 1), &suffix, 1) == 0;
    }
    return false;
}

template<typename CharT>
inline bool str_view_template<CharT>::ends_with(const str_view_template<CharT>& suffix, bool case_sensitive) const
{
    const size_t thisLen = length();
    const size_t suffixLen = suffix.length();
    if(thisLen >= suffixLen)
    {
        const int cmpResult = case_sensitive ?
            tstrncmp(m_Begin + (thisLen - suffixLen), suffix.m_Begin, suffixLen) :
            tstrnicmp(m_Begin + (thisLen - suffixLen), suffix.m_Begin, suffixLen);
        return cmpResult == 0;
    }
    return false;
}

template<typename CharT>
inline size_t str_view_template<CharT>::find(CharT ch, size_t pos) const
{
    const size_t thisLen = length();
    if(thisLen == 0)
        return SIZE_MAX;
    for(size_t i = pos; i < thisLen; ++i)
    {
        if(m_Begin[i] == ch)
            return i;
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::find(const str_view_template<CharT>& substr, size_t pos) const
{
    const size_t subLen = substr.length();
    if(subLen == 0)
        return pos;
    const size_t thisLen = length();
    if(thisLen < subLen)
        return SIZE_MAX;
    const size_t maxPos = thisLen - subLen;
    for(size_t i = pos; i <= maxPos; ++i)
    {
        if(memcmp(m_Begin + i, substr.m_Begin, subLen * sizeof(CharT)) == 0)
            return i;
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::rfind(CharT ch, size_t pos) const
{
    const size_t thisLen = length();
    if(thisLen == 0)
        return SIZE_MAX;
    for(size_t i = std::min(pos, thisLen - 1) + 1; i--; )
    {
        if(m_Begin[i] == ch)
            return i;
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::rfind(const str_view_template<CharT>& substr, size_t pos) const
{
    const size_t subLen = substr.length();
    if(subLen == 0)
        return pos;
    const size_t thisLen = length();
    if(thisLen < subLen)
        return SIZE_MAX;
    for(size_t i = std::min(pos, thisLen - subLen) + 1; i--; )
    {
        if(memcmp(m_Begin + i, substr.m_Begin, subLen * sizeof(CharT)) == 0)
            return i;
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::find_first_of(const str_view_template<CharT>& chars, size_t pos) const
{
    const size_t charsLen = chars.length();
    if(charsLen == 0)
        return SIZE_MAX;
    const size_t thisLen = length();
    for(size_t thisIndex = pos; thisIndex < thisLen; ++thisIndex)
    {
        for(size_t charsIndex = 0; charsIndex < charsLen; ++charsIndex)
        {
            if(m_Begin[thisIndex] == chars.m_Begin[charsIndex])
                return thisIndex;
        }
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::find_last_of(const str_view_template<CharT>& chars, size_t pos) const
{
    const size_t charsLen = chars.length();
    if(charsLen == 0)
        return SIZE_MAX;
    const size_t thisLen = length();
    if(thisLen == 0)
        return SIZE_MAX;
    for(size_t thisIndex = std::min(pos, thisLen - 1) + 1; thisIndex--; )
    {
        for(size_t charsIndex = 0; charsIndex < charsLen; ++charsIndex)
        {
            if(m_Begin[thisIndex] == chars.m_Begin[charsIndex])
                return thisIndex;
        }
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::find_first_not_of(const str_view_template<CharT>& chars, size_t pos) const
{
    const size_t charsLen = chars.length();
    if(charsLen == 0)
        return SIZE_MAX;
    const size_t thisLen = length();
    for(size_t thisIndex = pos; thisIndex < thisLen; ++thisIndex)
    {
        bool found = false;
        for(size_t charsIndex = 0; charsIndex < charsLen; ++charsIndex)
        {
            if(m_Begin[thisIndex] == chars.m_Begin[charsIndex])
            {
                found = true;
                break;
            }
        }
        if(!found)
            return thisIndex;
    }
    return SIZE_MAX;
}

template<typename CharT>
inline size_t str_view_template<CharT>::find_last_not_of(const str_view_template<CharT>& chars, size_t pos) const
{
    const size_t charsLen = chars.length();
    if(charsLen == 0)
        return SIZE_MAX;
    const size_t thisLen = length();
    if(thisLen == 0)
        return SIZE_MAX;
    for(size_t thisIndex = std::min(pos, thisLen - 1) + 1; thisIndex--; )
    {
        bool found = false;
        for(size_t charsIndex = 0; charsIndex < charsLen; ++charsIndex)
        {
            if(m_Begin[thisIndex] == chars.m_Begin[charsIndex])
            {
                found = true;
                break;
            }
        }
        if(!found)
            return thisIndex;
    }
    return SIZE_MAX;
}

template<typename CharT>
inline void swap(str_view_template<CharT>& lhs, str_view_template<CharT>& rhs)
{
    lhs.swap(rhs);
}
