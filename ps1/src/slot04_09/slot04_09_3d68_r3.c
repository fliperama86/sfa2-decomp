/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5478_slot04_09(Object *obj);

void func_801b3eec_slot04_09(Object *obj) {
    u16 t;
    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t == 0) {
        obj->field_07++;
        obj->field_165 = 0;
        obj->field_27b = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            obj->field_27b = 10;
        }
        func_801b5478_slot04_09(obj);
        func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 4);
    } else if ((u8)t != 2) {
        obj->field_3a = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x31c);
        func_801483a4(obj, 0, 0x54);
    }
}

void func_801b3fbc_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_a0 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
