/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80026ed0_slot27[])(Object *);
extern void (*data_80026ee0_slot27[])(Object *);
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern void func_8001435c_slot27(Object *obj);
extern void func_80014298_slot27(Object *obj);
extern SequenceStep *data_80027bc4_slot27[];

void func_80013ee4_slot27(Object *object) {
    data_80026ed0_slot27[object->field_04](object);
}

void func_80013f24_slot27(Object *object) {
    data_80026ee0_slot27[object->field_05](object);
}

void func_80013f64_slot27(Object *obj) {
    Object *o;

    obj->field_09 = 8;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_7c = 0x1e0;
    obj->field_46 = 0x40ff;
    obj->field_90 = (void *)0x80038000;
    obj->field_7a = 0;
    obj->field_0d = 0;
    obj->field_01 = 0;
    obj->field_05 = obj->field_05 + 1;
    o = obj->other;
    obj->field_0d = o->side;
    obj->field_0b = obj->field_0d ^ 1;
    obj->field_03 = o->kind;
    obj->field_48 = obj->field_03;
    func_8001435c_slot27(obj);
    func_80014298_slot27(obj);
}
