/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int format_memory_card_and_check_slot0f(int a);
void update_two_option_selection_slot0f(Object *o);

void func_800e1c70_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u16 b;
    update_two_option_selection_slot0f(o);
    b = o->pos_x_frac;
    if (b & 0x860) {
        if (b & 0x820) {
            if (obj->field_0c == 0) {
                obj->countdown = 2;
                o->field_01 += 1;
                o->state_0 += 1;
            } else {
                o->active_count = 1;
                o->field_01 = 0;
                o->ot_depth = 0;
                o->state_0 = 0;
            }
            play_sound_at(0, 0, 0x205);
        } else {
            o->active_count = 1;
            o->field_01 = 0;
            o->ot_depth = 0;
            o->state_0 = 0;
            play_sound_at(0, 0, 0x202);
        }
    }
}

void func_800e1d2c_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    s8 t;
    if (obj->countdown != 0) {
        obj->countdown = obj->countdown - 1;
    } else {
        if (obj->field_0b == 0) {
            o->field_0e = format_memory_card_and_check_slot0f(0);
        } else {
            o->field_0e = format_memory_card_and_check_slot0f(0x10);
        }
        t = -0x4c;
        o->field_0f = t;
        o->field_01 += 1;
        o->state_0 += 1;
    }
}
