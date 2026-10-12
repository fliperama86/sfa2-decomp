/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801bf120_slot04_01[];

void start_jump_facing_from_input_top_bit_slot04_01(Object *obj) {
    int i = 0;

    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, ((Slot04aObj *)obj)->side ^ 1, 0x31a);
    play_side_sound_at(obj, ((Slot04aObj *)obj)->side, 6);
    if (obj->mode_flag_2 == 0) {
        i = obj->input_bits >> 15;
    }
    obj->vel_x = data_801bf120_slot04_01[i];
    obj->facing_side = i;
    obj->vel_y = 0x20000;
    obj->accel_y = -0x2000;
    set_sequence_by_side_and_mode(obj, 0x19);
}
