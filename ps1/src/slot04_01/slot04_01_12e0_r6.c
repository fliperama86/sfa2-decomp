/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1b58_slot04_01(Object *obj);

void func_801b1a14_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80120554(obj, obj->side, 0x320);
        ((Slot04aObj *)obj)->field_1a4 += 0xff;
        if (((Slot04aObj *)obj)->field_1a4 & 0x80) {
            obj->state_3++;
            func_801307e0(obj, 0x21);
            return;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    update_sequence(obj);
}

void func_801b1aa8_slot04_01(Object *obj) {
    if (func_801b1b58_slot04_01(obj) != 0) {
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        obj->state_3++;
        func_80130678(obj, 0x11);
    }
    update_sequence(obj);
}
