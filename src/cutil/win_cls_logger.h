#pragma once

#include <Windows.h>
#include <iostream>
#include "cutil_global.h"
#include "std_str_util.h"

enum MessageType
{
    MT_TEXT = 1,
    MT_INFO,
    MT_WARN,
    MT_ERR
};

__TSTR_FUNC__ void clslog(const TStr &msg, MessageType type);
__CUTIL_EXPORT__ void clslog(const char *sz_msg, MessageType type);
__CUTIL_EXPORT__ void clslog(const wchar_t *sz_msg, MessageType type);
__CUTIL_EXPORT__ void clserr(const char *sz_err, const char *sz_inner_err);

