/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051918_slot28;
extern int data_80051950_slot28;

void func_8001788c_slot28(Object *obj) {
    data_8018f5a0->field_60 = (s16)data_8018f5a0->field_60 - 1;
    if ((s16)data_8018f5a0->field_60 < 0) {
        func_8011f240((Slab172 *)data_80051918_slot28.p);
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->field_52++;
    }
}

void func_80017908_slot28(Object *obj) {
    if (game_state.field_f0 == 0) {
        data_80051950_slot28 = 1;
        data_8018f5a0->field_4e++;
    }
}
