#pragma once

#include <string>

class PubTools
{
public:
    PubTools();
    virtual ~PubTools();

public:
    static std::string StrToUtf8W(const std::wstring &wstr);

    static std::wstring Utf8ToStrW(const std::string &utf8);

    static std::wstring GenerateUUID();
};

#define WStrToUtf8(wstr)\
    PubTools::StrToUtf8W(wstr)

#define Utf8ToWStr(utf8)\
    PubTools::Utf8ToStrW(utf8)
