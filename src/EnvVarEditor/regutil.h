#pragma once

#include <Windows.h>
#include "strutil.h"

__TSTR_FUNC__ bool reg_key_exists(HKEY hkey, const TStr &subkey);
__TSTR_FUNC__ bool reg_value_exists(HKEY hkey, const TStr &subkey, const TStr &name);
__TSTR_FUNC__ void reg_del_value(HKEY hkey, const TStr &subkey, const TStr &name);
__TSTR_FUNC__ TStr reg_get_str(HKEY hkey, const TStr &subkey, const TStr &name);
__TSTR_FUNC__ void reg_set_value(HKEY hkey, const TStr &subkey, const TStr &name, void *pVal, size_t lenVal, DWORD valType = REG_SZ);
__TSTR_FUNC__ void reg_set_str(HKEY hkey, const TStr &subkey, const TStr &name, const TStr &val, DWORD valType = REG_SZ);

__TSTR_FUNC__ bool reg_value_exists(HKEY hkey, const TStr &subkey, const TStr &name)
{
    HKEY hk_sub;
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, wsubkey.data(), 0, KEY_READ, &hk_sub)))
    {
        DWORD valType;
        DWORD valSize;
        if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, wname.data(), 0, &valType, 0, &valSize)))
        {
            RegCloseKey(hk_sub);
            return true;
        }
        RegCloseKey(hk_sub);
    }
    return false;
}
__TSTR_FUNC__ void reg_del_value(HKEY hkey, const TStr &subkey, const TStr &name)
{
    HKEY hk_sub;
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, wsubkey.data(), 0, KEY_WRITE, &hk_sub)))
    {
        if (ERROR_SUCCESS == (lret = RegDeleteValue(hk_sub, wname.data())))
        {
            RegCloseKey(hk_sub);
        }
        RegCloseKey(hk_sub);
    }
    if (ERROR_SUCCESS != lret)
    {
        throw std::exception("Failed to delete the reg value");
    }
}
__TSTR_FUNC__ TStr reg_get_str(HKEY hkey, const TStr &subkey, const TStr &name)
{
    HKEY hk_sub;
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);
    std::wstring result;
    if (ERROR_SUCCESS == (lret = RegOpenKeyEx(hkey, wsubkey.data(), 0, KEY_READ, &hk_sub)))
    {
        DWORD valType;
        DWORD valSize;
        if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, wname.data(), 0, &valType, 0, &valSize)))
        {
            wchar_t *szbuf = new wchar_t[valSize + 1]{0};
            if (ERROR_SUCCESS == (lret = RegQueryValueEx(hk_sub, wname.data(), 0, &valType, (LPBYTE)szbuf, &valSize)))
            {
                result.assign(szbuf);
            }
            delete[] szbuf;
        }
        RegCloseKey(hk_sub);
    }
    return tstr_to_wstr<TStr>(result);
}
__TSTR_FUNC__ void reg_set_value(HKEY hkey, const TStr &subkey, const TStr &name, void *pVal, DWORD lenVal, DWORD valType = REG_SZ)
{
    LSTATUS lret;
    std::wstring wsubkey = tstr_to_wstr(subkey);
    std::wstring wname = tstr_to_wstr(name);

    if (ERROR_SUCCESS == (lret = RegSetKeyValue(hkey, wsubkey.data(), wname.data(), valType, pVal, lenVal)))
    {
    }
    else
    {
        throw std::exception("Failed to set the reg value");
    }
}
__TSTR_FUNC__ void reg_set_str(HKEY hkey, const TStr &subkey, const TStr &name, const TStr &val, DWORD valType)
{
    reg_set_value<TStr>(hkey, subkey, name, (void*)val.data(), (DWORD)(val.length() * sizeof(typename TStr::value_type)));
}
