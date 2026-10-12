/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_facing_from_input_start_sequence_with_meter_slot04_0a(Object *obj) {
    obj->state_3++;
    if (obj->mode_flag_2 != 0 ? (u16)(box_margin[0] + 0xc0) < obj->pos_x : (obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 0;
    } else {
        obj->facing_side = 1;
    }
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    add_to_meter_clamped(obj, 3);
    set_sequence_by_side_and_mode(obj, 0x18);
}
