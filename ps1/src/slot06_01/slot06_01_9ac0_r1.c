/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea95c_slot06_01[];
extern u8 data_801ea964_slot06_01[];
extern ObjectFn data_801ea988_slot06_01[];
extern SequenceStep *data_801eb960_slot06_01[];

void func_801e9c58_slot06_01(Object *obj);

void func_801e9ac0_slot06_01(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->accel_y != 0) {
            func_801e9c58_slot06_01(obj);
            return;
        }
        data_801ea95c_slot06_01[obj->state_1](obj);
    }
    func_8011ffdc(obj);
}

void func_801e9b4c_slot06_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        u8 v;
        obj->state_1++;
        if (obj->accel_x == 3) {
            v = 0xb;
        } else if (obj->accel_x == 8) {
            v = 7;
        } else {
            v = get_random_byte();
        }
        obj->countdown = v;
    }
    advance_sequence_checked(obj);
}

void func_801e9bc8_slot06_01(Object *obj) {
    int v = obj->countdown - 1;
    obj->countdown = v;
    if ((s16)v == 0) {
        obj->state_1 = 0;
        obj->accel_x = data_801ea964_slot06_01[(u8)((get_random_byte() & 3) | ((u8)obj->accel_x << 2))];
        func_80130700(obj, data_801eb960_slot06_01[obj->accel_x]);
    }
}

void func_801e9c58_slot06_01(Object *obj) {
    data_801ea988_slot06_01[obj->state_1](obj);
    func_8011ffdc(obj);
}

void func_801e9cac_slot06_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = obj->state_1 + 1;
    }
    advance_sequence_checked(obj);
}
