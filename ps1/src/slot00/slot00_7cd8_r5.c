/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_8007a0ac_slot00[];
void func_80078458_slot00(Object *obj, int mode);

void func_800781d8_slot00(Object *obj) {
    obj->field_09 = 0;
    if (obj->field_66 == 0) {
        obj->sequence = seqs_a4_left[4];
    } else {
        obj->sequence = seqs_154_right[4];
    }
    obj->field_38 = obj->sequence->duration;
    obj->field_3a = obj->sequence->flags;
    obj->field_80 = 1;
}

void func_80078238_slot00(Object *obj) {
    func_80078458_slot00(obj, 0);
}
