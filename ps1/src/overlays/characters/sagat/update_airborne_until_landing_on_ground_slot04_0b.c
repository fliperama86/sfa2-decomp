/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *obj);

void update_airborne_until_landing_on_ground_slot04_0b(Object *obj) {
    if ((u8)move_ballistic_and_check_ground_slot04_0b(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x2a);
    }
}
