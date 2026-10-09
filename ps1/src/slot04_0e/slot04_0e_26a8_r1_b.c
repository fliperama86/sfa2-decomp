/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f58_slot04_0e(Object *obj);
void func_801b3284_slot04_0e(Object *obj);

void func_801b2f58_slot04_0e(Object *obj) {
    obj->field_07 = 6;
    obj->field_45 = 0;
    obj->field_14 = 0;
    obj->field_17b = 0;
    obj->pos_y = (u16)obj->field_70;
    func_801209c4(obj);
    func_801307e0(obj, 0x30);
}

void func_801b2fa8_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_1de == 0) {
        func_801b3284_slot04_0e(o);
    }
    if ((s16)o->field_3a >= 0) {
        func_80130efc(o);
    } else {
        o->field_4c = 0x60000;
        o->field_50 = 0x90000;
        o->field_58 = -0x8000;
        o->field_07++;
        o->field_54 = 0;
        if (o->field_0b == 0) {
            o->field_4c = -o->field_4c;
        }
        func_801307e0(o, 0x2f);
    }
}
