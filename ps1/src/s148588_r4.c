/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8014886c(Object *object) {
    table_8017cd44[object->field_04](object);
}

void func_801488ac(Object *object) {
    object->field_04 = object->field_04 + 1;
    func_80148a74(object);
    func_801488ec(object);
}

void func_801488ec(Object *object) {
    int base = object->field_03 * 2;
    u8 index = base + 0x18;
    object->field_09 = 4;
    object->field_0b = 1;
    if (object->field_48 >> 5 != 0) {
        index = base + 0x19;
        object->field_09 = 8;
        object->field_0b = 0;
    }
    func_80130768(object, index, seqs_8017c7f8);
}
