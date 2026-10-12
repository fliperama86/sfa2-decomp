/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80079938_slot00[])(Object *);
extern BoxTables data_800798d0_slot00;
extern SequenceStep *data_800798fc_slot00[];
extern FrameRecord data_80079908_slot00[];
extern void update_follower_copy_parent_position_slot00(Object *obj);

void call_handler_by_state_0_2_slot00(Object *obj) {
    data_80079938_slot00[obj->state_0](obj);
}

void func_80077300_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->state_0 = obj->state_0 + 1;
    obj->ot_depth = 0;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->box_tables = &data_800798d0_slot00;
    obj->active_state = 0;
    obj->health = 0;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->facing_side = p->facing_side;
    obj->frames = data_80079908_slot00;
    set_sequence_from_table_5(obj, obj->vel_table_index >> 1, data_800798fc_slot00);
    obj->frame = obj->frames + obj->sequence->frame_index;
    update_follower_copy_parent_position_slot00(obj);
}
