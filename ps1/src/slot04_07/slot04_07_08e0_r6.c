/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_dash_or_alternate_by_input_slot04_07(Object *obj) {
    u8 one;

    obj->state_3++;
    one = 1;
    obj->active_flag_3 = one;
    ((Slot04aObj *)obj)->field_1ca = 0;
    if (obj->direction_index != 4 || obj->field_25f != 0) {
        set_sequence_and_tables_from_direction(obj);
    } else if ((obj->input_bits_2 & 0x8000) == 0) {
        obj->state_3 = 2;
        play_sound_at(obj, obj->side, 0x324);
        obj->vel_x = 0x90000;
        obj->accel_x = -0x8000;
        set_sequence_and_tables_from_direction(obj);
        obj->field_278 = one;
    } else {
        obj->state_3 = 3;
        add_to_meter_clamped(obj, 1);
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, 0x2d);
    }
}
