/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

int save_memory_card_file_slot0f(int a, s16 *p);
void update_two_option_selection_slot0f(Object *o);
void init_save_buffer_from_defaults_slot0f(Object *o);

void confirm_two_option_start_countdown_or_back_slot0f(Object *o) {
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
                o->field_01 = 1;
                o->state_0 = 0;
            }
            play_sound_at(0, 0, 0x205);
        } else {
            o->field_01 = 1;
            o->state_0 = 0;
            play_sound_at(0, 0, 0x202);
        }
    }
}

void save_card_file_when_countdown_ends_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    s8 t;
    if (obj->countdown != 0) {
        obj->countdown = obj->countdown - 1;
    } else {
        init_save_buffer_from_defaults_slot0f(o);
        if (obj->field_0b == 0) {
            o->field_0e = save_memory_card_file_slot0f(0, &o->pos_x);
        } else {
            o->field_0e = save_memory_card_file_slot0f(0x10, &o->pos_x);
        }
        t = -0x4c;
        o->field_0f = t;
        o->field_01 += 1;
        o->state_0 += 1;
    }
}
