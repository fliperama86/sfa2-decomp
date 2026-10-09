/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0b80_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->field_3a != 0) {
        o->field_07++;
        func_80120554(o, o->side, 0x324);
        if (o->field_0b != 0) {
            o->field_4c = 0xd0000;
            o->field_54 = -0xd000;
        } else {
            o->field_4c = 0xfff30000;
            o->field_54 = 0xd000;
        }
    }
    func_80130efc(o);
}
