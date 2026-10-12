/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80028e8c_slot27[];
extern ObjectFn data_80028e94_slot27[];
extern SequenceStep *data_8002904c_slot27[];
void call_advance_sequence_checked_2_slot27(Object *obj);
void offset_position_by_selection_slot27(Object *obj);

void follow_ref_other_then_call_table_handler_slot27(Object *obj) {
    *(u16 *)&obj->pos_x = *(u16 *)&ref_other.p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&ref_other.p->pos_y;
    obj->ot_depth = ref_other.p->ot_depth - 1;
    data_80028e8c_slot27[obj->table_index](obj);
}

void call_advance_sequence_checked_slot27(Object *obj) {
    advance_sequence_checked(obj);
}

void call_handler_by_state_1_then_offset_slot27(Object *obj) {
    data_80028e94_slot27[obj->state_1](obj);
    offset_position_by_selection_slot27(obj);
}

void advance_or_switch_sequence_on_other_flag_slot27(Object *obj) {
    if (ref_other.p->field_6b == 0) {
        call_advance_sequence_checked_2_slot27(obj);
    } else {
        obj->state_1++;
        *(int *)data_8019045c = 1;
        set_sequence_from_table_5(obj, 1, data_8002904c_slot27);
    }
}
