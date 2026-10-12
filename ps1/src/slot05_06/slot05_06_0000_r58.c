/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd53c_slot05_06[];

void count_down_timer_set_side_bits_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->timer != 0) {
        ((Slot04aObj *)obj)->timer--;
        if (((Slot04aObj *)obj)->timer == 0) {
            game_state_second.side_bits = game_state.side_bits_2 | (1 << obj->side);
        }
    }
    update_sequence(obj);
}

void call_handler_by_state_2_2_slot05_06(Object *obj) {
    data_801dd53c_slot05_06[obj->state_2](obj);
}

void restore_facing_and_set_sequence_0_2_slot05_06(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_when_timer_zero_slot05_06(Object *object) {
    if (game_state.countdown_timer == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801cd098_slot05_06(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->state_2 = o->state_2 + 1;
    if (game_state_second.table_index == 0) {
        arg = 0x28;
    }
    set_sequence_by_side(o, arg);
}
