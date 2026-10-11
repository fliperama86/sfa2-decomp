/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f58_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);

void func_801b2e8c_slot04_0e(Object *o) {
    /* Stand-in, not recovered source: the one 8-bit local a, which holds the field, then the facing, then the sequence number, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
    /* Measured with a kept as one variable: as an int it differs in 2 slots, and without the (s8) cast below in 7. */
    u8 a;

    func_801b4914_slot04_0e(o);
    if (o->field_70 < o->pos_y) {
        func_801b2f58_slot04_0e(o);
    } else if ((s16)(o->field_70 - 0x30) < o->pos_y) {
        func_80130efc(o);
    } else {
        a = o->field_0b;
        if (a == 0) {
            a = 2;
        } else {
            a = 1;
        }
        if (o->field_164 != (s8)a) {
            func_80130efc(o);
        } else {
            a = 0x60;
            o->state_3++;
            o->vel_y = 0;
            o->accel_y = 0;
            if (o->field_49 == 0) {
                a = 0x2e;
            }
            func_801307e0(o, a);
            func_801209c4(o);
        }
    }
}
