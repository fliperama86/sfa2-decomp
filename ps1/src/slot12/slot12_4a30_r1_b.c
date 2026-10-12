/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Pos data_80028200_slot12[];
extern void (*data_800282a8_slot12[])(Object *);

void call_handler_by_state_1_2_slot12(Object *obj) {
    data_800282a8_slot12[obj->state_1](obj);
}

void advance_state_1_when_flag_is_2_slot12(Object *obj) {
    if (game_state.field_2bc == 2) {
        obj->field_01 = 1;
        obj->state_1++;
    }
}
