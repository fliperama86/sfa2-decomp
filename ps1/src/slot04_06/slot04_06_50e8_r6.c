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

    obj->state_0++;
    p = data_801c5758_slot04_06;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->table_index = p->kind;
    obj->field_0c = data_801c5758_slot04_06->field_0c;
    obj->image_row = data_801c5758_slot04_06->image_row;
    obj->field_0e = data_801c5758_slot04_06->field_0e;
    obj->variant_index = 0;
    func_801b5c5c_slot04_06(obj);
}

void func_801b5c5c_slot04_06(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c5758_slot04_06;
    if (obj->table_index == p->kind) {
        a = p->frame->frame_param;
        if (a == 0) {
            obj->variant_index = 0;
        } else {
            obj->pos_x = p->pos_x;
            obj->pos_y = p->pos_y;
            obj->facing_side = p->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != a) {
                func_801b5d68_slot04_06(obj, a);
            } else {
                advance_sequence_checked(obj);
            }
        }
    } else {
        func_801b5d10_slot04_06(obj);
    }
}
