/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7908_slot04_14[];
extern ObjectFn data_801c791c_slot04_14[];

void toggle_image_row_call_handler_by_state_1_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    if (o->table_index >= 6) {
        o->countdown_2 ^= 1;
        o->image_row = o->countdown_2 + obj->field_b0;
    }
    data_801c7908_slot04_14[o->state_1](o);
}

void call_handler_by_table_index_and_advance_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c791c_slot04_14[o->table_index >> 1](o);
        advance_sequence_checked(o);
        set_state_0_if_x_outside_margin_narrow(o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}

void apply_velocity_with_inverted_y_slot04_14(Object *o) {
    *(s32 *)&o->pos_x_frac += o->vel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
}
