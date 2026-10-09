/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_801b2d6c_slot04_09(Object *o);

void func_801b2ae4_slot04_09(Object *obj) {
    Object *p;
    int t;

    func_801b2d6c_slot04_09(obj);
    func_801b2858_slot04_09(obj);
    if (obj->field_3a & 1) {
        obj->field_07++;
        p = obj->other;
        p->field_04 = 1;
        p->field_05 = 1;
        p->field_06 = 0;
        p->field_07 = 0;
        p->field_63 = 0;
        p->field_60 = 0;
        p->field_61 = 1;
        p->field_62 = 3;
        p->field_73 = 0;
        t = obj->field_0b;
        obj->field_73 = 0;
        p->field_15b = 1;
        p->field_260 = 1;
        p->field_6b = 0;
        p->field_45 = 0xff;
        p->field_72 = t ^ 1;
        p->pos_y = obj->field_70;
        p->field_14 = 0;
        p->field_261 = 0;
        p->field_27a = 0;
        obj->field_292 = 0x1e;
        p->field_292 = 0x1e;
        obj->field_50 = obj->field_50 + 0x3c000;
        obj->field_4c = obj->field_4c << 1;
        obj->field_0b = obj->field_0b ^ 1;
        p->field_0b = obj->field_0b;
        func_801307e0(obj, 0x23);
    } else {
        func_80130efc(obj);
    }
}
