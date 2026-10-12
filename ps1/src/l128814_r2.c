/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void update_object_timers_and_state_2_handler(Object *object) {
    count_down_other_lock(object);
    decrement_meter_clamped_at_zero(object);
    if (object->field_6b != 0) {
        object->field_6b--;
        if (object->kind == 4) set_sequence_x28_if_state_7_0_1_and_active(object);
        if (object->kind == 0x12) set_sequence_x28_if_state_7_0_1_and_active(object);
    } else {
        count_down_object_byte_counter(object);
        count_down_hold_timer(object);
        count_down_countdown_4(object);
        decrement_cooldown_timer(object);
        if (object->mode_flag_2 != 0 && object->field_256 == 0 &&
            (game_state.config->mode_flag_c | game_state.config->mode_flag_a) == 0 &&
            object->state_2 != 9) {
            if (*(u16 *)&object->state_0 != 1 || object->state_2 != 7 ||
                ((object->kind != 6 && object->kind != 0xf) || object->field_15a != 6)) {
                dispatch_script_mode_with_one_time_init(object);
            }
        }
        handler_table_1960[object->state_2](object);
    }
    clamp_pos_x_to_bounds_and_push_other(object);
}
