/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014a688(Object *object) {
    if (object->field_157 != 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 1;
        object->state_3 = 0;
    }
}

void func_8014a6d8(Object *object) {
    if (object->field_157 == 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 2;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 1;
        object->state_3 = 2;
    }
}

void func_8014a72c(Object *object) {
    if (object->other->active_state != 1) {
        func_8014c914(object);
    } else {
        object->field_209 = 3;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void func_8014a788(Object *object) {
    Object *other = object->other;
    func_8014e7ec(object);
    if (other->active_state != 1) {
        func_8014c914(object);
    } else {
        data_80189464 = other->ground_y - data_80189464;
        if ((s16)data_80189464 < other->pos_y && other->vel_y > 0) {
            func_8014c914(object);
        } else {
            object->field_209 = 4;
            object->field_208 = 0;
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 0;
            object->state_3 = 0;
        }
    }
}

void func_8014a840(Object *object) {
    if (func_80142378(object)) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        object->facing_side = object->saved_facing;
    } else {
        func_8014c914(object);
    }
}

void func_8014a8b0(Object *object) {
    object->field_208 = 0;
    object->field_209 = 6;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 0;
    object->countdown_3 = *data_80189460++;
    if (func_80142378(object)) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_2 = 5;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        object->facing_side = object->saved_facing;
    }
}
