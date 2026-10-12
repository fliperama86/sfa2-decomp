/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Rec data_800f7d70_slot0f[];
extern u8 data_800f7fd0_slot0f[][16];
void func_800e8314_slot0f(u8 *buf, unsigned int n, int pos, int flags);
void func_800e7f90_slot0f(u8 *src, u8 *dst, int mode);

void func_800e7c20_slot0f(Object *obj, int a) {
    Slot12Rec *e;
    u8 *fr;
    u8 buf[16];

    fr = (u8 *)ptr_8019040c;
    fr += (s16)obj->health << 4;
    e = &data_800f7d70_slot0f[obj->table_index];
    if (game_state.mode_flag_2 == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_800e8314_slot0f(buf, *(int *)fr, 7, 0);
        func_800e7f90_slot0f(buf, data_800f7fd0_slot0f[obj->table_index], 0);
        e->data_ptr = data_800f7fd0_slot0f[obj->table_index];
        e->pos_x = obj->base_x + 0x70;
        e->pos_y = obj->base_y;
        e->pos_x = e->pos_x - 8;
        e->pos_y = e->pos_y - 8;
    } else if (obj->table_index == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_800e8314_slot0f(buf, *(int *)fr, 7, 0);
        func_800e7f90_slot0f(buf, data_800f7fd0_slot0f[obj->table_index], 1);
        e->data_ptr = data_800f7fd0_slot0f[obj->table_index];
        e->pos_x = obj->base_x + 0x38;
        e->pos_y = obj->base_y + 0x20;
        e->pos_x = e->pos_x - 8;
        e->pos_y = e->pos_y - 8;
    } else {
        e->field_08 = 8;
        e->field_09 = 0x10;
        e->field_0a = 1;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_800e8314_slot0f(buf, *(int *)fr, 7, 0);
        func_800e7f90_slot0f(buf, data_800f7fd0_slot0f[obj->table_index], 0);
        e->data_ptr = data_800f7fd0_slot0f[obj->table_index];
        e->pos_x = obj->base_x + 0xa0;
        e->pos_y = obj->base_y;
        e->pos_x = e->pos_x - 4;
        e->pos_y = e->pos_y - 8;
    }
}
