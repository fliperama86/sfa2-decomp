/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5478_slot04_09(Object *obj);

void func_801b43f0_slot04_09(Object *obj) {
    if (obj->field_67 != 0) {
        obj->field_67 = 0;
        obj->other->field_6b = 4;
    }
    if (obj->step_flags & 0xf) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 4);
    }
    update_sequence(obj);
}

void func_801b4470_slot04_09(Object *obj) {
    if (obj->step_flags & 0xf) {
        obj->state_3++;
        func_801b5478_slot04_09(obj);
    }
    update_sequence(obj);
}
