/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3a5c_slot04_03(Object *obj);

void func_801b3010_slot04_03(Object *obj) {
    int a = -6;
    int b = 0x4c;

    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3++;
        if (obj->active_state != 0) {
            a = -2;
            b = 0x4e;
        }
        spawn_effect_objects_at_offset(obj, a, b);
        reset_side_table_and_fields(obj);
        spawn_object_type_6d_for_side(obj);
        set_other_sequence_3_when_close(obj);
        func_801b3a5c_slot04_03(obj);
    }
    update_sequence(obj);
}
