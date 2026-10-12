/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_by_velocity_return_y_minus_ground_slot05_06(Object *obj);
extern ObjectFn data_801dd6cc_slot05_06[];

void set_landing_sequence_when_below_ground_slot05_06(Object *obj) {
    int a;
    int t;

    if ((move_by_velocity_return_y_minus_ground_slot05_06(obj) << 16) > 0) {
        t = obj->state_1;
        a = obj->table_index;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        t++;
        a = (a & 1) | 0x18;
        obj->state_1 = t;
        if (obj->sound_side == 0) {
            set_sequence_from_table_5(obj, a, data_1f8000b4);
        } else {
            set_sequence_from_table_5(obj, a, data_1f800164);
        }
    }
    advance_sequence_checked(obj);
}

void noop_slot00_2_slot05_06(Object *obj) {
}

void move_advance_state_0_at_ground_timed_limit_check_slot05_06(Object *obj) {
    GameState *g = &game_state;

    if ((move_by_velocity_return_y_minus_ground_slot05_06(obj) << 16) >= 0) {
        obj->state_0++;
    }
    if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
        ((Slot04aObj *)obj)->timer -= 1;
        if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
            clear_or_set_flag_by_pos_x_limit(obj);
            return;
        }
    }
    if (g->frame_counter & 1) {
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}

void call_state_1_table_handler_then_cull_check_2_slot05_06(Object *o) {
    data_801dd6cc_slot05_06[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}
