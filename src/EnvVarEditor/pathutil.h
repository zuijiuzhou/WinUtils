#pragma once

#include "strutil.h"
#include <regex>
#include <crtdefs.h>
#include <crtdbg.h>

#define __PATH_FUNC__ __TSTR_FUNC__

#define __PATH_SEPERATOR__ ';'

__PATH_FUNC__ std::vector<TStr> path_split(const TStr &data);
__PATH_FUNC__ int path_compare(const TStr &left, const TStr &right);
__PATH_FUNC__ TStr path_join(std::vector<TStr> items);
__PATH_FUNC__ TStr path_norm(const TStr &path);

__PATH_FUNC__ std::vector<TStr> path_split(const TStr &data)
{
    // std::wcout << __FUNCTIONW__ << std::endl;
    std::vector<TStr> ret;
    tstr_split(data, __PATH_SEPERATOR__, ret);
    return ret;
}
__PATH_FUNC__ int path_compare(const TStr &left, const TStr &right)
{
    TStr leftTemp = path_norm(left);
    TStr rightTemp = path_norm(right);
    int n = tstr_cmp_icase<TStr>(leftTemp, rightTemp);
    return n;
}
__PATH_FUNC__ TStr path_join(std::vector<TStr> items)
{
    return tstr_join<TStr>(items, str_to_tstr<TStr>(std::string(1, __PATH_SEPERATOR__)));
}
__PATH_FUNC__ TStr path_norm(const TStr &path)
{
    TStr ret = path;
    static TStr seperator_win(__TSTR__("\\"));
    static TStr seperator_unix(__TSTR__("/"));
    static TStr seperator_unix2(__TSTR__("//"));
    tstr_replace<TStr>(ret, seperator_win, seperator_unix);
    tstr_replace<TStr>(ret, seperator_unix2, seperator_unix);

    ret = tstr_trim(ret);
    ret = tstr_trim_end(ret, seperator_unix[0]);
    return ret;
}