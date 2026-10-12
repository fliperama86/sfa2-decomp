/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_and_trailing_byte_arrays_slot04_08(Object *obj);

void trigger_spawns_on_step_flag_high_byte_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3++;
        spawn_effect_objects_at_offset(obj, -0xb, 0x6a);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_and_trailing_byte_arrays_slot04_08(obj);
    }
    update_sequence(obj);
}
