/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012dbd4(Object *object) {
    if ((u8)func_80130184(object) != 0) {
        update_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
        object->active_state = 0;
        object->pos_y = object->ground_y;
        *(u32 *)&object->pos_y_frac &= 0xffff0000;
        func_801209c4(object);
    }
}

void func_8012dc58(Object *object) {
    if ((s16)object->step_flags >= 0 || (object->countdown = (s16)object->countdown - 1, (s16)object->countdown != 0)) {
        *(s32 *)&object->pos_x_frac += object->vel_x;
        update_sequence(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->step_applied_flag = 0;
        object->flag_166_set = 0;
        if ((s16)object->health < 0) {
            func_8012f4dc(object);
        } else {
            object->field_157 = 1;
            object->facing_side = object->saved_facing;
        }
    }
}

void func_8012dd00(Object *object) {
    object->countdown = (s16)object->countdown - 1;
    if ((s16)object->countdown == 0) {
        object->state_3 = 5;
        object->flag_166_set = 0;
        if ((s16)object->health < 0) {
            func_8012f4dc(object);
        } else {
            func_801308c4(object, 0x19);
        }
    }
}

void func_8012dd5c(Object *object) {
    if (object->hit_flag == 0) {
        if ((u8)func_8013cac8(object, 0xf, 0xd) == 0) {
            func_8012dde4(object);
        } else if (func_80141c4c(object) != 0) {
            object->state_0 = 1;
            object->state_1 = 8;
            object->state_2 = 0;
            object->state_3 = 0;
            object->state_change_flag = 0;
            func_80142b3c(object);
        }
    }
}

void func_8012dde4(Object *object) {
    if (object->kind == 6 && (u8)func_8013cac8(object, 0xe, 8) != 0 && func_80141c4c(object) != 0) {
        object->state_0 = 1;
        object->state_1 = 8;
        object->state_2 = 0;
        object->state_3 = 0;
        object->state_change_flag = 1;
        func_80142b3c(object);
    }
}

void func_8012de5c(Object *object) {
    table_80171a60[object->state_3](object);
}
