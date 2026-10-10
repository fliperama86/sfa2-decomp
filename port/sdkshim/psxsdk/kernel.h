/* Shim: the library units name this header; PsyZ's has the same role.

   PsyZ's kernel.h renames EnterCriticalSection and ExitCriticalSection
   (Win32 names) with macros. The units call the console's functions by
   their own names, which the port binds to the PS1 addresses; the macros
   are undone here. */
#ifndef SDKSHIM_PSXSDK_KERNEL_H
#define SDKSHIM_PSXSDK_KERNEL_H

#include <kernel.h>

#undef EnterCriticalSection
#undef ExitCriticalSection

#endif
