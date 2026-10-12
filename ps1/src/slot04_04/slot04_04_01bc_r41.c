/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_facing_and_countdown_then_start_sequence_slot04_04(Object *obj) {
    obj->state_3++;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    add_to_meter_clamped(obj, 4);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (!((s16)(box_margin[0] + 0xc0) < obj->pos_x)) {
            obj->facing_side = 1;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    obj->countdown = (u8)obj->countdown + 0xc00;
    set_sequence_by_side_and_mode(obj, 0x19);
}

void count_down_high_byte_advance_state_3_slot04_04(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
    }
}
