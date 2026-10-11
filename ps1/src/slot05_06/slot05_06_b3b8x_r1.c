/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);

/* The same form as func_801b33b8_slot04_06 of the first side's file, whose
   unit has the measured variants. The two differ in their addresses only. */
void func_801cb3b8_slot05_06(Object *obj) {
    int v;
    if (obj->vel_x >= 0) {
        Object *other;
        func_801cc814_slot05_06(obj);
        other = obj->other;
        v = other->field_6b != 0;
        if (v && other->field_61 == 1) {
            if ((s16)other->field_5c < 0) {
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
