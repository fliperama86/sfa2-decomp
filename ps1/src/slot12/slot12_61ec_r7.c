/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Prim data_8002c528_slot12[];
extern u8 data_80028970_slot12[];
void func_80014434_slot12(Object *obj, Slot12Prim *a, int b, int c);

void func_80016864_slot12(Object *obj) {
    Slot12Prim *a = data_8002c528_slot12;
    int i;
    int r;
    u8 *src;

    for (i = 0; i <= ((Slot12Obj *)obj)->field_5c; i++) {
        func_80014434_slot12(obj, a, 0x10, 0x10);
        a->tpage = 0x7f07;
        r = func_8015bd0c(0, 0, 0x3c0, 0x100);
        a->cmd = 0xe1000000 + (((Object *)data_801987c8)->field_a2 << 9) + (((Object *)data_801987c8)->field_a3 << 10) + (r & 0x1f);
        a++;
        a->tpage = 0x7f07;
        r = func_8015bd0c(0, 0, 0x3c0, 0x100);
        a->cmd = 0xe1000000 + (((Object *)data_801987c8)->field_a2 << 9) + (((Object *)data_801987c8)->field_a3 << 10) + (r & 0x1f);
        a++;
    }
    a = data_8002c528_slot12;
    src = (u8 *)obj->field_50;
    for (i = 0; i <= ((Slot12Obj *)obj)->field_5c; i++) {
        u8 c = *src++;
        int k = c & 0x7f;
        u8 u;
        u8 v;

        if ((c & 0x80) == 0) {
            u = data_80028970_slot12[k * 4];
            v = data_80028970_slot12[k * 4 + 1];
        } else {
            u = data_80028970_slot12[k * 4 + 2];
            v = data_80028970_slot12[k * 4 + 3];
        }
        a->u = u;
        a->v = v;
        a->x = obj->pos_x + i * 16 + 0x10;
        a->y = obj->pos_y;
        a++;
        a->u = u;
        a->v = v;
        a->x = obj->pos_x + i * 16 + 0x10;
        a->y = obj->pos_y;
        a++;
    }
    obj->field_4c = ((Slot12Obj *)obj)->field_5c * 16 + 0xc0;
}
