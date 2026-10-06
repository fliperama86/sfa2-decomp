/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Pos data_80028200_slot12[];
extern void (*data_800282a8_slot12[])(Object *);

void func_80014ab4_slot12(Object *obj) {
    data_800282a8_slot12[obj->field_05](obj);
}

void func_80014af4_slot12(Object *obj) {
    if (game_state.field_2bc == 2) {
        obj->field_01 = 1;
        obj->field_05++;
    }
}
