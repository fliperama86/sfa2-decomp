/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80152ad8(Object *object, int a) {
    int base = func_80151184() & 0xf;
    int off = a * 2 + 0x30;
    func_80130768(object, (table_801802cc[base] + off) & 0xff, (SequenceStep **)table_8017c7f8);
}
