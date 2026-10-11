/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void func_801b3104_slot04_08(Object *obj) {
    s16 d;
    u16 lim = 0xa0;
    Object *other;
    int k;

    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    d = (u16)obj->pos_x - data_801c3e98_slot04_08;
    if (obj->field_0b != 0) {
        d = -d;
    }
    if (d >= 0) {
        d = 2;
        if (obj->vel_x < 0) {
            d = 1;
        }
        if (obj->field_164 != d) {
            other = obj->other;
            d = -0x20;
            if (obj->field_0b != 0) {
                d = 0x20;
            }
            d = d + (u16)obj->pos_x - other->pos_x + 0x50;
            if (lim < (u16)d) {
                update_sequence(obj);
                return;
            }
        }
    }
    obj->state_3++;
    k = 0x52;
    if (obj->field_12a != 4) {
        k = 0x51;
    }
    func_801307e0(obj, k);
}

void func_801b31f0_slot04_08(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        func_801307e0(obj, 0x53);
    } else {
        update_sequence(obj);
    }
}
