/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141b28(Object *object);
/* functions of other units of this module */
void func_801b1378_slot04_12(Object *obj);
u8 func_801b1328_slot04_12(Object *obj);

u8 func_801b1328_slot04_12(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b1378_slot04_12(obj);
    }
    return r;
}
