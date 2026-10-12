/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);

/* One local holds two values in turn: whether field_6b is set, and later the
   halved field_12a. With the test in place, with the shift in place, with
   both in place, and with a local for each, this function differs from the
   original in 3 instruction slots each time. */
void func_801b33b8_slot04_06(Object *obj) {
    int v;
    if (obj->vel_x >= 0) {
        Object *other;
        func_801b4818_slot04_06(obj);
        other = obj->other;
        v = other->field_6b != 0;
        if (v && other->field_61 == 1) {
            if ((s16)other->health < 0) {
                goto set;
            }
            obj->state_3 += 3;
            obj->field_159 = 1;
        }
        update_sequence(obj);
    } else {
    set:
        obj->state_3++;
        v = obj->field_12a >> 1;
        func_801307e0(obj, v + 0x45);
    }
}
