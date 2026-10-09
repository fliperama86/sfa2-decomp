/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd378_slot05_06[];
void func_801ca048_slot05_06(Object *obj);
void func_801cc814_slot05_06(Object *obj);

void func_801c9cf4_slot05_06(Object *o) {
    u8 t;
    if (((Slot04aObj *)o)->field_1c3 == 0x2f) {
        t = 1;
        if (o->field_4b == 0) {
            t = 0xff;
        }
        o->field_165 = t;
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -2, 0x5c);
    }
    if (((Slot04aObj *)o)->field_1c3 != 0) {
        ((Slot04aObj *)o)->field_1c3--;
        t = 0;
        if (((Slot04aObj *)o)->field_1c3 != 0) {
            return;
        }
        o->field_165 = 0;
        if (o->field_4b == 0) {
            o->other->field_6b = 10;
            t = (o->field_12a >> 1) + 1;
        }
        o->field_27b = data_801dd378_slot05_06[t];
    }
    if ((s16)o->field_3a >= 0) {
        if (*(u8 *)&o->field_3a != 0) {
            *(u8 *)&o->field_3a = 0;
            func_801ca048_slot05_06(o);
        }
        if (o->field_4c >= 0) {
            func_801cc814_slot05_06(o);
        }
        func_80130efc(o);
    } else if (o->field_12a == 4) {
        o->field_45 = 1;
        o->field_50 = 0xb0000;
        o->field_58 = -0xe000;
        o->field_07++;
        if (o->field_0b == 0) {
            o->field_4c = -0x40000;
            o->field_54 = 0x500;
        } else {
            o->field_4c = 0x40000;
            o->field_54 = -0x500;
        }
        o->field_46 = 0;
        func_801204f4(o, o->side, 6);
        func_801307e0(o, 0x37);
    } else {
        func_801312b8(o);
    }
}
