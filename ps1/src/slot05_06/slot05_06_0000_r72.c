/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd6a0_slot05_06[];
extern ObjectFn data_801dd6c0_slot05_06[];
int move_by_velocity_return_y_minus_ground_slot05_06(Object *obj);

void call_handler_by_table_index_unless_gate_flag_slot05_06(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0 = 2;
    } else {
        data_801dd6a0_slot05_06[obj->table_index](obj);
    }
}

void bob_pos_y_and_advance_sequence_slot05_06(Object *obj) {
    int t;
    int a;

    t = obj->variant_index + 1;
    obj->variant_index = t & 0x1f;
    a = 1;
    if (t & 0x10) {
        a = -1;
    }
    obj->pos_y -= a;
    advance_sequence_checked(obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void call_state_1_table_handler_then_cull_check_slot05_06(Object *o) {
    data_801dd6c0_slot05_06[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}

void start_bounce_below_ground_advance_sequence_slot05_06(Object *obj) {
    if ((move_by_velocity_return_y_minus_ground_slot05_06(obj) << 16) > 0) {
        obj->vel_x = 0x2000;
        if ((obj->facing_side ^ (obj->table_index & 1)) != 0) {
            obj->vel_x = -0x2000;
        }
        obj->vel_y = 0x1c000;
        obj->accel_y = -0x1800;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        obj->state_1++;
    }
    advance_sequence_checked(obj);
}
