/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eeec0_slot06_12[];
extern ObjectFn data_801eeed0_slot06_12[];
extern ObjectFn data_801eeed8_slot06_12[];
extern ObjectFn data_801eeee0_slot06_12[];
extern ObjectFn data_801eeef0_slot06_12[];
extern ObjectFn data_801ef300_slot06_12[];
extern SequenceStep *data_801eee74_slot06_12[];

Block172 *func_8011f1e0(void);
void func_801e9648_slot06_12(Object *a, u8 *b);
void func_801e96b0_slot06_12(Object *obj);
void func_801e9994_slot06_12(Object *obj);
void func_801e9e0c_slot06_12(Object *obj);
void func_801e9be8_slot06_12(Object *obj);
void func_801e9b1c_slot06_12(Object *obj);

void func_801e9450_slot06_12(Object *obj) {
    data_801eeec0_slot06_12[obj->state_0](obj);
}

void func_801e9490_slot06_12(Object *obj) {
    obj->spawn_mode = 4;
    obj->field_0c = 0;
    obj->state_0++;
    data_801eeed0_slot06_12[obj->table_index](obj);
}

void func_801e94e4_slot06_12(Object *obj) {
    Object *p;

    obj->field_0f = 1;
    game_state.pair_ptr = (Pair *)&player_left;
    obj->hit_type = 0;
    obj->field_62 = 0;
    *(u32 *)&obj->health = *(u32 *)&((Object *)game_state.pair_ptr)->pos_x_frac;
    obj->field_60 = ((Object *)game_state.pair_ptr)->active_state;
    p = &player_left;
    p++;
    game_state.pair_ptr = (Pair *)p;
    obj->team_id = 0;
    obj->sound_side = 0;
    *(u32 *)&obj->field_60 = *(u32 *)&((Object *)game_state.pair_ptr)->pos_x_frac;
    obj->damage = ((Object *)game_state.pair_ptr)->active_state;
}

void func_801e9574_slot06_12(Object *obj) {
    obj->facing_side = 0;
    obj->field_0a = 1;
    obj->field_0e = 8;
    obj->ot_depth = 0x1a;
    obj->field_0f = 1;
    func_80130768(obj, 0, data_801eee74_slot06_12);
}

void func_801e95bc_slot06_12(Object *obj) {
    data_801eeed8_slot06_12[obj->table_index](obj);
}

void func_801e95fc_slot06_12(Object *obj) {
    Object *pair = &player_left;

    func_801e9648_slot06_12(pair, (u8 *)obj + 0x5c);
    func_801e9648_slot06_12(pair + 1, (u8 *)obj + 0x60);
}

void func_801e9648_slot06_12(Object *a, u8 *b) {
    u8 x = a->active_state;
    u8 y = b[4];

    if ((x & y) == 0 && (x | y) != 0) {
        b[5] = 0;
        b[6] = 0;
        func_801e96b0_slot06_12(a);
    }
    b[4] = a->active_state;
}

void func_801e96b0_slot06_12(Object *obj) {
    game_state.other_object = (Object *)func_8011f1e0();
    if (game_state.other_object != 0) {
        game_state.other_object->active_count = 1;
        game_state.other_object->field_02 = 0x35;
        game_state.other_object->table_index = 1;
        game_state.other_object->pos_x = obj->pos_x;
        game_state.other_object->pos_y = obj->ground_y;
        game_state.other_object->parent = obj;
        game_state.other_object->spawn_mode = 4;
    }
}

void func_801e9760_slot06_12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void func_801e97b4_slot06_12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e97d4_slot06_12(Object *obj) {
    data_801eeee0_slot06_12[obj->state_0](obj);
}

void func_801e9814_slot06_12(Object *obj) {
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->vel_x = obj->pos_x;
    func_80130768(obj, 1, data_801eee74_slot06_12);
}

void func_801e9864_slot06_12(Object *obj) {
    Slot06Layer *l2;
    s16 d;
    int x;

    if (game_state.field_65 == 0) {
        l2 = (Slot06Layer *)data_801aa5d4;
        x = l2->scroll_x;
        x -= l2->ref_x;
        x &= -0x80;
        d = obj->vel_x + x;
        obj->pos_x = d;
        d = *(u16 *)&l2->vel_x;
        d += 1;
        func_80130768(obj, d, data_801eee74_slot06_12);
    }
    func_80120028(obj);
}

void func_801e98e8_slot06_12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9908_slot06_12(Object *obj) {
    data_801eeef0_slot06_12[obj->state_0](obj);
}

void func_801e9948_slot06_12(Object *obj) {
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->vel_x = obj->pos_x;
    obj->vel_y = obj->pos_y;
    func_801e9994_slot06_12(obj);
}

void func_801e9994_slot06_12(Object *obj) {
    Slot06Layer *l2 = (Slot06Layer *)data_801aa5d4;

    obj->pos_x = l2->ref_x - l2->scroll_x + (u16)obj->pos_x;
    func_80130768(obj, 1, data_801eee74_slot06_12);
}

void func_801e99dc_slot06_12(Object *obj) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_0e;
        d -= l2->scroll_y;
        d += d >> 2;
        obj->pos_y = (u16)obj->vel_y - d;
        obj->pos_x = l2->ref_x - l2->scroll_x + (u16)obj->vel_x;
        advance_sequence_checked(obj);
    }
    func_80120028(obj);
}

void func_801e9a64_slot06_12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9a84_slot06_12(Object *obj) {
    data_801ef300_slot06_12[obj->state_0](obj);
}

void func_801e9ac4_slot06_12(Object *obj) {
    obj->field_01 = 0;
    obj->field_0c = 0;
    obj->accel_y = 0;
    obj->spawn_mode = 4;
    obj->state_0++;
    func_801e9e0c_slot06_12(obj);
    func_801e9be8_slot06_12(obj);
    func_801e9b1c_slot06_12(obj);
}
