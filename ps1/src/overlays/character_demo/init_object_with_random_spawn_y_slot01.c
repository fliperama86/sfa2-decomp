/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80024344_slot01[];
extern u8 data_80024a7c_slot01[];
extern SequenceStep *data_800299a8_slot01[];

void init_object_with_random_spawn_y_slot01(Object *obj) {
    obj->field_01 = 1;
    obj->ot_depth = 0;
    obj->facing_side = 0;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->state_0++;
    obj->pos_x = box_margin[0] + 0xc0;
    obj->pos_y = 0x78;
    obj->data_ptr_a = data_80024344_slot01;
    obj->data_ptr_b = data_80024a7c_slot01;
    obj->field_90 = (void *)0x8007a744;
    obj->spawn_x = 0x40;
    obj->spawn_y = 0x1f0 + get_random_byte() % 3;
    if (obj->table_index == 0x81 || obj->table_index == 0x83) {
        obj->spawn_y = 0x1f4;
    }
    set_sequence_from_table_5(obj, (s16)(obj->table_index - 0x80), data_800299a8_slot01);
}

void advance_state_when_step_flag_negative_slot01(Object *obj) {
    data_801ae066 = obj->step_flags;
    if (data_801ae066 & 0x8000) {
        obj->field_01 = 0;
        obj->state_0++;
        game_state.busy_flag = 0;
        single_object_active = 0;
    }
    advance_sequence_checked(obj);
}
