/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b57b4_slot04_09(Object *obj);
void func_801b6068_slot04_09(Object *obj);

void func_801b0438_slot04_09(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        func_801204f4(obj, obj->side, 0xd);
    }
    update_sequence(obj);
}

void func_801b0488_slot04_09(Object *obj) {
    u16 t = obj->step_flags;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        func_801204f4(obj, obj->side, 0xe);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 2) {
        obj->step_flags = u & 0xff00;
        func_801b57b4_slot04_09(obj);
    }
    update_sequence(obj);
}

void func_801b04f8_slot04_09(Object *obj) {
    u16 t = obj->step_flags;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        func_801204f4(obj, obj->side, 0xc);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 2) {
        obj->step_flags = u & 0xff00;
        func_801b6068_slot04_09(obj);
    }
    update_sequence(obj);
}
