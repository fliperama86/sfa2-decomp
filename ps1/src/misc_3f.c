/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013f2a8(Object *object, int index) {
    object->slots[(u8)index].field_00 = 0;
    data_80188f44 = 0;
}

void func_8013f2c8(void) {
    data_80188f44 = 0;
}

void func_8013f2d8(Object *object, int index) {
    object->slots[(u8)index].field_00 = 0;
    data_80188f44 = 1;
}

void func_8013f2fc(Object *object, int index) {
    Slot *slot = &object->slots[(u8)index];
    int level;
    u16 raw = slot->field_02;
    s16 value;
    if (object->field_7e == 0) {
        value = raw;
        if (value < 0x79) level = 0;
        else if (value < 0xf1) level = 2;
        else if (value < 0x1e1) level = 4;
        else if (value < 0x3c1) level = 6;
        else level = 8;
    } else {
        value = raw;
        if (value < 0xb) level = 0;
        else if (value < 0x10) level = 2;
        else if (value < 0x15) level = 4;
        else if (value < 0x1a) level = 6;
        else level = 8;
    }
    object->slots[(u8)index].field_05 = level;
    object->slots[(u8)index].field_00 = 0;
    data_80188f44 = 1;
}

void func_8013f3b8(Object *object) {
    if (object->side != 0) ring_right[0] = 1;
    else ring_left[0] = 1;
}
