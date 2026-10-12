/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"
extern s16 data_80055eb2;
extern ObjectFn data_80028e74_slot27[];

void slide_x_right_to_limit_slot27(Object *obj) {
    s16 *p;
    s16 x = obj->pos_x;
    s16 m = *(s16 *)box_margin;
    if (x < m + 0x153) {
        obj->pos_x = x + 8;
        p = &data_80055eb2;
        if (*p > 0x40) {
            *p -= 8;
        }
    } else {
        obj->pos_x = m + 0x153;
        game_state.timer_or_bits |= 1;
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void release_object_to_pool_table_11_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_with_refs_2_slot27(Object *obj) {
    ref_other.p = obj->parent;
    (*(Object **)&data_80190458) = obj->other;
    data_80028e74_slot27[obj->state_0](obj);
}
