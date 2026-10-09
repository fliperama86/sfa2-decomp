/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b35d4_slot04_04(Object *obj) {
    obj->field_07++;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 3);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (!((s16)(box_margin[0] + 0xc0) < obj->pos_x)) {
            obj->field_0b = 1;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x18);
}

void func_801b3684_slot04_04(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_801204f4(obj, obj->side, 0xc);
        func_80140770(obj, 4, 0xa, 0xf, 0, 0, 1);
    }
    func_80130efc(obj);
}
