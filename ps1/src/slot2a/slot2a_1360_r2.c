/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot2aSlot data_801e5164_slot2a[][2];
extern u16 *data_801e52c8_slot2a;

void func_801e14cc_slot2a(Object *obj) {
    int i;
    Cell20 *a;
    Cell20 *b;
    int y;

    a = data_801e5164_slot2a[obj->table_index][0].cells;
    b = data_801e5164_slot2a[obj->table_index][1].cells;
    y = (3 - ((Slot2aSel *)data_801e52c8_slot2a)->row_index) * 8 - 16;
    for (i = 0; i < 2; i++) {
        func_8015c100(a);
        a->pos_x = obj->pos_x + (i << 7);
        a->pos_y = obj->pos_y + y;
        a->tex_u = i << 7;
        a->tex_v = *(u8 *)data_801e52c8_slot2a * 48;
        a->width = 0x80;
        a->height = 0x30;
        a->color_r = 0x80;
        a->color_g = 0x80;
        a->color_b = 0x80;
        a->clut_or_tpage = (u16)func_8015bdd4(0x2c0, 0x80);
        a++;
        func_8015c100(b);
        b->pos_x = obj->pos_x + (i << 7);
        b->pos_y = obj->pos_y + y;
        b->tex_u = i << 7;
        b->tex_v = *(u8 *)data_801e52c8_slot2a * 48;
        b->width = 0x80;
        b->height = 0x30;
        b->color_r = 0x80;
        b->color_g = 0x80;
        b->color_b = 0x80;
        b->clut_or_tpage = (u16)func_8015bdd4(0x2c0, 0x80);
        b++;
    }
    func_80158a2c((Prim *)a, 0, 0, 0xc, 0);
    func_80158a2c((Prim *)b, 0, 0, 0xc, 0);
}
