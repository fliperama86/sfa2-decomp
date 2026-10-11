/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c20_slot04_08(Object *obj, Object *other) {
    if (other->field_15a != 0 || other->state_2 != 7 || other->state_3 == 4) {
        obj->state_0++;
    }
    *(s32 *)&obj->pos_x_frac = *(s32 *)&other->pos_x_frac;
    func_80131094(obj);
    obj->field_01 ^= 1;
}

void func_801b4ca0_slot04_08(Object *obj, Object *unused) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0++;
    }
    func_80131094(obj);
    obj->field_01 = 1;
}
