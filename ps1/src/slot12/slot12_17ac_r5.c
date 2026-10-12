/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Bank data_80028b48_slot12[];

void init_sprite_cells_from_list_slot12(Object *obj, Slot12List *list) {
    Cell20 *a = data_80028b48_slot12[0].cells;
    Cell20 *b = data_80028b48_slot12[1].cells;
    int i;
    int t;
    u16 n = list->count;

    for (i = 0; i < n; i++) {
        func_8015c100(a);
        func_8015c100(b);
        a->color_r = 0x80;
        b->color_r = 0x80;
        a->color_g = 0x80;
        b->color_g = 0x80;
        a->color_b = 0x80;
        b->color_b = 0x80;
        t = i / 16 * 16;
        a->tex_u = (i - t) << 4;
        b->tex_u = (i - t) << 4;
        a->tex_v = t;
        b->tex_v = t;
        a->clut_or_tpage = func_8015bdd4((s16)obj->spawn_x, (s16)obj->spawn_y);
        b->clut_or_tpage = func_8015bdd4((s16)obj->spawn_x, (s16)obj->spawn_y);
        a->width = 0x10;
        b->width = 0x10;
        a->height = 0x10;
        b->height = 0x10;
        a++;
        b++;
    }
    func_80158a2c((Prim *)a, 0, 0, 0x16, 0);
    func_80158a2c((Prim *)b, 0, 0, 0x16, 0);
}
