/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern Object *data_80188f20;
extern s16 table_8017736c[];
extern s16 table_80177390[];
extern s16 table_80197ef8[];

/*
 * What it does (inferred, not an original name): `object` is the object
 * that triggered a hit, `other` the object that was hit, `g` the Box32 record that the callers pass (inferred; the role is not known).
 * If the pool of spawnable objects (data_801a27d4, a byte counter) is not 0
 * and the pool hands out an object, it fills that object in as a hit effect
 * (kind 4, copies of fields of object and other, a link back to other,
 * a rectangle size, a draw hook, and a position at the centre of the
 * rectangle data_80188ed0) and counts the pool down. If other's hit_type
 * has bit 0x80 the effect's variant_index is 1. Otherwise, when g->effect_param has
 * bit 0x80 and the pool is still not empty, a second effect is made the
 * same way, without the position and without masking table_index. Afterwards,
 * if other's hit_type has bit 0x80 the function starts an animation on
 * other (play_sound_at, or play_side_sound_at when other's kind is 6) from the
 * tables at 0x80177390; else, unless g->table_index has bit 0x80, it starts
 * one chosen by an index from g->table_index (doubled, with bit 0 set when
 * other's health is negative): for an index below 16 one animation
 * (play_sound_at with other's side and table_8017736c[index]), otherwise
 * two (play_sound_at with other's side and 0x32e, then play_side_sound_at with
 * the other side value, side ^ 1, and table_8017736c[index]); then a
 * follow-up step from a table by other's sound_side (stop_sep_by_index), left
 * out when the table entry is 3.
 */
void spawn_hit_effects_and_play_sounds(Object *object, Object *other, Box32 *g)
{
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    /* Without it this function differs from the original in 22 instruction slots. */
    u8 unused[8];
    s16 idx;
    u8 k;
    s16 v;
    int x0;

    data_80188f20 = ref_other.p;
    if ((data_801a27d4 != 0) && ((ref_other.p = (Object *)pop_block_from_pool()) != 0)) {
        ref_other.p->active_count++;
        ref_other.p->field_02 = 4;
        ref_other.p->team_id = object->team_id;
        ref_other.p->facing_side = other->facing_flag;
        ref_other.p->field_0e = other->field_0e;
        ref_other.p->parent = other;
        ref_other.p->table_index = g->effect_param & 0x7f;
        other->field_17f = g->effect_param & 0x7f;
        ref_other.p->base_x = 0x240;
        ref_other.p->base_y = 0;
        ref_other.p->spawn_x = 0x60;
        ref_other.p->spawn_y = 0x1e0;
        ref_other.p->field_90 = (void *)0x800fb100;
        data_801a27d4--;
        /* out_40 of the rectangle data_80188ed0 (offset 0x40, inside that record) is read through a cast; the plain form data_80188ed0.out_40 differs from the original in 38 instruction slots. */
        x0 = *(s16 *)((char *)&data_80188ed0 + 0x40);
        /* out_3c of the rectangle data_80188ed0 (offset 0x3c, inside that record) is read through a cast; the plain form data_80188ed0.out_3c differs from the original in 36 instruction slots. */
        /* Stand-in, not recovered source: the assignment to idx inside the store below, whose value nothing reads, reproduces the original's choice of registers. Without it this function differs from the original in 36 instruction slots. */
        ref_other.p->pos_x = (idx = (((*(s16 *)((char *)&data_80188ed0 + 0x3c)) - x0) >> 1) + x0);
        /* Stand-in, not recovered source: the assignment to idx inside the store below, whose value nothing reads, reproduces the original's choice of registers. Without it this function differs from the original in 2 instruction slots. */
        ref_other.p->pos_y = (idx = ((((s16)data_80188ed0.out_44) - ((s16)data_80188ed0.out_48)) >> 1) + ((s16)data_80188ed0.out_48));
        ref_other.p->variant_index = 0;
        if (other->hit_type & 0x80) {
            ref_other.p->variant_index = 1;
        } else if (((g->effect_param & 0x80) && (data_801a27d4 != 0)) && ((ref_other.p = (Object *)pop_block_from_pool()) != 0)) {
            ref_other.p->active_count++;
            ref_other.p->field_02 = 4;
            ref_other.p->table_index = g->effect_param;
            ref_other.p->team_id = object->team_id;
            ref_other.p->facing_side = other->facing_flag;
            ref_other.p->field_0e = other->field_0e;
            ref_other.p->parent = other;
            ref_other.p->variant_index = 0;
            ref_other.p->base_x = 0x240;
            ref_other.p->base_y = 0;
            ref_other.p->spawn_x = 0x60;
            ref_other.p->spawn_y = 0x1e0;
            data_801a27d4--;
            ref_other.p->field_90 = (void *)0x800fb100;
        }
    }
    ref_other.p = data_80188f20;
    if (other->hit_type & 0x80) {
        if (other->kind != 6) {
            play_sound_at(other, other->sound_side, table_80177390[other->kind]);
        } else {
            play_side_sound_at(other, other->sound_side, table_80177390[6]);
        }
    } else if (!(g->table_index & 0x80)) {
        idx = g->table_index * 2;
        if (((s16)other->health) < 0) {
            idx |= 1;
        }
        if (idx >= 16) {
            play_sound_at(other, other->side, 0x32e);
            play_side_sound_at(other, other->side ^ 1, table_8017736c[idx]);
        } else {
            play_sound_at(other, other->side, table_8017736c[idx]);
        }
        k = other->sound_side;
        v = table_80197ef8[k];
        if (v != 3) {
            /* The second argument is the halfword v, passed whole: the original does not mask it here; the callee takes its low byte. */
            stop_sep_by_index(k, v);
        }
    }
}
