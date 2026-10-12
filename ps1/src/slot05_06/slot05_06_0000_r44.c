/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc84c_slot05_06(Object *object);

void func_801cbb6c_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_801cc84c_slot05_06(o);
    if (o->pos_y >= o->ground_y) {
        o->state_3++;
        func_80130af0(o);
        o->round_count = 0;
        o->field_69 = 0;
        o->active_state = 0;
        o->done_flag = 0;
        o->once_flag_2 = 0;
        o->flag_25a_set = 0;
        set_side_flag(o);
        o->pos_y = (u16)o->ground_y;
        obj->field_1c2 = 0xa;
        game_state.state_param = 0xa;
        play_side_sound_at(o, o->side, 0xa);
    } else {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        o->vel_x += o->accel_x;
    }
}

void func_801cbc30_slot05_06(Object *obj) {
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->facing_side = obj->saved_facing;
    func_801307e0(obj, 0x31);
}

void func_801cbc9c_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        obj->step_index = 0x1e;
        o->active_flag_3 = 1;
        o->state_3++;
        func_801307e0(o, 0x32);
    }
}
