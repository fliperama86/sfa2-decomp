/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ad8_slot28[];
extern u16 data_80041e28_slot28[];
extern u16 data_80041628_slot28[];
extern u16 data_80041a28_slot28[];
extern void (*data_80042264_slot28[])(Object *);

void func_800200ec_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051ad8_slot28[i];
        if (p != 0 && p->field_00 != 0) {
            func_8011f240((Slab172 *)p);
            data_80051ad8_slot28[i] = 0;
        }
    }
}

void func_80020158_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_80041e28_slot28[i];
        data_801a27e4_rows[5][i] = data_80041e28_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_80041628_slot28[i];
        data_801a27e4_rows[7][i] = data_80041628_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_80041a28_slot28[i];
        data_801a27e4_rows[8][i] = data_80041a28_slot28[i];
    }
}

void func_8002022c_slot28(Object *object) {
    data_80042264_slot28[object->field_04](object);
}
