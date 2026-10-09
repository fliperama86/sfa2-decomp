/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801cabd8_slot05_06(Object *obj) {
    func_801cc84c_slot05_06(obj);
    if (obj->pos_y < obj->field_70) {
        func_801cc814_slot05_06(obj);
    } else {
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
        func_801312b8(obj);
    }
}

void func_801cac40_slot05_06(Object *obj) {
    Object *p = obj->other;

    if ((s16)obj->field_3a >= 0) {
        func_801cc814_slot05_06(obj);
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_80120554(p, p->side, 0x34e);
        func_801307e0(obj, 0x2e);
    }
}
