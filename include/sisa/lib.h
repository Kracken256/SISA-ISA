/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#ifndef __SISA_LIB_H__
#define __SISA_LIB_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(SISA_DEFINES) && defined(export)
#error "conflicting definitions for export macro"
#elif !defined(SISA_DEFINES)
#define SISA_DEFINES
#ifdef _WIN32
#define export __declspec(dllexport)
#else
#define export __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // __SISA_LIB_H__
