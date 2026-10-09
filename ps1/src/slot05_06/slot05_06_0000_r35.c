/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801cacb4_slot05_06(Object *obj) {
    Object *p = obj->other;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        p->field_15b = 1;
        p->field_260 = 1;
        func_80140770(obj, 0, 0xc, 0x1a, 0, 0, 0);
        if ((s16)p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            game_state.field_6b = 4;
            func_80147000(obj);
            func_80120554(p, p->side, 0x34f);
        } else {
            func_80120554(p, p->side, 0x34e);
        }
        obj->field_4c = 0xfffe0000;
        obj->field_50 = 0x44000;
        obj->field_54 = 0;
        obj->field_58 = -0x4800;
        obj->field_07++;
        obj->field_45 = 1;
        func_801307e0(obj, 0x26);
    }
}

void func_801cadc0_slot05_06(Object *obj) {
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
