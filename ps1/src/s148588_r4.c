/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8014886c(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cd44[object->state_0](object);
}

void func_801488ac(Object *object) {
    object->state_0 = object->state_0 + 1;
    func_80148a74(object);
    func_801488ec(object);
}

void func_801488ec(Object *object) {
    int base = object->table_index * 2;
    u8 index = base + 0x18;
    object->ot_depth = 4;
    object->facing_side = 1;
    if (object->variant_index >> 5 != 0) {
        index = base + 0x19;
        object->ot_depth = 8;
        object->facing_side = 0;
    }
    func_80130768(object, index, seqs_8017c7f8);
}
