/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2a20_slot04_02(Object *obj) {
    obj->field_46 -= 0x100;
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x1e, 0x30);
        func_801482e0(obj, -0x2c, 0x30);
    }
    func_80130efc(obj);
}
