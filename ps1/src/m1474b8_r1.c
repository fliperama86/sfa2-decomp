/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801474b8(Object *object) {
    object->field_04 = object->field_04 + 1;
    data_80189458 = 0;
    data_8018945c = 0;
    object->field_a0 = 1;
    object->field_a1 = 0x10;
    object->field_a2 = 0;
    if (game_state.field_64 != 0) {
        func_801204f4(object->field_3c, object->field_3c->side, 0xd);
        func_8014f604(2);
        object->field_01 = 1;
        func_801476f8(object);
        func_80130768(object, game_state.field_6b, seqs_8017cbac);
        func_801477ac(object, 1, 0x10);
    } else {
        object->field_04 = 2;
    }
}
