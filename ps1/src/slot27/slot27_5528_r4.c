/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80055f30;
extern u16 data_80055f34;
extern u16 data_8002856c_slot27[];
extern Slot27Rec851c data_8002851c_slot27[];

void func_800158c0_slot27(Object *obj) {
    s16 *q = &game_state.field_d4;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int n;
    int t;
    int u;
    obj->pos_y = -0x100;
    n = game_state.field_40;
    *q = -0x100;
    t = 0x140 - data_8002856c_slot27[n];
    t -= data_8002851c_slot27[n].field_00;
    game_state.field_d2 = t;
    data_80055f30 = t;
    u = data_8002851c_slot27[n].field_02;
    u -= 0x110;
    *q = u;
    data_80055f34 = u;
}
