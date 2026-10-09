/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051a98_slot28[];
void func_8011f240(Slab172 *s);
extern u16 data_8003f184_slot28[];
extern u16 data_8003f584_slot28[];
extern void (*data_8003f9c0_slot28[])(Object *);
extern u8 data_8003f9d0_slot28[];

void func_8001ef04_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051a98_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051a98_slot28[i] = 0;
        }
    }
}

void func_8001ef70_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8003f184_slot28[i];
        data_801a27e4_rows[5][i] = data_8003f184_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8003f584_slot28[i];
        data_801a27e4_rows[7][i] = data_8003f584_slot28[i];
    }
}

void func_8001f000_slot28(Object *object) {
    data_8003f9c0_slot28[object->field_04](object);
}

void func_8001f040_slot28(Object *o) {
    o->field_0e = 0;
    o->field_0c = 0;
    o->field_0b = 0;
    o->field_48 = 0;
    o->field_04++;
    func_80130768(o, data_8003f9d0_slot28[o->field_03], (SequenceStep **)o->box_tables);
}
