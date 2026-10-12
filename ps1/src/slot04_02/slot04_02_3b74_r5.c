/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void hit_other_and_spawn_if_health_negative_slot04_02(Object *obj) {
    Object *p = obj->other;

    p->field_15b = 1;
    set_other_hit_reaction_state_8(obj, 0, 3, 0x14, 0x1c, 0, 0);
    if ((s16)p->health < 0) {
        obj->field_167 = 0xf;
        game_state.spawn_kind = 6;
        spawn_game_state_kind_object(obj);
    }
}
