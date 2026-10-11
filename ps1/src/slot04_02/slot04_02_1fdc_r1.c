/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6300_slot04_02[];
extern s32 data_801c630c_slot04_02[];
extern u16 data_801c6318_slot04_02[];

void func_801b1fdc_slot04_02(Object *obj) {
    int i = 1;

    obj->field_17b = i;
    obj->field_45 = i;
    obj->state_3 = i;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    i = obj->field_12a >> 1;
    *(s32 *)&obj->vel_y = 0x40000;
    obj->accel_y = -0x6000;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    if (obj->field_49 != 0) {
        obj->accel_y = data_801c6300_slot04_02[i];
    }
    if (obj->field_0b != 0) {
        obj->vel_x = data_801c630c_slot04_02[i];
    } else {
        obj->vel_x = -data_801c630c_slot04_02[i];
    }
    func_801307e0(obj, data_801c6318_slot04_02[i]);
}
