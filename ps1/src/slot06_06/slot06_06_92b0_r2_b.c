/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ee278_slot06_06[];
extern SequenceStep *data_801ee190_slot06_06[];
extern Slot06_06Rec83d8 data_801f83d8_slot06_06[][2];

void func_801e9c58_slot06_06(Object *obj, Slot06_06Rece1d8 *src, Slot06_06Rec83d8 *dst);

void func_801e9c58_slot06_06(Object *obj, Slot06_06Rece1d8 *src, Slot06_06Rec83d8 *dst) {
    dst->ot_depth = src->field_01;
    dst->pos_x = src->pos_x;
    dst->pos_y = src->pos_y;
    dst->param_byte = src->param_byte;
    dst->param_word_a = src->param_word_a;
    dst->param_word_b = src->param_word_b;
    func_80130700(obj, data_801ee190_slot06_06[src->table_index]);
    dst->sequence = obj->sequence;
}

void func_801e9cf8_slot06_06(Object *obj) {
    Slot06_06Rec83d8 *r;

    if ((s16)((Slot06Layer *)data_801aa5d4)->scroll_x < obj->vel_x) {
        r = &data_801f83d8_slot06_06[obj->table_index][0];
    } else {
        r = &data_801f83d8_slot06_06[obj->table_index][1];
    }
    obj->sequence = r->sequence;
    obj->sequence_changed_flag = 1;
    obj->pos_x = r->pos_x;
    obj->pos_y = 0xf8 - r->pos_y;
    obj->field_1c = r->param_word_a;
    obj->scroll_factor = r->param_word_b;
    obj->ot_depth = r->ot_depth;
    obj->field_0e = r->param_byte;
    func_80120028(obj);
}

void func_801e9db0_slot06_06(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9dd0_slot06_06(Object *obj) {
    data_801ee278_slot06_06[obj->state_0](obj);
}

void func_801e9e10_slot06_06(Object *obj) {
    u8 i = 6;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->state_0++;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    if (game_state.field_0a == 6 || game_state.skip_flag != 0) {
        i = 10;
    }
    func_80130700(obj, data_801ee190_slot06_06[i + obj->table_index]);
}

void func_801e9ea0_slot06_06(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9eec_slot06_06(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
