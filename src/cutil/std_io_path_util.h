#pragma once

#include <regex>
#include <crtdefs.h>
#include <crtdbg.h>
#include "cutil_global.h"
#include "std_str_util.h"

#define __PATH_SEPERATOR__ L';'

 __CUTIL_EXPORT__ std::vector<std::wstring> path_split(const std::wstring &data);
 __CUTIL_EXPORT__ int path_compare(const std::wstring &left, const std::wstring &right);
 __CUTIL_EXPORT__ std::wstring path_join(std::vector<std::wstring> items);
 __CUTIL_EXPORT__ std::wstring path_norm(const std::wstring &path);
