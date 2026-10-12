/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_vertical_velocity_slot04_06(Object *object);

void update_fall_landing_set_side_flag_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    apply_vertical_velocity_slot04_06(o);
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

void func_801b3c34_slot04_06(Object *obj) {
    obj->substate_index = 0;
    obj->field_12d = 0;
    obj->step_count_2 = 0;
    obj->step_limit = 0;
    obj->state_3++;
    add_to_meter_clamped(obj, 7);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side_and_mode(obj, 0x31);
}

void wait_step_flag_then_set_step_index_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        obj->step_index = 0x1e;
        o->active_flag_3 = 1;
        o->state_3++;
        set_sequence_by_side_and_mode(o, 0x32);
    }
}
