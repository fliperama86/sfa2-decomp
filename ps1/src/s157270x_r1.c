/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_child_countdown_follow_parent(Object *obj) {
    Object *other = obj->parent;
    u32 w;

    if (game_state.field_65 == 0) {
        int t = obj->countdown - 1;
        obj->countdown = t;
        if (t & 0x8000) {
            obj->countdown = 0;
            w = *(u32 *)&other->state_0;
            if ((w & 0xffffff) != 0x70001 && (w & 0xffff) != 0x701) {
                obj->state_0 = 2;
                return;
            }
        }
    }
    if (other->field_15a == 8 || game_state.check_flag == 0) {
        if ((s16)obj->countdown <= 0) {
            set_flag_and_push_if_mode_8(obj);
        }
    }
    if (other->field_15a != 8 || game_state.check_flag != 0) {
        obj->field_01 = 0;
        obj->state_0 = 2;
    }
}
