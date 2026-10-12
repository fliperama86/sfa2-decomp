/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_countdown_spawn_two_on_flag_slot04_02(Object *obj) {
    obj->countdown -= 0x100;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0x1e, 0x30);
        spawn_child_object_with_offsets(obj, -0x2c, 0x30);
    }
    update_sequence(obj);
}
