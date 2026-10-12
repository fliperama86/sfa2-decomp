/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The two locals are read without having been set: the original stores two saved registers that it never assigns. */
void func_801b4e38_slot04_0a(Object *obj) {
    Object *p[3];
    u8 a;
    int b;
    Object *t;

    if (data_80197f10 >= 3) {
        func_801460ec((u32 *)p);
        t = p[0];
        t->active_count = 1;
        t->field_02 = 0x15;
        t->table_index = 0;
        t->field_0e = obj->field_0e;
        t->variant_index = a;
        t->vel_x = b;
        t->countdown = 8;
        t->field_20 = 8;
        t->parent = obj;
        t->image_row = obj->image_row + 3;
        t->spawn_flag = 1;
        t->vel_y = 0x1000800;
        t->spawn_x = obj->spawn_x;
        t->spawn_y = obj->spawn_y;
        t->mode_id = 0x20;
        t->sound_side = obj->sound_side;
        t = p[1];
        t->active_count = 1;
        t->field_02 = 0x15;
        t->table_index = 1;
        t->field_0e = obj->field_0e;
        t->variant_index = a;
        t->vel_x = b;
        t->countdown = 0x10;
        t->field_20 = 0x10;
        t->parent = obj;
        t->image_row = obj->image_row + 4;
        t->image_row = obj->image_row;
        t->spawn_flag = 1;
        t->vel_y = 0x1000800;
        t->spawn_x = obj->spawn_x;
        t->spawn_y = obj->spawn_y;
        t->mode_id = 0x20;
        t->sound_side = obj->sound_side;
        t = p[2];
        t->active_count = 1;
        t->field_02 = 0x15;
        t->table_index = 2;
        t->field_0e = obj->field_0e;
        t->variant_index = a;
        t->vel_x = b;
        t->countdown = 0x18;
        t->field_20 = 0x18;
        t->parent = obj;
        t->image_row = obj->image_row + 4;
        t->image_row = obj->image_row;
        t->spawn_flag = 1;
        t->vel_y = 0x1000800;
        t->spawn_x = obj->spawn_x;
        t->spawn_y = obj->spawn_y;
        t->mode_id = 0x20;
        t->sound_side = obj->sound_side;
    }
}
