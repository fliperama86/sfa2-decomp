/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014268_slot12(Object *obj) {
    data_8018f5a0->field_4a = 2;
    data_8018f5a0->field_4c = 0;
}

void func_80014280_slot12(Object *obj) {
}

void func_80014288_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    data_8018f5a0->field_4a++;
    if (game_state.field_2bd != 0) {
        data_8018f5a0->field_4a = 6;
    }
    data_8018f5a0->field_4c = 0;
    data_8018f5a0->field_4e = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
    func_8011eb14();
    func_8011eae4();
}
