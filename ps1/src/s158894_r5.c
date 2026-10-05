/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801598bc(u32 a) {
    *hw.ctrl = a;
    data_8018d2bc[a >> 24] = a;
}
