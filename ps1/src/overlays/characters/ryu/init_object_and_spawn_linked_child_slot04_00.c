/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801b3df0_slot04_00[];
extern u8 data_801b5db8_slot04_00[];
extern u32 data_801bfc8c_slot04_00[];

void init_object_and_spawn_linked_child_slot04_00(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b3df0_slot04_00;
    obj->data_ptr_b = data_801b5db8_slot04_00;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801bfc8c_slot04_00[i];
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0;
        c->parent = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->linked_object = (u32)c;
        c->sound_side = s;
        c->image_row = obj->image_row;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}
