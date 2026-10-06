/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudSlot data_8002c4c8_slot12[4];
void func_80016cb4_slot12(Object *obj, HudSlot *c, s16 i);

void func_80016c50_slot12(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        func_80016cb4_slot12(obj, &data_8002c4c8_slot12[i], i);
    }
}
