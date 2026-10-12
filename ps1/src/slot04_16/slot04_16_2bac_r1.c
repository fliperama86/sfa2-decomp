/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ca1d8_slot04_16[];
extern ObjectRef data_80190468;

void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void choose_random_action_values_slot04_16(Object *obj, int arg);
void update_walk_by_input_or_random_slot04_16(Object *obj);

void start_backward_move_sequence_slow_slot04_16(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    ((Slot04bObj *)obj)->field_298 = 4;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged((GameState *)data_80190468.p, obj);
    obj->vel_x = 0xfffe8000;
    a = 0x47;
    if (obj->field_49 != 0) {
        a = 0x51;
    }
    set_sequence_by_side_and_mode(obj, a);
    if (obj->mode_flag_2 != 0) {
        choose_random_action_values_slot04_16(obj, 0x68);
    }
}

void func_801b2c40_slot04_16(Object *obj) {
    data_801ca1d8_slot04_16[obj->state_3](obj);
}

void start_backward_move_sequence_fast_slot04_16(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    ((Slot04bObj *)obj)->field_298 = 4;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged((GameState *)data_80190468.p, obj);
    a = 0x48;
    obj->vel_x = 0xffff0000;
    if (obj->field_49 != 0) {
        a = 0x52;
    }
    set_sequence_by_side_and_mode(obj, a);
    if (obj->mode_flag_2 != 0) {
        choose_random_action_values_slot04_16(obj, 0x98);
    }
}

void choose_random_action_values_slot04_16(Object *obj, int arg) {
    u8 a[4] = { 1, 1, 0, 0xff };
    u8 b[4] = { 0, 4, 8, 0x10 };
    u8 c[4] = { 8, 0xc, 0x10, 0x20 };

    ((Slot04bObj *)obj)->random_value_a = 0;
    ((Slot04bObj *)obj)->field_1aa = 0;
    ((Slot04bObj *)obj)->random_countdown = 0;
    if (get_random_byte() & 3) {
        if (obj->other->link_flags != 0) {
            ((Slot04bObj *)obj)->random_value_a = 1;
            ((Slot04bObj *)obj)->field_1aa = 0;
            ((Slot04bObj *)obj)->random_countdown = 0x46;
        } else if ((s16)obj->compare_limit <= (s16)arg) {
            ((Slot04bObj *)obj)->random_value_a = 1;
            ((Slot04bObj *)obj)->field_1aa = 0;
            ((Slot04bObj *)obj)->random_countdown = 0x46;
        } else {
            ((Slot04bObj *)obj)->random_value_a = a[get_random_byte() & 3];
            ((Slot04bObj *)obj)->field_1aa = b[get_random_byte() & 3];
            ((Slot04bObj *)obj)->random_countdown = c[get_random_byte() & 3];
        }
    }
}

void advance_state_3_on_step_flag_byte_slot04_16(Object *obj) {
    u16 t = obj->step_flags;

    if ((u8)t == 0) {
        update_sequence(obj);
    } else {
        obj->step_flags = t & 0xff00;
        obj->state_3 = obj->state_3 + 1;
        update_walk_by_input_or_random_slot04_16(obj);
    }
}
