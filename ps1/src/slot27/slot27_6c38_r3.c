/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_800292cc_slot27[];
void func_80016f5c_slot27(Object *obj);

void func_80016ee0_slot27(Object *obj) {
    func_80016f5c_slot27(obj);
    if (obj->field_00 == 2) {
        obj->field_04++;
    }
    if (ref_other.p->field_01 != 0) {
        if ((obj->field_48 & 0x80) == 0) {
            obj->field_01 = 1;
        }
    }
}

void func_80016f5c_slot27(Object *obj) {
    data_80190458.p = ref_other.p->other;
    if (data_80190458.p->kind < 0x15) {
        func_80130768(obj, data_80190458.p->kind, data_800292cc_slot27[data_80190458.p->side]);
        obj->field_48 = data_80190458.p->kind;
        *(u16 *)&obj->pos_x = *(u16 *)&ref_other.p->pos_x;
        *(u16 *)&obj->pos_y = *(u16 *)&ref_other.p->pos_y;
    } else {
        obj->field_48 = 0xff;
    }
}

void func_80017028_slot27(Object *obj) {
    obj->field_04++;
}
