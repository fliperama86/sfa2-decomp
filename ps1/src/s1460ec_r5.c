/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801476d8(void) {
    func_8011f240();
}

void func_801476f8(Object *object) {
    if (((0x1e >> object->field_03) & 1) == 0) {
        object->field_46 = 0;
    }
    if (object->field_45 == 0) {
        object->field_46 = 0;
    } else if (object->field_45 == 2) {
        object->field_46 = (func_80151184() & 7) | 8;
    } else if (object->field_45 == 4) {
        object->field_46 = (func_80151184() & 7) + 4;
    } else if (object->field_45 == 6) {
        object->field_46 = (func_80151184() & 7) + 2;
    }
}

void func_801477ac(int unused, u8 x, u8 y) {
    Rect rect;
    rect.x = (x + 9) << 4;
    rect.y = y + 0x1e0;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, data_801a37c4);
    func_80137220(3, 2);
}
