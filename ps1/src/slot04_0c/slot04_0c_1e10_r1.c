/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bda28_slot04_0c[];
extern s32 data_801bda44_slot04_0c[];

void spawn_offset_effect_and_play_sounds(Object *object);
void func_80145d20(Object *object);
void func_801428e4(Object *object);

void func_801b1e10_slot04_0c(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        spawn_offset_effect_and_play_sounds(obj);
        obj->countdown = 0x40;
    }
    update_sequence(obj);
}

void func_801b1e70_slot04_0c(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1ebc_slot04_0c(Object *obj) {
    data_801bda28_slot04_0c[obj->state_3](obj);
}

void start_sequence_25_with_table_vel_x_slot04_0c(Object *obj) {
    s32 v;
    obj->state_3 = obj->state_3 + 1;
    v = data_801bda44_slot04_0c[obj->direction_index >> 1];
    obj->accel_x = -0x8000;
    obj->vel_x = v;
    func_80145d20(obj);
    func_801428e4(obj);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x25);
}
