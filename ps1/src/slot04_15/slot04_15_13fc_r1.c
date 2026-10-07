/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b13fc_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 4);
    func_80138ae8(&game_state, o);
    obj->field_1ce = 0;
    obj->field_1cf = 0;
    func_801307e0(o, 0x27);
}
