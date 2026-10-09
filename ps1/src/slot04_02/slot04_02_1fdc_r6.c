/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);
void func_80130678(Object *object, int arg);

void func_801b2810_slot04_02(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_07++;
            obj->field_159 = 0;
            func_80130678(obj, 0x21);
        } else {
            func_80130efc(obj);
        }
    } else {
        obj->field_07 = 4;
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        n = 0x5e;
        if (obj->field_48 != 1) {
            n = 0x5f;
        }
        func_801307e0(obj, n);
    }
}

void func_801b28bc_slot04_02(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) == 0) {
        obj->field_07 = 4;
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        n = 0x5e;
        if (obj->field_48 != 1) {
            n = 0x5f;
        }
        func_801307e0(obj, n);
    } else {
        func_80130efc(obj);
    }
}
