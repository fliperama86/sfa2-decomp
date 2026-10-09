/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80029064_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void func_80016d90_slot27(Object *obj) {
    func_80131094(obj);
}

void func_80016db0_slot27(Object *obj) {
    obj->field_01 = 0;
    obj->field_04 = obj->field_04 + 1;
}

void func_80016dc4_slot27(Object *obj) {
    func_8011f38c(obj);
}

void func_80016de4_slot27(Object *obj) {
    *(int *)data_8019045c = -0x27;
    if (game_state.field_2e != 0) {
        *(int *)data_8019045c = 7;
    }
    *(u16 *)&obj->pos_x += *(u16 *)data_8019045c;
    *(u16 *)&obj->pos_y -= 8;
}

void func_80016e2c_slot27(Object *obj) {
    ref_other.p = obj->field_3c;
    data_80029064_slot27[obj->field_04](obj);
}

void func_80016e7c_slot27(Object *obj) {
    obj->field_90 = (void *)0x80038000;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_7c = 0x1e0;
    obj->field_7a = 0;
    obj->field_0d = 0;
    obj->field_48 = 0xff;
    obj->field_04++;
    obj->field_09 = ref_other.p->field_09;
    obj->field_01 = 0;
}
