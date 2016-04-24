#pragma once
#include <wtypes.h>
#include <string>

class Encoder
{
public:
    Encoder() {}
    ~Encoder() {}

    std::string UrlEncode(const std::string &str);
    std::string UrlDecode(const std::string &str);

    std::string UTF8UrlEncode(const std::string &str);
    std::string UTF8UrlDecode(const std::string &str);


private:

    std::string UTF8StringToAnsiString(const std::string &strUtf8);
    std::string AnsiStringToUTF8String(const std::string& strAnsi);

    void AnsiToUnicode(WCHAR* pUnicodeBuffer, int nUnicodeBufferSize, const char *pAnsiBuffer, int nAnsiBufferSize);
    void UnicodeToAnsi(char* pAnsiBuffer, int nAnsiBufferSize, WCHAR* pUnicodeBuffer, int nUnicodeBufferSize);

    void UTF8CharToUnicodeChar(WCHAR* pUnicodeBuffer, const char *pUTF8Buffer);
    void UnicodeCharToUTF8Char(char* pUTF8Buffer, const WCHAR* pUnicodeBuffer);

    char CharToInt(char ch);
    char StrToBin(char *pString);

};
