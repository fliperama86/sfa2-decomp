/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *object);

void move_until_ground_then_branch_landing_sequence_slot04_0b(Object *obj) {
    if (move_ballistic_and_check_ground_slot04_0b(obj)) {
        update_sequence(obj);
    } else {
        if (obj->direction_index == 4 && obj->variant_index_2 == 0) {
            obj->state_3 = 6;
            obj->variant_index_2 = 3;
            set_sequence_by_side_and_mode(obj, 0x48);
        } else {
            obj->state_3++;
            obj->active_state = 0;
            obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, 0x2a);
        }
    }
}
