/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_state_3_special_input_or_default_slot04_09(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    if (obj->direction_index == 2 && obj->field_25f == 0 && (obj->input_bits_2 & 0x8000) != 0) {
        obj->state_3 = 2;
        reset_direction_and_action_fields(obj);
        obj->field_225 = 1;
        if (obj->counter == 0) {
            obj->field_225 = 0;
            add_to_meter_clamped(obj, 1);
        }
        play_side_sound_at(obj, obj->side, 9);
        obj->field_278 = 1;
        set_sequence_by_side_and_mode(obj, 0x2b);
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}
