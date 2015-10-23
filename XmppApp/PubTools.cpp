#include "stdafx.h"
#include "PubTools.h"

#include <codecvt>

PubTools::PubTools()
{
}


PubTools::~PubTools()
{
}

std::string PubTools::StrToUtf8W(const std::wstring &wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

    return conv.to_bytes(wstr);
}

std::wstring PubTools::Utf8ToStrW(const std::string &utf8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

    return conv.from_bytes(utf8);
}