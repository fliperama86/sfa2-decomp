/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b57b4_slot04_09(Object *obj);
void func_801b6068_slot04_09(Object *obj);

void func_801b0438_slot04_09(Object *obj) {
    u16 t = obj->field_3a;

    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
        func_801204f4(obj, obj->side, 0xd);
    }
    func_80130efc(obj);
}

void func_801b0488_slot04_09(Object *obj) {
    u16 t = obj->field_3a;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
        func_801204f4(obj, obj->side, 0xe);
    }
    u = obj->field_3a;
    if ((u & 0xff) == 2) {
        obj->field_3a = u & 0xff00;
        func_801b57b4_slot04_09(obj);
    }
    func_80130efc(obj);
}

void func_801b04f8_slot04_09(Object *obj) {
    u16 t = obj->field_3a;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
        func_801204f4(obj, obj->side, 0xc);
    }
    u = obj->field_3a;
    if ((u & 0xff) == 2) {
        obj->field_3a = u & 0xff00;
        func_801b6068_slot04_09(obj);
    }
    func_80130efc(obj);
}
