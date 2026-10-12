/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot01State55e94 data_80055e94_slot01;
extern Slot01Rec152ec data_800152e4_slot01;
void set_record_cursor_and_spawn_y_slot01(Object *obj, Slot01Rec152ec *a1);

void update_x_shake_until_vel_x_zero_slot01(Object *obj) {
    Object *o = obj;
    int t = o->countdown - 1;
    o->countdown = t;
    if ((s16)t < 0) {
        o->countdown = 1;
        o->vel_x = o->vel_x + o->accel_x;
        if (o->vel_x == 0) {
            o->state_1++;
            ((Object *)o->linked_object)->state_0 = 2;
        }
        o->pos_x = (o->vel_x & 2) ? o->pos_x - o->vel_x : o->pos_x + o->vel_x;
    }
}

void advance_state_0_on_record_value_flag_slot01(Object *obj) {
    if (data_80055e94_slot01.value & 0x8000) {
        obj->state_0++;
    }
}

void init_object_and_activate_linked_object_slot01(Object *obj) {
    obj->state_0++;
    obj->pos_x = box_margin[0] + 5;
    ((Object *)obj->linked_object)->state_0 = 2;
    set_record_cursor_and_spawn_y_slot01(obj, &data_800152e4_slot01);
}

void load_velocities_from_table_slot01(Object *obj) {
    set_flag_and_push_if_mode_8(obj);
}
