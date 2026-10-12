/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *object);
extern ObjectFn data_801dd4ac_slot05_06[];

void call_effect_on_step_flag_byte_2_slot05_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        spawn_offset_effect_and_play_sounds(obj);
    }
    update_sequence(obj);
}

void func_801cbed0_slot05_06(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_3++;
        obj->vel_x = 0xa0000;
        obj->accel_x = -0xc000;
        set_sequence_by_side_and_mode(obj, 0x1a);
    } else {
        update_sequence(obj);
    }
}

void update_sequence_with_step_sound_and_move_slot05_06(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (t == 3) {
            play_sound_at(obj, obj->side, 0x324);
        }
        if ((s16)obj->step_flags == 2 && obj->vel_x >= 0) {
            move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
        }
        update_sequence(obj);
    }
}

void func_801cbfb4_slot05_06(Object *obj) {
    data_801dd4ac_slot05_06[obj->state_3](obj);
}
