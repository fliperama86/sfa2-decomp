/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, u16 arg);
void func_801b39c8_slot04_0d(Object *object, GameState *g);

void func_801b3920_slot04_0d(Object *object) {
    int a;
    s8 r;

    if ((s16)object->field_3a < 0) {
        r = func_80151184();
        a = r;
        if ((a & 3) != 0) {
            a = 0x32;
        } else {
            a = 0x33;
        }
        func_80130678(object, (u8)a);
    }
    func_801b39c8_slot04_0d(object, &game_state);
    func_80130efc(object);
}
