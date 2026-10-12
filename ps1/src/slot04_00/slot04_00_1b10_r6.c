/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801bfe7c_slot04_00[];

void advance_state_3_start_sequence_with_effects_a_slot04_00(Object *obj) {
    obj->state_3++;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x2a);
}

void update_sequence_spawn_on_flag_high_byte_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_active_ff(obj, 0x1b, 0x3c);
    }
    update_sequence(obj);
}

void update_sequence_set_other_lock_from_table_slot04_00(Object *obj) {
    int i = 0;

    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801bfe7c_slot04_00[i];
    }
}
