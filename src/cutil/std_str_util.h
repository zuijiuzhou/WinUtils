#pragma once

#include <string>
#include <vector>
#include <functional>
#include <regex>
#include <stdarg.h>
#include "cutil_global.h"

#ifdef _MSVC_LANG
#if (_MSVC_LANG >= 202002L)
#define __CXX_STD_20__
#endif
#endif

#define __TSTR_IS_STRING__ constexpr(std::is_same<TStr, std::string>::value)
#define __TSTR_IS_WSTRING__ constexpr(std::is_same<TStr, std::wstring>::value)

#ifdef __CXX_STD_20__
#define __TSTR_FUNC__ template <typename TStr> \
    requires std::is_same<TStr, std::string>::value || std::is_same<TStr, std::wstring>::value
#else
#define __TSTR_FUNC__ template <typename TStr>
#endif

#define __TSTR_TYPE_ERROR__ static_assert("only support std::string or std::wstring")

__CUTIL_EXPORT__ std::wstring str_to_wstr(const std::string &str);
__CUTIL_EXPORT__ std::string wstr_to_str(const std::wstring &wstr);
__TSTR_FUNC__ TStr str_to_tstr(const std::string &str);
__TSTR_FUNC__ TStr wstr_to_tstr(const std::wstring &wstr);
__TSTR_FUNC__ std::string tstr_to_str(const TStr &tstr);
__TSTR_FUNC__ std::wstring tstr_to_wstr(const TStr &tstr);
__TSTR_FUNC__ int tstr_cmp_icase(const TStr &left, const TStr &right);
__TSTR_FUNC__ void tstr_replace(TStr &str, const TStr &from, const TStr &to);
__TSTR_FUNC__ TStr tstr_join(std::vector<TStr> items, TStr seperator);
__TSTR_FUNC__ TStr tstr_trim_end(const TStr &tstr, typename TStr::value_type theChar = 32);
__TSTR_FUNC__ TStr tstr_trim_start(const TStr &tstr, typename TStr::value_type theChar = 32);
__TSTR_FUNC__ TStr tstr_trim(const TStr &tstr, typename TStr::value_type theChar = 32);
__TSTR_FUNC__ void tstr_split(const TStr &data, std::vector<TStr> &ret);
__TSTR_FUNC__ void tstr_split(const TStr &data, typename TStr::value_type seperator, std::vector<TStr> &ret);
__TSTR_FUNC__ TStr tstr_format(const typename TStr::value_type *fmt, ...);
__TSTR_FUNC__ bool tstr_is_clear(const TStr &tstr);

__TSTR_FUNC__ TStr str_to_tstr(const std::string &str)
{
    if __TSTR_IS_STRING__
    {
        return str;
    }
    else if __TSTR_IS_WSTRING__
    {
        return str_to_wstr(str);
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return TStr();
    }
}
__TSTR_FUNC__ TStr wstr_to_tstr(const std::wstring &wstr)
{
    if __TSTR_IS_STRING__
    {
        return wstr_to_str(wstr);
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
__TSTR_FUNC__ std::string tstr_to_str(const TStr &tstr)
{
    if __TSTR_IS_STRING__
    {
        return tstr;
    }
    else if __TSTR_IS_WSTRING__
    {
        return wstr_to_str(tstr);
    }
    else
    {
        __TSTR_TYPE_ERROR__;
        return std::string();
    }
}
__TSTR_FUNC__ std::wstring tstr_to_wstr(const TStr &tstr)
{
    if __TSTR_IS_STRING__
    {
        return str_to_wstr(tstr);
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
__TSTR_FUNC__ int tstr_cmp_icase(const TStr &left, const TStr &right)
{
    size_t min_num_chars = left.length() < right.length() ? left.length() : right.length();
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
__TSTR_FUNC__ void tstr_replace(TStr &str, const TStr &from, const TStr &to)
{
    if (str.empty() || from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != TStr::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
__TSTR_FUNC__ TStr tstr_join(std::vector<TStr> items, TStr seperator)
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
__TSTR_FUNC__ TStr tstr_trim_end(const TStr &tstr, typename TStr::value_type theChar)
{
    auto chrIdx = (int)tstr.length() - 1;
    while (chrIdx >= 0)
    {
        if (tstr.at(chrIdx) == theChar)
        {
            chrIdx--;
        }
        else
        {
            break;
        }
    }
    if (chrIdx > 0)
    {
        return TStr(tstr, 0, chrIdx + 1);
    }
    return TStr();
}
__TSTR_FUNC__ TStr tstr_trim_start(const TStr &tstr, typename TStr::value_type theChar)
{
    TStr ret = tstr;
    auto foundAt = std::find_if(ret.begin(), ret.end(), [theChar](typename TStr::value_type c)
                                { return theChar != c; });

    ret.erase(ret.begin(), foundAt);
    return ret;
}
__TSTR_FUNC__ TStr tstr_trim(const TStr &tstr, typename TStr::value_type theChar)
{
    return tstr_trim_start(tstr_trim_end(tstr, theChar), theChar);
}
__TSTR_FUNC__ void tstr_split(const TStr &data, std::vector<TStr> &ret)
{
    tstr_split(data, 32, ret);
}
__TSTR_FUNC__ void tstr_split(const TStr &data, typename TStr::value_type seperator, std::vector<TStr> &ret)
{
    TStr sep(1, seperator);
    std::basic_regex<typename TStr::value_type> reg(sep);
    std::regex_token_iterator<typename TStr::const_iterator> iter(data.begin(), data.end(), reg, (int)_TRUNCATE);
    ret.assign(iter, std::regex_token_iterator<typename TStr::const_iterator>());
    // std::vector<TStr> items = std::vector<TStr>(iter, std::regex_token_iterator<typename TStr::const_iterator>());
}
__TSTR_FUNC__ TStr tstr_format(const typename TStr::value_type *fmt, ...)
{
    TStr ret;
    va_list args;
    va_start(args, fmt);
    int n = 0;
    if __TSTR_IS_STRING__
    {
        n = vsnprintf(0, 0, fmt, args);
    }
    else if __TSTR_IS_WSTRING__
    {
        n = vswprintf(0, 0, fmt, args);
    }
    va_end(args);

    va_start(args, fmt);
    if (n > ret.capacity())
    {
        ret.reserve(n + 1);
    }
    if __TSTR_IS_STRING__
    {
        n = vsnprintf(ret.data(), ret.capacity(), fmt, args);
    }
    else if __TSTR_IS_WSTRING__
    {
        n = vswprintf(ret.data(), ret.capacity(), fmt, args);
    }
    va_end(args);
    return TStr(ret.data());
}
__TSTR_FUNC__ bool tstr_is_clear(const TStr &tstr)
{
    static const std::basic_regex<typename TStr::value_type> reg(str_to_tstr<TStr>("^\\s*$"));
    return tstr.length() == 0 || std::regex_match(tstr, reg);
};

#define __TSTR__(STDSTR) str_to_tstr<TStr>(STDSTR)

