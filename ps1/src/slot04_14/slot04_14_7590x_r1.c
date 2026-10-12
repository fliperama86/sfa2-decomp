/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern FrameRecord *data_1f8000a8;
extern FrameRecord *data_1f800158;
extern u8 data_801c7644_slot04_14[];
extern u16 data_801c7878_slot04_14[];
extern u16 data_801c7884_slot04_14[];
extern s32 data_801c7890_slot04_14[];
extern s32 data_801c78cc_slot04_14[];

int dispatch_table_pick_by_table_index_slot04_14(Object *obj, Object *parent);
void set_sequence_a_by_sound_side_slot04_14(Object *obj, u8 index);

void init_child_projectile_from_parent_tables_slot04_14(Object *o) {
    Object *p;
    int a;
    int v;
    int k;
    int t;
    ((Slot04bObj *)o)->data_table_ptr = data_801c7644_slot04_14;
    o->state_0++;
    ((Slot04bObj *)o)->field_b0 = o->image_row;
    p = o->parent;
    o->ot_depth = 0;
    o->field_49 = p->field_49;
    o->field_1c = p->field_1c;
    if (o->table_index != 0) {
        dispatch_table_pick_by_table_index_slot04_14(o, p);
    }
    k = o->table_index >> 1;
    t = data_801c7884_slot04_14[k];
    v = data_801c78cc_slot04_14[o->vel_table_index];
    o->vel_y = v;
    o->pos_y = o->pos_y - t;
    if (o->facing_side != 0) {
        a = data_801c7878_slot04_14[o->table_index >> 1];
        o->vel_x = data_801c7890_slot04_14[o->vel_table_index];
    } else {
        a = data_801c7878_slot04_14[o->table_index >> 1];
        a = -a;
        o->vel_x = -data_801c7890_slot04_14[o->vel_table_index];
    }
    o->pos_x = a + o->pos_x;
    if (p->side == 0) {
        ((Slot04bObj *)o)->data_pointer = (s32)data_1f8000a8;
    } else {
        ((Slot04bObj *)o)->data_pointer = (s32)data_1f800158;
    }
    set_sequence_a_by_sound_side_slot04_14(o, o->vel_table_index);
}
