/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b4be8_slot04_04(Object *obj);
void init_jump_velocity_by_scroll_position_slot04_04(Object *obj);

void set_state_5_and_start_variant_sequence_slot04_04(Object *obj) {
    int t = 0x1c;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 5;
    obj->state_3 = 0;
    add_to_meter_clamped(obj, 1);
    obj->step_limit = 0;
    obj->active_flag = 0;
    if (obj->variant_index != 0) {
        t = 0x1d;
    }
    set_sequence_by_side_and_mode(obj, t);
}

void func_801b4a40_slot04_04(Object *obj) {
    if (obj->step_limit == 0) {
        if (obj->active_flag != 0) {
            init_jump_velocity_by_scroll_position_slot04_04(obj);
        } else if (func_801b4be8_slot04_04(obj) < 0 && !(obj->pos_y < obj->ground_y)) {
            play_object_sound_by_kind(obj);
            func_80131638(obj);
        } else {
            update_sequence(obj);
        }
    }
}
