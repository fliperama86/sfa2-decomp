/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2048_slot04_06(Object *obj);

void func_801b1c5c_slot04_06(Object *o) {
    ((Slot04aObj *)o)->countdown_2 = 0x32;
    o->state_3++;
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_80145d20(o);
    play_side_sound_at(o, o->side, 7);
    o->substate_index = 0;
    o->field_12d = 0;
    o->step_count_2 = 0;
    o->step_limit = 0;
    o->accel_x = -0x6000;
    func_801b2048_slot04_06(o);
    func_801307e0(o, (o->direction_index >> 1) + 0x34);
}
