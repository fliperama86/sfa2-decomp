/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141b28(Object *object);
void func_801b10dc_slot04_08(Object *obj);

int func_801b108c_slot04_08(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b10dc_slot04_08(obj);
    }
    return r;
}
