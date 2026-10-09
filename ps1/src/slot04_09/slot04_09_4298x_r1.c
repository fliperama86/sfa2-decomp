/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b58dc_slot04_09(Object *obj);

void func_801b4298_slot04_09(Object *obj) {
    u16 t;
    Object *p;
    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x80) {
        obj->field_07++;
        obj->field_165 = 0;
        obj->field_27b = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            obj->field_27b = 0xe;
        }
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x16;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = 2;
            p->field_ad = 1;
            p->field_0e = obj->field_0e;
            p->field_26 = obj->field_26;
            *(u32 *)&p->field_a4 = 0x1000800;
            p->field_7a = 0x60;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            func_801b58dc_slot04_09(obj);
        }
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
