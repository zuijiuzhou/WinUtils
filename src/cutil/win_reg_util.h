#pragma once

#include <Windows.h>
#include <string>
#include "cutil_global.h"

__CUTIL_EXPORT__ bool reg_key_exists(HKEY hkey, const std::wstring &subkey);
__CUTIL_EXPORT__ bool reg_value_exists(HKEY hkey, const std::wstring &subkey, const std::wstring &name);
__CUTIL_EXPORT__ void reg_del_value(HKEY hkey, const std::wstring &subkey, const std::wstring &name);
__CUTIL_EXPORT__ std::wstring reg_get_str(HKEY hkey, const std::wstring &subkey, const std::wstring &name);
__CUTIL_EXPORT__ void reg_set_value(HKEY hkey, const std::wstring &subkey, const std::wstring &name, void *pVal, size_t lenVal, DWORD valType = REG_SZ);
__CUTIL_EXPORT__ void reg_set_str(HKEY hkey, const std::wstring &subkey, const std::wstring &name, const std::wstring &val, DWORD valType = REG_SZ);
