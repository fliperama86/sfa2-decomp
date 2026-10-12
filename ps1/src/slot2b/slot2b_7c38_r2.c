/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079954_slot2b[];
extern s32 data_8007995c_slot2b[];

void call_handler_by_state_2_2_slot2b(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_80079954_slot2b[o->state_2](o);
    }
    func_8011ffdc(o);
}

void func_80077dd4_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s8 v;
    s->countdown_timer = 2;
    o->state_2++;
    o->facing_side ^= 1;
    o->active_flag = 0;
    call_kind_9_handler_for_player(o);
    v = s->field_a0;
    if (v & 0x80) {
        advance_sequence_checked(o);
    } else {
        func_80138070(o, v);
    }
}

void func_80077e50_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->countdown_timer += 0xff;
    if (s->countdown_timer & 0x80) {
        o->active_count = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->vel_x = -o->vel_x;
        o->vel_y = data_8007995c_slot2b[o->vel_table_index >> 1];
    }
    advance_sequence_checked(o);
}
