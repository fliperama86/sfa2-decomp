/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_800282e8_slot27[];

void update_sequence_on_active_state_change_slot27(Object *obj) {
    int *p = data_8019045c;
    int idx;
    *p = obj->field_62;
    if (obj->active_state != *p) {
        obj->active_state = *p;
        *p = *p * 2;
        obj->ot_depth = *p + 3;
        *p = *p * 2;
        idx = obj->table_index + *p;
        *p = idx;
        set_sequence_from_table_5(obj, idx, data_800282e8_slot27[(*(Config **)&data_80190458)->table_index]);
    }
}
