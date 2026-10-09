/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002fce8_slot28[];
extern Object *data_800518fc_slot28[];

void func_80016df0_slot28(Object *obj) {
    if (game_state.field_f0 == 0) {
        data_8018f5a0->field_4e += 1;
    }
}

void func_80016e28_slot28(Object *obj, int arg) {
    func_80130768(data_800518fc_slot28[3], arg, data_8002fce8_slot28);
}
