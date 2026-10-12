/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea940_slot06_01[];
extern ObjectFn data_801ea94c_slot06_01[];
extern SequenceStep *data_801eb990_slot06_01;
extern SequenceStep *data_801eb994_slot06_01;

void func_801e982c_slot06_01(Object *obj);
void func_801e9894_slot06_01(Object *obj);
void func_801e98e0_slot06_01(Object *a, Object *b);

void func_801e9758_slot06_01(Object *o) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801ea940_slot06_01[o->state_1](o);
    }
    func_8011ffdc(o);
}

void func_801e97c4_slot06_01(Object *obj) {
    func_801e982c_slot06_01(obj);
    func_801e9894_slot06_01(obj);
}

void func_801e97f4_slot06_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 0;
    }
    advance_sequence_checked(obj);
}

void func_801e982c_slot06_01(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_46 = 0x40;
        if ((get_random_byte() & 7) == 0) {
            obj->state_1 = 1;
            func_80130700(obj, data_801eb990_slot06_01);
        }
    }
}

void func_801e9894_slot06_01(Object *obj) {
    Object *pair = &player_left;

    func_801e98e0_slot06_01(pair, obj);
    func_801e98e0_slot06_01(pair + 1, obj);
}

void func_801e98e0_slot06_01(Object *a, Object *b) {
    if ((u8)(a->state_2 - 7) < 2) {
        b->state_1 = 2;
        func_80130700(b, data_801eb994_slot06_01);
    }
}

void func_801e9928_slot06_01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9948_slot06_01(Object *obj) {
    data_801ea94c_slot06_01[obj->state_0](obj);
}
