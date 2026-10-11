/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027ccc_slot27[];
extern ObjectFn data_80027cdc_slot27[];
extern ObjectFn data_80027cec_slot27[];

void func_80014744_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80014764_slot27(Object *obj) {
    data_80027ccc_slot27[obj->state_1](obj);
}

void func_800147a4_slot27(Object *obj) {
    obj->field_01 = 1;
    obj->vel_y = 8;
    obj->state_1 = obj->state_1 + 1;
}

void func_800147c4_slot27(Object *obj) {
    data_80027cdc_slot27[obj->state_1](obj);
}

void func_80014804_slot27(Object *obj) {
    obj->field_7a = 0x20;
    obj->field_7c = 0x1e0;
    obj->field_01 = 1;
    obj->vel_y = 8;
    obj->state_1 = obj->state_1 + 1;
}

void func_80014834_slot27(Object *obj) {
    data_80027cec_slot27[obj->state_1](obj);
}

void func_80014874_slot27(Object *obj) {
    obj->field_7a = 0x10;
    obj->field_7c = 0x1e0;
    obj->field_01 = 1;
    obj->vel_x = -8;
    obj->state_1 = obj->state_1 + 1;
}
