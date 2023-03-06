#pragma once
#ifndef __CUITL_STATIC__
#ifdef __CUTIL_EXPORTS__
#define __CUTIL_EXPORT__ __declspec(dllexport)
#else 
#define __CUTIL_EXPORT__ __declspec(dllimport)
#endif
#else
#define __CUTIL_EXPORT__
#endif
