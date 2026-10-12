/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014268_slot12(Object *obj) {
    data_8018f5a0->phase_index = 2;
    data_8018f5a0->step_index = 0;
}

void func_80014280_slot12(Object *obj) {
}

void func_80014288_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    data_8018f5a0->phase_index++;
    if (game_state.mode_flag_2 != 0) {
        data_8018f5a0->phase_index = 6;
    }
    data_8018f5a0->step_index = 0;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
    func_8011eb14();
    func_8011eae4();
}
