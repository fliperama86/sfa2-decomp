/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void add_vel_x_and_accel_x_to_position_words_slot0b(Object *obj);

void update_slot0b_countdown_then_snap_pos_x_slot0b(Object *obj) {
    if (data_80190468.p->phase_state != 1) {
        { int t = obj->countdown - 1; obj->countdown = t; }
        if ((s16)obj->countdown >= 0) goto tail;
    }
    obj->state_1++;
    obj->pos_x = obj->health;
tail:
    add_vel_x_and_accel_x_to_position_words_slot0b(obj);
}

void func_801e1a54_slot0b(Object *obj) {
    if (data_80190468.p->phase_state != 2) return;
    obj->countdown = 0x10;
    obj->vel_x = -0x80000;
    obj->accel_x = 0;
    obj->state_1++;
    if (obj->variant_index != 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
}

void count_down_then_advance_state_0_slot0b(Object *obj) {
    { int t = obj->countdown - 1; obj->countdown = t; }
    if ((s16)obj->countdown < 0) {
        obj->state_0++;
    }
    add_vel_x_and_accel_x_to_position_words_slot0b(obj);
}

void release_object_record_4_slot0b(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
