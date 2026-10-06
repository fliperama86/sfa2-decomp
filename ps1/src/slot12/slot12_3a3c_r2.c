/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800243d4_slot12[];
extern u16 data_8002413c_slot12[];
extern u16 data_8002b6f0_slot12[16];
extern s16 data_8002b710_slot12[16];
extern u16 data_8002b730_slot12[20][16];
extern u16 data_8002b9b0_slot12[20][16];
extern u16 data_8002bc30_slot12;
extern u16 data_8002bc34_slot12;
void func_80013c74_slot12(void);

void func_80013b5c_slot12(int a) {
    int i;
    s16 c;
    for (i = 0; i < 16; i++) {
        data_8002b6f0_slot12[i] = data_800243d4_slot12[a * 16 + i];
    }
    c = 0x8421;
    for (i = 15; i >= 0; i--) {
        data_8002b710_slot12[i] = c;
    }
    data_8002bc30_slot12 = 0;
    data_8002bc34_slot12 = 0x1f;
    func_80013c74_slot12();
}

void func_80013bec_slot12(void) {
    int j;
    int i;
    u16 *src;
    for (j = 0; j < 20; j++) {
        for (i = 15; i >= 0; i--) {
            data_8002b9b0_slot12[j][i] = 0x421;
        }
    }
    src = data_8002413c_slot12;
    for (j = 0; j < 20; j++) {
        for (i = 0; i < 16; i++) {
            data_8002b730_slot12[j][i] = *src++;
        }
    }
}
