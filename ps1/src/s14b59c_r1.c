/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_8014b870, func_8014b8d8 (and r2 func_8014be78, func_8014bee8): the original computes
   field_212 as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014b59c(Object *object) {
    if (object->script_arg == 0) {
        u8 v;
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        v = object->saved_facing;
        object->active_flag = 0;
        object->facing_side = v;
        func_8014e7ec(object);
        func_8014dd14(object);
    } else if (object->script_arg == 2) {
        u8 v;
        object->field_208 = 3;
        object->field_209 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 1;
        v = object->saved_facing;
        object->active_flag = 0;
        object->facing_side = v;
        func_8014e7ec(object);
        func_8014dd14(object);
    }
}

void func_8014b650(Object *object) {
    if (object->script_arg == 0) {
        func_8014e7ec(object);
        func_8014dd14(object);
        if (func_8014dcc0(object)) {
            if (*(u32 *)&object->state_0 == 0x20101) {
                object->effect_request = 2;
            }
            object->field_208 = 4;
            object->field_209 = 0;
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 7;
            object->state_3 = 0;
            object->active_flag_3 = 1;
            object->active_flag = 0;
            object->field_157 = 0;
            object->facing_side = object->saved_facing;
        } else {
            func_8014c914(object);
        }
    }
}

void func_8014b704(Object *object) {
    if (object->script_arg == 0) {
        func_8014e7ec(object);
        func_8014dd14(object);
        if (func_8014dcc0(object)) {
            if (*(u32 *)&object->state_0 == 0x20101) {
                object->effect_request = 2;
            }
            object->field_208 = 5;
            object->field_209 = 0;
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 8;
            object->state_3 = 0;
            object->active_flag_3 = 1;
            object->active_flag = 0;
            object->field_157 = 0;
            object->facing_side = object->saved_facing;
        } else {
            func_8014c914(object);
        }
    }
}

void func_8014b7b8(Object *object) {
    if (object->script_arg == 0) {
        object->field_224++;
        func_8014e7ec(object);
        table_8017d030[(s16)data_80189464 >> 1](object);
    } else if (object->script_arg == 2) {
        func_8014c528(object);
    } else if (object->script_arg == 4) {
        object->field_224--;
        func_8014c914(object);
    }
}
