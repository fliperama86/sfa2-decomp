/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ea230_slot06_08(Object *obj);

extern SequenceStep *data_801ed480_slot06_08[];

/* One local holds the OR of the two flags and later the table index.
   Written as one expression for the OR, or with an index local of its own,
   this function differs from the original in seven instruction slots. */
void func_801ea160_slot06_08(Object *obj) {
    int v = game_state.field_65;
    v |= game_state.field_74;
    if (v == 0) {
        if (obj->field_05 != 0) {
            if ((s16)obj->field_3a & 0x8000) {
                v = 10;
                obj->field_05 = 0;
                if (obj->field_03 != 0) {
                    v = 12;
                }
                func_80130700(obj, data_801ed480_slot06_08[v]);
            }
            func_80131094(obj);
        } else {
            func_801ea230_slot06_08(obj);
        }
    }
    func_80120028(obj);
}
