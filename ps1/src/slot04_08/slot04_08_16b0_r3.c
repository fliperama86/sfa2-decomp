/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;
extern u8 data_801c3c24_slot04_08[];

void func_801b4298_slot04_08(Object *obj);

void func_801b19d0_slot04_08(Object *obj) {
    u16 a;
    int n;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
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
    func_801307e0(obj, (obj->direction_index >> 1) + n);
}

void func_801b1ab0_slot04_08(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = 0;
        obj->state_3++;
        func_801b4298_slot04_08(obj);
    }
    update_sequence(obj);
}
