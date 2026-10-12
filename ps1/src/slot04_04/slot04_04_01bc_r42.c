/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3a40_slot04_04(Object *obj);
void update_sequence_or_call_finish_on_flag_3_slot04_04(Object *obj);

void func_801b386c_slot04_04(Object *obj) {
    if (func_801b3a40_slot04_04(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 2;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        set_sequence_by_side_and_mode(obj, 0x1a);
    } else {
        if (((Slot04bObj *)obj)->mode != 0) {
            obj->state_3 = obj->state_3 + 1;
            play_side_sound_at(obj, obj->side, 0xc);
            set_other_hit_reaction_state_0(obj, 4, 0xa, 0x12, 0, 0, 1);
        }
        update_sequence(obj);
    }
}

void update_air_move_land_or_sequence_slot04_04(Object *obj) {
    if (func_801b3a40_slot04_04(obj) < 0 && obj->pos_y > obj->ground_y) {
        func_80131638(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3994_slot04_04(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    play_side_sound_at(obj, obj->side, 0xc);
    set_other_hit_reaction_state_0(obj, 4, 0xa, 0xf, 0, 0, 1);
    update_sequence_or_call_finish_on_flag_3_slot04_04(obj);
}
