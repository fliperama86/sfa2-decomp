/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b377c_slot04_04(Object *obj) {
    obj->field_07++;
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 4);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (!((s16)(box_margin[0] + 0xc0) < obj->pos_x)) {
            obj->field_0b = 1;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    obj->field_46 = (u8)obj->field_46 + 0xc00;
    func_801307e0(obj, 0x19);
}

void func_801b3838_slot04_04(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
    }
}
