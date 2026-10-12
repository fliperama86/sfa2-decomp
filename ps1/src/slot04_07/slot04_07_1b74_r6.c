/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);
int func_801b23e4_slot04_07(Object *obj);
void func_801b22bc_slot04_07(Object *obj);

void func_801b213c_slot04_07(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->vel_x = 0x80000;
    obj->countdown = 0x1e;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->accel_y = 0;
    *(s32 *)&((Slot04aObj *)obj)->field_1c0 = -0x8000;
    func_801307e0(obj, 0x23);
}

void func_801b21bc_slot04_07(Object *obj) {
    func_801b2994_slot04_07(obj);
    if (obj->mode_flag_2 == 0) {
        if (obj->pressed_bits & 0x68) {
            goto call_x;
        }
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown != 0 && (u16)func_801b23e4_slot04_07(obj) == 0) {
            goto fail;
        }
reset:
        obj->state_3 = 4;
        if (obj->mode_flag_2 != 0) {
            obj->state_3 = 2;
        }
        obj->countdown = 8;
        obj->accel_x = *(s32 *)&((Slot04aObj *)obj)->field_1c0;
        func_801307e0(obj, 0x24);
        return;
    }
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        goto reset;
    }
    if ((u16)func_801b23e4_slot04_07(obj) == 0) {
        goto fail;
    }
call_x:
    func_801b22bc_slot04_07(obj);
    return;
fail:
    update_sequence(obj);
}
