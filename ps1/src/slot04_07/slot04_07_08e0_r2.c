/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *object);

u8 func_8013f8c4(Object *object, int a, int b);
int func_80141618(Object *object);
int func_801412a4(Object *object);

void func_801b0998_slot04_07(Object *obj) {
    u8 one;

    obj->field_07++;
    ((Slot04aObj *)obj)->field_1ca = 0;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x12, 0x10) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        if (obj->field_12a == 4) {
            obj->field_07 = 2;
            obj->field_278 = one;
        }
    }
}

void func_801b0a44_slot04_07(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_801312b8(obj);
    } else {
        if ((t & 0x80) != 0 && func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_25f = 0xff;
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
