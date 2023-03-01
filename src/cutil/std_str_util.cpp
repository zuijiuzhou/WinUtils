#include "std_str_util.h"

std::wstring str_to_wstr(const std::string &str)
{
    size_t charLen = str.length() + 1;
    size_t memSize = charLen * sizeof(wchar_t);
    wchar_t *szbuf = (wchar_t *)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = mbstowcs_s(&n, szbuf, charLen, str.data(), _TRUNCATE);
    std::wstring result(szbuf);
    free(szbuf);
    return result;
}
std::string wstr_to_str(const std::wstring &wstr)
{
    size_t charLen = wstr.length() + 1;
    size_t memSize = charLen * sizeof(char);
    char *szbuf = (char *)malloc(memSize);
    memset(szbuf, 0, memSize);
    size_t n = 0;
    errno_t err = wcstombs_s(&n, szbuf, charLen, wstr.data(), _TRUNCATE);
    std::string result(szbuf);
    free(szbuf);
    return result;
}