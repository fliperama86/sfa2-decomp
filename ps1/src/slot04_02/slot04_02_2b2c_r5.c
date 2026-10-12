/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b359c_slot04_02(Object *object);

void func_801b31a4_slot04_02(Object *obj) {
    int n;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->active_flag_3 = 0;
    obj->variant_index = obj->facing_side ^ obj->direction_flag;
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->accel_x = -0x3000;
    if (obj->input_dir_second != 0) {
        *(s32 *)&obj->vel_x = 0x80000;
    } else {
        *(s32 *)&obj->vel_x = 0xb8000;
    }
    obj->field_252 = 0xff;
    n = 0x2d;
    if (obj->field_49 != 0) {
        obj->field_252 = 0;
        n = 0x64;
    }
    set_sequence_by_side_and_mode(obj, n);
    if (obj->field_7e == 0) {
        spawn_three_child_objects(obj);
        func_801b359c_slot04_02(obj);
    }
}
