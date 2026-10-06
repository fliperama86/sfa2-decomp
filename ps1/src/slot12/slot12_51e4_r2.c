/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80028484_slot12[][4];
extern u16 data_801903c0;
extern u16 data_801903c2;

void func_800152b4_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];
    int i = obj->field_03;
    if (i != 5) {
        obj->field_76 = data_80028484_slot12[i][0];
        obj->field_78 = data_80028484_slot12[i][1];
        obj->field_7a = data_80028484_slot12[i][2];
        obj->field_7c = data_80028484_slot12[i][3];
    }
}

void func_80015334_slot12(Object *obj) {
    s16 c;
    int k;
    obj->field_5e = data_801903c0;
    if (game_state.field_2bd != 0) {
        obj->field_5e = data_801903c2;
    }
    k = obj->field_03;
    if (k != 5) {
        c = obj->field_5e;
        if (c == 0) goto same;
        if (k == 0) goto copy;
        if (c < k) goto same;
        obj->field_5c = k - 1;
        goto end;
copy:
        obj->field_5c = c;
        goto end;
same:
        obj->field_5c = obj->field_03;
    }
end:;
}
