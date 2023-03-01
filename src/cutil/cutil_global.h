#pragma once
#ifndef CUITL_STATIC
#ifdef CUTIL_EXPORTS
#define CUTIL_EXPORT __declspec(dllexport)
#else 
#define CUTIL_EXPORT __declspec(dllimport)
#endif
#else
#define CUTIL_EXPORT
#endif
