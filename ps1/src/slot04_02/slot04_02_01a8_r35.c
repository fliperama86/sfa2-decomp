/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8014025c(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801b4ad8_slot04_02(Object *obj);
void func_801b4b68_slot04_02(Object *obj);
void func_801b4ba0_slot04_02(Object *obj);
void func_801b4c34_slot04_02(Object *obj);

void func_801b482c_slot04_02(Object *obj) {
    if (obj->field_50 < 0) {
        if ((obj->field_134 | obj->field_136) & 0x95) {
            if (obj->other->field_164 == 0 && (u8)func_8014025c(obj, -8, 0x24, -0x54, 0x18)) {
                func_801b4ad8_slot04_02(obj);
                return;
            }
            func_801b4b68_slot04_02(obj);
        }
        if (obj->field_50 < 0) {
            if ((obj->field_134 | obj->field_136) & 0x6a) {
                if (obj->other->field_164 == 0 && (u8)func_8014025c(obj, -8, 0x24, -0x30, 0x18)) {
                    func_801b4ba0_slot04_02(obj);
                } else {
                    func_801b4c34_slot04_02(obj);
                }
            }
        }
    }
}
