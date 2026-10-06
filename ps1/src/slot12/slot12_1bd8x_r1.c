/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Bank data_80028b48_slot12[];

void func_80011bd8_slot12(Object *obj, Slot12List *list) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    Cell20 *c;
    u32 *t;
    s16 *e;
    int i;
    int x;
    int y;
    int px;
    int py;
    u32 pa;
    u16 n = list->count;

    c = data_80028b48_slot12[data_801a27d0].cells;
    e = list->entries;
    t = (u32 *)((char *)data_801987c8 + 0x24);
    for (i = 0; i < n; i++) {
        x = *e++;
        y = *e++;
        px = ((x + 8) * obj->field_20) >> 4;
        py = ((y + 8) * obj->field_22) >> 4;
        c->field_08 = px + x + obj->pos_x;
        c->field_0a = py + y + obj->pos_y;
        pa = (u32)c & 0xffffff;
        *(u32 *)c = (*(u32 *)c & 0xff000000) | (*t & 0xffffff);
        *t = (*t & 0xff000000) | pa;
        c++;
    }
    *(u32 *)c = (*(u32 *)c & 0xff000000) | (*t & 0xffffff);
    *t = (*t & 0xff000000) | ((u32)c & 0xffffff);
}
