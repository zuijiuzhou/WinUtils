#pragma once

#include "regutil.h"

#define __HKEY_GLOBAL_ENV__ "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
#define __HKEY_USER_ENV__ "Environment"

__TSTR_FUNC__ TStr evar_get(const TStr &name, bool global = false);
__TSTR_FUNC__ bool evar_exists(const TStr &name, bool global = false);
__TSTR_FUNC__ void evar_set(const TStr &name, const TStr &val, DWORD valType = REG_SZ, bool global = false);
__TSTR_FUNC__ void evar_del(const TStr &name, bool global = false);
__TSTR_FUNC__ std::map<TStr, TStr> evar_list(bool global = false);

__TSTR_FUNC__ TStr evar_get(const TStr &name, bool global)
{
    TStr val = reg_get_str<TStr>(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, str_to_tstr<TStr>(global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__), name);
    return val;
}
__TSTR_FUNC__ bool evar_exists(const TStr &name, bool global)
{
    return reg_value_exists(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, str_to_tstr<TStr>(global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__), name);
}
__TSTR_FUNC__ void evar_set(const TStr &name, const TStr &val, DWORD valType, bool global)
{
    reg_set_str<TStr>(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, str_to_tstr<TStr>(global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__), name, val, valType);
}
__TSTR_FUNC__ void evar_del(const TStr &name, bool global)
{
    reg_del_value(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, str_to_tstr<TStr>(global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__), name);
}
__TSTR_FUNC__ std::map<TStr, TStr> evar_list(bool global){
    std::map<TStr, TStr> map;
    
    return map;
}