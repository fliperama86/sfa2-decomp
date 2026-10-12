/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801ea094_slot06_07(Object *obj);
void func_801ea2bc_slot06_07(Object *obj);

extern SequenceStep *data_801ee1a8_slot06_07;
extern ObjectFn data_801ee1f8_slot06_07[];
extern ObjectFn data_801ee208_slot06_07[];
extern SequenceStep **data_801ee210_slot06_07[];
extern Slot06_07Rece218 data_801ee218_slot06_07[];
extern ObjectFn data_801ee21c_slot06_07[];

u8 func_801ea094_slot06_07(Object *obj) {
    int r = 0;
    u8 k = obj->kind;
    if (k == 4 || k == 9 || k == 0xd || k == 0x12) {
        r = 1;
    }
    return r;
}

Object *func_801ea0d4_slot06_07(void) {
    return 0;
}

Object *func_801ea0dc_slot06_07(void) {
    return &player_left;
}

Object *func_801ea0ec_slot06_07(void) {
    return &player_right;
}

Object *func_801ea0fc_slot06_07(void) {
    Object *o = &player_right;
    if (player_right.pos_x > player_left.pos_x) {
        o = o - 1;
    }
    return o;
}

void func_801ea12c_slot06_07(Object *obj) {
    data_801ee1f8_slot06_07[obj->state_0](obj);
}

void func_801ea16c_slot06_07(Object *obj) {
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->field_0f = 1;
    if (obj->facing_side == 0) {
        obj->field_0a = 1;
    }
    func_801ea2bc_slot06_07(obj);
}

void func_801ea1bc_slot06_07(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801ee208_slot06_07[obj->state_1](obj);
        advance_sequence_checked(obj);
    }
    func_80120028(obj);
}

void func_801ea230_slot06_07(Object *obj) {
    if (game_state.pause_flag_2 != 0 && game_state.countdown_timer == 0) {
        Slot06Obj *s = (Slot06Obj *)obj;
        obj->state_1++;
        func_80130700(obj, s->sequence_table[obj->variant_index]);
    }
}

void func_801ea294_slot06_07(Object *obj) {
}

void func_801ea29c_slot06_07(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea2bc_slot06_07(Object *obj) {
    Slot06Obj *s = (Slot06Obj *)obj;
    s->sequence_table = data_801ee210_slot06_07[obj->table_index];
    obj->variant_index = data_801ee218_slot06_07[obj->table_index].entry_value;
    func_80130700(obj, s->sequence_table[data_801ee218_slot06_07[obj->table_index].entry_index]);
}

void func_801ea348_slot06_07(Object *obj) {
    data_801ee21c_slot06_07[obj->state_0](obj);
}

void func_801ea388_slot06_07(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    func_80130700(obj, data_801ee1a8_slot06_07);
}

void func_801ea3d0_slot06_07(Object *obj) {
    func_80120028(obj);
}

void func_801ea3f0_slot06_07(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
