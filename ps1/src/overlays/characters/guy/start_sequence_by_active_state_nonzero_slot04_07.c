/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_slots_bytes_slot04_07(Object *obj);

void start_sequence_by_active_state_nonzero_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->active_state != 0) {
        set_sequence_by_side(obj, 0x2f);
    } else {
        set_sequence_by_side(obj, 0x2b);
    }
}

void spawn_effects_on_step_flag_high_byte_slot04_07(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            spawn_effect_objects_at_offset(obj, -2, 0x4a);
        } else {
            spawn_effect_objects_at_offset(obj, -0x1b, 0x69);
        }
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot04_07(obj);
    }
    update_sequence(obj);
}
