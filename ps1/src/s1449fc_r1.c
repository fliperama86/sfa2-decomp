/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801449fc(Object *object) {
    object->field_04++;
}

void func_80144a10(Object *object) {
    table_8017b654[object->field_06](object);
    func_80131094(object);
    func_80120028(object);
}

void func_80144a6c(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->field_46 = 0xf;
    object->field_4c = 0xfff40000;
    object->field_54 = -0x8000;
    object->field_06++;
    func_80144ee4(object, 9);
}

void func_80144ac0(Object *object) {
    int v;
    func_80144220(object);
    v = object->field_46 - 1;
    object->field_46 = v;
    if (v & 0x80) {
        object->pos_x = 0xc0;
        object->field_06++;
    }
}

void func_80144b14(Object *object) {
    if ((s16)object->field_3a < 0) {
        if ((u8)func_801441c8(object) != 0 || game_state.config->field_a6 == 0) {
            func_801449fc(object);
        } else {
            object->field_06 = 3;
            func_80144ee4(object, 0xe);
        }
    }
}
