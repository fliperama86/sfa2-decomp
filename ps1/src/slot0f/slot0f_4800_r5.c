/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800ebbf0_slot0f[3])(Object *);
extern void (*data_800ebbfc_slot0f[3])(Object *);

void func_800e4d10_slot0f(Object *obj) {
    game_state.field_2bd = 0;
    data_800ebbf0_slot0f[data_8018f5a0->field_4a](obj);
}

void func_800e4d60_slot0f(Object *obj) {
    game_state.field_2bd = 1;
    data_800ebbfc_slot0f[data_8018f5a0->field_4a](obj);
}
