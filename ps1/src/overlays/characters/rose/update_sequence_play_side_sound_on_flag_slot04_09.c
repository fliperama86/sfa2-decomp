/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_linked_pooled_object_health_2_slot04_09(Object *obj);

void update_sequence_play_side_sound_on_flag_slot04_09(Object *obj) {
    if (obj->active_flag != 0) {
        obj->active_flag = 0;
        obj->other->field_6b = 4;
    }
    if (obj->step_flags & 0xf) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        play_side_sound_at(obj, ((Slot04aObj *)obj)->side, 4);
    }
    update_sequence(obj);
}

void update_sequence_spawn_linked_on_flag_slot04_09(Object *obj) {
    if (obj->step_flags & 0xf) {
        obj->state_3++;
        spawn_linked_pooled_object_health_2_slot04_09(obj);
    }
    update_sequence(obj);
}
