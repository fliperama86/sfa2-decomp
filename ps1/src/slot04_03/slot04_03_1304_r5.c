/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b17d0_slot04_03(Object *obj);
void func_801b1834_slot04_03(Object *obj);

void func_801b1790_slot04_03(Object *obj) {
    if (obj->substate_index == 0) {
        func_801b17d0_slot04_03(obj);
    } else {
        func_801b1834_slot04_03(obj);
    }
}

void func_801b17d0_slot04_03(Object *obj) {
    obj->substate_index++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x3b);
}
