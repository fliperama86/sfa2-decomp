/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80026988_slot27[];
extern u8 data_8002f158_slot27;

void func_800125f4_slot27(void) {
    Object *o;
    u32 m;
    u32 v;
    u32 *q;
    int a;

    if (game_state.field_06 != 0) return;
    o = ref_other.p;
    m = ~o->field_c4;
    q = (u32 *)data_8019045c;
    *q = m;
    v = o->field_c2 & m;
    *q = v & 0xf000;
    if ((v & 0xf000) == 0) return;
    a = (v >> 15) << 1;
    if (v & 0x2000) a = 1;
    if (v & 0x1000) a = 4;
    if (v & 0x4000) a = 3;
    a = data_80026988_slot27[a + o->kind * 5];
    if (data_8002f158_slot27 != 0) {
        if (a == 6) return;
        if (a == 8) return;
        if (a == 10) return;
        if (a == 14) return;
        if (a == 16) return;
    }
    if (a == 6 || a == 8 || a == 10 || a == 14 || a == 16) {
        data_8002f158_slot27 = 0xff;
    }
    ref_other.p->kind = a;
}
