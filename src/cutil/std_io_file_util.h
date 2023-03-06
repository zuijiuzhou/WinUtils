#pragma once
#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__
#include <io.h>
#include <assert.h>
#include <vector>
#include <memory>
#include <string>
#include <codecvt>
#include "cutil_global.h"
#include "text.h"

__CUTIL_EXPORT__ TextEncoding file_get_encoding(const char *buf, size_t lenBuf);
__CUTIL_EXPORT__ std::vector<std::wstring> file_all_lines(FILE *pFile);

#endif