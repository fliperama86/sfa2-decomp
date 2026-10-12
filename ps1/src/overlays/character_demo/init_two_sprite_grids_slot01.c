/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot01Rec31b98 data_80031b98_slot01[];

void init_two_sprite_grids_slot01(Object *obj) {
    Cell20 *a = data_80031b98_slot01[0].cells;
    Cell20 *b = data_80031b98_slot01[1].cells;
    int i;
    int j;

    obj->field_01 = 0;
    obj->health = 0;
    ((Slot01Obj *)obj)->min_limit = -0x1f;
    obj->state_0 = obj->state_0 + 1;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            func_8015c100(a);
            a->pos_x = j << 7;
            a->pos_y = i << 5;
            a->tex_u = 0;
            a->tex_v = 0;
            a->width = 0x80;
            a->color_r = 0x80;
            a->color_g = 0x80;
            a->color_b = 0x80;
            a->height = 0x20;
            a->clut_or_tpage = func_8015bdd4(0x10, i + 0x1e0);
            a++;
            func_8015c100(b);
            b->pos_x = j << 7;
            b->pos_y = i << 5;
            b->tex_u = 0;
            b->tex_v = 0;
            b->width = 0x80;
            b->color_r = 0x80;
            b->color_g = 0x80;
            b->color_b = 0x80;
            b->height = 0x20;
            b->clut_or_tpage = func_8015bdd4(0x10, i + 0x1e0);
            b++;
        }
    }
    func_80158a2c((Prim *)a, 0, 0, 0x1a, 0);
    func_80158a2c((Prim *)b, 0, 0, 0x1a, 0);
}
