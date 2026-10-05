/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Object *func_8011f1e0(void);

void func_80152b40(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 2; i++) {
        if (data_801a27cc != 0) {
            o = func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 10;
                o->field_03 = i * 4;
                o->field_0e = object->field_0e;
                o->field_1c = object->field_1c;
            o->field_46 = object->kind;
                    o->field_48 = 0;
                o->field_0d = 0;
                o->field_90 = (void *)0x800fb100;
                o->field_7a = 0x60;
                o->field_7c = 0x1e0;
                o->field_98 = &data_80172a48;
                o->field_3c = object;
                o->field_9c = &data_80173c9c;
                data_801a27cc--;
            }
        }
    }
}

void func_80152c34(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 2; i++) {
        if (data_801a27cc != 0) {
            o = func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 10;
                o->field_03 = i * 4;
                o->field_0e = object->field_0e;
                o->field_1c = object->field_1c;
            o->field_46 = object->kind;
                    o->field_48 = 2;
                o->field_0d = 0;
                o->field_90 = (void *)0x800fb100;
                o->field_7a = 0x60;
                o->field_7c = 0x1e0;
                o->field_98 = &data_80172a48;
                o->field_3c = object;
                o->field_9c = &data_80173c9c;
                data_801a27cc--;
            }
        }
    }
}

void func_80152d28(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 3; i++) {
        o = func_8011f1e0();
        if (o != 0) {
            o->field_00 = 1;
            o->field_02 = 10;
            o->field_03 = i * 4;
            o->field_0e = object->field_0e;
            o->field_1c = object->field_1c;
            o->field_46 = object->kind;
            o->field_3c = object;
            o->field_48 = 4;
            o->field_0d = 0;
            o->field_90 = (void *)0x800fb100;
            o->field_7a = 0x60;
            o->field_7c = 0x1e0;
            o->field_98 = &data_80172a48;
            o->field_9c = &data_80173c9c;
        }
    }
}

void func_80152df4(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 2; i++) {
        if (data_801a27cc != 0) {
            o = func_8011f1e0();
            if (o != 0) {
                o->field_00 = 1;
                o->field_02 = 10;
                o->field_03 = i * 4;
                o->field_0e = object->field_0e;
                o->field_1c = object->field_1c;
            o->field_46 = object->kind;
                    o->field_48 = 6;
                o->field_0d = 0;
                o->field_90 = (void *)0x800fb100;
                o->field_7a = 0x60;
                o->field_7c = 0x1e0;
                o->field_98 = &data_80172a48;
                o->field_3c = object;
                o->field_9c = &data_80173c9c;
                data_801a27cc--;
            }
        }
    }
}
