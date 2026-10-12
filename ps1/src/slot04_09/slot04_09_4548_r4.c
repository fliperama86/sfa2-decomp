/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_bytes_slot04_09(Object *obj);

void start_state_with_sound_7_and_sequence_slot04_09(Object *obj) {
    int a = 0x2b;
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    play_side_sound_at(obj, obj->side, 7);
    if (obj->active_state != 0) {
        a = 0x2f;
    }
    set_sequence_by_side(obj, a);
}

void func_801b48d4_slot04_09(Object *obj) {
    int a;
    if ((s16)obj->step_flags & 0xff00) {
        a = 0xa;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -7;
        }
        spawn_effect_objects_at_offset(obj, a, 0x50);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot04_09(obj);
    }
    update_sequence(obj);
}
