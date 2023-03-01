#include "win_evar_util.h"

 std::wstring evar_get(const std::wstring &name, bool global)
{
    std::wstring val = reg_get_str(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__, name);
    return val;
}
 bool evar_exists(const std::wstring &name, bool global)
{
    return reg_value_exists(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__, name);
}
 void evar_set(const std::wstring &name, const std::wstring &val, DWORD valType, bool global)
{
    reg_set_str(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__, name, val, valType);
}
 void evar_del(const std::wstring &name, bool global)
{
    reg_del_value(global ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, global ? __HKEY_GLOBAL_ENV__ : __HKEY_USER_ENV__, name);
}
 std::map<std::wstring, std::wstring> evar_list(bool global){
    std::map<std::wstring, std::wstring> map;
    
    return map;
}