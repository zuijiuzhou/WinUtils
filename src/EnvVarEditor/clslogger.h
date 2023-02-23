#pragma once

#include <Windows.h>
#include "strutil.h"

enum MessageType
{
    MT_TEXT = 1,
    MT_INFO,
    MT_WARN,
    MT_ERR
};

__TSTR_FUNC__ void clslog(const TStr &msg, MessageType type);
void clslog(const char *sz_msg, MessageType type);
void clslog(const wchar_t *sz_msg, MessageType type);
void clserr(const char *sz_err, const char *sz_inner_err);

__TSTR_FUNC__ void clslog(const TStr &msg, MessageType type)
{
    DWORD output = STD_OUTPUT_HANDLE;
    WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    std::string typeName;
    switch (type)
    {
    case MT_TEXT:
        output = STD_OUTPUT_HANDLE;
        attr = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
        break;
    case MT_INFO:
        output = STD_OUTPUT_HANDLE;
        attr = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        break;
    case MT_WARN:
        output = STD_OUTPUT_HANDLE;
        attr = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED;
        typeName = "WARN:";
        break;
    case MT_ERR:
        output = STD_ERROR_HANDLE;
        attr = FOREGROUND_INTENSITY | FOREGROUND_RED;
        typeName = "ERR:";
        break;
    default:
        break;
    }
    HANDLE hnd = GetStdHandle(output);
    SetConsoleTextAttribute(hnd, attr);
    std::cout << typeName << tstr_to_str(msg) << std::endl;
    SetConsoleTextAttribute(hnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void clslog(const char *sz_msg, MessageType type)
{
    clslog<std::string>(sz_msg, type);
}
void clslog(const wchar_t *sz_msg, MessageType type)
{
    clslog<std::wstring>(sz_msg, type);
}
void clserr(const char *sz_err, const char *sz_inner_err)
{
    int len = strlen(sz_err) + strlen(sz_inner_err) + 1;
    char* buffer = new char[len]{0};
    
}