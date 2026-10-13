/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_sequence_of_fourth_hud_object_9_slot28(Object *obj, int arg);

void advance_pass_and_play_side_sound_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    screen_state->pass_index += 1;
    play_side_sound_at(0, obj->side, 0x14);
    set_sequence_of_fourth_hud_object_9_slot28(o, 0);
}
