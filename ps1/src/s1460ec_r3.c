/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801466ec(Object *object) {
    fn_table_8017ca28[object->field_04](object);
}

void func_8014672c(Object *object) {
    object->field_7a = 0x60;
    object->field_7c = 0x1e0;
    object->field_90 = (void *)0x800fb100;
    object->field_98 = data_80172a48;
    object->field_9c = data_80173c9c;
    object->field_04 = object->field_04 + 1;
    func_80130768(object, object->field_48, table_8017c7f8);
}

void func_80146794(Object *object) {
    if (object->field_03 >= 0x10) {
        if (object->field_03 == 0x10) {
            object->pos_x = object->pos_x + 4;
        } else if (object->field_03 == 0x11) {
            object->pos_x = object->pos_x - 4;
        }
    }
    if ((s16)object->field_3a < 0) {
        object->field_04 = object->field_04 + 1;
    }
    func_80131094(object);
    func_80120028();
}

void func_80146820(void) {
    func_8011f240();
}

u8 func_80146840(Object *object) {
    return func_80146e3c(object, 0);
}

u8 func_80146864(Object *object) {
    return func_80146e3c(object, 1);
}

int func_80146888(Object *object) {
    u8 r = func_80146e3c(object, 2);
    return r;
}

u8 func_801468ac(Object *object) {
    return func_80146e3c(object, 4);
}

u8 func_801468d0(Object *object) {
    return func_80146e3c(object, 5);
}

int func_801468f4(Object *object) {
    u8 r = func_80146ba0(object, 2);
    return r;
}

u8 func_80146918(Object *object) {
    return func_80146ba0(object, 4);
}

int func_8014693c(Object *object) {
    u8 r = func_80146ba0(object, 5);
    return r;
}

void func_80146960(Object *object) {
    func_80146cf8(object, 0);
    func_80146cf8(object, 1);
}
