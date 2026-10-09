/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c7884_slot04_02[];
extern u16 data_801c7b44_slot04_02[];

void func_801b7e34_slot04_02(Object *obj, u8 arg) {
    s16 i;

    for (i = 1; i < 16; i++) {
        if (arg == 0) {
            data_801a2bc4[i] = data_801c7884_slot04_02[i];
        } else {
            data_801a2bc4[i] = data_801c7b44_slot04_02[i];
        }
    }
    func_80137220(0, 6);
}

void func_801b7ebc_slot04_02(Object *obj) {
    s16 i;
    int row;

    row = obj->field_a0 << 4;
    for (i = 1; i < 16; i++) {
        if (obj->field_03 == 0) {
            data_801a2bc4[i] = data_801c7884_slot04_02[row + i];
        } else {
            data_801a2bc4[i] = data_801c7b44_slot04_02[row + i];
        }
    }
    func_80137220(0, 6);
    obj->field_a0++;
}
