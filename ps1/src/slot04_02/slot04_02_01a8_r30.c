/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3aa8_slot04_02(Object *obj) {
    obj->field_17b = 1;
    obj->field_07 = 1;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    func_801307e0(obj, obj->field_49 != 0 ? 0x68 : 0x37);
}

void func_801b3b0c_slot04_02(Object *obj) {
    u16 n = 0x32;

    if ((s16)obj->field_3a < 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_49 != 0) {
            n = 0x69;
        }
        n = (obj->field_12a >> 1) + n;
        func_801307e0(obj, n);
    } else {
        func_80130efc(obj);
    }
}
