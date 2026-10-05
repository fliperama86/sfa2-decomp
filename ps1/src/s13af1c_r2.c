/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern ObjectRef ref_third;

void func_8013b7b8(void) {
    kind_pair_handlers[kind_pair_table[ref_third.p->field_02 + (ref_second.p->field_02 << 5)]]();
}
