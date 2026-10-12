/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0564_slot04_00[];

void update_countdown_2_then_set_sequence_slot04_00(Object *obj) {
    if (obj->vel_table_index != 4 || obj->countdown_2 == 0) {
        update_object_motion_unless_paused(obj);
    } else {
        obj->countdown_2--;
        if (obj->countdown_2 != 0) {
            update_object_motion_unless_paused(obj);
        } else {
            obj->image_row--;
            func_80138070(obj, 7);
        }
    }
}

void call_handler_by_state_2_unless_paused_slot04_00(Object *obj) {
    if ((game_state.config->pause_flag_b | game_state.config->pause_flag_a) != 0) {
        func_8011ffdc(obj);
    } else {
        data_801c0564_slot04_00[obj->state_2](obj);
        func_8011ffdc(obj);
    }
}
