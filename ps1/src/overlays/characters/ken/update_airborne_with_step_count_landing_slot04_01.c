/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_with_gravity_and_check_ground_slot04_01(Object *object);
void land_on_ground_set_sequence_slot04_01(Object *object);

void update_airborne_with_step_count_landing_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (move_with_gravity_and_check_ground_slot04_01(o) != 0) {
        land_on_ground_set_sequence_slot04_01(o);
    } else if ((s16)o->step_flags & 0x8000) {
        play_sound_at(o, o->side, 0x320);
        obj->step_count = obj->step_count - 1;
        if (obj->step_count & 0x80) {
            o->state_3++;
            set_sequence_by_side_and_mode(o, 0x21);
        } else {
            update_sequence(o);
        }
    } else {
        update_sequence(o);
    }
}
