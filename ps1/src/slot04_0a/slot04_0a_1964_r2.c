/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b1c00_slot04_0a(Object *obj);

void func_801b1afc_slot04_0a(Object *obj) {
    if (func_801b1e04_slot04_0a(obj)) {
        func_801b1c00_slot04_0a(obj);
    } else if (obj->field_50 > 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x25);
    }
}

void func_801b1b7c_slot04_0a(Object *obj) {
    if (func_801b1e04_slot04_0a(obj)) {
        func_801b1c00_slot04_0a(obj);
    } else if (obj->field_67 == 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->other->field_6b = 0x10;
        func_801307e0(obj, 0x23);
    }
}

void func_801b1c00_slot04_0a(Object *obj) {
    obj->field_07 = 7;
    func_801209c4(obj);
    obj->field_14 = 0;
    obj->pos_y = (u16)obj->field_70;
    func_801307e0(obj, 0x48);
}
