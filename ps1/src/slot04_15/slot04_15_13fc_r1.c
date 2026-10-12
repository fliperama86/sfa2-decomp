/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b13fc_slot04_15(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->active_flag_4 = 1;
    o->state_3++;
    func_80141f28(o, 4);
    func_80138ae8(&game_state, o);
    obj->active_flag_2 = 0;
    obj->table_index_3 = 0;
    func_801307e0(o, 0x27);
}
