/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_80029348_slot12;
extern int data_8002934c_slot12;
extern u8 data_80022e30_slot12[];

void func_8001231c_slot12(int a) {
    if (data_8002934c_slot12 == 2) {
        func_801204f4(data_8002d56c_slot12, data_8002d56c_slot12->side, data_80022e30_slot12[data_80029348_slot12]);
    } else if (data_8002934c_slot12 == 1) {
        if (data_80029348_slot12 == 2) {
            func_801204f4(data_8002d56c_slot12, data_8002d56c_slot12->side, 0xe);
        } else if (data_80029348_slot12 == 0xe) {
            func_801204f4(data_8002d56c_slot12, data_8002d56c_slot12->side, 0x15);
        }
    }
    data_8002934c_slot12 = data_8002934c_slot12 - 1;
}
