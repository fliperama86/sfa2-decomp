/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801beff4_slot04_01[];
extern u8 data_801beffc_slot04_01[];

void func_801b2108_slot04_01(Object *o) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int y;
    int t;
    int i;
    int v;
    int w;

    y = (u16)o->field_70;
    o->pos_y = y;
    o->pos_y = y = y - *(u16 *)((u8 *)data_801beff4_slot04_01 + (o->field_3a & 0xe));
    t = (s16)o->field_3a;
    y = t;
    i = 0;
    t = t & 0xff00;
    if (t == 0x200) {
        o->field_3a = y & 0xff;
        o->field_165 = 0;
        if (o->field_4b == 0) {
            o->other->field_6b = 0xa;
            i = (o->field_12a >> 1) + 1;
        }
        o->field_27b = data_801beffc_slot04_01[i];
        func_801204f4(o, o->side, 5);
    }
    w = (s16)o->field_3a;
    v = w;
    w = w & 0xff00;
    if (w == 0x100) {
        o->field_3a = v & 0xff;
        func_80120554(o, o->side, 0x31c);
        func_801483a4(o, -0x18, 0x1c);
    }
}
