/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8003025c_slot28[];
extern u16 data_8002fe5c_slot28[];
extern void (*data_80030694_slot28[])(Object *);

void func_80016ee8_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8003025c_slot28[i];
        data_801a27e4_rows[5][i] = data_8003025c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8002fe5c_slot28[i];
        data_801a27e4_rows[7][i] = data_8002fe5c_slot28[i];
    }
}

void func_80016f78_slot28(Object *object) {
    data_80030694_slot28[object->field_04](object);
}
