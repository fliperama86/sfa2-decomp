/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c15b0_slot04_05[];

void func_801b2b1c_slot04_05(Object *obj) {
    u16 i = 0;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c15b0_slot04_05[i];
    }
}
