/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001e2ec_slot28(Object *obj, int arg);

void advance_pass_and_play_side_sound_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    data_8018f5a0->pass_index += 1;
    play_side_sound_at(0, obj->side, 0x14);
    func_8001e2ec_slot28(o, 0);
}
