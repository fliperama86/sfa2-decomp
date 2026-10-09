/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b17d0_slot04_03(Object *obj);
void func_801b1834_slot04_03(Object *obj);

void func_801b1790_slot04_03(Object *obj) {
    if (obj->field_12c == 0) {
        func_801b17d0_slot04_03(obj);
    } else {
        func_801b1834_slot04_03(obj);
    }
}

void func_801b17d0_slot04_03(Object *obj) {
    obj->field_12c++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x3b);
}
