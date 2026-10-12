/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2584_slot04_02(Object *obj);
void func_801b252c_slot04_02(Object *obj);

void update_airborne_repeat_countdown_or_land_slot04_02(Object *obj) {

    if (func_801b2584_slot04_02(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            play_sound_at(obj, obj->side, 0x320);
            ((Slot04bObj *)obj)->countdown_2--;
            if (((Slot04bObj *)obj)->countdown_2 == 0) {
                obj->state_3++;
                set_sequence_by_side_and_mode(obj, 0x1f);
                return;
            }
        }
        update_sequence(obj);
    } else {
        func_801b252c_slot04_02(obj);
    }
}

void func_801b24b0_slot04_02(Object *obj) {
    if (func_801b2584_slot04_02(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_3++;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        func_801b252c_slot04_02(obj);
    }
}

void func_801b252c_slot04_02(Object *obj) {
    obj->state_3++;
    play_object_sound_by_kind(obj);
    obj->active_flag_3 = 0;
    obj->active_state = 0;
    obj->pos_y_frac = 0;
    obj->pos_y = ((Slot04bObj *)obj)->ground_y;
    set_sequence_by_side(obj, 0x11);
}
