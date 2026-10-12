/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
void func_80014dec_slot27(Object *o);

void func_80014d64_slot27(Object *obj) {
    obj->ot_depth = 4;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->state_0++;
    obj->field_90 = (void *)0x80038000;
    obj->spawn_x = 0;
    obj->field_0d = 0;
    obj->field_01 = 1;
    obj->field_0b = ref_other.p->field_0b;
    obj->vel_x = 0;
    obj->vel_y = 0;
    func_80014dec_slot27(obj);
}
