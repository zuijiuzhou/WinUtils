#pragma once

#include <Windows.h>
#include <string>
#include "cutil_global.h"

CUTIL_EXPORT bool reg_key_exists(HKEY hkey, const std::wstring &subkey);
CUTIL_EXPORT bool reg_value_exists(HKEY hkey, const std::wstring &subkey, const std::wstring &name);
CUTIL_EXPORT void reg_del_value(HKEY hkey, const std::wstring &subkey, const std::wstring &name);
CUTIL_EXPORT std::wstring reg_get_str(HKEY hkey, const std::wstring &subkey, const std::wstring &name);
CUTIL_EXPORT void reg_set_value(HKEY hkey, const std::wstring &subkey, const std::wstring &name, void *pVal, size_t lenVal, DWORD valType = REG_SZ);
CUTIL_EXPORT void reg_set_str(HKEY hkey, const std::wstring &subkey, const std::wstring &name, const std::wstring &val, DWORD valType = REG_SZ);
