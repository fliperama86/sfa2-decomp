/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6c68_slot04_10[];

void func_801b4cdc_slot04_10(Object *obj) {
    s16 i;
    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        i = 0;
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            i = (obj->field_12a >> 1) + 1;
        }
        ((Slot04bObj *)obj)->field_27b = data_801c6c68_slot04_10[i];
        if (obj->field_4b == 0) {
            ref_other.p = obj->other;
            ref_other.p->field_6b = 0xa;
        }
    }
}
