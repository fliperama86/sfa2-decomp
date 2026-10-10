/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* A library routine. The form of _SsSndReplay (sdk/libsnd/replay.c), which
   sets the byte this one clears and clears another bit of the same word;
   written with this project's names for the table. */
void func_8016a16c(short a, short b) {
    table_801ac628[a][b].field_2b = 0;
    table_801ac628[a][b].field_90 &= ~0x100;
}
