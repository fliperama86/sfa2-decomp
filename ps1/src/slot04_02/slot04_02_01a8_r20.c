/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2584_slot04_02(Object *object);
void func_801b252c_slot04_02(Object *object);

void func_801b21a4_slot04_02(Object *obj) {

    if ((s16)obj->field_3a & 0x8000) {
        func_80120554(obj, obj->side, 0x320);
        ((Slot04bObj *)obj)->field_1a4--;
        if (((Slot04bObj *)obj)->field_1a4 == 0) {
            obj->field_07++;
            func_801307e0(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->field_10 += obj->field_4c;
    func_80130efc(obj);
}

void func_801b223c_slot04_02(Object *obj) {
    if (func_801b2584_slot04_02(obj) == 0) {
        func_801b252c_slot04_02(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2288_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
