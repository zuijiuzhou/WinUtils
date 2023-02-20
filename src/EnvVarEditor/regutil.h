#pragma once

#include <Windows.h>
#include "strutil.h"

__TSTR_FUNC__ bool reg_key_exists(HKEY hkey, const TStr &subkey);
__TSTR_FUNC__ bool reg_value_exists(HKEY hkey, const TStr &subkey, const TStr &name);
__TSTR_FUNC__ TStr reg_str_value(HKEY hkey, const TStr &subkey, const TStr &name, DWORD maxlen = MAX_PATH);
__TSTR_FUNC__ void reg_str_value_set(HKEY hkey, const TStr &subkey, const TStr &name, const TStr &val, DWORD valType = REG_SZ);

__TSTR_FUNC__ bool reg_value_exists(HKEY hkey, const TStr &subkey, const TStr &name)
{
    HKEY hk_sub;
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);
    if (ERROR_SUCCESS == (lret == RegOpenKeyEx(hkey, wsubkey.data(), 0, KEY_READ, &hk_sub)))
    {
        wchar_t szbuf[1] = {0};
        DWORD valType = REG_SZ;
        DWORD valSize = sizeof(szbuf);
        if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, wname.data(), 0, &valType, (LPBYTE)szbuf, &valSize)))
        {
            RegCloseKey(hk_sub);
            return true;
        }
        RegCloseKey(hk_sub);
    }
    return false;
}
__TSTR_FUNC__ TStr reg_str_value(HKEY hkey, const TStr &subkey, const TStr &name, DWORD maxlen)
{
    HKEY hk_sub;
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);
    std::wstring result;
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, wsubkey.data(), 0, KEY_READ, &hk_sub)))
    {
        wchar_t *szbuf = new wchar_t[maxlen]{0};
        DWORD valType = REG_EXPAND_SZ;
        DWORD valSize = maxlen * sizeof(wchar_t);

        if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, wname.data(), 0, &valType, (LPBYTE)szbuf, &valSize)))
        {
            result.assign(szbuf);
        }
        delete[] szbuf;
        RegCloseKey(hk_sub);
    }
    return tstr_to_wstr<TStr>(result);
}
__TSTR_FUNC__ void reg_str_value_set(HKEY hkey, const TStr &subkey, const TStr &name, const TStr &val, DWORD valType)
{
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);
    std::wstring wval = tstr_to_wstr(val);

    if (ERROR_SUCCESS == (lret = RegSetKeyValue(hkey, wsubkey.data(), wname.data(), REG_SZ, wval.data(), wval.length() * sizeof(wchar_t))))
    {
    }
    else
    {
        throw std::exception("faild to set the env value");
    }
}