/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b7a4c_slot04_02(Object *obj);
void func_801b7b10_slot04_02(Object *obj);
void func_801b7b44_slot04_02(Object *obj);
void func_801b7b74_slot04_02(Object *obj);

void func_801b79b4_slot04_02(Object *obj) {
    Object *p = obj->parent;

    obj->state_0++;
    obj->field_1c = p->field_1c;
    obj->table_index = p->kind;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->spawn_x = 0x60;
    obj->variant_index = 0;
    obj->spawn_y = 0x1e0;
    obj->field_90 = p->field_90;
    obj->data_ptr_a = p->data_ptr_a;
    obj->data_ptr_b = p->data_ptr_b;
    func_801b7a4c_slot04_02(obj);
}

void func_801b7a4c_slot04_02(Object *obj) {
    Object *p = obj->parent;
    u16 t;
    u16 u;
    u16 w;

    obj->field_01 = 0;
    if (obj->table_index != p->kind) {
        func_801b7b10_slot04_02(obj);
    } else {
        w = p->step_flags;
        t = w & 0xff;
        u = t;
        if (u == 0) {
            obj->variant_index = 0;
        } else if ((w & 0x80) != 0) {
            func_801b7b10_slot04_02(obj);
        } else {
            func_801b7b74_slot04_02(obj);
            obj->field_01 = 1;
            if (obj->variant_index != u) {
                obj->variant_index = t;
                func_801b7b44_slot04_02(obj);
            } else {
                advance_sequence_checked(obj);
                func_8011ffdc(obj);
            }
        }
    }
}
