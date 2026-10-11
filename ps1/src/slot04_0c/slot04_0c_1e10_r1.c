/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bda28_slot04_0c[];
extern s32 data_801bda44_slot04_0c[];

void func_80146998(Object *object);
void func_80145d20(Object *object);
void func_801428e4(Object *object);

void func_801b1e10_slot04_0c(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        func_80146998(obj);
        obj->field_46 = 0x40;
    }
    update_sequence(obj);
}

void func_801b1e70_slot04_0c(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1ebc_slot04_0c(Object *obj) {
    data_801bda28_slot04_0c[obj->state_3](obj);
}

void func_801b1efc_slot04_0c(Object *obj) {
    s32 v;
    obj->state_3 = obj->state_3 + 1;
    v = data_801bda44_slot04_0c[obj->field_12a >> 1];
    obj->accel_x = -0x8000;
    obj->vel_x = v;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138ae8(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x25);
}
