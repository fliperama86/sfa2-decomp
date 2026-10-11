/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2f0c_slot04_0b[];

void func_801b3cc0_slot04_0b(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 == 0) {
        o->field_00 = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->vel_x = -o->vel_x;
        o->vel_y = -data_801c2f0c_slot04_0b[o->field_ac >> 1];
    }
    func_80131094(o);
}
