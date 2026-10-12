/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void land_reset_fields_and_choose_next_state(Object *object);
extern s16 data_801c4a18_slot04_12[];
/* functions of other units of this module */
int move_by_velocity_return_new_vel_y_slot04_12(Object *obj);
void init_jump_velocity_by_screen_position_slot04_12(Object *obj);

void set_sequence_by_direction_and_variant_slot04_12(Object *obj) {
    s16 a = 0xc;

    add_to_meter_clamped(obj, *(s16 *)((u8 *)data_801c4a18_slot04_12 + (obj->direction_index & 0xfe)));
    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (obj->input_dir_second != 0) {
        a += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)(a + (obj->direction_index >> 1)));
}

void update_jump_flag_start_or_land_on_ground_slot04_12(Object *obj) {
    if (obj->step_limit == 0) {
        if (obj->active_flag != 0) {
            init_jump_velocity_by_screen_position_slot04_12(obj);
        } else if (move_by_velocity_return_new_vel_y_slot04_12(obj) < 0 && !(obj->pos_y < obj->ground_y)) {
            play_object_sound_by_kind(obj);
            land_reset_fields_and_choose_next_state(obj);
        } else {
            update_sequence(obj);
        }
    }
}
