/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1550_slot04_03(Object *obj) {
    int a = 0x22;

    obj->active_flag_4 = 1;
    obj->field_225 = 1;
    obj->state_3++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    play_side_sound_at(obj, obj->side, 4);
    play_side_sound_at(obj, obj->side, 0xc);
    if (obj->field_49 != 0) {
        a = 0x43;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + a);
}
