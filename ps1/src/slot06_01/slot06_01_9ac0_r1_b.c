/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea998_slot06_01[];
extern SequenceStep *data_801eb984_slot06_01[];
extern SequenceStep *data_801eb9ac_slot06_01;

void func_801e9d78_slot06_01(Object *obj) {
    if (player_left.state_2 == 5 || player_right.state_2 == 8) {
        obj->state_1++;
        func_80130700(obj, data_801eb9ac_slot06_01);
    } else {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_1 = 1;
        }
        advance_sequence_checked(obj);
    }
}

void func_801e9dfc_slot06_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 1;
    }
    advance_sequence_checked(obj);
}

void func_801e9e34_slot06_01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9e54_slot06_01(Object *obj) {
    data_801ea998_slot06_01[obj->state_0](obj);
}

void func_801e9e94_slot06_01(Object *obj) {
    SequenceStep *s;
    u16 y;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->base_x = 0x300;
    obj->base_y = 0x100;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    obj->state_0++;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    y = obj->pos_y;
    obj->pos_y = 0xf8 - y;
    func_80130700(obj, data_801eb984_slot06_01[0]);
    func_8011ffdc(obj);
}

void func_801e9f24_slot06_01(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9f70_slot06_01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
