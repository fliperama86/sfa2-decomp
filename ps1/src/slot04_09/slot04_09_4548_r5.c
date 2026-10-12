/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c7de8_slot04_09[];
extern u8 data_801c7de4_slot04_09[];

void func_801b4b04_slot04_09(Object *o) {
    int a = 0x39;
    o->active_flag_4 = 1;
    o->field_225 = 1;
    o->state_3++;
    if (o->counter == 0) {
        o->field_225 = 0;
        func_80141f28(o, 6);
    }
    func_80138ae8(&game_state, o);
    o->vel_y = 0;
    o->accel_y = 0;
    o->vel_x = data_801c7de8_slot04_09[o->direction_index];
    o->accel_x = data_801c7de8_slot04_09[o->direction_index + 1];
    if (o->facing_side != 0) {
        o->vel_x = -o->vel_x;
        o->accel_x = -o->accel_x;
    }
    o->countdown = data_801c7de4_slot04_09[o->direction_index >> 1];
    if (o->field_49 != 0) {
        a = 0x4c;
    }
    func_801307e0(o, (o->direction_index >> 1) + a);
}
