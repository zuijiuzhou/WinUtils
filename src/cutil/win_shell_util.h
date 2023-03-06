#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include "cutil_global.h"
#include "std_io_path_util.h"

__CUTIL_EXPORT__ int start_as_admin(const std::wstring &exe, const std::wstring &params);

__CUTIL_EXPORT__ int restart_as_admin(bool waitForExit = true);

