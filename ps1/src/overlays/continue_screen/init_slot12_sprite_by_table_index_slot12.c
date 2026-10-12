/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_800175e0_slot12[];
extern u8 data_80017630_slot12[];
extern SequenceStep *table_80017870_slot12[];

void init_slot12_sprite_by_table_index_slot12(Object *obj) {
    obj->field_01 = 1;
    obj->field_0c = 1;
    obj->field_90 = (void *)0x80040000;
    obj->data_ptr_a = data_800175e0_slot12;
    obj->data_ptr_b = data_80017630_slot12;
    obj->field_0e = 0;
    obj->table_index_2 = 0;
    obj->image_row = 0;
    obj->facing_side = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->state_0++;
    if (obj->table_index == 0) {
        obj->ot_depth = 9;
        obj->pos_x = 0x90;
        obj->pos_y = 0x38;
        set_sequence_from_table_5(obj, 2, table_80017870_slot12);
    } else if (obj->table_index == 1) {
        obj->ot_depth = 0xb;
        obj->pos_x = 0;
        obj->pos_y = 0x28;
        set_sequence_from_table_5(obj, 0, table_80017870_slot12);
    } else if (obj->table_index == 2) {
        obj->ot_depth = 0xb;
        obj->pos_x = 0xc0;
        obj->pos_y = 0x28;
        set_sequence_from_table_5(obj, 1, table_80017870_slot12);
    }
}

void advance_state_0_on_status_flags_slot12(Object *obj) {
    if (game_state.status_flags != 0 && obj->table_index == 0) {
        obj->state_0++;
    }
    set_flag_and_push_if_mode_8(obj);
}
