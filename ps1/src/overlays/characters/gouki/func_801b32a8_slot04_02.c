/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void func_801b359c_slot04_02(Object *obj);
void move_x_with_accel_until_speed_limit_slot04_02(Object *obj);

void func_801b32a8_slot04_02(Object *obj) {
    int n;

    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    obj->field_29c = 2;
    obj->field_252 = 0xff;
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->accel_x = -0x3000;
    if (obj->input_dir_second != 0) {
        obj->vel_x = 0x80000;
    } else {
        obj->vel_x = 0xb8000;
    }
    if (obj->field_49 != 0) {
        n = 0x64;
        obj->field_252 = 0;
    } else {
        n = 0x2d;
    }
    set_sequence_by_side_and_mode(obj, n);
    if (obj->field_7e == 0) {
        spawn_three_child_objects(obj);
        func_801b359c_slot04_02(obj);
    }
}

void update_step_offset_then_start_slide_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        play_side_sound_at(obj, obj->side, 0xc);
        move_x_with_accel_until_speed_limit_slot04_02(obj);
    } else {
        obj->pos_x = obj->pos_x + (s8)*(u8 *)&obj->step_flags;
        update_sequence(obj);
    }
    if (obj->field_7e == 0) {
        func_801b359c_slot04_02(obj);
    }
}
