/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u32 data_801c711c_slot04_11[];
extern s32 data_801c74b0_slot04_11[];

void choose_jump_variant_at_sequence_end_slot04_11(Object *obj) {
    int s;
    s16 w;
    int d;
    int vx;
    int vy;
    int vz;
    int vw;
    int q;
    /* Stand-in, not recovered source: the local k, which holds the constant 0x2000 for one test, and the local q, which keeps the masked value apart from its 16-bit copy w, reproduce the original's choice of registers. Without it this function differs from the original in 3 instruction slots. */
    /* One stand-in of two locals: with either taken out alone (the test written with the literal, or w set from the mask directly) the figure is 3 as well. Inferred: with the plain forms the compiler tests the unmasked value. */
    int k = 0x2000;

    if ((obj->step_flags << 16) < 0) {
        if (obj->mode_flag_2 != 0) {
            if (test_random_bit_in_table(obj, data_801c711c_slot04_11) != 0) {
                goto act_b;
            }
            goto act_c;
        } else {
            s = obj->input_bits_2;
            if ((s & 0x1000) == 0) {
                goto act_c;
            }
        }
act_b:
        d = obj->vel_x >> 2;
        obj->state_3 = 7;
        obj->vel_y = 0x10000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
        obj->vel_x = d;
        set_sequence_by_side_and_mode(obj, 0x34);
        return;
act_c:
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 6);
        if (obj->mode_flag_2 != 0) {
            ref_other.p = obj->other;
            d = ref_other.p->pos_x - obj->pos_x;
            s = -0x8000;
            if (d >= 0) {
                s = 0x2000;
            }
            if (obj->facing_side != 0) {
                s ^= 0xa000;
            }
            d += 0x10;
            if (d < 0x21) {
                s = 0;
            }
        }
        d = 1;
        vx = 0;
        vz = 0;
        vy = -0x8000;
        q = s & -0x6000;
        w = q;
        vw = -0x8000;
        if (w != 0) {
            d = 0;
            vx = 0x28000;
            vz = 0;
            vy = -0x8000;
            vw = -0x6000;
            if ((w & data_801c74b0_slot04_11[obj->facing_side]) == 0) {
                vx = -0x28000;
            }
            if ((q & k) != 0) {
                obj->facing_side ^= 1;
            }
        }
        obj->variant_index = d;
        d += 0x2e;
        if (obj->field_49 != 0) {
            vx <<= 1;
            vy <<= 1;
        }
        obj->vel_x = vx;
        obj->accel_x = vz;
        obj->vel_y = vy;
        obj->accel_y = vw;
        set_sequence_by_side_and_mode(obj, d + obj->direction_index);
    } else {
        update_sequence(obj);
    }
}
