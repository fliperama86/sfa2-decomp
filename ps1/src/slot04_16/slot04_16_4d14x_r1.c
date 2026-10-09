/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801ca3fc_slot04_16[];

void func_801b4d14_slot04_16(Object *obj) {
    s16 i;

    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        i = 0;
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            i = (obj->field_12a >> 1) + 1;
        }
        ((Slot04bObj *)obj)->field_27b = data_801ca3fc_slot04_16[i];
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
    }
}
