/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_two_offset_child_objects_slot04_09(Object *obj);

void start_state_2_spawn_two_children_slot04_09(Object *obj) {
    obj->active_state = 1;
    obj->state_2 = obj->state_2 + 1;
    spawn_two_offset_child_objects_slot04_09(obj);
    play_side_sound_at(obj, obj->side, 0x10);
    set_sequence_by_side(obj, 0x22);
}

void reset_states_on_sequence_end_slot04_09(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->ground_y;
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}
