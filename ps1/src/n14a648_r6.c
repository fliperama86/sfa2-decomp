/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

typedef void (*ScriptFn)(void);
extern ScriptFn data_8017d368[];
extern ScriptFn data_8017d3b4[];

u8 func_8014e474(void) {
    data_8017d368[data_80189468 >> 1]();
    return 1;
}

u8 func_8014e4bc(void) {
    data_8017d3b4[data_80189468 >> 1]();
    return 1;
}
