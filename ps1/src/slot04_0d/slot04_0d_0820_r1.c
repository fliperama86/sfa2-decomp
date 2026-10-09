/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2978_slot04_0d[];

void func_80142a14(Object *object);
void func_80130dc0(Object *object);

/* functions of other units of this module */

void func_801b0820_slot04_0d(Object *obj) {
    func_80142a14(obj);
}

void func_801b0840_slot04_0d(Object *obj) {
    s16 t = obj->field_3a;
    int d;

    if (t >= 0) {
        if ((t & 0xff) == 0) {
            func_80130efc(obj);
        } else {
            if (obj->field_0b == 0) {
                d = -2;
            } else {
                d = 2;
            }
            obj->pos_x = d + obj->pos_x;
            func_80130efc(obj);
        }
    } else {
        func_801312b8(obj);
    }
}

void func_801b08bc_slot04_0d(Object *obj) {
    obj->field_157 = 1;
    data_801c2978_slot04_0d[obj->field_07](obj);
}

void func_801b0900_slot04_0d(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b0930_slot04_0d(Object *obj) {
    func_80142a14(obj);
}
