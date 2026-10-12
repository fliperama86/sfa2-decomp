/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_children_per_direction_index(Object *, int, int);

void spawn_direction_children_on_low_flag_byte_a_slot04_0c(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        spawn_children_per_direction_index(obj, -0x10, 0x2e);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}
