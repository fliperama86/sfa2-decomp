/* Shim of the game's common.h for the sound library units (see README.md in this folder).
   Gives the fixed-width types, then the PsyZ header of the same name. */
#ifndef SDKSHIM_COMMON_H
#define SDKSHIM_COMMON_H

#include <psyz/types.h>
/* The units use bool, true and false as a plain int (found by comparing assembly: _Bool differs). */
typedef int bool;
#define true 1
#define false 0

#include_next <common.h>

#endif
