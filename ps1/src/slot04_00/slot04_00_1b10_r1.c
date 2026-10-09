/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1c5c_slot04_00(Object *obj);

void func_801b1b10_slot04_00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80120554(obj, obj->side, 0x320);
        ((Slot04aObj *)obj)->field_1a4--;
        if (((Slot04aObj *)obj)->field_1a4 & 0x80) {
            obj->field_07++;
            func_801307e0(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
    func_80130efc(obj);
}

void func_801b1ba4_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) != 0) {
        func_801209c4(obj);
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        obj->field_07++;
        func_80130678(obj, 0x11);
    } else {
        func_80130efc(obj);
    }
}
