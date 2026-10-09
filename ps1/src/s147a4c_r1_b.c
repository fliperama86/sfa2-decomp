/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_801482e0(Object *parent, int x_arg, int y_arg) {
    u16 x = x_arg;
    u16 y = y_arg;
    Object *object = (Object *)func_8011f1e0();
    if (object != 0) {
        object->field_00 = 1;
        object->field_02 = 7;
        object->field_03 = 0;
        object->field_0c = 0;
        object->field_0d = 0;
        object->field_45 = 0;
        object->field_48 = 0;
        object->field_0e = parent->field_0e;
        object->field_1c = parent->field_1c;
        object->field_3c = parent;
        object->field_5c = x;
        object->field_5e = y;
        object->field_0b = parent->field_0b;
        object->field_7a = 0x60;
        object->field_7c = 0x1e0;
        object->field_98 = data_80172a48;
        object->field_90 = (void *)0x800fb100;
        object->field_9c = data_80173c9c;
    }
}
