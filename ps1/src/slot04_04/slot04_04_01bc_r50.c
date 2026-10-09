/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b449c_slot04_04(Object *obj);
void func_801b44f8_slot04_04(Object *obj);
void func_801b4bc4_slot04_04(Object *obj);

void func_801b445c_slot04_04(Object *obj) {
    if (obj->field_07 == 0) {
        func_801b449c_slot04_04(obj);
    } else {
        func_801b44f8_slot04_04(obj);
    }
}

void func_801b449c_slot04_04(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a == 4) {
        func_801204f4(obj, obj->side, 0xb);
    }
    func_801b4bc4_slot04_04(obj);
}
