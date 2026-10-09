/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80026908_slot27[])(Object *);
extern Slot27Rec9338 data_80029338_slot27[];

void func_800119c8_slot27(Object *obj) {
    ref_other.p = obj->other;
    data_80026908_slot27[obj->field_04](obj);
}

void func_80011a18_slot27(Object *obj) {
    obj->box_tables = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_60 = 0;
    obj->field_61 = 0;
    obj->field_45 = 0;
    obj->field_66 = 0;
    obj->field_5c = 0;
    obj->field_5e = 0;
    obj->field_54 = 0;
    obj->field_58 = 0;
    obj->field_62 = 0;
    obj->field_63 = 0;
    obj->field_70 = 0;
    obj->field_73 = 0;
    obj->field_72 = 0;
    obj->field_62 = 0;
    obj->field_6b = 0;
}

void func_80011a64_slot27(void) {
    Slot27Rec9338 *r = &data_80029338_slot27[ref_other.p->side];

    r->field_00 = 0;
    r->field_01 = 0;
    r->field_02 = 0;
    r->field_03 = 0;
    r->field_04 = 0;
    r->field_05 = 0;
    r->field_06 = 0;
}
