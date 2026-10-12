/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_3_pick_sequence_by_direction_slot04_0a(Object *obj) {
    int a;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    a = 0x15;
    if (obj->variant_index == 0) {
        a = 0xf;
    }
    if (obj->input_dir_second == 0) {
        a -= 3;
        if (obj->direction_index != 0 && obj->pos_y < obj->ground_y - 0x30 && (obj->input_bits_2 & 0xe000) != 0 && (u8)check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 4;
            return;
        }
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + a));
}
