/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801e7630_slot0b[];

void init_object_position_and_sequence_slot0b(Object *obj) {
    obj->field_01 = 1;
    obj->ot_depth = 0;
    obj->facing_side = 0;
    obj->field_0e = 0;
    obj->field_0f = 0;
    obj->scroll_factor = 0;
    obj->state_0++;
    obj->pos_x = box_margin[0];
    obj->pos_y = data_801aa5ea[0] + 8;
    set_sequence_from_table_5(obj, obj->field_02, data_801e7630_slot0b);
}

void update_object_at_box_margin_position_slot0b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->field_01 = 0;
        obj->state_0++;
        data_80190468.p->state_2 = 0;
    }
    obj->pos_x = box_margin[0];
    obj->pos_y = data_801aa5ea[0] + 8;
    advance_sequence_checked(obj);
}

void reset_slot_object_and_busy_flag_slot0b(Object *obj) {
    u32 *p = (u32 *)obj;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    ((u8 *)p)[8] = 0x18;
    data_801ae02c = 0;
    game_state.busy_flag = 0;
}
