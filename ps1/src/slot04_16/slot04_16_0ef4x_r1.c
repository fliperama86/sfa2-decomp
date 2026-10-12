/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"

void play_side_sound_at(Object *o, int b, int c);
void update_sequence(Object *object);

/* functions of other units of this module */

void func_801b0ef4_slot04_16(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags < 0 && *(u8 *)&((Slot04bObj *)obj)->saved_pos_x_frac == 3 && (s16)obj->countdown < 0x1e) {
        play_side_sound_at(obj, obj->side, 0xe);
        ((Slot04bObj *)obj)->saved_pos_x_frac = 0;
    }
    t = obj->countdown;
    if (t == 0) {
        update_sequence(obj);
    } else {
        t -= 1;
        obj->countdown = t;
        if (t != 0) {
            update_sequence(obj);
        } else {
            game_state.config->side_bits |= 1 << obj->side;
            update_sequence(obj);
        }
    }
}
