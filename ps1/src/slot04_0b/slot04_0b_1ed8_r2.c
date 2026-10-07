/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *obj);

void func_801b202c_slot04_0b(Object *o) {
    func_801b2510_slot04_0b(o);
    if (o->field_50 < 0) {
        o->field_07++;
        o->field_54 = 0;
        o->field_50 = 0;
        o->field_58 = 0xffff0000;
        if (o->field_0b == 0) {
            o->field_4c = 0x10000;
        } else {
            o->field_4c = 0xffff0000;
        }
        func_801307e0(o, 0x23);
    } else {
        func_80130efc(o);
    }
}

void func_801b20b0_slot04_0b(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_801b2510_slot04_0b(o);
    if (o->pos_y < o->field_70) {
        if (obj->field_3a != 0) {
            return;
        }
    } else {
        o->field_07++;
        o->field_14 = 0;
        o->field_45 = 0;
        o->field_17b = 0;
        o->pos_y = (u16)o->field_70;
        func_801209c4(o);
    }
    func_80130efc(o);
}
