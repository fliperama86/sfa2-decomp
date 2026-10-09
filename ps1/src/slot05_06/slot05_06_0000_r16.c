/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801c9458_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    obj->field_1c3 = 0;
    o->field_17b = 1;
    o->field_07++;
    if (o->field_49 != 0) {
        o->field_225 = 1;
    }
    func_80141f28(o, 5);
    func_80138ae8(&game_state, o);
    func_801307e0(o, 0x1e);
}

void func_801c94c8_slot05_06(Object *obj) {
    obj->field_4c = 0x40000;
    obj->field_50 = 0x28000;
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_54 = 0;
    obj->field_58 = -0xa000;
    obj->field_45 = 1;
    obj->field_07++;
}
