/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801ef494_slot06_0e[];
extern u16 data_801ef4a4_slot06_0e[];

void func_801eaa28_slot06_0e(Object *obj);

/* One local holds the OR of the two flags and later the table index.
   Written as one expression for the OR, or with a local for each, this
   function differs from the original in eleven instruction slots. */
void func_801ea900_slot06_0e(Object *obj) {
    Object *c = (Object *)obj->linked_object_b;
    int v = game_state.field_65;

    v |= game_state.field_74;
    if (v == 0) {
        advance_sequence_checked(obj);
    }
    v = (s16)obj->countdown + obj->table_index;
    if (!((s16)((Slot06Layer *)cam_obj)->scroll_y < data_801ef494_slot06_0e[v])) {
        obj->pos_y = 0xf8 - data_801ef4a4_slot06_0e[v];
        func_801eaa28_slot06_0e(obj);
        obj->countdown += 2;
    }
    if (((Slot06Layer *)data_801aa544)->state_index == 2) {
        obj->state_0++;
        c->state_0 = 2;
    }
    func_80120028(obj);
}
