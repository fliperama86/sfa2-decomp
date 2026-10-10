# sdkshim

Headers that let the units of `ps1/src/sdk/libsnd` and `ps1/src/sdk/libspu`
compile for the PC program from published files only: these headers and
PsyZ's include folder (the port's dependency). They stand in for the
headers that the repository does not carry (`ps1/src/sdk/include`).
Every line here is written by this project; nothing is copied from that
folder. `hostbuild.py --sound-library --psyz DIR` puts this folder, then
PsyZ's include folder, on the include path of those units and of the files
of `ps1/src/library_nonmatching`, and of no other unit. No path of
`ps1/src/sdk/include` is ever given to the compiler.

## The headers

- `common.h`: includes `<psyz/types.h>` (the fixed-width names `s16`, `u8`
  and the rest, taken from PsyZ), adds `bool`, `true` and `false`, then
  forwards to PsyZ's own `common.h` (`#include_next`). `bool` is a plain
  `int` and not C's `_Bool`: in one private comparison, made once with the
  matching work's headers (which the repository does not carry), the
  assembly of `libspu/s_srmp.c` was the same only with `int`. The need for each name
  came from the compiler's errors on the units.
- `psxsdk/libspu.h`: forwards to PsyZ's `libspu.h`. Adds nothing but the
  renaming below.
- `psxsdk/kernel.h`: forwards to PsyZ's `kernel.h`, then undoes its macros
  that rename `EnterCriticalSection` and `ExitCriticalSection` (they avoid
  the Win32 names). The units call the console's functions by their own
  names, which the port binds to the PS1 addresses.
- `psxsdk/stdarg.h`: `va_list` and `va_start`, `va_arg`, `va_end`,
  `va_copy` through the compiler's builtins.

## Seven declarations of PsyZ read under other names

The units define these functions with the parameter and result types of
this game's SDK version, which the units and the published
`libsnd_i.h` / `libspu_internal.h` show; PsyZ declares them with other
types, and C rejects the two. `psxsdk/libspu.h` renames them with macros
while it includes PsyZ's header and undoes the macros after, so the units'
own declarations are the only ones of these names: `SpuFree`,
`SpuSetTransferStartAddr`, `SpuIsTransferCompleted`, `SpuWritePartly`,
`SpuSetReverb`, `SpuInitMalloc`, `SpuClearReverbWorkArea`. PsyZ's files are
not changed.

## `__psyz`

The units are compiled without `__psyz` and without `VERSION_PC`, the
console's path. PsyZ's `common.h` includes `psyz.h` only under `__psyz`
(read at the pin), and it is not defined for them. The units' own accesses
to the sound chip's registers are therefore left as the library's C has them.
This build does not serve those registers.

## The one unit whose assembly differs

A one-off comparison, made once with files the repository does not carry
(the matching work's headers) and not repeatable from this repository: the
assembly of 66 of the 67 units is byte-identical to the build with those
headers. `libspu/s_sca.c`
(`SpuSetCommonAttr`) differs: PsyZ declares the two halves of `SpuVolume`
as `short`, the library C was written for `unsigned short`, and the
compiler schedules the unit's comparisons differently. Every read of such
a field in that unit is stored into 16 bits or cast to `s16` first, so the
behaviour should not depend on it. `port/tools/test_sdkshim.py` tests that:
the unit compiled with PsyZ's `short` and with a temporary `unsigned short`
copy of PsyZ's header, both run on plain memory with 150,000 inputs, the
blocks compared byte for byte; and a control that must find differences
when one cast is removed. It cannot compare with the matching work's
headers, which the repository does not have.
