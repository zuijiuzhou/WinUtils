#pragma once

#include <regex>
#include <crtdefs.h>
#include <crtdbg.h>
#include "cutil_global.h"
#include "std_str_util.h"

#define __PATH_SEPERATOR__ L';'

 CUTIL_EXPORT std::vector<std::wstring> path_split(const std::wstring &data);
 CUTIL_EXPORT int path_compare(const std::wstring &left, const std::wstring &right);
 CUTIL_EXPORT std::wstring path_join(std::vector<std::wstring> items);
 CUTIL_EXPORT std::wstring path_norm(const std::wstring &path);
