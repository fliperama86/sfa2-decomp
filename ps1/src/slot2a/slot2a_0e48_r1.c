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
extern int data_801e515c_slot2a;
int func_8015bdd4(int a, int b);

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
            a->field_08 = base + e->field_02;
            a->field_0a = e->field_04 + (obj->field_03 ? 0x91 : 0x31);
            a->field_0c = *(u8 *)((char *)data_801e226c_slot2a + e->key * 8) + 0xc0;
            a->field_0d = *(u8 *)((char *)data_801e226e_slot2a + e->key * 8);
            a->field_10 = *(u16 *)((char *)data_801e2270_slot2a + e->key * 8);
            a->field_12 = *(u16 *)((char *)data_801e2272_slot2a + e->key * 8);
            a->field_04 = 0x80;
            a->field_05 = 0x80;
            a->field_06 = 0x80;
            a->field_0e = func_8015bdd4(0x70, 0x1fa);
            a++;
            func_8015c100(b);
            b->field_08 = base + e->field_02;
            b->field_0a = e->field_04 + (obj->field_03 ? 0x91 : 0x31);
            b->field_0c = *(u8 *)((char *)data_801e226c_slot2a + e->key * 8) + 0xc0;
            b->field_0d = *(u8 *)((char *)data_801e226e_slot2a + e->key * 8);
            b->field_10 = *(u16 *)((char *)data_801e2270_slot2a + e->key * 8);
            b->field_12 = *(u16 *)((char *)data_801e2272_slot2a + e->key * 8);
            b->field_04 = 0x80;
            b->field_05 = 0x80;
            b->field_06 = 0x80;
            b->field_0e = func_8015bdd4(0x70, 0x1fa);
            b++;
            base += *(u16 *)((char *)data_801e2270_slot2a + e->key * 8);
            data_801e515c_slot2a++;
        }
    }
    func_80158a2c((Prim *)a, 0, 0, 0xb, 0);
    func_80158a2c((Prim *)b, 0, 0, 0xb, 0);
}
