/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c7644_slot04_11[];

u8 get_random_table_byte_slot04_11(Object *obj);
int apply_velocity_and_acceleration_slot04_11(Object *obj);
void latch_pressed_bits_into_input_bits_slot04_11(Object *obj);

void update_airborne_land_choose_sequence_slot04_11(Object *obj) {
    Config *c;

    latch_pressed_bits_into_input_bits_slot04_11(obj);
    apply_velocity_and_acceleration_slot04_11(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        c = game_state.config;
        if ((c->mode_flag_b | c->mode_flag_c | c->mode_flag_a) == 0 && (u8)is_other_object_state_0_and_1_six_one(obj) == 0) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x4c);
        } else {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x6f);
        }
    } else {
        update_sequence(obj);
    }
}

void update_choose_jump_direction_by_input_slot04_11(Object *o) {
    u16 a;
    u8 t = 1;
    s32 d;

    latch_pressed_bits_into_input_bits_slot04_11(o);
    if ((o->step_flags << 16) < 0) {
        a = *(u16 *)&((Slot04bObj *)o)->input_bits;
        o->active_state = t;
        o->facing_side ^= 1;
        if (o->mode_flag_2 != 0) {
            a = get_random_table_byte_slot04_11(o);
        }
        if (a != 0) {
            o->facing_side = t;
            if ((a & 0x40) == 0) {
                o->facing_side = 0;
                if ((a & 8) == 0) {
                    o->state_3 += 3;
                    *(u16 *)&((Slot04bObj *)o)->input_bits = 0;
                    ref_other.p = o->other;
                    o->vel_x = (*(s32 *)&ref_other.p->pos_x_frac - *(s32 *)&o->pos_x_frac) / 26;
                    o->accel_x = 0;
                    o->vel_y = 0xa0000;
                    o->accel_y = -0x6000;
                    d = (u32)~o->vel_x >> 31;
                    o->facing_side = d;
                    set_sequence_by_side_and_mode(o, 0x4f);
                    return;
                }
            }
        }
        d = 0x90000;
        *(u16 *)&((Slot04bObj *)o)->input_bits = 0;
        o->state_3++;
        if (o->facing_side != 0) {
            d = -0x90000;
        }
        o->vel_y = 0xa0000;
        o->vel_x = d;
        o->accel_x = 0;
        o->accel_y = -0x6000;
        set_sequence_by_side_and_mode(o, 0x4d);
    } else {
        update_sequence(o);
    }
}

void update_airborne_land_or_wall_contact_slot04_11(Object *obj) {
    latch_pressed_bits_into_input_bits_slot04_11(obj);
    if (apply_velocity_and_acceleration_slot04_11(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->state_3 = 0xa;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x6f);
    } else if (obj->ground_y + 0x48 >= obj->pos_y && (data_801c7644_slot04_11[obj->facing_side] & obj->bound_push_flag) != 0) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->vel_y = 0;
        obj->accel_y = 0;
        set_sequence_by_side_and_mode(obj, 0x4e);
    } else {
        update_sequence(obj);
    }
}
