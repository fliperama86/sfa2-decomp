/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ad8_slot04_02(Object *obj);
void func_801b4b68_slot04_02(Object *obj);
void func_801b4ba0_slot04_02(Object *obj);
void func_801b4c34_slot04_02(Object *obj);

void func_801b494c_slot04_02(Object *obj) {
    s16 d;

    if (obj->field_50 < 0 && obj->field_129 == 0) {
        if (obj->other->field_164 == 0) {
            d = obj->pos_x - obj->other->pos_x;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x19 && (u8)func_8014025c(obj, -8, 0x24, -0x54, 0x18)) {
                func_801b4ad8_slot04_02(obj);
                return;
            }
            obj->field_46 = obj->field_46 - 0x100;
            func_801b4b68_slot04_02(obj);
        }
        if (obj->field_50 < 0 && obj->field_129 == 0 && obj->other->field_164 == 0) {
            d = obj->pos_x - obj->other->pos_x;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x19 && (u8)func_8014025c(obj, -8, 0x24, -0x30, 0x18)) {
                func_801b4ba0_slot04_02(obj);
            } else {
                obj->field_46 = obj->field_46 - 0x100;
                func_801b4c34_slot04_02(obj);
            }
        }
    }
}
