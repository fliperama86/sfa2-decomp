/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014aa4c(Object *object) {
    if ((*(unsigned *)&object->field_208 & 0xffff0000) == 0) {
        func_8014ab74(object);
    } else {
        func_8014dc30(object);
        if (object->countdown_3 == 0) {
            func_8014c914(object);
        } else {
            object->field_208 = 0;
            object->field_209 = 8;
            set_state_one_unless_set(object);
        }
    }
}

void func_8014aacc(Object *object) {
    if (object->link_flags == 0 || object->linked_object_3 == 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 9;
        set_state_one_unless_set(object);
    }
}

void func_8014ab20(Object *object) {
    if ((u8)func_8014e758(object)) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 10;
        set_state_one_unless_set(object);
    }
}

void func_8014ab74(Object *object) {
    func_8014dc30(object);
    if (object->countdown_3 == 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 0;
        set_state_one_unless_set(object);
    }
}

void func_8014abc8(Object *object) {
    object->countdown_3 = 1;
    object->field_208 = 0;
    object->field_209 = 8;
    set_state_one_unless_set(object);
}

void func_8014abf8(Object *object) {
    u16 value = *data_80189460++;
    object->field_208 = 0;
    object->field_209 = 8;
    object->countdown_3 = value;
    set_state_one_unless_set(object);
}

void func_8014ac3c(Object *object) {
    func_8014e7ec(object);
    object->countdown_3 = *data_80189460++;
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_209 = 0xb;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void func_8014acd0(Object *object) {
    func_8014e7ec(object);
    object->countdown_3 = *data_80189460++;
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 < 0) {
        func_8014c914(object);
    } else {
        object->field_209 = 0xc;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void func_8014ad64(Object *object) {
    if ((u8)func_8014e718(object)) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 0xd;
        set_state_one_unless_set(object);
    }
}

void func_8014adb8(Object *object) {
    if (func_80142030(object)) {
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

void func_8014ae28(Object *object) {
    object->active_flag = 0;
    object->field_208 = 0;
    object->field_209 = 5;
    set_state_one_unless_set(object);
}

void set_state_one_unless_set(Object *object) {
    if ((*(unsigned *)&object->state_0 & 0xffffff) != 1) {
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void func_8014ae88(Object *object) {
    table_8017d010[object->script_arg >> 1](object);
}

void func_8014aecc(Object *object) {
    func_8014e7ec(object);
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->state_0 = 1;
        object->field_209 = 0;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 0;
        object->direction_flag = 0;
    }
}

void func_8014af50(Object *object) {
    func_8014e7ec(object);
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 1;
        object->direction_flag = 1;
    }
}
