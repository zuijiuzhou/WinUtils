#pragma once
#include <map>
#include "cutil_global.h"
#include "win_reg_util.h"

#define __HKEY_GLOBAL_ENV__ L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
#define __HKEY_USER_ENV__ L"Environment"

CUTIL_EXPORT std::wstring evar_get(const std::wstring &name, bool global = false);
CUTIL_EXPORT bool evar_exists(const std::wstring &name, bool global = false);
CUTIL_EXPORT void evar_set(const std::wstring &name, const std::wstring &val, DWORD valType = REG_SZ, bool global = false);
CUTIL_EXPORT void evar_del(const std::wstring &name, bool global = false);
CUTIL_EXPORT std::map<std::wstring, std::wstring> evar_list(bool global = false);
