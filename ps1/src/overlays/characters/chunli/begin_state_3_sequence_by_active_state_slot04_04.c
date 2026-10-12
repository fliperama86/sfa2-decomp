/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_slots_81_bytes_slot04_04(Object *obj);

void begin_state_3_sequence_by_active_state_slot04_04(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        set_sequence_by_side(obj, 0x2f);
    } else {
        play_side_sound_at(obj, obj->side, 8);
        set_sequence_by_side(obj, 0x2b);
    }
}

void update_sequence_trigger_effects_on_flag_slot04_04(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -3;
            b = 0x50;
        } else {
            a = -7;
            b = 0x47;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_81_bytes_slot04_04(obj);
    }
    update_sequence(obj);
}
