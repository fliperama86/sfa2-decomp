/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801483a4(Object *object, int a, int b);
extern u8 data_801c43c0_slot04_04[];

void func_801b28b8_slot04_04(Object *o) {
    o->field_46 = (s16)o->field_46 - 0x100;
    if (o->field_46 & 0xff00) {
        o->field_07++;
        o->field_46 = ((Slot04bObj *)o)->field_46 + 0x3800;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        func_801204f4(o, o->side, 6);
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0xc, 0x30);
    }
}

void func_801b295c_slot04_04(Object *o) {
    o->field_46 = (s16)o->field_46 - 0x100;
    if ((o->field_46 & 0xff00) == 0) {
        o->field_07++;
        if (o->field_4b != 0) {
            ((Slot04bObj *)o)->field_27b = data_801c43c0_slot04_04[3];
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            ((Slot04bObj *)o)->field_27b = data_801c43c0_slot04_04[o->field_12a >> 1];
        }
        o->field_165 = 0;
        func_801204f4(o, o->side, 7);
    }
}

void func_801b2a08_slot04_04(Object *o) {
    if (*(u8 *)&o->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
    }
    func_80130efc(o);
}
