/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_800282e8_slot27[];
extern void (*data_800283c8_slot27[])(Object *);

void func_800151f4_slot27(Object *obj) {
    data_800283c8_slot27[obj->field_04](obj);
}
