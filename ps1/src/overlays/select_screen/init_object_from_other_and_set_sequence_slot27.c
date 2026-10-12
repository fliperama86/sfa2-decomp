/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_8002904c_slot27[];
extern ObjectFn data_80028e84_slot27[];

void init_object_from_other_and_set_sequence_slot27(Object *obj) {
    int idx;
    obj->field_01 = 1;
    obj->state_0++;
    obj->facing_side = ref_other.p->facing_side;
    obj->pos_x = ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y;
    obj->ot_depth = ref_other.p->ot_depth + 0xff;
    obj->field_90 = (void *)0x80038000;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->spawn_x = 0;
    obj->image_row = 0;
    data_80028e84_slot27[obj->table_index](obj);
    idx = data_8019045c[0];
    set_sequence_from_table_5(obj, idx, data_8002904c_slot27);
}
