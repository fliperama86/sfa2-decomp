/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c15b0_slot04_05[];

void func_801b2b1c_slot04_05(Object *obj) {
    u16 i = 0;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c15b0_slot04_05[i];
    }
}
