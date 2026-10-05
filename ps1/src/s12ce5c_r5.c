/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 func_80130184(Object *object);
u8 func_8013cac8(Object *object, int a, int b);
int func_80141c4c(Object *object);

void func_8012dbd4(Object *object) {
    if (func_80130184(object) != 0) {
        func_80130efc(object);
    } else {
        object->field_04 = 1;
        object->field_05 = 1;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_15b = 0;
        object->field_247 = 0;
        object->field_45 = 0;
        object->pos_y = object->field_70;
        *(u32 *)&object->field_14 &= 0xffff0000;
        func_801209c4(object);
    }
}

void func_8012dc58(Object *object) {
    if ((s16)object->field_3a >= 0 || (object->field_46 = (s16)object->field_46 - 1, (s16)object->field_46 != 0)) {
        *(s32 *)&object->field_10 += object->field_4c;
        func_80130efc(object);
    } else {
        object->field_04 = 1;
        object->field_05 = 1;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_15b = 0;
        object->field_247 = 0;
        object->field_166 = 0;
        if ((s16)object->field_5c < 0) {
            func_8012f4dc(object);
        } else {
            object->field_157 = 1;
            object->field_0b = object->field_158;
        }
    }
}

void func_8012dd00(Object *object) {
    object->field_46 = (s16)object->field_46 - 1;
    if ((s16)object->field_46 == 0) {
        object->field_07 = 5;
        object->field_166 = 0;
        if ((s16)object->field_5c < 0) {
            func_8012f4dc(object);
        } else {
            func_801308c4(object, 0x19);
        }
    }
}

void func_8012dd5c(Object *object) {
    if (object->field_260 == 0) {
        if (func_8013cac8(object, 0xf, 0xd) == 0) {
            func_8012dde4(object);
        } else if (func_80141c4c(object) != 0) {
            object->field_04 = 1;
            object->field_05 = 8;
            object->field_06 = 0;
            object->field_07 = 0;
            object->field_25b = 0;
            func_80142b3c(object);
        }
    }
}

void func_8012dde4(Object *object) {
    if (object->kind == 6 && func_8013cac8(object, 0xe, 8) != 0 && func_80141c4c(object) != 0) {
        object->field_04 = 1;
        object->field_05 = 8;
        object->field_06 = 0;
        object->field_07 = 0;
        object->field_25b = 1;
        func_80142b3c(object);
    }
}

void func_8012de5c(Object *object) {
    table_80171a60[object->field_07](object);
}
