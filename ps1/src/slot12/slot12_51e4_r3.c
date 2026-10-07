/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_8011f240(Slab172 *s);

extern void (*data_800284ac_slot12[])(Object *);
extern void (*data_800284c4_slot12[])(Object *);
extern void (*data_800284cc_slot12[])(Object *);
void func_80015910_slot12(Object *obj);

void func_800153c0_slot12(Object *obj) {
    ref_first.p = (Object *)table_8016e5c4;
    if (game_state.field_2bd != 0) {
        ref_first.p = (Object *)table_8016e614;
    }
    data_800284ac_slot12[obj->field_03](obj);
}

void func_8001542c_slot12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_8001544c_slot12(Object *obj) {
    data_800284c4_slot12[obj->field_05](obj);
}

void func_8001548c_slot12(Object *obj) {
    obj->field_05 = obj->field_05 + 1;
    func_80015910_slot12(obj);
}

void func_800154b8_slot12(Object *obj) {
    data_800284cc_slot12[obj->field_05](obj);
}
