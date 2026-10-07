/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c5758_slot04_06;

void func_801b5c5c_slot04_06(Object *obj);
void func_801b5d68_slot04_06(Object *obj, int a);
void func_801b5d10_slot04_06(Object *obj);

void func_801b5bb8_slot04_06(Object *obj) {
    Object *p;

    obj->field_04++;
    p = data_801c5758_slot04_06;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    obj->field_03 = p->kind;
    obj->field_0c = data_801c5758_slot04_06->field_0c;
    obj->field_0d = data_801c5758_slot04_06->field_0d;
    obj->field_0e = data_801c5758_slot04_06->field_0e;
    obj->field_48 = 0;
    func_801b5c5c_slot04_06(obj);
}

void func_801b5c5c_slot04_06(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c5758_slot04_06;
    if (obj->field_03 == p->kind) {
        a = p->frame->field_09;
        if (a == 0) {
            obj->field_48 = 0;
        } else {
            obj->pos_x = p->pos_x;
            obj->pos_y = p->pos_y;
            obj->field_0b = p->field_0b;
            obj->field_01 = 1;
            if (obj->field_48 != a) {
                func_801b5d68_slot04_06(obj, a);
            } else {
                func_80131094(obj);
            }
        }
    } else {
        func_801b5d10_slot04_06(obj);
    }
}
