/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2068_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = 0x1e;
        obj->state_3++;
    }
    update_sequence(obj);
}

void func_801b20a8_slot04_05(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, 0x26);
    }
}
