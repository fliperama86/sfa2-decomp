/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern u8 data_801dd318_slot05_06[];
void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void apply_vertical_velocity_slot05_06(Object *obj);

void update_countdown_2_then_set_other_lock_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u8 i;

    if (obj->countdown_2 != 0) {
        if (obj->countdown_2 == 0x31) {
            play_sound_at(o, o->side, 0x31c);
            spawn_children_per_direction_index(o, -0x26, 0x62);
        }
        obj->countdown_2--;
        if (obj->countdown_2 == 0) {
            i = 0;
            o->field_165 = 0;
            if (o->field_4b == 0) {
                o->other->field_6b = 0xa;
                i = (o->direction_index >> 1) + 1;
            }
            o->other_lock = data_801dd318_slot05_06[i];
        }
    } else {
        if (obj->mode != 0) {
            obj->countdown_2 = 10;
            o->state_3++;
        } else {
            update_sequence(o);
        }
    }
}

void move_until_landed_or_countdown_2_ends_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    move_x_by_vel_x_facing_then_accelerate_slot05_06(o);
    apply_vertical_velocity_slot05_06(o);
    if (obj->countdown_2 != 0) {
        if (--obj->countdown_2 == 0) goto done;
    }
    if (o->pos_y < o->ground_y) return;
done:
    o->active_state = 0;
    o->other_state_code = 5;
    o->state_3++;
    play_object_sound_by_kind(o);
    update_sequence(o);
}
