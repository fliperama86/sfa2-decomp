/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern Object *data_8002d568_slot12;
extern u16 data_800175cc_slot12[];

void func_8001129c_slot12(void) {
    Object *o = data_8002d56c_slot12;
    int k = (s16)o->field_b0;
    u16 t;

    if (k == 0x5f) {
        t = o->field_b2;
        if ((unsigned)(t - 1) < 5) {
            func_80120554(0, 0, data_800175cc_slot12[(s16)t]);
        } else {
            func_801204f4(data_8002d568_slot12, data_8002d568_slot12->side, data_800175cc_slot12[(s16)t]);
        }
    } else if (k == 0x5e) {
        func_801204f4(data_8002d568_slot12, data_8002d568_slot12->side, 7);
    }
}
