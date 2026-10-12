/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1458_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    obj->countdown_2 = 0;
    o->active_flag_4 = 1;
    o->state_3++;
    if (o->field_49 != 0) {
        o->field_225 = 1;
    }
    func_80141f28(o, 5);
    func_80138ae8(&game_state, o);
    func_801307e0(o, 0x1e);
}

void func_801b14c8_slot04_06(Object *obj) {
    obj->vel_x = 0x40000;
    obj->vel_y = 0x28000;
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->accel_x = 0;
    obj->accel_y = -0xa000;
    obj->active_state = 1;
    obj->state_3++;
}
