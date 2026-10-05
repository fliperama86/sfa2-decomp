/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_80144220(Object *object) {
    *(s32 *)&object->field_10 += object->field_4c;
    object->field_4c = object->field_4c + object->field_54;
}

void func_80144244(Object *object) {
    *(s32 *)&object->field_10 += object->field_4c;
    *(s32 *)&object->field_14 -= object->field_50;
}

void func_80144268(Object *object) {
    handlers_b5f4[object->field_06](object);
    func_80131094(object);
    func_80120028(object);
}

void func_801442c4(Object *object) {
    object->pos_x = -0x40;
    object->pos_y = 0x70;
    object->field_46 = 0xf;
    object->field_4c = 0xc0000;
    object->field_54 = 0x8000;
    object->field_06 = object->field_06 + 1;
    func_80130768(object, 8, table_8017b50c);
}
