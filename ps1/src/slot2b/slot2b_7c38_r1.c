/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007994c_slot2b[];

void call_handler_by_state_2_slot2b(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_8007994c_slot2b[o->state_2](o);
    }
    func_8011ffdc(o);
}

void func_80077ca4_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->countdown_timer = 2;
    o->state_2++;
    o->facing_side ^= 1;
    o->active_flag = 0;
    call_kind_9_handler_for_player(o);
    func_80138070(o, (o->vel_table_index >> 1) + 6);
}

void countdown_reset_states_flip_vel_x_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->countdown_timer--;
    if (s->countdown_timer == 0) {
        o->active_count = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->vel_y = 0;
        o->vel_x = -o->vel_x;
    }
    advance_sequence_checked(o);
}
