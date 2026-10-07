/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_80024470_slot12[];
int func_8015bdd4(int a, int b);

void func_80014300_slot12(Object *obj, Slot12Sprite *s) {
    u16 *p = (u16 *)obj->sequence->field_04;
    u32 idx;
    u32 *ot;
    u32 v;
    u32 t;
    u32 w;
    u16 a;
    u16 b;
    u8 *base;
    a = *p++;
    b = *p++;
    idx = data_80024470_slot12[obj->field_09];
    s->field_18 = a;
    s->field_19 = b;
    a = *p++;
    b = *p++;
    s->field_14 = a + obj->pos_x;
    s->field_16 = b + obj->pos_y;
    a = *p++;
    b = *p++;
    s->field_1a = func_8015bdd4(a, b);
    a = *p++;
    b = *p++;
    v = func_8015bd0c(0, 0, a, b);
    base = (u8 *)data_801987c8;
    ot = (u32 *)data_801987c8 + idx;
    w = base[0xa2] << 9;
    t = (base[0xa3] << 10) + 0xe1000000;
    s->word_04 = w + t + (v & 0x1f);
    ((PrimTag *)s)->addr = ((PrimTag *)ot)->addr;
    ((PrimTag *)ot)->addr = (u32)s;
}
