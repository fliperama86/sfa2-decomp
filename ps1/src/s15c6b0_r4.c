/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015eff0(int a, void (*b)(void));

void func_801656ec(void) {
    if (data_801904d4 == 0) {
        data_8018dae8 = 0;
        func_801575dc();
        if (data_8018dadc != 0) {
            func_8015f050(0);
            data_8018dadc = 0;
        } else if (data_8018dae4 != -1) {
            if (data_8018dae4 == 0) {
                func_8015eff0(0, data_8018daf0);
            } else {
                func_8015eff0(data_8018dae4, 0);
            }
            data_8018dae4 = -1;
        }
        func_8015786c();
    }
}
