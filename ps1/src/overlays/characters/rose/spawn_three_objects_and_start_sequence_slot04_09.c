/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_three_pooled_objects_slot04_09(Object *obj);

void spawn_three_objects_and_start_sequence_slot04_09(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_pooled_objects_slot04_09(obj);
    set_sequence_by_side_and_mode(obj, 0x2f);
}

void update_hit_sequence_then_reset_on_flag_slot04_09(Object *obj) {
    u16 t;
    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t == 0) {
        obj->field_165 = 0;
        obj->state_3++;
        reset_object_to_state_1_0_sequence_0(obj);
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0xe, 0x50);
    }
}
