/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c4180_slot04_04[];
int func_801b3a40_slot04_04(Object *obj);
void init_jump_velocity_by_screen_position_slot04_04(Object *obj);

void set_sequence_by_direction_and_variant_slot04_04(Object *obj) {
    s16 a = 0xc;

    add_to_meter_clamped(obj, *(s16 *)((u8 *)data_801c4180_slot04_04 + (obj->direction_index & 0xfe)));
    if (obj->variant_index != 0) {
        a = 0x12;
    }
    if (obj->input_dir_second != 0) {
        a += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)(a + (obj->direction_index >> 1)));
}

void func_801b0a40_slot04_04(Object *obj) {
    if (obj->step_limit == 0) {
        if (obj->active_flag != 0) {
            init_jump_velocity_by_screen_position_slot04_04(obj);
        } else if (func_801b3a40_slot04_04(obj) < 0 && !(obj->pos_y < obj->ground_y)) {
            play_object_sound_by_kind(obj);
            func_80131638(obj);
        } else {
            update_sequence(obj);
        }
    }
}
