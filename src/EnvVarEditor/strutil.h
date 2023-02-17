#pragma once

#include <string>
#include <vector>

#ifdef _MSVC_LANG
#if (_MSVC_LANG >= 202002L)
#define CXX_STD_20
#endif
#endif

#define __API__
#define __TSTR_IS_STRING__ constexpr(std::is_same<TStr, std::string>::value)
#define __TSTR_IS_WSTRING__ constexpr(std::is_same<TStr, std::wstring>::value)
#ifdef CXX_STD_20
#define __TSTR_FUNC__ template <typename TStr> requires std::is_same<TStr, std::string>::value || std::is_same<TStr, std::wstring>::value
#else
#define __TSTR_FUNC__ template <typename TStr>
#endif
#define __TSTR_TYPE_ERROR__ static_assert("only support std::string or std::wstring")

std::wstring string2Wstring(const std::string &str);
std::string wstring2String(const std::wstring &wstr);
__TSTR_FUNC__ TStr string2TStr(const std::string &str);
__TSTR_FUNC__ TStr wstring2TStr(const std::wstring &wstr);
__TSTR_FUNC__ std::string tstr2String(const TStr &tstr);
__TSTR_FUNC__ std::wstring tstr2Wstring(const TStr &tstr);
__TSTR_FUNC__ int tstrCompareIgnoreCase(const TStr &left, const TStr &right);
__TSTR_FUNC__ void tstrReplace(TStr &str, const TStr &from, const TStr &to);
__TSTR_FUNC__ TStr tstrJoin(std::vector<TStr> items, TStr seperator);

std::wstring string2Wstring(const std::string &str)
{
    size_t charLen = str.length() + 1;
    size_t memSize = charLen * sizeof(wchar_t);
    wchar_t *szbuf = (wchar_t *)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = mbstowcs_s(&n, szbuf, charLen, str.data(), _TRUNCATE);
    std::wstring result(szbuf);
    free(szbuf);
    return result;
}
std::string wstring2String(const std::wstring &wstr)
{
    size_t charLen = wstr.length() + 1;
    size_t memSize = charLen * sizeof(char);
    char *szbuf = (char *)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = wcstombs_s(&n, szbuf, charLen, wstr.data(), _TRUNCATE);
    std::string result(szbuf);
    free(szbuf);
    return result;
}
__TSTR_FUNC__ TStr string2TStr(const std::string &str)
{
    if __TSTR_IS_STRING__
    {
        return str;
    }
    else if __TSTR_IS_WSTRING__
    {
        return string2Wstring(str);
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_FUNC__ TStr wstring2TStr(const std::wstring &wstr)
{
    if __TSTR_IS_STRING__
    {
        return wstring2String(wstr);
    }
    else if __TSTR_IS_WSTRING__
    {
        return wstr;
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_FUNC__ std::string tstr2String(const TStr &tstr)
{
    if __TSTR_IS_STRING__
    {
        return tstr;
    }
    else if __TSTR_IS_WSTRING__
    {
        return wstring2String(tstr);
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return std::string();
    }
}
__TSTR_FUNC__ std::wstring tstr2Wstring(const TStr &tstr)
{
    if __TSTR_IS_STRING__
    {
        return string2Wstring(tstr);
    }
    else if __TSTR_IS_WSTRING__
    {
        return tstr;
    }
    else
    {
        return std::wstring();
        // __TSTR_TYPE_ERROR__;
    }
}
__TSTR_FUNC__ int tstrCompareIgnoreCase(const TStr &left, const TStr &right)
{
    size_t min_num_chars = min(left.length(), right.length());
    if (min_num_chars == 0)
    {
        return left.length() > right.length() ? 1 : -1;
    }
    for (size_t i = 0; i < min_num_chars; i++)
    {
        int n = towupper(left.at(i)) - towupper(right.at(i));
        if (n == 0)
        {
            continue;
        }
        return n > 0 ? 1 : -1;
    }
    return left.length() == right.length() ? 0 : (left.length() > right.length() ? 1 : -1);
}
__TSTR_FUNC__ void tstrReplace(TStr &str, const TStr &from, const TStr &to)
{
    if (from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while (start_pos = str.find(from, start_pos) != TStr::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
__TSTR_FUNC__ TStr tstrJoin(std::vector<TStr> items, TStr seperator)
{
    bool isfirst = true;
    TStr result;
    for (auto &item : items)
    {
        if (isfirst)
        {
            isfirst = false;
        }
        else
        {
            result += seperator;
        }
        result += item;
    }
    return result;
}

#define __TSTR__(STDSTR) string2TStr<TStr>(STDSTR)