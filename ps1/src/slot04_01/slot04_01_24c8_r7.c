/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_bytes_slot04_01(Object *object);

void update_sequence_spawn_effects_on_flag_slot04_01(Object *obj) {
    int a = -0x24;
    int b = 0x55;

    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3++;
        if (obj->active_state != 0) {
            a = -5;
            b = 0x45;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot04_01(obj);
    }
    update_sequence(obj);
}
