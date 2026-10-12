/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_800ebca0_slot0f[];

/* The locals m and t hold the two shifted bytes of the tag and r holds the result of func_8015bd0c before they are read: with the expression of m in the sum 4 instruction slots differ, with that of t in parentheses in the sum 2, with the call in the sum 52. The read of the table entry idx stays before the stores to s: moved after the store to arg_a, 10 instruction slots differ. */
void fill_sprite_from_sequence_and_link_slot0f(Object *obj, Slot12Sprite *s) {
    u16 *p = (u16 *)obj->sequence->data_ptr;
    u16 a = *p++;
    u16 b;
    u32 idx;
    int r;
    u32 *ot;
    u32 t;
    u32 m;
    idx = data_800ebca0_slot0f[obj->ot_depth];
    b = *p++;
    s->arg_a = a;
    s->arg_b = b;
    a = *p++;
    b = *p++;
    s->x = a + obj->pos_x;
    s->y = b + obj->pos_y;
    a = *p++;
    b = *p++;
    s->lookup_result = func_8015bdd4(a, b);
    a = *p++;
    r = func_8015bd0c(0, 0, a, *p);
    ot = (u32 *)data_801987c8;
    m = ((Object *)ot)->step_count << 9;
    t = (((Object *)ot)->field_a3 << 10) + 0xe1000000;
    s->word_04 = m + t + (r & 0x1f);
    ((PrimTag *)s)->addr = ((PrimTag *)&ot[idx])->addr;
    ((PrimTag *)&ot[idx])->addr = (u32)s;
}
