/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

int spawn_object_copying_position_slot05_06(Object *obj) {
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count = 1;
        ref_other.p->field_02 = 0x16;
        ref_other.p->table_index = 0;
        ref_other.p->field_0e = obj->field_0e;
        ref_other.p->field_1c = obj->field_1c;
        ref_other.p->ot_depth = obj->ot_depth;
        ref_other.p->pos_x = obj->pos_x;
        ref_other.p->pos_y = obj->pos_y;
        ref_other.p->facing_side = obj->facing_side;
        ref_other.p->spawn_x = obj->spawn_x;
        ref_other.p->spawn_y = obj->spawn_y;
        ref_other.p->field_02 = 0x10;
        ref_other.p->mode_id = 0x20;
        ref_other.p->field_90 = (void *)0x800fb100;
        ref_other.p->sound_side = obj->sound_side;
        ref_other.p->data_ptr_a = data_80172a48;
        ref_other.p->data_ptr_b = data_80173c9c;
        return 1;
    }
    return 0;
}
