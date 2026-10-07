/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_8007a0ac_slot00[];
void func_80078458_slot00(Object *obj, int mode);

void func_800780f4_slot00(Object *obj) {
    obj->field_a4 = 2;
    obj->field_a5 = 0;
    obj->field_50 = data_8007a0ac_slot00[obj->field_03 * 2];
    obj->field_58 = data_8007a0ac_slot00[obj->field_03 * 2 + 2];
    if (obj->field_0b != 0) {
        obj->field_4c = data_8007a0ac_slot00[obj->field_03 * 2 - 1];
        obj->field_54 = data_8007a0ac_slot00[obj->field_03 * 2 + 1];
    } else {
        obj->field_4c = -data_8007a0ac_slot00[obj->field_03 * 2 - 1];
        obj->field_54 = -data_8007a0ac_slot00[obj->field_03 * 2 + 1];
    }
    func_80078458_slot00(obj, 2);
}
