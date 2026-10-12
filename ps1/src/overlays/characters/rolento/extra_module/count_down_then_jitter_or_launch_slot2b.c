/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern Object *data_8007ef34_slot2b;

void set_sequence_from_table_index_entry_slot2b(Object *obj);
void set_velocity_accel_from_table_slot2b(Object *obj);
void place_object_at_target_box_offset_slot2b(Object *obj);

void count_down_then_jitter_or_launch_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    int d;
    obj->countdown_timer--;
    if ((obj->countdown_timer & 0x80) != 0) {
        o->state_1++;
        if (data_8007ef34_slot2b->other->field_15b != 0) {
            d = (get_random_byte() & 0x3f) - 0x20;
            d += *(u16 *)&o->pos_x;
            o->pos_x = d;
            d = (get_random_byte() & 0x3f) - 0x20;
            *(u16 *)&o->pos_y -= d;
            o->variant_index = (game_state.field_24 & 1) + 0x51;
            set_sequence_from_table_index_entry_slot2b(o);
            play_side_sound_at(o, data_8007ef34_slot2b->side, 0xf);
        } else {
            o->state_1++;
            obj->countdown_timer = 0x18;
            set_velocity_accel_from_table_slot2b(o);
        }
    } else {
        place_object_at_target_box_offset_slot2b(o);
        advance_sequence_checked(o);
    }
}

void advance_state_or_sequence_on_flag_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0++;
    } else {
        advance_sequence_checked(obj);
    }
}
