/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c0708_slot04_03[];

void set_countdown_and_start_direction_sequence_slot04_03(Object *obj) {
    obj->countdown = 0x300;
    obj->state_3++;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x29);
}

void update_sequence_then_spawn_children_on_step_flag_slot04_03(Object *obj) {
    update_sequence(obj);
    if (obj->step_flags != 0) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0x34, 0x4c);
    }
}

void set_other_lock_and_play_two_sounds_slot04_03(Object *obj) {
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) != 0) {
        u8 i = 0;

        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c0708_slot04_03[i];
        play_side_sound_at(obj, obj->side, 6);
        play_side_sound_at(obj, obj->side, 0xc);
    }
}
