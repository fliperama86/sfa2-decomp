/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7518_slot04_11[];
extern s16 data_801c7520_slot04_11[];

int apply_damage_to_other_return_defeated(Object *object, int a, int b);
void func_801b37f8_slot04_11(Object *obj);

void update_slide_and_apply_hit_steps_slot04_11(Object *obj) {
    int a;
    int i;
    int j;
    int y;
    u16 t;

    a = obj->vel_x;
    if (obj->facing_side == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s32)obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    t = obj->step_flags;
    a = t & 0xff;
    if (a != 0) {
        if (a & 0x80) {
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            /* Without it (plain braces) this function differs from the original in 32 instruction slots. */
            do {
                obj->state_3++;
                ref_other.p = obj->other;
                ref_other.p->round_count++;
                spawn_effect_at_other_offset(obj, 6, 4, 0x3c);
                ref_other.p = obj->other;
                ref_other.p->field_15b = 1;
                i = obj->direction_index;
                set_other_hit_reaction_state_0(obj, 0x10, 5, data_801c7518_slot04_11[i], data_801c7518_slot04_11[i + 1], 0, 0);
            } while (0);
            ref_other.p = obj->other;
            if ((s16)ref_other.p->health < 0) {
                play_sound_at(ref_other.p, ref_other.p->side, 0x339);
                func_801b37f8_slot04_11(obj);
            } else {
                play_sound_at(ref_other.p, ref_other.p->side, 0x306);
            }
            return;
        }
        a -= 8;
        obj->step_flags = t >> 8;
        ref_other.p = obj->other;
        ref_other.p->round_count++;
        j = a >> 1;
        y = data_801c7520_slot04_11[j + 3];
        /* Stand-in, not recovered source: the local i, the index of the other arm, used here for the table's third halfword, reproduces the original's choice of registers. Without it this function differs from the original in 13 instruction slots. */
        i = data_801c7520_slot04_11[j + 2];
        spawn_effect_at_other_offset(obj, 6, data_801c7520_slot04_11[j], data_801c7520_slot04_11[j + 1]);
        ref_other.p = obj->other;
        if (apply_damage_to_other_return_defeated(obj, i, y) != 0) {
            play_sound_at(ref_other.p, ref_other.p->side, 0x339);
            func_801b37f8_slot04_11(obj);
        } else {
            play_sound_at(ref_other.p, ref_other.p->side, 0x306);
        }
    }
    update_sequence(obj);
}
