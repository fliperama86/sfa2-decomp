/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3a6c_slot04_06(Object *obj) {
    Object *other;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        func_80140770(obj, 0, 0xc, 3, 0, 0, 0);
        if ((s16)other->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x16;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            }
        }
        other->field_247 = 0;
        obj->field_17b = 0;
        func_801312b8(obj);
    }
}
