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
    Object *c = (Object *)obj->field_34;
    int v = game_state.field_65;

    v |= game_state.field_74;
    if (v == 0) {
        func_80131094(obj);
    }
    v = (s16)obj->field_46 + obj->field_03;
    if (!((s16)((Slot06Layer *)cam_obj)->field_16 < data_801ef494_slot06_0e[v])) {
        obj->pos_y = 0xf8 - data_801ef4a4_slot06_0e[v];
        func_801eaa28_slot06_0e(obj);
        obj->field_46 += 2;
    }
    if (((Slot06Layer *)data_801aa544)->field_05 == 2) {
        obj->field_04++;
        c->field_04 = 2;
    }
    func_80120028(obj);
}
