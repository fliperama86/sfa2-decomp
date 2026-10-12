/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void save_counter_on_first_press_slot04_04(Object *obj);

void update_countdown_set_cursor_timer_slot04_04(Object *obj) {
    int a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown + 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        } else {
            a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    push_other_by_vel_x(obj);
    update_sequence(obj);
}

void count_down_then_reset_states_slot04_04(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->counter++;
    update_sequence(o);
    o->countdown = t = o->countdown - 1;
    if ((t & 0xff) != 0) {
        save_counter_on_first_press_slot04_04(o);
    } else {
        if (o->active_state != 0) {
            o->state_0 = 1;
        }
        o->state_2 = 3;
        o->state_1 = 0;
        o->state_3 = 1;
        func_80142c70(o);
    }
}

void save_counter_on_first_press_slot04_04(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->pressed_bits != 0) {
        if (obj->saved_count == 0) {
            obj->saved_count = obj->counter;
        }
    }
    push_other_by_vel_x(o);
}

void clear_slots_81_bytes_slot04_04(Object *object) {
    u8 *p = (u8 *)object->slots;
    int i;
    u8 z = 0;

    for (i = 0x50; i >= 0; i--) {
        *p++ = z;
    }
}
