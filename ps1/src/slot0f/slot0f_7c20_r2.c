/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Rec data_800f7d60_slot0f[];
extern u8 data_800f8070_slot0f[][10];
void func_800e7f90_slot0f(u8 *src, u8 *dst, int mode);

void func_800e7e8c_slot0f(Object *obj, int a) {
    Slot12Rec *e;
    u8 *fr;

    fr = (u8 *)ptr_8019040c;
    fr += (s16)obj->health << 4;
    e = &data_800f7d60_slot0f[obj->table_index];
    e->field_08 = 0x10;
    e->field_09 = 0x10;
    e->field_0a = 2;
    e->field_0b = 0x12;
    e->field_00 = 0;
    if (a == 1) {
        e->field_0b = 0x1d;
    }
    fr += 4;
    func_800e7f90_slot0f(fr, data_800f8070_slot0f[obj->table_index], 0);
    e->data_ptr = data_800f8070_slot0f[obj->table_index];
    e->pos_x = obj->base_x + 0x30;
    e->pos_y = obj->base_y;
    e->pos_x = e->pos_x - 8;
    e->pos_y = e->pos_y - 8;
}
