/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void store_image_rows_for_object_slot04_0a(Object *object);
void load_velocities_from_table_5_slot04_0a(Object *object);

void update_sequence_then_appear_beside_other_slot04_0a(Object *obj) {
    Object *o;
    u8 b;
    u16 t = obj->step_flags;
    if (t & 0xff) {
        obj->step_flags = t & 0xff00;
        store_image_rows_for_object_slot04_0a(obj);
    }
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        o = obj->other;
        b = o->facing_side;
        obj->facing_side = b;
        if (b == 0) {
            obj->pos_x = o->pos_x + 0x28;
        } else {
            obj->pos_x = o->pos_x - 0x28;
        }
        play_side_sound_at(obj, obj->side, 0xf);
        set_sequence_by_side_and_mode(obj, 0x4a);
    }
}

void update_sequence_or_reset_on_end_flag_slot04_0a(Object *obj) {
    u16 t = obj->step_flags;
    if (t & 0xff) {
        obj->step_flags = t & 0xff00;
        load_velocities_from_table_5_slot04_0a(obj);
    }
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
