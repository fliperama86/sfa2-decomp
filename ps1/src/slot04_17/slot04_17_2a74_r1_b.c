/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;
extern ObjectFn data_801ce49c_slot04_17[];

void update_meter_by_mode_or_table(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void spawn_three_child_objects_at_object_spawn(Object *object);
void call_reset_object_to_state_1_0_sequence_0_slot04_17(Object *obj);
void spawn_game_state_object_when_step_limit_clear_slot04_17(Object *obj);

void move_x_decelerating_then_update_or_end_slot04_17(Object *obj) {
    int a = obj->vel_x;

    if (obj->facing_side == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s32)obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        call_reset_object_to_state_1_0_sequence_0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void spawn_game_state_object_when_step_limit_clear_slot04_17(Object *obj) {
    obj->field_167 = (obj->direction_index >> 1) + 0xc;
    if (obj->step_limit == 0) {
        obj->step_limit = 0xff;
        data_80190468.p->field_6b = 5;
        spawn_game_state_kind_object(obj);
    }
}

void call_table_function_by_state_3_7_slot04_17(Object *obj) {
    data_801ce49c_slot04_17[obj->state_3](obj);
}

void advance_state_and_set_fixed_sequence_slot04_17(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    set_sequence_by_side_and_mode(obj, 0x40);
}
