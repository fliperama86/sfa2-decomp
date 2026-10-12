/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fn_table_8017cb00 holds this function and is declared with this parameter. */
void func_801476d8(Object *p) {
    func_8011f240((Slab172 *)p);
}

void func_801476f8(Object *object) {
    if (((0x1e >> object->table_index) & 1) == 0) {
        object->countdown = 0;
    }
    if (object->active_state == 0) {
        object->countdown = 0;
    } else if (object->active_state == 2) {
        object->countdown = (get_random_byte() & 7) | 8;
    } else if (object->active_state == 4) {
        object->countdown = (get_random_byte() & 7) + 4;
    } else if (object->active_state == 6) {
        object->countdown = (get_random_byte() & 7) + 2;
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
