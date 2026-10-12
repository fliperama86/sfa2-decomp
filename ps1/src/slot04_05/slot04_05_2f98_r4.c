/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_bytes_slot04_05(Object *obj);

void update_sequence_with_effect_spawn_on_step_flag_b_slot04_05(Object *obj) {
    int a;
    if ((s16)obj->step_flags & 0xff00) {
        a = 2;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -1;
        }
        spawn_effect_objects_at_offset(obj, a, 0x50);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot04_05(obj);
    }
    update_sequence(obj);
}
