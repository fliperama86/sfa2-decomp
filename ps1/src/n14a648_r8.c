/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014e6d0(void);
void func_8014e710(void);

u8 func_8014e654(Object *unused) {
    if (data_80189468 == 0 || data_80189468 == 2) {
        func_8014e6d0();
        return 1;
    } else if (data_80189468 == 4 || data_80189468 == 6) {
        func_8014e710();
    }
    return 1;
}
