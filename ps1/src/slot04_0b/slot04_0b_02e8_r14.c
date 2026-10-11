/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b16b8_slot04_0b(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->field_45 = 1;
        o->state_3++;
    }
    func_80130efc(o);
}
