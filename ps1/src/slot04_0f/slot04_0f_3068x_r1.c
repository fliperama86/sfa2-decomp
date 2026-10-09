/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3068_slot04_0f(Object *obj) {
    if (obj->field_50 >= 0) {
        *(s32 *)&obj->field_14 -= obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->field_70 <= obj->pos_y) {
            *(s32 *)&obj->field_10 += obj->field_4c;
            obj->field_4c += obj->field_54;
            return;
        }
    }
    obj->field_07++;
    obj->field_259 = 0;
    obj->field_258 = 0;
    obj->field_25a = 0;
    func_801204f4(obj, obj->side, 0xe);
    func_801307e0(obj, 0x47);
}
