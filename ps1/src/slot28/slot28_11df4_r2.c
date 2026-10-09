/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800469cc_slot28[];
extern void (*data_800469d0_slot28[])(Object *);
extern ObjectFn data_800469d8_slot28[];

void func_80021f74_slot28(Object *o) {
    o->field_0e = 0;
    o->field_0c = 0;
    o->field_0b = 0;
    o->field_48 = 0;
    o->field_04++;
    func_80130768(o, data_800469cc_slot28[o->field_03], (SequenceStep **)o->box_tables);
}

void func_80021fc8_slot28(Object *obj) {
    data_800469d0_slot28[obj->field_03](obj);
    obj->field_01 = 1;
}

void func_8002201c_slot28(Object *obj) {
    data_800469d8_slot28[obj->field_05](obj);
    func_80131094(obj);
}

void func_80022070_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_05 += 1;
    }
}
