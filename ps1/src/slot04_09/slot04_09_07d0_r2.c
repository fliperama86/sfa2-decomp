/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0a94_slot04_09(Object *obj) {
    obj->field_159 = 1;
    obj->state_3++;
    if (obj->field_12a == 2 && obj->field_25f == 0 && (obj->field_130 & 0x8000) != 0) {
        obj->state_3 = 2;
        func_80130ec0(obj);
        obj->field_225 = 1;
        if (obj->field_246 == 0) {
            obj->field_225 = 0;
            func_80141f28(obj, 1);
        }
        play_side_sound_at(obj, obj->side, 9);
        obj->field_278 = 1;
        func_801307e0(obj, 0x2b);
    } else {
        func_80130dc0(obj);
    }
}
