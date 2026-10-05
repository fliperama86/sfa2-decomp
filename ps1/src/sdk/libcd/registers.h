// SPDX-License-Identifier: MIT
#include "types.h"

#ifdef SDK_PART
extern volatile u8* libcd_CDRegister0;
extern volatile u8* libcd_CDRegister1;
extern volatile u8* libcd_CDRegister2;
extern volatile u8* libcd_CDRegister3;
#else
static volatile u8* libcd_CDRegister0 = (u8*)0x1F801800;
static volatile u8* libcd_CDRegister1 = (u8*)0x1F801801;
static volatile u8* libcd_CDRegister2 = (u8*)0x1F801802;
static volatile u8* libcd_CDRegister3 = (u8*)0x1F801803;
#endif
