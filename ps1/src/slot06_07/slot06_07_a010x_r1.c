/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801ea094_slot06_07(Object *obj);

extern Object *(*data_801ee1e8_slot06_07[])(void);

Object *func_801ea010_slot06_07(void) {
    Object *l = &player_left;
    u8 i;
    i = func_801ea094_slot06_07(l) != 0;
    if (func_801ea094_slot06_07(l + 1) != 0) {
        i |= 2;
    }
    return data_801ee1e8_slot06_07[i]();
}
