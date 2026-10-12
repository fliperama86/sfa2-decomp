/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1a08_slot04_04(Object *obj);

void func_801b19bc_slot04_04(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        func_801b1a08_slot04_04(obj);
    }
}

void func_801b1a08_slot04_04(Object *obj) {
    u16 a;

    obj->state_3++;
    a = 0x21;
    obj->active_flag_4 = 0;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    func_801307e0(obj, a + obj->direction_index + 1);
}
