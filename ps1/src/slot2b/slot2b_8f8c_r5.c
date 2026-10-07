/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern SequenceStep *data_8017c850;

void func_800795b4_slot2b(Object *obj);

void func_800794a0_slot2b(Object *o) {
    if ((s16)o->field_3a < 0) {
        o->field_04++;
    }
    func_80131094(o);
}
