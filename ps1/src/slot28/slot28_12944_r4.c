/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80048ccc_slot28[][2];
extern s32 data_80048cf4_slot28[][2];
void set_sequence_from_box_tables_index_slot28(Object *obj, int idx);

void init_object_position_from_table_index_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int k = 0;
    obj->field_0c = 0;
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0++;
    obj->pos_x = data_80048ccc_slot28[obj->table_index][0];
    obj->pos_y = data_80048ccc_slot28[obj->table_index][1];
    if (obj->table_index != 0) {
        int j = obj->table_index - 1;
        obj->vel_x = data_80048cf4_slot28[j][0];
        obj->vel_y = data_80048cf4_slot28[j][1];
        k = 3;
    }
    set_sequence_from_box_tables_index_slot28(obj, k);
}

void set_sequence_from_box_tables_index_slot28(Object *obj, int idx) {
    set_sequence_from_table_5(obj, idx, (SequenceStep **)obj->box_tables);
}
