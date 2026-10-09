/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b1c00_slot04_0a(Object *obj);
void func_801b2048_slot04_0a(Object *obj);

void func_801b1e6c_slot04_0a(Object *obj) {
    int k;

    if (obj->field_50 < 0) {
        if ((func_80151184() & 0xf) != 0) {
            k = func_80151184() & 3;
            if (k != 3) {
                obj->field_07++;
                func_801307e0(obj, k | 0x28);
                func_80141f28(obj, 2);
                func_80138ae8(&game_state, obj);
                func_801b2048_slot04_0a(obj);
                if (func_801b1e04_slot04_0a(obj)) {
                    func_801b1c00_slot04_0a(obj);
                }
                return;
            }
        }
    }
    func_801b2048_slot04_0a(obj);
    if (func_801b1e04_slot04_0a(obj)) {
        func_801b1c00_slot04_0a(obj);
    }
    func_80130efc(obj);
}
