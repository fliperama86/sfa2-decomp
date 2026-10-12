/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b22bc_slot04_07(Object *obj);
void func_801b2428_slot04_07(Object *obj);

void func_801b2534_slot04_07(Object *obj) {
    s16 t = obj->countdown;

    if (t != 0) {
        t -= 1;
        obj->countdown = t;
        if (obj->pressed_bits & 0x68) {
            func_801b22bc_slot04_07(obj);
        } else {
            func_801b2428_slot04_07(obj);
        }
    } else {
        func_801b2428_slot04_07(obj);
    }
}
