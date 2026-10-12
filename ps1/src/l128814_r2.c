/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void update_object_timers_and_state_2_handler(Object *object) {
    func_80129bb8(object);
    decrement_meter_clamped_at_zero(object);
    if (object->field_6b != 0) {
        object->field_6b--;
        if (object->kind == 4) func_80132070(object);
        if (object->kind == 0x12) func_80132070(object);
    } else {
        func_80129b68(object);
        func_80129c40(object);
        func_80129b90(object);
        decrement_cooldown_timer(object);
        if (object->mode_flag_2 != 0 && object->field_256 == 0 &&
            (game_state.config->mode_flag_c | game_state.config->mode_flag_a) == 0 &&
            object->state_2 != 9) {
            if (*(u16 *)&object->state_0 != 1 || object->state_2 != 7 ||
                ((object->kind != 6 && object->kind != 0xf) || object->field_15a != 6)) {
                func_8014a1ec(object);
            }
        }
        handler_table_1960[object->state_2](object);
    }
    clamp_pos_x_to_bounds_and_push_other(object);
}
