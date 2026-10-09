/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801befb0_slot04_01[];

void func_801b1b9c_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    data_801befb0_slot04_01[o->field_07](o);
    if (obj->field_1a5 != 0) {
        if (o->field_50 < 0) {
            if (o->pos_y - o->field_70 >= -0x1f) {
                obj->field_1a5 = 0;
            }
        }
    }
}
