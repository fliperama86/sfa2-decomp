/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801aa5e4[];

void func_801b1d94_slot04_05(Object *o) {
    *(s32 *)&o->field_10 = data_801aa5e4[0] - ((Slot04aObj *)o)->field_a0 + *(s32 *)&o->field_10;
    ((Slot04aObj *)o)->field_a0 = data_801aa5e4[0];
    if ((u8)func_80130184(o) != 0) {
        if (o->field_50 < 0 || o->pos_y <= 0x50) {
            if (o->field_164 != 0) {
                o->field_07++;
            }
        }
        if (*(u8 *)&o->field_3a != 2) {
            func_80130efc(o);
        }
    } else {
        o->field_07 += 3;
        o->field_45 = 0;
        o->pos_y = o->field_70;
        func_801307e0(o, 0x32);
    }
}
