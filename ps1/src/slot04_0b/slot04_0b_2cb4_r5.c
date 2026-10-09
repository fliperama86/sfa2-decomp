/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3550_slot04_0b(Object *obj);

void func_801b349c_slot04_0b(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_160 = 0x64;
    obj->field_46 = 0;
    obj->field_a1 = 0;
    func_801307e0(obj, 0x18);
}

void func_801b3504_slot04_0b(Object *obj) {
    if ((u8)obj->field_3a != 2) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_801b3550_slot04_0b(obj);
    }
}
