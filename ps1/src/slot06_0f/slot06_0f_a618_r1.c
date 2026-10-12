/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ea6e4_slot06_0f(Object *obj);
int func_801ea77c_slot06_0f(Object *obj);
int func_801ea7e4_slot06_0f(Object *obj);
void func_801ea91c_slot06_0f(Object *obj, int idx);
void func_801ea96c_slot06_0f(Object *obj);

extern ObjectFn data_801eeffc_slot06_0f[];

void func_801ea618_slot06_0f(Object *obj) {
    data_801eeffc_slot06_0f[obj->state_2](obj);
}

void func_801ea658_slot06_0f(Object *obj) {
    int idx;

    if (game_state.config->hold_flag == 0) {
        obj->state_2 = obj->state_2 + 1;
        idx = 5;
        if (*(s32 *)&game_state.config->object_ptr == (s32)game_state.other_object) {
            play_side_sound_at(obj, obj->sound_side, 0x12);
            idx = 4;
        }
        func_801ea91c_slot06_0f(obj, idx);
    }
    func_801ea6e4_slot06_0f(obj);
}

void func_801ea6e4_slot06_0f(Object *obj) {
    advance_sequence_checked(obj);
}

void func_801ea704_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
    func_801ea96c_slot06_0f(obj);
}

void func_801ea734_slot06_0f(Object *obj) {
    int idx = (s16)obj->pos_x > (s16)game_state.other_object->pos_x;

    if (obj->variant_index != idx) {
        obj->variant_index = idx;
        func_801ea91c_slot06_0f(obj, idx);
    }
}

int func_801ea77c_slot06_0f(Object *obj) {
    s16 v = *(s16 *)&game_state.other_object->other->state_0;

    if (v != 0x101) {
        return v == 0x103;
    }
    return 1;
}

void func_801ea7b4_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 1;
    obj->state_2 = 0;
    obj->state_3 = 0;
    func_801ea96c_slot06_0f(obj);
}

int func_801ea7e4_slot06_0f(Object *obj) {
    s16 v = *(s16 *)&game_state.other_object->state_0;

    if (v == 0x101) {
        if (obj->hit_type != 0xff) {
            return 1;
        }
    } else if (v == 0x103) {
        return 1;
    }
    return 0;
}

void func_801ea828_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 2;
    obj->state_2 = 0;
    obj->state_3 = 0;
    func_801ea96c_slot06_0f(obj);
}

int func_801ea85c_slot06_0f(Object *obj) {
    if (func_801ea77c_slot06_0f(obj)) {
        return func_801ea7e4_slot06_0f(obj) != 0;
    }
    return 0;
}

void func_801ea898_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 1;
    obj->state_2 = 0;
    obj->state_3 = 0;
    func_801ea96c_slot06_0f(obj);
}

int func_801ea8c8_slot06_0f(Object *obj) {
    return game_state.config->enabled_flag != 0;
}

void func_801ea8e0_slot06_0f(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 3;
    obj->state_2 = 0;
    obj->state_3 = 0;
}

void func_801ea8fc_slot06_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
