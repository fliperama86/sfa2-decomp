/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int apply_velocity_and_acceleration_slot04_17(Object *obj);
void latch_pressed_bits_into_input_bits_slot04_17(Object *obj);

void update_rise_latch_input_land_or_switch_sequence_slot04_17(Object *obj) {
    latch_pressed_bits_into_input_bits_slot04_17(obj);
    apply_velocity_and_acceleration_slot04_17(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3 = 4;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x4c);
    } else if (obj->ground_y - 0x8a >= obj->pos_y) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x50);
    } else {
        update_sequence(obj);
    }
}
