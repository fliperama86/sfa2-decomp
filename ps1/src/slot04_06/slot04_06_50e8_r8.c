/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5df0_slot04_06(Object *obj) {
    obj->field_44 = 1;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->vel_y = 0x40000;
    obj->state_0++;
    func_80130768(obj, 0x23, seqs_8017c7f8);
}

void func_801b5e3c_slot04_06(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    if ((s16)obj->step_flags < 0) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}
