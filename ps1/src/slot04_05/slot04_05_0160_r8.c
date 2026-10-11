/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0adc_slot04_05(Object *obj) {
    func_80142a14(obj);
}

void func_801b0afc_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        update_sequence(obj);
    }
}
