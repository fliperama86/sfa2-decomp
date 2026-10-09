/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);
void func_801b3e68_slot04_0a(Object *obj);

void func_801b235c_slot04_0a(Object *obj) {
    int a;

    obj->field_07++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 0xf);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x4b;
    }
    func_801307e0(obj, a);
}

void func_801b23d0_slot04_0a(Object *obj) {
    u16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        func_801b3e68_slot04_0a(obj);
    }
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_46 = 0xc;
        obj->field_07++;
    }
}
