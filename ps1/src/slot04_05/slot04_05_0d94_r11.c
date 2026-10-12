/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b364c_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->field_45 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
    }
    if (*(u8 *)&obj->step_flags != 2) {
        update_sequence(obj);
    }
}
