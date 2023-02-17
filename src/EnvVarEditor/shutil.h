#pragma once

#include "pathutil.h"
#include <ShlObj.h>

__TSTR_FUNC__ int startAsAdmin(const TStr &exe, const TStr &params);

int restartAsAdmin(bool waitForExit = true);

__TSTR_FUNC__ int startAsAdmin(const TStr &exe, const TStr &params)
{
    std::wstring wexe = tstr2Wstring(exe);
    std::wstring wparams = tstr2Wstring(params);
    return 0;
}

int restartAsAdmin(bool waitForExit)
{
    TCHAR exe[MAX_PATH];
    ZeroMemory(exe, MAX_PATH);
    GetModuleFileName(NULL, exe, MAX_PATH);
    LPWSTR cmdline = GetCommandLine();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("CreateProcess failed (%d)", GetLastError());
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}