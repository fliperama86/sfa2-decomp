/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
void func_80014dec_slot27(Object *o);

void func_80014d64_slot27(Object *obj) {
    obj->field_09 = 4;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_7c = 0x1e0;
    obj->field_04++;
    obj->field_90 = (void *)0x80038000;
    obj->field_7a = 0;
    obj->field_0d = 0;
    obj->field_01 = 1;
    obj->field_0b = ref_other.p->field_0b;
    obj->field_4c = 0;
    obj->field_50 = 0;
    func_80014dec_slot27(obj);
}
