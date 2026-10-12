/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_with_sound_and_clear_flags_slot04_07(Object *obj) {
    obj->state_3++;
    ((Slot04aObj *)obj)->once_flag = 0;
    ((Slot04aObj *)obj)->seen_flag = 0;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    play_sound_at(obj, obj->side, 0x31c);
    set_sequence_by_side_and_mode(obj, 0x40);
}

void spawn_pooled_objects_on_step_flag_byte_slot04_07(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_side_sound_at(obj, obj->side, 4);
        obj->countdown |= 0x3800;
        spawn_children_per_direction_index(obj, 0, 0x5c);
    }
    update_sequence(obj);
}
