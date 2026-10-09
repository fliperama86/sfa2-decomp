/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it: the original sets no argument register before that call, so the callee receives what this function's caller passed. No unit of the tree calls this function by name. */
void func_801476d8(Slab172 *p) {
    func_8011f240(p);
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
