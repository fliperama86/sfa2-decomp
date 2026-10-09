/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_8002904c_slot27[];
extern ObjectFn data_80028e84_slot27[];

void func_80016ad4_slot27(Object *obj) {
    int idx;
    obj->field_01 = 1;
    obj->field_04++;
    obj->field_0b = ref_other.p->field_0b;
    obj->pos_x = ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y;
    obj->field_09 = ref_other.p->field_09 + 0xff;
    obj->field_90 = (void *)0x80038000;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_7c = 0x1e0;
    obj->field_7a = 0;
    obj->field_0d = 0;
    data_80028e84_slot27[obj->field_03](obj);
    idx = data_8019045c[0];
    func_80130768(obj, idx, data_8002904c_slot27);
}
