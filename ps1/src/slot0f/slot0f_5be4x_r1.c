/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_800ebca0_slot0f[];
int func_8015bdd4(int a, int b);

void func_800e5be4_slot0f(Object *obj, Slot12Sprite *s) {
    u16 *p = (u16 *)obj->sequence->field_04;
    u16 a = *p++;
    u16 b;
    u32 idx;
    int r;
    u32 *ot;
    u32 t;
    u32 m;
    idx = data_800ebca0_slot0f[obj->field_09];
    b = *p++;
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
    r = func_8015bd0c(0, 0, a, *p);
    ot = (u32 *)data_801987c8;
    m = ((Object *)ot)->field_a2 << 9;
    t = (((Object *)ot)->field_a3 << 10) + 0xe1000000;
    s->word_04 = m + t + (r & 0x1f);
    ((PrimTag *)s)->addr = ((PrimTag *)&ot[idx])->addr;
    ((PrimTag *)&ot[idx])->addr = (u32)s;
}
