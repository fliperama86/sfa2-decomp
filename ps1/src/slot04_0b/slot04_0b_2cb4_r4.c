/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_slots_bytes_slot04_0b(Object *obj);

void func_801b3268_slot04_0b(Object *obj) {
    int a = -0x24;
    int b = 0x64;

    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3++;
        if (obj->active_state != 0) {
            a = -7;
            b = 0x5f;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        clear_slots_bytes_slot04_0b(obj);
    }
    update_sequence(obj);
}
