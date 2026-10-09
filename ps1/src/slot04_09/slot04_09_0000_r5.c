/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6210_slot04_09(Object *obj);
void func_801b5ab4_slot04_09(Object *obj);

void func_801b0568_slot04_09(Object *obj) {
    u16 t = obj->field_3a;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
    }
    u = obj->field_3a;
    if ((u & 0xff) == 2) {
        obj->field_3a = u & 0xff00;
        func_801b6210_slot04_09(obj);
    }
    u = obj->field_3a;
    if ((u & 0xff) == 3) {
        obj->field_3a = u & 0xff00;
        func_80130678(obj, 0x32);
    }
    func_80130efc(obj);
}

void func_801b05f4_slot04_09(Object *obj) {
    u16 t;
    u16 u;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((t & 0xff) == 1) {
        obj->field_3a = t & 0xff00;
        func_801204f4(obj, obj->side, 0xf);
    }
    u = obj->field_3a;
    if ((u & 0xff) == 2) {
        obj->field_3a = u & 0xff00;
        func_801b5ab4_slot04_09(obj);
    }
    u = obj->field_3a;
    if ((u & 0xff) == 3) {
        obj->field_3a = u & 0xff00;
        if (obj->field_0b != 0) {
            obj->pos_x += 8;
        } else {
            obj->pos_x -= 8;
        }
    }
}
