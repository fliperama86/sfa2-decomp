/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1fc0_slot04_0b(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_80130efc(o);
    if (obj->field_3a != 0) {
        o->field_45 = 1;
        o->field_07++;
        if (o->field_0b == 0) {
            o->pos_x -= 0x28;
        } else {
            o->pos_x += 0x28;
        }
    }
}
