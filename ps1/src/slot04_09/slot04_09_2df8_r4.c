/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3190_slot04_09(Object *obj) {
    u16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t == 0) {
        obj->field_07++;
        obj->field_27b = 0;
        if (obj->field_165 & 0x80) {
            obj->other->field_6b = 10;
            obj->field_27b = 2;
        }
        obj->field_165 = 0;
    } else if ((u8)t != 2) {
        obj->field_3a = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x19, 0x31);
    }
}
