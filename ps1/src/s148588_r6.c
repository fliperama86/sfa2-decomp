/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80148c8c(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cebc[object->field_04](object);
}

void func_80148ccc(Object *object) {
    object->field_04 = object->field_04 + 1;
    object->field_0c = 0;
    object->field_4c = *(s32 *)&object->field_10;
    object->field_50 = 0;
    object->field_58 = 0;
    object->pos_y = object->pos_y - 8;
    object->field_58 = func_80151184() & 0x30;
    object->field_54 = 0;
    object->field_46 = 0;
    func_80130768(object, object->field_48, seqs_8017c7f8);
}
