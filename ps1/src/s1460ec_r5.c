/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fn_table_8017cb00 holds this function and is declared with this parameter. */
void func_801476d8(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

void set_countdown_random_by_active_state(Object *object) {
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

void store_image_row_and_queue_job(int unused, u8 x, u8 y) {
    Rect rect;
    rect.x = (x + 9) << 4;
    rect.y = y + 0x1e0;
    rect.w = 0x10;
    rect.h = 1;
    func_80158028(&rect, data_801a37c4);
    queue_image_upload_job(3, 2);
}
