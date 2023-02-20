#pragma once
#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__
#include "strutil.h"
#include <io.h>
#include <assert.h>

TextEncoding file_encoding(FILE *pFile);

TextEncoding file_encoding(FILE *pFile)
{
    assert(fseek(pFile, 0, SEEK_END) == 0);
    size_t fileLen = ftell(pFile);
    TextEncoding te;
    if (fileLen == 0)
    {
        te = TE_UNKNOW;
    }
    else
    {
        assert(fseek(pFile, 0, SEEK_SET) == 0);
        char buf[64] = {0};
        size_t bufLen = 64;
        size_t readLen = fread(buf, sizeof(char), bufLen, pFile);
        char c0 = buf[0],
             c1 = buf[1],
             c2 = buf[2];
        if (readLen >= 3 && c0 == 0xef && c1 == 0xbb && c2 == 0xbf)
        {
            te = TE_UTF8;
        }
        else if (readLen >= 2 && c0 == 0xff && c1 == 0xfe)
        {
            te = TE_UTF16_LE;
        }
        else if (readLen >= 2 && c0 == 0xfe && c1 == 0xff)
        {
            te = TE_UTF16_BE;
        }
        else
        {
            assert(fseek(pFile, 0, SEEK_SET) == 0);
            size_t charIdx = 0;
            while (ftell(pFile) < fileLen)
            {
                readLen = fread(buf, sizeof(char), bufLen, pFile);
                charIdx = 0;
                while (charIdx < readLen)
                {
                    // ASC2
                    if (buf[charIdx] < 0x80)
                    {
                    }
                    // 非UTF8
                    else if (buf[charIdx] < 0xc0)
                    {
                    }
                    // 双字节UTF-8
                    else if (buf[charIdx] < 0xe0)
                    {
                        //
                        if (charIdx == readLen - 1)
                        {
                        }
                    }
                    charIdx++;
                }
            }
        }
        te = TE_UNKNOW;
    }
    return te;
}
#endif