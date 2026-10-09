/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2584_slot04_02(Object *obj);
void func_801b252c_slot04_02(Object *obj);
void func_80130678(Object *object, int arg);

void func_801b2408_slot04_02(Object *obj) {

    if (func_801b2584_slot04_02(obj) != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            func_80120554(obj, obj->side, 0x320);
            ((Slot04bObj *)obj)->field_1a4--;
            if (((Slot04bObj *)obj)->field_1a4 == 0) {
                obj->field_07++;
                func_801307e0(obj, 0x1f);
                return;
            }
        }
        func_80130efc(obj);
    } else {
        func_801b252c_slot04_02(obj);
    }
}

void func_801b24b0_slot04_02(Object *obj) {
    if (func_801b2584_slot04_02(obj) != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_07++;
            func_80130678(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        func_801b252c_slot04_02(obj);
    }
}

void func_801b252c_slot04_02(Object *obj) {
    obj->field_07++;
    func_801209c4(obj);
    obj->field_159 = 0;
    obj->field_45 = 0;
    obj->field_14 = 0;
    obj->pos_y = ((Slot04bObj *)obj)->field_70;
    func_80130678(obj, 0x11);
}
