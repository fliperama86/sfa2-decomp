/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0b80_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->field_3a != 0) {
        o->state_3++;
        func_80120554(o, o->side, 0x324);
        if (o->field_0b != 0) {
            o->vel_x = 0xd0000;
            o->accel_x = -0xd000;
        } else {
            o->vel_x = 0xfff30000;
            o->accel_x = 0xd000;
        }
    }
    update_sequence(o);
}
