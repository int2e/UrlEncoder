#include "stdafx.h"
#include "Encoder.h"
#include <string>
#include <assert.h>

using namespace std;
string Encoder::UrlEncode(const string &str)
{
    string strResult;
    size_t nLength = str.length();
    unsigned char* pString = (unsigned char*)str.c_str();
    for (size_t i = 0; i < nLength; i++)
    {
        if (isalnum((BYTE)str[i]))
        {
            char szTemp[2] = { 0 };
            sprintf(szTemp, "%c", str[i]);
            strResult.append(szTemp);
        }
        else if (isspace((BYTE)str[i]))
        {
            strResult.append("+");
        }
        else
        {
            char szTemp[4] = { 0 };
            sprintf(szTemp, "%%%X%X", pString[i] >> 4, pString[i] % 16);
            strResult.append(szTemp);
        }
    }
    return strResult;
}

string Encoder::UrlDecode(const string &str)
{
    string strResult;
    char szTemp[2];
    size_t i = 0;
    size_t nLength = str.length();
    while (i < nLength)
    {
        if (str[i] == '%')
        {
            szTemp[0] = str[i + 1];
            szTemp[1] = str[i + 2];
            strResult += StrToBin(szTemp);
            i = i + 3;
        }
        else if (str[i] == '+')
        {
            strResult += ' ';
            i++;
        }
        else
        {
            strResult += str[i];
            i++;
        }
    }
    return strResult;
}


string Encoder::UTF8UrlEncode(const string &str)
{
    return UrlEncode(AnsiStringToUTF8String(str));
}

string Encoder::UTF8UrlDecode(const string &str)
{
    return UTF8StringToAnsiString(UrlDecode(str));
}


string Encoder::UTF8StringToAnsiString(const string &strUtf8)
{
    string strResult;
    char szBuffer[4] = { 0 };
    int nUTF8StringLength = strUtf8.length();
    const char* pUTF8Buffer = strUtf8.c_str();
    int nResultLength = nUTF8StringLength + (nUTF8StringLength >> 2) + 2;
    strResult.resize(nResultLength);
    char* pAnsiBuffer = (char*)strResult.c_str();
    ZeroMemory(pAnsiBuffer, nResultLength);
    int i = 0;
    int j = 0;
    while (i < nUTF8StringLength)
    {
        if (*(pUTF8Buffer + i) >= 0)
        {
            pAnsiBuffer[j++] = pUTF8Buffer[i++];
        }
        else
        {
            WCHAR cchWideChar;
            UTF8CharToUnicodeChar(&cchWideChar, pUTF8Buffer + i);
            UnicodeToAnsi(szBuffer, 2, &cchWideChar, 1);
            unsigned short int nTemp = 0;
            nTemp = pAnsiBuffer[j] = szBuffer[0];
            nTemp = pAnsiBuffer[j + 1] = szBuffer[1];
            nTemp = pAnsiBuffer[j + 2] = szBuffer[2];
            i += 3;
            j += 2;
        }
    }
    pAnsiBuffer[j] = '\0';
    return strResult;
}

string Encoder::AnsiStringToUTF8String(const string& strAnsi)
{
    string strResult;

    const char *pAnsiString = strAnsi.c_str();
    int nAnsiStringLength = strAnsi.length();

    char szBuffer[4] = { 0 };
    strResult.clear();
    int i = 0;
    while (i < nAnsiStringLength)
    {
        if (pAnsiString[i] >= 0)
        {
            char szAsciistr[2] = { 0 };
            szAsciistr[0] = (pAnsiString[i++]);
            strResult.append(szAsciistr);
        }
        else
        {
            WCHAR cchWideChar;
            AnsiToUnicode(&cchWideChar, 1, pAnsiString + i, 2);
            UnicodeCharToUTF8Char(szBuffer, &cchWideChar);
            strResult.append(szBuffer);
            i += 2;
        }
    }
    return strResult;
}



void Encoder::AnsiToUnicode(WCHAR* pUnicodeBuffer, int nUnicodeBufferSize, const char *pAnsiBuffer, int nAnsiBufferSize)
{
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pAnsiBuffer, nAnsiBufferSize, pUnicodeBuffer, nUnicodeBufferSize);
}

void Encoder::UnicodeToAnsi(char* pAnsiBuffer, int nAnsiBufferSize, WCHAR* pUnicodeBuffer, int nUnicodeBufferSize)
{
    WideCharToMultiByte(CP_ACP, NULL, pUnicodeBuffer, nUnicodeBufferSize, pAnsiBuffer, nAnsiBufferSize, NULL, NULL);
}

void Encoder::UTF8CharToUnicodeChar(WCHAR* pUnicodeBuffer, const char *pUTF8Buffer)
{
    char* pChar = (char *)pUnicodeBuffer;
    pChar[1] = ((pUTF8Buffer[0] & 0x0F) << 4) + ((pUTF8Buffer[1] >> 2) & 0x0F);
    pChar[0] = ((pUTF8Buffer[1] & 0x03) << 6) + (pUTF8Buffer[2] & 0x3F);
}

void Encoder::UnicodeCharToUTF8Char(char* pUTF8Buffer, const WCHAR* pUnicodeBuffer)
{
    const char* pChar = (const char *)pUnicodeBuffer;
    pUTF8Buffer[0] = (0xE0 | ((pChar[1] & 0xF0) >> 4));
    pUTF8Buffer[1] = (0x80 | ((pChar[1] & 0x0F) << 2)) + ((pChar[0] & 0xC0) >> 6);
    pUTF8Buffer[2] = (0x80 | (pChar[0] & 0x3F));
}

char Encoder::CharToInt(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return (char)(ch - '0');
    }

    if (ch >= 'a' && ch <= 'f')
    {
        return (char)(ch - 'a' + 10);
    }
    if (ch >= 'A' && ch <= 'F')
    {
        return (char)(ch - 'A' + 10);
    }
    return -1;
}

char Encoder::StrToBin(char *pString)
{
    char szBuffer[2];
    char ch;
    szBuffer[0] = CharToInt(pString[0]); //make the B to 11 -- 00001011 
    szBuffer[1] = CharToInt(pString[1]); //make the 0 to 0 -- 00000000 
    ch = (szBuffer[0] << 4) | szBuffer[1]; //to change the BO to 10110000 
    return ch;
}
