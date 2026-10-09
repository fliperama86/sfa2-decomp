/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801dd31c_slot05_06[];
extern s32 data_801dd328_slot05_06[];

void func_801c969c_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int i = o->field_12a >> 1;
    s16 x;

    if (o->field_49 != 0) {
        i += 3;
    }
    x = data_801dd31c_slot05_06[i];
    if (o->field_06 == 8) {
        x = 0x60;
    }
    if ((u8)func_8013f8c4(o, -0x20, x)) {
        o->field_159 = 1;
        o->field_07 += 2;
        o->pos_y = obj->field_70;
        func_801204f4(o, o->side, 9);
        func_80120554(o->other, o->other->side, 0x31a);
        if (o->field_06 == 8) {
            func_80145f98(o);
        }
        func_801307e0(o, 0x1f);
    } else {
        o->field_07++;
        func_801204f4(o, o->side, 1);
        func_801307e0(o, 0x27);
    }
}

void func_801c97ac_slot05_06(Object *o) {
    s32 *p;

    o->field_12c = 0;
    o->field_12d = 0;
    o->field_12e = 0;
    o->field_12f = 0;
    o->field_07++;
    func_80141f28(o, 5);
    p = &data_801dd328_slot05_06[o->field_12a * 2];
    o->field_4c = *p++;
    o->field_50 = *p++;
    o->field_54 = *p++;
    o->field_58 = *p;
    func_801307e0(o, 0x1f);
}
