/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);
int func_801b1c5c_slot04_00(Object *obj);
void func_801b1fb8_slot04_00(Object *obj);

void func_801b1e58_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) != 0) {
        func_801b1fb8_slot04_00(obj);
    } else {
        if ((s16)obj->field_3a & 0x8000) {
            func_80120554(obj, obj->side, 0x320);
            ((Slot04aObj *)obj)->field_1a4--;
            if (((Slot04aObj *)obj)->field_1a4 & 0x80) {
                obj->field_07++;
                func_801307e0(obj, 0x31);
                return;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b1ef8_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) != 0) {
        func_801b1fb8_slot04_00(obj);
    } else if ((s16)obj->field_3a & 0x8000) {
        obj->field_07++;
        func_80130678(obj, 0x21);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1f70_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) == 0) {
        func_80130efc(obj);
    } else {
        func_801b1fb8_slot04_00(obj);
    }
}
