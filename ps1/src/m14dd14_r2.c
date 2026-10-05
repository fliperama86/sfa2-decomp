/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014eb9c(int a, int b) {
    func_801203b4(0);
    func_801203b4(1);
    if (a == b) {
        if (b == 0xd) {
            func_8014ed04();
        } else if (b == 0x10) {
            func_8014ed3c();
        } else if (b == 0x11 || b == 0x13) {
            func_8014ed74();
        } else goto other;
    } else {
other:
        if ((a == 0x11 || a == 0x13) && (b == 0x11 || b == 0x13)) {
            func_8014ed74();
        } else {
            func_8014ec80(a, b);
        }
    }
    player_left.field_90 = (void *)data_801a27d8;
    player_right.field_90 = (void *)data_801a27dc;
}
