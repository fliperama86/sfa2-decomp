/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1c5c_slot04_00(Object *obj);

void func_801b25f0_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        if (--((Slot04aObj *)obj)->step_count == 0) {
            obj->state_3++;
            func_801307e0(obj, (obj->direction_index >> 1) + 0x35);
            return;
        }
    }
    *(s32 *)&obj->pos_y_frac += 0x2000;
    update_sequence(obj);
}

void func_801b2670_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) != 0) {
        obj->state_3++;
        func_801209c4(obj);
        func_801307e0(obj, 0x30);
    } else {
        update_sequence(obj);
    }
}
