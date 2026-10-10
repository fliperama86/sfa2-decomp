/* Shim: the library units name this header; PsyZ's has the same role.

   The units define these functions with the parameter and result types of
   this game's SDK version (the units and libsnd_i.h / libspu_internal.h
   say which), where PsyZ declares them with other types. PsyZ's
   declarations are read under other names, so that the units' own
   declarations and definitions are the only ones of these names. */
#ifndef SDKSHIM_PSXSDK_LIBSPU_H
#define SDKSHIM_PSXSDK_LIBSPU_H

#define SpuFree SdkShim_PsyZ_SpuFree
#define SpuSetTransferStartAddr SdkShim_PsyZ_SpuSetTransferStartAddr
#define SpuIsTransferCompleted SdkShim_PsyZ_SpuIsTransferCompleted
#define SpuWritePartly SdkShim_PsyZ_SpuWritePartly
#define SpuSetReverb SdkShim_PsyZ_SpuSetReverb
#define SpuInitMalloc SdkShim_PsyZ_SpuInitMalloc
#define SpuClearReverbWorkArea SdkShim_PsyZ_SpuClearReverbWorkArea

#include <libspu.h>

#undef SpuFree
#undef SpuSetTransferStartAddr
#undef SpuIsTransferCompleted
#undef SpuWritePartly
#undef SpuSetReverb
#undef SpuInitMalloc
#undef SpuClearReverbWorkArea

#endif
