/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2f0c_slot04_0b[];

void func_801b3cc0_slot04_0b(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 == 0) {
        o->field_00 = 1;
        o->field_04 = 1;
        o->field_05 = 0;
        o->field_06 = 0;
        o->field_07 = 0;
        o->field_4c = -o->field_4c;
        o->field_50 = -data_801c2f0c_slot04_0b[o->field_ac >> 1];
    }
    func_80131094(o);
}
