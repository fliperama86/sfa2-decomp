/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2a58_slot04_03(Object *obj) {
    s16 t = obj->step_flags;
    u8 c;

    if (t & 0x8000) {
        obj->state_3 = 0xb;
        func_801307e0(obj, 0x31);
    } else if (t != 0) {
        if (obj->field_12a < 3) {
            goto set;
        }
        c = obj->field_129 - 1;
        obj->field_129 = c;
        if (c & 0x80) {
        set:
            obj->state_3 = 0xb;
            func_801307e0(obj, 0x31);
        } else {
            obj->state_3++;
            func_801204f4(obj, obj->side, 6);
            func_801204f4(obj, obj->side, 0xc);
            func_801307e0(obj, 0x2e);
        }
    } else {
        func_80130efc(obj);
    }
}
