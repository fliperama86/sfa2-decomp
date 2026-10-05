/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014b13c(Object *object) {
    func_8014e7ec(object);
    object->field_21c = *data_80189460++;
    data_80189464 = data_80189464 - object->field_21e;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 5;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_48 = 0;
        object->field_21a = 0;
    }
}

void func_8014b1dc(Object *object) {
    func_8014e7ec(object);
    object->field_21c = *data_80189460++;
    data_80189464 = data_80189464 - object->field_21e;
    if ((s16)data_80189464 < 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 6;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_48 = 1;
        object->field_21a = 1;
    }
}

void func_8014b27c(Object *object) {
    u16 old;
    u16 value;
    func_8014e7ec(object);
    value = *data_80189460++;
    old = data_80189464;
    object->field_21c = value;
    data_80189464 = old - object->field_21e + 3;
    if (data_80189464 < 6) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 7;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_48 = 0;
        object->field_21a = 0;
        if (old >= object->field_21e) {
            object->field_48 = 1;
            object->field_21a = 1;
        }
    }
}
