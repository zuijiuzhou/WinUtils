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
void clslog(const char *szMsg, MessageType type);
void clslog(const wchar_t *szMsg, MessageType type);

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
        typeName = "INFO:";
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
    std::cout << typeName << tstr2String(msg) << std::endl;
    SetConsoleTextAttribute(hnd, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void clslog(const char *szMsg, MessageType type)
{
    clslog<std::string>(szMsg, type);
}
void clslog(const wchar_t *szMsg, MessageType type)
{
    clslog<std::wstring>(szMsg, type);
}