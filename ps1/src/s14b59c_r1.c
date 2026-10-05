/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_8014b870, func_8014b8d8 (and r2 func_8014be78, func_8014bee8): the original computes
   field_212 as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014b59c(Object *object) {
    if (object->field_20f == 0) {
        u8 v;
        object->field_208 = 3;
        object->field_209 = 0;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 5;
        object->field_07 = 0;
        object->field_159 = 1;
        object->field_21b = 0;
        v = object->field_158;
        object->field_67 = 0;
        object->field_0b = v;
        func_8014e7ec(object);
        func_8014dd14(object);
    } else if (object->field_20f == 2) {
        u8 v;
        object->field_208 = 3;
        object->field_209 = 1;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 5;
        object->field_07 = 0;
        object->field_159 = 1;
        object->field_21b = 1;
        v = object->field_158;
        object->field_67 = 0;
        object->field_0b = v;
        func_8014e7ec(object);
        func_8014dd14(object);
    }
}

void func_8014b650(Object *object) {
    if (object->field_20f == 0) {
        func_8014e7ec(object);
        func_8014dd14(object);
        if (func_8014dcc0(object)) {
            if (*(u32 *)&object->field_04 == 0x20101) {
                object->field_bd = 2;
            }
            object->field_208 = 4;
            object->field_209 = 0;
            object->field_04 = 1;
            object->field_05 = 0;
            object->field_06 = 7;
            object->field_07 = 0;
            object->field_159 = 1;
            object->field_67 = 0;
            object->field_157 = 0;
            object->field_0b = object->field_158;
        } else {
            func_8014c914(object);
        }
    }
}

void func_8014b704(Object *object) {
    if (object->field_20f == 0) {
        func_8014e7ec(object);
        func_8014dd14(object);
        if (func_8014dcc0(object)) {
            if (*(u32 *)&object->field_04 == 0x20101) {
                object->field_bd = 2;
            }
            object->field_208 = 5;
            object->field_209 = 0;
            object->field_04 = 1;
            object->field_05 = 0;
            object->field_06 = 8;
            object->field_07 = 0;
            object->field_159 = 1;
            object->field_67 = 0;
            object->field_157 = 0;
            object->field_0b = object->field_158;
        } else {
            func_8014c914(object);
        }
    }
}

void func_8014b7b8(Object *object) {
    if (object->field_20f == 0) {
        object->field_224++;
        func_8014e7ec(object);
        table_8017d030[(s16)data_80189464 >> 1](object);
    } else if (object->field_20f == 2) {
        func_8014c528(object);
    } else if (object->field_20f == 4) {
        object->field_224--;
        func_8014c914(object);
    }
}
