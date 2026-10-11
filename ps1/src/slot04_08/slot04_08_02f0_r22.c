/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3dec_slot04_08[];

void func_801b4728_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4790_slot04_08(Object *obj) {
    data_801c3dec_slot04_08[obj->state_3](obj);
}
