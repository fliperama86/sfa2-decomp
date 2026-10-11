/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1478_slot04_05[];

void func_801b1870_slot04_05(Object *obj) {
    int a = 0x22;

    obj->state_3++;
    obj->field_17b = 1;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->vel_x = data_801c1478_slot04_05[obj->field_12a * 2];
    obj->vel_y = data_801c1478_slot04_05[obj->field_12a * 2 + 1];
    obj->accel_x = data_801c1478_slot04_05[obj->field_12a * 2 + 2];
    obj->accel_y = data_801c1478_slot04_05[obj->field_12a * 2 + 3];
    if (obj->field_49 != 0) {
        a = 0x35;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}

void func_801b196c_slot04_05(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        obj->field_45 = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0x10);
    }
}
