/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;
extern u8 data_801c3c24_slot04_08[];

void push_other_x_if_hit_check_slot04_08(Object *obj);

void start_move_with_fixed_velocity_and_target_x_slot04_08(Object *obj) {
    u16 a;
    int n;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    *(s32 *)&obj->vel_x = 0x80000;
    obj->accel_x = -0xc000;
    a = *(u16 *)(data_801c3c24_slot04_08 + (obj->input_dir_second & 0xfe));
    if (obj->facing_side == 0) {
        a = -a;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    data_801c3e98_slot04_08 = a + obj->pos_x;
    n = 0x23;
    if (obj->field_49 != 0) {
        n = 0x6b;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + n);
}

void advance_state_3_on_step_flag_byte_slot04_08(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = 0;
        obj->state_3++;
        push_other_x_if_hit_check_slot04_08(obj);
    }
    update_sequence(obj);
}
