/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8003940c_slot28[];
extern u16 data_8003980c_slot28[];
extern u16 data_80039c0c_slot28[];
extern void (*data_8003a148_slot28[])(Object *);

void func_8001c52c_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8003940c_slot28[i];
        data_801a27e4_rows[5][i] = data_8003940c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8003980c_slot28[i];
        data_801a27e4_rows[7][i] = data_8003980c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_80039c0c_slot28[i];
        data_801a27e4_rows[8][i] = data_80039c0c_slot28[i];
    }
}

void func_8001c600_slot28(Object *object) {
    data_8003a148_slot28[object->field_04](object);
}
