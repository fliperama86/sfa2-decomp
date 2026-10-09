/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3a84_slot04_04(Object *object);

void func_801b2a48_slot04_04(Object *o) {
    if (func_801b3a84_slot04_04(o) < 0 && ((s16)o->field_3a & 0x8000)) {
        o->field_58 = -0x6000;
        if (o->pos_y > o->field_70) {
            o->field_14 = 0;
            o->field_45 = 0;
            o->field_159 = 0;
            o->field_07++;
            o->pos_y = o->field_70;
            func_801209c4(o);
            func_801307e0(o, o->field_12a + 0x54);
            return;
        }
    }
    func_80130efc(o);
}

void func_801b2aec_slot04_04(Object *o) {
    if (((s16)o->field_3a & 0x8000) == 0) {
        func_80130efc(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->field_249 = 5;
        func_801312b8(o);
    }
}
