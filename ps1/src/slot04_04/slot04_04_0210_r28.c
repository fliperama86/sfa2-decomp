/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void func_801b4bc4_slot04_04(Object *obj);
extern ObjectFn data_801c45cc_slot04_04[];

void func_801b45e0_slot04_04(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    func_801b4bc4_slot04_04(obj);
}

void func_801b4610_slot04_04(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4678_slot04_04(Object *obj) {
    data_801c45cc_slot04_04[obj->field_07](obj);
}
