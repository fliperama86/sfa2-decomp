/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s16 data_801ef494_slot06_0e[];
extern u16 data_801ef4a4_slot06_0e[];

void start_sequence_from_lookup_index_slot06_0e(Object *obj);

/* One local holds the OR of the two flags and later the table index.
   Written as one expression for the OR, or with a local for each, this
   function differs from the original in eleven instruction slots. */
void update_object_by_scroll_y_table_slot06_0e(Object *obj) {
    Object *c = (Object *)obj->linked_object_b;
    int v = game_state.field_65;

    v |= game_state.field_74;
    if (v == 0) {
        advance_sequence_checked(obj);
    }
    v = (s16)obj->countdown + obj->table_index;
    if (!((s16)((Slot06Layer *)cam_obj)->scroll_y < data_801ef494_slot06_0e[v])) {
        obj->pos_y = 0xf8 - data_801ef4a4_slot06_0e[v];
        start_sequence_from_lookup_index_slot06_0e(obj);
        obj->countdown += 2;
    }
    if (((Slot06Layer *)background_layers)->state_index == 2) {
        obj->state_0++;
        c->state_0 = 2;
    }
    set_flag_and_push_if_mode_8(obj);
}
