/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot2aColB data_801e226c_slot2a[];
extern Slot2aColB data_801e226e_slot2a[];
extern Slot2aColH data_801e2270_slot2a[];
extern Slot2aColH data_801e2272_slot2a[];
extern Slot2aSpawn data_801e22b4_slot2a[];
extern Slot2aBank data_801e2fdc_slot2a[];
extern u32 data_801e515c_slot2a;

void func_801e0e48_slot2a(Object *obj) {
    Slot2aSpawn *e;
    Cell20 *a;
    Cell20 *b;
    short base;
    int j;
    int n;

    a = data_801e2fdc_slot2a[obj->field_03].left;
    b = data_801e2fdc_slot2a[obj->field_03].right;
    e = data_801e22b4_slot2a;
    data_801e515c_slot2a = 0;
    for (; e->key != 0x80; e++) {
        base = 0;
        for (j = 0; j < e->count; j++) {
            func_8015c100(a);
            a->pos_x = base + e->field_02;
            a->pos_y = e->field_04 + (obj->field_03 ? 0x91 : 0x31);
            a->tex_u = *(u8 *)((char *)data_801e226c_slot2a + e->key * 8) + 0xc0;
            a->tex_v = *(u8 *)((char *)data_801e226e_slot2a + e->key * 8);
            a->width = *(u16 *)((char *)data_801e2270_slot2a + e->key * 8);
            a->height = *(u16 *)((char *)data_801e2272_slot2a + e->key * 8);
            a->color_r = 0x80;
            a->color_g = 0x80;
            a->color_b = 0x80;
            a->field_0e = func_8015bdd4(0x70, 0x1fa);
            a++;
            func_8015c100(b);
            b->pos_x = base + e->field_02;
            b->pos_y = e->field_04 + (obj->field_03 ? 0x91 : 0x31);
            b->tex_u = *(u8 *)((char *)data_801e226c_slot2a + e->key * 8) + 0xc0;
            b->tex_v = *(u8 *)((char *)data_801e226e_slot2a + e->key * 8);
            b->width = *(u16 *)((char *)data_801e2270_slot2a + e->key * 8);
            b->height = *(u16 *)((char *)data_801e2272_slot2a + e->key * 8);
            b->color_r = 0x80;
            b->color_g = 0x80;
            b->color_b = 0x80;
            b->field_0e = func_8015bdd4(0x70, 0x1fa);
            b++;
            base += *(u16 *)((char *)data_801e2270_slot2a + e->key * 8);
            data_801e515c_slot2a++;
        }
    }
    func_80158a2c((Prim *)a, 0, 0, 0xb, 0);
    func_80158a2c((Prim *)b, 0, 0, 0xb, 0);
}
