/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int apply_velocity_and_acceleration_slot04_17(Object *obj);

void update_rise_until_height_then_sound_and_sequence_slot04_17(Object *obj) {
    apply_velocity_and_acceleration_slot04_17(obj);
    if (obj->pos_y <= obj->ground_y - 0x8a) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x2d);
    } else {
        update_sequence(obj);
    }
}
