/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0998_slot04_07(Object *obj) {
    u8 one;

    obj->state_3++;
    ((Slot04aObj *)obj)->field_1ca = 0;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x12, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        if (obj->field_12a == 4) {
            obj->state_3 = 2;
            obj->field_278 = one;
        }
    }
}

void func_801b0a44_slot04_07(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_801312b8(obj);
    } else {
        if ((t & 0x80) != 0 && func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_25f = 0xff;
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
