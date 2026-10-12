/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_fx_fy_with_steps_slot0b(Slot0bObj *obj);

void count_down_then_advance_handler_and_move_slot0b(Slot0bObj *obj) {
    int v;
    if (data_80190468.p->phase_state == 2) {
        v = obj->countdown_timer - 1;
        obj->countdown_timer = v;
        if ((s16)v < 0) {
            obj->handler_index++;
        }
        update_fx_fy_with_steps_slot0b(obj);
    }
}

void release_object_record_2_slot0b(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void release_object_record_3_slot0b(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void update_fx_fy_with_steps_slot0b(Slot0bObj *obj) {
    obj->fx += obj->fx_step;
    obj->fx_step += obj->fx_step_accel;
    obj->fy += obj->fy_step;
    obj->fy_step += obj->fy_step_accel;
}
