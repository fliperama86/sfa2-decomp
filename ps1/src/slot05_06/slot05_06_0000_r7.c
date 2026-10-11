/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);

void func_801c8858_slot05_06(Object *obj) {
    int one;

    obj->vel_x = 0xa0000;
    obj->state_3 = obj->state_3 + 1;
    obj->accel_x = -0xc000;
    one = 1;
    obj->field_159 = one;
    func_80130dc0(obj);
    obj->field_278 = one;
}

void func_801c88b4_slot05_06(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        if ((s16)obj->step_flags == 3) {
            play_sound_at(obj, obj->side, 0x324);
        }
        if ((s16)obj->step_flags == 2 && obj->vel_x >= 0) {
            func_801cc814_slot05_06(obj);
        } else {
            if (((*(u8 *)&obj->step_flags & 0x80) != 0 && (u8)func_80141618(obj) != 0) || (u8)func_801412a4(obj) != 0) {
                obj->state_3 = 0;
            }
        }
        update_sequence(obj);
    }
}
