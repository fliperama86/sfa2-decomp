/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014b13c(Object *object) {
    func_8014e7ec(object);
    object->countdown_3 = *data_80189460++;
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 5;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 0;
        object->direction_flag = 0;
    }
}

void func_8014b1dc(Object *object) {
    func_8014e7ec(object);
    object->countdown_3 = *data_80189460++;
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 < 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 6;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 1;
        object->direction_flag = 1;
    }
}

void func_8014b27c(Object *object) {
    u16 old;
    u16 value;
    func_8014e7ec(object);
    value = *data_80189460++;
    old = data_80189464;
    object->countdown_3 = value;
    data_80189464 = old - object->compare_limit + 3;
    if (data_80189464 < 6) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 7;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 0;
        object->direction_flag = 0;
        if (old >= object->compare_limit) {
            object->variant_index = 1;
            object->direction_flag = 1;
        }
    }
}
