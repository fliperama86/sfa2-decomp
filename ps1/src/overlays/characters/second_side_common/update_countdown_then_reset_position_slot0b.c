/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_fx_fy_with_steps_slot0b(Slot0bObj *obj);
void call_step_on_anim_member_slot0b(Slot0bObj *obj);

void update_countdown_then_reset_position_slot0b(Slot0bObj *obj) {
    int v;
    if (ref_game_state.p->phase_state == 1 || (v = obj->countdown_timer - 1, obj->countdown_timer = v, (s16)v < 0)) {
        obj->step_count++;
        if (obj->entry_index == 0) {
            play_sound_at(0, 0, 0x32d);
        }
        ((Object *)obj)->pos_x = obj->field_5c;
        ((Object *)obj)->pos_y = obj->field_5e;
        call_step_on_anim_member_slot0b(obj);
    } else {
        update_fx_fy_with_steps_slot0b(obj);
    }
}
