/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);

void func_801b1ad4_slot04_06(Object *o) {
    Object *p;
    if ((s16)o->field_3a >= 0) {
        func_80130efc(o);
    } else {
        p = o->other;
        p->field_15b = 1;
        func_80140770(o, 1, 0xf, -0x200, 0, 0, 0);
        if ((s16)p->field_5c < 0) {
            if (o->field_49 == 0) {
                o->field_167 = 2;
            } else {
                o->field_167 = 0x16;
            }
        }
        o->field_4c = -0x20000;
        o->field_58 = -0x4800;
        o->field_50 = 0x44000;
        o->field_54 = 0;
        o->field_45 = 1;
        o->field_07++;
        func_801307e0(o, 0x26);
    }
}

void func_801b1bb0_slot04_06(Object *o) {
    func_801b4850_slot04_06(o);
    if (o->pos_y < o->field_70) {
        func_801b4818_slot04_06(o);
    } else {
        o->field_45 = 0;
        func_801209c4(o);
        o->field_17b = 0;
        o->pos_y = o->field_70;
        func_801312b8(o);
    }
}
