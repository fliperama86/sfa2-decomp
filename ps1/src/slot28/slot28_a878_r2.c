/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051998_slot28[];
extern u16 data_80036ff4_slot28[];
extern u16 data_800367f4_slot28[];
extern u16 data_80036bf4_slot28[];
extern void (*data_8003742c_slot28[])(Object *);

void func_8001a9e8_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051998_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051998_slot28[i] = 0;
        }
    }
}

void func_8001aa54_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_80036ff4_slot28[i];
        data_801a27e4_rows[5][i] = data_80036ff4_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_800367f4_slot28[i];
        data_801a27e4_rows[7][i] = data_800367f4_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_80036bf4_slot28[i];
        data_801a27e4_rows[8][i] = data_80036bf4_slot28[i];
    }
}

void func_8001ab28_slot28(Object *object) {
    data_8003742c_slot28[object->field_04](object);
}
