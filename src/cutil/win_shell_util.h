#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include "cutil_global.h"
#include "std_io_path_util.h"

CUTIL_EXPORT int start_as_admin(const std::wstring &exe, const std::wstring &params);

CUTIL_EXPORT int restart_as_admin(bool waitForExit = true);

