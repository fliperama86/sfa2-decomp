/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);
int func_801b23e4_slot04_07(Object *obj);
void func_801b22bc_slot04_07(Object *obj);

void func_801b213c_slot04_07(Object *obj) {
    obj->field_17b = 1;
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->field_4c = 0x80000;
    obj->field_46 = 0x1e;
    obj->field_54 = 0;
    obj->field_50 = 0;
    obj->field_58 = 0;
    *(s32 *)&((Slot04aObj *)obj)->field_1c0 = -0x8000;
    func_801307e0(obj, 0x23);
}

void func_801b21bc_slot04_07(Object *obj) {
    func_801b2994_slot04_07(obj);
    if (obj->field_cd == 0) {
        if (obj->field_134 & 0x68) {
            goto call_x;
        }
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 != 0 && (u16)func_801b23e4_slot04_07(obj) == 0) {
            goto fail;
        }
reset:
        obj->field_07 = 4;
        if (obj->field_cd != 0) {
            obj->field_07 = 2;
        }
        obj->field_46 = 8;
        obj->field_54 = *(s32 *)&((Slot04aObj *)obj)->field_1c0;
        func_801307e0(obj, 0x24);
        return;
    }
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        goto reset;
    }
    if ((u16)func_801b23e4_slot04_07(obj) == 0) {
        goto fail;
    }
call_x:
    func_801b22bc_slot04_07(obj);
    return;
fail:
    func_80130efc(obj);
}
