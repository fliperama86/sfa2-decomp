/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9790_slot06_0f(Object *obj);

extern SequenceStep *data_801ecba4_slot06_0f[];
extern ObjectFn data_801eb4b0_slot06_0f[];
extern ObjectFn data_801eb4b8_slot06_0f[];
extern ObjectFn data_801eb4c0_slot06_0f[];
extern ObjectFn data_801eb4c8_slot06_0f[];

void func_801e9790_slot06_0f(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eb4b0_slot06_0f[obj->field_03](obj);
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9804_slot06_0f(Object *obj) {
    data_801eb4b8_slot06_0f[obj->state_1](obj);
}

void func_801e9844_slot06_0f(Object *obj) {
    if ((func_80151184() & 3) == 0) {
        obj->state_1++;
        func_80130700(obj, data_801ecba4_slot06_0f[3]);
    }
}

void func_801e9898_slot06_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_1 = 0;
    }
}

void func_801e98b4_slot06_0f(Object *obj) {
    data_801eb4c0_slot06_0f[obj->state_1](obj);
}

void func_801e98f4_slot06_0f(Object *obj) {
    obj->state_1++;
    func_80130700(obj, data_801ecba4_slot06_0f[5]);
}

void func_801e992c_slot06_0f(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        return;
    }
    obj->state_1 = 0;
    func_80130700(obj, data_801ecba4_slot06_0f[4]);
}

void func_801e9968_slot06_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9988_slot06_0f(Object *obj) {
    data_801eb4c8_slot06_0f[obj->state_0](obj);
}
