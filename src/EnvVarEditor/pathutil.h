#pragma once

#include "strutil.h"
#include <regex>
#include <crtdefs.h>
#include <crtdbg.h>

#define __PATH__FUNC__ __TSTR_FUNC__

#define __PATH_SEPERATOR__ ";"

__PATH__FUNC__ std::vector<TStr> path_split(const TStr &data);
__PATH__FUNC__ int path_compare(const TStr &left, const TStr &right);
__PATH__FUNC__ TStr path_join(std::vector<TStr> items);

__PATH__FUNC__ std::vector<TStr> path_split(const TStr &data)
{   
    // std::wcout << __FUNCTIONW__ << std::endl;
    std::basic_regex<typename TStr::value_type> reg(__TSTR__(__PATH_SEPERATOR__));
    std::regex_token_iterator<typename TStr::const_iterator> iter(data.begin(), data.end(), reg, _TRUNCATE);
    std::vector<TStr> items = std::vector<TStr>(iter, std::regex_token_iterator<typename TStr::const_iterator>());
    return items;
}
__PATH__FUNC__ int path_compare(const TStr &left, const TStr &right)
{
    TStr leftTemp = left;
    TStr rightTemp = right;
    TStr seperator_win(__TSTR__("\\"));
    TStr seperator_unix(__TSTR__("/"));
    TStr seperator_unix2(__TSTR__("//"));

    tstr_replace<TStr>(leftTemp, seperator_win, seperator_unix);
    tstr_replace<TStr>(leftTemp, seperator_unix2, seperator_unix);
    tstr_replace<TStr>(rightTemp, seperator_win, seperator_unix);
    tstr_replace<TStr>(rightTemp, seperator_unix2, seperator_unix);
    leftTemp =  tstr_trim_end(leftTemp, '/'); 
    rightTemp =  tstr_trim_end(rightTemp, '/');
    int n = tstr_cmp_icase<TStr>(leftTemp, rightTemp);
    return n;
}
__PATH__FUNC__ TStr path_join(std::vector<TStr> items)
{
    return tstr_join<TStr>(items, __TSTR__(__PATH_SEPERATOR__));
}

