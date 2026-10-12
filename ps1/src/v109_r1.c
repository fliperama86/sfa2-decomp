/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (v14 retry): 244 vs 252 bytes. Original ends with if (flag) {...; a1 = 0xe} else a1 = s1 (state kept in $s1, 0xe loaded into $a1). Here the 0xc is constant-propagated or both paths share one register. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_pos_x_table_step_then_pick_sequence(Object *o) {
    u16 state;
    if (apply_table_step_to_pos_x(o)) {
        o->state_3++;
        o->facing_side = (o->facing_flag ^ 1) & 1;
        set_state_1_1_2_0_clear_action_timer(o);
        return;
    }
    if ((s16)o->health >= 0) {
        if (o->mode_flag_2) {
            select_sequence_by_meter_or_other_field(o);
        } else if ((o->input_bits_2 & 0x2000) == 0) {
            update_sequence(o);
        } else {
            o->field_157 = 0;
            state = 0xc;
        }
    }
    if ((o->pressed_bits & 0x4000) == 0) {
        set_sequence_by_side_and_mode_2(o, state);
        return;
    }
    o->field_157++;
    o->action_timer = 0;
    o->field_60 = 0;
    state = 0xe;
    set_sequence_by_side_and_mode_2(o, state);
}
